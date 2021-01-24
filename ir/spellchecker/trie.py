from string import punctuation
import numpy as np
from itertools import product


class Node:
    def __init__(self, spell_model, level=-1, letter=None, N=10):
        self.spell_model = spell_model

        self.nodes = dict('')
        self.word = ''
        self.level = level
        self.letter = letter
        self.N = N

    def language(self, word):
        rus = dict(zip("йцукенгшщзхъфывапролджэячсмитьб", "qwertyuiop[]asdfghjkl;'zxcvbnm,"))
        eng = {eng_char: rus_char for rus_char, eng_char in rus.items()}

        keyboard = eng if word[0] in eng else rus

        for i in range(len(word)):
            if word[i] not in keyboard:
                return False, word

        for i in range(len(word)):
            word = word[:i] + keyboard[word[i]] + word[i + 1:]

        return True, word

    def add_word(self, word, position=0):
        if len(word) == position:
            self.word = word

        else:
            if word[position] not in self.nodes.keys():
                self.nodes[word[position]] = Node(self.spell_model, self.level + 1, word[position], self.N)
            self.nodes[word[position]].add_word(word, position + 1)

    def get_proba(self, word, proba, prev_letter):
        if self.level != -1 and self.letter != word[self.level]:

            if word[self.level] not in self.spell_model.changes_dict or\
                    self.letter not in self.spell_model.changes_dict[word[self.level]]:
                proba = 0
            elif self.level == 0:
                proba *= sum(self.spell_model.changes_dict[word[self.level]][self.letter].values())
            elif prev_letter not in self.spell_model.changes_dict[word[self.level]][self.letter].keys():
                proba = 0
            else:
                proba *= self.spell_model.changes_dict[word[self.level]][self.letter][prev_letter]

        return proba

    def insert_step(self, word, proba, letter, prev_letter, letter_deleted):
        result = []
        if self.level > 0 and letter in self.spell_model.inserts_dict.keys() and\
                    prev_letter in self.spell_model.inserts_dict[letter].keys() and not letter_deleted:

            word = word[:self.level + 1] + letter + word[self.level + 1:]
            proba *= self.spell_model.inserts_dict[letter][prev_letter] / 3
            result += self.nodes[letter].step(word, proba, self.letter, True)

        return result

    def step(self, word, proba=1, prev_letter='', letter_added=False, letter_deleted=False, split=False):
        result = []
        proba = self.get_proba(word, proba, prev_letter)

        if self.level == -1:
            for i in punctuation:
                if i in word:
                    return []

        if proba <= 1.0e-13:
            return []

        if self.level + 1 == len(word):
            for node_letter in self.nodes:
                result += self.insert_step(word, proba, node_letter, self.letter, letter_deleted)
            result += [(self.word, proba)] if self.word != '' else []
            return result

        else:
            for node_letter in self.nodes:
                result += self.insert_step(word, proba, node_letter, prev_letter, letter_deleted)
                result += self.nodes[node_letter].step(word, proba, self.letter)

            if not letter_added and not letter_deleted and\
                    self.level >= 0 and self.letter in self.spell_model.deletes_dict and prev_letter in \
                        self.spell_model.deletes_dict[self.letter]:
                delete_proba = proba * self.spell_model.deletes_dict[self.letter][prev_letter] / 3
                word_after_delete = word[:self.level] + word[self.level + 1:]
                result += self.step(word_after_delete, delete_proba, prev_letter, True, True)

        if self.level == -1:
            changed = self.language(word)

            if changed[0]:
                changed_word = changed[1]
                for node_letter in self.nodes:
                    result += self.nodes[node_letter].step(changed_word, proba * self.spell_model.changes_med)

            if len(result) == 0 and split == False:
                for split in range(1, len(word)):
                    c = 0.04 ** abs(split - len(word) / 2)

                    res_1 = self.step(word[:split], self.spell_model.changes_med * c, self.letter, split=True)
                    res_2 = self.step(word[split:], self.spell_model.changes_med * c, self.letter, split=True)

                    res_1 = res_1 if res_1 != [] else [(word[:split], 0)]
                    res_2 = res_2 if res_2 != [] else [(word[split:], 0)]

                    proba = res_1[0][1] * res_2[0][1]
                    result += [(res_1[0][0] + ' ' + res_2[0][0], proba)] if proba != 0 else []

        result.sort(key=lambda x: x[1], reverse=True)

        return result[:self.N]


class Trie:
    def __init__(self, spell_model, lang_model):
        self.spell_model = spell_model
        self.lang_model = lang_model

        self.master_node = Node(spell_model)
        for word in self.lang_model.words.keys():
            self.master_node.add_word(word)

    def get_proba(self, words, proba, alpha=1.0):
        freq = 0
        for word in words.split():
            if word not in self.lang_model.words:
                freq += 1 / self.lang_model.freqs_sum
            else:
                freq += self.lang_model.words[word] / self.lang_model.freqs_sum

        return words, alpha * np.log(freq) + np.log(proba)

    def search(self, word, top=10, alpha=1.0):
        lowered_word = word.lower()
        result = self.master_node.step(lowered_word)[:top]

        for i in range(len(result)):
            result[i] = self.get_proba(result[i][0], result[i][1], alpha)
            if result[i][0] == lowered_word:
                result[i] = (word, result[i][1])

        result.sort(key = lambda x: x[1], reverse = True)
        if result == []:
            result.append((word, 1))

        return result

    def get_query_proba(self, sample):
        proba = 0
        if len(sample) > 1:
            for i in range(len(sample) - 1):
                bigram = sample[i][0] + " " + sample[i + 1][0]

                if bigram in self.lang_model.bigrams:
                    proba += self.lang_model.bigrams[bigram]
                else:
                    proba += np.log(1 / self.lang_model.bigrams_freqs_sum)

                proba += sample[i][1]
            proba += sample[-1][1]

        else:
            proba += sample[0][1]

        return " ".join([word[0] for word in sample]), proba

    def fix_split(self, query):
        words = query.split()
        new_queries = []
        for i in range(len(words) - 1):
            new_query = (" ".join(words[:i]) + " " + words[i] + " ".join(words[i + 1:])).strip().split()
            for i in range(len(new_query)):
                new_query[i] = self.get_proba(new_query[i], 1e-10, 2)
            new_queries.append(new_query)

        return new_queries

    def __call__(self, query, alpha=1.0, top=10):
        query = query.lower()
        words = query.split()

        result = []
        for word in words:
            result.append(self.search(word, top, alpha))


        words_num = len(words)
        num = 1 if words_num > 15 else 2 if words_num > 9 else 11 - words_num

        for i in range(len(result)):
            result[i] = result[i][:num]

        samples = [list(item) for item in list(product(*result))]
        samples += self.fix_split(query)

        new_queries = [self.get_query_proba(sample) for sample in samples]
        new_queries.sort(key=lambda x: x[1], reverse=True)

        return new_queries[0][0]
