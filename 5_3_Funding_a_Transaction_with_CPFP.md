# 5.3: Funding a Transaction with CPFP

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

If your Bitcoin transaction is stuck, and you're the _recipient_, you can clear it using CPFP (child-pays-for-parent). 
If your Bitcoin transaction is stuck, and you're sender, you can resend it using RBF (replace-by-fee).

> **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.13.0, which means that many miners should be using it, but not necessarily all of them.

## Understand How CPFP Works

RBF was all about the sender. He'd messed up, and needed to increase the fee, or he wanted to be smart and combine transactions for a variety of reasons. It was a powerful sender-oriented feature. In some ways, CPFP is RBF's opposite, because it empowers the recipient who knows that his money hasn't arrived yet and wants to speed it up. However, it's also a much simpler feature, with less wide applicability. 

Basically, the idea of CPFP is that a recipient has a transaction that hasn't been confimed in a block that he wants to spend. So, he includes it in a transaction and pays a high-enough fee to encourage a miner to include both the original (parent) transaction and the new (child) transaction in a block. As a result, the parent and child transactions clear simultaneously.

It should be noted that CPFP is not a new feature, like RBF, which raised new questions about the protocol (and in particular pointed out the untrustworthiness of unconfirmed transactions). It's just a new incentivization scheme that can be built into transaction selection for miners. This also means that it's not as reliable as a protocol change like RBF: there might be reaosns that the parent is never put into a block, and that will prevent the child from ever bring put into a block.

## Spend Unconfirmed UTXOs

Funding a transaction with CPFP is a very simple process using the methods you're already familiar with:

   1. Find the txid and vout of the unconfirmed transaction. This may be the trickiest part, as `bitcoin-cli` generally tries to protect you from these transactions. The sender might be able to send them, or even if you have the transaction id, you should be able to figure out the vout in a blockchain explorer.
   
   You do have one other option: `bitcoin-cli getmempool` which canbe used to list the contents of your entire mempool, whichis very the unconfirmed transactions will be. You can then get more information on a specific transaction:
   ```
   $ bitcoin-cli getrawmempool
[
  "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927"
]

user1@blockstream:~$ bitcoin-cli getrawtransaction 4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927 1
{
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0020000006b483045022100a2a9bba6acb1f3b8519ccc7b7805cbfc9cd218ba913105a971d99a15fa84356e022039190918bbde569608a80d53cc6fc81655b42302c0486a7dfdf31393e6b6de4a0121029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac804a5d05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000",
  "txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
  "hash": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
  "size": 226,
  "vsize": 226,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "c08a7c8fec1be42cf5637e6992add28536737a96a0d8f4fb22e58ecb223e844e",
      "vout": 2,
      "scriptSig": {
        "asm": "3045022100a2a9bba6acb1f3b8519ccc7b7805cbfc9cd218ba913105a971d99a15fa84356e022039190918bbde569608a80d53cc6fc81655b42302c0486a7dfdf31393e6b6de4a[ALL] 029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71",
        "hex": "483045022100a2a9bba6acb1f3b8519ccc7b7805cbfc9cd218ba913105a971d99a15fa84356e022039190918bbde569608a80d53cc6fc81655b42302c0486a7dfdf31393e6b6de4a0121029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71"
      },
      "sequence": 1
    }
  ],
  "vout": [
    {
      "value": 0.10000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }, 
    {
      "value": 0.90000000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 c101d8c34de7b8d83b3f8d75416ffaea871d6649 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "my7UvPbPWDbqCxYpAcUZELDbK1X7w9Whmc"
        ]
      }
    }
  ]
}
```
   2. Create a raw transaction using your unconfirmed transaction an input.
   3. Double the transaction fees (or more).
   4. Cross your fingers.
   
That's really all there is to it.

### Be Aware of Nuances

Though CPFP is laid out as a recipient using a new transaction to pay for an old one that hasn't been confirmed, there's nuance to this.

A sender could use CPFP to free up funds if he received change from a transaction. He would just use that change as his input. Mind you, he'd do better to use RBF as long as it was enabled, as the total fees would then be lower.

A recipient could use CPFP even if he isn't planning on immediately spending the money, for example if he's worried that the funds may not be resent if the transaction expires. In this case, he just creates a child transaction that sends all the money (minus a transaction fee) to a change address.

## Summary: Funding a Transaction with CPFP

You can take advantage of the CPFP incentives to free up funds that have been sent to you that are stuck. Just use the unconfirmed transaction as UTXO and pay a higher-than-average transaction fee.

_What is the power of CPFP?_ Mostly, CPFP is just useful to get funds unstuck when you're the recipient and the sender isn't being helpful for whatever reason. It doesn't have the more powerful possibilities of the other features described in this chapter, and is included largely because of its parallel to RBF.
