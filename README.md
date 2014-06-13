NOTE. Now part of Moses:
=========================

ProbingPT 2.0 has been integrated into Moses master. All updates to the code will happen on the moses repository and not here.
As of 13.06.2014 this repo is deprecated and exists only for reference.

To build [Moses](https://github.com/moses-smt/mosesdecoder) with ProbingPT do a:
```
./bjam -j10 --with-probing-pt
```


ProbingPT 2.0
========================================

Efficient phrase table implementation using kenLM's probing hash table. Models are taken from [StatMT](http://www.statmt.org/moses/RELEASE-1.0/models/en-cs/model/)
Use phrase-table.1.gz as source from any language.

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

To create the library that links into Moses:
---------------------------------------------
```
ln -s ~/workspace/boost/boost_1_55_0 boost
cd helpers
./compile-lib.sh 
```

Then checkout Moses's [ProbingPT branch](https://github.com/moses-smt/mosesdecoder/tree/nikolay_probingPT) and build it with:
```
./bjam -j5 --with-probing-pt=/path/to/probingPT/repository
```

Probing PT 2.0
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
