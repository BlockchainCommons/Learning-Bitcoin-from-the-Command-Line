# 9.2: Using CLTV in Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

`OP_CHECKLOCKTIMEVERIFY` (or CLTV) is the natural complement to `nLockTime`. It moves the idea of locking transactions by an absolute time or blockheight into the realm of opcodes, allowing for the locking of individual UTXOs.

### Remember nLockTime

Before digging into CLTV, we should first recall how `nLockTime` works.

As detailed in [§6.4: Sending a Transaction with a Locktime](6_4_Sending_a_Transaction_with_a_Locktime.md), locktime is enabled by setting two variables, `nLockTime` and the `nSequence`. The `nSequence` must be set to less than 0xffffffff, then the `nLockTime` is interpreted as follows:

* If the `nLockTime` is less than 500 million, it is interpreted as a blockheight.
* If the `nLockTime` is 500 million or more, it is interpreted as a UNIX timestamp.

The transaction cannot be spent (or even put on the block chain) until either the blockheight or time is reached. In the meantime, the transaction can be cancelled by respending any of the UTXOs that make up the transaction.

## Understand the CLTV Opcode

`OP_CHECKLOCKTIMEVERIFY` works within the same paradigm of absolute blockheights or absolute UNIX times, but it runs as part of a Bitcoin Script. It reads one argument, which can be a blockheight or an absolute UNIX time. Through a somewhat convoluted methodology, it compares that argument to the current time. If it's too early, the script fails, but if the time condition has met, the script carries on. 

Because CLTV is just part of a script (and presumably part of a P2SH transaction), the transaction immediately goes into the mempool; as soon as it's verified, it goes onto the blockchain, and the funds are considered spent. The trick is that all the outputs that were locked with the CLTV aren't available for _respending_ until the CLTV allows it.

This is how a `OP_CHECKLOCKTIMEVERIFY` might be used to check against May 24, 2017:
```
1495652013 OP_CHECKLOCKTIME VERIFY
```
But we'll usually depict this in an abstraction like this:
```
<May24> OP_CHECKLOCKTIME VERIFY
```
Or this:
```
<AbsoluteTime> OP_CHECKLOCKTIME VERIFY
```
This is how `OPCHECKLOCKTIMEVERIFY` might be used to check against a blockheight that was reached on May 24, 2017:
```
467951 OP_CHECKLOCKTIME VERIFY
```
But we'll usually abtract it like this:
```
<AbsoluteBlockHeight> OP_CHECKLOCKTIME VERIFY
```

> **VERSION WARNING:** CLTV became available with Bitcoin Core 0.11.2, and should be fairly widely deployed at this time.

### Understand How CLTV Really Works

The above explanation is sufficient to use and understand CLTV. However, [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) lays out all the details.

In order for CLTV to verify, allowing continued execution of a Bitcoin Script, the following must occur:

* The `nSequence` field must be set to less than 0xffffffff
* There must be an operand on the stack that's 0 or greater.
* Both the stack operand and `nLockTime` must either be above or below 500 million, to depict the same sort of absolute timelock.
* The `nLockTime` must be greater than or equal to the stack operand.

So the first thing to note here is that `nLockTime` is still used with CLTV. To be precise, it's required in a transaction that tries to _respend_ a CLTV-timelocked UTXO. That means that it's not a part of the script's logic. It's just the timer that's used to release the funds, _as defined in the script_. 

This is managed through a clever understanding of how `nLockTime` works: a value for `nLockTime` must always be chosen that is less than or equal to the present time, so that the respending transaction can be put on the blockchain. However, due to CLTV's requirements, a value must also be chosen that is greater than or equal to CLTV's argument. The union of these two sets is 0 until the present time matches the CLTV argument. Afterward, any value can be chosen between CLTV's argument and the present time.

## Write a CLTV Script

`OP_CHECKLOCKTIMEVERIFY` includes an `OP_VERIFY`, which means that it will immediately halt the script if its verification does not succeed. It has one other quirk: unlike most "Verify" commands, it leaves what it's testing on the stack (just in case you want to make any other checks against the time). This means that an `OP_CHECKLOCKTIMEVERIFY` is usually followed by an `OP_DROP` to clear the stack.

The following simple locking script could be used to transform a P2PKH output to CLTV-P2PKH transaction:
```
<NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```

### Encode a CLTV Script

Of course, as with any complex Bitcoin Scripts, this would actually be encoded in a P2SH script, as explained in [§8.1: Understanding the Foundation of P2SH](8_1_Understanding_the_Foundation_of_P2SH.md). 

Assuming that <NextYear> were "1546288031" (hex: 0x5c2a7b9f) and <pubKeyHash> were "371c20fb2e9899338ce5e99908e64fd30b789313", this `redeemScript` would be built as:
```
OP_PUSHDATA (4 bytes) 0x5c2a7b9f OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 OP_PUSHDATA (20 bytes) 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
```
Which translates into hex as:
```
04 5C2A7B9F b1 75 76 a9 14 371c20fb2e9899338ce5e99908e64fd30b789313 88 ac
```
The `decodescript` RPC can make sure we got it right:
```
$ bitcoin-cli -named decodescript hexstring=045C2A7B9Fb17576a914371c20fb2e9899338ce5e99908e64fd30b78931388ac
{
  "asm": "-528165468 OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG",
  "type": "nonstandard",
  "p2sh": "2NEPS1idT9zVGm8WJbHMyGTSwAqLHbM9RPp"
}
```

We're not going to continuously show how all Bitcoin Scripts are abstracted into P2SH transactions, but will instead offer these shorthands: when we describe a script, it will be a `redeemScript`, which would normally be serialized and hashed in a locking script and serialized in the unlocking script; and when we show an unlocking procedure, it will be the second round of validation, following the confirmation of the locking script hash.

## Spend a CLTV UTXO

In order to spend a UTXO that is locked with a CLTV, you must set `nLockTime` on your new transaction. Usually, you just want to set it to the present time or the present block, as appropriate. As long the CLTV time or blockheight is in the past, and as long as you supply any other data required by the unlocking script, you'll be able to process the UTXO.

In the case of the above example, the following unlocking script would suffice, provided that `nLockTime` was set to somewhere in advance of the <NextYear> date, and provided it was indeed at least <NextYear>:
```
<signature> <pubKey>
```

### Run a CLTV Script

To run the Script, you would first concatenate the unlocking and locking scripts:
```
Script: <signature> <pubKey> <NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]
```
The sthree constants would be pushed onto the stack:
```
Script: OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <NextYear> ]
```
Then, `OP_CHECKLOCKTIMEVERIFY` would run. It finds something on the stack and verifies that `nSequence` isn't 0xffffffff. Finally, it compares `<NextYear>` with `nLockTime`. If they are both the same sort of representation and if `nLockTime ≥ <NextYear>`, then it successfully processes (else, it ends the script):
```
Script: OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <NextYear> ]
```
Then, `OP_DROP` gets rid of that `<NextYear>` left around:
```
Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> ]
```
Finally, the remainder of the script runs, which is a normal check of a signature and public key.
## Summary: Using CLTV in Scripts

`OP-CHECKLOCKTIMEVERIFY` is a simple opcode that looks at a single arguments, interprets it as a blockheight or UNIX timestamp, and only continues with a UTXO's redemption if that blockheight or UNIX timestamp is in the past. Setting `nLockTime` on the new transaction is what allows Bitcoin to make this calculation.

_What is the Power of CLTV?_ You've already seem that simple locktimes were one of the bases of Smart Contracts. CLTV takes the next step. Now you can both guarantee that a UTXO can't be spent before a certain _and_ guarantee that it won't be spent before that. In its simplest form, this could be used to create a trust that someone could only access when they reached 18 or a retirement fund that they could only access when they turned 50. However it's true power comes when combined with conditionals, which allow CLTV to be used only in some situations.
