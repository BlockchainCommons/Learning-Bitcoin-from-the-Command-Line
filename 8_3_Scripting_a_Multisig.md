# 8.3: Scripting a Multisig

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Before we close out this intro to P2SH scripting, it's worth examining an even more notable example. Ever since [§6.1: Sending a Transaction to a Multisig](6_1_Sending_a_Transaction_to_a_Multisig.md), we've been casually saying that the `bitcoin-cli` interface wraps its multisig transaction in a P2SH transaction. In fact, this is the standard methodology for creating multisigs on the Blockchain. Here's how that works, in depth.

## Understand the Multisig Code

Multisig transactions are created in Bitcoin using the `OP_CHECKMULTISIG` code. 

`OP_CHECKMULTISIG` expects a long string of arguments that looks like this: `0 ... sigs ... <m> ... addresses ... <n> OP_CHECKMULTISIG`. When `OP_CHECKMULTISIG` is run, it does the following:

1. Pop the first value from the stack (`<n>`).
2. Pop "n" values from the stack as Bitcoin addresses (hashed public keys).
3. Pop the next value from the stack (`<m>`).
4. Pop "m" values from the stack as potential signatures.
5. Pop an `0` from the stack due to a mistake in the original coding.
6. Compare the signatures to the Bitcoin adddresses.
7. Push a `True` or `False` depending on the result.

The operands of `OP_MULTISIG` are typically divided, with the `0` and the signatures coming from the unlocking script and the "m", "n", and addresses being detailed by the locking script.

The requirement for that `0` as the first operand for `OP_CHECKMULTISIG` is a consensus rule. Because the original version of `OP_CHECKMULTISIG` accidentally popped an extra item off the stack, Bitcoin must forever follow that standard, lest complex redemption scripts from that time period accidentally be broken, rendering old funds unredeemable. 

_What is a consensus rule?_ These are the rules that the Bitcoin nodes follow to work together. In large part they're defined by the Bitcoin Core code. These rules include lots of obvious mandates, such as the limit to how many Bitcoins are created for each block and the rules for how transactions may be respent. However, they also include fixes for bugs that have appeared over the years, because once a bug has been introduced into the Bitcoin codebase, it must be continually supported, lest old Bitcoins become unspendable. 

## Create a Raw Multisig 

As discussed in [§8.1: Building a Bitcoin Script with P2SH](8_1_Building_a_Bitcoin_Script_with_P2SH.md), multisigs are one of the standard Bitcoin transaction types. A transaction can be created with a locking script that uses the raw `OP_CHECKMULTISIG` command, and it will be accepted into a block. This is the classic methodology for using multisigs in Bitcoin.

As an example, we will revisit the multisig created in [§6.1](6_1_Sending_a_Transaction_to_a_Multisig.md) one final time and build a new locking script for it using this methodology. As you may recall, that was a 2-of-2 multisig built from `$address1` and `$address2`. 

An `OP_CHECKMULTISIG` locking script requires the "m" (`2`), the addresses, and the "n" (`2`). You can write the following `scriptPubKey`:
```
2 $address1 $address2 2 OP_CHECKMULTISIG
```
If this looks familiar, that's because it's the multisig that you deserialized in [§8.1](8_1_Building_a_Bitcoin_Script_with_P2SH.md).
```
2 0307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819 0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28 2 OP_CHECKMULTISIG
```

> **WARNING:** For classic `OP_CHECKMULTISIG` signatures, "n" must be ≤ 3 for the transaction to be standard.

## Unlock a Raw Multisig

The `scriptSig` for a standard multisig address must then submit the missing operands for `OP_CHECKMULTISIG`: a `0` followed by "m" signatures. You could submit either of the following:
```
0 $signature1 signature2
```

### Run a Raw Multisig Script 

In order to reuse the multisig UTXO, run the `scriptSig` and `scriptPubKey` as follows:
```
Script: 0 $signature1 $signature2 2 $address1 $address2 2 OP_CHECKMULTISIG
Stack: [ ]
```
You place all the constants on the stack:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 2 ]
```
Then, the `OP_CHECKMULTISIG` begins to run. First, the "2" is popped:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 ]
```
Then, the "2" tells `OP_CHECKMULTISIG `to pop two addresses:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 ]
```
Then, the next "2" is popped:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 ]
```
Then, the "2" tells `OP_CHECKMULTISIG` to pop two signatures:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 ]
```
Then, one more item is mistakenly popped:
```
Script: OP_CHECKMULTISIG
Stack: [ ]
```
Then, `OP_CHECKMULTISIG` completes its operation by comparing the "m" signatures to the "n" addresses:
```
Script:
Stack: [ True ]
```
## Understand the Limitations of Raw Multisig Scripts

Unfortunately, the technique of embedding a raw multisig into a transaction has some notable drawbacks:

1. Because there's no standard address format for multisigs, each sender has to: enter a long and cumbersome multisig script; have software that allows this; and be trusted not to mess it up.
2. Because multisigs can be much longer than typical locking scripts, the blockchain incurs more costs. This requires higher transaction fees from the sender and creates more nuisance for every node.

These were generally problems with any sort of complex Bitcoin script, but they quickly became very real problems when applied to multisigs, which were some of the first complex scripts to be widely used on the Bitcoin network. P2SH transactions were created to solve these problems, starting in 2012. 

_What is a P2SH multisig?_ P2SH multisigs were the first implementation of P2SH transactions. They simply package up a standard multisig transaction into a standard P2SH transaction. This allows for address standardization; reduced data storage; and increased "m" and "n" counts.

## Create a P2SH Multisig

P2SH multisigs are the modern methodology for creating multisigs on the Blockchains. They can be created very simply, using the same process seen in the prvious two sections.

### Create the Lock for the P2SH Multisig

To create a P2SH multisig, follow the standard steps for creating a P2SH locking script:

1. Serialize `2 $address1 $address2 2 OP_CHECKMULTISIG`.
   1. `<serializedMultiSig>` = "52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
2. SHA-256 and RIPEMD-160 hash the serialized script.
   1. `<hashedMultiSig>` = "babf9063cee8ab6e9334f95f6d4e9148d0e551c2"
3. Save `<serialized99Equal>` for future reference as the redeemScript.
   1. `<redeemScript>` = "52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
4. Produce a P2SH Multisig locking script that includes the hashed script (`OP_HASH160 <hashedMultisig> OP_EQUAL`).
   1. `scriptPubKey` = "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287"
   
You can then create a transaction using that `scriptPubKey`.

## Unlock the P2SH Multisig

To unlock this multisig transaction requires that the recipient produce a scriptSig that includes the two signatures and the `redeemScript`.

### Run the First Round of P2SH Validation

To unlock the P2SH multisig, first confirm the script:

1. Produce an unlocking script of `0 $signature1 $signature2 <serializedMultiSig>`.
2. Concatenate that with the locking script of `OP_HASH160 <hashedMultisig> OP_EQUAL`.
3. Validate `0 $signature1 $signature2 <serializedMultiSig> OP_HASH160 <hashedMultisig> OP_EQUAL`.

### Run the Second Round of P2SH Validation

Then, run the multisig script:

1. Deserialize `<serializedMultiSig>` to `2 $address1 $address2 2 OP_CHECKMULTISIG`.
2. Concatenate that with the earlier operands in the unlocking script, `0 $signature1 $signature2`.
3. Validate `0 $signature1 $signature2 2 $address1 $address2 2 OP_CHECKMULTISIG`.

Now you know how the multisig transaction in [§6.1](6_1_Sending_a_Transaction_to_a_Multisig.md) was actually created, how it was  validated for spending, and why that `redeemScript` was so important.

## Summary: Creating Multisig Scripts

Multisigs are a standard transaction type, but they're a bit cumbersome to use, so they're regularly incorporated in P2SH transactions, as was the case in [§6.1](6_1_Sending_a_Transaction_to_a_Multisig.md), when we created our first multisigs. The result is cleaner, smaller, and more standardized — but more importantly, it's a great real-world example of how P2SH scripts really work.
