# 8.2: Building a Bitcoin Script with P2SH

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You know that Bitcoin Scripts can be used to control the redemption of UTXOs, and that standard Bitcoin transactions depend on very specific locking scripts. The next step is creating Scripts of your own ... but that requires a very specific techniques.

## Rebuild a Script as a P2SH

In [§7.2: Running a Bitcoin Script](7_2_Running_a_Bitcoin_Script.md), we offered a simple example of a non-standard locking script, `OP_ADD 99 OP_EQUAL`, and we executed it as a simple concatenation of an unlocking script with that locking script. We're now going to repeat that exercise, this time within the constraints of a P2SH transaction.

### Create the Lock for the P2SH Transaction

To lock this transaction do the following:

1. Serialize `OP_ADD 99 OP_EQUAL` (`<serialized99Equal>`) then SHA-256 and RIPEMD-160 hash it (`<hashed99Equal>`).
   a. OP_ADD = 0x93
   b. 99 = 0x01, 0x63, the first to push one byte onto the stack, the second as the hex translation of 99
   c. OP_EQUAL = 0x87
   d. `<serialized99Equal>` = "93016387" 
   e. `<hashed99Equal>` = "3f58b4f7b14847a9083694b9b3b52a4cea2569ed"
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
