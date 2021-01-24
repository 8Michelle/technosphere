import pickle
from trie import Trie
from models import SpellModel, LangModel


def get_queries(file_name='queries_all.txt'):
    queries = []
    correct = []
    with open(file_name) as f:
        for query in f:
            query = query.lower()
            separated = query.split(sep='\t')

            if len(separated) == 2:
                correct.append((separated[0], separated[1][:-1]))
                queries.append(separated[1])

            else:
                queries.append(separated[0][:-1])

    return queries, correct


def main_index():
    queries, correct = get_queries()

    spell_model = SpellModel(correct)
    lang_model = LangModel(queries)

    trie = Trie(spell_model, lang_model)
    with open('trie.pkl', 'wb') as f:
        pickle.dump(trie, f)


if __name__ == '__main__':
    main_index()
