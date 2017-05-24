# 9.2: Using CLTV in Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

`OP_CHECKLOCKTIMEVERIFY` (or CLTV) is the natural complement to `nLockTime`. It moves the idea of locking transactions by an absolute time or blockheight into the realm of opcodes, allowing for the locking of individual UTXOs.

## Differentiate nLockTime and CLTV

Before digging into CLTV, we should first recall how `nLockTime` works.

### Remember nLockTime

As detailed in [§6.4: Sending a Transaction with a Locktime](6_4_Sending_a_Transaction_with_a_Locktime.md), locktime is enabled by setting two variables, the `nLockTime` and the `nSequence`. The `nSequence` must be set to less than 0xffffffff, then the `nLockTime` is interpreted as follows:

* If the `nLockTime` is less than 500 million, it is interpreted as a blockheight.
* If the `nLockTime` is 500 million or more, it is interpreted as a UNIX timestamp.

The transaction can not be spent (or even put on the block chain) until either the blockheight or time is reached. In the meantime, the transaction can be cancelled by respending any of the UTXOs that make up the transaction.

### Meet CLTV

`OP_CHECKLOCKTIMEVERIFY` works within the same paradigm of absolute blockheights or absolute UNIX times, but it runs as part of a Bitcoin Script. It reads one argument, which can be a blockheight or an absolute UNIX time. Through a somewhat convoluted methodology, it compares that argument to the current time. If it's too early, the script fails, but if the time condition has met, the script carries on.

This is how a `OPCHECKLOCKTIMEVERIFY` might be used to check against May 24, 2017:
```
1495652013 OP_CHECKLOCKTIME VERIFY
```
But we'll usually depict this in an abstraction like:
```
<May 24> OP_CHECKLOCKTIME VERIFY
```
This is how [blockheight]



## Write a CLTV

## Spend a CLTV UTXO

## Summary: Using CLTV in Scripts
