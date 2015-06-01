#ifndef __ENDIAN_H
#define __ENDIAN_H

#include <stdint.h>
#include <string.h>
#include "common.h"

typedef enum {
    BBP_BIG,
    BBP_LITTLE
} bbp_endian_t;

bbp_endian_t bbp_host_endian() {
    static const union {
        uint16_t i;
        uint8_t c[2];
    } test = { 0x1234 };

    return ((test.c[0] == 0x34) ? BBP_LITTLE : BBP_BIG );
}

uint16_t bbp_swap16(uint16_t n) {
    return (n >> 8) |
           ((n & 0xff) << 8);
}

uint32_t bbp_swap32(uint32_t n) {
    return (n >> 24) |
           ((n & 0xff0000) >> 8) |
           ((n & 0xff00) << 8) |
           ((n & 0xff) << 24);
}

uint64_t bbp_swap64(uint64_t n) {
    return (n >> 56) |
           ((n & 0xff000000000000) >> 40) |
           ((n & 0xff0000000000) >> 24) |
           ((n & 0xff00000000) >> 8) |
           ((n & 0xff000000) << 8) |
           ((n & 0xff0000) << 24) |
           ((n & 0xff00) << 40) |
           ((n & 0xff) << 56);
}

uint16_t bbp_eint16(bbp_endian_t e, uint16_t n) {
    if (bbp_host_endian() == e) {
        return n;
    }
    return bbp_swap16(n);
}

uint32_t bbp_eint32(bbp_endian_t e, uint32_t n) {
    if (bbp_host_endian() == e) {
        return n;
    }
    return bbp_swap32(n);
}

uint64_t bbp_eint64(bbp_endian_t e, uint64_t n) {
    if (bbp_host_endian() == e) {
        return n;
    }
    return bbp_swap64(n);
}

void bbp_reverse(uint8_t *dst, size_t len) {
    size_t i;
    const size_t stop = len >> 1;
    for (i = 0; i < stop; ++i) {
        uint8_t *left = dst + i;
        uint8_t *right = dst + len - i - 1;
        const uint8_t tmp = *left;
        *left = *right;
        *right = tmp;
    }
}

void bbp_ecopy(bbp_endian_t e, uint8_t *dst, uint8_t *src, size_t len) {
    memcpy(dst, src, len);
    if (bbp_host_endian() != e) {
        bbp_reverse(dst, len);
    }
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
