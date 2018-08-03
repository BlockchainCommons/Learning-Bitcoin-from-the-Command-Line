# 15.2: Mining the Regtest

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to generate (i.e. mine) blocks using a Regtest (Regression Test) blockchain.
To generate blocks on a new blockchain requires very minimal proof-of-work and it will take less than a second, due to the low difficulty and that it follows the Testnet rules.


## Generating blocks

You can generate/mine new blocks using the RPC method `generate`. It only makes sense to use this method on regtest, due to the high difficulty it's very unlikely that it will yield to new blocks in the mainnet or testnet:
```
$ bitcoin-cli -regtest generate 101
[
  "57f17afccf28b9296048b6370312678b6d8e48dc3a7b4ef7681d18ed3d91c122",
  "631ff7b8135ce633c774828be3b8505726459eb65c339aab981b10363befe5a7",
  ...
  "1162dbfe025c7da94ee1128dc26d518a94508f532c19edc0de6bc673a909d02c",
  "20cb2e815c3d42d6a117a204a0b5e726ab641c826e441b5b3417aca33f2aba48"
]
```
The output is the block hash of every block generated (in our example, 101 hashes).


This command will generate 101 blocks using a special RPC to generate the blocks on your regtest network. Running this command only makes sense on the regtest, if you try to run on the mainnet or testnet, it is very unlikely that it will be able to yield any block. On regtest, this takes less than a second on a generic PC.
Because this is a new block chain using Bitcoin’s default rules, the first blocks pay a block reward of 50 bitcoins.
Unlike mainnet, in regtest mode only the first 150 blocks pay a reward of 50 bitcoins. After that, the reward halves after 150 blocks, so it pays 25, 12.5, and so on...

However, a block must have 100 confirmations before that reward can be spent, so we generate 101 blocks to get access to the coinbase transaction from block #1.


## What's Next?

After starting your bitcoind on regtest and generating the first blocks, you have balance in your address to spend and [test using Regtest blockchain](15_3_Testing_with_Regtest.md).
