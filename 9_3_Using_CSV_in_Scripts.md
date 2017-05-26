# 9.3: Using CSV in Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

`nLockTime` and `OP_CHECKLOCKTIMEVERIFY` (or CLTV) are just one side of the timelock equation. On the other are `nSequence` and `OP_CHECKSEQUENCEVERIFY`, which can be used to check against relative times rather than absolute times.

> **VERSION WARNING:** CSV became available with Bitcoin Core 0.12.1, in spring 2016.

## Understand nSequence

Every UTXO used in a transaction has an `nSequence` (or if you prefer `sequence`) value. It's been a prime tool for Bitcoin expansions as discussed previously in [§5.2: Resending a Transaction with RBF](5_2_Resending_a_Transaction_with_RBF.md) and [§6.4 Sending a Transaction with a Locktime.md](6_4_Sending_a_Transaction_with_a_Locktime.md), where it was used to signal RBF and `nLockTime`, respectively. However, there's one more use for `nSequence`, described by [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki): you can use it to create a relative timelock on a transaction.

A relative timelock is a lock that's placed on a specific input of a transaction and that's calculated in relation to the mining date of the UTXO being used in th einput. For example, if a UTXO was mined at block #468260 and a transaction was created where the input for that UTXO was given an `nSequence` of 100, then the new transaction could not be mined until at least block #468360.

Easy!

> **SEQUENCE NOTE (III):** This is the third use of the `nSequence` value in Bitcoin. Any `nSequence` value from 1 to 0xf0000000-1 will be interpreted as a relative timelock if `nVersion ≥ 2` (which was the default starting in Bitcoin Core 0.14.0). You should be careful to ensure that relative timelocks don't conflict with the other two uses of `nSequence`, for signalling `nTimeLock` and RBF. `nTimeLock` usually sets a value of 0xffffffff-1, where a relative timelock is disallowed; and RBF usually sets a value of "1", where a relative timelock is irrelevent, because it defines a timelock of 1 block. In general, remember: with a `nVersion` value of 2, a `nSequence` value of 0x00000001 to 0xf0000000-1 allows relative timelock, RBF, and `nTimeLock`; a `nSequence` value of 0xf0000000 to 0xffffffff-2 allows RBF and `nTimeLock`; a `nSequence` value of 0xffffffff-1 allows only `nTimeLock`; a `nSequence` value of 0xffffffff allows none; and `nVersion` can be set to 1 to disallow relative timelocks for any value of `nSequence`. Whew!

### Create a CSV Relative Block Time

The format for using `nSequence` to represent relative time locks is defined in [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) and is slightly more complex than just inputting a number, like you did for `nTimeLock`. Instead, the BIP specifications breaks up the four byte number into three parts:

* The first two bytes are used to specify a relative locktime.
* The 23rd bit is used to signal if the lock refers to a time rather than a blockheight.
* The 32nd bit is used to signal if relative timelocks are deactivated.

If you want to create a block-based relative timelock, the construction is still quite easy: you set `nSequence` to a value between 1 and 0xffff (65535). The new transaction can be mined that numebr of blocks after the associated UTXO was mined.

### Create a CSV Relative Time

You can instead set `nSequence` as a relative time, where the lock lasts for 512 seconds time the value of `nSequence`.

In order to do that:

1. Decide how far in the future to set your relative timelock.
2. Convert that to seconds.
3. Divide by 512.
4. Round that value up or down and set it as `nSequence`.
5. Set bit 23 to true.

To set a time 6 months n the future, you would first calculate as follows:
```
$ seconds=$((6*30*24*60*60))
$ nvalue=$(($seconds/512))
$ hexvalue=$(printf '%x\n' $nvalue)
$ relativevalue=$(printf '%x\n' $((0x$hexvalue | 0x400000)))
$ echo $relativevalue
4224679
```
If you convert that back you'll see that 4224679 = 10000000111011010100111. The 23rd digit is set to a "1"; meanwhile the first 2 bytes, 0111011010100111, convert to 76A7 in hex or 30375 in decimal. Multiply that by 512 and you get 15.55 million seconds, which indeed is 180 days.

## Create a Transaction with a Relative Timelock

So you want to create a relative timelock? All you have to do is issue a tranaction where the `nSequence` in an input is set in accordance with the above: with the `nSequence` for that input set such that the first two bytes define the timelock, the 23rd bit defines the type of timelock, and the 32nd bit is set to false. It's a straight up two-byte number for a timelock based on relative blockheight and it's a two-byte number `0x400000` for a timelock based on relative time.

Issue the transaction, but it can't legally be mined until enough blocks or enough time has passed beyond the time that the transaction forming the UTXO was mined.

Except pretty much no one does this. The new [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) definitions for `nSequence` were incorporated into Bitcoin Core at the same time as [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) which describes the CSV opcode, which works with `nSequence`, just like the CLTV opcode works with `nTimeLock`. Just like CLTV, CSV offers increased capabilities. So almost all usage of relative timelocks has been with the CSV opcode, not with the raw `nSequence` value on its own.

|                  | Absolute Timelock | Relative Timelock |
|:------------------:|-------------------|-------------------|
| **Lock Transaction** | nTimeLock         | nSequence         |
| **Lock Output**      | OP_CHECKLOCKTIMEVERIFY| OP_CHECKSEQUENCEVERIFY |

## Understand the CSV Opcode

`OP_SEQUENCEVERIFY` in Bitcoin Scripts works pretty much like `OP_LOCKTIMEVERIFY`. 

You might require a UTXO be held for a hundred blocks past its mining:
```
100 OP_CHECKSEQUENCEVERIFY
```
Or your might make a more complex calculation to require that a UTXO be held for six months, in which case you'll end up with a more complex number:
```
4224679 OP_CHECKSEQUENCEVERIFY
```
In this case we'll probably use a shorthand:
```
<+6Months> OP_CHECKSEQUENCEVERIFY
```

> **WARNING:** Remember that your a relative time lock is a time span since the mining of the UTXO used as an input. It is _not_ a timespan after you create the transaction. If you use a UTXO that's already been confirmed a hundred times, and you place a relative timelock of 100 blocks on it, it will be eligible for mining immediately. Relative timelocks have some very specific uses, but they probably don't apply if your only goal is to determine some set time in the future.

### Understand How CSV Really Works

CSV has many of the same subtleties in usage as CLTV:

* The `nVersion` field must be set to 2 or more.
* The `nSequence` field must be set to less than 0xf0000000.
* There must be an operand on the stack that's between 0 and 0xf0000000-1.
* Both the stack operand and `nSequence` must have the same value on the 23rd bit.
* The `nSequence` must be greater than or equal to the stack operand.

Just as with CLTV, when you are respending a UTXO with a CSV in its locking conditions, you must set the `nSequence` to enable the transaction. You'll usually set it to the number of blocks or the converted value of seconds since the UTXO was mined, as appropriate. 

## Write a CSV Script

Just like `OP_CHECKLOCKTIMEVERIFY`, `OP_CHECKSEQUENCEVERIFY` includes an implicit `OP_VERIFY` and leaves its arguments on the stack, requiring an `OP_DROP` when you're all done.

A script that would lock funds until six months had passed following the mining of the input, and that would then require a standard P2PKH-style signature would look as follows:
```
<+6Months> OP_CHECKSEQUENCEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```

### Encode a CLTV Script

When you encode a CLTV script, be careful how you encode the integer value for the relative locktime. It should be passed as a 3-byte integer, which means that you're ignoring the top byte which could inactivate the relative locktime. Since it's an integer, be sure you convert it to little-endian.

This can be done with the `integer2lehex.sh` shell script from the previous chapter.

For a relative time of 100 blocks:
```
$ ./integer2lehex.sh 100
Integer: 100
LE Hex: 64
Length: 1 bytes
Hexcode: 0164
```
Though that should be padded out to `000064`, requiring a code of `030164`.

For a relative time of 6 months:
```
$ ./integer2lehex.sh 4224679
Integer: 4224679
LE Hex: a77640
Length: 3 bytes
Hexcode: 03a77640
```

## Spend a CSV UTXO

## Summary: Using CSV in Scripts

