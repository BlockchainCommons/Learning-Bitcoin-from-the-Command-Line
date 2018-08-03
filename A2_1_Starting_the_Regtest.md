# 15.1: Starting the Regtest

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to build a Regtest (Regression Test) by hand to be able to develop new applications without the need to interact with other peers and blocks.
Bitcoin Core’s regression test mode (regtest mode) lets you instantly create a brand-new private block chain with the same basic rules as testnet—but one major difference: you choose when to create new blocks, so you have complete control over the environment.

## Starting Bitcoind on Regtest

After [setting up your Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md), you are now able to use Regtest. To start Bitcoind (Bitcoin Daemon) on Regtest and create a private Blockchain, you have to use the following command:
```
$ bitcoind -regtest -daemon
```

## Resetting the Regtest Blockchain

Regtest wallets and block chain state (chainstate) are saved in the regtest subdirectory of the Bitcoin configuration directory:
```
user@mybtc:~/.bitcoin# ls
bitcoin.conf  regtest  testnet3
```

If you want to start a brand new Blockchain using regtest, all you have to do is delete the `regtest` folder and restart the Bitcoind:
```
$ rm -rf regtest
```

## What's Next?

After starting your bitcoind on regtest, you can now use RPC commands to [generate/mine blocks in your private chain](15_2_Mining_with_Regtest.md).
This will allow you to get balance in your wallet and [test the Regtest blockchain](15_3_Testing_with_Regtest.md).
