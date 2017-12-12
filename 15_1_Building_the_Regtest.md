# 15.1: Building the Regtest

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to build a Regtest (Regression Test) by hand to be able to develop new applications without the need to interact with other peers and blocks.
Bitcoin Core’s regression test mode (regtest mode) lets you instantly create a brand-new private block chain with the same basic rules as testnet—but one major difference: you choose when to create new blocks, so you have complete control over the environment.

## Starting Bitcoind in Regtest Mode

To start your Bitcoind (Bitcoin Daemon) in Regtest mode and create a private Blockchain, you have to use the following command:
```
$ bitcoind -regtest -daemon
```

## What's next

After starting your bitcoind in the Regtest mode, you can now use Regtest-specific RPC commands to [generate/mine blocks in your private chain](15_3_Mining_with_Regtest.md).
This will allow you to get balance into your wallet and [test the Regtest blockchain](15_2_Testing_with_Regtest.md).

