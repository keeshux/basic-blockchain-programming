#include "common.h"
#include "base58.h"
#include "hash.h"

int main() {
    uint8_t pub_bytes[33] = {
        0x02,
        0x82, 0x00, 0x6e, 0x93, 0x98, 0xa6, 0x98, 0x6e,
        0xda, 0x61, 0xfe, 0x91, 0x67, 0x4c, 0x3a, 0x10,
        0x8c, 0x39, 0x94, 0x75, 0xbf, 0x1e, 0x73, 0x8f,
        0x19, 0xdf, 0xc2, 0xdb, 0x11, 0xdb, 0x1d, 0x28
    };
    uint8_t address_bytes[21];
    char *address;

    const char address_exp[] = "mqMi3XYqsPvBWtrJTk8euPWDVmFTZ5jHuK";

    /* */

    bbp_print_hex("pub", pub_bytes, sizeof(pub_bytes));

    address_bytes[0] = 0x6f;
    bbp_hash160(address_bytes + 1, pub_bytes, 33);
    bbp_print_hex("hash160", address_bytes + 1, 20);

    address = bbp_base58check(address_bytes, 21);
    printf("address      : %s\n", address);
    printf("address (exp): %s\n", address_exp);
    free(address);

    return 0;
}
