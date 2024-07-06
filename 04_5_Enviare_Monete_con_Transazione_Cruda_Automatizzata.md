# 4.5: Sending Coins with Automated Raw Transactions

This chapter lays out three ways to send funds via Bitcoin's cli interface. [§4.1](04_1_Sending_Coins_The_Easy_Way.md) described how to do so with a simple command, and [§4.4](04_4_Sending_Coins_with_a_Raw_Transaction.md) detailed how to use a more dangerous raw transaction. This final section splits the difference by showing how to make raw transactions simpler and safer. 

## Let Bitcoin Calculate For You

The methodology for automated raw transactions is simple: you create a raw transaction, but you use the `fundrawtransaction` command to ask the bitcoind to run the calculations for you.

In order to use this command, you'll need to ensure that your ~/.bitcoin/bitcoin.conf file contains rational variables for calculating transaction fees. Please see [§4.1: Sending Coins The Easy Way](04_1_Sending_Coins_The_Easy_Way.md) for more information on this.

For very conservative numbers, we suggested adding the following to the `bitcoin.conf`:
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

To use `fundrawtransaction` you first need to create a bare-bones raw transaction that lists _no_ inputs and _no_ change address. You'll just list your recipient and how much you want to send them, in this case `$recipient` and `0.0002` BTC.
```
$ recipient=n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
$ unfinishedtx=$(bitcoin-cli -named createrawtransaction inputs='''[]''' outputs='''{ "'$recipient'": 0.0002 }''')
```

## Fund Your Bare Bones Transaction

You then tell `bitcoin-cli` to fund that bare-bones transaction:
```
$ bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx
{
  "hex": "02000000012db87641c6a21e5a68b20c226428544978e6ac44964d5d8060d7388000c584eb0100000000feffffff02204e0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac781e0000000000001600140cc9cdcf45d4ea17f5227a7ead52367aad10a88400000000",
  "fee": 0.00022200,
  "changepos": 1
}
```
That provides a lot of useful information, but once you're confident with how it works, you'll want to use JQ to save your hex to a variable, as usual:
```
$ rawtxhex3=$(bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx | jq -r '.hex')
```
## Verify Your Funded Transaction

It seems like magic, so the first few times you use `fundrawtransaction`, you'll probably want to verify it.

Running `decoderawtransaction` will show that the raw transaction is now laid out correctly, using one or more of your UTXOs and sending excess funds back to a change address:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex3
{
  "txid": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "hash": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "version": 2,
  "size": 116,
  "vsize": 116,
  "weight": 464,
  "locktime": 0,
  "vin": [
    {
      "txid": "eb84c5008038d760805d4d9644ace67849542864220cb2685a1ea2c64176b82d",
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
      "value": 0.00020000,
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
      "value": 0.00007800,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "hex": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r"
        ]
      }
    }
  ]
}
```
One thing of interest here is the change address, which is the second `vout`. Note that it's a `tb1` address, which means that it's Bech32; when we gave Bitcoin Core the total ability to manage our change, it did so using its default address type, Bech32, and it worked fine. That's why our change to SegWit addresses in [§4.6](04_6_Creating_a_Segwit_Transaction.md) really isn't that big of a deal, but there are some gotchas for wider usage, which we'll talk about there.

Though we saw the fee in the `fundrawtransaction` output, it's not visible here. However, you can verify it with the `txfee-calc.sh` JQ script created in the [JQ Interlude](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/04_2__Interlude_Using_JQ.md):
```
$ ~/txfee-calc.sh $rawtxhex3
.000222
```
Finally, you can use `getaddressinfo` to see that the generated change address really belongs to you:
```
$ bitcoin-cli -named getaddressinfo address=tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r
{
  "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
  "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "pubkey": "038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5",
  "ischange": true,
  "timestamp": 1592335137,
  "hdkeypath": "m/0'/1'/10'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
  ]
}
```
Note the `ismine` results.

## Send Your Funded Transaction

At this point you can sign and send the transaction as usual.
```
$ signedtx3=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex3 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx3
8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa
```
In several minutes, you'll have your change back:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa",
    "vout": 1,
    "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
    "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
    "amount": 0.00007800,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
    "safe": true
  }
]
```

## Summary: Sending Coins with Automated Raw Transactions

If you must send funds with raw transactions then `fundrawtransaction` gives you a nice alternative where fees, inputs, and outputs are calculated for you, so you don't accidentally lose a bunch of money.

> :fire: ***What is the power of sending coins with automated raw transactions?***

> _The advantages._ It provides a nice balance. If you're sending funds by hand and `sendtoaddress` doesn't offer enough control for whatever reason, you can get some of the advantages of raw transactions without the dangers. This methodology should be used whenever possible if you're sending raw transactions by hand.

> _The disadvantages._ It's a hodge-podge. Though there are a few additional options for the `fundrawtransaction` command that weren't mentioned here, your control is still limited. You'd probably never want to use this method if you were writing a program where the whole goal is to know exactly what's going on.

## What's Next?

Complete your "Sending of Bitcoin Transactions" with [§4.6: Creating a Segwit Transaction](04_6_Creating_a_Segwit_Transaction.md).

