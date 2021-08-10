# 10.6: Spending a P2SH Transaction

Before we close out this overview of P2SH transactions, we're going to touch upon how to spend them. This section is mainly an overview, referring back to a previous section where we _already_ spent a P2SH transaction.

## Use the Redeem Script

As we saw in [§6.2: Spending a Transaction with a Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md), spending a P2SH transaction is all about having that serialized version of the locking script, the so-called _redeemScript_. So, the first step in being able to spend a P2SH transaction is making sure that you save the _redeemScript_ before you give out the P2SH address to everyone. 

### Collect Your Variables

Because P2SH addresses other than the special multisig and nested Segwit addresses aren't integrated into `bitcoin-cli` there will be no short-cuts for P2SH spending like you saw in [§6.3: Sending an Automated Multisig](06_3_Sending_an_Automated_Multisig.md). You're going to need to collect all the more complex variables on your own!

This means that you need to collect:

   * The `hex` of the `scriptPubKey` for the transaction you're spending 
   * The serialized `redeemScript`
   * Any private keys, since you'll be signing by hand
   * All of the regular `txids`, `vouts`, and `addresses` that you'd need

## Create the Transaction

As we saw in §6.2, the creation of a transaction is pretty standard:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```
However, signing requires entering extra information for the (1) `scriptPubKey`; (2) the `redeemScript`; and (3) any required private keys.

Here's the example of doing so for that P2SH-embedded multisig in §6.2:
```
$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
```
With any other sort of P2SH you're going to be including a different `redeemscript`, but otherwise the practice is exactly the same. The only difference is that after two chapters of work on Scripts you now understand what the `scriptPubKey` is and what the `redeemScript` is, so hopefully what were mysterious elements four chapters ago are now old hat.

## Summary: Spending a P2SH Transaction

You already spent a P2SH back in Chapter 6, when you resent a multsig transaction the hard way, which required lining up the `scriptPubKey` and `redeemScript` information. Now you know that the `scriptPubKey` is a standardized P2SH locking script, while the `redeemScript` matches a hash in that locking script and that you need to be able to run it with the proper variables to receive a `True` result. But other than knowing more, there's nothing new in spending a P2SH transaction, because you already did it!

## What's Next?

Advance through "Bitcoin Scripting" with [Chapter Eleven: Empowering Timelock with Bitcoin Scripts](11_0_Empowering_Timelock_with_Bitcoin_Scripts.md).
