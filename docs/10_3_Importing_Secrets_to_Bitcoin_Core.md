# 10.3: Importing Secrets to Bitcoin Core

You've used your seed (from §10.1) to generate account keys (in
§10.2), which is what you need to create descriptors that will allow
you to input addresses derived from the seed into Bitcoin Core.

## Use Mainnet

As mentioned in the last section, we've been working with mainnet in
the examples in this chapter.  As a result, the `coin_type` has been
`0h` in our `m/purpose'/coin_type'/account'/` derivation paths, and
we've been generating `xprv`.  That means you're going to need to use
mainnet to test out how to import and export works in this chapter.

To ensure this, go to ~/.bitcoin/bitcoin.conf and change the `signet=1` line to `signet=0`:

```
cat bitcoin.conf

| server=1
| dbcache=1536
| par=1
| maxuploadtarget=137
| maxconnections=16
| rpcuser=StandUp
| rpcpassword=4737ef625b30471038897d1f44c3423d
| rpcallowip=127.0.0.1
| debug=tor
| prune=550
| signet=0
| [test]
| rpcbind=127.0.0.1
| rpcport=18332
| [main]
| rpcbind=127.0.0.1
| rpcport=8332
| [regtest]
| rpcbind=127.0.0.1
| rpcport=18443
| [signet]
| rpcbind=127.0.0.1
| rpcport=38332
```

You then need to find `bitcoind` in your process table:
```
ps auxww | grep bitcoin

| standup    24832  3.1  9.8 4931716 395660 ?      SLsl 17:07   0:11 /usr/local/bin/bitcoind -conf=/home/standup/.bitcoin/bitcoin.conf
| standup    24945  0.0  0.0   6520  2240 pts/0    S+   17:13   0:00 grep bitcoin
```

Then convince it to die (`kill -9`):
``` 
kill -9 24832
```

This will restart `bitcoind` with the new config file, which will now
be using mainnet instead of signet.

## Create a Descriptor

You're now ready to create a descriptor, just like you did in
[§4.2](04_2_Integrating_Addresses_Descriptors.md), except this time
you're going to be creating a wallet descriptor for a whole account.

If you review
[§3.4](3_4_Understanding_the_Descriptor_Wallet/#examine-descriptors-with-listdescriptors), you can see that WPKH descriptors look like this:

```
      "desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
      ...
      "desc": "wpkh([1bbc6484/84h/0h/0h]xpub6BfHmLzHGdGL3yXKFead4LtYqkDES6aFYMpieipj1WvdnXyvj5UZ4KYGsnAphLLjovhvrvs8u7S7zPHE1wFg4yhdp7REJRsyiDqVfQbNB8f/1/*)#827z83pg",
```

PKH descriptors look like this:
```
      "desc": "pkh([1bbc6484/44h/0h/0h]xpub6CegJzUt9VSpYTzQffpKVRcRDF9RV7XW4DdoReGXra1V3TPBVqGJ2dD9DtePRh5XzeePXZHjYBetzdccBV4jhtSdNPCNjUWS5zaNzFc1f9L/0/*)#03uc5rwf",
      ...
      "desc": "pkh([1bbc6484/44h/0h/0h]xpub6CegJzUt9VSpYTzQffpKVRcRDF9RV7XW4DdoReGXra1V3TPBVqGJ2dD9DtePRh5XzeePXZHjYBetzdccBV4jhtSdNPCNjUWS5zaNzFc1f9L/1/*)#79eefk73",
```

You need to create new descriptors in the same format.

### Put Together the Parts

Looking at that format, the first thing you need to do is remove the
`m` at the head of each account descriptor path. (They were used by
`keytool`, but not `bitcoin-cli`.)
```
ADP_NOM_WPKH=$(echo $ADP_WPKH | sed -e s'/^m//')
ADP_NOM_PKH=$(echo $ADP_PKH | sed -e s'/^m//')
```

Then you can organize all of your data into the format for these
descriptors, laying each out as a ranged descriptor:
```
DESC_WPKH="wpkh([$FINGERPRINT$ADP_NOM_WPKH]$AKEY58_WPKH/0/*)"
DESC_PKH="pkh([$FINGERPRINT$ADP_NOM_PKH]$AKEY58_PKH/0/*)"
```
Here's your WPKH descriptor:
```
echo $DESC_WPKH

| wpkh([35dad980/84h/0h/0h]xprv9z7kiySAUeGtnrbQpzqFsE8uDQufoofkLqxW2kPRcLppKx6kthUEfp4mTjyQijpcfQ5iXgYVH9EzASguJ1PNTU3YnztKGmmtBJciRU9iFre/0/*)
```

Here's your PKH descriptor:
```
echo $DESC_PKH

| pkh([35dad980/44h/0h/0h]xprv9z9awxh6i3ii4MwxeQpBedgcS5utbeZwcThsWCSeHurKrMJ2FHFnzPFscd1WhpsNT18wMctBw6KcfgzPWiGUDTPDV59czbSUPdmD6L5yogy/0/*)
```

As an aside, you can use `keytool` to produce an output descriptor if
you've uncertain about the format, but it produces a watch-only
descriptor, which means you'll have to subsituted the `xprv` for the
`xpub`, and it also doesn't show the fingerprint if you tell it a
specific account derivation path, so it's probably better to do it by
hand (and really get a good understanding of how the descriptor
works).

Nonetheless, here's an example of how an `output-descriptor` output
works for keytool using the default derivation path (which is
`84h/0h/0h`):

```
keytool --seed $SEED --address-index '*' output-descriptor

| wpkh([35dad980/84h/0h/0h]xpub6D778Uy4K1qC1Lfsw2NGEN5dmSkADGPbi4t6q8o3AgMoCkRuSEnVDcPFK28WujQRgvY47UBXJ2zJJ5ZBKELMwA3AS9Zo3yUU8XNu5dZE5As/0/*)
```

### Add the Checksum

As usual with Bitcoin Core, you need a checksum for your descriptor.

First, take a quick look at your WPKH descriptor using `getdescriptorinfo` just to be sure everything looks right.

```
bitcoin-cli getdescriptorinfo $DESC_WPKH

| {
|   "descriptor": "wpkh([35dad980/84h/0h/0h]xpub6D778Uy4K1qC1Lfsw2NGEN5dmSkADGPbi4t6q8o3AgMoCkRuSEnVDcPFK28WujQRgvY47UBXJ2zJJ5ZBKELMwA3AS9Zo3yUU8XNu5dZE5As/0/*)#p5auymld",
|   "checksum": "ac2yuqwk",
|   "isrange": true,
|   "issolvable": true,
|   "hasprivatekeys": true
| }
```

Note that these results actually include two checksums! The first one (`p5auymld`) is paired with a new copy of your descriptor that replaces the private key with the public key. You'll ignore that! The second one (`ac2yuqwk`) is the checksum for the private key descriptor that you entered.

You can use `jq` to capture the checksums for both descriptors:

```
CS_WPKH=$(bitcoin-cli getdescriptorinfo $DESC_WPKH | jq -r '.checksum')
CS_PKH=$(bitcoin-cli getdescriptorinfo $DESC_PKH | jq -r '.checksum')
```

And you can finalize the descriptors for use with Bitcoin Core:
```
DESC_WITH_CS_WPKH=$DESC_WPKH#$CS_WPKH
DESC_WITH_CS_PKH=$DESC_PKH#$CS_PKH
```

## Check Addresses

When you're moving wallets (or descriptors) between apps, you always
want to make sure that they look the same on your source and
destination systems. The best way to do that is to check addresses.

Using `bitcoin-cli`, you can check address with `deriveaddresses`, which will show
the addresses associated with a descriptor.

Here are the first three WPKH addresses on Bitcoin Core:
```
bitcoin-cli deriveaddresses $DESC_WITH_CS_WPKH 2

| [
|   "bc1q7jn4qxknxr3d58jwzjyw3x93w7qxwhkypftgaz",
|   "bc1q3jal42asrk6jpsyaxxcl5wtevjkc4xaszcq89c",
|   "bc1q96auv67f4ell2ayrgauwghhkrf50e30g8mthgy"
| ]
```

On `keytool`, you can go all the way back to the seed and use the account derivation path to output addresses of a specific type. This
is a very robust check, because it verifies that the derivation of the
account key was done correctly:

```
keytool --seed $SEED --account-derivation-path m/84h/0h/0h --address-index 0 address-segwit
| bc1q7jn4qxknxr3d58jwzjyw3x93w7qxwhkypftgaz

keytool --seed $SEED --account-derivation-path m/84h/0h/0h --address-index 1 address-segwit
| bc1q3jal42asrk6jpsyaxxcl5wtevjkc4xaszcq89c

keytool --seed $SEED --account-derivation-path m/84h/0h/0h --address-index 2 address-segwit
bc1q96auv67f4ell2ayrgauwghhkrf50e30g8mthgy
```

All three check out!

You can do the same with your PKH addresses.

Here they are in Bitcoin Core:
```
bitcoin-cli deriveaddresses $DESC_WITH_CS_PKH 2

| [
|   "1GPJoLSUCZ1Bb9KFmevNHY28hsgVoDMexH",
|   "1KUvmWCL7uCQ3FNSLpkATTTfMqpB1zcs6c",
|   "1Q48NmPqPCkqxqJwSnj1ZEja8eVRuBvJxp"
| ]
```

Here's the check, now looking for PKH addresses from `keytool`:

```
keytool --seed $SEED --account-derivation-path m/44h/0h/0h --address-index 0 address-pkh
| 1GPJoLSUCZ1Bb9KFmevNHY28hsgVoDMexH

keytool --seed $SEED --account-derivation-path m/44h/0h/0h --address-index 1 address-pkh
| 1KUvmWCL7uCQ3FNSLpkATTTfMqpB1zcs6c

keytool --seed $SEED --account-derivation-path m/44h/0h/0h --address-index 2 address-pkh
| 1Q48NmPqPCkqxqJwSnj1ZEja8eVRuBvJxp
```

## Import Addresses

You're ready to import the addresses.

First, you need to create a wallet without any descriptors of its own:
```
bitcoin-cli -named createwallet wallet_name="seed" blank=true

| {
|   "name": "seed"
| }
```

Then, you just need to import the descriptor with the `importdescriptors` command that you've used previously:

```
bitcoin-cli -rpcwallet=seed importdescriptors '''[{ "desc": "'$DESC_WITH_CS_WPKH'", "timestamp":1770329126, "active": true, "range": [0,100] }]'''

| [
|   {
|     "success": true
|   }
| ]
```

Again, you want to check an address:

```
bitcoin-cli -rpcwallet=seed getnewaddress

| bc1q7jn4qxknxr3d58jwzjyw3x93w7qxwhkypftgaz
```

You can do the same with the PKH address now:
```
bitcoin-cli -rpcwallet=seed importdescriptors '''[{ "desc": "'$DESC_WITH_CS_PKH'", "timestamp":1770329126, "active": true, "range": [0,100] }]'''

| [
|   {
|     "success": true
|   }
| ]
```

And check the address one last time:

```
bitcoin-cli -named -rpcwallet=seed getnewaddress address_type=legacy

| 1GPJoLSUCZ1Bb9KFmevNHY28hsgVoDMexH
```

## Make Change

You can now generate WPKH and PKH addresses derived from your seed using your Bitcoin Core wallet.

There's one catch that's obvious when you try to make change:

```
bitcoin-cli -rpcwallet="seed" getrawchangeaddress

| error code: -4
| error message:
| Error: This wallet has no available keys
```

This is entirely expected. As we've seen previously, the fourth digit
in a derivation path says whether an address is intended for external
usage (`0`) or for change (`1`). To make a change address you use the
same account key, but change the address derivation range from `0/*`
to `1/*.

This is easy to set up by creating a new variable for the descriptor (and its checksum) that uses `1/*` instead of `0/*` for the ranged address derivation path.

```
DESC_WPKH_C="wpkh([$FINGERPRINT$ADP_NOM_WPKH]$AKEY58_WPKH/1/*)"
CS_WPKH_C=$(bitcoin-cli getdescriptorinfo $DESC_WPKH_C | jq -r '.checksum')
DESC_WITH_CS_WPKH_C=$DESC_WPKH_C#$CS_WPKH_C
```

Once you have that, you can import. You also need to set the `internal:` value for `importdescriptors` to `true` for this to work.

```
bitcoin-cli -rpcwallet=seed importdescriptors '''[{ "desc": "'$DESC_WITH_CS_WPKH_C'", "timestamp":1770329126, "internal": true, "active": true, "range": [0,999] }]'''
[
  {
    "success": true
  }
]
```

Now you can make change:

```
bitcoin-cli -rpcwallet="seed" getrawchangeaddress

| bc1qpecala06gpe3cm3xamfnga92h3yvksvxspgdld
```

## Examining Your Wallet

If you `listdescriptors` on your seed-based wallet, you can now see three different descriptors:

```
bitcoin-cli -rpcwallet="seed" listdescriptors

| {
|   "wallet_name": "seed",
|   "descriptors": [
|     {
|       "desc": "pkh([35dad980/44h/0h/0h]xpub6D8wMUDzYRH1Gr2RkSMC1mdLz7kP17HnygdUJarFrFPJj9dAnpa3YBaMTu2qevi53aZzMtdhzM215NWeCT6pZmNWkkm9AyoPq4yb9U1ojhy/0/*)#93z28stv",
|       "timestamp": 1770329126,
|       "active": true,
|       "internal": false,
|       "range": [
|         0,
|         999
|       ],
|       "next": 1,
|       "next_index": 1
|     },
|     {
|       "desc": "wpkh([35dad980/84h/0h/0h]xpub6D778Uy4K1qC1Lfsw2NGEN5dmSkADGPbi4t6q8o3AgMoCkRuSEnVDcPFK28WujQRgvY47UBXJ2zJJ5ZBKELMwA3AS9Zo3yUU8XNu5dZE5As/0/*)#p5auymld",
|       "timestamp": 1770329126,
|       "active": false,
|       "range": [
|         0,
|         1000
|       ],
|       "next": 2,
|       "next_index": 2
|     },
|     {
|       "desc": "wpkh([35dad980/84h/0h/0h]xpub6D778Uy4K1qC1Lfsw2NGEN5dmSkADGPbi4t6q8o3AgMoCkRuSEnVDcPFK28WujQRgvY47UBXJ2zJJ5ZBKELMwA3AS9Zo3yUU8XNu5dZE5As/1/*)#sqcaew04",
|       "timestamp": 1770329126,
|       "active": true,
|       "internal": true,
|       "range": [
|         0,
|         999
|       ],
|       "next": 1,
|       "next_index": 1
|     }
|   ]
| }
```

You've now got three descriptors in your wallet: an external WPKH
address, a change WPKH address, and an eternal PKH address.

It would be trivial to keep expanding this wallet using account keys
based on your seed until you had all 8 descriptors that are in the
standard wallet created by Bitcoin Core. (The difference would be that
you would have clear control of a seed, without having to worry about
Bitcoin Core's unique file format for their wallets.)

## Summary: Importing Secrets to Bitcoin Core

In the previous section, you generated account keys for specific
derivation paths based on your seed. That was most of the battle!
Here, you just had to incorporate those keys into descriptors in order
to import them into Bitcoin Core. The result? A wallet where you have
a resilient seed that you can backup without depending on Bitcoin
Core's unique file format.

## What's Next?

Continue "Exploring the Ecosystem" with [§10.4: Exporting Secrets from
Bitcoin Core](10_4_Exporting_Secrets_from_Bitcoin_Core.md).



