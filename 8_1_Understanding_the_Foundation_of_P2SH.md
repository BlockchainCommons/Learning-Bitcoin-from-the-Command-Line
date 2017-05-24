# 8.1: Understanding the Foundation of P2SH

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You know that Bitcoin Scripts can be used to control the redemption of UTXOs. The next step is creating Scripts of your own ... but that requires a very specific techniques.

## Know the Bitcoin Standards

Here's the gotcha for using Bitcoin Scripts: for security reasons, most Bitcoin nodes will only accept five types of "standard" Bitcoin transactions.

* __Pay to Public Key (P2PK)__ — An older, deprecated transaction (`<pubKey> OP_CHECKSIG`) that has been replaced by the better security of P2PKH.
* __Pay to Public Key Hash (P2PKH)__ — A standard transaction (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) that pays to the hash of a public key.
* __Multisig__ — A transaction for a group of keys, as explained more fully in [§8.3](8_3_Scripting_a_Multisig.md).
* __Null Data__ — An unspendable transaction (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ — A transaction that pays out to a specific script, as explained more fully here.

So how do you write a more complex Bitcoin Script? The answer is in that last sort of standard transaction, the P2SH. You can put any sort of long and complex script into a P2SH transaction, and as long as you follow the standard rules for embedding your script and for redeeming the funds, you'll get the full benefits of Bitcoin Scripting.

> **VERSION WARNING:** Arbitrary P2SH scripts only became standard as of Bitcoin Core 0.10.0. Before that, only P2SH Multisigs were allowed.

## Understand the P2SH Script

You already saw a P2SH transaction when you created a multisig in [§6.1: Sending a Transaction to a Multisig](6_1_Sending_a_Transaction_to_a_Multisig.md). Though multisig is one of the standard transaction types, `bitcoin-cli` simplifies the usage of its multisigs by embedding them into P2SH transactions, as described more fully in [§8.3: Scripting a Multisig](8_3_Scripting_a_Multisig.md).

So, let's look one more time at the `scriptPubKey` of that P2SH multisig:
```
      "scriptPubKey": {
        "asm": "OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL",
        "hex": "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz"
        ]
      }
```
The locking script is quite simple looking: `OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL`. As usual, there's a big chunk of data in the middle. This is a hash of another, hidden locking script (`redeemScript`) that's embedded _within_ the P2SH. In other words, the standard locking script for a P2SH address is: `OP_HASH160 <redeemScriptHash> OP_EQUAL`.

_What is a redeemScript?_ Each P2SH transaction carries the fingerprint of a hidden locking script within it as a 20-byte hash. When a P2SH transaction is redeemed, the full (unhashed) `redeemScript` is included as part of the `scriptSig`. Bitcoin will make sure the `redeemScript` matches the hash; then it actually runs the `redeemScript` to see if the funds can be spent (or not).

One of the interesting elements of P2SH transactions is that neither the sender nor the Blockchain actually knows what the `redeemScript` is! A sender just sends to a standardized P2SH addressesd marked with a "2" prefix and they don't worry about how the recipient is going to retrieve the funds at the end.

> **TESTNET vs MAINNET:** Reminder: on testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

## Understand How to Build a P2SH Script

Since the visible locking script for a P2SH transaction is so simple, creating a transaction of this sort is quite simple too. In theory. All you need to do is create a transaction whose locking script includes a 20-byte hash of the `redeemScript`. That hashing is done with Bitcoin's standard `OP_HASH160`.

Overall, just four steps are required:

1. Create an arbitrary locking script with Bitcoin Script.
2. Create a serialized version of that locking script.
3. Perform a SHA-256 hash on those serialized bytes.
4. Perform a RIPEMD-160 hash on the results of that SHA-256 hash.

_What is OP_HASH160?_ The standard hash operation for Bitcoin performs a SHA-256 hash, then a RIPEMD-160 hash.

Each of those steps of course takes some work on its own.

### Create a Locking Script

This is the subject of chapters 7-10. You can use any of the Bitcoin Script methods described therein to create any sort of locking script, as long as the resultant serialized `redeemScript` is 520 bytes or less. 

_Why are P2SH scripts limited to 520 bytes?_ As with many things in Bitcoin, the answer is backward compatibility: new functionality has to constantly be built within the old constraints of the system. Is this case, 520 bytes is the maximum that can be pushed onto the stack at once. Since the whole redeemScript is pushed onto the stack as part of the redemption process, it hits that limit.

### Serialize a Locking Script

Serializing a locking script is a two-part process. First, you must turn it into hexcode, then you must transform that hex into binary.

#### Create the Hex Code

Creating the hexcode that is necessary to serialize a script is both a simple translation and something that's complex enough that it goes beyond any shell script that you're likely to write. As with a few other aspects of P2SH scripts, it's something that you'll probably process through an API, not by hand.

You create hexcode by stepping through your locking script and turning each element into one-byte command, possibly followed by additional data, per the guide at the [Bitcoin Wiki Script page](https://en.bitcoin.it/wiki/Script):

* The constants 1-16 are translated to 0x51 to 0x61 (OP_1 to OP_16)
* The constant -1 is translate to 0x4f (OP_1NEGATE)
* Larger constants are translated into 0x01 to 0x4e (OP_PUSHDATA, including a specification of how many bytes to push)
* Operators are translated to the matching byte for that opcode

The constants are the most troublesome part. Often you'll need to translate decimals into hexidecimals. This can be done with the `printf` command:
```
$ decimal=1546288031
$ hex=(printf '%x\n' $decimal)
$ echo $hex
5c2a7b9f
```
And you'll always need to know the size of your constants. You can just remember that every two hexidecimal characters is one byte. Or, you can use `echo -n` piped to `wc -c`, and divide that in two:
```
user1@blockstream:~$ echo -n $hex | wc -c
8
```
#### Create the Hex Code: A Multisig Example

It may be easier to understand this by taking an existing hexcode and translating it back to Bitcoin Script. For example, look at the `redeemScript` that you used [§6.1](6_1_Sending_a_Transaction_to_a_Multisig.md):
```
52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae
```
You can translate this back to Script by hand using the [Bitcoin Wiki Script page](https://en.bitcoin.it/wiki/Script) as a reference. Just look at one byte (two hex characters) of data at a time, unless you're told to look at more by an OP_PUSHDATA command (0x01 to 0x4e).

The whole Script will break apart as follows:
```
52 / 21 / 0307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819 / 21 / 0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28 / 52 / ae
```
Here's what the individual parts mean:

* 0x52 = OP_2
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x0307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819 = the next 33 bytes (public-key hash)
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28 = the next 33 bytes (public-key hash)
* 0x52 = OP_2
* 0xae = OP_CHECKMULTISIG

In other words, that `redeemScript` was a translation of of "2 0307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819 0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28 2 OP_CHECKMULTISIG" 

If you'd like a mechanical hand with this sort of translation in the future, you can use `bitcoin-cli decodescript`:
```
$ bitcoin-cli -named decodescript hexstring=52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae
{
  "asm": "2 0307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819 0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28 2 OP_CHECKMULTISIG",
  "reqSigs": 2,
  "type": "multisig",
  "addresses": [
    "mg7YqyvK8HUFvpgZ5iYTfZ5vjfaJWnNTd9", 
    "mfduLxpR6Bq1ARctV2TauhetWwqnqH1vYS"
  ],
  "p2sh": "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz"
}
```
Also consider the Python [Transaction Script Compiler](https://github.com/Kefkius/txsc), which translates back and forth.

_If this is intimidating, don't worry about it; as we said, this will usually be done with an API. As we lay bare the foundation of P2SH Scripting, we're mostly explaining what those APIs will do._

#### Transform the Hex to Binary

Once you've got hexcode, you can finish the serialization by turning it into binary. On the command line, this just requires a simple invocation of `xxd -r -p`, however you probably want to do that as part of a a single pipe that will also hash the script ...

### Hash a Serialized Script

As we noted, a 20-byte OP_HASH160 hash is created through a combination of a SHA-256 hash and a RIPEMD-160 hash. Hashing a serialized script thus takes two commands: `openssl dgst -sha256 -binary` does the SHA-256 hash and outputs a binary to be sent through the pipe, then `openssl dgst -rmd160` takes that binary stream, does a RIPEMD-160 hash, and finally outputs a human-readable hexcode.

Here's the whole pipe:
```
$ echo -n "52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae" | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= babf9063cee8ab6e9334f95f6d4e9148d0e551c2
```

## Understand How to Send a P2SH Script Transaction

So how do you actually send your P2SH transaction? That's unfortunately another place where you probably need an API, because `bitcoin-cli` doesn't provide any support for sending P2SH transactions.

Again, however, the theory is very simple:

1. Embed your hash in a `OP_HASH160 <redeemScriptHash> OP_EQUAL` script.
2. Translate that into hex, using the same method as above: `a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287`.
3. Use that hex as your `scriptPubKey`. 
4. Create the rest of the transaction.

Note that a P2SH Script transaction will _always_ start with an `a914`, which is the OP_HASH160 followed by an OP_PUSHDATA of 20 bytes (hex: 0x14); and it will _always_ end with a `87`, which is an OP_EQUAL. So all you have to do is put your hashed redeem script in between those numbers.

## Understand How to Unlock a P2SH Script Transaction

The trick to redeeming a P2SH transaction is that the recipient must have saved the secret serialized locking script that was hashed to create the P2SH address. This is called a `redeemScript` because it's what the recipient needs to redeem his funds. 

An unlocking `scriptSig` for a P2SH transaction is formed as: `... data ... <redeemScript>`. The `data` must _solely_ be data that is pushed onto the stack, not operators. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) calls them signatures, but that's not an actual requirement.)

When a UTXO is redeemed, it runs in two rounds of verification:

1. First, the redeemScript in the `scriptSig` is hashed and compared to the hashed script in the `scriptPubKey`. 
2. If they match, then a second round of verification begins.
3. Second, the redeemScript is run using the prior data that was pushed on the stack. 
4. If that second round of verification _also_ succeeds, the UTXO is unlocked.

> **WARNING:** You can create a perfectly valid transaction with a hashed redeemScript, but if the redeemScript doesn't run, or doesn't run correctly, your funds are lost forever. So, test, test, test the script!

## Summary: Understanding the Foundation of P2SH

Arbitrary Bitcoin Scripts are non-standard in Bitcoin. However, you can incorporate them into standard transactions by using the P2SH address type. You just hash your script as part of the locking script, then you reveal and run it as part of the unlocking script. As long as you can also satisfy the script, the UTXO can be spent. Mind you, this is all somewhat more theoretical than previous sections, because it isn't easy to create redeemScripts by hand, nor is it possible to incorporate them into transactions using `bitcoin-cli`. 

_What is the power of P2SH?_ You already know the power of Bitcoin Script, which allows you to create more complex Smart Contracts of all sorts. P2SH is what actually unleashes that power by letting you include arbitrary Bitcoin Script in standard Bitcoin transactions.
