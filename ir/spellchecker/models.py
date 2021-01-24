import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from collections import defaultdict


class LangModel:
    def __init__(self, queries):
        self.make_vectorizer(queries)
        self.bigrams_freqs(queries)

    def make_vectorizer(self, queries):
        vectorizer = CountVectorizer(token_pattern=r"(?u)\b\w+\b")
        vectors = vectorizer.fit_transform(queries)

        words = vectorizer.get_feature_names()
        freqs = np.array(vectors.sum(axis=0))[0]

        self.freqs_sum = freqs.sum()
        for i in range(len(words)):
            words[i] = words[i][:20]

        self.words = dict(zip(words[50:474500], freqs[50:474500]))

    def bigrams_freqs(self, queries):
        vectorizer = CountVectorizer(ngram_range=(2, 2))
        X = vectorizer.fit_transform(queries)

        names = vectorizer.get_feature_names()
        freqs = np.array(X.sum(axis=0))[0]

        bad = np.where(freqs==1)[0][::-1]
        for idx in bad:
            names.pop(idx)

        self.bigrams_freqs_sum = freqs.sum()
        freqs = freqs[np.where(freqs > 1)[0]] / self.bigrams_freqs_sum

        self.bigrams = dict(zip(names, freqs))


def language_check(string):
    chars = 'абвгдеёжзийклмнопрстуфхцчшщъыьэюяabcdefghijklmnopqrstuvwxyz0123456789'

    for char in string:
        if char not in chars:
            return False

    return True


def levenstein_dist(word1, word2):
    matrix = np.zeros((len(word1) + 1, len(word2) + 1))

    for i in range(1, matrix.shape[0]):
        matrix[i, 0] = i

    for i in range(1, matrix.shape[1]):
        matrix[0, i] = i

    for i in range(1, matrix.shape[0]):
        for j in range(1, matrix.shape[1]):
            matrix[i, j] = min(matrix[i, j-1], matrix[i-1, j], matrix[i-1, j-1])\
                           + int(word1[i-1] != word2[j-1])

    return matrix


def get_opcodes(matrix):
    if matrix.shape[0] == 1 and matrix.shape[1] == 1:
        return []

    curr = matrix[-1, -1]
    result = []
    labels = [0, 0, 0]

    if matrix.shape[0] > 1 and matrix[-2, -1] <= curr:
        result += [[matrix.shape[0] - 2, matrix.shape[1] - 1, -1]]
        labels[0] = 1

    if matrix.shape[1] > 1 and matrix[-1, -2] <= curr:
        result += [[matrix.shape[0] - 1, matrix.shape[1] - 2,  1]]
        labels[1] = 1

    if matrix.shape[0] > 1 and matrix.shape[1] > 1 and matrix[-2, -2] <= curr:
        result += [[matrix.shape[0] - 2, matrix.shape[1] - 2, 0]]
        labels[2] = 1

    if labels[2] == 1:
        result += get_opcodes(matrix[:-1, :-1])
    elif labels[0] == 1:
        result += get_opcodes(matrix[:-1, :])
    elif labels[1] == 1:
        result += get_opcodes(matrix[:, :-1])

    return result


def process_dict(main_dict, threshold=100):
    new_dict = dict()
    for key in main_dict:
        if main_dict[key] >= threshold:
            new_dict[key] = main_dict[key]

    main_dict = new_dict

    for key in main_dict:
        main_dict[key] = main_dict[key] / sum(main_dict.values())

    median = np.median(np.array([main_dict[k] for k in main_dict]))

    return main_dict, median


def get_letter_bigramms(word1, word2):
    if not language_check(word1 + word2):
        return [], [], []

    matrix = levenstein_dist(word1, word2)
    for i in range(min(matrix.shape[0], matrix.shape[1])):
        if matrix[i][i] != 0:
            i -= 1
            break

    matrix = matrix[i:, i:]

    opcodes = get_opcodes(matrix)
    for k, _ in enumerate(opcodes):
        opcodes[k] = (opcodes[k][0] + i, opcodes[k][1] + i, opcodes[k][2])

    opcodes = list(set(opcodes))

    changes = []
    inserts = []
    deletes = []

    for labels in opcodes:

        if labels[-1] == 0:
            changes.append((word2[labels[1]-1] + word1[labels[0]], word2[labels[1]-1] + word2[labels[1]]))
        if labels[-1] == 1:
            inserts.append((word2[labels[1]-1] + '*', word2[labels[1]-1] + word2[labels[1]]))
        if labels[-1] == -1:
            deletes.append((word2[labels[1]-1] + word1[labels[0]], word2[labels[1]-1] + '*'))

    change_num = 0

    while change_num < len(changes):
        if changes[change_num][0] == changes[change_num][1]:
            changes.pop(change_num)
        else:
            change_num += 1

    return changes, inserts, deletes


def get_query_changes(query_pair):
    changes = []
    inserts = []
    deletes = []

    orig = query_pair[0].split()
    fix = query_pair[1].split()

    if len(orig) == len(fix):
        for word_first, word_second in zip(orig, fix):
            if word_first != word_second:
                change_letter, insert_letter, delete_letter = get_letter_bigramms(word_first, word_second)
                changes += change_letter
                inserts += insert_letter
                deletes += delete_letter

    return changes, inserts, deletes


class SpellModel:
    def __init__(self, corpus):
        self.create_dicts(corpus)

        self.changes_dict, self.changes_med = process_dict(self.changes_dict, 40)
        self.inserts_dict, self.inserts_med = process_dict(self.inserts_dict, 100)
        self.deletes_dict, self.deletes_med = process_dict(self.deletes_dict, 100)

        self.prepare_changes()
        self.prepare_inserts()
        self.prepare_deletes()

    def create_dicts(self, corpus):
        self.changes_dict = defaultdict(int)
        self.inserts_dict = defaultdict(int)
        self.deletes_dict = defaultdict(int)

        for query_pair in corpus:
            changes, inserts, deletes = get_query_changes(query_pair)

            for change in changes:
                self.changes_dict[change] += 1

            for insert in inserts:
                self.inserts_dict[insert] += 1

            for delete in deletes:
                self.deletes_dict[delete] += 1

    def prepare_changes(self):
        changes_dict = dict()
        for key in self.changes_dict:
            if key[0][1] not in changes_dict.keys():

                changes_dict[key[0][1]] = dict()
                if key[1][1] not in changes_dict[key[0][1]].keys():
                    changes_dict[key[0][1]][key[1][1]] = dict()
                    changes_dict[key[0][1]][key[1][1]][key[0][0]] = self.changes_dict[key]

                else:
                    changes_dict[key[0][1]][key[1][1]][key[0][0]] = self.changes_dict[key]

            else:
                if key[1][1] not in changes_dict[key[0][1]].keys():
                    changes_dict[key[0][1]][key[1][1]] = dict()
                    changes_dict[key[0][1]][key[1][1]][key[0][0]] = self.changes_dict[key]

                else:
                    changes_dict[key[0][1]][key[1][1]][key[0][0]] = self.changes_dict[key]

        self.changes_dict = changes_dict

    def prepare_inserts(self):
        inserts_dict = dict()
        for key in self.inserts_dict:
            if key[1][1] not in inserts_dict.keys():

                inserts_dict[key[1][1]] = dict()
                inserts_dict[key[1][1]][key[0][0]] = self.inserts_dict[key]

            else:
                inserts_dict[key[1][1]][key[0][0]] = self.inserts_dict[key]

        self.inserts_dict = inserts_dict

    def prepare_deletes(self):
        deletes_dict = dict()
        for key in self.deletes_dict:

            if key[0][1] not in deletes_dict.keys():
                deletes_dict[key[0][1]] = dict()
                deletes_dict[key[0][1]][key[0][0]] = self.deletes_dict[key]

            else:
                deletes_dict[key[0][1]][key[0][0]] = self.deletes_dict[key]

        self.deletes_dict = deletes_dict
