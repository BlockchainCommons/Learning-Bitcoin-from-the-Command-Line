# 7.3: Spending a Transaction with a Multisig

You've created a multisig and you've even imported it into a multisig wallet, but how do you spend it? The top answer today is PSBTs, which we'll meet in the next chapter, but Bitcoin Core offers an old school method as well.

## Find Your Funds

You've already used `importdescriptors` to import your multisig into a watchonly wallet, perhaps using the descriptor provided by createmultisig (as explained in [§7.1](07_1_Creating_Multisig_Public_Keys.md)), perhaps using a descriptor that you created by hand (as described in [§7.2](07_2_Creating_Multisig_Descriptors.md)).

Afterward the funds should show up when you `listunspent`:
```
$ bitcoin-cli -rpcwallet=watchmulti listunspent
[
  {
    "txid": "eb41b6d829e83d32ae8e3bd80ece0330db959a90a2c2a3d7f9bf7a177836133e",
    "vout": 1,
    "address": "tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd",
    "label": "",
    "witnessScript": "5221039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d3312103c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c052ae",
    "scriptPubKey": "00203e11a03af7650e20bdf4768130a4c746b4fd75d696d3420bf6e7d0f860a1b98f",
    "amount": 0.00200000,
    "confirmations": 16,
    "spendable": true,
    "solvable": true,
    "desc": "wsh(multi(2,[38101947]039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,[0394feb3]03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#n5s3dxtx",
    "parent_descs": [
      "wsh(multi(2,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#k626xmlq"
    ],
    "safe": true
  }
]
```

However, they still aren't easily spendable (even if `bitcoin-cli` claims otherwise).

## Spend You Funds

There have traditionally been three ways to spend multisig funds using Bitcoin Core. The one most closely integrated with Bitcoin Core is to create a raw transaction, then to sign it with both of the wallets. That's what we'll use here.

### Set Up Your Variables

As usual, you need to know where your funds are going. Here, we're going to send funds to our normal, single-sig wallet, but send change back to the multisig:

```
recipient=$(bitcoin-cli -rpcwallet="" getnewaddress)
change=tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd
```

> ⚠️ **Do Not Reuse Addresses.** The best practice is to not reuse addresses in Bitcoin, because doing so creates easy-to-see correlation, as multiple transactions go in and out of the same address. However, to properly create a new multisig change address would require bringing together two new public keys. We could do that with a ranged descriptor, but it's beyond the scope of our simple single-use watch-only address we created, so for this example we're engaging in the bad practice of reusing our address for our change.

Since we're successfully watching the multisig address, we can grab its UTXO and vout as usual:
```
utxo_txid=$(bitcoin-cli -rpcwallet="watchmulti" listunspent | jq -r '.[0] | .txid') 
utxo_vout=$(bitcoin-cli -rpcwallet="watchmulti" listunspent | jq -r '.[0] | .vout')
utxo_pubkey=$(bitcoin-cli -rpcwallet="watchmulti" listunspent | jq -r '.[0] | .scriptPubKey')
```

redeem_script="5221039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d3312103c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c052ae"

rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.01, "'$change'": 0.0099  }''')

$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "2f07d92f7e13c13ced08ebaf63881fb804d05e5413d0013dbb559dc0805ee8f0",
  "hash": "2f07d92f7e13c13ced08ebaf63881fb804d05e5413d0013dbb559dc0805ee8f0",
  "version": 2,
  "size": 125,
  "vsize": 125,
  "weight": 500,
  "locktime": 0,
  "vin": [
    {
      "txid": "eb41b6d829e83d32ae8e3bd80ece0330db959a90a2c2a3d7f9bf7a177836133e",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967293
    }
  ],
  "vout": [
    {
      "value": 0.01000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 933439cf21eeff085ad1cc9ceb506810fd823779",
        "desc": "addr(tb1qjv6rnnepamlsskk3ejwwk5rgzr7cydmeyaktds)#md8rtul5",
        "hex": "0014933439cf21eeff085ad1cc9ceb506810fd823779",
        "address": "tb1qjv6rnnepamlsskk3ejwwk5rgzr7cydmeyaktds",
        "type": "witness_v0_keyhash"
      }
    },
    {
      "value": 0.00990000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 3e11a03af7650e20bdf4768130a4c746b4fd75d696d3420bf6e7d0f860a1b98f",
        "desc": "addr(tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd)#vjkdfwda",
        "hex": "00203e11a03af7650e20bdf4768130a4c746b4fd75d696d3420bf6e7d0f860a1b98f",
        "address": "tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd",
        "type": "witness_v0_scripthash"
      }
    }
  ]
}

bitcoin-cli -rpcwallet="" -named signrawtransactionwithwallet hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_pubkey'", "witnessScript": "'$redeem_script'", "amount": 0.02 } ]'''

[and this isn't working ...]

In total, you're going to need to collect three things: extended information about the UTXO; the redeemScript; and all the private keys involved. You'll of course need a new recipient address too. The private keys need to wait for the signing step, but everything else can be done now.

### Access the UTXO information

To start with, grab the `txid` and the `vout` for the transaction that you want to spend, as usual. In this case, it was retrieved from the `gettransaction` info, above:
```
$ utxo_txid=b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521
$ utxo_vout=0
```
However, you need to also access a third bit of information about the UTXO, its `scriptPubKey`/`hex`, which is the script that locked the transaction. Again, you're probably doing this by looking at the details of the transaction:
```
$ utxo_spk=a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387
```

### Record the Redeem Script

Hopefully, you saved the `redeemScript`. Now you should record it in a variable.

This was drawn from our creation of the address in the previous section.
```
redeem_script="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
```
### Decide Your Recipient

We're just going to send the money back to ourself. This is useful because it frees the funds up from the multisig, converting them into a normal P2PKH transaction that can later be confirmed by a single private key:
```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
## Create Your Transaction

You can now create your transaction. This is no different than usual.
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```

## Sign Your Transaction

You're now ready to sign your transaction. This is a multi-step process because you'll need to do it on multiple machines, each of which will contribute their own private keys.

### Dump Your First Private Key

Because this transaction isn't making full use of your wallet, you're going to need to directly access your private keys. Start on `machine1`, where you should retrieve any of that user's private keys that were involved in the multisig:
```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR
```
> :warning: **WARNING:** Directly accessing your private keys from the shell is very dangerous behavior and should be done with extreme care if you're using real money. At the least, don't save the information into a variable that could be accessed from your machine. Removing your shell's history is another great step. At the most, don't do it.

### Make Your First Signature

You can now make your first signature with the `signrawtransactionwithkey` command. Here's where things are different: you're going to need to coach the command on how to sign. You do these by adding the following new information:

* Include a `prevtxs` argument that includes the `txid`, the `vout`, the `scriptPubKey`, and the `redeemScript` that you recorded, each of them an individual key-value pair in the JSON object. 
* Include a `privkeys` argument that lists the private keys you dumped on this machine.

```
machine1$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": false,
  "errors": [
    {
      "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
      "vout": 0,
      "witness": [
      ],
      "scriptSig": "0047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
      "sequence": 4294967295,
      "error": "CHECK(MULTI)SIG failing with non-zero signature (possibly need more signatures)"
    }
  ]
}

```
That produces scary errors and says that it's `failing`. This is all fine. You can see that the signature has been partially successfully because the `hex` has gotten longer. Though the transaction has been partially signed, it's not done because it needs more signatures. 

### Repeat for Other Signers

You can now pass the transaction on, to be signed again by anyone else required for the multisig. They do this by running the same signing command that you did but: (1) with the longer `hex` that you output from (`bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]' | jq -r '.hex'`); and (2) with their own private key.

> :information_source: **NOTE — M-OF-N VS N-OF-N:** Obviously, if you have an n-of-n signature (like the 2-of-2 multisignature in this example), then everyone has to sign, but if you hae a m-of-n multisignature where "m < n", then the signature will be complete when only some ("m") of the signers have signed.

To do so first they access their private keys:
```
machine2$ bitcoin-cli -named dumpprivkey address=$address2
cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz
```
Second, they sign the new `hex` using all the same `prevtxs` values:
```
machine1$  bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000d90047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e301473044022000a402ec4549a65799688dd531d7b18b03c6379416cc8c29b92011987084e9f402205470e24781509c70e2410aaa6d827aa133d6df2c578e96a496b885584fb039200147522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": true
}
```
Third, they may need to send on the even longer `hexstring` they produce to additional signers.

But in this case, we now see that the signature is `complete`!

## Send Your Transaction

When done, you should fall back on the standard JQ methodology to save your `hexstring` and then to send it:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]' | jq -r .hex)
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```

## Understand the Importance of This Expanded Signing Methodology

This took some work, and as you'll soon learn, the foolishness with the private keys, the redeem script, and the scriptpubkey isn't actually required to redeem from multisignature addresses using newer versions of Bitcoin Core. So, what was the point?

This redemption methodology shows a standard way to sign and reuse P2SH transactions. In short, to redeem P2SH funds, a `signrawtransactionwithkey` needs to:

1. Include the `scriptPubKey`, which explains the P2SH cryptographic puzzle.
2. Include the `redeemScript`, which solves the P2SH cryptographic puzzle, and introduces a new puzzle of its own.
3. Be run on each machine holding required private keys.
4. Include the relevant signatures, which solve the redeemScript puzzle.

Here, we saw this methodology used  to redeem multisig funds. In the future you can also use it to redeem funds that were locked with other, more complex P2SH scripts, as explained starting in Chapter 9.

## Summary: Spending a Transaction with a Multisig

It turns out that spending money sent to a multisig address can take quite a bit of work. But as long as you have your original addresses and your redeemScript, you can do it by signing a raw transaction with each different address, and providing some more information along the way.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§6.3: Sending & Spending an Automated Multisig](06_3_Sending_an_Automated_Multisig.md).
