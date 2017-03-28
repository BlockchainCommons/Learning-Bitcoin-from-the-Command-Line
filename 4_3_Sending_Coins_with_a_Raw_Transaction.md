
## Create a Change Address

Our first raw transaction was very simplistic: we sent the entirety of a UTXO to a new address. More frequently, you'll want to send someone an amount of money that doesn't match a UTXO. But you'll recall that the excess money from a UTXO that's not sent to your recipient just becomes a transaction fee. So, how do you send someone just part of a UTXO, while keeping the rest for yourself?

The solution is to create a change address:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ
```
Note that this is a new function: `getrawchangeaddress`. It's largely the same as `getnewaddress` but is optimized for use as a change address in a raw transaction, so it doesn't do things like make entries in your address book.

You now have an additional address inside your wallet that you can use to receive change from a UTXO!


## Write a Raw Transaction with Two Outputs

You're now ready to write a more complex raw transaction. It will be based on the two unspent UTXOs still in our wallet:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
    "vout": 1,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 237,
    "spendable": true,
    "solvable": true
  }, 
  {
    "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
    "vout": 0,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 237,
    "spendable": true,
    "solvable": true
  }
]
```
We want to send 0.1 BTC back to the TP faucet. This requires combining our two UTXOs, because neither contains quite that amount of bitcoin. It also requires sending to two addresses: one for the TP faucet (n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi) and one as a change address (mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ). In other words, we're going to have two inputs and two outputs in our raw transaction.

### Set Up Your Variables

We've already have `$changeaddress` and `$recipient` variables from our previous examples:
```
$ echo $changeaddress
mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
We also need to record the txid and vout for each of our two UTXOs:
```
$ utxo_txid_1="c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589"
$ utxo_vout_1="1"
$ utxo_txid_2="ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8"
$ utxo_vout_2="0"

$ echo $utxo_txid_1
c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589
$ echo $utxo_vout_1
1
$ echo $utxo_txid_2
ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8
$ echo $utxo_vout_2
0
```

### Write the Transaction

Writing the new raw transaction is surprisingly simple. All you need to do is include an additional, comma-separated JSON object in the JSON array of inputs and an additional, comma-separated key-value pair in the JSON object of outputs.

Here's the example. Take a look at it carefully to understand where the inputs end the outputs begin:
```
rawtxhex2=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' '''{ "'$recipient'": 0.1, "'$changeaddress'": 0.0555 }''')
```
We were _very_ careful figuring out our money math. These two UTXOs contain .156 BTC. After sending .1 BTC, we'll have .056 BTC left. We again chose .0005 BTC for the transaction fee: this transaction about twice as big, but it's still under the average transaction size quoted by Bitcoin Fees, so that should be more than enough to make this transaction go through quickly. To accomodate that fee, we set our change to .0555 BTC. If we'd messed up our math and instead set our change to .051 BTC, that additional .0045 BTC would be lost. That's $5 or $6 gone to the miners when it didn't need to be. If we'd forgot to make change at all, then the whole .056 ($70!) would have disappeared. So, again, _be careful_. 

Back to the raw transaction itself: we'll be honest, it took us a few times to get all the commas, quotes, and brackets in the right places, writing from the command line. We got JSON errors when we didn't format things quite right; when we fixed the errors, our variable (correctly) got filled with the hex for the raw transaction. So, as always, be careful here too, and check your work:
```


### Finish It Up

You can now sign, seal, and deliver your transaction, and it's yours (and the faucet's):
```
$ bitcoin-cli signrawtransaction $rawtxhex2
{
  "hex": "010000000289d57862bcb7520c1dd09793acda109ce5699bb61244387eae9a6a1e95b6abc1010000006a4730440220194cf452c76630419b90ef16bc9c0f31bd3187b97357eff6b610a3054968579b02200bcdb125ac182bda338c1a64d7887efd3e4eaf1a8e7f517eb7630d4dc258bf88012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffffd896f88117172336813c840d25b69f69ec106e4e8f2982f82d815b0527a77cab000000006a47304402201e65fe69dd231f73a9f493ca6c49503d51a766cf27be1281c9a4eefd04b4ac1c022003750d1ec93ef7be552aa55656c1860c986ef04a8733587f4f7be38707cccaf2012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffff0280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88acb0af5400000000001976a914b9f2579814712f82a3dadfd73e0356339c6705b488ac00000000",
  "complete": true
}
$ signedtx2="010000000289d57862bcb7520c1dd09793acda109ce5699bb61244387eae9a6a1e95b6abc1010000006a4730440220194cf452c76630419b90ef16bc9c0f31bd3187b97357eff6b610a3054968579b02200bcdb125ac182bda338c1a64d7887efd3e4eaf1a8e7f517eb7630d4dc258bf88012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffffd896f88117172336813c840d25b69f69ec106e4e8f2982f82d815b0527a77cab000000006a47304402201e65fe69dd231f73a9f493ca6c49503d51a766cf27be1281c9a4eefd04b4ac1c022003750d1ec93ef7be552aa55656c1860c986ef04a8733587f4f7be38707cccaf2012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffff0280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88acb0af5400000000001976a914b9f2579814712f82a3dadfd73e0356339c6705b488ac00000000"
$ bitcoin-cli sendrawtransaction $signedtx2
18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae
```
### Wait

As usual, your money will be in flux for a while. You used all of your UTXOs, so your `getbalance` may show that you have cash, but the transaction must be confirmed before you see a UTXO with your change.

But, in 10 minutes or less (probably), you'll have your remaining money back and fully spendable again:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae",
    "vout": 1,
    "address": "mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ",
    "account": "",
    "scriptPubKey": "76a914b9f2579814712f82a3dadfd73e0356339c6705b488ac",
    "amount": 0.05550000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  }
]
```

This also might be a good time to revisit a blockchain explorer, so that you can see more intuitively how the inputs, outputs, and transaction fee are all laid out: [https://live.blockcypher.com/btc-testnet/tx/18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae/](https://live.blockcypher.com/btc-testnet/tx/18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae/).
$ bitcoin-cli decoderawtransaction $rawtxhex2
{
  "txid": "99b7454c20313806454f9fd4f3e0454ce93e6edd5e84dfc731bcc0ad352b2260",
  "hash": "99b7454c20313806454f9fd4f3e0454ce93e6edd5e84dfc731bcc0ad352b2260",
  "size": 160,
  "vsize": 160,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }, 
    {
      "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
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
      "value": 0.05550000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 b9f2579814712f82a3dadfd73e0356339c6705b4 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914b9f2579814712f82a3dadfd73e0356339c6705b488ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ"
        ]
      }
    }
  ]
}
```
