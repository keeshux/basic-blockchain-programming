#include "common.h"
#include "hash.h"

int main() {
    char message[] = "Hello Bitcoin!";
    uint16_t prefix = 0xd17f;
    uint8_t suffix = 0x8c;
    uint8_t digest[32];
    uint8_t ser[35];

    /* SHA-2 digest is big-endian */

    /* 1st round = 518ad5a375fa52f84b2b3df7933ad685eb62cf69869a96731561f94d10826b5c */
    bbp_sha256(digest, (uint8_t *)message, strlen(message));
    bbp_print_hex("SHA256(message)", digest, 32);

    /* 2nd round = 90986ea4e28b847cc7f9beba87ea81b221ca6eaf9828a8b04c290c21d891bcda */
    bbp_sha256(digest, digest, 32);
    bbp_print_hex("hash256(message)", digest, 32);

    *(uint16_t *)(ser) = bbp_eint16(BBP_LITTLE, prefix);
    memcpy(ser + 2, digest, 32);
    bbp_reverse(ser + 2, 32);
    *(ser + 2 + 32) = suffix;

    /* 7fd1dabc91d8210c294cb0a82898af6eca21b281ea87babef9c77c848be2a46e98908c */
    bbp_print_hex("ser", ser, sizeof(ser));

    return 0;
}
