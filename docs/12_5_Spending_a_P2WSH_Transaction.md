# 12.5: Spending a P2SH or P2WSH Transaction

Before we close out this overview of P2SH transactions, we're going to
touch on how to spend them.

## Use the Redeem Script

In the past we talked about recording your `redeemScript`, but you
actually never had to use it, because when you spent a multisig in
[§8.1](08_1_Spending_a_Multisig_with_a_PSBT.md), your wallet had
already recorded your redeem script, and it got automatically
incorporated into your PSBT when you processed the PSBT using that
wallet.

That's how things _should_ work. However, there may be situations
where your wallet either does not have the redeem script (because it's
a primitive wallet or you've moved to a new wallet) or is unable to
process the redeem script (which is often the case if you're moving
outside of standard P2SH multisig transactions). In these cases, it's
vitally important to have recorded the `redeemScript` for any P2SH or
P2WSH address, so you can actually spend it.

### Collect Your Variables

In these situations you're need to collect some additional information
before creating a transaction:

   * The `hex` of the `scriptPubKey` for the transaction you're spending 
   * The serialized `redeemScript`
   * All of the regular `txids`, `vouts`, and `addresses` that you'd need

## Create the Transaction

You create the transaction as normal:

```
rawtxhex=$(bitcoin-cli -named createrawtransaction \
  inputs='''[ { "txid": "'$utxo_txid'",  "vout": '$utxo_vout' } ]''' \
  outputs='''{ "'$recipient'": 0.00005}''')
```

But then you `signrawtransactionwithwallet` and you include your additional information.

Here's an example of doing so for a P2SH script:

```
bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex \
  prevtxs='''[ { "txid": "'$utxo_txid'", \
                 "vout": '$utxo_vout', \
		 "scriptPubKey": "'$utxo_spk'", \
		 "redeemScript": "'$redeem_script'" } ]'''
```

Here's the almost identical methodology for signing a P2WSH script:

```
bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex \
  prevtxs='''[ { "txid": "'$utxo_txid'", \
                 "vout": '$utxo_vout', \
		 "scriptPubKey": "'$utxo_spk'", \
		 "witnessScript": "'$redeem_script'" } ]'''
```


With any other sort of P2SH or P2WSH, you're going to be including a
different `redeemscript` for each transaction, but otherwise the
practice is always exactly the same. The only difference is that after
two chapters of work on Scripts you now understand what the
`scriptPubKey` is and what the `redeemScript` is.

## Summary: Spending a P2SH or P2WSH Transaction

You already spent a P2WSH back in Chapter 8, when you resent a multsig
transaction using a PSBT, but that hid all the requirements of
`scriptPubKey` and `redeemScript` information from you. Now you can
see the methodology for spending a P2SH or P2WSH if you don't have
that wallet support and you need to supply the `redeemScript`
yourself.

## What's Next?

Advance through "Bitcoin Scripting" with [Chapter Thirteen: Empowering
Timelock with Bitcoin
Scripts](13_0_Empowering_Timelock_with_Bitcoin_Scripts.md).
