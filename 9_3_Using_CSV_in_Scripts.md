# 9.3: Using CSV in Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

`nLockTime` and `OP_CHECKLOCKTIMEVERIFY` (or CLTV) are just one side of the timelock equation. On the other are `nSequence` and `OP_CHECKSEQUENCEVERIFY`, which can be used to check against relative times rather than absolute times.

> **VERSION WARNING:** CSV became available with Bitcoin Core 0.12.1, in spring 2016.

## Understand nSequence

Every UTXO used in a transaction has an `nSequence` (or if you prefer `sequence`) value. It's been a prime tool for Bitcoin expansions as discussed previously in [§5.2: Resending a Transaction with RBF](5_2_Resending_a_Transaction_with_RBF.md) and [§6.4 Sending a Transaction with a Locktime.md](6_4_Sending_a_Transaction_with_a_Locktime.md), where it was used to signal RBF and `nLockTime`, respectively. However, there's one more use `nSequence`, which was described by [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki): you can use it to create a relative timelock on a transaction.

A relative timelock is a lock that's placed on a specific input of a transaction and that's calculated in relation to the mining date of that transaction. For example, if a UTXO was mined at block #468260 and a transaction was created where the input for that UTXO was given an `nSequence` of 100, then the new transaction could not be mined until at least block #468360.

Easy!

> **WARNING:** `nSequence` is only interpreted as a relative timelock if the transaction is marked with `nVersion` of 2 or more. This is default for Bitcoin Core starting with version 0.14.0. 

> **SEQUENCE NOTE (III):** This is the third use of the `nSequence` value in Bitcoin. Any `nSequence` value from 1 to 0xf0000000-1 will be interpreted as a relative timelock if `nVersion ≥ 2`. You should be careful to ensure that relative timelocks don't conflict with the other two uses of `nSequence`, for signalling `nTimeLock` and RBF. `nTimeLock` usually sets a value of 0xffffffff-1, where only `nTimeLock` is allowed of the three; and RBF usually sets a value of "1", where RBF and `nTimeLock` are both allowed and where a relative timelock is irrelevent, because it defines a timelock of 1 block. In general, remember: a `nVersion` value of 2 and a `nSequence` value of 0x00000001 to 0xf0000000-1 allows relative timelock, RBF, and `nTimeLock`; a `nSequence` value of 0xf0000000 to 0xffffffff-2 allows RBF and `nTimeLock`; a `nSequence` value of 0xffffffff-1 allows only `nTimeLock`; a `nSequence` value of 0xffffffff allows none; and `nVersion` can be set to 1 to disallow relative timelocks for any value of `nSequence`. Whew!

### Create a CSV Relative Block Time

The format for using `nSequence` to represent relative time locks is defined in [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) and is slightly more complex than just inputting a number, like you did for `nTimeLock`. Instead, the BIP sepecification breaks up the four byte number into three parts:

* The first two bytes are used to specify a relative locktime.
* The 23rd bit is used to signal if the lock refers to a time rather than a blockheight.
* The 32nd bit is used to signal if relative timelocks are deactivated.

If you want to create a block-based relative timelock, the construction is still quite easy


### Create a CSV Relative Time

[WARNING: NOT REALLY USED]

## Understand the CSV Opcode

### Understand How CSV Really Works

## Write a CSV Script

## Spend a CSV UTXO

## Summary: Using CSV in Scripts

