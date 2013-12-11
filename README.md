Proj4 - Honours project. First alpha
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
<clang++||g++> filename.cpp -I./ -L./lib/kenlm/lib/ -lkenlm -lz -lbz2 -llzma -o output.o
```

First alpha
------------

After building you can create a phrase table by:

```
./store_binary.o ../test/phrase-table.1.gz 916284 /tmp/data.bin /tmp/hash.bin
```
Where you provide the path to the phrase table, the number of rows of the phrase table and output locations for the two binaries.

Querying the binary is done by:

```
./query_binary.o data.bin hash.bin 916284
```

KenLM
------
This project uses [kenLM](http://kheafield.com/code/kenlm/) licensed under LGPL