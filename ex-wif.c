#include "common.h"
#include "base58.h"
#include "hash.h"

int main() {
    uint8_t priv_bytes[32] = {
        0x16, 0x26, 0x07, 0x83, 0xe4, 0x0b, 0x16, 0x73,
        0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
        0xfc, 0x3e, 0xa4, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
        0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0xdc, 0x42
    };
    uint8_t wif_bytes[34];
    char *wif;

    const char wif_exp[] = "cNKkmrwHuShs2mvkVEKfXULxXhxRo3yy1cK6sq62uBp2Pc8Lsa76";

    /* */

    bbp_print_hex("priv", priv_bytes, sizeof(priv_bytes));

    wif_bytes[0] = 0xef;
    memcpy(wif_bytes + 1, priv_bytes, 32);
    wif_bytes[33] = 0x01;

    wif = bbp_base58check(wif_bytes, 34);
    printf("WIF      : %s\n", wif);
    printf("WIF (exp): %s\n", wif_exp);
    free(wif);

    return 0;
}
