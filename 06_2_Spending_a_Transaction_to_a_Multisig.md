# 6.2: Spending a Transaction with a Multisig

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The classic, and complex, way of spending funds sent to a multisignature address using `bitcoin-cli` requires that you do a lot of foot work.

## Find Your Funds

To start with, you need to find your funds; your computer doesn't know to look for them, because they're not associated with any addresses in your walet. You can alert `bitcoind` to do so using the `importaddress` command:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz
```
This command can take a while to run because it does a `rescan` to find all the related transactions. Afterward the funds should show up when you `listunspent` ... but they still aren't easily spendable.
```
$ bitcoin-cli listunspent
[
  {
    "txid": "621be11aac439d6ec58be398058fc33c3e89cf45138a0e73e05b7001f9b6e328",
    "vout": 0,
    "address": "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz",
    "account": "",
    "scriptPubKey": "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287",
    "amount": 1.29950000,
    "confirmations": 62,
    "spendable": false,
    "solvable": false
  }
]
```
(In fact, your wallet claims they're not `spendable` at all!)

## Set Up Your Variables

When you're ready to spend the funds received by a multisignature address, you're going need to collect a _lot_ of data: much more than you need when you spend a normal P2PKH UTXO. That's in part because the info on the multisig address isn't in your wallet, and in part because you're spending money that was sent to a P2SH (pay-to-script-hash) address, and that's a lot more demanding.

In total, you're going to need to collect three things: extended information about the UTXO; the redeemScript; and all the private keys involved. You'll of course need a new recipient address too. The private keys need to wait for the signing step, but everything else can be done now.

### Access the UTXO information

To start with, grab the `txid` and the `vout` for the transaction that you want to spend, as usual. For example, if it's your 0th UTXO:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
```
However, you need to also access a third bit of information about the UTXO, its `scriptPubKey`, which is the script that locked the transaction. This is done with a simple `bitcoin-cli`/`jq` invocation that mirrors the previous commands.
```
$ utxo_spk=$(bitcoin-cli listunspent | jq -r '.[0] | .scriptPubKey')
```

### Record the Redeem Script

Hopefully, you saved the `redeemScript`. Now you should record it in a variable.
```
$ redeem_script="52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
```
### Decide Your Recipient

We're just going to send the money back to ourself. This is useful because it frees the funds up from the multisig, converting them into a normal P2PKH transaction that can later be confirmed by a single private key:
```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
## Create Your Transaction

You can now create your transaction. This is no different than usual.
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.299}''')
```

## Sign Your Transaction

You're now ready to sign your transaction. This is a multi-step process because you'll need to do it on multiple machines, each of which will contribute their own private keys.

### Dump Your First Private Key

Because this transaction isn't making full use of your wallet, you're going to need to directly access your private keys. Start on `machine1`, where you should retrieve any of that user's private keys that were involved in the multisig:
```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw
```
> **WARNING:** Directly accessing your private keys from the shell is very dangerous behavior and should be done with extreme care if you're using real money. At the least, don't save the information into a variable that could be accessed from your machine. Removing your shell's history is another great step. At the most, don't do it.

### Make Your First Signature

You can now make your first signature with the `signrawtransaction` command. Here's where things are different: you're going to need to coach the command on how to sign. You do these by adding the following new information:

* Include a `prevtxs` argument that includes the `txid`, the `vout`, the `scriptPubKey`, and the `redeemScript` that you recorded, each of them an individual key-value pair in the JSON object. 
* Include a `privkeys` argument that lists the private keys you dumped on this machine.

```
machine1$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]'
{
  "hex": "020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000",
  "complete": false,
  "errors": [
    {
      "txid": "621be11aac439d6ec58be398058fc33c3e89cf45138a0e73e05b7001f9b6e328",
      "vout": 0,
      "scriptSig": "00483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae",
      "sequence": 4294967295,
      "error": "Operation not valid with the current stack size"
    }
  ]
}
```
That produces scary errors and says that it's not `complete`. This is all correct. You can see that the signature has been partially successfully because the `hex` has gotten longer. Though the transaction has been partially signed, it's not done because it needs more signatures. 

### Repeat for Other Signers

You can now pass the transaction on, to be signed again by anyone else required for the mutisig. They do this by running the same signing command that you did but: (1) with the longer `hex` that you output (`bitcoin-cli -named signrawtransaction hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]' | jq -r '. | .hex'`); and (2) with their own private key.

> **M-OF-N VS N-OF-N:** Obviously, if you have an n-of-n signature (like the 2-of-2 multisignature in this example), then everyone has to sign, but if you hae a m-of-n multisignature where "m < n", then the signature will be complete when only some ("m") of the signers have signed.

To do so first they access their private keys:
```
$ bitcoin-cli -named dumpprivkey address=$address2
cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B
```
Second, they sign the new `hex` using all the same `prevtxs` values:
```
$ bitcoin-cli -named signrawtransaction hexstring=020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B"]'
{
  "hex": "020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b6200000000db00483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa01483045022100d5190eb824535423f67b15040efaba66953ea39f312540dd38504ed85ba6436402206171883ff28c235030550c36cadb31e40aaa9a74f71579557b74a5684545675c014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000",
  "complete": true
}
```
Third, they may need to send on the even longer `hexstring` they produce to additional signers.

In this case, we now see that the signature is `complete`!

## Send Your Transaction

When done, you should fall back on the standard JQ methodology to save your `hexstring` and then to send it:
```
$ signedtx = $(bitcoin-cli -named signrawtransaction hexstring=020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B"]' | jq -r '.hex')
user1@blockstream2:~$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```

## Understand the Importance of This Expanded Signing Methodology

This took some work, and as you'll soon learn, the foolishness with the private keys, the redeem script, and the scriptpubkey isn't actually required to redeem from multisignature addresses using newer versions of Bitcoin Core. So, what was the point?

This redemption methodology shows a standard way to sign and reuse P2SH transactions. In short, to redeem P2SH funds, a `signrawtransaction` needs to:

1. Include the `scriptPubKey`, which explains the P2SH cryptographic puzzle.
2. Include the `redeemScript`, which solves the P2SH cryptographic puzzle, and introduces a new puzzle of its own.
3. Be run on each machine holding required signatures.
4. Include the relevant signatures, which solve the redeemScript puzzle.

Here, we saw this methodology used  to redeem multisig funds. In the future you can also use it to redeem funds that were locked with other, more complex P2SH scripts, as explained starting in Chapter 9.

## Summary: Spending a Transaction with a Multisig

It turns out that spending money sent to a multisig address can take quite a bit of work. But as long as you have your original addresses and your redeemScript, you can do it by signing a raw transaction with each different address, and providing some more information along the way.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§6.3: Sending & Spending an Automated Multisig](06_3_Sending_an_Automated_Multisig.md).
