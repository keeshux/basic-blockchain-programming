#include "common.h"
#include "ec.h"
#include "hash.h"

int main() {
    uint8_t pub_bytes[33] = {
        0x02,
        0x82, 0x00, 0x6e, 0x93, 0x98, 0xa6, 0x98, 0x6e,
        0xda, 0x61, 0xfe, 0x91, 0x67, 0x4c, 0x3a, 0x10,
        0x8c, 0x39, 0x94, 0x75, 0xbf, 0x1e, 0x73, 0x8f,
        0x19, 0xdf, 0xc2, 0xdb, 0x11, 0xdb, 0x1d, 0x28
    };
    uint8_t der_bytes[] = {
        0x30, 0x44, 0x02, 0x20, 0x2b, 0x2b, 0x52, 0x9b,
        0xdb, 0xdc, 0x93, 0xe7, 0x8a, 0xf7, 0xe0, 0x02,
        0x28, 0xb1, 0x79, 0x91, 0x8b, 0x03, 0x2d, 0x76,
        0x90, 0x2f, 0x74, 0xef, 0x45, 0x44, 0x26, 0xf7,
        0xd0, 0x6c, 0xd0, 0xf9, 0x02, 0x20, 0x62, 0xdd,
        0xc7, 0x64, 0x51, 0xcd, 0x04, 0xcb, 0x56, 0x7c,
        0xa5, 0xc5, 0xe0, 0x47, 0xe8, 0xac, 0x41, 0xd3,
        0xd4, 0xcf, 0x7c, 0xb9, 0x24, 0x34, 0xd5, 0x5c,
        0xb4, 0x86, 0xcc, 0xcf, 0x6a, 0xf2
    };
    const char message[] = "This is a very confidential message\n";

    EC_KEY *key;
    const uint8_t *der_bytes_copy;
    ECDSA_SIG *signature;
    uint8_t digest[32];
    int verified;

    const char *r_exp = "2B2B529BDBDC93E78AF7E00228B179918B032D76902F74EF454426F7D06CD0F9";
    const char *s_exp = "62DDC76451CD04CB567CA5C5E047E8AC41D3D4CF7CB92434D55CB486CCCF6AF2";
    const char *digest_exp = "4554813e91f3d5be790c7c608f80b2b00f3ea77512d49039e9e3dc45f89e2f01";

    /* */

    key = bbp_ec_new_pubkey(pub_bytes, sizeof(pub_bytes));
    if (!key) {
        puts("Unable to create keypair");
        return -1;
    }

    der_bytes_copy = der_bytes;
    signature = d2i_ECDSA_SIG(NULL, &der_bytes_copy, sizeof(der_bytes));
    printf("r      : %s\n", BN_bn2hex(signature->r));
    printf("r (exp): %s\n", r_exp);
    printf("s      : %s\n", BN_bn2hex(signature->s));
    printf("s (exp): %s\n", s_exp);

    bbp_sha256(digest, (uint8_t *)message, strlen(message));
    bbp_print_hex("digest      ", digest, 32);
    printf("digest (exp): %s\n", digest_exp);
    verified = ECDSA_do_verify(digest, sizeof(digest), signature, key);

    switch (verified) {
        case 1:
            puts("verified");
            break;
        case 0:
            puts("not verified");
            break;
        case -1:
            puts("library error");
            break;
    }

    ECDSA_SIG_free(signature);
    EC_KEY_free(key);

    return 0;
}
