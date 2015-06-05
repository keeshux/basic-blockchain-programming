#ifndef __VARINT_H
#define __VARINT_H

#include <stdint.h>
#include "endian.h"

typedef enum {
    BBP_VARINT16 = 0xfd,
    BBP_VARINT32 = 0xfe,
    BBP_VARINT64 = 0xff
} bbp_varint_t;

uint64_t bbp_varint_get(uint8_t *bytes, size_t *len) {
    uint8_t prefix = *bytes;
    uint64_t value;

    *len = sizeof(uint8_t);

    if (prefix < BBP_VARINT16) {
        value = prefix;
    } else {
        uint8_t *ptr = bytes + *len;

        switch (prefix) {
            case BBP_VARINT16:
                value = bbp_eint16(BBP_LITTLE, *(uint16_t *)ptr);
                *len += sizeof(uint16_t);
                break;
            case BBP_VARINT32:
                value = bbp_eint32(BBP_LITTLE, *(uint32_t *)ptr);
                *len += sizeof(uint32_t);
                break;
            case BBP_VARINT64:
                value = bbp_eint64(BBP_LITTLE, *(uint64_t *)ptr);
                *len += sizeof(uint64_t);
                break;
        }
    }

    return value;
}

void bbp_varint_set(uint8_t *bytes, uint64_t n, size_t *len) {
    *len = sizeof(uint8_t);

    if (n < BBP_VARINT16) {
        *bytes = (uint8_t)n;
    } else {
        uint8_t header;

        if (n <= UINT16_MAX) {
            header = BBP_VARINT16;
            *(uint16_t *)(bytes + 1) = bbp_eint16(BBP_LITTLE, n);
            *len += sizeof(uint16_t);
        } else if (n <= UINT32_MAX) {
            header = BBP_VARINT32;
            *(uint32_t *)(bytes + 1) = bbp_eint32(BBP_LITTLE, n);
            *len += sizeof(uint32_t);
        } else {
            header = BBP_VARINT64;
            *(uint64_t *)(bytes + 1) = bbp_eint64(BBP_LITTLE, n);
            *len += sizeof(uint64_t);
        }

        *bytes = header;
    }
}

size_t bbp_varint_size(uint64_t n) {
    if (n < BBP_VARINT16) {
        return 1;
    } else if (n <= UINT16_MAX) {
        return 1 + sizeof(uint16_t);
    } else if (n <= UINT32_MAX) {
        return 1 + sizeof(uint32_t);
    } else {
        return 1 + sizeof(uint64_t);
    }
}

#endif
