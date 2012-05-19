#!/bin/sh

mypath=${0%/*}
LIBDIR=$mypath/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBDIR
cd $mypath
./ztranslate

