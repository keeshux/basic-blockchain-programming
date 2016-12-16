#ifndef __BASE58_H
#define __BASE58_H

#include <stdlib.h>
#include <openssl/bn.h>
#include "endian.h"
#include "hash.h"

static const char bbp_base58_alphabet[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

char *bbp_base58(const uint8_t *bytes, size_t len) {
    size_t str_len;
    char *str;
    BN_CTX *ctx;
    BIGNUM *base, *x, *r;
    int i, j;
    
    str_len = len * 138 / 100 + 2;
    str = calloc(str_len, sizeof(char));

    ctx = BN_CTX_new();
    BN_CTX_start(ctx);

    base = BN_new();
    x = BN_new();
    r = BN_new();
    BN_set_word(base, 58);
    BN_bin2bn(bytes, len, x);
    
    i = 0;
    while (!BN_is_zero(x)) {
        BN_div(x, r, x, base, ctx);
        str[i] = bbp_base58_alphabet[BN_get_word(r)];
        ++i;
    }
    for (j = 0; j < len; ++j) {
        if (bytes[j] != 0x00) {
            break;
        }
        str[i] = bbp_base58_alphabet[0];
        ++i;
    }
    bbp_reverse((uint8_t *)str, i);
    
    BN_clear_free(r);
    BN_clear_free(x);
    BN_free(base);
    BN_CTX_end(ctx);
    BN_CTX_free(ctx);

    return str;
}

char *bbp_base58check(const uint8_t *bytes, size_t len) {
    size_t check_len;
    uint8_t *check;
    uint8_t digest[32];
    char *str;

    check_len = len + 4;
    check = calloc(check_len, sizeof(char));

    bbp_hash256(digest, bytes, len);
    memcpy(check, bytes, len);
    memcpy(check + len, digest, 4);

    str = bbp_base58(check, check_len);
    free(check);

    return str;
}

#endif
