#ifndef __TX_H
#define __TX_H

#include <stdint.h>
#include "common.h"
#include "endian.h"
#include "varint.h"

typedef struct {
    uint64_t value;
    uint64_t script_len;
    uint8_t *script;
} bbp_txout_t;

typedef struct {
    uint8_t txid[32];
    uint32_t index;
} bbp_outpoint_t;

typedef struct {
    bbp_outpoint_t outpoint;
    uint64_t script_len;
    uint8_t *script;
    uint32_t sequence;
} bbp_txin_t;

typedef struct {
    uint32_t version;
    uint64_t inputs_len;
    bbp_txin_t *inputs;
    uint64_t outputs_len;
    bbp_txout_t *outputs;
    uint32_t locktime;
} bbp_tx_t;

typedef enum {
    BBP_SIGHASH_ALL = 0x01
} bbp_sighash_t;

typedef uint8_t *bbp_message_t;

void bbp_outpoint_fill(bbp_outpoint_t *outpoint, const char *txid, uint32_t index) {
    bbp_parse_hex(outpoint->txid, txid);
    bbp_reverse(outpoint->txid, 32);
    outpoint->index = bbp_eint32(BBP_LITTLE, index);
}

void bbp_txout_create_p2pkh(bbp_txout_t *txout, const uint64_t value, const char *hash160) {
    char script[52] = { 0 };
    sprintf(script, "76a914%s88ac", hash160);

    txout->value = bbp_eint64(BBP_LITTLE, value);
    txout->script = bbp_alloc_hex(script, (size_t *)&txout->script_len);
}

void bbp_txout_destroy(bbp_txout_t *txout) {
    free(txout->script);
}

void bbp_txin_create_p2pkh(bbp_txin_t *txin, const bbp_outpoint_t *outpoint,
        const char *sig, const char *pub, bbp_sighash_t flag) {

    char script[400] = { 0 };
    sprintf(script, "%02lx%s%02x%02lx%s", strlen(sig) / 2 + 1, sig, flag, strlen(pub) / 2, pub);

    memcpy(&txin->outpoint, outpoint, sizeof(bbp_outpoint_t));
    txin->script = bbp_alloc_hex(script, (size_t *)&txin->script_len);
    txin->sequence = 0xffffffff;
}

void bbp_txin_destroy(bbp_txin_t *txin) {
    free(txin->script);
}

/* signable message */

void bbp_txin_create_signable(bbp_txin_t *txin, const bbp_outpoint_t *outpoint, const bbp_txout_t *utxo) {
    memcpy(&txin->outpoint, outpoint, sizeof(bbp_outpoint_t));
    txin->script_len = utxo->script_len;
    txin->script = malloc(utxo->script_len);
    memcpy(txin->script, utxo->script, utxo->script_len);
    txin->sequence = 0xffffffff;
}

void bbp_txin_create_truncated(bbp_txin_t *txin, const bbp_outpoint_t *outpoint) {
    memcpy(&txin->outpoint, outpoint, sizeof(bbp_outpoint_t));
    txin->script_len = 0;
    txin->script = NULL;
    txin->sequence = 0xffffffff;
}

size_t bbp_tx_size(const bbp_tx_t *tx, bbp_sighash_t flag) {
    size_t size = 0;
    int i;

    /* version */
    size += sizeof(uint32_t);

    /* inputs count */
    size += bbp_varint_size(tx->inputs_len);

    /* inputs */
    for (i = 0; i < tx->inputs_len; ++i) {
        bbp_txin_t *txin = &tx->inputs[i];

        /* outpoint */
        size += sizeof(bbp_outpoint_t);

        /* script */
        size += bbp_varint_size(txin->script_len);
        size += txin->script_len;

        /* sequence */
        size += sizeof(uint32_t);
    }

    /* outputs count */
    size += bbp_varint_size(tx->outputs_len);

    /* outputs */
    for (i = 0; i < tx->outputs_len; ++i) {
        bbp_txout_t *txout = &tx->outputs[i];

        /* value */
        size += sizeof(uint64_t);

        /* script */
        size += bbp_varint_size(txout->script_len);
        size += txout->script_len;
    }

    /* locktime */
    size += sizeof(uint32_t);

    if (flag) {

        /* sighash */
        size += sizeof(uint32_t);
    }

    return size;
}

void bbp_tx_serialize(const bbp_tx_t *tx, uint8_t *raw, bbp_sighash_t flag) {
    uint8_t *ptr;
    size_t varlen;
    int i;

    ptr = raw;

    /* version */
    *(uint32_t *)ptr = bbp_eint32(BBP_LITTLE, tx->version);
    ptr += sizeof(uint32_t);

    /* inputs count */
    bbp_varint_set(ptr, tx->inputs_len, &varlen);
    ptr += varlen;

    /* inputs */
    for (i = 0; i < tx->inputs_len; ++i) {
        bbp_txin_t *txin = &tx->inputs[i];

        /* outpoint */
        memcpy(ptr, txin->outpoint.txid, 32);
        ptr += 32;
        *(uint32_t *)ptr = bbp_eint32(BBP_LITTLE, txin->outpoint.index);
        ptr += sizeof(uint32_t);

        /* script */
        bbp_varint_set(ptr, txin->script_len, &varlen);
        ptr += varlen;
        memcpy(ptr, txin->script, txin->script_len);
        ptr += txin->script_len;

        /* sequence */
        *(uint32_t *)ptr = bbp_eint32(BBP_LITTLE, txin->sequence);
        ptr += sizeof(uint32_t);
    }

    /* outputs count */
    bbp_varint_set(ptr, tx->outputs_len, &varlen);
    ptr += varlen;

    /* outputs */
    for (i = 0; i < tx->outputs_len; ++i) {
        bbp_txout_t *txout = &tx->outputs[i];

        /* value */
        *(uint64_t *)ptr = bbp_eint64(BBP_LITTLE, txout->value);
        ptr += sizeof(uint64_t);

        /* script */
        bbp_varint_set(ptr, txout->script_len, &varlen);
        ptr += varlen;
        memcpy(ptr, txout->script, txout->script_len);
        ptr += txout->script_len;
    }

    /* locktime */
    *(uint32_t *)ptr = bbp_eint32(BBP_LITTLE, tx->locktime);
    ptr += sizeof(uint32_t);

    if (flag) {

        /* sighash */
        *(uint32_t *)ptr = bbp_eint32(BBP_LITTLE, flag);
    }
}

#endif
