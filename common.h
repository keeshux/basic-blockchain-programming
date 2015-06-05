#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "endian.h"

void bbp_print_hex(const char *label, const uint8_t *v, size_t len) {
    size_t i;
    const size_t mul = ((bbp_host_endian() == BBP_LITTLE) ? 0 : 1);

    printf("%s: ", label);
    for (i = 0; i < len; ++i) {
        const size_t offset = mul * (len - 2 * i - 1);
        printf("%02x", v[i + offset]);
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
    const size_t mul = ((e == BBP_LITTLE) ? 0 : 1);
    size_t i;

    for (i = 0; i < count; ++i) {
        const char hi = bbp_hex2byte(str[i * 2]);
        const char lo = bbp_hex2byte(str[i * 2 + 1]);
        const size_t offset = mul * (count - 2 * i - 1);

        v[i + offset] = hi * 16 + lo;
    }
}

uint8_t *bbp_alloc_hex(bbp_endian_t e, const char *str, size_t *len) {
    const size_t count = strlen(str) / 2;
    const size_t mul = ((e == BBP_LITTLE) ? 0 : 1);
    size_t i;

    uint8_t *v = malloc(count);

    for (i = 0; i < count; ++i) {
        const char hi = bbp_hex2byte(str[i * 2]);
        const char lo = bbp_hex2byte(str[i * 2 + 1]);
        const size_t offset = mul * (count - 2 * i - 1);

        v[i + offset] = hi * 16 + lo;
    }

    *len = count;

    return v;
}

#endif
