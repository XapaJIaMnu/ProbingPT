 Proj4 - tests
================

This directory contains tests for various components.

To build a test do:

```
<clang++||g++> helpers/tests/filename.cpp helpers/* -I./ -L./lib/kenlm/lib/ -lkenlm -lz -lbz2 -llzma -lboost_serialization -o output.o
```