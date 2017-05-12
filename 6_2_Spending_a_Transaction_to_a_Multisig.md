# 6.2: Spending a Transaction with a Multisig

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Money spent to a multisig address doesn't do you any good if you can't spend it. There are two ways to do this: a complex methodology that lays bare how multisig works; and a simple methodology that takes advantage of your wallet.

## Spend Funds Sent to a Multisig Address, Complexly

The classic, and complex, way of spending funds sent to a multisignature address using `bitcoin-cli` requires you to do a lot of foot work.

### Find Your Funds

To start with, you need to find your funds, because your computer doesn't know to look for them, because they're not associated with any addresses in your walet. You can alert `bitcoind` to do so using the `importaddress` command:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz
```
This command can take a while to run because it does a `rescan` to find all the related transactions. Afterward the funds should show up when you `listunspent` ... but they aren't easily spendable yet.
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

### Set Up Your Variables

When you're ready to spend the funds received by a multisignature address, you're going need to collect a _lot_ of data: much more than you need when you spend a normal P2PKH UTXO. That's in part because the info on the multisig address isn't in your wallet, and in part because you're spending money that was sent to a P2SH (pay-to-script) address, and that's a lot more demanding, as future chapters will explain.

In total, you're going to need to collect four things: extended information about the UTXO; the redeemScript; a recipient address; and all the private keys involved. We'll collect the first three now, then save the private keys for the signing step.

#### Access the UTXO information

To start with, grab the `txid` and the `vout` for the transaction that you want to spend, as usual. For example, if it's your 0th UTXO:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
```
However, you need to also access a third bit of information about the UTXO, its `scriptPubKey`, which is the script that locks the transaction. This is done with a simple `bitcoin-cli`/`jq` invocation that mirrors the previous commands.
```
$ utxo_spk=$(bitcoin-cli listunspent | jq -r '.[0] | .scriptPubKey')
```

#### Record the Redeem Script

Hopefully, you saved the `redeemScript`. Now you should record it in a variable.
```
$ redeem_script="52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
```
#### Decide Your Recipient

We're just going to send the money back to ourself. This is useful because it frees the funds up from the multisig, converting them into a normal P2PKH transaction that can be confirmed by a single private key:
```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
### Create Your Transaction

You can now create your transaction. To do so you add new `inputs` variables for the `scriptPubKey` and the `redeemScript`:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' outputs='''{ "'$recipient'": 1.299}''')
```

### Sign Your Transaction

You're now ready to sign your transaction. This is a multi-step process because you'll need to do it on multiple machines, each of which will contribute their own private keys.

#### Dump Your First Private Key

Because this transaction isn't making full use of your wallet, you're going to need to directly access your private keys. Start on `machine1`, where you should retrieve any of that user's private keys that were involved in the multisig:
```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw
```
> **WARNING:** Directly accessing your private keys from the shell is very dangerous behavior and should be done with extreme care if you're using real money. At the least, don't save the information into a variable that could be accessed from your machine. Removing your shell's history is another great step.

#### Make Your First Signature

You can now make your first signature with the `signrawtransaction` command. This time you must add two new variables: `prevtxs` should be a list of all the `inputs` from your raw transaction, which can just be cut-and-paste; while `privkeys` should be a list of all the private keys you dumped on this machine.
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
That produces scary errors and says that it's not `complete`. This is all all correct. You can see that the signature has been partially successfully because the `hex` has gotten longer. The transaction has been partially signed but is not done because it needs more signatures. 

### Repeat for Other Signers

You can now pass the transaction on, to be signed again by anyone else required for the mutisig. They do this by running the same signing command that you did but: (1) with the longer `hex` that you output (`bitcoin-cli -named signrawtransaction hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]' | jq -r '. | .hex'`); and (2) with their own private key.

> **M-OF-N VS N-OF-N:** Obviously, if you have an n-of-n signature (like the 2-of-2 multisignature in this example), then everyone has to sign, but if you hae a m-of-n multisignature where "m < n", then the signature will be complete when only some ("m") of the signers have signed.

Each of these signers will follow the same procedure. 

First, they access their private keys:
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
You'll note that this now says signature is `complete`. You should now fall back on the standard JQ methodology to save your `hexstring` and then to send it:
```
$ signedtx = $(bitcoin-cli -named signrawtransaction hexstring=020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B"]' | jq -r '.hex')
user1@blockstream2:~$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```
Whew! That took some work, but the multisignature transaction was successfully reused!

## Optional: Spend Funds Sent to a Multisig Address, Simply

The standard technique for creating multisignature addresses and for spending their funds is complex, but is a worthwhile exercise for understanding a bit more about how they work, and how you can manipulate them at a relatively low level. However, Bitcoin core has made multisigs a little bit easier in new releases. 

The following describes an easier way to spend funds received at a multisig address, but it requires you to have prepared beforehand with a new `addmultisigaddress` command. It's probably not what you'd want to do if you were writing multisig wallet programs, but if you were just trying to receive some funds by hand, it might save you some hair-pulling.

> **VERSION WARNING:** The `addmultisigaddress` command is available in Bitcoin Core v 0.10 or higher.

### Prepare with addmultsigaddress

You start off creating P2PKH addresses and retrieving public keys as usual for each user who will be part of the multisig:
```
machine1$ address3=$(bitcoin-cli getnewaddress)
machine1$ echo $address3
mkMkhbUzcSPdEHUoRQkBKHe8otP1SzWWeb
machine1$ bitcoin-cli -named validateaddress address=$address3 | jq -r '. | .pubkey'
02e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba4

machine2$ address4=$(bitcoin-cli getnewaddress)
$ echo $address4
mkyeUBPDoeyFrfLE4V5oAQfee99pT2W1E3
$ bitcoin-cli -named validateaddress address=$address4 | jq -r '. | .pubkey'
030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e07776
```
But now you're going to create the multisig on _each machine that contributes signatures_ using a new command, `addmultisigaddress`, instead of `createmultisig`. This new command saves some of the information into your wallet, making it a lot easier to spend the money afterward.
```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$address3'","030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e07776"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["02e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba4","'$address4'"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```
As noted in the previous section, it doesn't matter whether you use addresses or public keys: you'll get the same multisig address. However, you must use the same order. Thus, it's best for the members of the multisig to check amongst themselves to make sure they all got the same result.

The members of the multisig will still need to run `importaddress` to watch for funds received on the multisig address:
```
machine1$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```

### Respend with a Fairly Normal Transaction

Afterward, you will be able to receive funds on the multisiganture address as normal. The use of `addmultisigaddress` is simply a bureaucratic issue on the part of the recipients: a bit of bookkeeping to make life easier for them.

But, it makes life a lot easier. Because everything is in the wallet, the signers will be able to resepend the funds sent to the multisignature address exactly the same as any other address ... other than the need to sign on multiple machines.

To respend from a multisignature address where `addmultisigaddress` has saved the information into your wallet, you start by collecting your variables, but just the normal ones:
```
machine1$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
machine1$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout') 
machine1$ recipient=$(bitcoin-cli getrawchangeaddress)
```
You create a raw transaction:
```
machine1$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.2995}''')
```
Then you sign it:
```
machine1$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex
{
  "hex": "02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000",
  "complete": false,
  "errors": [
    {
      "txid": "7da644e75010fbd026f042a9d0bfd7a83563e24a7c3a615ce388f4451ca6cd4e",
      "vout": 0,
      "scriptSig": "00473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652ae",
      "sequence": 4294967295,
      "error": "Operation not valid with the current stack size"
    }
  ]
}
```
Note that you didn't have to save any other variables or add any other input information to these commands. Most importantly, you didn't make your private keys vulnerable by directly manipulating them. Instead the process was _exactly_ the same as respending a normal UTXO, except that the transaction wasn't fully signed at the end.

### Sign It On Other Machines

The final step is exporting the partially signed `hex` to any other machines and signing it again:
```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000 | jq -r '.hex')
```
When everyone that's required has signed, you're off to the races:
```
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d
```
As with the shortcut demonstrated in [4.5: Sending Coints with Automated Raw Transactions](4_5_Sending_Coins_with_Automated_Raw_Transactions.md), the result is a lot easier, but you lose some control in the process.

## Summary: Spending a Transaction with a Multisig

It turns out that spending money sent to a multisig address can take quite a bit of work. But as long as you have your original addresses and your redeemScript, you can do it with `bitcoin-cli`; if you want to make things even easier, you can take advantage of wallet features in newer version of Bitcoin Core.
