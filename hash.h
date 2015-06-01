#ifndef __HASH_H
#define __HASH_H

#include <openssl/sha.h>
#include <openssl/ripemd.h>

void bbp_sha256(uint8_t *digest, const uint8_t *message, size_t len) {
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, message, len);
    SHA256_Final(digest, &ctx);
}

void bbp_rmd160(uint8_t *digest, const uint8_t *message, size_t len) {
    RIPEMD160_CTX ctx;
    RIPEMD160_Init(&ctx);
    RIPEMD160_Update(&ctx, message, len);
    RIPEMD160_Final(digest, &ctx);
}

void bbp_hash256(uint8_t *digest, const uint8_t *message, size_t len) {
    uint8_t tmp[SHA256_DIGEST_LENGTH];
    bbp_sha256(tmp, message, len);
    bbp_sha256(digest, tmp, SHA256_DIGEST_LENGTH);
}

void bbp_hash160(uint8_t *digest, const uint8_t *message, size_t len) {
    uint8_t tmp[SHA256_DIGEST_LENGTH];
    bbp_sha256(tmp, message, len);
    bbp_rmd160(digest, tmp, SHA256_DIGEST_LENGTH);
}

#endif
