# 5.1: Watching for Stuck Transactions

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Sometimes a Bitcoin transaction can get stuck. Usually it's because there wasn't sufficient transaction fee, but it can also be because of a one-time network or software glitch.

## Watch Your Transactions

You should _always_ watch to ensure that your transactions go out. `bitcoin-cli listtransactions` will show all of your incoming and outgoing transactions, while `bitcoin-cli gettransaction` with a txid will show a specific transaction. 

The following shows a transaction that has not been put into a block. You can tell this because it has no confirmations. 
```
$  bitcoin-cli -named gettransaction txid=0f618e38efe887028a5dd04d0e12241431978b4de32f70308c13a114d7cfcbd2
{
  "amount": -0.84927000,
  "fee": -0.00000400,
  "confirmations": 0,
  "trusted": true,
  "txid": "0f618e38efe887028a5dd04d0e12241431978b4de32f70308c13a114d7cfcbd2",
  "walletconflicts": [
  ],
  "time": 1491588722,
  "timereceived": 1491588722,
  "bip125-replaceable": "no",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.84927000,
      "vout": 0,
      "fee": -0.00000400,
      "abandoned": false
    }
  ],
  "hex": "0200000001c6cb05c5212bb7ec14add72883a17f0a68aa399354bc2f7cf02df6e7d68a6937000000006a47304402203f21d769cbf3cf1626ef09bb35d0d8e88efe9f14d097f4b493628e96b2e1c90b0220162ff60701525e70942f5090dc48ec2b2f3b87cd40185351ab316991567f61c50121029045eaa55d283526c723e6d5495d9b3f077b545563f86465aafcd9bfdd50359effffffff0118e20f05000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
}
```
A transaction can be considered stuck if it stays in this state for an extended amount of time. Not too many years ago, you could be sure that every transaction would go out _eventually_. But, that's no longer the case due to the increased usage of Bitcoin. Now, if a transaction is stuck too long, it will drop out of the mempool and then be lost from the Bitcoin network.

_What is the mempool?_ The Bitcoin mempool (or Memory Pool) is the pool of all unconfirmed transactions. When a miner makes a block, he uses transactions from his mempool. Then, when a block is verified, all the miners remove the transactions it contains from their pools. As of Bitcoin 0.12, unconfirmed transactions can also expire from mempools if they're old enough: typically, 72 hours or more. Mining pools might have their mempool-management mechanisms. Though the mempool should be largely the same across the Bitcoin network, there will be variations on each machine: different transactions might have propogated to a specific node or (more notably) it might have different limits on how much it's willing to store. 

This list of all [unconfirmed transactions](https://blockchain.info/unconfirmed-transactions) might not match any individual machine's mempool, but it should (mostly) be a superset of them.

## Decide What to Do

If your transaction is stuck longer than you want, you can typically do one of four things:

**1. Wait Until it Clears.** If you sent your transaction with a low or medium fee, it should eventually go through. As shown at [Bitcoin Fees](https://bitcoinfees.21.co/), transactions with fees of 100 or less satoshis per byte tend to be delayed by 25-100 blocks (up to 16 or 17 hours!), but they eventually appear in blocks.

**2. Wait Until it Expires.** If you accidentally sent with no transaction fee, or if any number or other conditions are met, then your transaction might never go through. However, your coins aren't lost. As long as you don't have a wallet that purposefully resends unconfirmed transactions, it should clear from the mempool in three days or so, and then you can try again.

**3. Use RBF as the Sender.** If you are the sender of the transaction, and you opted-in to RBF (Replace-By-Fee), then you can try again with a higher fee.

**4. Use CPFP as the Receiver.** Alternatively, if you are the receiver of the transaction, you can use CPFP (Child-pays-for-parent) to use the unconfirmed transaction as an input to a new transaction.

## Summary: Watching for Stuck Transactios

This is an introduction to the power of Bitcoin transactions. If you know that a transaction is stuck, then you can decide to free it up with features like RBF or CPFP.
