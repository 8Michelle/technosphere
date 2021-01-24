import struct
import math
from cityhash import CityHash64
import gzip
import simple9
import re
from nltk.corpus import stopwords
from pymystem3 import Mystem
from collections import OrderedDict
from search_tree import QueryTree
import os
import argparse
import pymorphy2


# mystem = Mystem()
# stop_list = stopwords.words("russian")
morph = pymorphy2.MorphAnalyzer()
with open("stopwords.txt", "r") as f:
    stop_list = f.read().split(" ")

def bin_search_term(h, data):
    left = 0
    right = len(data) // 3
    while right - left > 1:
        medium = (right + left) // 2
        if data[medium * 3] > h:
            right = medium
        else:
            left = medium

    if data[left * 3] == h:
        return left * 3


def get_posting_list(term):
    with open("dict", "rb") as f:
        n_backets = struct.unpack("i", f.read(4))[0]
        h = CityHash64(term)
        backet_num = h % n_backets

        f.seek(backet_num * 4 + 4)
        offset, offset_next = struct.unpack("ii", f.read(8))

        f.seek(8 + n_backets * 4 + offset)
        data = list(struct.unpack("QII" * ((offset_next - offset) // 16), f.read(offset_next - offset)))

        index = bin_search_term(h, data)
        if index:
            block_offset = data[index + 1]
            block_size = data[index + 2]

            with open("index", "rb") as f:
                f.seek(block_offset)
                diff_posting_list = list(simple9.decode(f.read(block_size)))

            posting_list = [diff_posting_list[0]]
            for i in diff_posting_list[1:]:
                posting_list.append(posting_list[-1] + i)
            return posting_list

        else:
            return []


def get_token(token):
    # cleaned_token = mystem.lemmatize(token.lower())[0] if token not in stop_list and len(token) > 1 else None
    cleaned_token = morph.parse(token.lower())[0].normal_form if token not in stop_list and len(token) > 1 else None
    return cleaned_token


def validate_query(query):
    if "| !" in query:
        return False

    query_list = query.split(" ")
    for i, _ in enumerate(query_list[1:]):
        if query_list[i][0] == '!' and query_list[i + 1] == "|" and query_list[i - 1] != "&":
            return False

    inv = True
    for item in query.split():
        if item not in {"&", "|"} and item[0] != '!':
            inv = False
    if inv:
        return False

    return True


def normalize_query(query):
    tokens = query.split(" ")
    normalized_query = []
    for token in tokens:
        if token in {"&", "|"}:
            normalized_query.append(token)
        else:
            index = int(token[0] == "!")
            start = '!' if index else ''
            normalized_token = get_token(token[index:])
            if normalized_token:
                normalized_query.append(start + get_token(token[index:]))

    clear_normalized_query = []
    size = len(normalized_query)
    for i, token in enumerate(normalized_query):
        if (i == 0 or i == size - 1) and (normalized_query[i] == "&" or normalized_query[i] == "|"):
            continue
        if normalized_query[i] in {"&", "|"} and normalized_query[i + 1] in {"&", "|"}:
            continue
        clear_normalized_query.append(token)

    return " ".join(clear_normalized_query)


def search():
    if "index" not in os.listdir() or "dict" not in os.listdir() or "dict" not in os.listdir():
        print("Error! Some index files not found. Please launch 'index.sh'")
        return

    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--dmp', nargs='+')
    query = parser.parse_args()
    query = ' '.join(query.dmp)

    if not validate_query(query):
        print("Error! Invalid query.")
        return

    query = query.encode(errors='replace').decode(errors='replace')

    normalized_query = normalize_query(query)
    posting_lists = {}

    for term in normalized_query.split(" "):
        if term not in {"|", "&"}:
            index = int(term[0] == '!')
            posting_lists[term[index:]] = get_posting_list(term[index:])

    tree = QueryTree(normalized_query, posting_lists)
    docids = tree.search_result()

    with gzip.GzipFile("urls", "r") as f:
        urls = f.read().decode().split(" ")

        print(len(docids))
        for docid in docids:
            print(urls[docid])


if __name__ == "__main__":
    search()
