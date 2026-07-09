# 12.3: Running a Bitcoin Script with P2SH or P2WSH

Now that you know both the theory
([§12.1](12_1_Understanding_the_Foundation_of_PW2SH.md)) and practice
([§12.2](12_2_Building_the_Structure_of_P2WSH.md)) behind P2SH and
P2WSH addresses, you're ready to turn a non-standard Bitcoin Script
into an actual transaction. We'll be reusing the simple locking script
from [§11.2: Running a Bitcoin
Script](11_2_Running_a_Bitcoin_Script.md), `OP_ADD 99 OP_EQUAL`.

## Create a P2SH Transaction

We're going to format this as a P2SH transaction, so that we can
easily run through the entire unlocking process as part of the locking
script. But, it would be exactly the same as a P2WSH transaction, just
with a different hashing and encoding mechanism.

To lock a transaction with this script, do the following:

1. Serialize `OP_ADD 99 OP_EQUAL`:
   1. OP_ADD = 0x93 — a simple opcode translation
   2. 99 = 0x01, 0x63 — this opcode pushes one byte onto the stack, 99 (hex: 0x63)
      * No worries about endian conversion because it's only one byte
   3. OP_EQUAL = 0x87 — a simple opcode translation
   4. `<serialized99Equal>` = "93016387" 
   
```
btcc OP_ADD 99 OP_EQUAL

| 93016387
```

2. Save `<serialized99Equal>` for future reference as the `redeemScript`.
   1. `<redeemScript>` = "93016387"
3. SHA-256 and RIPEMD-160 hash the serialized script.
   1. `<hashed99Equal>` = "3f58b4f7b14847a9083694b9b3b52a4cea2569ed"

```
echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160

| RIPEMD-160(stdin)= 3f58b4f7b14847a9083694b9b3b52a4cea2569ed
```

4. Produce a P2SH locking script that includes the `<hashed99Equal>`. That's `a914` + hash + `87`.
   1. `scriptPubKey` = "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87"

You can then create a transaction using this `scriptPubKey`, probably via an API.

## Unlock the P2SH Transaction

To unlock this transaction requires that the recipient produce a
`scriptSig` that prepends two constants totalling ninety-nine to the
serialized script: `1 98 <serialized99Equal>`.

### Run the First Round of Validation

The process of unlocking the P2SH transaction then begins with a first
round of validation, which checks that the redeem script matches the
hashed value in the locking script.

Concatenate `scriptSig` and `scriptPubKey` and execute them, as normal:
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
Running: <serialized99Equal> OP_HASH160
Stack: [ 1 98 <hashed99Equal> ]

Script: OP_EQUAL
Stack: [ 1 98 <hashed99Equal> <hashed99Equal> ]

Script: 
Running: <hashed99Equal> <hashed99Equal> OP_EQUAL
Stack: [ 1 98 True ]
```
The first round of validation for the script ends with a `True` on top
of the stack, and so it succeeds ... even though there's other cruft
below it.

However, because this was a P2SH script, the execution isn't done. 

### Run the Second Round of Validation

For the second round of validation, verify that the values in the
unlocking script satisfy the `redeemScript`: deserialize the
`redeemScript` ("93016387" = "OP_ADD 99 OP_EQUAL"), then execute it
using the items in the `scriptSig` prior to the serialized script:

```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: [ ]

Script: 98 OP_ADD 99 OP_EQUAL
Stack: [ 1 ]

Script: OP_ADD 99 OP_EQUAL
Stack: [ 1 98 ]

Script: 99 OP_EQUAL
Running: 1 98 OP_ADD
Stack: [ 99 ]

Script: OP_EQUAL
Stack: [ 99 99 ]

Script: 
Running: 99 99 OP_EQUAL
Stack: [ True ]
```
With that second validation _also_ true, the UTXO can now be spent!

## Working with a P2WSH Script

The same methodology would be used for a P2WSH script:

```
`<script>` = `OP_ADD 99 OP_EQUAL`
`<serialized99Equal>` = "93016387"
`<redeemScript>` = "93016387"
```

The first difference is that we hash only with SHA-256:

```
echo -n $redeemScript | xxd -r -p | openssl dgst -sha256

| SHA2-256(stdin)= 481308649657448504a969ead1f79b256c555ccf0c5a70bfeef56d0768dde2ac
```

The second difference is that our P2WSH embed is just `0020` in front of that script hash, for `OP_0` and then a push of the 32-byte hash:

```
`scriptPubKey` = "0020481308649657448504a969ead1f79b256c555ccf0c5a70bfeef56d0768dde2ac"
```

Unlocking should then look much the same, except that the first round
of validation is done by consensus, rather than explicit
`scriptPubKey`, and it hashes the supplied redeem script with SHA-256
rather than RIPEMD-160, just as was done in the `scriptPubKey`
creation. (The second round of validation, where the actual script is
tested, then follows as normal.)

## Summary: Building a Bitcoin Script with P2SH or P2WSH

Once you know the technique of building P2SHes and P2WSHes, any Script
can be embedded in a Bitcoin transaction; and once you understand the
technique of validating P2SHes and P2WSHes, it's easy to run the
scripts in two rounds.

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§12.4: Scripting a
Multisig](12_4_Scripting_a_Multisig.md).
