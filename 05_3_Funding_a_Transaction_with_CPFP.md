# 5.3: Funding a Transaction with CPFP

If your Bitcoin transaction is stuck, and you're the _recipient_, you can clear it using CPFP (child-pays-for-parent). This is alternative to the _sender's_ ability to do so with RBF.

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.13.0, which again means that most people should be using it by now.

## Understand How CPFP Works

RBF was all about the sender. He messed up and needed to increase the fee, or he wanted to be smart and combine transactions for a variety of reasons. It was a powerful sender-oriented feature. In some ways, CPFP is RBF's opposite, because it empowers the recipient who knows that his money hasn't arrived yet and wants to speed it up. However, it's also a much simpler feature, with less wide applicability. 

Basically, the idea of CPFP is that a recipient has a transaction that hasn't been confirmed in a block that he wants to spend. So, he includes that unconfirmed transaction in a new transaction and pays a high-enough fee to encourage a miner to include both the original (parent) transaction and the new (child) transaction in a block. As a result, the parent and child transactions clear simultaneously.

It should be noted that CPFP is not a new protocol feature, like RBF. It's just a new incentivization scheme that can be used for transaction selection by miners. This also means that it's not as reliable as a protocol change like RBF: there might be reasons that the child is not selected to be put into a block, and that will prevent the parent from ever being put into a block.

## Spend Unconfirmed UTXOs

Funding a transaction with CPFP is a very simple process using the methods you're already familiar with:

   1. Find the `txid` and `vout` of the unconfirmed transaction. This may be the trickiest part, as `bitcoin-cli` generally tries to protect you from unconfirmed transactions. The sender might be able to send you this info; even with just the `txid`, you should be able to figure out the `vout` in a blockchain explorer.
   
   You do have one other option: use `bitcoin-cli getrawmempool`, which can be used to list the contents of your entire mempool, where the unconfirmed transactions will be. You may have to dig through several if the mempool is particularly busy. You can then get more information on a specific transaction with `bitcoin-cli getrawtransaction` with the verbose flag set to `true`:
   ```
$ bitcoin-cli getrawmempool
[
  "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061"
]

$ bitcoin-cli getrawtransaction 95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061 true
{
  "txid": "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061",
  "hash": "9729e47b8aee776112a82cec46df7638d112ca51856c53e238a9b1f7af3be4ce",
  "version": 2,
  "size": 247,
  "vsize": 166,
  "weight": 661,
  "locktime": 1773277,
  "vin": [
    {
      "txid": "7a0178472300247d423ac4a04ff9165fa5b944104f6d6f9ebc557c6d207e7524",
      "vout": 0,
      "scriptSig": {
        "asm": "0014334f3a112df0f22e743ad97eec8195a00faa59a0",
        "hex": "160014334f3a112df0f22e743ad97eec8195a00faa59a0"
      },
      "txinwitness": [
        "304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae01",
        "03574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.01000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 f079f77f2ef0ef1187093379d128ec28d0b4bf76 OP_EQUAL",
        "hex": "a914f079f77f2ef0ef1187093379d128ec28d0b4bf7687",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5"
        ]
      }
    },
    {
      "value": 0.02598722,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 8799be12fb9eae6644659d95b9602ddfbb4b2aff OP_EQUAL",
        "hex": "a9148799be12fb9eae6644659d95b9602ddfbb4b2aff87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N5cDPPuCTtYq13oXw8RfpY9dHJW8sL64U2"
        ]
      }
    }
  ],
  "hex": "0200000000010124757e206d7c55bc9e6f6d4f1044b9a55f16f94fa0c43a427d2400234778017a0000000017160014334f3a112df0f22e743ad97eec8195a00faa59a0feffffff0240420f000000000017a914f079f77f2ef0ef1187093379d128ec28d0b4bf768742a727000000000017a9148799be12fb9eae6644659d95b9602ddfbb4b2aff870247304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae012103574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9dd0e1b00"
}
```
Look through the `vout` array. Find the object that matches your address. (Here, it's the only one.) The `n` value is your `vout`. You now have everything you need to create a new CPFP transaction.
```
$ utxo_txid=2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5
$ utxo_vout=0
$ recipient2=$(bitcoin-cli getrawchangeaddress)
```

   2. Create a raw transaction using your unconfirmed transaction as an input.
   3. Double the transaction fees (or more).
   
When you take these steps, everything should look totally normal, despite the fact that you're working with an unconfirmed transaction. To verify that all was well, we even looked at the results of our signature before we saved off the information to a variable:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient2'": 0.03597 }''')

$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex'
02000000012b137ef780666ba214842ff6ea2c3a0b36711bcaba839c3710f763e3d9687fed000000006a473044022003ca1f6797d781ef121ba7c2d1d41d763a815e9dad52aa8bc5ea61e4d521f68e022036b992e8e6bf2c44748219ca6e0056a88e8250f6fd0794dc69f79a2e8993671601210317b163ab8c8862e09c71767112b828abd3852e315441893fa0f535de4fa39b8dffffffff01905abd07000000001976a91450b1d90a130c4f3f1e5fbfa7320fd36b7265db0488ac00000000

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e
```

   4. Crossing your fingers is not needed. You have verified your data is correct. From this point on, things are out of your hands.
   
Your transactions may go through quickly. They may not. It all depends on whether the miners who are randomly generating the current blocks have the CPFP patch or not. But you've given your transactions the best chance.

That's really all there is to it.

### Be Aware of Nuances

Though CPFP is usually described as being about a recipient using a new transaction to pay for an old one that hasn't been confirmed, there's nuance to this.

A _sender_ could use CPFP to free up a transaction if he received change from it. He would just use that change as his input, and the resultant use of CPFP would free up the entire transaction. Mind you, he'd do better to use RBF as long as it was enabled, as the total fees would then be lower.

A _recipient_ could use CPFP even if he wasn't planning on immediately spending the money, for example if he's worried that the funds may not be resent if the transaction expires. In this case, he just creates a child transaction that sends all the money (minus a transaction fee) to a change address. That's what we did in our example, above.

## Summary: Funding a Transaction with CPFP

You can take advantage of the CPFP incentives to free up funds that have been sent to you but have not been confirmed. Just use the unconfirmed transaction as UTXO and pay a higher-than-average transaction fee.

> :fire: ***What is the power of CPFP?*** Mostly, CPFP is just useful to get funds unstuck when you're the recipient and the sender isn't being helpful for whatever reason. It doesn't have the more powerful possibilities of RBF, but is an alternatve way to exert control over a transaction after it's been placed in the mempool, but before it's confirmed in a block.

## What's Next?

Advance through "bitcoin-cli" with [Chapter Six: Expanding Bitcoin Transactions with Multisigs](06_0_Expanding_Bitcoin_Transactions_Multisigs.md).
