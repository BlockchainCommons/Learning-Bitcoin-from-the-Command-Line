# 9.3: Using CSV in Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

`nLockTime` and `OP_CHECKLOCKTIMEVERIFY` (or CLTV) are just one side of the timelock equation. On the other are `nSequence` and `OP_CHECKSEQUENCEVERIFY`, which can be used to check against relative times rather than absolute times.

> **VERSION WARNING:** CSV became available with Bitcoin Core 0.12.1, in spring 2016.

## Understand nSequence

Every UTXO used in a transaction has an `nSequence` (or if you prefer `sequence`) value. It's been a prime tool for Bitcoin expansions. So, if it's not set to 0xffffffff, that's a mark that an unconfirmed transaction is eligible for RBF, as discussed in [§5.2: Resending a Transaction with RBF](5_2_Resending_a_Transaction_with_RBF.md), with increases in the `nSequence` showing progressively newer replacement transactions. Similarly, a value of less than 0xffffffff tells Bitcoin to pay attention to `nLockTime`, as described in [§6.4 Sending a Transaction with a Locktime.md](6_4_Sending_a_Transaction_with_a_Locktime.md).

There's one more use `nSequence`, which was described by [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki). 

...


### Create a CSV Relative Time

### Create a CSV Relative Block Time

## Understand the CSV Opcode

### Understand How CSV Really Works

## Write a CSV Script

## Spend a CSV UTXO

## Summary: Using CSV in Scripts

