NOTE. Now part of Moses:
=========================

ProbingPT 2.1 has been integrated into Moses master. All updates to the code will happen on the moses repository and not here.
As of 13.06.2014 this repo is deprecated and exists only for reference.

To build [Moses](https://github.com/moses-smt/mosesdecoder) with ProbingPT do a:
```
./bjam -j10 --with-probing-pt
```


ProbingPT 2.1
========================================

Efficient phrase table implementation using kenLM's probing hash table. Models are taken from [StatMT](http://www.statmt.org/moses/RELEASE-1.0/models/en-cs/model/)
Use phrase-table.1.gz as source from any language.

Changelog from 2.0
-------------------
- Fixed improper hashing in some cases.
- Fixed a crash when a probability score is exactly 0.
- Added an API check so that you can't load the PT if the API has changed.
- Added initial preparation work to support reordering tables.

Build
------

Build KenLM first:

```
cd lib/kenlm
./bjam -j 5 link=static
```

Now build the testsfiles with the following command:

```
<clang++||g++> filename.cpp helpers/*.cpp -I./ -L./lib/kenlm/lib/ -lkenlm -lz -lbz2 -llzma  -lboost_serialization --std=c++11 -O3 -o output.o
```

Probing PT 2.1 demo decoder.
------------

You can try out Probing PT with a demo decoder:

Build both store_binary_vid and query_binary_vid:

```
<clang++||g++> store_binary_vid.cpp helpers/*.cpp -I./ -L./lib/kenlm/lib/ -lkenlm -lz -lbz2 -llzma  -lboost_serialization --std=c++11 -O3 -o store_binary_new.o
<clang++||g++> query_binary_vid.cpp helpers/*.cpp -I./ -L./lib/kenlm/lib/ -lkenlm -lz -lbz2 -llzma  -lboost_serialization --std=c++11 -O3 -o query_binary_new.o
```

After building you can create a phrase table by:

```
./store_binary_new.o path-to-phrasetable destination-dir
```
Where you provide the path to the phrase table and the location where the binary phrase table is created

Querying the binary is done by:

```
./query_binary.o destination-dir
```

KenLM
------
This project uses [kenLM](http://kheafield.com/code/kenlm/) licensed under LGPL
