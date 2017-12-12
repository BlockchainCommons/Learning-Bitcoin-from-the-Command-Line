# 15.2: Testing with Regtest

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to test a Regtest (Regression Test).


## Verifying balance

After [mining blocks](15_3_Mining_with_Regtest.md) and getting the rewards, you can verify the balance on your wallet:
```
$ bitcoin-cli -regtest getbalance
```

## Testing the Regtest
Now you should be able to use this balance for any type of interaction with the private Blockchain, such as sending Bitcoin transactions according to [Chapter 4]((04_0_Sending_Bitcoin_Transactions.md)) in this guide.

It is important to note that for your transactions to complete, you will have to generate/mine new blocks so that the transactions can be included into them.
