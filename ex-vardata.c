#include "common.h"
#include "varint.h"

int main() {
    uint8_t bytes[] = {
        0xfd, 0x0a, 0x00, 0xe3,
        0x03, 0x41, 0x8b, 0xa6,
        0x20, 0xe1, 0xb7, 0x83,
        0x60
    };

    size_t len;
    size_t varlen;
    uint8_t data[100] = { 0 };

    const char data_exp[] = "e303418ba620e1b78360";

    /* */

    len = bbp_varint_get(bytes, &varlen);
    printf("len: %lu, varlen: %lu\n", len, varlen);

    memcpy(data, bytes + varlen, len);

    bbp_print_hex("data      ", data, len);
    printf("data (exp): %s\n", data_exp);

    return 0;
}
