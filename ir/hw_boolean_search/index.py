import struct
import simple9
from cityhash import CityHash64
import collections
import os
import gzip
import re
# from nltk.corpus import stopwords
# from pymystem3 import Mystem
from tqdm import tqdm
import gzip
import pymorphy2


# mystem = Mystem()
# stop_list = stopwords.words("russian")
with open("stopwords.txt", "r") as f:
    stop_list = f.read().split(" ")
morph = pymorphy2.MorphAnalyzer()


def get_tokens(doc):
    tokens = " ".join(re.findall(r"\w+", doc))
    # cleaned_tokens = [token.lower() for token in mystem.lemmatize(tokens)\
    #                  if token not in stop_list and len(token) > 1]
    cleaned_tokens = []
    for token in tokens.split():
        # print(token)
        if token not in stop_list and len(token) > 1:
            # print("here")
            # print(morph.parse(token.lower())[0].normal_form)
            cleaned_tokens.append(morph.parse(token.lower())[0].normal_form)
    # cleaned_tokens = [morph.parse(token.lower()) for token in tokens \
    #                   if token not in stop_list and len(token) > 1]
    return list(set(cleaned_tokens))


def parse_and_transform_docs(dirname):
    data = []
    for filename in os.listdir(dirname)[:1]:
        print(filename)
        with gzip.open(dirname + filename) as f:
            data.append(f.read().decode("utf-8", errors="ignore"))

    data = " ".join(data)
    data = re.sub("\x1a", " ", data)
    splitted_data = re.split("(?P<url>https?://[^\s]+)", data)[1:]

    urls = splitted_data[::2]
    docs = splitted_data[1::2]

    urls_docs = [(urls[i], docs[i]) for i in range(len(docs))]
    urls_docs.sort(key=lambda x: x[0])

    urls = [item[0] for item in urls_docs]
    docs = [item[1] for item in urls_docs]

    docids_terms = []
    for doc in tqdm(docs, total=len(docs)):
        docids_terms.append(get_tokens(doc))

    return docids_terms, urls


def invert_index(docids_terms):
    terms_docids = collections.OrderedDict()

    for docid, terms in enumerate(docids_terms):
        for term in terms:

            if term not in terms_docids:
                terms_docids[term] = [docid]
            else:
                terms_docids[term].append(docid)
    return terms_docids


def diff_docids(terms_docids):
    terms_docids_diff = collections.OrderedDict()
    for term in terms_docids:
        docids_diff = [terms_docids[term][0]]

        i = 1
        while i < len(terms_docids[term]):
            docids_diff.append(terms_docids[term][i] - terms_docids[term][i - 1])
            i += 1

        terms_docids_diff[term] = docids_diff

    return terms_docids_diff


def compress_index(terms_docids_diff, index_filename="index"):
    terms_docids_diff_encoded = collections.OrderedDict()
    for term in terms_docids_diff:
        terms_docids_diff_encoded[term] = simple9.encode(terms_docids_diff[term])

    terms_coords = []
    for ind, item in enumerate(terms_docids_diff_encoded.items()):
        if ind == 0:
            offset = 0
        else:
            offset = terms_coords[ind - 1][1] + terms_coords[ind - 1][2]

        terms_coords.append((item[0], offset, len(item[1])))

    index = b"".join(terms_docids_diff_encoded.values())

    with open(index_filename, 'wb') as f:
        f.write(index)

    return terms_coords


def find_optim_backets(hashed_terms_coords):
    max_backets = len(hashed_terms_coords)
    min_backets = 1

    while max_backets - min_backets > 1:
        medium = (max_backets + min_backets) // 2

        backets = [[] for _ in range(medium)]
        for item in hashed_terms_coords:
            backets[item[0] % medium].append(item)

        max_size = 0
        for backet in backets:
            max_size = max(max_size, len(backet) * 16)

        if max_size > 4096:
            min_backets = medium
        else:
            max_backets = medium

    return max_backets


def serialize_dict(terms_coords, dict_filename="dict"):
    hashed_terms_coords = []
    for item in terms_coords:
        hashed_terms_coords.append((CityHash64(item[0]), item[1], item[2]))

    n_backets = find_optim_backets(hashed_terms_coords)

    backets = [[] for _ in range(n_backets)]
    for item in hashed_terms_coords:
        backets[item[0] % n_backets].append(item)

    for backet in backets:
        backet.sort(key=lambda x: x[0])

    flat_backets = []
    for backet in backets:
        flat_backets += [i for item in backet for i in item]

    backet_offsets = [0]
    for backet in backets:
        offset = backet_offsets[-1] + len(backet) * 16
        backet_offsets.append(offset)

    with open(dict_filename, "wb") as f:
        f.write(struct.pack("i", n_backets))
        f.write(struct.pack("i" * len(backet_offsets), *backet_offsets))
        f.write(struct.pack("QII" * len(hashed_terms_coords), *flat_backets))


def index():
    if "docs" not in os.listdir():
        print("Error! Docs not found. Please move docs to 'docs' folder.")
        return

    docids_terms, urls = parse_and_transform_docs("docs/")

    with gzip.GzipFile("urls", "w") as f:
        for url in urls:
            f.write((url + " ").encode())

    terms_docids = invert_index(docids_terms)
    terms_docids_diff = diff_docids(terms_docids)

    terms_coords = compress_index(terms_docids_diff)
    serialize_dict(terms_coords)


if __name__ == "__main__":
    index()
