# 18.3: Using Bitcoin with MuSig2

As demonstrated in the previous section, signing with Schnorr is
easy. But what about signing Schnorr multisigs? There are at the
moment three main ways to do so: FROST and MuSig2 can be seamlessly
integrated into a Bitcoin transaction, but aren't seamlessly
integrated into Bitcoin Core. Instead, Bitcoin Core depends on its
Merkle trees for multisigs. This section and the next cover FROST and
MuSig2, while the next chapter includes discusison of MAST multisigs.

## Understand MuSig2

Bitcoin multisigs can't be naively created using Schnorr. If you just
aggregate two keys, it's possible to cancel out one of the keys
(provided you *know* the public key you're cancelling) and then sign
with the other key. To resolve this requires a more complex
protocols. One of these is MuSig2.

MuSig2 is a multisig method (n of n), not a threshold method (m of
n). That means that, by default, all of the participants in a MuSig2
signature must sign. The only current methodology for using MuSig2 to
create a threshold signature is to create a MAST that lays out each of
the possibilities, as briefly mentioned in
[§19.3](19_3_Creating_a_Schnorr_Multisig.md).

## Create a MuSig2 Address

Creating a MuSig2 multisig address requires largely the same
methodology as demonstrated in
[§7.2](07_2_Creating_Multisig_Descriptors.md), except this time we're
going to generate it from two different wallets, not two different
machines.

```sh
bitcoin-cli createwallet musig1

| {
|   "name": "musig1"
| }

bitcoin-cli createwallet musig2

| {
|   "name": "musig2"
| }
```

> ℹ️ **Multi Multisig Methods.** Multisigs should really be generated
from different private keys held on different devices. This simplified
multi-wallet methodology is used only to make it easier for you to
follow along, without having to spin up a second machine.

As usual, you will extract addresses and public keys from each wallet:

```sh
musigaddress1=$(bitcoin-cli -rpcwallet=musig1 getnewaddress)
musigpubkey1=$(bitcoin-cli -rpcwallet=musig1 -named getaddressinfo address=$musigaddress1 | jq -r '.pubkey')

musigaddress2=$(bitcoin-cli -rpcwallet=musig2 getnewaddress)
musigpubkey2=$(bitcoin-cli -rpcwallet=musig2 -named getaddressinfo address=$musigaddress2 | jq -r '.pubkey')
```

You can then use the "tr(musig(...))" descriptor, to denote this
address as a MuSig2 Taproot address, when combining these public keys.

```sh
musigdesc="tr(musig($musigpubkey1,$musigpubkey2))"
musigcs=$(bitcoin-cli getdescriptorinfo $musigdesc | jq -r '.checksum')
musigdescwithcs=$musigdesc#$musigcs
```

Finally, you'll create a third, watchonly account, to store the MuSig2
descriptor (and allow you to monitor the address).

```sh
bitcoin-cli createwallet "musigwatchonly" true true

| {
|   "name": "musigwatchonly"
| }

bitcoin-cli -rpcwallet=musigwatchonly importdescriptors '''[{ "desc": "'$musigdescwithcs'", "timestamp": "now" }]'''

| [
|   {
|     "success": true
|   }
| ]
```

Now you just need your address:

```sh
bitcoin-cli -rpcwallet=musigwatchonly getaddressesbylabel ""

| {
|   "tb1plj6kpu6mw7pd0jngwaz6555aehjudg05reh2jy2mjq0quyuzyvjsfkhw57": {
|     "purpose": "receive"
|   }
| }
```

As soon as you send funds to that address, you'll officially have
Bitcoin assets locked with a MuSig2 signature.

The question will be how to retrieve them!

## Spend from a MuSig2 Address

As we saw in [§8.1](8_1_Spending_a_Multisig_with_a_PSBT), the best way
to spend a multisig is through a PSBT. That's the case for MuSig2
multisigs as well.

The process of spending the funds therefore start with the creation of
a PSBT. As before, we're going to send the money from the multisig
address to the two creators of the multisig.

```sh
split1=$(bitcoin-cli -rpcwallet=musig1 getnewaddress)
split2=$(bitcoin-cli -rpcwallet=musig2 getnewaddress)

utxo_txid=$(bitcoin-cli -rpcwallet=musigwatchonly listunspent | jq -r '.[0] | .txid')
utxo_vout=$(bitcoin-cli -rpcwallet=musigwatchonly listunspent | jq -r '.[0] | .vout')

psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$split1'": 0.0029,"'$split2'": 0.0029 }''')
```

### Load the Multisig Info

Our old friend `analyzepsbt` says that the next thing required is the
updater.

```sh
bitcoin-cli analyzepsbt $psbt

| {
|   "inputs": [
|     {
|       "has_utxo": false,
|       "is_final": false,
|       "next": "updater"
|     }
|   ],
|   "next": "updater"
| }
```

Using `walletprocesspsbt` on the readonly wallet should load the UTXO.

```sh
psbt=$(bitcoin-cli -rpcwallet="musigwatchonly" walletprocesspsbt $psbt | jq -r '.psbt')

bitcoin-cli analyzepsbt $psbt

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": false,
|       "next": "updater"
|     }
|   ],
|   "estimated_vsize": 130,
|   "estimated_feerate": 0.00093092,
|   "fee": 0.00012102,
|   "next": "updater"
| }
```

It does, but it doesn't tell you that the "signer" is the next step,
as we'd expect. It turns out that `analyzepsbt` doesn't properly
support MuSig2 signatures as of Bitcoin Core 31.1, and so it's not
going to show you the next steps properly until you're ready to
finalize.

> ⚠️ **Version Warning:** These examples were built with Bitcoin Core
31.1, when MuSig2 integration was still partial. Though the whole
system works, it doesn't give proper responses to analysis. This is
expected to improve, so some of these `analyzepsbt` results will
likely be different in the future.

### Round 1 of Signing

You nonetheless proceed forward with signing:

```sh
sig1=$(bitcoin-cli -rpcwallet="musig1" walletprocesspsbt $psbt | jq -r '.psbt')
sig2=$(bitcoin-cli -rpcwallet="musig2" walletprocesspsbt $psbt | jq -r '.psbt')
psbt_round1=$(bitcoin-cli combinepsbt '''["'$sig1'", "'$sig2'"]''')
```

Afterward, the PSBT still isn't ready to be finalized:

```sh
bitcoin-cli analyzepsbt $psbt_round1

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": false,
|       "next": "updater"
|     }
|   ],
|   "estimated_vsize": 130,
|   "estimated_feerate": 0.00093092,
|   "fee": 0.00012102,
|   "next": "updater"
| }
```

A look with `decodepsbt` reveals that quite a lot has gone on,
including the addition of pubkeys and pubnonces (whatever those are!)
from each participant, but there are no signatures!

```sh
bitcoin-cli decodepsbt $psbt_round1

| {
|   "tx": {
|     "txid": "b7b61fde33cf4bd92ca2a2bf0dd18c2d1c647fce2bcb153bc65a55df23e77c2f",
|     "hash": "b7b61fde33cf4bd92ca2a2bf0dd18c2d1c647fce2bcb153bc65a55df23e77c2f",
|     "version": 2,
|     "size": 113,
|     "vsize": 113,
|     "weight": 452,
|     "locktime": 0,
|     "vin": [
|       {
|         "txid": "da394f1decbfa228b0e24a3dfd48fad059bac2c25d5029d2c4fd64cc01688900",
|         "vout": 473,
|         "scriptSig": {
|           "asm": "",
|           "hex": ""
|         },
|         "sequence": 4294967293
|       }
|     ],
|     "vout": [
|       {
|         "value": 0.00290000,
|         "n": 0,
|         "scriptPubKey": {
|           "asm": "0 79f006598b3a92338566a33ad2c851614666a841",
|           "desc": "addr(tb1q08cqvkvt82fr8ptx5vad9jz3v9rxd2zpzv7mkq)#x8vd6m8w",
|           "hex": "001479f006598b3a92338566a33ad2c851614666a841",
|           "address": "tb1q08cqvkvt82fr8ptx5vad9jz3v9rxd2zpzv7mkq",
|           "type": "witness_v0_keyhash"
|         }
|       },
|       {
|         "value": 0.00290000,
|         "n": 1,
|         "scriptPubKey": {
|           "asm": "0 49efd0e52318e535fd4b013c8ca9769ad9bb281c",
|           "desc": "addr(tb1qf8hapefrrrjntl2tqy7ge2tkntvmk2quah4a9e)#x5c9s5a2",
|           "hex": "001449efd0e52318e535fd4b013c8ca9769ad9bb281c",
|           "address": "tb1qf8hapefrrrjntl2tqy7ge2tkntvmk2quah4a9e",
|           "type": "witness_v0_keyhash"
|         }
|       }
|     ]
|   },
|   "global_xpubs": [
|   ],
|   "psbt_version": 0,
|   "proprietary": [
|   ],
|   "unknown": {
|   },
|   "inputs": [
|     {
|       "witness_utxo": {
|         "amount": 0.00592102,
|         "scriptPubKey": {
|           "asm": "1 fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "desc": "rawtr(fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325)#svd2lm0a",
|           "hex": "5120fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "address": "tb1plj6kpu6mw7pd0jngwaz6555aehjudg05reh2jy2mjq0quyuzyvjsfkhw57",
|           "type": "witness_v1_taproot"
|         }
|       },
|       "taproot_bip32_derivs": [
|         {
|           "pubkey": "319ae6ea6d8e51dec150cca7c947deb32c53f13fc7a419196c6a45cc383251bd",
|           "master_fingerprint": "1af283b0",
|           "path": "m",
|           "leaf_hashes": [
|           ]
|         },
|         {
|           "pubkey": "b3ddf561bcb16f12a8bdae9dc5577229f892b538fe296cec680b43a51aea706f",
|           "master_fingerprint": "e90bd7e0",
|           "path": "m",
|           "leaf_hashes": [
|           ]
|         },
|         {
|           "pubkey": "f73be05fa2dae3ac969bb40667b6c66d9e92ddde302053a4b1602ee540fbd95e",
|           "master_fingerprint": "be075d2d",
|           "path": "m",
|           "leaf_hashes": [
|           ]
|         }
|       ],
|       "taproot_internal_key": "f73be05fa2dae3ac969bb40667b6c66d9e92ddde302053a4b1602ee540fbd95e",
|       "musig2_participant_pubkeys": [
|         {
|           "aggregate_pubkey": "03f73be05fa2dae3ac969bb40667b6c66d9e92ddde302053a4b1602ee540fbd95e",
|           "participant_pubkeys": [
|             "02319ae6ea6d8e51dec150cca7c947deb32c53f13fc7a419196c6a45cc383251bd",
|             "02b3ddf561bcb16f12a8bdae9dc5577229f892b538fe296cec680b43a51aea706f"
|           ]
|         }
|       ],
|       "musig2_pubnonces": [
|         {
|           "participant_pubkey": "02319ae6ea6d8e51dec150cca7c947deb32c53f13fc7a419196c6a45cc383251bd",
|           "aggregate_pubkey": "02fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "pubnonce": "036772d6511251888c0461be416592de501648f706006732b1390639a9359e30f7030bba86343255e5b7eebc276084b077421bb45d4ba466430180653ed829652c40"
|         },
|         {
|           "participant_pubkey": "02b3ddf561bcb16f12a8bdae9dc5577229f892b538fe296cec680b43a51aea706f",
|           "aggregate_pubkey": "02fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "pubnonce": "03ec44562d6bd19a57d47ab0f5554bcddce6385c877b54590df898cc8251dabea6031dc5ffcf5928073f8be736a7cad82845a8c26e86a78938f007ad5372fe5018bf"
|         }
|       ]
|     }
|   ],
|   "outputs": [
|     {
|       "bip32_derivs": [
|         {
|           "pubkey": "03c544d7c0329494974195c51c630b7f14eb919f7dbc5c7bec49184ca19f7f64ef",
|           "master_fingerprint": "0355e63d",
|           "path": "m/84h/1h/0h/0/1"
|         }
|       ]
|     },
|     {
|       "bip32_derivs": [
|         {
|           "pubkey": "02d209e51b70dac6d8e4c0c510964d7c957074bbff64272b14314cf90315d44ca2",
|           "master_fingerprint": "d6ca3dc0",
|           "path": "m/84h/1h/0h/0/1"
|         }
|       ]
|     }
|   ],
|   "fee": 0.00012102
| }
```

What's going on?

The two major Schnorr multisig protocols, MuSig2 and FROST, are both
multiround protocols. That means that each participant provides some
information, it's sent around to the other participants, and only then
can they create a final signature.

For MuSig2, in the first round, each participant offers a "pubnonce":
essentially a bit of randomness that sort of fills the role of a
public key. In the second round, they then sign using their private
nonce and their private key. Those signature shares are then combined to
produce the final signature.

You've conducted the first round of the MuSig2 protocol, and acquired
the nonces, but another round is required to acquire the signatures.

### Round 2 of Signing

You send your combined PSBT around to everyone to process again, this
time actually acquiring signatures:

```
sig1b=$(bitcoin-cli -rpcwallet="musig1" walletprocesspsbt $psbt_round1 | jq -r '.psbt')
sig2b=$(bitcoin-cli -rpcwallet="musig2" walletprocesspsbt $psbt_round1 | jq -r '.psbt')
psbt_round2=$(bitcoin-cli combinepsbt '''["'$sig1b'", "'$sig2b'"]''')
```

Finally, `analyzepsbt` shows the need for something other than an updater!

```sh
bitcoin-cli analyzepsbt $psbt_round2

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": false,
|       "next": "finalizer"
|     }
|   ],
|   "fee": 0.00012102,
|   "next": "finalizer"
| }
```

Here's what the combined PSBT looks like:

```sh
bitcoin-cli decodepsbt $psbt_round2

| {
|   "tx": {
|     "txid": "b7b61fde33cf4bd92ca2a2bf0dd18c2d1c647fce2bcb153bc65a55df23e77c2f",
|     "hash": "b7b61fde33cf4bd92ca2a2bf0dd18c2d1c647fce2bcb153bc65a55df23e77c2f",
|     "version": 2,
|     "size": 113,
|     "vsize": 113,
|     "weight": 452,
|     "locktime": 0,
|     "vin": [
|       {
|         "txid": "da394f1decbfa228b0e24a3dfd48fad059bac2c25d5029d2c4fd64cc01688900",
|         "vout": 473,
|         "scriptSig": {
|           "asm": "",
|           "hex": ""
|         },
|         "sequence": 4294967293
|       }
|     ],
|     "vout": [
|       {
|         "value": 0.00290000,
|         "n": 0,
|         "scriptPubKey": {
|           "asm": "0 79f006598b3a92338566a33ad2c851614666a841",
|           "desc": "addr(tb1q08cqvkvt82fr8ptx5vad9jz3v9rxd2zpzv7mkq)#x8vd6m8w",
|           "hex": "001479f006598b3a92338566a33ad2c851614666a841",
|           "address": "tb1q08cqvkvt82fr8ptx5vad9jz3v9rxd2zpzv7mkq",
|           "type": "witness_v0_keyhash"
|         }
|       },
|       {
|         "value": 0.00290000,
|         "n": 1,
|         "scriptPubKey": {
|           "asm": "0 49efd0e52318e535fd4b013c8ca9769ad9bb281c",
|           "desc": "addr(tb1qf8hapefrrrjntl2tqy7ge2tkntvmk2quah4a9e)#x5c9s5a2",
|           "hex": "001449efd0e52318e535fd4b013c8ca9769ad9bb281c",
|           "address": "tb1qf8hapefrrrjntl2tqy7ge2tkntvmk2quah4a9e",
|           "type": "witness_v0_keyhash"
|         }
|       }
|     ]
|   },
|   "global_xpubs": [
|   ],
|   "psbt_version": 0,
|   "proprietary": [
|   ],
|   "unknown": {
|   },
|   "inputs": [
|     {
|       "witness_utxo": {
|         "amount": 0.00592102,
|         "scriptPubKey": {
|           "asm": "1 fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "desc": "rawtr(fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325)#svd2lm0a",
|           "hex": "5120fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "address": "tb1plj6kpu6mw7pd0jngwaz6555aehjudg05reh2jy2mjq0quyuzyvjsfkhw57",
|           "type": "witness_v1_taproot"
|         }
|       },
|       "taproot_bip32_derivs": [
|         {
|           "pubkey": "319ae6ea6d8e51dec150cca7c947deb32c53f13fc7a419196c6a45cc383251bd",
|           "master_fingerprint": "1af283b0",
|           "path": "m",
|           "leaf_hashes": [
|           ]
|         },
|         {
|           "pubkey": "b3ddf561bcb16f12a8bdae9dc5577229f892b538fe296cec680b43a51aea706f",
|           "master_fingerprint": "e90bd7e0",
|           "path": "m",
|           "leaf_hashes": [
|           ]
|         },
|         {
|           "pubkey": "f73be05fa2dae3ac969bb40667b6c66d9e92ddde302053a4b1602ee540fbd95e",
|           "master_fingerprint": "be075d2d",
|           "path": "m",
|           "leaf_hashes": [
|           ]
|         }
|       ],
|       "taproot_internal_key": "f73be05fa2dae3ac969bb40667b6c66d9e92ddde302053a4b1602ee540fbd95e",
|       "musig2_participant_pubkeys": [
|         {
|           "aggregate_pubkey": "03f73be05fa2dae3ac969bb40667b6c66d9e92ddde302053a4b1602ee540fbd95e",
|           "participant_pubkeys": [
|             "02319ae6ea6d8e51dec150cca7c947deb32c53f13fc7a419196c6a45cc383251bd",
|             "02b3ddf561bcb16f12a8bdae9dc5577229f892b538fe296cec680b43a51aea706f"
|           ]
|         }
|       ],
|       "musig2_pubnonces": [
|         {
|           "participant_pubkey": "02319ae6ea6d8e51dec150cca7c947deb32c53f13fc7a419196c6a45cc383251bd",
|           "aggregate_pubkey": "02fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "pubnonce": "036772d6511251888c0461be416592de501648f706006732b1390639a9359e30f7030bba86343255e5b7eebc276084b077421bb45d4ba466430180653ed829652c40"
|         },
|         {
|           "participant_pubkey": "02b3ddf561bcb16f12a8bdae9dc5577229f892b538fe296cec680b43a51aea706f",
|           "aggregate_pubkey": "02fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "pubnonce": "03ec44562d6bd19a57d47ab0f5554bcddce6385c877b54590df898cc8251dabea6031dc5ffcf5928073f8be736a7cad82845a8c26e86a78938f007ad5372fe5018bf"
|         }
|       ],
|       "musig2_partial_sigs": [
|         {
|           "participant_pubkey": "02319ae6ea6d8e51dec150cca7c947deb32c53f13fc7a419196c6a45cc383251bd",
|           "aggregate_pubkey": "02fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "partial_sig": "d4566f7317280efeb0df495a7419b83bae5ba701b67e45266dd94dd5c712fde1"
|         },
|         {
|           "participant_pubkey": "02b3ddf561bcb16f12a8bdae9dc5577229f892b538fe296cec680b43a51aea706f",
|           "aggregate_pubkey": "02fcb560f35b7782d7ca687745aa529dcde5c6a1f41e6ea9115b901e0e13822325",
|           "partial_sig": "0f6e6c809fc1381285fe935daac70551ded5649fd1e946fcd297fc8f49e60a86"
|         }
|       ]
|     }
|   ],
|   "outputs": [
|     {
|       "bip32_derivs": [
|         {
|           "pubkey": "03c544d7c0329494974195c51c630b7f14eb919f7dbc5c7bec49184ca19f7f64ef",
|           "master_fingerprint": "0355e63d",
|           "path": "m/84h/1h/0h/0/1"
|         }
|       ]
|     },
|     {
|       "bip32_derivs": [
|         {
|           "pubkey": "02d209e51b70dac6d8e4c0c510964d7c957074bbff64272b14314cf90315d44ca2",
|           "master_fingerprint": "d6ca3dc0",
|           "path": "m/84h/1h/0h/0/1"
|         }
|       ]
|     }
|   ],
|   "fee": 0.00012102
| }
```

The `pubnonces` have now been supplemented with partial signatures.

### Finalize the PSBT

At this point, you just need to finalize and send:

```sh
psbt_hex=$(bitcoin-cli finalizepsbt $psbt_round2 | jq -r '.hex')

bitcoin-cli -named sendrawtransaction hexstring=$psbt_hex

| b7b61fde33cf4bd92ca2a2bf0dd18c2d1c647fce2bcb153bc65a55df23e77c2f
```

You've successfully respent a MuSig2 Schnorr transaction with a MuSig2
signature!

## Summary: Using Bitcoin with MuSig2

Bitcoin Core supports MuSig2, but the lack of proper analysis makes it
unintuitive to use. Still, the overall process is simple once you get
to know it: fill in the UTXO; have each signer process to provide a
nonce; combine the PSBTs; have each signer process again to sign;
combine the PSBTs again; and send.

## What's Next?

Continue "Using Schnorr" with [§18.4: Using Bitcoin With
MuSig2](18_4_Using_Bitcoin_With_MuSig2.md).

