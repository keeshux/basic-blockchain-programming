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

    len = bbp_varint_get(bytes, &varlen);
    printf("len: %lu, varlen: %lu\n", len, varlen);

    bbp_ecopy(BBP_LITTLE, data, bytes + varlen, len);

    /* e303418ba620e1b78360 */
    bbp_print_hex("data", data, len);

    return 0;
}
