# 6.1: Sending a Transaction with a Multisig

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The first way to vary how you send a basic transaction is to use a multisig. This gives you the ability to require that multiple people (or at least multiple private keys) authorize the use of funds.

## Understand How Multisigs Work

In a typical P2PKH transaction, bitcoins are sent to an address based on your public key, which in turn means that the related private key is required to unlock the transaction, solving the cryptographic puzzle and allowing you to reuse the funds. But what if you could instead lock a transaction with _multiple_ private keys. This would effectively allow funds to be sent to a group of people, where those people all have to agree to reuse the funds. 

_What is a multisignature?_ A multisignature is a methodology that allows more than one person to jointly create a digital signature. It's a general technique for the cryptographic use of keys that goes far beyond Bitcoin.

Technically, a multisignature cryptographic puzzle is created by Bitcoin using the OP_CHECKMULTISIG command, and typically that's encapsulated in a P2SH address. [§8.4: Scripting a Multisig](08_4_Scripting_a_Multisig.md) will detail how that works more precisely. For now, all you need to know is that you can use `bitcoin-cli` command to create multisignature addresses; funds can be mailed to these addresses just like any normal P2PKH address, but multiple private keys will be required for the redemption of the funds.

_What is a multisignature transaction?_ A multisignature transaction is a Bitcoin transaction that has been sent to a multisignature address, thus requiring the signatures of certain people from the multisignature group to reuse the funds.

Simple multisignatures require everyone in the group to sign the UTXO when it's spent. However, there's more complexity possible. Multisignatures are generally described as being "m of n". That means that the transaction is locked with a group of "n" keys, but only "m" of them are required to unlock the transaction. 

_What is a m-of-n multisignature?_ In a multisignature, "m" signatures out of a group of "n" are required to form the signature, where "m ≤ n".

## Create a Multisig Address

In order to lock a UTXO with multiple private keys, you must first create a multisignature address. The examples used here show the creation (and usage) of a 2-of-2 multisignature.

### Create the Addresses

To create a multisignature address, you must first ready the P2PKH addresses that the multisig will combine. Best practice suggests that you always create new addresses. This means that the participants will each run the `getnewaddress` command on their own machine:
```
machine1$ address1=$(bitcoin-cli getnewaddress)
```
And:
```
machine2$ address2=$(bitcoin-cli getnewaddress)
```
Afterwards, one of the recipients (or perhaps some third party) will need to collect the signatures. 

#### Collect Remote Public Keys

But, there's a catch! You might recall that a Bitcoin address is actually the hash of a public key, not the public key itself. But, you need the full public key to create a multisignature! For any P2PKH addresses that were created on the machine that's creating the multisignature address, there's no problem. The full public key (and the private key for that matter) is sitting in that machine's wallet, so `bitcoin-cli` will be able to access them. But for any addresses created on remote machines, you'll need more.

As a result, any remote user must look up the complete information about his address and send the associated public key to the multisig creator. This can be done with the `validateaddress` command.
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

This process needs to be undertaken for _every_ address from a machine other than the one where the multisig is being built. Obviously, if some third-party is creating the address, then the full publickey will need to be sent for _every_ address.

> **WARNING:** Bitcoin's use of public-key hashes as addresses, instead of public keys, actually represents an additional layer of security. Thus, sending a public key slightly increases the vulnerability of the associated address, for some far-future possibility of a compromise of the elliptic curve. You shouldn't worry about having to occasionally send out a public key for a usage such as this, but you should be aware that the public-key hashes represent security, and so the actual public keys should not be sent around willy nilly.

### Create the Address

A multisig can now be created with the `createmultisig` command:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$address1'","0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28"]'''
{
  "address": "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz",
  "redeemScript": "52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
}
```
When creating the multisignature address, you list how many signatures are required with the `nrequired` argument (that's "m" in a "m-of-n" multisignature), then you list the total set of possible signatures with the `keys` argument (that's "n"). Note that the the `keys` entries can be asymmetric. In this case, we included `$address1`, which is a P2PKH address from a local machine, and `0367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a28`, which is a public key from a remote machine. The `createmultisig` command is smart enough to try to convert any addresses into public keys ... but if that info isn't in your local wallet, expect to see a "no full public key for address" error.

> **M-OF-N VS N-OF-N:** This example shows the creation of a simple 2-of-2 multisig. If you instead want to create an m-of-n signature where "m < n", you adjust the `nrequired` field and/or the number of signatures in the `keys` JSON object. For a 1-of-2 multisig, you'd set `nrequired=1`, while for a 2-of-3 multisig, you'd leave `nrequired=2`, but add one more public key or address to the `keys` listing.

When used correctly, `createmultisig` returns two results, both of which are critically important.

The _address_ is what you'll give out to people who want to send funds. You'll notice that it has a new prefix of `2`, rather than the prefixes you've seen on Bitcoin addresses to date. That's because `createmultisig` is actually creating a totally new type of address called a P2SH address. It works exactly like a standard P2PKH address for sending funds, but you'll need to do a lot more work to spend them. 

> **TESTNET vs MAINNET:** On testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

The _redeemScript_ is what you need to redeem the funds, along with the private keys for "m" of the "n" addresses. This script is another special feature of P2SH addresses and will be fully explained in [§8.1: Building a Bitcoin Script with P2SH](08_1_Building_a_Bitcoin_Script_with_P2SH.md). For now, just be aware that it's a bit of data that's required to get your money.

_What is a P2SH address?_ P2SH stands for Pay-to-script. It's a different type of receipient than a standard P2PKH address, used for funds whose redemption are based on more complex Bitcoin Scripts. `bitcoin-cli` uses P2SH encapsulation to help standardize and simplify its multisigs as "P2SH multisigs".

> **WARNING:** P2SH multisig addresses, like the ones created by `bitcoin-cli`, have a limit for "m" and "n" in multisigs based on the maximum size of the redeem script, which is currently 520 bytes. Pratically, you won't hit this unless you're doing something excessive.

### Save Your Work

Here's an important caveat: nothing about your multisig is saved into your wallet using these basic techniques. In order to later redeem money sent to this multisignature address, you're going to need to retain two crucial bits of information:

   * A list of the Bitcoin addresses used in the multisig.
   * The `redeemScript` output by `createmultsig`.
   
Technically, the `redeemScript` can be recreated by rerunning `createmultisig` with the complete list of addresses and/or public keys _in the same order_ and with the right m-of-n count. But, it's better to hold onto it and save yourself stress and grief.

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

## Summary: Sending a Transaction with a Multisig

Multisigs addresses lock funds to multiple private keys — possibly requiring all of those private keys for redemption, and possibly requiring just some from the set. They're easy enough to create with `bitcoin-cli` and they're entirely normal to send to. This ease is due in large part to the invisible use of P2SH (pay-to-script) addresses, a large topic that will get more coverage in the future.

_What is the power of multisignatures?_ Multisignatures allow the modeling of a variety of financial arrangements such as corporations, partnerships, committees, and other groups. A 1-of-2 multisig might be a married couple's joint bank account, while a 2-of-2 multisig might be used for large expenditures by a Limited Liability Partnership. Multisignatures also form one of the bases of Smart Contracts. For example, a real estate deal could be closed with a 2-of-3 multisig, where the signatures are submitted by the buyer, the seller, and an escrow agent. Once the escrow agent agrees that all of the conditions have been met, he frees up the funds for the seller; or alternatively, the buyer and seller can jointly free the funds.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§6.2: Spending a Transaction with a Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md).
