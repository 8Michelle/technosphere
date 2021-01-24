from bs4 import BeautifulSoup
from joblib import Parallel, delayed
import pickle
import os
from tqdm import tqdm
import re

PATH = "/Users/michelle/data/nn_text-relevance-competition-ir-1-ts-fall-2020/"
PUNC = '!#$%&()*+<=>?@[\\]^_`{|}~'


def parse_text(content):
    if content:
        for s in content(["style", "script", "[document]", "head", "title"]):
            s.extract()
        return content.get_text(separator=" ")


def parse_meta_info(content):
    meta_tags = content.find_all("meta", {
        'name': ["keywords", "KEYWORDS", "keywords",
                 "description", "Description", "DESCRIPTION"]
    })
    return " ".join([str(tag.get("content")) for tag in meta_tags if tag is not None])


def parse_headers(content):
    headers = []
    for i in range(1, 7):
        header_tags = content.find_all(f"h{i}")
        headers.append(" ".join([str(tag.get_text()) for tag in header_tags if tag is not None]))
    return headers


def handle_text(text):
    new_text = re.sub(r'[!#$%&()*+<=>?@\[\\\]^_`{|}~\n\t\r]', " ", text)
    new_text = re.sub(' +', " ", new_text)
    new_text = re.sub("[.]", ";", new_text)
    new_text = re.sub("\xa0", " ", new_text)
    return new_text


def parse_file(filename):
    try:
        with open(os.path.join(PATH, "content", filename), "rb") as f:
            html = pickle.load(f)
        content = BeautifulSoup(html, "html.parser")

        if content:

            # extract title
            title = ""
            title_tag = content.find("title")
            if title_tag is not None:
                title = title_tag.get_text()
            title = handle_text(title)

            # extract meta
            meta_info = parse_meta_info(content)
            meta_info = handle_text(meta_info)

            # extract headers
            headers = parse_headers(content)
            headers = [handle_text(header) for header in headers]

            # extract text
            # text = parse_text(content)
            # text = handle_text(text)
            text = ""

            with open(os.path.join(PATH, "parsed_punc_content", f"parsed_punc_{filename}"), "wb") as f:
                pickle.dump((text, title, meta_info, headers), f)

    except Exception:
        print(f"problem with {filename}")


def main():
    listdir = os.listdir(os.path.join(PATH, "content"))
    Parallel(n_jobs=4)(delayed(parse_file)(filename) for filename in tqdm(listdir, total=len(listdir)))


if __name__ == '__main__':
    main()
