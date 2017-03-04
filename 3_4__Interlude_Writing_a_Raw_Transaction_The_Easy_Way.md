# Interlude: Writing a Raw Transaction the Easy Way

The purpose of this tutorial is to show you the very basics of Bitcoin raw transactions, so that you can work at things at a fundamental level. If you were writing a wallet or something other Bitcoin software, you'd probably want to do things exactly as described here. However, if you were (satoshi forfend!) regularly sending bitcoins about through raw transactions created by hand, then you'd want to have a little better insurance that you weren't making mistakes.

The bitcoin-cli accomodates this with a `fundrawtransaction` command. First, you have to make sure that your ~/.bitcoin/bitcoin.conf file has some rational variables for calculating transaction fees. We've been quite aggressive with fees in this tutorial, to make sure the examples finish quickly, but the following would allow for cheaper transmissions that might take a few hours:
```
 paytxfee=0.0001
 mintxfee=0.0001
 txconfirmtarget=25
 ```
 With that in hand (and in bitcoin.conf) you can use `createrawtransaction` with just your output(s), then run `fundrawtransaction` on the resulting hex:
 ```
$ unfinishedtx=$(bitcoin-cli createrawtransaction '''[]''' '''{ "'$recipient'": 0.04 }''')
$ bitcoin-cli fundrawtransaction $unfinishedtx
{
  "hex": "0100000001aec98249082a086dd7ef614f04b8621f9bba4566b8921cbe5d73f7e391a7f1180100000000feffffff0208951700000000001976a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac00093d00000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "changepos": 0,
  "fee": 0.00004520
}
$ rawtxhex3="0100000001aec98249082a086dd7ef614f04b8621f9bba4566b8921cbe5d73f7e391a7f1180100000000feffffff0208951700000000001976a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac00093d00000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
It seems like magic, but when you examine the raw transaction, you'll see it works:
```
$ bitcoin-cli decoderawtransaction $rawtxhex3
{
  "txid": "3099716446e0761e823077fc2f33c158b1dbaa7c157cb03c25a7cf6b7ac4921d",
  "hash": "3099716446e0761e823077fc2f33c158b1dbaa7c157cb03c25a7cf6b7ac4921d",
  "size": 119,
  "vsize": 119,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.01545480,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 f26e11dc0fcc79fe76fca1d24d7588798922ca74 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n3cogFw4y3A1LJgRz2G97otEGT7BK8ca3D"
        ]
      }
    }, 
    {
      "value": 0.04000000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }
  ]
}
```
As you can see, `fundrawtransaction` chose UTXOs for the inputs, it created a change address, it calculated a (low) transaction fee based on the variables in the bitcoin.conf file, and it sent the amount of the UTXO minus the tx fee back to the change address.

You can even verify that the new address belongs to you:
```
$ bitcoin-cli validateaddress n3cogFw4y3A1LJgRz2G97otEGT7BK8ca3D
{
  "isvalid": true,
  "address": "n3cogFw4y3A1LJgRz2G97otEGT7BK8ca3D",
  "scriptPubKey": "76a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac",
  "ismine": true,
  "iswatchonly": false,
  "isscript": false,
  "pubkey": "0347251fdd4fe8f9c66c0c137cc641e864dd27fc5dc0c8d0c85ff884d2a3fa1574",
  "iscompressed": true,
  "hdkeypath": "m/0'/0'/4'",
  "hdmasterkeyid": "75807dcb1226537ceb54c38c4a75a61407fdd02d"
}
```
At this point you could sign and send the transaction as usual ... then wait much longer for it to come back due to the lower transaction fees!
```
$ signedtx3="0100000001aec98249082a086dd7ef614f04b8621f9bba4566b8921cbe5d73f7e391a7f118010000006b483045022100a9b1454114bb2c04b51619eb5a00ad391605920ae801405b6191a64d1fb1e6e8022054def9ccbd75cb7929279cfef73ac573cdac7a325a1e3c8f43e139a1340b5d4b012103f7c794378db1c070b07d74f427f394f8a5d53f1abe1d2dab100d5a7a49db8785feffffff0208951700000000001976a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac00093d00000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
$ bitcoin-cli sendrawtransaction $signedtx3
ecfc625fff594683e90d21618b64f44f7046c55bcda6468c1c37c1abe8b83913
```
And voila!
```
$ bitcoin-cli getbalance
0.01545480
```
We will _not_ be using this technique in the rest of the tutorial.
