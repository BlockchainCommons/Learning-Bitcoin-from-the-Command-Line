# 5.3: Funding a Transaction with CPFP

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

If your Bitcoin transaction is stuck, and you're the _recipient_, you can clear it using CPFP (child-pays-for-parent). This is alternative to the _sender's_ ability to do so with RBF.

> **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.13.0, which means that many miners should be using it, but not necessarily all of them.

## Understand How CPFP Works

RBF was all about the sender. He messed up and needed to increase the fee, or he wanted to be smart and combine transactions for a variety of reasons. It was a powerful sender-oriented feature. In some ways, CPFP is RBF's opposite, because it empowers the recipient who knows that his money hasn't arrived yet and wants to speed it up. However, it's also a much simpler feature, with less wide applicability. 

Basically, the idea of CPFP is that a recipient has a transaction that hasn't been confirmed in a block that he wants to spend. So, he includes that unconfirmed transaction in a new transaction and pays a high-enough fee to encourage a miner to include both the original (parent) transaction and the new (child) transaction in a block. As a result, the parent and child transactions clear simultaneously.

It should be noted that CPFP is not a new protocol feature, like RBF. It's just a new incentivization scheme that can be used for transaction selection by miners. This also means that it's not as reliable as a protocol change like RBF: there might be reasons that the parent is never put into a block, and that will prevent the child from ever bring put into a block.

## Spend Unconfirmed UTXOs

Funding a transaction with CPFP is a very simple process using the methods you're already familiar with:

   1. Find the `txid` and `vout` of the unconfirmed transaction. This may be the trickiest part, as `bitcoin-cli` generally tries to protect you from unconfirmed transactions. The sender might be able to send you this info; even with just the `txid`, you should be able to figure out the `vout` in a blockchain explorer.
   
   You do have one other option: use `bitcoin-cli getmempool`, which can be used to list the contents of your entire mempool, where the unconfirmed transactions will be. You may have to dig through several if the mempool is particularly busy. You can then get more information on a specific transaction with `bitcoin-cli getrawtransaction`:
   ```
$ bitcoin-cli getrawmempool
[
  "ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b"
]

$ bitcoin-cli getrawtransaction ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b 1
{
  "hex": "02000000010d5151273464ac8fa74c1b57ff8e4650b8e190c488b1cfadae6561ac3988e83c000000006a473044022059c402379c8e7d5bf60840fc75497ee67694e6dfc9b0391c59b4b202edf4194b0220686c5a27bee1a9214e410cbce3d11e47ca57736d7546ba208adcc068a3f826f5012103fb14f31f5f143463198a3e79c7529cae529a32857ada29bec26f7ea9d8ea67f3ffffffff0130e1be07000000001976a914cbecb861750ee3b00ea845f5c1efa72d1c541cec88ac00000000",
  "txid": "ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b",
  "hash": "ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b",
  "size": 191,
  "vsize": 191,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d",
      "vout": 0,
      "scriptSig": {
        "asm": "3044022059c402379c8e7d5bf60840fc75497ee67694e6dfc9b0391c59b4b202edf4194b0220686c5a27bee1a9214e410cbce3d11e47ca57736d7546ba208adcc068a3f826f5[ALL] 03fb14f31f5f143463198a3e79c7529cae529a32857ada29bec26f7ea9d8ea67f3",
        "hex": "473044022059c402379c8e7d5bf60840fc75497ee67694e6dfc9b0391c59b4b202edf4194b0220686c5a27bee1a9214e410cbce3d11e47ca57736d7546ba208adcc068a3f826f5012103fb14f31f5f143463198a3e79c7529cae529a32857ada29bec26f7ea9d8ea67f3"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 1.29950000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 cbecb861750ee3b00ea845f5c1efa72d1c541cec OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914cbecb861750ee3b00ea845f5c1efa72d1c541cec88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mz7D3c6tMtNXV6CTsEwDBJ2vhSVTo9bSMN"
        ]
      }
    }
  ]
}
```
Look through the `vout` array. Find the the object that matches your address. (Here, it's the only one.) The `n` value is your `vout`. You now have everything you need to create a new CPFP transaction.
```
$ utxo_txid=ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b
$ utxo_vout=0
$ recipient2=$(bitcoin-cli getrawchangeaddress)
```

   2. Create a raw transaction using your unconfirmed transaction an input.
   3. Double the transaction fees (or more).
   
When you take these steps, everything should look totally normal, despite the fact that you're working with an unconfirmed transaction. To verify that all was well, we even looked at the results of our signature before we saved off the information to a variable:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient2'": 1.2985 }''')

$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex'
02000000012b137ef780666ba214842ff6ea2c3a0b36711bcaba839c3710f763e3d9687fed000000006a473044022003ca1f6797d781ef121ba7c2d1d41d763a815e9dad52aa8bc5ea61e4d521f68e022036b992e8e6bf2c44748219ca6e0056a88e8250f6fd0794dc69f79a2e8993671601210317b163ab8c8862e09c71767112b828abd3852e315441893fa0f535de4fa39b8dffffffff01905abd07000000001976a91450b1d90a130c4f3f1e5fbfa7320fd36b7265db0488ac00000000
user1@blockstream:~$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e
```

   4. Cross your fingers.
   
Your transactions may go through quickly. They may not. It all depends on whether the miners who are randomly generating the current blocks have the CPFP patch or not. But you've given your transactions the best chance.

That's really all there is to it.

### Be Aware of Nuances

Though CPFP is usually described as being about a recipient using a new transaction to pay for an old one that hasn't been confirmed, there's nuance to this.

A _sender_ could use CPFP to free up a transaction if he received change from it. He would just use that change as his input, and the resultant use of CPFP would free up the entire transaction. Mind you, he'd do better to use RBF as long as it was enabled, as the total fees would then be lower.

A _recipient_ could use CPFP even if he wasn't planning on immediately spending the money, for example if he's worried that the funds may not be resent if the transaction expires. In this case, he just creates a child transaction that sends all the money (minus a transaction fee) to a change address. That's what we did in our example, above.

## Summary: Funding a Transaction with CPFP

You can take advantage of the CPFP incentives to free up funds that have been sent to you but have not been confirmed. Just use the unconfirmed transaction as UTXO and pay a higher-than-average transaction fee.

_What is the power of CPFP?_ Mostly, CPFP is just useful to get funds unstuck when you're the recipient and the sender isn't being helpful for whatever reason. It doesn't have the more powerful possibilities of RBF, but is an alternatve way to exert control over a transaction after it's been placed in the mempool, but before it's confirmed in a block.

## What's Next?

Advance through "bitcoin-cli" with [Chapter Six: Expanding Bitcoin Transactions](06_0_Expanding_Bitcoin_Transactions.md).
