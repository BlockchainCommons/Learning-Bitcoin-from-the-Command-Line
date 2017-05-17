# 8.1: Building a Bitcoin Script with P2SH

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You know that Bitcoin Scripts can be used to control the redemption of UTXOs, and that standard Bitcoin transactions all depend on very specific locking scripts. The next step is starting to create Scripts of your own ... but that requires a very specific techniques.

## Know the Bitcoin Standards

Here's the catch for Bitcoin Scripts: for security reasons, Bitcoin nodes will only accept five types of "standard" Bitcoin transactions:

* __Pay to Public Key (P2PK)__ — This old deprecated form of transaction (`<pubKey> OP_CHECKSIG`) has been replaced by the better security of P2PKH.
* __Pay to Public Key Hash (P2PKH)__ — A standard transaction (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) on the Bitcoin network, that pays to the hash of a public key.
* __Multisig__ — A transaction for a group of keys, as explained more fully in the next section.
* __Null Data__ — An unspendable transaction (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ — A transaction that pays out to a specific script

So how do you write a more complex Bitcoin Script? The trick is in that last sort of standard transaction, the P2SH. You can put any sort of long and complex script into a P2SH transaction, and as long as you follow the standard rules for embedding your script and for redeeming the funds, you'll get all the benefits of Bitcoin Scripting.

Besides providing security, embedding all non-standard Bitcoin Scripts as standard P2SHs also provides dramatically improved standardization, making it easy to acess and use more complex Scripts of this sort.

## Lock a P2SH Transaction

You've already seen a P2SH transaction. As we'll explain more fully in the next section, `bitcoin-cli` embeds its multisigs into P2SH. So, your multisig transaction from earlier chapters provides a full example of an locking script for a P2SH transaction:
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
As usual, there's a big chunk of data in the middle. This is a hash of the script that is required to free up the funds, meaing that the standard locking script for a P2SH address is: `OP_HASH160 <scriptHash> OP_EQUAL`.

This means that neither the sender nor the Blockchain actually knows what the script is! They just send to a standardized P2SH addressesd marked with a "2" prefix and they don't worry about how the receiving is going to retrieve the funds at the end.

> **TESTNET vs MAINNET:** Reminder: on testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

### Hash a P2SH Script

Creating a P2SH transaction is pretty simple in theory. All you need to do is create a transaction that has a 20-byte hash of the Bitcoin locking script. The hashing is done with Bitcoin's standard OP_HASH160, which means that the following three steps are required:

1. Create a serialized version of your locking script.
2. Perform a SHA-256 hash on these serialized bytes.
3. Perform a RIPEMD-160 hash on the results of the SHA-256 hash.

_What is OP_HASH160?_ The standard hash operation for Bitcoin performs a SHA-256 hash, then a RIPEMD-160 hash.

These steps can all be easily performed through a Bash script. However, the process of getting that hash (and the P2SH locking conditions) into a transaction are still pretty cumbersome, as it's not easily support by the `bitcoin-cli` tool. The precise steps required are all discussed in [8.4: Executing a Bitcoin Script](8_4_Executing_a_Bitcoin_Script.md).

## Unlock a P2SH Script Transaction

The trick to redeeming a P2SH transaction is saving the serialized locking script that was hashed to create the P2SH address. This is known as the `redeemScript`, because it's what the recipient will need to redeem his funds. The unlocking script is then formed as: `... data ... <serializedLockingScript>`. The data must _solely_ be data that is pushed onto the stack, not operators. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) just calls them signatures.)

When a UTXO is redeemed, the redeemScript in the `scriptSig` is hashed and compared to that in the `scriptPubKey`. If they match, then a whole secondary sequence of verification begins where the redeemScript is run using the redeemScript and the other data that was pushed on the stack. If that second verification _also_ succeeds, then the UTXO is unlocked.

## Run an Example

In [7.2: Running a Bitcoin Script](7_2_Running_a_Bitcoin_Script.md), we offered a simple example of a non-standard locking script. Here we vary is slight as `OP_ADD 100 OP_EQUAL`. To unlock the transaction requires two constants that add to a hundred.

To actually lock this transaction would require the following:

1. Serialize `OP_ADD 100 OP_EQUAL` (`<serializedHundredEqual>`) then SHA-256 and RIPEMD-160 hash it (`<hashedHundredEqual>`).
2. Produce a P2SH locking script that includes the hashed script (`OP_HASH160 <hashedHundredEqual> OP_EQUAL`).
3. Create the transaction.

To unlock it requires:

4. Produce an unlocking script that prepends the constants to the serialized script (`99 1 <serializedHundredEqual>`).

### The First Round of Validation

The validation to unlock the P2SH then begins as follows:

The first validation checks the hash of the script:
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
The Script ends with a True on top of the stack, and so it succeeds ... even though there's other cruft below it.

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

Arbitrary Bitcoin scripts are standard in Bitcoin. However, you can incorporate them using the P2SH address type. You just hash your script as part of the locking script, then you reveal it as part of the redemption script. As long as you can also satisfy the script, the UTXO can be spent.
