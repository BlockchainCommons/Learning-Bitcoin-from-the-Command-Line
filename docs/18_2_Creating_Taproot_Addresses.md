# 18.2: Creating Taproot Addresses

Creating Taproot addresses, which are locked with a puzzle that
requires a Schnorr signature, is simplicity itself.

## Create a P2TR Address

A Taproot address requires that its UTXO be signed with a Schnorr
signature. All that you need to do to create one is generate an
address of type `bech32m` and then receive funds there.

```sh
bitcoin-cli -named getnewaddress address_type=bech32m

| tb1p0uvz780glaq08j8edv82udqj58v5vs8elkqdq8234g5mw48vjmvs5rqa8m
```

> 📖 **What is a bech32m?** Bech32 was the address format created for
use with Segwit v0. It was defined in
[BIP-173](https://github.com/bitcoin/bips/blob/master/bip-0173.mediawiki):
it uses only 32 lower-case letters and numbers (excluding "1", "b",
"i", and "o") and has a 6-character checksum at the end. It was
considered better than the older base58 format because it encoded more
efficiently in QR codes, guaranteed error detection, and was less
prone to human mistakes. Bech32m is a modified version of bech32
defined in
[BIP-350](https://github.com/bitcoin/bips/blob/master/bip-0350.mediawiki). It's
used for Segwit v1 (Taproot). It resolves a small issue where deleting
"q"s before a final "p" did not invalidate the checksum.

A bech32m address starts with bc1p (for mainnet) or tb1p (for testnet
and signet) where a bech32 address starts with bc1q (for mainnet) or
tb1q (for testnet and signet).

## Examine a P2TR Address

If you look at a `decoderawtransaction` for a transaction sent to a
P2TR address, the main thing you note is that the `type` is now `witness_v1_taproot`:

```sh
|     {
|       "value": 0.00200772,
|       "n": 1003,
|       "scriptPubKey": {
|         "asm": "1 7f182f1de8ff40f3c8f96b0eae3412a1d94640f9fd80d01d51aa29b754ec96d9",
|         "desc": "rawtr(7f182f1de8ff40f3c8f96b0eae3412a1d94640f9fd80d01d51aa29b754ec96d9)#9akpgdjz",
|         "hex": "51207f182f1de8ff40f3c8f96b0eae3412a1d94640f9fd80d01d51aa29b754ec96d9",
|         "address": "tb1p0uvz780glaq08j8edv82udqj58v5vs8elkqdq8234g5mw48vjmvs5rqa8m",
|         "type": "witness_v1_taproot"
|       }
|     },
```

This is in comparison to a Segwit v0 address, which is `witness_v0_keyhash`:

```sh
|     {
|       "value": 0.00200772,
|       "n": 13,
|       "scriptPubKey": {
|         "asm": "0 00339a7bfcb0a3eefe6ede08eadaf468903c8571",
|         "desc": "addr(tb1qqqee57lukz37alnwmcyw4kh5dzgrept30uuz4j)#xzeac206",
|         "hex": "001400339a7bfcb0a3eefe6ede08eadaf468903c8571",
|         "address": "tb1qqqee57lukz37alnwmcyw4kh5dzgrept30uuz4j",
|         "type": "witness_v0_keyhash"
|       }
|     },
```

## Sign with Schnorr

Spending your P2TR UTXO and signing with a Schnorr signature just
requires you to create and send a transaction, using any of the
techniques from [chapter 5](05_0_Sending_Bitcoin_Transactions.md).

But how do you know you actually signed with Schnorr?

Here's an example of a transaction that spends a Schnorr-locked UTXO:

```sh
bitcoin-cli decoderawtransaction 02000000000101acf6326a456bc54a2da1f8ff650d32c1251df86378fc5749e77a76a31d2ea350eb03000000fdffffff0219880100000000001600148fc40a49f2e4cc13c8dcf06664dc77c7e8cd53a9a086010000000000160014ee29e81d96be31c8d993b1148a7085867e685ab801401050f5c05cb7f4ad5bd5bdcb06300c99f36cc02de572ce828830261a9e325a2789d15363a7e88c13bc9a9ea73aeaf3d9415848aacf14f644745217b17eb76e2f93e00400

| {
|   "txid": "c36acc711a70e0b5de7186fbb179babdaa4c8d05cbd5d68e4398e0f2fb09230a",
|   "hash": "111e7403745fd13248e00da417c3ffb8844ddf7a5fcc8b06edf05eab21a3adc7",
|   "version": 2,
|   "size": 181,
|   "vsize": 130,
|   "weight": 520,
|   "locktime": 319635,
|   "vin": [
|     {
|       "txid": "50a32e1da3767ae74957fc7863f81d25c1320d65fff8a12d4ac56b456a32f6ac",
|       "vout": 1003,
|       "scriptSig": {
|         "asm": "",
|         "hex": ""
|       },
|       "txinwitness": [
|         "1050f5c05cb7f4ad5bd5bdcb06300c99f36cc02de572ce828830261a9e325a2789d15363a7e88c13bc9a9ea73aeaf3d9415848aacf14f644745217b17eb76e2f"
|       ],
|       "sequence": 4294967293
|     }
|   ],
| 
|   ...
|   
| }
```

Note that there's a single `txinwitness`:
`1050f5c05cb7f4ad5bd5bdcb06300c99f36cc02de572ce828830261a9e325a2789d15363a7e88c13bc9a9ea73aeaf3d9415848aacf14f644745217b17eb76e2f`. That
identifies it as a Schnorr signature because it's 64 bytes long.

Compare this to a transaction that spends a tradition Segwit (v0) UTXO:

```sh
bitcoin-cli decoderawtransaction 020000000001020a2309fbf2e098438ed6d5cb058d4caabdba79b1fb8671deb5e0701a71cc6ac30100000000fdffffff0a2309fbf2e098438ed6d5cb058d4caabdba79b1fb8671deb5e0701a71cc6ac30000000000fdffffff027ed3010000000000225120e3036eaf39529152ccfe89297a947893ae682198b06a3bc759b16d28aff2ffba8038010000000000225120269c54034ace169029e278f50e7c262fbea695e72edd26cc51f36ecba829a7520247304402203fe3bcd20994676b5e44f1758ca50315824e6d3be58f630b8773dccc4b048b7e02201fbbc5fa2c2df134f5d5f29a8320073d803b069007ee3db2ff98d9e87806e646012103973f2dfe31c7818763dbeaadee906970d6049a28fc2f083eb043058a760cce8a0247304402206cf945bb71fb7bca0c4951d16275be6ded1df13036821b583455f3d1ac2e1f1702201ab40a91c97e433e14a3ac1239ff153ecdf66b96386029e868f60bd7613cbdb601210278f8c8200dbc952571485d9688b06a1d1c5d4db2e582f9f28ad83d36b9cf32e894e00400

| {
|   "txid": "6c20958943b2644476aba7f44569494c80c461d1888403f2420dca783c176ccd",
|   "hash": "7d1c679dcfac029b66d01db9429bfee441e816f9b7fbb4921d44db5205fcf72c",
|   "version": 2,
|   "size": 394,
|   "vsize": 232,
|   "weight": 928,
|   "locktime": 319636,
|   "vin": [
|     {
|       "txid": "c36acc711a70e0b5de7186fbb179babdaa4c8d05cbd5d68e4398e0f2fb09230a",
|       "vout": 1,
|       "scriptSig": {
|         "asm": "",
|         "hex": ""
|       },
|       "txinwitness": [
|         "304402203fe3bcd20994676b5e44f1758ca50315824e6d3be58f630b8773dccc4b048b7e02201fbbc5fa2c2df134f5d5f29a8320073d803b069007ee3db2ff98d9e87806e64601",
|         "03973f2dfe31c7818763dbeaadee906970d6049a28fc2f083eb043058a760cce8a"
|       ],
|       "sequence": 4294967293
|     },
|     {
|       "txid": "c36acc711a70e0b5de7186fbb179babdaa4c8d05cbd5d68e4398e0f2fb09230a",
|       "vout": 0,
|       "scriptSig": {
|         "asm": "",
|         "hex": ""
|       },
|       "txinwitness": [
|         "304402206cf945bb71fb7bca0c4951d16275be6ded1df13036821b583455f3d1ac2e1f1702201ab40a91c97e433e14a3ac1239ff153ecdf66b96386029e868f60bd7613cbdb601",
|         "0278f8c8200dbc952571485d9688b06a1d1c5d4db2e582f9f28ad83d36b9cf32e8"
|       ],
|       "sequence": 4294967293
|     }
|   ],
| 
|   ...
|   
| }
```

This time, the `txinwitness` contains two elements: a 72-byte
signature (which identifies it as ECDSA) and a public key.

You don't need to know all the details, simply the fact that when you
create a Taproot address, it's locked with Schnorr, and any up-to-date
Bitcoin program will sign with Schnorr when you go to spend it.

## Summary: Creating Taproot Addresses

Creating Taproot address in Bitcoin Core simply requires that you
create a `bech32m` address with `createnewaddress`. Funds sent to the
address will then be locked with Schnorr, and you'll sign with Schnorr
when you're ready to spend them.

## What's Next?

Continue "Using Schnorr" with [§18.3: Using Bitcoin With FROST](18_3_Using_Bitcoin_With_FROST.md).

