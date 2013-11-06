Proj4 - Honours project. Nothing, as of yet.
========================================

Build
------

Build KenLM first:

```
cd lib/kenlm
./bjam -j 5 link=static
```

Now build the testsfiles with the following command:

```
<clang++||g++> filename.cpp -I./ -L./lib/kenlm/lib/ -lkenlm -lz -lbz2 -llzma output.o
```

KenLM
------
This project uses [kenLM](http://kheafield.com/code/kenlm/) licensed under LGPL