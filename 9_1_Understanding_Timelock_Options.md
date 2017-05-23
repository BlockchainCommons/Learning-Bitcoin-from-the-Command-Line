# 9.1: Understanding Timelock Options

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

In [§6.4: Sending a Transaction with a Locktime](6_4_Sending_a_Transaction_with_a_Locktime.md), `nLocktime` provided a great basis for locking transactions so that they couldn't be spent until some point in the future — based either on time or blockheight. But, that's not the only way to put a timelock on a transaction.

## Understand the Limitations of nLockTime

1. [whole transaction]
2. [can't even be put on network]

## Understand the Possibilities of Timelock Scripts

[a script locks a single output]
[can be used for as part of more complex redemption conditions]
- largely depend on conditionals in next chapter

here's a catch: [can't be negative; make available in the future, but not the converse]

### CLTV

[like nLockTime, but for a single output]

### CSV

[relative time]

## Summary: Understanding Timelock Options
