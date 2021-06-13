# 5.1: Watching for Stuck Transactions

Sometimes a Bitcoin transaction can get stuck. Usually it's because there wasn't sufficient transaction fee, but it can also be because of a one-time network or software glitch.

## Watch Your Transactions

You should _always_ watch to ensure that your transactions go out. `bitcoin-cli listtransactions` will show all of your incoming and outgoing transactions, while `bitcoin-cli gettransaction` with a txid will show a specific transaction. 

The following shows a transaction that has not been put into a block. You can tell this because it has no confirmations. 
```
$ bitcoin-cli -named gettransaction txid=fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e
{
  "amount": -0.00020000,
  "fee": -0.00001000,
  "confirmations": 0,
  "trusted": true,
  "txid": "fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e",
  "walletconflicts": [
  ],
  "time": 1592953220,
  "timereceived": 1592953220,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00020000,
      "vout": 0,
      "fee": -0.00001000,
      "abandoned": false
    }
  ],
  "hex": "02000000014cda1f42a1bd39d8d0ff5958a804bc2bc548b71d7ceadbde53ea15aeaf1e2691000000006a473044022016a7a9f045a0f6a52129f48adb7da35c2f54a0741d6614e9d55b8a3bc3e1490a0220391e9085a3697bc790e94bb924d5310e16f23489d9c600864a32674e871f523c01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff02204e000000000000160014751e76e8199196d454941c45d1b3a323f1433bd6e8030000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919600000000"
```
A transaction can be considered stuck if it stays in this state for an extended amount of time. Not too many years ago, you could be sure that every transaction would go out _eventually_. But, that's no longer the case due to the increased usage of Bitcoin. Now, if a transaction is stuck too long, it will drop out of the mempool and then be lost from the Bitcoin network.

> :book: ***What is mempool?*** Mempool (or Memory Pool) is a pool of all unconfirmed transactions at a bitcoin node. These are the transactions that a node has received from the p2p network which are not yet included in a block. Each bitcoin node can have a slightly different set of transactions in its mempool: different transactions might have propogated to a specific node. This depends on when the node was last started and also its limits on how much it's willing to store. When a miner makes a block, he uses transactions from his mempool. Then, when a block is verified, all the miners remove the transactions it contains from their pools. As of Bitcoin 0.12, unconfirmed transactions can also expire from mempools if they're old enough, typically, 72 hours, and as of version 0.14.0 eviction time was increased to 2 weeks. Mining pools might have their own mempool-management mechanisms.

This list of all [unconfirmed transactions](https://blockchain.info/unconfirmed-transactions) might not match any individual machine's mempool, but it should (mostly) be a superset of them.

## Decide What to Do

If your transaction is stuck longer than you want, you can typically do one of four things:

**1. Wait Until it Clears.** If you sent your transaction with a low or medium fee, it should eventually go through. As shown at [Mempool Space](https://mempool.space), those with lower fees _will_ get delayed. (Take a look at the leftmost transaction, and see how long it's been waiting and how much it paid for its fee.)

**2. Wait Until it Expires.** If you accidentally sent with no transaction fee, or if any number or other conditions are met, then your transaction might never go through. However, your coins aren't lost. As long as you don't have a wallet that purposefully resends unconfirmed transactions, it should clear from the mempool in three days or so, and then you can try again.

**3. Use RBF as the Sender.** If you are the sender of the transaction, and you opted-in to RBF (Replace-By-Fee), then you can try again with a higher fee. See [§5.2: Resending a Transaction with RBF](05_2_Resending_a_Transaction_with_RBF.md).

**4. Use CPFP as the Receiver.** Alternatively, if you are the receiver of the transaction, you can use CPFP (Child-pays-for-parent) to use the unconfirmed transaction as an input to a new transaction. See [§5.3: Funding a Transaction with CPFP](05_3_Funding_a_Transaction_with_CPFP.md).

## Summary: Watching for Stuck Transactions

This is an introduction to the power of Bitcoin transactions. If you know that a transaction is stuck, then you can decide to free it up with features like RBF or CPFP.

## What's Next?

Continue "Controlling Bitcoin Transactions" with [§5.2: Resending a Transaction with RBF](05_2_Resending_a_Transaction_with_RBF.md).  
