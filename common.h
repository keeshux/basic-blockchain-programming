#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "endian.h"

size_t bbp_eoffset(size_t i, size_t len) {
    return ((bbp_host_endian() == BBP_LITTLE) ? i : (len - i - 1));
}

void bbp_print_hex(const char *label, const uint8_t *v, size_t len) {
    size_t i;

    printf("%s: ", label);
    for (i = 0; i < len; ++i) {
        printf("%02x", v[bbp_eoffset(i, len)]);
    }
    printf("\n");
}

uint8_t bbp_hex2byte(const char ch) {
    if ((ch >= '0') && (ch <= '9')) {
        return ch - '0';
    }
    if ((ch >= 'a') && (ch <= 'f')) {
        return ch - 'a' + 10;
    }
    return 0;
}

void bbp_parse_hex(bbp_endian_t e, uint8_t *v, const char *str) {
    const size_t count = strlen(str) / 2;
    size_t i;

    for (i = 0; i < count; ++i) {
        const char hi = bbp_hex2byte(str[i * 2]);
        const char lo = bbp_hex2byte(str[i * 2 + 1]);

        v[bbp_eoffset(i, count)] = hi * 16 + lo;
    }
}

uint8_t *bbp_alloc_hex(bbp_endian_t e, const char *str, size_t *len) {
    const size_t count = strlen(str) / 2;
    size_t i;

    uint8_t *v = malloc(count);

    for (i = 0; i < count; ++i) {
        const char hi = bbp_hex2byte(str[i * 2]);
        const char lo = bbp_hex2byte(str[i * 2 + 1]);

        v[bbp_eoffset(i, count)] = hi * 16 + lo;
    }

    *len = count;

    return v;
}

#endif
