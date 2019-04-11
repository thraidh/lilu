#!/bin/bash

rm -rf target
CC=clang-7 CXX=clang++-7 meson setup target
