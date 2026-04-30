# 6.3: Funding a Transaction with CPFP

If your Bitcoin transaction is stuck, and you're the _recipient_, you can clear it using CPFP (child-pays-for-parent). This is alternative to the _sender's_ ability to do so with RBF.

## Understand How CPFP Works

RBF was all about the sender. He messed up and needed to increase the fee, or he wanted to be smart and combine transactions for a variety of reasons. It was a powerful sender-oriented feature. In some ways, CPFP is RBF's opposite, because it empowers the recipient who knows that their money hasn't arrived yet and wants to speed it up. However, it's also a much simpler feature, with less wide applicability. 

Basically, the idea of CPFP is that a recipient has a transaction that hasn't been confirmed in a block and they want to spend that UTXO. So, they include that unconfirmed transaction in a new transaction and pay a high-enough fee to encourage a miner to include both the original (parent) transaction and the new (child) transaction in a block. As a result, the parent and child transactions clear simultaneously.

It should be noted that CPFP is not a new protocol feature, like RBF. It's just an incentivization scheme that can be used for transaction selection by miners. This also means that it's not as reliable as a protocol change like RBF: there might be reasons that the child is not selected to be put into a block, and that will prevent the parent from ever being put into a block.

## Spend Unconfirmed UTXOs

Funding a transaction with CPFP is a very simple process using the methods you're already familiar with:

   1. Find the `txid` and `vout` of the unconfirmed transaction. This may be the trickiest part, as `bitcoin-cli` generally tries to protect you from unconfirmed transactions. The sender might be able to send you this info; even with just the `txid`, you should be able to figure out the `vout` in a blockchain explorer.
   
   You do have one other option: use `bitcoin-cli getrawmempool`, which can be used to list the contents of your entire mempool, where the unconfirmed transactions will be. You may have to dig through several if the mempool is particularly busy. You can then get more information on a specific transaction with `bitcoin-cli getrawtransaction` with the verbose flag set to `true`:

```
$ bitcoin-cli getrawmempool
[
  "72ab93c9e66f3cad39620f4689748704012b9f28acf896b348208e0fc0bafa73",
  "115ff6412b832f7292d2117708ef88eb61be6a24bd8f0360e7dd73c02cafa36c",
  "074a5055b8d3ae1282b913cd6c063911b2bc8f414eed4f44efc6a0459364dc80",
  "e5f2e728aad9aafa1eab72c3654068c50cbbd5605f850f2b8a88375e580077e0"
]

$ bitcoin-cli getrawtransaction 115ff6412b832f7292d2117708ef88eb61be6a24bd8f0360e7dd73c02cafa36c true
{
  "txid": "115ff6412b832f7292d2117708ef88eb61be6a24bd8f0360e7dd73c02cafa36c",
  "hash": "cbebd8db046db6a265ce1e8c5ccf2127bbdb5f13d89bca9de0f46c25d33383ed",
  "version": 2,
  "size": 223,
  "vsize": 142,
  "weight": 565,
  "locktime": 295321,
  "vin": [
    {
      "txid": "a2de81cb60a445e4766c45d168f51d4caa57990859126d2553dba84b5f6ee4ea",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "3044022022cd7118792474d35a4ef6b4b3189d9fa88e2c6e8e2cc953598a0b9f2004bf8702207b1e72005eec6994dc4ad9099dbb640766ae50c10ecc13729fc107fd2d7cdacf01",
        "021ab1d59b685df8373c8b5ba2fc2f1c5695a91f435df3bfbe73d7fd6797dab98a"
      ],
      "sequence": 4294967293
    }
  ],
  "vout": [
    {
      "value": 0.00001000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 c524be5df153dd875bd4dc92c4b8593403c1defd OP_EQUAL",
        "desc": "addr(2NBDd84nZm882RwvKHifVMyHVzVsMDMStJr)#6c3e9nhz",
        "hex": "a914c524be5df153dd875bd4dc92c4b8593403c1defd87",
        "address": "2NBDd84nZm882RwvKHifVMyHVzVsMDMStJr",
        "type": "scripthash"
      }
    },
    {
      "value": 1.28985887,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 df9bb477b0c1e15c123343ddb6001de7f3f08e4f",
        "desc": "addr(tb1qm7dmgaasc8s4cy3ng0wmvqqaulelprj0vgvyys)#qs0dv45g",
        "hex": "0014df9bb477b0c1e15c123343ddb6001de7f3f08e4f",
        "address": "tb1qm7dmgaasc8s4cy3ng0wmvqqaulelprj0vgvyys",
        "type": "witness_v0_keyhash"
      }
    }
  ],
  "hex": "02000000000101eae46e5f4ba8db53256d1259089957aa4c1df568d1456c76e445a460cb81dea20000000000fdffffff02e80300000000000017a914c524be5df153dd875bd4dc92c4b8593403c1defd871f2bb00700000000160014df9bb477b0c1e15c123343ddb6001de7f3f08e4f02473044022022cd7118792474d35a4ef6b4b3189d9fa88e2c6e8e2cc953598a0b9f2004bf8702207b1e72005eec6994dc4ad9099dbb640766ae50c10ecc13729fc107fd2d7cdacf0121021ab1d59b685df8373c8b5ba2fc2f1c5695a91f435df3bfbe73d7fd6797dab98a99810400"
}
```
Look through the `vout` array. Find the object that matches your address. The `n` value is your `vout`. You now have everything you need to create a new CPFP transaction.
```
$ utxo_txid=115ff6412b832f7292d2117708ef88eb61be6a24bd8f0360e7dd73c02cafa36c
$ utxo_vout=1
$ recipient2=$(bitcoin-cli getrawchangeaddress)
```

   2. Create a raw transaction using your unconfirmed transaction as an input.
   3. Double your expected transaction fees (or more). (This transaction uses a fee of about $1.41, which is ridiculous based on current fees, but definitely should be enough to free up the transaction.)
   
When you take these steps, everything should look totally normal, despite the fact that you're working with an unconfirmed transaction. To verify that all was well, we even looked at the results of our signature before we saved off the information to a variable:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient2'": 1.28983887 }''')

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ txid=$(bitcoin-cli -named sendrawtransaction hexstring=$signedtx)
```

   4. Crossing your fingers is not needed. You have verified your data is correct. From this point on, things are out of your hands.
   
Your transactions may go through quickly. They may not. It all depends on whether the miners who are randomly generating the current blocks support CPFP  or not. But you've given your transactions the best chance.

That's really all there is to it.

### Be Aware of Nuances

Though CPFP is usually described as being about a recipient using a new transaction to pay for an old one that hasn't been confirmed, there's nuance to this.

A _sender_ could use CPFP to free up a transaction if he received change from it. He would just use that change as his input, and the resultant use of CPFP would free up the entire transaction. Mind you, he'd do better to use RBF as long as it was enabled, as the total fees would then be lower.

A _recipient_ could use CPFP even if he wasn't planning on immediately spending the money, for example if he's worried that the funds may not be resent if the transaction expires. In this case, he just creates a child transaction that sends all the money (minus a transaction fee) to a change address. That's what we did in our example, above.

## Summary: Funding a Transaction with CPFP

You can take advantage of the CPFP incentives to free up funds that have been sent to you but have not been confirmed. Just use the unconfirmed transaction as the UTXO and pay a higher-than-average transaction fee.

> 🔥 ***What is the power of CPFP?*** Mostly, CPFP is just useful to get funds unstuck when you're the recipient and the sender isn't being helpful for whatever reason. It doesn't have the more powerful possibilities of RBF, but is an alternatve way to exert control over a transaction after it's been placed in the mempool, but before it's confirmed in a block.

## What's Next?

Move on to "Expanding Bitcoin Transactions" with [Chapter Seven:
Expanding Bitcoin Transactions with
Multisigs](06_0_Expanding_Bitcoin_Transactions_Multisigs.md).
