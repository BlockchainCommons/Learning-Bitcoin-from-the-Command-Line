# 15.1: Building the Regtest

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to build a Regtest (Regression Test) by hand to be able to develop new applications without the need to interact with other peers and blocks.
Bitcoin Core’s regression test mode (regtest mode) lets you instantly create a brand-new private block chain with the same basic rules as testnet—but one major difference: you choose when to create new blocks, so you have complete control over the environment.

## Starting Bitcoind in Regtest Mode

To start your bitcoind (Bitcoin Daemon) in Regtest mode and create a private Blockchain, you have to use the following command:
```
$ bitcoind -regtest -daemon
```

## Generating blocks

You can generate/mine new blocks using the RPC method `generate`. This method is only available in the Regtest mode, using the following command:
```
$ bitcoin-cli -regtest generate 101
```

This command will generate 101 blocks using a special RPC which is only available in regtest mode. This takes less than a second on a generic PC. 
Because this is a new block chain using Bitcoin’s default rules, the first blocks pay a block reward of 50 bitcoins. 
Unlike mainnet, in regtest mode only the first 150 blocks pay a reward of 50 bitcoins. 
However, a block must have 100 confirmations before that reward can be spent, so we generate 101 blocks to get access to the coinbase transaction from block #1.

## Verifying balance

After mining blocks and getting the reward for the first one, you can verify the balance on your wallet:
```
$ bitcoin-cli -regtest getbalance
```

Now you can use this balance for any kind of interaction with the private Blockchain, such as sending Bitcoin transactions according to [Chapter 4]((04_0_Sending_Bitcoin_Transactions.md)) in this guide.
