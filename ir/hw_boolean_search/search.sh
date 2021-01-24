#!/bin/bash

echo "Please enter your query: "
read query
python3 search.py -d "$query"