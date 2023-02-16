# 9.1: Understanding the Foundation of Transactions

The foundation of Bitcoin is the ability to protect transactions, something that's done with a simple scripting language.

## Know the Parts of the Cryptographic Puzzle

As described in [Chapter 1](01_0_Introduction.md), the funds in each Bitcoin transaction are locked with a cryptographic puzzle. To be precise, we said that Bitcoin is made up of "a sequence of atomic transactions". We noted that: "Each transaction is authenticated by a sender with the solution to a previous cryptographic puzzle that was stored as a script. The new transaction is locked for the recipient with a new cryptographic puzzle that is also stored as a script." Those scripts, which lock and unlock transactions, are written in Bitcoin Script.

> :book: ***What is Bitcoin Script?*** Bitcoin Script is a stack-based Forth-like language that purposefully avoids loops and so is not Turing-complete. It's made up of individual opcodes. Every single transaction in Bitcoin is locked with a Bitcoin Script; when the locking transaction for a UTXO is run with the correct inputs, that UTXO can then be spent.

The fact that transactions are locked with scripts means that they can be locked in a variety of different ways, requiring a variety of different keys. In fact, we've met a number of different locking mechanisms to date, each of which used different opcodes:

   * OP_CHECKSIG, which checks a public key against a signature, is the basis of the classic P2PKH address, as will be fully detailed in [§9.3: Scripting a P2PKH](09_4_Scripting_a_P2PKH.md).
   * OP_CHECKMULTISIG similarly checks multisigs, as will be fully detailed in [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md).
   * OP_CHECKLOCKTIMEVERIFY and OP_SEQUENCEVERIFY form the basis of more complex Timelocks, as will be fully detailed in [§11.2: Using CLTV in Scripts](11_2_Using_CLTV_in_Scripts.md) and [§11.3: Using CSV in Scripts](11_3_Using_CSV_in_Scripts.md).
   * OP_RETURN is the mark of an unspendable transaction, which is why it's used to carry data, as was alluded to in [§8.2: Sending a Transaction with Data](08_2_Sending_a_Transaction_with_Data.md).

## Access Scripts In Your Transactions

You may not realize it, but you've already seen these locking and unlocking scripts as part of the raw transactions you've been working with. The best way to look into these scripts in more depth is thus to create a raw transaction, then examine it.

### Create a Test Transaction

To examine real unlocking and locking scripts, create a quick raw transaction by grabbing an unspent Legacy UTXO sitting around, and resending it to a Legacy change address, minus a transaction fee:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ recipient=$(bitcoin-cli -named getrawchangeaddress address_type=legacy)
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.0009 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
```
You don't actually need to send it: the goal is simply to produce a complete transaction that you can examine.

> **NOTE:** Why legacy addresses? Because their scripts are more meaningful. However, we'll also offer an example of a native SegWit P2WPKH in [§9.5](09_5_Scripting_a_P2WPKH.md).

### Examine Your Test Transaction

You can now examine your transaction in depth by using `decoderawtransaction` on the `$signedtx`:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "hash": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "version": 2,
  "size": 191,
  "vsize": 191,
  "weight": 764,
  "locktime": 0,
  "vin": [
    {
      "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
      "vout": 0,
      "scriptSig": {
        "asm": "304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b",
        "hex": "47304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c01210315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
    }
  ]
}

```
The two scripts are found in the two different parts of the transaction.

The `scriptSig` is located in the `vin`. This is the _unlocking_ script. It's what's run to access the UTXO being used to fund this transaction. There will be one `scriptSig` per UTXO in a transaction.

The `scriptPubKey` is located in the `vout`. This is the _locking_ script. It's what locks the new output from the transaction. There will be one `scriptPubKey` per output in a transaction.

> :book: ***How do the scriptSig and scriptPubKey interact?*** The `scriptSig` of a transaction unlocks the previous UTXO; this new transaction's output will then be locked with a `scriptPubKey`, which can in turn be unlocked by the `scriptSig` of the transaction that reuses that UTXO.

### Read The Scripts in Your Transaction

Look at the two scripts and you'll see that each includes two different representations: the `hex` is what actually gets stored, but the more readable assembly language (`asm`) can sort of show you what's going on.

Take a look at the `asm` of the unlocking script and you'll get your first look at what Bitcoin Scripting looks like:
```
04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
```
As it happens, that mess of numbers is a private-key signature followed by the associated public key. Or at least that's hopefully what it is, because that's what's required to unlock the P2PKH UTXO that this transaction is using.

Read the locking script and you'll see it's a lot more obvious:
```
OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG
```
That is the standard method in Bitcoin Script for locking a P2PKH transaction.

[§9.4](09_4_Scripting_a_P2PKH.md) will explain how these two scripts go together, but first you will need to know how Bitcoin Scripts are evaluated.

## Examine a Different Sort of Transaction

Before we leave this foundation behind, we're going to look at a different type of locking script. Here's the `scriptPubKey` from the multisig transaction that you created in [§6.1: Sending a Transaction with a Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md).
```
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
```

Compare that to the `scriptPubKey` from your new P2PKH transaction:
```
    "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
```

These two transactions are _definitely_ locked in different ways. Bitcoin recognises the first as `scripthash` (P2SH) and the second as `pubkeyhash` (P2PKH), but you should also be able to see the difference in the different `asm` code: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL` versus `OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG`. This is the power of scripting: it can very simply produce some of the dramatically different sorts of transactions that you learned about in the previous chapters.

## Summary: Understanding the Foundation of Transactions

Every Bitcoin transaction includes at least one unlocking script (`scriptSig`), which solves a previous cryptographic puzzle, and at least one locking script (`scriptPubKey`), which creates a new cryptographic puzzle. There's one `scriptSig` per input and one `scriptPubKey` per output. Each of these scripts is written in Bitcoin Script, a Forth-like language that further empowers Bitcoin.

> :fire: ***What is the power of scripts?*** Scripts unlock the full power of Smart Contracts. With the appropriate opcodes, you can make very precise decisions about who can redeem funds, when they can redeem funds, and how they can redeem funds. More intricate rules for corporate spending, partnership spending, proxy spending, and other methodologies can also be encoded within a Script. It even empowers more complex Bitcoin services such as Lightning and sidechains.

## What's Next?

Continue "Introducing Bitcoin Scripts" with [§9.2: Running a Bitcoin Script](09_2_Running_a_Bitcoin_Script.md).
