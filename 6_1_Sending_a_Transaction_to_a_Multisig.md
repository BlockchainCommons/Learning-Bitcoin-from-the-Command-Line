# 6.1: Sending a Transaction with a Multisig

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The first way to vary how you send a basic transaction is to use a multisig. This gives you the ability to require that multiple people (or at least multiple private keys) authorize the use of funds.

## Understand How Multisigs Work

For a typical P2PKH transaction, bitcoins are sent to an address based on your public key, which in turn means that the related private key is required to unlock the transaction, solving the cryptographic puzzle and allowing you to reuse the funds. But what if you could instead lock a transaction with _multiple_ private keys. This would effectively allow keys to be sent to a group of people, where that group of people all has to agree to reuse the funds. 

It's a great model for corporations, partnerships, committees, and other groups, but it also has even wider capabilities ...

_What is a multisignature?_ A multisignature is simply a methodology that allows more than one person to jointly create a digital signature. It's a general technique for the cryptographic use of keys that goes far beyond Bitcoin.

_What is a multisignature transaction?_ A multisignature transaction is a Bitcoin transaction that requires the signatures of multiple people to reuse the funds.

Simple multisignatures just require everyone in a group to sign a transaction. However, there's more possible complexity than that. Multisignatures are generally described as being "m of n". That means that the transaction is locked with a group of "n" keys, but only "m" of them are required to unlock the transaction. If you've got a simple partnership, where both partners must authorize the reuse of funds, that's a "2 of 2" multisig. But, if you instead want to set up the equivalent of a join bank account where either partner can reuse funds, that's a "1 of 2" multisig. M-of-n multisigs where m < n are what allow for even more interesting uses of multisigs including escrows. For example, a real estate deal could be closed with a 2-of-3 multisig, where the signatures are submitted by the buyer, the seller, and the escrow agent. Once the escrow agent agrees that all of the conditions have been met, he frees up the funds for the seller; or alternatively, the buyer and seller can jointly free the funds.

_What is a m-of-n multisignature?_ A multisignature where "m" signatures out of a group of "n" are required to form the signature and "m ≤ n".

Technically, a multisignature is created by Bitcoin with the OP_CHECKMULTISIG command, and typically that's encapsulated in a P2SH address. Chapter 8 will detail how that works more precisely. For now, all you need to know is that you can use `bitcoin-cli` command to create multisignature addresses that can be mailed to just like any normal address, but which will require multiple private keys for redemption.

## Create a Multisig Address

In order to receive funds through a multisignature address, you must create a multisignature address. This example shows the creation of a 2-of-2 multisignature.

### Create the Addresses

The first step is to have each of the recipients for the multisignature address contribute their own address. 

This means that they'll each run the `getnewaddress` command on their own machine:
```
machine1$ address1=$(bitcoin-cli getnewaddress)
```
And:
```
machine2$ address2=$(bitcoin-cli getnewaddress)
```
Afterwards, one of the recipients (or perhaps some third party) will need to collect the signatures. 

#### Collect Remote Public Keys

But, there's a catch! You might recall that a Bitcoin address is actually the hash of a public key, not the public key itself. But, you need the full public key to create a multisignature! For any addresses created on your machine, no problem. The full public key (and the private key for that matter) is sitting in your wallet, so `bitcoin-cli` will be able to access them. But for any addresses created on remote machines, you'll need more.

The remote user must look up the complete information on his address and send you the associated public key. This can be done with the `validateaddress` command.
```
machine2$ bitcoin-cli -named validateaddress address=$address2
{
  "isvalid": true,
  "address": "mfduLxpR6Bq1ARctV2TauhetWwqnqH1vYS",
  "scriptPubKey": "76a9140150730730b1b681a7757f1188322dcb31d8ddbd88ac",
  "ismine": true,
  "iswatchonly": false,
  "isscript": false,
  "pubkey": "0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28",
  "iscompressed": true,
  "account": "",
  "timestamp": 1494285568,
  "hdkeypath": "m/0'/0'/1'",
  "hdmasterkeyid": "2333fedaf15c11ca577af6d9ac51d3c506fc13c5"
}
```
The `pubkey` address (`0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28`) is what's required.

This process needs to be undertaken for _every_ address from a machine other than the one where the multisig is being buil. Obviously, if some third-party is creating the address, then the full publickey will need to be sent for _every_ address.

### Create the Address

A multisig can now be created with the `createmultisig` command:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$address1'","0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28"]'''
{
  "address": "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz",
  "redeemScript": "52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
}
```
Note that the `createmultisig` command can be used with asymmetric inputs. In this case, it processed `$address1`, which is a public-key hash address from a local machine, and `0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28`, which is a public key from a remote machine. The `createmultisig` command is smart enough to try to convert any addresses into public keys ... but if that info isn't in your local wallet, expect to see a "no full public key for address" error.

> **M-OF-N VS N-OF-N:** This example shows the creation of a simple 2-of-2 multisig. If you instead want to create an m-of-n signature where "m < n", you adjust the `nrequired` field and/or the number of signatures in the `keys` JSON object. For a 1-of-2 multisig, you'd set `nrequired=1` while for a 2-of-3 multisig, you'd leave `nrequired=2`, but add one more public key or address to the `keys` listing.

When used correctly, `createmultisig` returns two results, both of which are critically important.

The _address_ is what you'll give out to people who want to send funds. You'll notice that it has a new prefix of `2`, rather than the prefixes you've seen on Bitcoin addresses to date. That's because `createmultisig` is actually creating a totally new type of address called a P2SH address. It works exactly like a standard P2PKH address for sending funds, but you'll need to do a lot more work to redeem. 

> **TESTNET vs MAINNET:** On testnet, the prefix for P2SH addresses is `2`, and on mainnet, it's `3`.

The _redeemScript_ is what you need to redeem the funds, along with the private keys for the associated addresses. This is another special feature of P2SH addresses and will be fully explained in "8.2: Scripting with a Multisig Script". For now, just be aware that it's a bit of data that's required to redeem your money.

### Save Your Work

Here's an important caveat: nothing about your multisig is saved into your wallet using these basic techniques. In order to later redeem money sent to this multisig address, you're going to need to retain two crucial bits of information:

   * A list of the Bitcoin addresses used in the multisig.
   * The `redeemScript` output by `createmultsig`.
   
Technically, the `redeemScript` can be recreated by rerunning `createmultisig` with the complete list of addresses and/or public keys _in the same order_ and with the right m-of-n count. But, it's better to hold onto it and save yourself the grief.

## Send to a Multisig Address

If you've got a multisignature in a convenient P2SH format, like the one generated by `bitcoin-cli`, it can be sent to exactly like a normal address.
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.2995}''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "ad16ea68a62af2d3930a48c5ca811bf66935f768bb369a85298ee6697167c667",
  "hash": "ad16ea68a62af2d3930a48c5ca811bf66935f768bb369a85298ee6697167c667",
  "size": 83,
  "vsize": 83,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "ad16098f5c8904a4de7c152efc56359c22be37d447cd78019c398791a7bdd928",
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
      "value": 1.29950000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL",
        "hex": "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz"
        ]
      }
    }
  ]
}
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
621be11aac439d6ec58be398058fc33c3e89cf45138a0e73e05b7001f9b6e328
```
As you can see, there was nothing unusual in the creation of the transaction, and it looked entirely normal, albeit with an address with a different prefix than normal (`2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz`).

## Spend Funds Sent to a Multisig Address

Obviously, if you've received funds sent to a multisig, you need to be able to spend it.

### Find Your Funds

You can see any transactions related to your funds if you import the new multisig address into your wallet:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz
```
This can take a while, because it does a `rescan` to find all the related transactions. Afterward the funds should show up when you `listunspent` ... but they aren't necessarily easily spendable yet.
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

When you're ready to spend your money received by a multisig address, you're going need to collect a _lot_ of data. Much more than you needed to when you spend a normal P2PKH UTXO. That's in part because the info on the multisig address isn't in your wallet, and in part because you're spending money that was send to a script address (P2SH), and that's a lot more demanding, as future chapters will explain.

In total, you're going to need to collect four things: extended information about the UTXO; the redeemScript; a recipient address; and all the private keys involved. We'll collect the first two now, then save the private keys for the signing step.

#### Access the UTXO information

To start with, grab the `txid` and the `vout` for the transaction that you want to spend, as usual.
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')

$ echo $utxo_txid
621be11aac439d6ec58be398058fc33c3e89cf45138a0e73e05b7001f9b6e328
$ echo $utxo_vout
0
```
However, you need to also access a third bit of information about the UTXO, its `scriptPubKey`, which is the script that locks the transaction. This is done with a simple `bitcoin-cli`/`jq` invocation that mirrors the previous information.
```
$ utxo_spk=$(bitcoin-cli listunspent | jq -r '.[0] | .scriptPubKey')
```

#### Record the Redeem Script

Hopefully, you saved the `redeemScript`. Now you should record it in a variable.
```
$ redeem_script="52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
```
#### Decide Your Recipient

We're just going to send the money back to ourself. This is useful because it frees the funds up from the multisig and convert them into a normal P2PKH transaction that can be confirmed by a single private key:
```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
### Create Your Transaction

You can now create your transaction, including new `inputs` variables for the `scriptPubKey` and the `redeemScript`:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' outputs='''{ "'$recipient'": 1.299}''')
```

### Sign Your Transaction

You're now ready to sign your transaction. This is a multi-step process because you'll need to do it on multiple machines, each of which will contribute their own private keys.

#### Dump Your Local Private Keys

Because this transaction isn't making full use of your wallet, you're going to need to directly access your private key. Start on `machine1` where you should get information on any of that user's keys that were involved in the multisig:
```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw
```
> **WARNING:** Directly accessing your private keys is subpar behavior, and should be done with extreme care if you're using real money. At the least, don't save the information into a variable that could be accessed from your machine. Removing your shell's history is another great step.

#### Make Your First Signature

You can now make your first signature with the `signrawtransaction` command, except you're going to have to add two variables: `prevtxs` should be a list of all the `inputs` from your raw transaction; while `privkeys` should be a list of all the private keys you dumped on this machine.
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
That produces scary errors and says that it's not `complete`. That's all correct. The transaction has been partially signed but is not done because it needs more signatures. Note in particular your `hex` has gotten longer, demonstrating that it's partially signed.

### Sign Your Transaction Again

Now you can pass the transaction on to be signed again by anyone else required for the mutisig. They do this by running the same signing command that you did but (1) with the longer `hex` that you output (`bitcoin-cli -named signrawtransaction hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]' | jq -r '. | .hex'`) and (2) with their own private key.

They would then dump their own private key on their own machine:
```
$ bitcoin-cli -named dumpprivkey address=$address2
cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B
```
And sign the new `hex` using all the same `prevtxs` values as you:
```
$ bitcoin-cli -named signrawtransaction hexstring=020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B"]'
{
  "hex": "020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b6200000000db00483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa01483045022100d5190eb824535423f67b15040efaba66953ea39f312540dd38504ed85ba6436402206171883ff28c235030550c36cadb31e40aaa9a74f71579557b74a5684545675c014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000",
  "complete": true
}
```
You'll note that this now says signature is complete, which means that you should just fall back on the standard JQ methodology to save your `hexstring` and then to send it:
```
$ signedtx = $(bitcoin-cli -named signrawtransaction hexstring=020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B"]' | jq -r '.hex')
user1@blockstream2:~$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```
Whew! That took some work, but the transaction was successfully used!

## Optional: Make Multisigs Easier

The above showed the standard technique for creating multisigs and spending funds received through them. It was a worthwhile exercise for understanding a bit more about how they work, and how you'd be manipulating them at a relatively low level. However, Bitcoin core has made multisigs a little bit easier in new releases. 

The following describes a slightly different way to create multisigs using the `addmultisigaddress` command, which then allows easier spending of the money. It's probably not what you'd want to do if you were writing multisig wallet programs, but if you were just trying to retrieving some funds by hand, it might save you some hair-pulling.

### Create Multisigs Everywhere

You start off creating addresses and retrieving public keys as usual for each user who will be part of the multisig:
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
But now you're going to create the multisig on _each machine that contributes signatures_ using a new command, `addmultisigaddress`. The difference between this and `createmultisig` is that it saves some of the information into your wallet, making it a lot easier to spend the money afterward.
```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$address3'","030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e07776"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["02e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba4","'$address4'"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```
As we noted earlier, it doesn't matter whether you use addresses or public keys: you'll get the same multisig address. However, you must use the same order. Thus, it's best for the members of the multisig to check amongst themselves to make sure they all got the same result.

The members of the multisig will still need to run `importaddress` to watch for funds received on the multisig address:
```
machine1$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```

### Respend Your Funds

There are no changes to sending or receiving funds. The use of `addmultisigaddress` is simply a technical issue on your sides as the recipient: a bit of bookkeeping to make life easier for you.


## Summary: Sending a Transaction with a Multisig

_What is the power of multisignatures?_
