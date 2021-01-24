import pickle
import os
from collections import defaultdict
from joblib import Parallel, delayed

PATH = "/Users/michelle/data/text-relevance-competition-ir-1-ts-fall-2020/"


def indexing(text):
    text_index = defaultdict(set)
    words = text.split()
    text_length = len(words)
    for index, word in enumerate(words):
        text_index[word].add(index)

    return text_index, text_length


def handle(file):
    print(file)
    with open(os.path.join(PATH, "parsed_content", file), "rb") as f:
        doc = pickle.load(f)

    text_index = indexing(doc[0])
    title_index = indexing(doc[1])
    meta_index = indexing(doc[2])
    headers_indices = [indexing(header) for header in doc[3]]
    # url = doc[4]

    with open(os.path.join(PATH, "index", file), "wb") as f:
        pickle.dump((
            text_index,
            title_index,
            meta_index,
            headers_indices,
            # url
        ), f)


def main():
    files = os.listdir(os.path.join(PATH, "parsed_content"))
    Parallel(n_jobs=4)(delayed(handle)(file) for file in files\
                       if file.endswith(".pkl") and\
                       file.split(".")[0].split("_")[1] != "00")


if __name__ == '__main__':
    main()
