# 5.2: Resending a Transaction with RBF

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

If your Bitcoin transaction is stuck, and you're sender, you can resend it using RBF (replace-by-fee).

> **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.12.0, which means that by now most miners should be using it. However, it only reached full maturity in the Bitcoin core wallet Bitcoin Core v 0.14.0.

## Opt-In for RBF

RBF is an opt-in Bitcoin feature. Transactions are only eligible for using RBF if they've been created with a special RBF flag. This is done by setting their sequence number (which is typically set automatically, so that it's more than 0 and less than 0xffffffff (4294967295).

This is simply done by adding a `sequence` variable to the vins:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction transactions='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 2.9, "'$changeaddress'": 1.0 }''')
```
You should of course sign and send your transaction as usual:
```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
4075dbf84303c01adcb0b36cd2c164e2b447192c2d9fbf5fde3b99d0ac7e64b6
```
Now, when you look at your transaction, you should see something new: the `bip125-replaceable` line, which has always been marked `no` before, is now marked `yes`:
```
$ bitcoin-cli gettransaction 4075dbf84303c01adcb0b36cd2c164e2b447192c2d9fbf5fde3b99d0ac7e64b6
{
  "amount": -2.90000000,
  "fee": 0.00000000,
  "confirmations": 0,
  "trusted": true,
  "txid": "4075dbf84303c01adcb0b36cd2c164e2b447192c2d9fbf5fde3b99d0ac7e64b6",
  "walletconflicts": [
  ],
  "time": 1491597044,
  "timereceived": 1491597044,
  "bip125-replaceable": "yes",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -2.90000000,
      "vout": 0,
      "fee": 0.00000000,
      "abandoned": false
    }
  ],
  "hex": "020000000100e3da9fa8b5368dd383ebabb820ae257d8f19bb1a9e3fb36f6c3e1b83e6d0f3000000006b483045022100950af7346a5bff5546627b6e9379573cb7e56420d6187f90a8b7aab8fcad730502200c4e66b96b499fec020f9397861bda765c29ec873fc8e61f85a32c73e650173b0121031008e55d06d25bb2acdf85395fe4c2df28492325d8ffc3734a2dc372436b6a330100000002800c4911000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00e1f505000000001976a914d83b41b8f67b31310bb487b66d859a6d0a92682c88ac00000000"
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

> **VERSION WARNING:** The walletrbf flag require Bitcoin Core v.0.14.0.

## Replace a Transaction the Hard Way: By Hand

### Use RBF Wisely

-low to high transaction fees
-combine transactions

It doesn't solve the same problem. Core devs are looking for compressing transactions, not necessarily adding fees alone.

## Replace a Transaction the Easy Way: By bumpfee

> **VERSION WARNING:** The bumpfee RPC require Bitcoin Core v.0.14.0.
