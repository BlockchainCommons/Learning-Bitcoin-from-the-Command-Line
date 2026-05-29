# 10.4: Exporting Secrets from Bitcoin Core

We've spent three sections importing a secret into Bitcoin Core. We're
now going to reverse course and see how to export a secret. (It's a
lot easier!)

## Reset Your Wallet

We're going to continue working in
[mainnet](https://learningbitcoin.blockchaincommons.com/10_3_Importing_Secrets_to_Bitcoin_Core/#use-mainnet)
for this example, but you should use a Bitcoin Core-generated wallet,
not the descriptor-based wallet we imported in the last section.

Unload your "seed" wallet from the last section or create a new ""
wallet depending on what you've done to date with Bitcoin Core.

```
bitcoin-cli unloadwallet "seed"

| {
| }

bitcoin-cli listwallets

| [
|   ""
| ]
```

## Understand What You Can Export

It would be ideal to export seeds or seed phrases, but Bitcoin Core
doesn't currently support doing so.

That leaves you with two options:

1. Export a master private key, which can be used to recreate all descriptors.
2. Export individual descriptors.

The first option makes it easy to store just a tiny bit of information
and restore everything from it, but you have to know what derivation
paths your addresses are attached to (or else, you have to check
addresses for every possible derivation path when you import).

The second option requires more data storage, but ensures that you
have the precise data on the accounts (derivation paths) where you
might have funds.

## Understand Bitcoin Core Outputs

The `listdescriptors` RPC command is what you use to retrieve
information about your keys and descriptors for export. You've seen it a number
of times:

```
bitcoin-cli listdescriptors

| {
|   "wallet_name": "",
|   "descriptors": [
|     {
|       "desc": "pkh([3ce1d173/44h/0h/0h]xpub6BxVLeeQGSK4tVGLLZopaabNwFUZe223FfLCAa3PWNxpwqnCcZVRvDNH13fKh4Ea5w785U9a7u1LX5Tu9m9gpdi2i5PEE6aMNvXGi2LuKdV/0/*)#q8nl9w4h",
|       "timestamp": 1779999396,
|       "active": true,
|       "internal": false,
|       "range": [
|         0,
|         999
|       ],
|       "next": 0,
|       "next_index": 0
|     },
|     {
|       "desc": "pkh([3ce1d173/44h/0h/0h]xpub6BxVLeeQGSK4tVGLLZopaabNwFUZe223FfLCAa3PWNxpwqnCcZVRvDNH13fKh4Ea5w785U9a7u1LX5Tu9m9gpdi2i5PEE6aMNvXGi2LuKdV/1/*)#3nk7cm90",
|       "timestamp": 1779999396,
|       "active": true,
|       "internal": true,
|       "range": [
|         0,
|         999
|       ],
|       "next": 0,
|       "next_index": 0
|     },
|     {
|       "desc": "sh(wpkh([3ce1d173/49h/0h/0h]xpub6BopUUpLTpQDti8eEjdRKhp1bGcxHXJ9Mvdm23iSPSPh84ofH6icrDg8sH4kowsZEDcYynaHPqVS4mvrMvgHpi2sNKSBEG9s11SeR9yfvJ2/0/*))#32q0qkdf",
|       "timestamp": 1779999396,
|       "active": true,
|       "internal": false,
|       "range": [
|         0,
|         999
|       ],
|       "next": 0,
|       "next_index": 0
|     },
|
| ...
|   ]
| }
```

By default, each descriptor shown by `listdescriptors` contains the
account public key for that descriptor. You'll note that they're the
same for the external and internal descriptors for the address, but
when you move on to a different derivation path, they're different.

So the public keys for the two `44h/0h/0h` addresses are
`xpub6BxVLe...` while the public key for the `49h/0h/0h` address is
`xpub6BopUU...`. You know that these addresses are account keys
because the account derivation path is given before the key in the
brackets.

Now, public keys aren't at all useful if you're trying to backup your
secrets. Fortunately, `listdescriptors` will give you private keys if
you add a `true` argument (which sets the `private` named argument to
true).

```
bitcoin-cli -rpcwallet="" listdescriptors true

| {
|   "wallet_name": "",
|   "descriptors": [
|     {
|       "desc": "pkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/44h/0h/0h/0/*)#e0uydrgt",
|       "timestamp": 1779999396,
|       "active": true,
|       "internal": false,
|       "range": [
|         0,
|         999
|       ],
|       "next": 0,
|       "next_index": 0
|     },
|     {
|       "desc": "pkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/44h/0h/0h/1/*)#gme9skcn",
|       "timestamp": 1779999396,
|       "active": true,
|       "internal": true,
|       "range": [
|         0,
|         999
|       ],
|       "next": 0,
|       "next_index": 0
|     },
|     {
|       "desc": "sh(wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/49h/0h/0h/0/*))#pfqdkhdr",
|       "timestamp": 1779999396,
|       "active": true,
|       "internal": false,
|       "range": [
|         0,
|         999
|       ],
|       "next": 0,
|       "next_index": 0
|     },
|
| ...
| 
|   ]
| }
```

You'll note that the [bracketed] prefix is now gone. Instead, the
entire derivation path appears _after_ the key (which is now a private
key). That means that you're now seeing the master private key, before
any derivation occurs. As you'd expect, that master key is exactly the
same for each of the accounts (derivation paths).

It's great that Bitcoin Core gives you access to the master private
key.

It's not great that it shows the underived master key in each of its
account descriptors, because that means that if any of them is
compromised, they all are. (But we can use `keytool` to change what we
store, which we'll do in the next section.)

### Testing Derivation

If you're ever unsure of what Bitcoin Core (or any other Bitcoin
program) is showing you, then you can always use `keytool` to test
your understanding.

In this case, we think we understand that `xprv9s21Zr...` is a master
private key and ``xpub6BxVLe...` is an account public key, derived
from it for `44h/0h/0h`. You can verify that by inputting the
(purported) master key into `keytool` and verifying that the account
public key for `44h/0h/0h` is what you expect.

```
keytool --master-key xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4 --account-derivation-path "m/44h/0h/0h" account-pub-key-base58

| xpub6BxVLeeQGSK4tVGLLZopaabNwFUZe223FfLCAa3PWNxpwqnCcZVRvDNH13fKh4Ea5w785U9a7u1LX5Tu9m9gpdi2i5PEE6aMNvXGi2LuKdV
```

It is!

## Store Your Data

You can now collect the information you'd need to restore this
data. `jq` can be used to pull out the whole list of descriptors:

```
DESCS=$(bitcoin-cli listdescriptors true | jq -r '.descriptors[].desc')
DESC_ARRAY=($DESCS)

for ((i = 0; i < 8; i++)); do
  echo "$i: ${DESC_ARRAY[$i]}";
done

| 0: pkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/44h/0h/0h/0/*)#e0uydrgt
| 1: pkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/44h/0h/0h/1/*)#gme9skcn
| 2: sh(wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/49h/0h/0h/0/*))#pfqdkhdr
| 3: sh(wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/49h/0h/0h/1/*))#82ggd6xh
| 4: tr(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/86h/0h/0h/0/*)#q58cmp0l
| 5: tr(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/86h/0h/0h/1/*)#3qzex5l8
| 6: wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/84h/0h/0h/0/*)#u8jwrsql
| 7: wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/84h/0h/0h/1/*)#dnh079s8
```

Since all of the descriptors include the master private key, you can
retrieve it from any of them.

```
MP_KEY=$(echo ${DESC_ARRAY[0]} | awk -F"[()]" '{print $2}' | awk -F"/" '{print $1}')
echo $MP_KEY

| xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/44h/0h/0h/0/*
```

## Summary: Exporting Secrets from Bitcoin Core

Bitcoin Core doesn't currently allow you to export the seed. However,
you can export either your master private key or your descriptors from
the `listdescriptors` command

## What's Next?

Conclude "Exploring the Ecosystem" with [§10.5: Storing Secrets with
Envelope](0_5_Storing_Secrets_with_Envelope.md).




