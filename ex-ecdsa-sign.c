#include "common.h"
#include "ec.h"
#include "hash.h"

int main() {
    uint8_t priv_bytes[32] = {
        0x16, 0x26, 0x07, 0x83, 0xe4, 0x0b, 0x16, 0x73,
        0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
        0xfc, 0x3e, 0xa4, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
        0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0xdc, 0x42
    };
    const char message[] = "This is a very confidential message\n";

    EC_KEY *key;
    uint8_t digest[32];
    ECDSA_SIG *signature;
    uint8_t *der, *der_copy;
    size_t der_len;

    const char digest_exp[] = "4554813e91f3d5be790c7c608f80b2b00f3ea77512d49039e9e3dc45f89e2f01";

    /* */

    key = bbp_ec_new_keypair(priv_bytes);
    if (!key) {
        puts("Unable to create keypair");
        return -1;
    }

    bbp_sha256(digest, (uint8_t *)message, strlen(message));
    bbp_print_hex("digest      ", digest, 32);
    printf("digest (exp): %s\n", digest_exp);

    signature = ECDSA_do_sign(digest, sizeof(digest), key);
    printf("r: %s\n", BN_bn2hex(signature->r));
    printf("s: %s\n", BN_bn2hex(signature->s));

    der_len = ECDSA_size(key);
    der = calloc(der_len, sizeof(uint8_t));
    der_copy = der;
    i2d_ECDSA_SIG(signature, &der_copy);
    bbp_print_hex("DER-encoded", der, der_len);

    free(der);
    ECDSA_SIG_free(signature);
    EC_KEY_free(key);

    return 0;
}
