#include "common.h"
#include "endian.h"
#include "tx.h"
#include "hash.h"
#include "ec.h"

int main() {
    uint8_t priv[32];
    EC_KEY *key;
    uint8_t *msg;
    size_t msg_len;
    uint8_t digest[32];
    uint8_t *sig;
    unsigned int sig_len;

    /* */

    /* keypair */
    bbp_parse_hex(priv, "16260783e40b16731673622ac8a5b045fc3ea4af70f727f3f9e92bdd3a1ddc42");
    key = bbp_ec_new_keypair(priv);

    /* message */
    msg = bbp_alloc_hex("0100000001f3a27f485f9833c8318c490403307fef1397121b5dd8fe70777236e7371c4ef3000000001976a9146bf19e55f94d986b4640c154d86469934191951188acffffffff02e0fe7e01000000001976a91418ba14b3682295cb05230e31fecb00089240660888ace084b003000000001976a9146bf19e55f94d986b4640c154d86469934191951188ac0000000001000000", &msg_len);

    /* signature */
    bbp_hash256(digest, msg, msg_len);
    sig_len = ECDSA_size(key);
    sig = malloc(sig_len);
    ECDSA_sign(0, digest, sizeof(digest), sig, &sig_len, key);

    /* */

    bbp_print_hex("digest", digest, sizeof(digest));
    bbp_print_hex("signature", sig, sig_len);

    free(sig);
    free(msg);
    EC_KEY_free(key);

    return 0;
}
