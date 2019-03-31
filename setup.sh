#!/bin/bash

rm -rf target
CC=clang CXX=clang++ meson setup target
