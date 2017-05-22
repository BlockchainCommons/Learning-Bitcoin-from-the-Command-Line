# 8.1: Building a Bitcoin Script with P2SH

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You know that Bitcoin Scripts can be used to control the redemption of UTXOs, and that standard Bitcoin transactions depend on very specific locking scripts. The next step is creating Scripts of your own ... but that requires a very specific techniques.

## Know the Bitcoin Standards

Here's the gotcha for using Bitcoin Scripts: for security reasons, most Bitcoin nodes will only accept five types of "standard" Bitcoin transactions.

* __Pay to Public Key (P2PK)__ — An older, deprecated transaction (`<pubKey> OP_CHECKSIG`) that has been replaced by the better security of P2PKH.
* __Pay to Public Key Hash (P2PKH)__ — A standard transaction (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) that pays to the hash of a public key.
* __Multisig__ — A transaction for a group of keys, as explained more fully in the next section.
* __Null Data__ — An unspendable transaction (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ — A transaction that pays out to a specific script.

So how do you write a more complex Bitcoin Script? The answer is in that last sort of standard transaction, the P2SH. You can put any sort of long and complex script into a P2SH transaction, and as long as you follow the standard rules for embedding your script and for redeeming the funds, you'll get all the benefits of Bitcoin Scripting.

> **VERSION WARNING:** Arbitrary (non-standard) P2SH scripts only became standard as of Bitcoin Core 0.10.0. Before that, only P2SH Multisigs were allowed.

## Lock a P2SH Transaction

You already saw a P2SH transaction when you created a multisig in [§6.1: Sending a Transaction to a Multisig](6_1_Sending_a_Transaction_to_a_Multisig.md). Though multisig is one of the standard transaction type, `bitcoin-cli` actually simplifies the usage of its multisigs by embedding them into P2SH transactions, as described more fully in the next section.

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
The locking script is quite simple looking: `OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL`. As usual, there's a big chunk of data in the middle. This is a hash of the locking script that's embedded _within_ the P2SH, which is the hidden locking script that's required to redeem the funds. This means that the standard (visible) locking script for a P2SH address is: `OP_HASH160 <redeemScriptHash> OP_EQUAL`.

One of the interesting elements of P2SH transactions is that neither the sender nor the Blockchain actually knows what the redeem script is! A sender just sends to a standardized P2SH addressesd marked with a "2" prefix and they don't worry about how the recipient is going to retrieve the funds at the end.

> **TESTNET vs MAINNET:** Reminder: on testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

### Build a P2SH Script

Since the visible locking script for a P2SH transaction is so simple, creating a transaction of this sort is quite simple too. In theory. All you need to do is create a transaction that has a 20-byte hash of the Bitcoin locking script. The hashing is done with Bitcoin's standard OP_HASH160, which means that the following three steps are required:

1. Create a serialized version of your locking script.
2. Perform a SHA-256 hash on these serialized bytes.
3. Perform a RIPEMD-160 hash on the results of the SHA-256 hash.

_What is OP_HASH160?_ The standard hash operation for Bitcoin performs a SHA-256 hash, then a RIPEMD-160 hash.

Each of those steps of course takes some work on their own.

#### Serialize a Locking Script

Here's the thing: you're probably never going to do this by hand, and you probably won't even be able to do it from the shell. The reason is that simple sounding "serialize" step. This isn't some easy conversion, like running ascii-to-binary. Instead, it's a step-by-step process of translating each element of the script to a nibble of data that represents either an opcode or part of the data that's being pushed onto the stack by an opcode.

For example, look at the `redeemScript` that you used [§6.1](6_1_Sending_a_Transaction_to_a_Multisig.md):
```
52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae
```
You can deserialize this by hand using the [Bitcoin Wiki Script page](https://en.bitcoin.it/wiki/Script) or by using the handy tool at [Chain Query](https://chainquery.com/bitcoin-api/decodescript). Just look at one byte (two hex characters) of data at a time, unless you're told to look at more by an OP_PUSHDATA command (0x01 to 0x78):

* 0x52 = OP_2
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x0307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819 = 33 bytes of data, the first public-key hash
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28 = 33 bytes of data, the second public-key hash
* 0x52 = OP_2
* 0xae = OP_CHECKMULTISIG

In other words, that redeemScript was a serialization of of "2 0307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819 0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28 2 OP_CHECKMULTISIG" ... but creating that serialization would take a whole compiler. 

This is going to be the first of several tasks regarding P2SH transactions that will require a larger API. But for now, you have the theory: the locking script needs to be serialized before it can be used, and that serialization involves more complexity than you can manage by hand or by shell script.

#### Hash a Serialized Script

[((]

## Send a P2SH Script Transaction

[((]

## Unlock a P2SH Script Transaction

The trick to redeeming a P2SH transaction is that the recipient must have saved the secret serialized locking script that was hashed to create the P2SH address. This is known as the `redeemScript`, because it's what the recipient will need to redeem his funds. The unlocking `scriptSig` is then formed as: `... data ... <serializedLockingScript>`. The `data` must _solely_ be data that is pushed onto the stack, not operators. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) calls them signatures, but that's not an actual requirement.)

When a UTXO is redeemed, the redeemScript in the `scriptSig` is hashed and compared to that in the `scriptPubKey`. If they match, then a whole second round of verification begins where the redeemScript is run using the other data that was pushed on the stack. If that second round of verification _also_ succeeds, then the UTXO is unlocked.

> **WARNING:** You can create a perfectly valid transaction with a hashed redeemScript, but if the redeemScript doesn't run, or doesn't run correctly, your funds are lost forever. So, test, test, test.

## Rebuild a Script as a P2SH

In [§7.2: Running a Bitcoin Script](7_2_Running_a_Bitcoin_Script.md), we offered a simple example of a non-standard locking script, `OP_ADD 99 OP_EQUAL`, and we executed it as a simple concatenation of an unlocking script with that locking script. We're now going to repeat that exercise, this time within the constraints of a P2SH transaction.

### Create the Lock for the P2SH Transaction

To lock this transaction do the following:

1. Serialize `OP_ADD 99 OP_EQUAL` (`<serialized99Equal>`) then SHA-256 and RIPEMD-160 hash it (`<hashed99Equal>`).
2. Save `<serialized99Equal>` for future reference as the `redeemScript`.
3. Produce a P2SH locking script that includes the hashed script (`OP_HASH160 <hashed99Equal> OP_EQUAL`).
4. Create a transaction using that `scriptPubKey`.

### Run the First Round of Validation

To unlock this transaction requires that the recipient produce a `scriptSig` that prepends two constants totalling ninety-nine to the serialized script: `1 98 <serialized99Equal>`.

The validation to unlock the P2SH transaction then begins with a first round of validation. Concatenate `scriptSig` and `scriptPubKey` and execute them, as normal:
```
Script: 1 98 <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: []

Script: 98 <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 ]

Script: <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 98 ]

Script: OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 98 <serialized99Equal> ]

Script: <hashed99Equal> OP_EQUAL
Stack: [ 1 98 <hashed99Equal> ]

Script: OP_EQUAL
Stack: [ 1 98 <hashed99Equal> <hashed99Equal> ]

Script: 
Stack: [ 1 98 True ]
```
The Script ends with a `True` on top of the stack, and so it succeeds ... even though there's other cruft below it.

However, because this was a P2SH script, the execution isn't done. 

### Run the Second Round of Validation

For the second round of validation, deserialize the `redeemScript`, then execute it using the items in the `scriptSig` before the serialized script:

```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: [ ]

Script: 98 OP_ADD 99 OP_EQUAL
Stack: [ 1 ]

Script: OP_ADD 99 OP_EQUAL
Stack: [ 1 98 ]

Script: 99 OP_EQUAL
Stack: [ 99 ]

Script: OP_EQUAL
Stack: [ 99 99 ]

Script: 
Stack: [ True ]
```
With that second validation _also_ true, the UTXO can now be spent!

## Summary: Building a Bitcoin Script with P2SH

Arbitrary Bitcoin Scripts are non-standard in Bitcoin. However, you can incorporate them into standard transactions by using the P2SH address type. You just hash your script as part of the locking script, then you reveal and run it as part of the redemption script. As long as you can also satisfy the script, the UTXO can be spent. 

_What is the power of P2SH?_ You already know the power of Bitcoin Script, which allows you to create more complex Smart Contracts of all sorts. P2SH is what actually unleashes that power by letting you include arbitrary Bitcoin Script in standard Bitcoin transactions.
