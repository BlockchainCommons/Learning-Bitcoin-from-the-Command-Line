# 15.3: Mining the Regtest

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to generate (i.e. mine) blocks using a Regtest (Regression Test) blockchain.
To generate the Genesis block and the next blocks on a new blockchain requires very minimal proof-of-work, due to the low difficulty and that it follows the Testnet rules.


## Generating blocks

You can generate/mine new blocks using the RPC method `generate`. This method is only available in the Regtest mode, using the following command:
```
$ bitcoin-cli -regtest generate 101
```

This command will generate 101 blocks using a special RPC which is only available in regtest mode. This takes less than a second on a generic PC. 
Because this is a new block chain using Bitcoin’s default rules, the first blocks pay a block reward of 50 bitcoins. 
Unlike mainnet, in regtest mode only the first 150 blocks pay a reward of 50 bitcoins. 
However, a block must have 100 confirmations before that reward can be spent, so we generate 101 blocks to get access to the coinbase transaction from block #1.

