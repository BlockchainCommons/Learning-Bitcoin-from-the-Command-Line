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

### Collect the Addresses

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

#### Make Sure It's a Full Public Key for Remote Machines

But, there's a catch! You might recall that a Bitcoin address is actually the hash of a public key, not the public key itself. But, you need the full public key to create a multisignature! If you created the address on your machine, no problem. The full public key (and the private key for that matter) is sitting in your wallet. But if the address was created on a remote key, you don't have that. 

So, to collect an address from a remote user for the creation of a multisignature, you need them to send you the full public key, which they can access with the `validateaddress` command.
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

This process needs to be undertaken for _every_ address from a machine other than the one where the multisig is to be created. Obviously, if some third-party is creating the address, then the full publickey will need to be sent for _every_ address.

### Create the Address

A multisig can now be created with the `createmultisig` command:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$address1'","0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28"]'''
{
  "address": "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz",
  "redeemScript": "52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
}
```
Note that if you instead want to create an m-of-n signature wither "m < n", you just adjust the `nrequired` field or the number of signatures in the `keys` JSON object. For a 1-of-2 multisig, you'd just set `nrequired=1` while for a 2-of-3 multisig, you'd leave `nrequired=2`, but add one more public key or address to the `keys` listing.

Also note that the `createmultisig` command can be used with asymmetric inputs. In this case, it processed `$address1`, which is a public-key hash address, and `0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28` is a public key. The `createmultisig` command is smart enough to try to look up the full public key if you feed it in address. If it's not in your local wallet, expect to see a "no full public key for address" error.

When used correctly, `createmultisig` returns two results, both of which are critically important.

The _address_ is what you'll give out to people to receive money. They can send to it exactly like a standard P2PKH address, but they'll know that it's actually a P2SH address because it has a different prefix. The number `2` at the start of any address marks it as P2SH on the Testnet.

> **TESTNET vs MAINNET:** On mainnet, the prefix is instead `3` for P2SH addresses.

The _redeemScript_ is required to actually use the funds, alongside the private keys, of course. This will be fully explained in "8.2: Scripting with a Multisig Script". For now, be sure you don't lose your redeemScript. Fortunately, it can also be recreated at a later time by rerunning `createmultisig` with _all_ of the public keys. So, more broadly, don't lose your redeemScript along with any of the public keys. (But really, don't lose the redeemScript.)

### Optional: Save the Address

The reason that you need to be careful about not losing your redeemScript is because `bitcoin-cli` doesn't automatically save the multisignature to your wallet the way that it saves addresses that you create with the normal `getnewaddress` commands. If you wish to do so, rerun [[instead run?]] your multisig using the `addmultisigaddress` command and all the same arguments. 

[[EXAMPLE]]

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

## Summary: Sending a Transaction with a Multisig

_What is the power of multisignatures?_
