# 4.5: Sending Coins with Automated Raw Transactions

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This chapter lays out three ways to send funds via Bitcoin's cli interface. Section 4.1 described how to do so with a simple command, and Section 4.4 detailed how to use a more dangerous raw transaction. This final section splits the difference by showing how to make raw transactions simpler. 

## Let Bitcoin Calculate For You

The methodology is simple: you create a raw transaction, but you use the `fundrawtransaction` command to ask the bitcoind to run the calculations for you.

In order to use this command, you'll need to have your ~/.bitcoin/bitcoin.conf file has some rational variables for calculating transaction fees. Please see [Section 4.1](4_1_Sending_Coins_The_Easy_Way.md) for more information on this.

For very conservative numbers, we suggested adding the following to the bitcoin.conf:
```
mintxfee=0.0001
txconfirmtarget=6
```
To keep the tutorial moving along (and more generally to move money fast) we suggested the following:
```
mintxfee=0.001
txconfirmtarget=1
```

## Create a Bare Bones Raw Transaction

To use `fundrawtransaction` you need to create a bare-bones raw transaction that lists _no_ inputs and _no_ change address. You'll just list your recipient and what you want to send them:
 ```
$ unfinishedtx=$(bitcoin-cli -named createrawtransaction transactions='''[]''' outputs='''{ "'$recipient'": 1.0 }''')
```

## Fund Your Bare Bones Transaction

You then tell `bitcoin-cli` to fund that bare-bones transaction:
```
$ bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx
{
  "hex": "020000000169847669938c6a66ef790b87ebb6233059609bee4601476c5948db1a4defc9690100000000feffffff02a8e30f05000000001976a914a6f0ee37c44947f4137d56e4aab12f27ad50369188ac00e1f505000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "changepos": 0,
  "fee": 0.00022600
}
```
That provides a lot of useful information, but once you're confident with how it works, you'll want to use JQ to save your hex to a variable, as usual:
```
$ rawtxhex3=$(bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx | jq -r '.hex')
```
## Verify Your Funded Transaction

It seems like magic, so the first few times you do this, you'll probably want to verify it.

Running `decoderawtransaction` will show that the raw transaction is now laid out correctly, using one or more of your UTXOs and splitting up the money between multiple addresses:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex3
{
  "txid": "2e34603b7449d29412fb7b0d184085d4d839d965f2bba361749c20d9dbae3d0b",
  "hash": "2e34603b7449d29412fb7b0d184085d4d839d965f2bba361749c20d9dbae3d0b",
  "size": 119,
  "vsize": 119,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469",
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
      "value": 0.84927400,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 123cd8796558d195e52137ce3800e5f8120ee46f OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914123cd8796558d195e52137ce3800e5f8120ee46f88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8"
        ]
      }
    }, 
    {
      "value": 1.00000000,
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
We saw the fee in the more extensive output, before we saved things to a variable with JQ, but you can verify it with the `btctxfee` JQ alias:
```
$ btctxfee $rawtxhex3
.00023
```
Finally, you can use `validateaddress` to see that the generated change address really belongs to you:
```
$ bitcoin-cli -named validateaddress address=mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8
{
  "isvalid": true,
  "address": "mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8",
  "scriptPubKey": "76a914123cd8796558d195e52137ce3800e5f8120ee46f88ac",
  "ismine": true,
  "iswatchonly": false,
  "isscript": false,
  "pubkey": "029045eaa55d283526c723e6d5495d9b3f077b545563f86465aafcd9bfdd50359e",Y
  "iscompressed": true,
  "timestamp": 1489170694,
  "hdkeypath": "m/0'/0'/11'",
  "hdmasterkeyid": "144a68bde927a1fed7c2b71ad9010b0201819be5"
}
```
Note the `ismine` results.

## Send Your Funded Transaction

At this point you could sign and send the transaction as usual.
```
$ signedtx3=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex3 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx3
```
In several minutes, you'll have your change back:
```
$ bitcoin-cli listunspent
[

  {
    "txid": "37698ad6e7f62df07c2fbc549339aa680a7fa18328d7ad14ecb72b21c505cbc6",
    "vout": 0,
    "address": "mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8",
    "scriptPubKey": "76a914123cd8796558d195e52137ce3800e5f8120ee46f88ac",
    "amount": 0.84927400,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  }
]
```

## Summary: Sending Coins with Automated Raw Transactions

If you must send funds with raw transactions `fundrawtransaction` gives you a nice alternative where fees, inputs, and outputs are calculated for you, so you don't accidentally lose a bunch of money.

### Why Use Automated Raw Transactions

_The advantages._ It provides a nice balance. If you're sending funds by hand and `sendtoaddress` doesn't offer enough control for whatever reason, you can get some of the advantages of raw transactions without the dangers. This methodology should be used whenever possible if you're sending raw transactions by hand.

_The disadvantages._ It's a hodge-podge. Though there are a few more options on the `fundrawtransaction` command that weren't mentioned here, your control is still limited. So, you're using raw transactions, but you're still limited in what you can do. You'd probably never want to use this method if you were writing a program where the whole goal is to know exactly what's going on.
