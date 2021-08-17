# 11.2: Using CLTV in Scripts

`OP_CHECKLOCKTIMEVERIFY` (or CLTV) is the natural complement to `nLockTime`. It moves the idea of locking transactions by an absolute time or blockheight into the realm of opcodes, allowing for the locking of individual UTXOs.

> :warning: **VERSION WARNING:** CLTV became available with Bitcoin Core 0.11.2, but should be fairly widely deployed at this time.

## Remember nLockTime

Before digging into CLTV, we should first recall how `nLockTime` works.

As detailed in [§8.1: Sending a Transaction with a Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md), locktime is enabled by setting two variables, `nLockTime` and the `nSequence`. The `nSequence` must be set to less than 0xffffffff (usually: 0xffffffff-1), then the `nLockTime` is interpreted as follows:

* If the `nLockTime` is less than 500 million, it is interpreted as a blockheight.
* If the `nLockTime` is 500 million or more, it is interpreted as a UNIX timestamp.

A transaction with `nLockTime` set cannot be spent (or even put on the block chain) until either the blockheight or time is reached. In the meantime, the transaction can be cancelled by respending any of the UTXOs that make up the transaction.

## Understand the CLTV Opcode

`OP_CHECKLOCKTIMEVERIFY` works within the same paradigm of absolute blockheights or absolute UNIX times, but it runs as part of a Bitcoin Script. It reads one argument, which can be a blockheight or an absolute UNIX time. Through a somewhat convoluted methodology, it compares that argument to the current time. If it's too early, the script fails; if the time condition has been met, the script carries on. 

Because CLTV is just part of a script (and presumably part of a P2SH transaction), a CLTV transaction is not kept out of the mempool like an `nLockTime` transaction is; as soon as it's verified, it goes onto the blockchain, and the funds are considered spent. The trick is that all the outputs that were locked with the CLTV aren't available for _respending_ until the CLTV allows it.

### Understand a CLTV Absolute Time

This is how `OP_CHECKLOCKTIMEVERIFY` would be used to check against May 24, 2017:
```
1495652013 OP_CHECKLOCKTIMEVERIFY
```
But we'll usually depict this in an abstraction like this:
```
<May24> OP_CHECKLOCKTIMEVERIFY
```
Or this:
```
<AbsoluteTime> OP_CHECKLOCKTIMEVERIFY
```

### Understand a CLTV Absolute Block Height

This is how `OP_CHECKLOCKTIMEVERIFY` would check against a blockheight that was reached on May 24, 2017:
```
467951 OP_CHECKLOCKTIMEVERIFY
```
But we'll usually abtract it like this:
```
<AbsoluteBlockHeight> OP_CHECKLOCKTIMEVERIFY
```

### Understand How CLTV Really Works

The above explanation is sufficient to use and understand CLTV. However, [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) lays out all the details.

A locking script will only allow a transaction to respend a UTXO locked with a CLTV if `OP_CHECKLOCKTIMEVALUE` verifies all of the following:

* The `nSequence` field must be set to less than 0xffffffff, usually 0xffffffff-1 to avoid confilcts with relative timelocks.
* CLTV must pop an operand off the stack and it must be 0 or greater.
* Both the stack operand and `nLockTime` must either be above or below 500 million, to depict the same sort of absolute timelock.
* The `nLockTime` value must be greater than or equal to the stack operand.

So the first thing to note here is that `nLockTime` is still used with CLTV. To be precise, it's required in the transaction that tries to _respend_ a CLTV-timelocked UTXO. That means that it's not a part of the script's requirements. It's just the timer that's used to release the funds, _as defined in the script_. 

This is managed through a clever understanding of how `nLockTime` works: a value for `nLockTime` must always be chosen that is less than or equal to the present time (or blockheight), so that the respending transaction can be put on the blockchain. However, due to CLTV's requirements, a value must also be chosen that is greater than or equal to CLTV's operand. The union of these two sets is `NULL` until the present time matches the CLTV operand. Afterward, any value can be chosen between CLTV's operand and the present time. Usually, you'd just set it to the present time (or block).

## Write a CLTV Script

`OP_CHECKLOCKTIMEVERIFY` includes an `OP_VERIFY`, which means that it will immediately halt the script if its verification does not succeed. It has one other quirk: unlike most "verify" commands, it leaves what it's testing on the stack (just in case you want to make any other checks against the time). This means that an `OP_CHECKLOCKTIMEVERIFY` is usually followed by an `OP_DROP` to clear the stack.

The following simple locking script could be used to transform a P2PKH output to a timelocked-P2PKH transaction:
```
<NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```

### Encode a CLTV Script

Of course, as with any complex Bitcoin Scripts, this CLTV script would actually be encoded in a P2SH script, as explained in [§10.1: Understanding the Foundation of P2SH](10_1_Understanding_the_Foundation_of_P2SH.md) and [§10.2: Building the Structure of P2SH](10_2_Building_the_Structure_of_P2SH.md). 

Assuming that `<NextYear>` were the integer "1546288031" (little-endian hex: 0x9f7b2a5c) and `<pubKeyHash>` were "371c20fb2e9899338ce5e99908e64fd30b789313", this `redeemScript` would be built as:
```
OP_PUSHDATA (4 bytes) 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 OP_PUSHDATA (20 bytes) 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
```
Which translates into hex as:
```
04 9f7b2a5c b1 75 76 a9 14 371c20fb2e9899338ce5e99908e64fd30b789313 88 ac
```
Or if you prefer:
```
$ btcc 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
049f7b2a5cb17576a914371c20fb2e9899338ce5e99908e64fd30b78931388ac
```
The `decodescript` RPC can verify that we got it right:
```
{
  "asm": "1546288031 OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG",
  "type": "nonstandard",
  "p2sh": "2MxANZMPo1b2jGaeKTv9rwcBEiXcXYCc3x9",
  "segwit": {
    "asm": "0 07e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "hex": "002007e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qqlj4hu02ah6ra3f274ah0ttnxpgxcgy6wrghlghps5esf25wfedse4yw4w"
    ],
    "p2sh-segwit": "2N4HTwMjVdm38bdaQ5h3X3VktLY74D2qBoK"
  }
}
```

We're not going to continuously show how all Bitcoin Scripts are encoded into P2SH transactions, but will instead offer these shorthands: when we describe a script, it will be a `redeemScript`, which would normally be serialized and hashed in a locking script and serialized in the unlocking script; when we show an unlocking procedure, it will be the second round of validation, following the confirmation of the locking script hash.

## Spend a CLTV UTXO

In order to spend a UTXO that is locked with a CLTV, you must set `nLockTime` on your new transaction. Usually, you just want to set it to the present time or the present block, as appropriate. As long the CLTV time or blockheight is in the past, and as long as you supply any other data required by the unlocking script, you'll be able to process the UTXO.

In the case of the above example, the following unlocking script would suffice, provided that `nLockTime` was set to somewhere in advance of the `<NextYear>` date, and provided it was indeed at least `<NextYear>`:
```
<signature> <pubKey>
```

### Run a CLTV Script

To run the Script, you would first concatenate the unlocking and locking scripts:
```
Script: <signature> <pubKey> <NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]
```
The three constants would be pushed onto the stack:
```
Script: OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <NextYear> ]
```
Then, `OP_CHECKLOCKTIMEVERIFY` runs. It finds something on the stack and verifies that `nSequence` isn't 0xffffffff. Finally, it compares `<NextYear>` with `nLockTime`. If they are both the same sort of representation and if `nLockTime ≥ <NextYear>`, then it successfully processes (else, it ends the script):
```
Script: OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <NextYear> OP_CHECKLOCKTIMEVERIFY
Stack: [ <signature> <pubKey> <NextYear> ]
```
Then, `OP_DROP` gets rid of that `<NextYear>` left around:
```
Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <NextYear> OP_DROP
Stack: [ <signature> <pubKey> ]
```
Finally, the remainder of the script runs, which is a normal check of a signature and public key.

## Summary: Using CLTV in Scripts

`OP_CHECKLOCKTIMEVERIFY` is a simple opcode that looks at a single argument, interprets it as a blockheight or UNIX timestamp, and only allows its UTXO to be unlocked if that blockheight or UNIX timestamp is in the past. Setting `nLockTime` on the spending transaction is what allows Bitcoin to make this calculation.

> :fire: ***What is the Power of CLTV?*** You've already seem that simple locktimes were one of the bases of Smart Contracts. CLTV takes the next step. Now you can both guarantee that a UTXO can't be spent before a certain time _and_ guarantee that it won't be spent either. In its simplest form, this could be used to create a trust that someone could only access when they reached 18 or a retirement fund that they could only access when they turned 50. However its true power comes when combined with conditionals, where the CLTV only activates in certain situations.

## What's Next?

Continue "Empowering Timelock" with [§11.3: Using CSV in Scripts](11_3_Using_CSV_in_Scripts.md). 
