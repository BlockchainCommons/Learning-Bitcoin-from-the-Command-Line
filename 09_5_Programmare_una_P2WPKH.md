# 9.5: Scripting a P2WPKH

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

P2PKHs are fine for explaining the fundamental way that Bitcoin Scripts work, but what about native SegWit P2WPKH scripts, which are increasingly becoming the majority of Bitcoin transactions? As it turns out, P2WPKH addresses don't use Bitcoin Scripts like traditional Bitcoin addresses do, and so this section is really a digression from the scripting of this chapter — but an important one, because it outlines the _other_ major way in which Bitcoins can be transacted.

## View a P2WPKH Script

It's easy enough to see what a P2WPKH script looks like. The following raw transaction was created by spending a P2WPKH UTXO and then sending the money on to a P2WPKH change address — just as we did with a legacy address in [§9.1](09_1_Understanding_the_Foundation_of_Transactions.md).
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "bdf8f12768a9870d41ac280f8bb4f8ecd9d2fa66fffc75606811f5751c17cb3a",
  "hash": "ec09c84cae48694bec7fd3461b3c5b38a76829c56e9d876037bf2484d443174b",
  "version": 2,
  "size": 191,
  "vsize": 110,
  "weight": 437,
  "locktime": 0,
  "vin": [
    {
      "txid": "3f5417bc7a3a4144d715f3f006d35ea2b405f06091cbb9ce492e04ccefe02b18",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001",
        "03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 92a0db923b3a13eb576a40c4b35515aa30206cba",
        "hex": "001492a0db923b3a13eb576a40c4b35515aa30206cba",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qj2sdhy3m8gf7k4m2grztx4g44gczqm96y6sszv"
        ]
      }
    }
  ]
}
```
There are probably two surprising things here: (1) There's no `scriptSig` to unlock the previous transaction; and (2) the `scriptPubKey` to lock the new transaction is just `0 92a0db923b3a13eb576a40c4b35515aa30206cb`.

That's, quite simply, because P2WPKH works differently!

## Understand a P2WPKH Transaction

A P2WPKH transaction contains all the same information as a classic P2PKH transaction, but it places it in weird places, not within a traditional Bitcoin Script — and, that's the exact point of SegWit transactions, to pull the "witness" information, which is to say the public keys and signatures, out of the transaction to support a change to block size. 

But, if you look carefully, you'll see that the empty `scriptSig` has been replaced with two entries in a new `txinwitness` section. If you examine their sizes and formatting, they should seem familiar: they're a signature and public key. Similarly, if you look in the `scriptPubKey`, you'll see that it's made up of a `0` (actually: `OP_0`, it's the SegWit version number) and another long number, which is the public-key hash.

Here's a comparison of our two examples:
| Type | PubKeyHash | PubKey | Signature |
|----------------|----------|-------------|---------|
| SegWit | 92a0db923b3a13eb576a40c4b35515aa30206cba | 03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903 | 3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001 |
| non-SegWit | 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c |

So how does this work? It depends on old code interpreting this as a valid transaction and new code knowing to check the new "witness" information

### Read a SegWit Script on an Old Machine

If a node has not been upgraded to support SegWit, then it does its usual trick of concatenating the `scriptSig` and the `scriptPubKey`. This produces: `0 92a0db923b3a13eb576a40c4b35515aa30206cba` (because there's only a `scriptPubKey`). Running that will produce a stack with everything on it in reverse order:
```
$ btcdeb '[0 92a0db923b3a13eb576a40c4b35515aa30206cba]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
miniscript failed to parse script; miniscript support disabled
valid script
2 op script loaded. type `help` for usage information
script                                   |  stack 
-----------------------------------------+--------
0                                        | 
92a0db923b3a13eb576a40c4b35515aa30206cba | 
#0000 0
btcdeb> step
		<> PUSH stack 
script                                   |  stack 
-----------------------------------------+--------
92a0db923b3a13eb576a40c4b35515aa30206cba | 0x
#0001 92a0db923b3a13eb576a40c4b35515aa30206cba
btcdeb> step
		<> PUSH stack 92a0db923b3a13eb576a40c4b35515aa30206cba
script                                   |                                   stack 
-----------------------------------------+-----------------------------------------
                                         | 92a0db923b3a13eb576a40c4b35515aa30206cba
                                         | 0x
```
Bitcoin Scripts are considered successful if there's something in the Stack, and it's non-zero, so SegWit scripts automatically succeed on old nodes as long as the `scriptPubKey` is correctly created with a non-zero pub-key hash. This is called an "anyone-can-spend" transaction, because old nodes verified them as correct without any need for signatures.

> :book: ***Why can't old nodes steal SegWit UTXOs?*** SegWit was enabled on the Bitcoin network when 95% of miners signalled that they were ready to start using it. That means that only 5% of nodes at that point might have registered anyone-can-spend SegWit transactions as valid without going through the proper work of checking the `txinwitness`. If they incorrectly incorporated an invalid anyone-can-spend UTXO into a block, the other 95% of nodes would refuse to validate that block, and so it would quickly be orphaned rather than being added to the "main" blockchain. (Certainly, 51% of nodes could choose to stop interpreting SegWit transactions correctly, but 51% of nodes can do anything on a consensus network like a blockchain.)

Because old nodes always see SegWit scripts as correct, they will always verify them, even without understanding their content.

### Read a SegWit Script on a New Machine

A machine that understands how SegWit work does the exact same things that it would with an old P2PKH script, but it doesn't use a script per se: it just knows that it needs to hash the public key in the `txinwitness`, check that against the hashed key after the version number in the `scriptPubKey` and then run `OP_CHECKSIG` on the signature and public key in the `txinwitness`.

So, it's another way of doing the same thing, but without having the scripts built into the transactions. (The process is built into the node software instead.)

## Summary: Scripting a Pay to Witness Public Key Hash

To a large extent you _don't_ script a P2WPKH. Instead, Bitcoin Core creates the transaction in a different way, placing the witness information in a different place rather than a traditional `scriptSig`. That means that P2WPKHs are a digression from the Bitcoin Scripts of this part of the book, because they're an expansion of Bitcoin that steps away from traditional Scripting.

However, SegWit was also a clever usage of Bitcoin Scripts. Knowing that there would be nodes that didn't upgrade and needing to stay backward compatible, the developers created the P2WPKH format so that it generated a script that always validated on old nodes (while still having that script provide information to new nodes in the form of a version number and a hashed public key).

When you're programming from the command line, you fundamentally don't have to worry about this, other than knowing that you won't find traditional scripts in raw SegWit transactions (which, again, was the point).

## What's Next?

Continue "Bitcoin Scripting" with [Chapter 10: Embedding Bitcoin Scripts in P2SH Transactions](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md).
