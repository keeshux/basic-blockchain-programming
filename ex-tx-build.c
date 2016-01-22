#include "common.h"
#include "endian.h"
#include "tx.h"
#include "hash.h"
#include "ec.h"

int main() {
    bbp_txout_t outs[2];
    bbp_txout_t prev_outs[1];
    bbp_txin_t ins_sign[1];
    bbp_outpoint_t outpoint;
    bbp_tx_t tx;
    uint8_t *msg;
    size_t msg_len;

    const char msg_exp[] = "0100000001f3a27f485f9833c8318c490403307fef1397121b5dd8fe70777236e7371c4ef3000000001976a9146bf19e55f94d986b4640c154d86469934191951188acffffffff02e0fe7e01000000001976a91418ba14b3682295cb05230e31fecb00089240660888ace084b003000000001976a9146bf19e55f94d986b4640c154d86469934191951188ac0000000001000000";

    /* */

    /* output 1 (0.251 BTC) */
    bbp_txout_create_p2pkh(&outs[0], 25100000, "18ba14b3682295cb05230e31fecb000892406608");

    /* output 2 (change, 0.619 BTC) */
    bbp_txout_create_p2pkh(&outs[1], 61900000, "6bf19e55f94d986b4640c154d864699341919511");

    /* input from utxo (0.87 BTC) */
    bbp_outpoint_fill(&outpoint, "f34e1c37e736727770fed85d1b129713ef7f300304498c31c833985f487fa2f3", 0);
    bbp_txout_create_p2pkh(&prev_outs[0], 87000000, "6bf19e55f94d986b4640c154d864699341919511");
    bbp_txin_create_signable(&ins_sign[0], &outpoint, &prev_outs[0]);

    /* message */
    tx.version = bbp_eint32(BBP_LITTLE, 1);
    tx.outputs_len = 2;
    tx.outputs = outs;
    tx.inputs_len = 1;
    tx.inputs = ins_sign;
    tx.locktime = 0;
    msg_len = bbp_tx_size(&tx, BBP_SIGHASH_ALL);
    msg = malloc(msg_len);
    bbp_tx_serialize(&tx, msg, BBP_SIGHASH_ALL);

    /* */

    bbp_print_hex("outs[0].script", outs[0].script, outs[0].script_len);
    bbp_print_hex("outs[1].script", outs[1].script, outs[1].script_len);
    puts("");
    bbp_print_hex("ins_sign[0].outpoint.txid", ins_sign[0].outpoint.txid, 32);
    printf("ins_sign[0].outpoint.index: %u\n", ins_sign[0].outpoint.index);
    bbp_print_hex("ins_sign[0].script", ins_sign[0].script, ins_sign[0].script_len);
    puts("");
    bbp_print_hex("msg      ", msg, msg_len);
    printf("msg (exp): %s\n", msg_exp);

    free(msg);
    bbp_txout_destroy(&outs[0]);
    bbp_txout_destroy(&outs[1]);
    bbp_txout_destroy(&prev_outs[0]);
    bbp_txin_destroy(&ins_sign[0]);

    return 0;
}
