# 7.1: Understanding the Foundation of Transactions

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

## Know the Parts of the Cryptographic Puzzle

As described in [Chapter 1](1_0_Introducing_Bitcoin.md), the funds in each Bitcoin transaction are locked with a cryptographic puzzle. To be precise, we said that Bitcoin is made up of "a sequence of atomic transactions: each of which is enabled by the sender with the solution to a cryptographic puzzle that is stored as a script; each of which is locked for the recipient with a new cryptographic puzzle that is stored as a script". Those scripts, that lock transactions and that unlock transactions, are written in Bitcoin Script.

_What is Bitcoin Script?_ Bitcoin Script is a stack-based Forth-like language that is purposefully avoids loops and is not Turing-complete. It's made up of individual opcodes. Every single transaction in Bitcoin is locked with a Bitcoin Script; when the locking transaction is run with the correct inputs, it will allow a UTXO to be respent.

The fact that transactions are locked with scripts means that they can be unlocked in a variety of different ways. In fact, we've met a number of different unlocking mechanisms to date, each of which incorporates different opcodes:

   * OP_CHECKSIG, which checks a public key against a signature is the basis of a P2PKH address, as will be fully detailed in [7.3: Scripting a Pay to Public Key Hash](7_3_Scripting_a_Pay_to_Public_Key_Hash.md).
   * OP_CHECKMULTISIG similarly checks multisigs, as will be fully detailed in Chapter 8.
   * OP_CHECKLOCKTIMEVERIFY and OP_SEQUENCEVERIFY form the basis of more complex Timelocks, as will be fully detailed in Chapter 9.
   * OP_RETURN is the mark of an unspendable transaction, which is why it's used to carry data.

## Access Scripts In Your Transactions

You may not realize it, but you've already seen these locking and unlocking scripts, all as part of the raw transactins you've been working with. 

The best way to look into them with more depth is thus to create a raw transaction, then examine it.

### Create a Test Transaction

This quick raw transaction grabs the first unspent transaction sitting around, and resends it to a change address, minus a transaction fee:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient=$(bitcoin-cli getrawchangeaddress)
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.2985 }''')
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
```
We're creating it and signing it but not sending it: the goal is simply produce a complete transaction that we can examine.

### Examing Your Test Transaction

You can now examine your transaction in depth by examining the `$signedtx`:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "7b5f161ff9f9275c94dca38348c91056fe6829a2b768ed82ede4bcf7c7384a68",
  "hash": "7b5f161ff9f9275c94dca38348c91056fe6829a2b768ed82ede4bcf7c7384a68",
  "size": 192,
  "vsize": 192,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04",
      "vout": 0,
      "scriptSig": {
        "asm": "3045022100c4ef5b531061a184404e84ab46beee94e51e8ae15ce98d2f3e10ae7774772ffd02203c546c399c4dc1d6eea692f73bb3fff490ea2e98fe300ac6a11840c7d52b6166[ALL] 0319cd3f2485e3d47552617b03c693b7f92916ac374644e22b07420c8812501cfb",
        "hex": "483045022100c4ef5b531061a184404e84ab46beee94e51e8ae15ce98d2f3e10ae7774772ffd02203c546c399c4dc1d6eea692f73bb3fff490ea2e98fe300ac6a11840c7d52b616601210319cd3f2485e3d47552617b03c693b7f92916ac374644e22b07420c8812501cfb"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 1.29850000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914371c20fb2e9899338ce5e99908e64fd30b78931388ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mkYMA2i2vzEXjoDn4GfvPoFr3MJ62uUtMx"
        ]
      }
    }
  ]
}
```
The two scripts are found in the two different parts of the transaction.

The `scriptSig` is found in the `vin`. This is the _unlocking_ script. It's what's run to unlock the associated UTXO. There will be one `scriptSig` per UTXO in a transaction.

The `scriptPubKey` is found in the `vout`. This is the _locking_ script. It's what locks the new output from the transaction. There will be one output in a transaction.

To be precise: the `scriptSig` of this transaction will unlock the previous UTXO; this new transaction's outputs will be locked with the `scriptPubKey`, which will be unlocked by the `scriptSig` of the transaction that reuses this UTXO.

### Read The Scripts in Your Transaction

Note that each of these scripts includes two different representations: the `hex` is what actually gets stored, but the more readable assembly language (`asm`) can sort of show you what's going on.

So, here's your first look at what Bitcoin Scripting looks like.

The unlocking script is:
```
"3045022100c4ef5b531061a184404e84ab46beee94e51e8ae15ce98d2f3e10ae7774772ffd02203c546c399c4dc1d6eea692f73bb3fff490ea2e98fe300ac6a11840c7d52b6166[ALL] 0319cd3f2485e3d47552617b03c693b7f92916ac374644e22b07420c8812501cfb"
```
As it happens, that mess of numbers is a private-key signature followed by the associated public key. Or at least hopefully that's what it is, because that's what's required to unlock the P2PKH UTXO that this transaction is using.

The locking script is:
```
OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
```
That in turn is the standard method in Bitcoin Script for locking a P2PKH transaction.

[7.3: Scripting a Pay to Public Key Hash](7_3_Scripting_a_Pay_to_Public_Key_Hash.md) explains how these go together, but first you need to know how Bitcoin Scripts are evaluated.

## Summary: Understanding the Foundation of Transactions

Every Bitcoin transaction includes at least one unlocking script (`scriptPubKey`), which solves a previous cryptographic puzzle, and at least one locking script (`scriptPubKey`), which creates a new cryptographic puzzle. There's one per input and one per output. Each of these scripts is written in Bitcoin Script, a Forth-like language that further empowers Bitcoin.
