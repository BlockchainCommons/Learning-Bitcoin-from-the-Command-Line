# 6.1: Watching for Stuck Transactions

Sometimes a Bitcoin transaction can get stuck. Usually it's because there wasn't sufficient transaction fee, but it can also be because of a one-time network or software glitch.

## Watch Your Transactions

You should _always_ watch to ensure that your transactions go out. `bitcoin-cli listtransactions` will show all of your incoming and outgoing transactions, while `bitcoin-cli gettransaction` with a txid will show a specific transaction. 

The following shows a transaction that has not been put into a block. You can tell this because it has no confirmations. 

```sh
bitcoin-cli -named gettransaction txid=1050f6e2d68e7e4555bd682f24157f9bb19f666cf2ba1d1b3ff959ff4f893654

| {
|   "amount": -0.00100000,
|   "fee": -0.00000141,
|   "confirmations": 0,
|   "trusted": true,
|   "txid": "1050f6e2d68e7e4555bd682f24157f9bb19f666cf2ba1d1b3ff959ff4f893654",
|   "wtxid": "d98414f5627f10337c9c2f4ff096a4ec5d15b91898ebf3fac3fa21b9ad299740",
|   "walletconflicts": [
|   ],
|   "mempoolconflicts": [
|   ],
|   "time": 1773350380,
|   "timereceived": 1773350380,
|   "bip125-replaceable": "yes",
|   "details": [
|     {
|       "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
|       "category": "send",
|       "amount": -0.00100000,
|       "vout": 1,
|       "fee": -0.00000141,
|       "abandoned": false
|     }
|   ],
|   "hex": "02000000000101b345bc2c6d1410c949c25edbdd96eecea02a0675f2c561d473b8a58b8e88a8735d01000000fdffffff02741b06000000000016001499981dc044f0b3f893d27cb4b246dd65ffd8bde7a086010000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac90247304402200391dde68ab21714f3ff352a4ef0c38b038d5bf6695787ad2c0583dc5b80b7110220581985bb88df2b452e188a60a558b2b26d47d1e0adc9c60d7d7eb569419e9d920121021b78fa3ed908a1ed1c2623a2311015b347de707d507d147dc0e83d84ec06255200000000",
|   "lastprocessedblock": {
|     "hash": "0000000d73256bcfb06de6e894d8f9e481af1143ac60a072cbeb1878ef7cacf7",
|     "height": 295310
|   }
| }
```

A transaction can be considered stuck if it stays in this state for an extended amount of time. Early on, you could be sure that every transaction would go out _eventually_. But, that's no longer the case due to the increased usage of Bitcoin. Now, if a transaction is stuck too long, it will drop out of the mempool and then be lost from the Bitcoin network.

> 📖 ***What is mempool?*** Mempool (or Memory Pool) is a pool of all unconfirmed transactions at a bitcoin node. These are the transactions that a node has received from the peer-to-peer network which are not yet included in a block. Each bitcoin node can have a slightly different set of transactions in its mempool: different transactions might have propogated to a specific node. This depends on when the node was last started and also its limits on how much it's willing to store. When a miner makes a block, they use transactions from their own mempool. Then, when a block is verified, all the miners remove the transactions that block contains from their individual pools. Most mempools will not keep unconfirmed transactions for longer than two weeks, and that number could drop dramatically (to as low as a few days) in times of congestion.

This list of all [unconfirmed transactions on the mainnet](https://blockchain.info/unconfirmed-transactions) might not match any individual machine's mempool, but it should (mostly) be a superset of them.

## Decide What to Do

If your transaction is stuck longer than you want, you can typically do one of four things:

**1. Wait Until it Clears.** If you sent your transaction with a low or medium fee, it should eventually go through. As shown at [Mempool Space](https://mempool.space), those with lower fees _will_ get delayed. (Take a look at the leftmost transaction, and see how long it's been waiting and how much it paid for its fee.)

**2. Wait Until it Expires.** If you accidentally sent with no transaction fee, or if any number or other conditions are met, then your transaction might never go through. However, your coins aren't lost. As long as you don't have a wallet that purposefully resends unconfirmed transactions, it should eventually clear from the mempool, and then you can try again.

**3. Use RBF as the Sender.** If you are the sender of the transaction, and you opted-in to RBF (Replace-By-Fee), then you can try again with a higher fee. See [§6.2: Resending a Transaction with RBF](06_2_Resending_a_Transaction_with_RBF.md).

**4. Use CPFP as the Receiver.** Alternatively, if you are the receiver of the transaction, you can use CPFP (Child-pays-for-parent) to use the unconfirmed transaction as an input to a new transaction. See [§6.3: Funding a Transaction with CPFP](06_3_Funding_a_Transaction_with_CPFP.md)

## Summary: Watching for Stuck Transactions

This is an introduction to the power of Bitcoin transactions. If you know that a transaction is stuck, then you can decide to free it up with features like RBF or CPFP.

| Role | Feature | Section |
|-------|----------|-----|
| ✉️ Sender | RBF | §6.2 |
| 📨 Recipient | CPFP | §6.3 |

## What's Next?

Continue "Controlling Bitcoin Transactions" with [§6.2: Resending a
Transaction with RBF](06_2_Resending_a_Transaction_with_RBF.md).
