# 8.2: Creating Multisig Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Ever since [§6.1: Sending a Transaction to a Multisig](6_1_Sending_a_Transaction_to_a_Multisig.md), we've been saying that the `bitcoin-cli` interface wraps its multisig transaction in a P2SH transaction. In fact, this is the standard methodology for creating multisigs on the Blockchain. This section explains it all in depth.

## Create a Multisig Transaction

Multisig transactions are created in Bitcoin using the `OP_CHECKMULTISIG` code. 

`OP_CHECKMULTISIG` when used in a locking script expects arguments as follows: `<m> ... sigs ... <n> OP_CHECKMULTISIG`. When OP_CHECKMULTISIG is run, it pops the first value from the stack (`<n>`), then it pops "n" values as public-key-hash addresses, then it pops the next value from the stack (`<m>`). Afterward, OP_CHECKMULTISIG pops "m" values as potential signatures, then it pops one more value from the stack due to a mistake in the original coding; these should all be supplied by the unlocking script. Finally, it compares the potential signatures to the Bitcoin addresses, and if they match, it pushes `TRUE`, else it returns a `FALSE`. Whew!

_What is a consensus rule?_ These are the rules that the Bitcoin nodes follow to work together. In large part they're defined by the Bitcoin Core code. These rules include lots of obvious mandates, such as the limit to how many Bitcoins are created for each block and the rules for how transactions may be respent. However, they also include fixes for bugs that have appeared over the years, because once a bug has been introduced into the Bitcoin codebase, it must be continually supported, lest old Bitcoins become unspendable. The extra-pop bug in OP_CHECKMULTISIG is one of the most obvious consensus rules of this sort. Because the original version of OP_CHECKMULTISIG accidentally popped an extra item off the stack, Bitcoin must forever follow that rule, lest complex redemption scripts accidentally be broken, rendering coins unredeemable. Recent versions of Bitcoin core require that leading constant, which is not used, be a "0".

### Lock a Multisig Transaction

The multisig in §6.1 was a 1-of-2 multisig built from `$address1` and `$address2`. the resultant `scriptPubKey` would appear as follows:
```
1 $address1 $address2 2 OP_CHECKMULTISIG
```
This is the form of multisig that is allowed as a standard locking script ... but which isn't usually used any more.

> **WARNING:** For classic `OP_CHECKMULTISIG` signatures, "n" must be ≤ 3 for the transaction to be standard.

### Unlock a Multisig Transaction

The `scriptSig` for a multisig address simply requires the submission of the missing operands for OP_CHECKMULTISIG: a "0" followed by "m" signatures. Either of the following would work for our example:
```
0 $signature1
```
Or:
```
0 $signature2
```
The script would then be run as follows:
```
Script: 0 $signature1 1 $address1 $address2 2 OP_CHECKMULTISIG
Stack: [ ]
```
All the constants are put on the stack:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 1 $address1 $address2 2 ]
```
Then, the "2" is popped:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 1 $address1 $address2 ]
```
Then, the "2" tells `OP_CHECKMULTISIG `to pop two addresses:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 1 ]
```
Then, the "1" is popped:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 ]
```
Then, the "1" tells `OP_CHECKMULTISIG` to pop one signature:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 ]
```
Then, one more item is mistakenly popped:
```
Script: OP_CHECKMULTISIG
Stack:
```
Then, `OP_CHECKMULTISIG` completes its operation by comparing the "m" signatures to the "n" addresses:
```
Script:
Stack: [ True ]
```
### Understand the Limitations of Multisig

Unfortunately, the technique of embedding the entire multisig into a transaction has some notable drawbacks:

1. There's no standard address format for multisigs, which means that each sender has to: enter a long and cumbersome multisig script; have software that allows this; and be trusted not to mess it up.
2. Because multisigs can be much longer than typical locking scripts, they add more data to the blockchain. This requires higher transaction fees from the sender and creates more nuisance for every node.

These were generally problems with using arbitrary locking scripts for Bitcoin, but they quickly became very real problems when applied to multisigs, because they were some of the first complex scripts to be widely used on the Bitcoin network.

## Create a Multisig Transaction with P2SH

P2SH transactions were created to solve the very real problems of complex Bitcoin Scripts. The idea was laid out in 2012 and immediately applied to multisigs, resulting in "P2SH multisigs", which simply package up a standard multisig transaction into a standard P2SH transaction. This allows for address standardization; reduced data storage; and increased "m" and "n" counts.

The technique accidentally follows the description in [8.1: Building a Bitcoin Script with P2SH](8_1_Building_a_Bitcoin_Script_with_P2SH.md), offering a pratical first example of how P2SHes really work. Here's a look at how our example multisig would work:

### Create the Lock for the P2SH Multisig Transaction

1. Serialize `1 $address1 $address2 2 OP_CHECKMULTISIG` (`<serializedMultiSig>`) then SHA-256 and RIPEMD-160 hash it (`<hashedMultisig>`).
2. Save `<serializedMultiSig>` for future reference as the `redeemScript`.
3. Produce a P2SH Multisig locking script that includes the hashed script (`OP_HASH160 <hashedMultisig> OP_EQUAL`).
4. Create a transaction using that `scriptPubKey`.

### Run the First Round of P2SH Validation

1. Validate `0 $signature1 <serializedMultiSig> OP_HASH160 <hashedMultisig> OP_EQUAL` or `0 $signature1 <serializedMultiSig> OP_HASH160 <hashedMultisig> OP_EQUAL`

### Run the Second Round of P2SH Validation

1. Deserialize `<serializedMultiSig>`
2. Validate `0 $signature1 1 $address1 $address2 2 OP_CHECKMULTISIG` or `0 $signature2 1 $address1 $address2 2 OP_CHECKMULTISIG`

And now you know how the multisig transaction in §6.1 was actually created, and how it was actually validated for spending.

## Summary: Creating Multisig Scripts

Multisigs are a standard transaction type, but they're a bit cumbersome to use, so they're regularly incorporated in P2SH transactions, as they were in §6.1, when we created our first multisigs. The result is cleaner, smaller, and more standardized — but more importantly, it's also a great real-world example of how P2SH scripts really work.
