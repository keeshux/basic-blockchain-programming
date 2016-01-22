#include "common.h"

int main() {
    uint8_t n8 = 0x01;
    uint16_t n16 = 0x4523;
    uint32_t n32 = 0xcdab8967;
    uint64_t n64 = 0xdebc9a78563412ef;
    uint8_t ser[15];

    const char ser_exp[] = "0123456789abcdef123456789abcde";

    /* */

    *ser = n8;
    *(uint16_t *)(ser + 1) = bbp_eint16(BBP_LITTLE, n16);
    *(uint32_t *)(ser + 3) = bbp_eint32(BBP_LITTLE, n32);
    *(uint64_t *)(ser + 7) = bbp_eint64(BBP_LITTLE, n64);

    bbp_print_hex("ser      ", ser, sizeof(ser));
    printf("ser (exp): %s\n", ser_exp);

    return 0;
}
