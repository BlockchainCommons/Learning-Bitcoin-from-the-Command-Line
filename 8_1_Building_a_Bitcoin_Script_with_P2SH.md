# 8.1: Building a Bitcoin Script with P2SH

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You know that Bitcoin Scripts can be used to control the redemption of UTXOs, and that standard Bitcoin transactions all depend on very specific locking scripts. The next step is creating Scripts of your own ... but that requires a very specific techniques.

## Know the Bitcoin Standards

Here's the gotcha for using Bitcoin Scripts: for security reasons, Bitcoin nodes will only accept five types of "standard" Bitcoin transactions:

* __Pay to Public Key (P2PK)__ — An older, deprecated form of transaction (`<pubKey> OP_CHECKSIG`) that has been replaced by the better security of P2PKH.
* __Pay to Public Key Hash (P2PKH)__ — A standard transaction (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) on the Bitcoin network that pays to the hash of a public key.
* __Multisig__ — A transaction for a group of keys, as explained more fully in the next section.
* __Null Data__ — An unspendable transaction (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ — A transaction that pays out to a specific script.

So how do you write a more complex Bitcoin Script? The answer is in that last sort of standard transaction, the P2SH. You can put any sort of long and complex script into a P2SH transaction, and as long as you follow the standard rules for embedding your script and for redeeming the funds, you'll get all the benefits of Bitcoin Scripting.

## Lock a P2SH Transaction

You already saw a P2SH transaction when you created a multisig in [6.1: Sending a Transaction to a Multisig](6_1_Sending_a_Transaction_to_a_Multisig.md). Though multisig is one of the standard transaction type, `bitcoin-cli` actually simplifies the usage of its multisigs by embedding them into P2SH transactions, as described more fully in the next section.

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
The locking script is quite simple looking: `OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL`. As usual, there's a big chunk of data in the middle. This is a hash of the script that is required to free up the funds, meaing that the standard locking script for a P2SH address is: `OP_HASH160 <scriptHash> OP_EQUAL`.

One of the interesting elements of P2SH transactions is that neither the sender nor the Blockchain actually knows what the script is! A sender just sends to a standardized P2SH addressesd marked with a "2" prefix and they don't worry about how the recipient is going to retrieve the funds at the end.

> **TESTNET vs MAINNET:** Reminder: on testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

### Build a P2SH Script

Since the locking script for a P2SH transaction is so simple, creating a transaction of this sort is quite simple too. In theory. All you need to do is create a transaction that has a 20-byte hash of the Bitcoin locking script. The hashing is done with Bitcoin's standard OP_HASH160, which means that the following three steps are required:

1. Create a serialized version of your locking script.
2. Perform a SHA-256 hash on these serialized bytes.
3. Perform a RIPEMD-160 hash on the results of the SHA-256 hash.

_What is OP_HASH160?_ The standard hash operation for Bitcoin performs a SHA-256 hash, then a RIPEMD-160 hash.

These steps can all be easily performed through a Bash script. However, the process of getting that hash (and thus the P2SH locking conditions) into a transaction are still pretty cumbersome, as it's not easily support by the `bitcoin-cli` tool. A more precise step-by-step guide is detailed in [8.4: Executing a Bitcoin Script](8_4_Executing_a_Bitcoin_Script.md).

## Unlock a P2SH Script Transaction

The trick to redeeming a P2SH transaction is that the recipient must have saved the serialized locking script that was hashed to create the P2SH address. This is known as the `redeemScript`, because it's what the recipient will need to redeem his funds. The unlocking `scriptSig` is then formed as: `... data ... <serializedLockingScript>`. The `data` must _solely_ be data that is pushed onto the stack, not operators. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) just calls them signatures.)

When a UTXO is redeemed, the redeemScript in the `scriptSig` is hashed and compared to that in the `scriptPubKey`. If they match, then a whole second round of verification begins where the redeemScript is run using the other data that was pushed on the stack. If that second round of verification _also_ succeeds, then the UTXO is unlocked.

> **WARNING:** You can create a perfectly valid transaction with a hashed redeemScript, but if the redeemScript doesn't run, or doesn't run correctly, your funds are lost forever.

## Run a Sample P2SH Script

In [7.2: Running a Bitcoin Script](7_2_Running_a_Bitcoin_Script.md), we offered a simple example of a non-standard locking script, `OP_ADD 100 OP_EQUAL`, and we detailed how to run it as a simple concatenation of an unlocking script with that locking script. We're now going to repeat that exercise, this time within the constraints of a P2SH transaction.

### Lock a P2SH Transaction

To lock this transaction requires the following:

1. Serialize `OP_ADD 100 OP_EQUAL` (`<serializedHundredEqual>`) then SHA-256 and RIPEMD-160 hash it (`<hashedHundredEqual>`).
2. Save `<serializedHundredEqual>` for future reference as the `redeemScript`.
3. Produce a P2SH locking script that includes the hashed script (`OP_HASH160 <hashedHundredEqual> OP_EQUAL`).
4. Create the transaction with that `scriptPubKey`.

### Unlock a P2SH Transaction: Round One

To unlock this transaction requires that the recipient produce a `scriptSig` that prepends two constants totalling one hundred to the serialized script: `1 99 <serializedHundredEqual>`.

The validation to unlock the P2SH transaction then begins with a first round of validation, which concatenates `scriptSig` and `scriptPubKey` as normal:
```
Script: 1 99 <serializedHundredEqual> OP_HASH160 <hashedHundredEqual> OP_EQUAL
Stack: []

Script: 99 <serializedHundredEqual> OP_HASH160 <hashedHundredEqual> OP_EQUAL
Stack: [ 1 ]

Script: <serializedHundredEqual> OP_HASH160 <hashedHundredEqual> OP_EQUAL
Stack: [ 1 99 ]

Script: OP_HASH160 <hashedHundredEqual> OP_EQUAL
Stack: [ 1 99 <serializedHundredEqual> ]

Script: <hashedHundredEqual> OP_EQUAL
Stack: [ 1 99 <hashedHundredEqual> ]

Script: OP_EQUAL
Stack: [ 1 99 <hashedHundredEqual> <hashedHundredEqual> ]

Script: 
Stack: [ 1 99 True ]
```
The Script ends with a `True` on top of the stack, and so it succeeds ... even though there's other cruft below it.

However, because this was a P2SH script, the execution isn't done. 

### The Second Round of Validation

For the second round of validation, Bitcoin now runs the actual redeemScript against the items pushed onto the stack ahead of it:

```
Script: OP_ADD 100 OP_EQUAL
Stack: [ 1 99 ]

Script: 100 OP_EQUAL
Stack: [ 100 ]

Script: OP_EQUAL
Stack: [ 100 100 ]

Script: 
Stack: [ True ]
```
With that second validation _also_ true, the UTXO can now be spent!

## Summary: Building a Bitcoin Script with P2SH

Arbitrary Bitcoin Scripts are non-standard in Bitcoin. However, you can incorporate them into standard transactions by using the P2SH address type. You just hash your script as part of the locking script, then you reveal and run it as part of the redemption script. As long as you can also satisfy the script, the UTXO can be spent.
