# 5.5: Sending a Raw Transaction with a Locktime

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The `bitcoin-cli` interface for creating raw transactions has one last option that we haven't covered yet: locktime. This gives you the ability to send raw transactions at some time in the future.

## Understand How Locktime Works

When you create a locktime transaction, you include a number that represents either a block height (if it's a small number) or a UNIX timestamp (if it's a big number). This tells the Bitcoin network that the transaction may not be put into a block until either the specified time has arrived or the blockchain has reached the specified height. 

_What is nLockTime?_ It's the same thing as locktime. More specifically, it's what locktime is called internal to the Bitcoin Core source code.

When a locktime transaction is waiting to go into a block, it can be cancelled. This means that it is far, far from finalized. In fact, the ability to cancel is the whole purpose of a locktime transaction.

## Create a Locktime Transaction

In order to create a locktime transaction, you need to first determine what you will set the locktime to.

Most frequently you will set the locktime to a UNIX timestamp representing a specific date and time. You can do this at a web site like [UNIX Time Stamp](http://www.unixtimestamp.com/) or [Epoch Convertor](https://www.epochconverter.com/). However, it'd be better to write your own script on your local machine, so that you know the UNIX timestamp you receive is accurate. If you don't do that, at least double check on two different sites.

_Why Would I Use a UNIX Timestamp?_ Using a UNIX timestamp makes it easy to definitively link a transaction to a specific time, without worrying about whether the speed of block creation might change at some point. Particularly if you're creating a locktime that's far in the future, it's the safer thing to do. But, beyond that, it's just more intuitive, creating a direct correlation between some calendar date and the time when the transaction can be mined.

> **WARNING:** When using UNIX timestamps for locktime, there's a bit of wriggle room: the release of blocks isn't regular and block times can be two hours ahead of real time, so the locktime usually means "within a few hours of this time, plus or minus".

Alternatively, you can set the locktime to a smaller number representing a block height. [figure out current][10/]

[500M warning]
### Figure Out Your Locktime

==
Even today, setting all sequence numbers to 0xffffffff (the default in Bitcoin Core) can still disable the time lock, so if you want to use locktime, at least one input must have a sequence number below the maximum. Since sequence numbers are not used by the network for any other purpose, setting any sequence number to zero is sufficient to enable locktime.
==

### Write Your Transaction

[Note Sequence Number]

### Send Your Transaction

## Cancel a Locktime Transaction

## Summary: Sending a Raw Transaction with a Locktime

