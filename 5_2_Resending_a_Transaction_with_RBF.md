# 5.2: Resending a Transaction with RBF

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

If your Bitcoin transaction is stuck, and you're sender, you can resend it using RBF (replace-by-fee).

> **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.12.0, which means that by now most miners should be using it. However, it only reached full maturity in the Bitcoin core wallet Bitcoin Core v 0.14.0.

## Opt-In for RBF

RBF is an opt-in Bitcoin feature. Transactions are only eligible for using RBF if they've been created with a special RBF flag. This is done by setting their sequence number (which is typically set automatically, so that it's less than 0xffffffff-1. (4294967294).

This is simply done by adding a `sequence` variable to the vins:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction transactions='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.1, "'$changeaddress'": 0.9 }''')
```
You should of course sign and send your transaction as usual:
```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2
```
Now, when you look at your transaction, you should see something new: the `bip125-replaceable` line, which has always been marked `no` before, is now marked `yes`:
```
$ bitcoin-cli -named gettransaction txid=7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2
{
  "amount": -0.10000000,
  "fee": 0.00000000,
  "confirmations": 0,
  "trusted": true,
  "txid": "7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2",
  "walletconflicts": [
  ],
  "time": 1491603320,
  "timereceived": 1491603320,
  "bip125-replaceable": "yes",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": 0.00000000,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0010000006b483045022100834731cd64efcc078d6c3e59cf0963599ffbc44722b7851b0404bb68e4a1fec70220759a0887ea791592c8119bbe61842eb3850a20cdf8433b4ba00d4ead752facfe012103456575f59a127a4c3e79c23f185899fa0a9ccd40162d05617fb112fa31bd14e5010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac804a5d05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
The `bip125-replaceable` flag will stay `yes` until the transaction receives confirmations. At that point it is no longer replacable.

_Should I trust transactions with no confirmations?_ No, never. This was true before RBF and it was true after RBF. Transactions must receive confirmations before they are trustworthy. This is especially true if a transaction is marked as `bip125-replaceable`, becausethen it can be ... replaced.

### Optional: Always Opt-In for RBF

If you prefer, you can _always_ opt in for RBF. Do so by running your `bitcoind` with the `-walletrbf` command or simply adding it to your bitcoin.conf file:
```
walletrbf=1
```
Once you've done this (and restarted your `bitcoind`), then all transactions should have a lower sequence number and be marked as `bip125-replaceable`.

[[THIS NEEDS TO BE FURTHER TESTED; APPARENTLY DOESN'T WORK WITH walletrbf=1 ... haven't checked with the flag]]

> **VERSION WARNING:** The walletrbf flag require Bitcoin Core v.0.14.0.

## Understand How RBF Works

The RBF functionality is based on [BIP 125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki), which lists the following rules for using RBF:

> 1. The original transactions signal replaceability explicitly or through inheritance as described in the above Summary section.

We've already done this by setting our sequence number to less than 0xffffffff-1. (4294967294).

> 2. The replacement transaction pays an absolute higher fee than the sum paid by the original transactions.
> 3. The replacement transaction does not contain any new unconfirmed inputs that did not previously appear in the mempool. (Unconfirmed inputs are inputs spending outputs from currently unconfirmed transactions.)
> 4. The replacement transaction must pay for its own bandwidth in addition to the amount paid by the original transactions at or above the rate set by the node's minimum relay fee setting. For example, if the minimum relay fee is 1 satoshi/byte and the replacement transaction is 500 bytes total, then the replacement must pay a fee at least 500 satoshis higher than the sum of the originals.
> 5. The number of original transactions to be replaced and their descendant transactions which will be evicted from the mempool must not exceed a total of 100 transactions.

_What is a BIP?_ A BIP is a Bitcoin Improvement Proposal. It's an in-depth suggestion for a change to the Bitcoin Core code. Often, when a BIP has been sufficiently discussed and updated, it will become an actual part of the Bitcoin Core code. For example, BIP 125 was implemented in Bitcoin Core 0.12.0.

The other thing to understand about RBF is that in order to use it, you must double-spend, reusing one or more the same UTXOs. Just sending another transaction with a different UTXO to the same recipient won't do the trick (and will likely result in your losing money. Instead, you must purposefully create a conflict, where the same UTXO is used in two different transactions. 

Faced with this conflict, the miners will know to use the one with the higher sequence number, and they'll be incentivized to do so by the higher fees which are promised as part of the BIP.

_What is a double-spend?_ A double-spend occurs when someone sends the same electronic funds to two different people (or, to the same person twice, in two different transactions). This is a central problem in any e-cash system. It's solved in Bitcoin by the immutable ledger: once a transaction is sufficiently confirmed, no miners will verify transactions that reuse the same UTXO. However, it's possible to double-spend _before_ a transaction has been confirmed — which is why you always want one or more confirmations before you finalize a transaction. In the case of RBF, you purposefully double-spend because an initial transaction has stalled, and the miners accept it if you meet the specific criteria laid out by BIP 125.

## Replace a Transaction the Hard Way: By Hand

In order to create an RBF transaction by hand, all you have to do is create a raw transaction: (1) that replaces a previous raw transaction that opted-in to RBF and that is not confirmed; (2) that reuses one or more of the same UTXOs; (3) that includes a higher sequence number than the previous use of the UTXO; and (4) that increases fees.

The following example just reuses our existing variables, but increments the sequence number and decreases the amount sent to the change address, to increase the fee from the accidental 0 BTC of the original transaction to an overly generous 0.01 BTC in the new transaction:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction transactions='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 2 } ]''' outputs='''{ "'$recipient'": 0.1, "'$changeaddress'": 0.89 }''')
```
We of course my resign it and resend it:
```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39
```
After several blocks have been created, the original transaction continues to hang around:
```
$ bitcoin-cli -named gettransaction txid=7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2
{
  "amount": -0.10000000,
  "fee": 0.00000000,
  "confirmations": -5,
  "trusted": false,
  "txid": "7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2",
  "walletconflicts": [
    "959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39"
  ],
  "time": 1491603320,
  "timereceived": 1491603320,
  "bip125-replaceable": "yes",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": 0.00000000,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0010000006b483045022100834731cd64efcc078d6c3e59cf0963599ffbc44722b7851b0404bb68e4a1fec70220759a0887ea791592c8119bbe61842eb3850a20cdf8433b4ba00d4ead752facfe012103456575f59a127a4c3e79c23f185899fa0a9ccd40162d05617fb112fa31bd14e5010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac804a5d05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
Note that `bitcoin-cli` recognizes that there's a conflict with another transaction in the `walletconflicts` section. Also note that it's listed with _negative confirmations_, which marks how long it's been since the double-spend was confirmed.

Meanwhile, the new transaction worked fine:
```
$ bitcoin-cli -named gettransaction txid=959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39
{
  "amount": -0.10000000,
  "fee": -0.01000000,
  "confirmations": 5,
  "blockhash": "00000000000006eeb468791e5ee0d86613c03acd871ef7d89c25fd28474754d5",
  "blockindex": 20,
  "blocktime": 1491603862,
  "txid": "959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39",
  "walletconflicts": [
    "7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2"
  ],
  "time": 1491603673,
  "timereceived": 1491603673,
  "bip125-replaceable": "no",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.01000000,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0010000006a47304402207fea4a11db8576257b9d9e104aa07cb3d3ae6a42e38dd7126111276ca5b45daa0220594a3553cc278c43fd015b35029d5b9596d4ac9f36d3d20fb1a8c9efface5c50012103456575f59a127a4c3e79c23f185899fa0a9ccd40162d05617fb112fa31bd14e5020000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac40084e05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
Our recipients have their money, and the original, failed transaction will eventually fall out of the mempool.

## Replace a Transaction the Easy Way: By bumpfee

> **VERSION WARNING:** The bumpfee RPC require Bitcoin Core v.0.14.0.

### Use RBF Wisely

-low to high transaction fees
-combine transactions

It doesn't solve the same problem. Core devs are looking for compressing transactions, not necessarily adding fees alone.
