# 11.5: Scripting a P2WPKH

Though P2PKHs do a great job explaining the fundamental way that
Bitcoin Scripts work, what about native SegWit P2WPKH scripts, which
are today the majority of Bitcoin transactions? As it turns out,
P2WPKH addresses don't use Bitcoin Scripts like traditional Bitcoin
addresses do, and so this section is really a digression from the
scripting of this chapter — but an important one, because it outlines
the _other_ major way in which Bitcoins can be transacted.

## View a P2WPKH Script

It's easy enough to see what a P2WPKH script looks like. The following
raw transaction was created by spending a P2WPKH UTXO and then sending
the money on to a P2WPKH change address — just as we did with a legacy
address in
[§11.1](11_1_Understanding_the_Foundation_of_Transactions.md).

```
bitcoin-cli -named decoderawtransaction hexstring=$signedtx

| {
|   "txid": "14bcbdd7d4847aecc834771ab5c0b7c56f9c0432474110d75a2a3c66f258b3a6",
|   "hash": "5aa1ce0f5c3090778307bee2feb39fa6bf48db646d4b6d924d956daeb89c71d2",
|   "version": 2,
|   "size": 191,
|   "vsize": 110,
|   "weight": 437,
|   "locktime": 0,
|   "vin": [
|     {
|       "txid": "0a445db0f3aaf818a93395f02d1ab40d399c47e7cc6f41300e8d8fc46805fdbf",
|       "vout": 1655,
|       "scriptSig": {
|         "asm": "",
|         "hex": ""
|       },
|       "txinwitness": [
}         "30440220753d7a1042e536aa5272f2efd56031cf676b35a0916f6450952ffdaf64a0c37f0220685c15afa60361c305596fd7741e00868a6b12b2ae6057b96709c06eb425a45201",
|         "03ba764643259c0056e63fac0a5af8a4895014848c498890850aff327cdcd438b0"
|       ],
|       "sequence": 4294967293
|     }
|   ],
|   "vout": [
|     {
|       "value": 0.00098000,
|       "n": 0,
|       "scriptPubKey": {
|         "asm": "0 2567d7ef358c07fead8d33940d2f25fb68fe88e2",
|         "desc": "addr(tb1qy4na0me43srlatvdxw2q6te9ld50az8zg946g9)#59laaxcx",
|         "hex": "00142567d7ef358c07fead8d33940d2f25fb68fe88e2",
|         "address": "tb1qy4na0me43srlatvdxw2q6te9ld50az8zg946g9",
|         "type": "witness_v0_keyhash"
|       }
|     }
|   ]
| }
```

There are probably two surprising things here:

1. There's no `scriptSig` to unlock the previous transaction; and
2. The `scriptPubKey` to lock the new transaction is just `0 2567d7ef358c07fead8d33940d2f25fb68fe88e2`.

That's, quite simply, because P2WPKH works differently!

## Understand a P2WPKH Transaction

A P2WPKH transaction contains all the same information as a classic
P2PKH transaction, but it places it in weird places, not within a
traditional Bitcoin Script — and, that's the exact point of SegWit
transactions, to pull the "witness" information, which is to say the
public keys and signatures, out of the transaction to support a change
to block size.

But, if you look carefully, you'll see that the empty `scriptSig` has
been replaced with two entries in a new `txinwitness` section. If you
examine their sizes and formatting, they should seem familiar: they're
a signature and public key. Similarly, if you look in the
`scriptPubKey`, you'll see that it's made up of a `0` (actually:
`OP_0`, it's the SegWit version number) and another long number, which
is the public-key hash.

**P2PKH (§11.1):**

   * Signature (`scriptSig`):
      * 30440220252a4b161934f2fa3e00048735530dec19e8e66dabd702437d62ac100910432f022010deca96e14b96f467f42886bc18e1ce81639d69c14830d87e0c6a5c9ab05f3b
   * Public Key (`scriptSig`):
      * 0254e9741695210633a048c6193aa802630532eeeca6364cf4bb529d156aeee4aa
   * Public Key Hash (`scriptPubKey`):
      * a5d106eb8ee51b23cf60d8bd98bc285695f233f3
   * Script (`scriptPubKey`):
      * OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG

**P2WPKH (§11.5):**

   * Signature (`txinwitness`):
      * 30440220753d7a1042e536aa5272f2efd56031cf676b35a0916f6450952ffdaf64a0c37f0220685c15afa60361c305596fd7741e00868a6b12b2ae6057b96709c06eb425a45201
   * Public Key (`txinwitness`):
      * 03ba764643259c0056e63fac0a5af8a4895014848c498890850aff327cdcd438b0
   * Public Key Hash (`scriptPubKey`):   
      * 2567d7ef358c07fead8d33940d2f25fb68fe88e2   
| Type | PubKeyHash | PubKey | Signature |
   * Script (`scriptPubKey`):
      * 0 <pubKeyHash>

If a node has not been upgraded to support SegWit, then it does its
usual trick of concatenating the `scriptSig` and the
`scriptPubKey`. This produces: `0
2567d7ef358c07fead8d33940d2f25fb68fe88e2` (because there's only a
`scriptPubKey`). Running that will produce a stack with everything on
it in reverse order:

```
btcdeb '[0 2567d7ef358c07fead8d33940d2f25fb68fe88e2]'

| btcdeb 5.0.24 -- type `btcdeb -h` for start up options
| LOG: signing segwit taproot
| notice: btcdeb has gotten quieter; use --verbose if necessary (this message is temporary)
| 2 op script loaded. type `help` for usage information

script                                   |  stack 
-----------------------------------------+--------
0                                        | 
2567d7ef358c07fead8d33940d2f25fb68fe88e2 | 
#0000 0

btcdeb> step
		<> PUSH stack 
script                                   |  stack 
-----------------------------------------+--------
2567d7ef358c07fead8d33940d2f25fb68fe88e2 |      0x
#0001 2567d7ef358c07fead8d33940d2f25fb68fe88e2

btcdeb> step
		<> PUSH stack 2567d7ef358c07fead8d33940d2f25fb68fe88e2
script                                   |                                   stack 
-----------------------------------------+-----------------------------------------
                                         | 2567d7ef358c07fead8d33940d2f25fb68fe88e2
                                         |                                       0x
```

Bitcoin Scripts are considered successful if there's something in the
Stack, and it's non-zero, so P2WPKH scripts automatically succeed on
old nodes that weren't upgraded to SegWit as long as the
`scriptPubKey` is correctly created with a non-zero pub-key hash. This
is called an "anyone-can-spend" transaction, because old nodes
verified them as correct without any need for signatures.

> 📖 ***Why can't old nodes steal SegWit UTXOs?*** SegWit was enabled
on the Bitcoin network when 95% of miners signalled that they were
ready to start using it. That means that only 5% of nodes at that
point might have registered anyone-can-spend SegWit transactions as
valid without going through the proper work of checking the
`txinwitness`. If they incorrectly incorporated an invalid
anyone-can-spend UTXO into a block, the other 95% of nodes would
refuse to validate that block, and so it would quickly be orphaned
rather than being added to the "main" blockchain. (Certainly, 51% of
nodes could choose to stop interpreting SegWit transactions correctly,
but 51% of nodes can do anything on a consensus network like a
blockchain.)

Because old nodes always see SegWit scripts as correct, they will
always verify them, even without understanding their content.

### Read a SegWit Script on a New Machine

A machine that understands how SegWit work does the exact same things
that it would with an old P2PKH script, but it doesn't use a script
per se: it just knows that it needs to hash the public key in the
`txinwitness`, check that against the hashed key after the version
number in the `scriptPubKey` and then run `OP_CHECKSIG` on the
signature and public key in the `txinwitness`.

So, it's another way of doing the same thing, but without having the
scripts built into the transactions. (The process is built into the
node software instead.)

## Summary: Scripting a Pay to Witness Public Key Hash

To a large extent you _don't_ script a P2WPKH. Instead, Bitcoin Core
creates the transaction in a different way, placing the witness
information in a different place rather than a traditional
`scriptSig`. That means that P2WPKHs are a digression from the Bitcoin
Scripts of this part of the book, because they're an expansion of
Bitcoin that steps away from traditional Scripting (or rather: that
integrates their work into the node software.)

However, SegWit was also a clever usage of Bitcoin Scripts. Knowing
that there would be nodes that didn't upgrade and needing to stay
backward compatible, the developers created the P2WPKH format so that
it generated a script that always validated on old nodes (while still
having that script provide information to new nodes in the form of a
version number and a hashed public key).

When you're programming from the command line, you fundamentally don't
have to worry about this, other than knowing that you won't find
traditional scripts in raw SegWit transactions (which, again, was the
point).

## What's Next?

Continue "Bitcoin Scripting" with [Chapter 12: Embedding Bitcoin
Scripts in P2SH
Transactions](12_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md).
