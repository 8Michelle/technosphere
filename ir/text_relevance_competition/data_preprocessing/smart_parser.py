import pickle
import re
import os
from bs4 import BeautifulSoup
from pymystem3 import Mystem
from joblib import Parallel, delayed


def get_text_from_html(content):
    if content:
        for s in content(["style", "script", "[document]", "head", "title"]):
            s.extract()
        return content.get_text(separator=" ")
    

def clean_text(text):
    text = text.replace("-\n", "")
    text = re.sub(re.compile("\W+"), " ", text)
    text = " ".join(re.split(re.compile("(\d+)"), text))
    text = text.replace('_', "")
    return text


def lemmatize_text(text, stemmer):
    return "".join(stemmer.lemmatize(text)).lower()


def get_meta_info(content):
    result = ""
    meta_tags = content.find_all("meta", {
    'name': ["keywords", "KEYWORDS", "keywords",
             "description", "Description", "DESCRIPTION"]
    })
    return " ".join([str(tag.get("content")) for tag in meta_tags if tag is not None])


def get_headers(content):
    headers = []
    for i in range(1, 7):
        header_tags = content.find_all(f"h{i}")
        headers.append(" ".join([str(tag.get_text()) for tag in header_tags if tag is not None]))
    return headers


def parse(filename, stemmer):
    try:
        with open(os.path.join(PATH, "content", filename), "rb") as f:
            data = pickle.load(f)
        content = BeautifulSoup(data, "html.parser")

        if content:

            # extract title
            title = ""
            title_tag = content.find("title")
            if title_tag is not None:
                title = title_tag.get_text()
            cleaned_title = clean_text(title)
            lemmed_title = lemmatize_text(cleaned_title, stemmer)

            # extract meta
            meta_info = get_meta_info(content)
            cleaned_meta_info = clean_text(meta_info)
            lemmed_meta_info = lemmatize_text(cleaned_meta_info, stemmer)

            # extract headers
            headers = get_headers(content)
            cleaned_headers = [clean_text(header) for header in headers]
            lemmed_headers = [lemmatize_text(header, stemmer) for header in cleaned_headers]

            # extract text
            text = get_text_from_html(content)
            cleaned_text = clean_text(text)
            lemmed_text = lemmatize_text(cleaned_text, stemmer)

            with open(os.path.join(PATH, "parsed_content", f"parsed_{filename}"), "wb") as f:
                pickle.dump((lemmed_text, lemmed_title, lemmed_meta_info, lemmed_headers), f)
    except:
        print(f"problem with {filename}")
        
if __name__ == '__main__':
    PATH = "./"
    stemmer = Mystem()
    listdir = os.listdir(os.path.join(PATH, "content"))
    Parallel(n_jobs=16)(delayed(parse)(filename, stemmer) for filename in listdir)
