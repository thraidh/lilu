#!/bin/bash
set -e
if [ grammar.js -nt src/parser.c ]; then
    ./node_modules/.bin/tree-sitter generate
fi
./node_modules/.bin/tree-sitter parse f1

