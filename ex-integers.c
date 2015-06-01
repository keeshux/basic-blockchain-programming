#include "common.h"

int main() {
    uint8_t n8 = 0x01;
    uint16_t n16 = 0x4523;
    uint32_t n32 = 0xcdab8967;
    uint64_t n64 = 0xdebc9a78563412ef;
    uint8_t ser[15];

    *ser = n8;
    *(uint16_t *)(ser + 1) = bbp_eint16(BBP_LITTLE, n16);
    *(uint32_t *)(ser + 3) = bbp_eint32(BBP_LITTLE, n32);
    *(uint64_t *)(ser + 7) = bbp_eint64(BBP_LITTLE, n64);

    /* 0123456789abcdef123456789abcde */
    bbp_print_hex("ser", ser, sizeof(ser));

    return 0;
}
