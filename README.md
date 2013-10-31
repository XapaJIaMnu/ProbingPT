Proj4 - Honours project. Nothing, as of yet.
========================================

Build
------

Build KenLM first:

```
cd lib/kenlm
./bjam -j 5
```
Install the libkenlm library to your path.

(Assuming you are already in lib/kenlm)
```
cp lib/libkenlm.so /usr/lib
```

Now build the testsfiles with the following command:

```
<clang++||g++> filename.cpp -I./ -L./ -lkenlm -o output.o
```

KenLM
------
This project uses [kenLM](http://kheafield.com/code/kenlm/) licensed under LGPL