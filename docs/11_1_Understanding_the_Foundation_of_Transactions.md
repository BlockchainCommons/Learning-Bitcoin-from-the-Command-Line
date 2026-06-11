# 11.1: Understanding the Foundation of Transactions

The foundation of Bitcoin is the ability to protect transactions,
something that's done with a simple scripting language.

## Know the Parts of the Cryptographic Puzzle

As described in [Chapter 1](01_0_Introduction.md), the funds in each
Bitcoin transaction are locked with a cryptographic puzzle. To be
precise, we said that Bitcoin is made up of "a sequence of atomic
transactions". We noted that: "Each transaction is authenticated by a
sender with the solution to a previous cryptographic puzzle that was
stored as a script. The new transaction is locked for the recipient
with a new cryptographic puzzle that is also stored as a script."
Those scripts, which lock and unlock transactions, are written in
Bitcoin Script.

> 📖 ***What is Bitcoin Script?*** Bitcoin Script is a stack-based
Forth-like language that purposefully avoids loops and so is not
Turing-complete. It's made up of individual opcodes. Every single
transaction in Bitcoin is locked with a Bitcoin Script; when the
locking transaction for a UTXO is run with the correct inputs, that
UTXO can then be spent.

The fact that transactions are locked with scripts means that they can
be locked in a variety of different ways, requiring a variety of
different keys. In fact, the different sorts of addresses that we met
in [§4.1](04_1_Understanding_the_Address) each have a different sort
of script and use different "opcodes" (or functions) within that
script:

   * OP_CHECKSIG checks a public key against a signature. It's the
     basis of both the P2PKH and P2WPKH addresses, as will be fully
     detailed in [§11.4](11_4_Scripting_a_P2PKH.md) and
     [§11.5](11_5_Scripting_a_P2WPKH.md).
   * Arbitrary opcodes can be used in P2SH and P2WSH addresses, which
     are built to have user-defined keys and scripts.
   * OP_CHECKMULTISIG is one of those arbitrary opcodes, enabling
     multisigs, as will be fully detailed in
     [§12.4](12_4_Scripting_a_Multisig.md).

Some of the "expanded" Bitcoin functionality, such as Locktime and
data storage, also depends on specific opcodes:

   * OP_CHECKLOCKTIMEVERIFY and OP_SEQUENCEVERIFY form the basis of
     more complex Timelocks, as will be fully detailed in
     [§13.2](13_2_Using_CLTV_in_Scripts.md) and
     [§13.3](13_3_Using_CSV_in_Scripts.md).
   * OP_RETURN is the mark of an unspendable transaction, which is why
     it's used to carry data, as was alluded to in
     [§9.2](09_2_Sending_a_Transaction_with_Data.md).
     
> 📖 ***What is an Opcode?*** Opcode stands for "operation code". It's
typically associated with machine-language code, and is a simple
function (or "operator").


## Access Scripts In Your Transactions

It turns out that scripts are somewhat hidden from the user in
modern-day Segwit transactions: the scripting code for a standard
P2WPKH address is actually interpreted by the software instead of
being a part of the transaction, as was the case in classic Bitcoin
transactions. As a result, we're going to be working with "legacy"
P2PKH transactions for the majority of this chapter, to gain a good
understanding of how the underlying scripts work, before we displace
the standard locations of scripts with a P2WPKH transaction.

In fact, because the puzzle of a Bitcoin transaction comes in two
parts, we're going to make things very clear by ensuring that our
first transaction for testing scripts has both a P2PKH input and a
P2PKH output.

### Create a Test Transaction

If you don't already have a "legacy" (P2PKH) address that contains
funds, you can generate one with `getnewaddress`:

```
bitcoin-cli -named getnewaddress address_type=legacy

| moNhddRsGhaF18L5e62vEisLZxnzEV2VbF
```

You should then fund that.

You can now create a quick raw transaction by grabbing that unspent
"legacy" UTXO and resending it to a Legacy change address, minus a
transaction fee:

```
utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
recipient=$(bitcoin-cli -named getrawchangeaddress address_type=legacy)
rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00098 }''')
signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
```

You don't actually need to send it: the goal is simply to produce a
complete transaction that you can examine.

### Examine Your Test Transaction

You can now examine your transaction in depth by using `decoderawtransaction` on the `$signedtx`:

```
bitcoin-cli -named decoderawtransaction hexstring=$signedtx

| {
|   "txid": "fe8a790c21c289521a010f7fcaa66863b48eb956ed535f5c5d659ddf3dc0d45a",
|   "hash": "fe8a790c21c289521a010f7fcaa66863b48eb956ed535f5c5d659ddf3dc0d45a",
|   "version": 2,
|   "size": 191,
|   "vsize": 191,
|   "weight": 764,
|   "locktime": 0,
|   "vin": [
|     {
|       "txid": "c7fd5419c9c16a577ca7919b2e2c3bce816753ddc3a26b50a55443522ccb4879",
|       "vout": 1,
|       "scriptSig": {
|         "asm": "30440220252a4b161934f2fa3e00048735530dec19e8e66dabd702437d62ac100910432f022010deca96e14b96f467f42886bc18e1ce81639d69c14830d87e0c6a5c9ab05f3b[ALL] 0254e9741695210633a048c6193aa802630532eeeca6364cf4bb529d156aeee4aa",
|         "hex": "4730440220252a4b161934f2fa3e00048735530dec19e8e66dabd702437d62ac100910432f022010deca96e14b96f467f42886bc18e1ce81639d69c14830d87e0c6a5c9ab05f3b01210254e9741695210633a048c6193aa802630532eeeca6364cf4bb529d156aeee4aa"
|       },
|       "sequence": 4294967293
|     }
|   ],
|   "vout": [
|     {
|       "value": 0.00098000,
|       "n": 0,
|       "scriptPubKey": {
|         "asm": "OP_DUP OP_HASH160 dd78bf31853bd9c0e7cb108fc98d799955d60987 OP_EQUALVERIFY OP_CHECKSIG",
|         "desc": "addr(n1hzH5yyTPiAwkiFPpxcGQvsXNxc35dqNv)#w4xwe6te",
|         "hex": "76a914dd78bf31853bd9c0e7cb108fc98d799955d6098788ac",
|         "address": "n1hzH5yyTPiAwkiFPpxcGQvsXNxc35dqNv",
|         "type": "pubkeyhash"
|       }
|     }
|   ]
| }
```
The two scripts are found in the two different parts of the transaction.

The `scriptSig` is located in the `vin`. This is the _unlocking_
script. It's what's run to access the UTXO being used to fund this
transaction. There will be one `scriptSig` per UTXO in a transaction.

The `scriptPubKey` is located in the `vout`. This is the _locking_
script. It's what locks the new output from the transaction. There
will be one `scriptPubKey` per output in a transaction.

> 📖 ***How do the scriptSig and scriptPubKey interact?*** The
`scriptSig` of a transaction unlocks the previous UTXO; this new
transaction's output will then be locked with a `scriptPubKey`, which
can in turn be unlocked by the `scriptSig` of the transaction that
reuses that UTXO.

### Read The Scripts in Your Transaction

Look at the two scripts and you'll see that each includes two
different representations: the `hex` is what actually gets stored, but
the more readable assembly language (`asm`) can sort of show you
what's going on.

Take a look at the `asm` of the unlocking script and you'll get your
first look at what Bitcoin Scripting looks like:

```
30440220252a4b161934f2fa3e00048735530dec19e8e66dabd702437d62ac100910432f022010deca96e14b96f467f42886bc18e1ce81639d69c14830d87e0c6a5c9ab05f3b[ALL] 0254e9741695210633a048c6193aa802630532eeeca6364cf4bb529d156aeee4aa
```

As it happens, that mess of numbers is a private-key signature
followed by the associated public key. Or at least that's hopefully
what it is, because that's what's required to unlock the P2PKH UTXO
that this transaction is using.

Read the locking script and you'll see it's a lot more obvious:

```
OP_DUP OP_HASH160 dd78bf31853bd9c0e7cb108fc98d799955d60987 OP_EQUALVERIFY OP_CHECKSIG
```

That is the standard method in Bitcoin Script for locking a P2PKH transaction.

[§11.4](11_4_Scripting_a_P2PKH.md) will explain how these two scripts
go together, but first you will need to know how Bitcoin Scripts are
evaluated.

## Examine a Different Sort of Transaction

Before we leave this foundation behind, we're going to look at a
different type of locking script. Here's what the `scriptPubKey` looks like
in a classic P2SH multisig. 


```
|       "scriptPubKey": {
|         "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
|         "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
|         "reqSigs": 1,
|         "type": "scripthash",
|         "addresses": [
|           "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
|         ]
|       }
```

(Again, a P2WSH multisig would look different because of the ways that
SegWit deals with standard transactions.)

Compare that to the `scriptPubKey` from your new P2PKH transaction:
```
|       "scriptPubKey": {
|         "asm": "OP_DUP OP_HASH160 dd78bf31853bd9c0e7cb108fc98d799955d60987 OP_EQUALVERIFY OP_CHECKSIG",
|         "desc": "addr(n1hzH5yyTPiAwkiFPpxcGQvsXNxc35dqNv)#w4xwe6te",
|         "hex": "76a914dd78bf31853bd9c0e7cb108fc98d799955d6098788ac",
|         "address": "n1hzH5yyTPiAwkiFPpxcGQvsXNxc35dqNv",
|         "type": "pubkeyhash"
|       }
```

These two transactions are _definitely_ locked in different
ways. Bitcoin recognises the first as `scripthash` (P2SH) and the
second as `pubkeyhash` (P2PKH), but you should also be able to see the
difference in the different `asm` code: `OP_HASH160
a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL` versus `OP_DUP
OP_HASH160 dd78bf31853bd9c0e7cb108fc98d799955d60987 OP_EQUALVERIFY
OP_CHECKSIG`.  This is the power of scripting: it can very simply
produce some of the dramatically different sorts of transactions that
you learned about in the previous chapters.

## Summary: Understanding the Foundation of Transactions

Every Bitcoin transaction includes at least one unlocking script
(`scriptSig`), which solves a previous cryptographic puzzle, and at
least one locking script (`scriptPubKey`), which creates a new
cryptographic puzzle. There's one `scriptSig` per input and one
`scriptPubKey` per output. Each of these scripts is written in Bitcoin
Script, a Forth-like language that further empowers Bitcoin.

> 🔥 ***What is the power of scripts?*** Scripts unlock the full power
of Smart Contracts. With the appropriate opcodes, you can make very
precise decisions about who can redeem funds, when they can redeem
funds, and how they can redeem funds. More intricate rules for
corporate spending, partnership spending, proxy spending, and other
methodologies can also be encoded within a Script. It even empowers
more complex Bitcoin services such as Lightning and sidechains.

## What's Next?

Continue "Introducing Bitcoin Scripts" with [§11.2: Running a Bitcoin
Script](11_2_Running_a_Bitcoin_Script.md).
