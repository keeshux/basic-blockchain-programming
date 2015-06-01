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

#endif
