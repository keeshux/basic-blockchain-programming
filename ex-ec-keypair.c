#include "common.h"
#include "ec.h"

int main() {
    uint8_t priv_bytes[32] = {
        0x16, 0x26, 0x07, 0x83, 0xe4, 0x0b, 0x16, 0x73,
        0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
        0xfc, 0x3e, 0xa4, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
        0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0xdc, 0x42
    };

    EC_KEY *key;
    uint8_t priv[32];
    uint8_t *pub;
    const BIGNUM *priv_bn;

    point_conversion_form_t conv_forms[] = {
        POINT_CONVERSION_UNCOMPRESSED,
        POINT_CONVERSION_COMPRESSED
    };
    const char *conv_forms_desc[] = {
        "uncompressed",
        "compressed"
    };
    int i;

    /* create keypair */

    key = bbp_ec_new_keypair(priv_bytes);
    if (!key) {
        puts("Unable to create keypair");
        return -1;
    }
    bbp_print_hex("priv", priv_bytes, sizeof(priv));

    /* get private key back from EC_KEY */

    priv_bn = EC_KEY_get0_private_key(key);
    if (!priv_bn) {
        puts("Unable to decode private key");
        return -1;
    }
    BN_bn2bin(priv_bn, priv);
    bbp_print_hex("priv", priv, sizeof(priv));

    /* get encoded public key from EC_KEY in all conversion forms */

    for (i = 0; i < sizeof(conv_forms) / sizeof(point_conversion_form_t); ++i) {
        size_t pub_len;
        uint8_t *pub_copy;

        EC_KEY_set_conv_form(key, conv_forms[i]);

        pub_len = i2o_ECPublicKey(key, NULL);
        pub = calloc(pub_len, sizeof(uint8_t));

        /* pub_copy is needed because i2o_ECPublicKey alters the input pointer */
        pub_copy = pub;
        if (i2o_ECPublicKey(key, &pub_copy) != pub_len) {
            puts("Unable to decode public key");
            return -1;
        }

        printf("conversion form: %s\n", conv_forms_desc[i]);
        bbp_print_hex("pub", pub, pub_len);

        free(pub);
    }

    /* release keypair */

    EC_KEY_free(key);

    return 0;
}
