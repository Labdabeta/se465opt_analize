#!/bin/bash 

opt -print-callgraph $1.bc.orig 2> $1.txt
./test $1.txt 1>$1.out

exit
