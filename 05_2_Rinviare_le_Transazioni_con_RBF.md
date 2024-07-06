# 5.2: Resending a Transaction with RBF

If your Bitcoin transaction is stuck, and you're the sender, you can resend it using RBF (replace-by-fee). However, that's not all that RBF can do: it's generally a powerful and multipurpose feature that allows Bitcoin senders to recreate transactions for a variety of reasons.

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.12.0, that reached full maturity in the Bitcoin Core wallet with Bitcoin Core v 0.14.0. Obviously, most people should be using it by now.

## Opt-In for RBF

RBF is an opt-in Bitcoin feature. Transactions are only eligible for using RBF if they've been created with a special RBF flag. This is done by setting any of the transaction's UTXO sequence numbers (which are typically set automatically), so that it's more than 0 and less than 0xffffffff-1 (4294967294).

This is accomplished simply  by adding a `sequence` variable to your UTXO inputs:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.00007658, "'$changeaddress'": 0.00000001 }''')
```
You should of course sign and send your transaction as usual:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
```
Now, when you look at your transaction, you should see something new: the `bip125-replaceable` line, which has always been marked `no` before, is now marked `yes`:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375                                                                     
      
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": true,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
The `bip125-replaceable` flag will stay `yes` until the transaction receives confirmations. At that point, it is no longer replaceable.

> :book: ***Should I trust transactions with no confirmations?*** No, never. This was true before RBF and it was true after RBF. Transactions must receive confirmations before they are trustworthy. This is especially true if a transaction is marked as `bip125-replaceable`, because then it can be ... replaced.

> :information_source: **NOTE — SEQUENCE:** This is the first use of the `nSequence` value in Bitcoin. You can set it between 1 and 0xffffffff-2 (4294967293) and enable RBF, but if you're not careful you can run up against the parallel use of `nSequence` for relative timelocks. We suggest always setting it to "1", which is what Bitcoin Core does, but the other option is to set it to a value between 0xf0000000 (4026531840) and 0xffffffff-2 (4294967293). Setting it to "1" effectively makes relative timelocks irrelevent and setting it to 0xf0000000 or higher deactivates them. This is all explained further in [§11.3: Using CSV in Scripts](11_3_Using_CSV_in_Scripts.md). For now, just choose one of the non-conflicting values for `nSequence`.

### Optional: Always Opt-In for RBF

If you prefer, you can _always_ opt in for RBF. Do so by running your `bitcoind` with the `-walletrbf` command. Once you've done this (and restarted your `bitcoind`), then all UTXOs should have a lower sequence number and the transaction should be marked as `bip125-replaceable`.

> :warning: **VERSION WARNING:** The walletrbf flag require Bitcoin Core v.0.14.0.

## Understand How RBF Works

The RBF functionality is based on [BIP 125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki), which lists the following rules for using RBF:

> 1. The original transactions signal replaceability explicitly or through inheritance as described in the above Summary section.

This means that the sequence number must be set to less than 0xffffffff-1. (4294967294), or the same is true of unconfirmed parent transactions.

> 2. The replacement transaction pays an absolute higher fee than the sum paid by the original transactions.
> 3. The replacement transaction does not contain any new unconfirmed inputs that did not previously appear in the mempool. (Unconfirmed inputs are inputs spending outputs from currently unconfirmed transactions.)
> 4. The replacement transaction must pay for its own bandwidth in addition to the amount paid by the original transactions at or above the rate set by the node's minimum relay fee setting. For example, if the minimum relay fee is 1 satoshi/byte and the replacement transaction is 500 bytes total, then the replacement must pay a fee at least 500 satoshis higher than the sum of the originals.
> 5. The number of original transactions to be replaced and their descendant transactions which will be evicted from the mempool must not exceed a total of 100 transactions.

> :book: ***What is a BIP?*** A BIP is a Bitcoin Improvement Proposal. It's an in-depth suggestion for a change to the Bitcoin Core code. Often, when a BIP has been sufficiently discussed and updated, it will become an actual part of the Bitcoin Core code. For example, BIP 125 was implemented in Bitcoin Core 0.12.0.

The other thing to understand about RBF is that in order to use it, you must double-spend, reusing one or more the same UTXOs. Just sending another transaction with a different UTXO to the same recipient won't do the trick (and will likely result in your losing money). Instead, you must purposefully create a conflict, where the same UTXO is used in two different transactions. 

Faced with this conflict, the miners will know to use the one with the higher fee, and they'll be incentivized to do so by that higher fee.

> :book: ***What is a double-spend?*** A double-spend occurs when someone sends the same electronic funds to two different people (or, to the same person twice, in two different transactions). This is a central problem for any e-cash system. It's solved in Bitcoin by the immutable ledger: once a transaction is sufficiently confirmed, no miners will verify transactions that reuse the same UTXO. However, it's possible to double-spend _before_ a transaction has been confirmed — which is why you always want one or more confirmations before you finalize a transaction. In the case of RBF, you purposefully double-spend because an initial transaction has stalled, and the miners accept your double-spend if you meet the specific criteria laid out by BIP 125.

> :warning: **WARNING:** Some early discussions of this policy suggested that the `nSequence` number also be increased. This in fact was the intended use of `nSequence` in its original form. This is _not_ a part of the published policy in BIP 125. In fact, increasing your sequence number can accidentally lock your transaction with a relative timelock, unless you use sequence numbers in the range of 0xf0000000 (4026531840) to 0xffffffff-2 (4294967293).

## Replace a Transaction the Hard Way: By Hand

In order to create an RBF transaction by hand, all you have to do is create a raw transaction that: (1) replaces a previous raw transaction that opted-in to RBF and that is not confirmed; (2) reuses one or more of the same UTXOs; (3) increases fees; and (4) pays the minimum bandwidth of both transactions [which already may be taken care of by (3)].

The following example just reuses our existing variables, but decreases the amount sent to the change address, to increase the fee from the accidental 0 BTC of the original transaction to an overly generous 0.01 BTC in the new transaction:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.000075, "'$changeaddress'": 0.00000001 }''')
```
We of course must re-sign it and resend it:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
```
You can now look at your original transaction and see that it has `walletconflicts`:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
This represents the fact that two different transactions are both trying to use the same UTXO. 

Eventually, the transaction with the larger fee should be accepted:
```
$ bitcoin-cli -named gettransaction txid=c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
{
  "amount": 0.00000000,
  "fee": -0.00000299,
  "confirmations": 2,
  "blockhash": "0000000000000055ac4b6578d7ffb83b0eccef383ca74500b00f59ddfaa1acab",
  "blockheight": 1773266,
  "blockindex": 9,
  "blocktime": 1592955002,
  "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
  "walletconflicts": [
    "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375"
  ],
  "time": 1592954467,
  "timereceived": 1592954467,
  "bip125-replaceable": "no",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b010000000001000000024c1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077dcdd98d85f6247450185c2b918a0f434d9b2e647330d741944ecae60d6ff790220424f85628cebe0ffe9fa11029b8240d08bdbfcc0c11f799483e63b437841b1cd0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Meanwhile, the original transaction with the lower fee starts picking up negative confirmations, to show its divergence from the blockchain:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": -2,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Our recipients have their money, and the original, failed transaction will eventually fall out of the mempool.

## Replace a Transaction the Easy Way: By bumpfee

Raw transactions are very powerful, and you can do a lot of interesting things by combining them with RBF. However, sometimes _all_ you want to do is free up a transaction that's been hanging. You can now do that with a simple command, `bumpfee`.

For example, to increase the fee of transaction `4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927` you would run:
```
$ bitcoin-cli -named bumpfee txid=4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927
{
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "origfee": 0.00000000,
  "fee": 0.00022600,
  "errors": [
  ]
}
```
The result is the automatic generation of a new transaction that has a fee determined by your bitcoin.conf file:
```
$ bitcoin-cli -named gettransaction txid=75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f
{
  "amount": -0.10000000,
  "fee": -0.00022600,
  "confirmations": 0,
  "trusted": false,
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "walletconflicts": [
    "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927"
  ],
  "time": 1491605676,
  "timereceived": 1491605676,
  "bip125-replaceable": "yes",
  "replaces_txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.00022600,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0020000006b48304502210094e54afafce093008172768d205d99ee2e9681b498326c077f0b6a845d9bbef702206d90256d5a2edee3cab1017b9b1c30b302530b0dd568e4af6f2d35380bbfaa280121029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac38f25c05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```

> :warning: **VERSION WARNING:** The `bumpfee` RPC requires Bitcoin Core v.0.14.0.

## Summary: Resending a Transaction with RBF

If a transaction is stuck, and you don't want to wait for it to expire entirely, if you opted-in to RBF, then you can double-spend using RBF to create a replacement transaction (or just use `bumpfee`).

> :fire: ***What is the power of RBF?*** Obviously, RBF is very helpful if you created a transaction with too low of a fee and you need to get those funds through. However, the ability to generally replace unconfirmed transactions with updated ones has more power than just that (and is why you might want to continue using RBF with raw transactions, even following the advent of `bumpfee`). 

> For example, you might send a transaction, and then before it's confirmed, combine it with a second transaction. This allows you to compress multiple transactions down into a single one, decreasing overall fees. It might also offer benefits to privacy. There are other reasons to use RBF too, for smart contracts or transaction cut-throughs, as described in the [Opt-in RBF FAQ](https://bitcoincore.org/en/faq/optin_rbf/).

## What's Next?

Continue "Controlling Bitcoin Transactions" with [§5.3: Funding a Transaction with CPFP](05_3_Funding_a_Transaction_with_CPFP.md).  
