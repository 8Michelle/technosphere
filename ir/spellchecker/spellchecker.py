import pickle
import sys
from models import SpellModel, LangModel
from trie import Trie, Node


def main():
    with open('trie.pkl', 'rb') as f:
        trie = pickle.load(f)
    for query in sys.stdin:
        print(trie(query, alpha=2))


if __name__ == "__main__":
    main()
