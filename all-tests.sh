#!/bin/sh
OPENSSL_INCLUDE=/usr/local/opt/openssl/include/
for FILE in `ls *.c`; do
    TEST=${FILE%.*}

    echo === $TEST ===
    echo
    ./test.sh $TEST
    echo
done
