# 5.2: Resending a Transaction with RBF

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Sometimes a Bitcoin transaction can get stuck. Usually it's because there wasn't sufficient transaction fee, but it can also be because of a one-time network or software glitch.

## Watch for Stuck Transactions

You should _always_ watch to ensure that your transactions go out. `bitcoin-cli listunspent` will show all of your incoming and outgoing transactions, while `bitcoin-cli gettransaction` with a txid will show a specific transaction. 

The following shows a transaction that has not been put into a block. Note that it has no confirmations. 
```
$ bitcoin-cli gettransaction 0f618e38efe887028a5dd04d0e12241431978b4de32f70308c13a114d7cfcbd2
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
A transaction can be considered stuck if it stays in this state for an extended amount of time. Not too many years ago, you could be sure it would go out _eventually_. But, that's nolonger the case due to the increased usage of the Bitcoin network. Now, if a Bitcoin transaction is stuck for too long, it will eventually drift out of the mempool, and then it will _never_ get placed in a block.

_What is the mempool?_ The Bitcoin mempool (or Memory Pool) is the pool of all the unconfirmed transactions. Miners pull from it 

## Set Your Transaction Fee

1. Abandon, then rebuild, fulfilling all outputs
2. Use bumpfee
