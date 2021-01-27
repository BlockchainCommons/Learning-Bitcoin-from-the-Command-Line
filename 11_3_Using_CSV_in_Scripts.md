# 11.3: Using CSV in Scripts

`nLockTime` and `OP_CHECKLOCKTIMEVERIFY` (or CLTV) are just one side of the timelock equation. On the other side are `nSequence` and `OP_CHECKSEQUENCEVERIFY`, which can be used to check against relative times rather than absolute times.

> :warning: **VERSION WARNING:** CSV became available with Bitcoin Core 0.12.1, in spring 2016.

## Understand nSequence

Every input into in a transaction has an `nSequence` (or if you prefer `sequence`) value. It's been a prime tool for Bitcoin expansions as discussed previously in [§5.2: Resending a Transaction with RBF](05_2_Resending_a_Transaction_with_RBF.md) and [§8.1 Sending a Transaction with a Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md), where it was used to signal RBF and `nLockTime`, respectively. However, there's one more use for `nSequence`, described by [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki): you can use it to create a relative timelock on a transaction.

A relative timelock is a lock that's placed on a specific input of a transaction and that's calculated in relation to the mining date of the UTXO being used in the input. For example, if a UTXO was mined at block #468260 and a transaction was created where the input for that UTXO was given an `nSequence` of 100, then the new transaction could not be mined until at least block #468360.

Easy!

> :information_source: **NOTE — SEQUENCE:** This is the third use of the `nSequence` value in Bitcoin. Any `nSequence` value without the 32nd bit set (1<<31), so 0x00000001 to 0x7ffffffff, will be interpreted as a relative timelock if `nVersion ≥ 2` (which is the default starting in Bitcoin Core 0.14.0). You should be careful to ensure that relative timelocks don't conflict with the other two uses of `nSequence`, for signalling `nTimeLock` and RBF. `nTimeLock` usually sets a value of 0xffffffff-1, where a relative timelock is disallowed; and RBF usually sets a value of "1", where a relative timelock is irrelevent, because it defines a timelock of 1 block. 

> In general, remember: with a `nVersion` value of 2, a `nSequence` value of 0x00000001 to 0x7fffffff allows relative timelock, RBF, and `nTimeLock`; a `nSequence` value of 0x7fffffff to 0xffffffff-2 allows RBF and `nTimeLock`; a `nSequence` value of 0xffffffff-1 allows only `nTimeLock`; a `nSequence` value of 0xffffffff allows none; and `nVersion` can be set to 1 to disallow relative timelocks for any value of `nSequence`. Whew!

### Create a CSV Relative Block Time

The format for using `nSequence` to represent relative time locks is defined in [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) and is slightly more complex than just inputting a number, like you did for `nTimeLock`. Instead, the BIP specifications breaks up the four byte number into three parts:

* The first two bytes are used to specify a relative locktime.
* The 23rd bit is used to positively signal if the lock refers to a time rather than a blockheight.
* The 32nd bit is used to positively signal if relative timelocks are deactivated.

With that said, the construction of a block-based relative timelock is still quite easy, because the two flagged bits are set to `0`, so you just set `nSequence` to a value between 1 and 0xffff (65535). The new transaction can be mined that number of blocks after the associated UTXO was mined.

### Create a CSV Relative Time

You can instead set `nSequence` as a relative time, where the lock lasts for 512 seconds times the value of `nSequence`.

In order to do that:

1. Decide how far in the future to set your relative timelock.
2. Convert that to seconds.
3. Divide by 512.
4. Round that value up or down and set it as `nSequence`.
5. Set the 23rd bit to true.

To set a time 6 months n the future, you must first calculate as follows:
```
$ seconds=$((6*30*24*60*60))
$ nvalue=$(($seconds/512))
```
Then, turn it into hex:
```
$ hexvalue=$(printf '%x\n' $nvalue)
```
Finally, bitwise-or the 23rd bit into the hex value you created:
```
$ relativevalue=$(printf '%x\n' $((0x$hexvalue | 0x400000)))
$ echo $relativevalue
4224679
```
If you convert that back you'll see that 4224679 = 10000000111011010100111. The 23rd digit is set to a "1"; meanwhile the first 2 bytes, 0111011010100111, convert to 76A7 in hex or 30375 in decimal. Multiply that by 512 and you get 15.55 million seconds, which indeed is 180 days.

## Create a Transaction with a Relative Timelock

So you want to create a simple transaction with a relative timelock? All you have to do is issue a transaction where the `nSequence` in an input is set as shown above: with the `nSequence` for that input set such that the first two bytes define the timelock, the 23rd bit defines the type of timelock, and the 32nd bit is set to false. 

Issue the transaction and you'll see that it can't legally be mined until enough blocks or enough time has passed beyond the time that the UTXO was mined.

Except pretty much no one does this. The [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) definitions for `nSequence` were incorporated into Bitcoin Core at the same time as [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki). which describes the CSV opcode, the `nSequence` equivalent to the CLTV opcode. Just like CLTV, CSV offers increased capabilities. So, almost all usage of relative timelocks has been with the CSV opcode, not with the raw `nSequence` value on its own.

|                      | Absolute Timelock | Relative Timelock |
|:--------------------:|-------------------|-------------------|
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
In this case we'll use a shorthand:
```
<+6Months> OP_CHECKSEQUENCEVERIFY
```

> :warning: **WARNING:** Remember that a relative timelock is a time span since the mining of the UTXO used as an input. It is _not_ a timespan after you create the transaction. If you use a UTXO that's already been confirmed a hundred times, and you place a relative timelock of 100 blocks on it, it will be eligible for mining immediately. Relative timelocks have some very specific uses, but they probably don't apply if your only goal is to determine some set time in the future.

### Understand How CSV Really Works

CSV has many of the same subtleties in usage as CLTV:

* The `nVersion` field must be set to 2 or more.
* The `nSequence` field must be set to less than 0x80000000.
* When CSV is run, there must be an operand on the stack that's between 0 and 0xf0000000-1.
* Both the stack operand and `nSequence` must have the same value on the 23rd bit.
* The `nSequence` must be greater than or equal to the stack operand.

Just as with CLTV, when you are respending a UTXO with a CSV in its locking conditions, you must set the `nSequence` to enable the transaction. You'll usually set it to the exact value in the locking script.

## Write a CSV Script

Just like `OP_CHECKLOCKTIMEVERIFY`, `OP_CHECKSEQUENCEVERIFY` includes an implicit `OP_VERIFY` and leaves its arguments on the stack, requiring an `OP_DROP` when you're all done.

A script that would lock funds until six months had passed following the mining of the input, and that would then require a standard P2PKH-style signature would look as follows:
```
<+6Months> OP_CHECKSEQUENCEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```

### Encode a CSV Script

When you encode a CSV script, be careful how you encode the integer value for the relative locktime. It should be passed as a 3-byte integer, which means that you're ignoring the top byte, which could inactivate the relative locktime. Since it's an integer, be sure you convert it to little-endian.

This can be done with the `integer2lehex.sh` shell script from the previous chapter.

For a relative time of 100 blocks:
```
$ ./integer2lehex.sh 100
Integer: 100
LE Hex: 64
Length: 1 bytes
Hexcode: 0164
```
Though that should be padded out to `000064`, requiring a code of `03000064`.

For a relative time of 6 months:
```
$ ./integer2lehex.sh 4224679
Integer: 4224679
LE Hex: a77640
Length: 3 bytes
Hexcode: 03a77640
```

## Spend a CSV UTXO

To spend a UTXO locked with a CSV script, you must set the `nSequence` of that input to a value greater than the requirement in the script, but less than the time between the UTXO and the present block. Yes, this means that you need to know the exact requirement in the locking script ... but you have a copy of the `redeemScript`, so if you don't know the requirements, you deserialize it, and then set the `nSequence` to the number that's shown there.

## Summary: Using CSV in Scripts

`nSequence` and CSV offer an alternative to `nLockTime` and CLTV where you lock a transaction based on a relative time since the input was mined, rather than basing the lock on a set time in the future. They work almost identically, other than the fact that the `nSequence` value is encoded slightly differently than the `nLockTime` value, with specific bits meaning specific things.

> :fire: ***What is the power of CSV?*** CSV isn't just a lazy way to lock, when you don't want to calculate a time in the future. Instead, it's a totally different paradigm, a lock that you would use if it was important to create a specific minimum duration between when a transaction is mined and when its funds can be respent. The most obvious usage is (once more) for an escrow, when you want a precise time between the input of funds and their output. However, it has much more powerful possibilities in off-chain transactions, including payment channels. These applications are by definition built on transactions that are not actually put onto the blockchain, which means that if they are later put on the blockchain an enforced time-lapse can be very helpful. [Hashed Timelock Contracts](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts) have been one such implementation, empowering the Lightning payment network. They're discussed in [§13.3: Empowering Bitcoin with Scripts](13_3_Empowering_Bitcoin_with_Scripts.md).

## What's Next?

Advance through "Bitcoin Scripting" with [Chapter Twelve: Expanding Bitcoin Scripts](12_0_Expanding_Bitcoin_Scripts.md).
