# 19.3: Creating a Taproot Multisig

Taproot multisigs can be easily created using descriptors. Doing so
reveals the full power of Taproot addresses.

## Understand a Taproot Multisig Descriptor

[BIP
387](https://github.com/bitcoin/bips/blob/master/bip-0387.mediawiki)
defines two Taproot multisig descriptors, `multi_a` and
`sortedmulti_a`. Their arguments take the form
`(threshold,key1,key2,...)`. They differ from `multi` and
`sortedmulti` because they produce Tapscript for use in a MAST.

As described in BIP 387, the multisig will be created using the
`OP_CHECKSIGADD` method described in the [previous
section](19_2_Designing_Tapscripts.md), not one of the more complex
methodologies made possible by the MAST.

## Create a Taproot Multisig Descriptor

The `multi_a` and `sortedmulti_a` descriptors both define script
paths. To include them in a `tr()` descriptor, you therefore use the
format `tr(KEY,TREE)` as also discussed in [the previous
section](19_2_Designing_Tapscripts.md).

You could put a lot of different things in the required key path,
including an unspendable key path if you only want the scriptpath to
apply. But in this situation we're going to take full advantage of the
`tr(KEY,TREE)` format to allow funds to be spendable either by a
single key or a multisig of 2-of-3 keys. Doing so displays the full
power of Taproot.

As it happens, we already have an example that does exactly that, the
excutives example laid out using Bitcoin Script in
[§15.2](15_2_Writing_Complex_Multisig_Scripts) and Miniscript in
[§16.4](16_4_Using_BDK). We're going to turn it into a real address
here, where either one president or two out of three vice-presidents
can spend from their multisig address.

The first thing you need to do is create wallets for all four
participants (or in a real situation, have them each working on a
different machine).

```sh
bitcoin-cli createwallet "prez"

| {
|   "name": "prez"
| }

bitcoin-cli createwallet "vp1"

| {
|   "name": "vp1"
| }

bitcoin-cli createwallet "vp2"

| {
|   "name": "vp2"
| }

bitcoin-cli createwallet "vp3"

| {
|   "name": "vp3"
| }
```

You then collect the public keys for addresses in each of the wallets,
a technique that you've used multiple times before.

```sh
prezaddr=$(bitcoin-cli -rpcwallet=prez getnewaddress)
prezpk=$(bitcoin-cli -rpcwallet=prez -named getaddressinfo address=$prezaddr | jq -r '.pubkey')

vp1addr=$(bitcoin-cli -rpcwallet=vp1 getnewaddress)
vp1pk=$(bitcoin-cli -rpcwallet=vp1 -named getaddressinfo address=$vp1addr | jq -r '.pubkey')

vp2addr=$(bitcoin-cli -rpcwallet=vp2 getnewaddress)
vp2pk=$(bitcoin-cli -rpcwallet=vp2 -named getaddressinfo address=$vp2addr | jq -r '.pubkey')

vp3addr=$(bitcoin-cli -rpcwallet=vp3 getnewaddress)
vp3pk=$(bitcoin-cli -rpcwallet=vp3 -named getaddressinfo address=$vp3addr | jq -r '.pubkey')
```

Now you can create your descriptor in the form `tr(KEY,TREE)` which in
this case will be `tr(PREZ-KEY,sortedmulti_a(THRESHOLD,VP-KEYS...))`.

```sh
tapdesc="tr($prezpk,sortedmulti_a(2,$vp1pk,$vp2pk,$vp3pk))"
tapcs=$(bitcoin-cli getdescriptorinfo $tapdesc | jq -r '.checksum')
tapdescwithcs=$tapdesc#$tapcs
```

As usual, you also create a watchonly wallet to store the descriptor
and extract your multisig address.

```sh
bitcoin-cli createwallet "prezwatchonly" true true

| {
|   "name": "prezwatchonly"
| }

bitcoin-cli -rpcwallet=prezwatchonly importdescriptors '''[{ "desc": "'$tapdescwithcs'", "timestamp": "now" }]'''

| [
|   {
|     "success": true
|   }
| ]

bitcoin-cli -rpcwallet=prezwatchonly getaddressesbylabel ""

| {
|   "tb1pttvmdfpws9mh8874mrcqnkfx67yqw3507g5m7ja2f9c7x9z7ju2qnl8csv": {
|     "purpose": "receive"
|   }
| }
```

## Prepare the PSBT

At this point either the president should be able to sign with the
single-sig key path or two of the vice presidents should be able to
sign with the multisig script path.

But, does it work?

We'll try it both ways, using the following PSBT, which sends all the
money to the president alone.

```sh
utxo_txid=$(bitcoin-cli -rpcwallet=prezwatchonly listunspent | jq -r '.[0] | .txid')
utxo_vout=$(bitcoin-cli -rpcwallet=prezwatchonly listunspent | jq -r '.[0] | .vout')

abscond=$(bitcoin-cli -rpcwallet=prez getnewaddress)

psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$abscond'": 0.00195 }''')
```

As usual, the first thing that's needed is to add the information
about the UTXO to the PSBT, which you can do from the watchonly wallet
with the descriptor.

```sh
psbt=$(bitcoin-cli -rpcwallet="prezwatchonly" walletprocesspsbt $psbt | jq -r '.psbt')
```

`analyzepsbt` shows that the UTXO is added, but that there are no signatures.

```sh
bitcoin-cli analyzepsbt $psbt

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": false,
|       "next": "updater"
|     }
|   ],
|   "estimated_vsize": 99,
|   "estimated_feerate": 0.00050505,
|   "fee": 0.00005000,
|   "next": "updater"
| }
```

## The President Signs

One possibility is that the president signs on his own:

```sh
prezpsbt=$(bitcoin-cli -rpcwallet="prez" walletprocesspsbt $psbt | jq -r '.psbt')
```

One signature, and it's ready to go!

```
bitcoin-cli analyzepsbt $prezpsbt

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": true,
|       "next": "extractor"
|     }
|   ],
|   "fee": 0.00005000,
|   "next": "extractor"
| }
```
Finalization gives you the hex to broadcast.

```sh
bitcoin-cli finalizepsbt $prezpsbt

| {
|   "hex": "0200000000010197669272637e525605366facfb6bdfd54c680bd17e71936f5e86773b0d02ca680000000000fdffffff01b8f90200000000001600141b4d9e1e0a6c8bd9ee02afed40a37f0a17080b660140e6fd0a517649f28d792952bd63771ccef0bbfd532f1a7559b43c778d4d88444550448065ee31f675c024c3bfdcddf753be1d426561f871a8615e8d24f1c1eaa700000000",
|   "complete": true
| }
```

## The VPs Sign

The other option is that two VPs sign:

```sh
vp1psbt=$(bitcoin-cli -rpcwallet="vp1" walletprocesspsbt $psbt | jq -r '.psbt')
vp2psbt=$(bitcoin-cli -rpcwallet="vp2" walletprocesspsbt $psbt | jq -r '.psbt')
```

Looking at the PSBTs, we see neither is ready to go (which is what we expect!).

```sh
bitcoin-cli analyzepsbt $vp1psbt

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": false,
|       "next": "updater"
|     }
|   ],
|   "estimated_vsize": 99,
|   "estimated_feerate": 0.00050505,
|   "fee": 0.00005000,
|   "next": "updater"
| }

bitcoin-cli analyzepsbt $vp2psbt

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": false,
|       "next": "updater"
|     }
|   ],
|   "estimated_vsize": 99,
|   "estimated_feerate": 0.00050505,
|   "fee": 0.00005000,
|   "next": "updater"
| }
```

Instead we have to combine them and then we're ready for the finalizer.

```sh
vppsbt=$(bitcoin-cli combinepsbt '''["'$vp1psbt'", "'$vp2psbt'"]''')

bitcoin-cli analyzepsbt $vppsbt

| {
|   "inputs": [
|     {
|       "has_utxo": true,
|       "is_final": false,
|       "next": "finalizer"
|     }
|   ],
|   "fee": 0.00005000,
|   "next": "finalizer"
| }
```

With that final step, you again get your hex for broadcast.

```sh
bitcoin-cli finalizepsbt $vppsbt

| {
|   "hex": "0200000000010197669272637e525605366facfb6bdfd54c680bd17e71936f5e86773b0d02ca680000000000fdffffff01b8f90200000000001600141b4d9e1e0a6c8bd9ee02afed40a37f0a17080b660540f4e6c17d03538797be4634ed2f157c263d1ee6371b67381b8584e0371974ad2bcc4d43bc7cc4ac172e2884f89536e8983fd3202defa186df2715c003e7aebe4d408bd5efb6831f4ec4073e0eefb367d1d52131954f058178b7fbbb92761b101d67aa3dee77ae4a9b45943a86688a11a309703be3d4d7317b0ff231a7cc4dc403b6006820a611ed5cebe29acd81073cba744d2893f2b4abf6f8074af92e02997b48ef1014ac20c179ec710ecf1601c29c1766b6ca535424497425701d649b8a58bc1ac52d3ba5ba20fb800917309e54549fc64ee193799995231a28997f1c80515647e6cf54713666ba529c21c0b7f68cf6265367d6cdeb7b80d06f1e683069373dcd8f4b2f16a0c5a75e3b71ce00000000",
|   "complete": true
| }
```

## Summary: Creating a Taproot Multisig

A Taproot multisig can be created with the `multisig_a` or
`sortedmultisig_a` descriptors. But that's just the script path and
each Taproot script path must be linked to a key path, which offers an
alternate way to unlock the transaction.

A Taproot multisig therefore doesn't just show another way to create
multisigs. It also displays the full power of Taproot, which will only
increase in the future as it achieves greater usage.
