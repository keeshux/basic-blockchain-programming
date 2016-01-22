#!/bin/sh
OPENSSL_INCLUDE=/usr/local/opt/openssl/include/
touch $1.c
gcc -ansi -Wall -Wno-deprecated-declarations -I $OPENSSL_INCLUDE -pedantic -lcrypto -o $1.out $1.c
./$1.out
