#!/bin/sh
touch $1.c
gcc -ansi -Wall -Wno-deprecated-declarations -pedantic -lcrypto -o $1.out $1.c
./$1.out
