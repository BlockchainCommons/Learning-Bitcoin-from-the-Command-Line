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

EXAMPLES FOR LATER:

```
bitcoin-cli -named createmultisig nrequired=2 keys='''["'$address1'","'$address2'","'$address3'"]'''
{
  "address": "2MuuAVa8xVRq6GbirCQHsYR9wvVNtWooEz7",
  "redeemScript": "52210215a369d12f462ad9e909356afad1dc6b8002cf4b59c9609114d0a86654d6bc642102f367a7cdaa87301ca8c68bd33797e11cc0edccf1bc9e6306171d16bb843c4beb21025a21ac5b7616c777c70a0b615255139dc43b700201a576acb34ec43c25b87fde53ae"
}
$ bitcoin-cli -named createmultisig nrequired=3 keys='''["'$address1'","'$address2'","'$address3'"]'''
{
  "address": "2NFi2zxZNSVnC6AWezif2BVKj94ubt3zS72",
  "redeemScript": "53210215a369d12f462ad9e909356afad1dc6b8002cf4b59c9609114d0a86654d6bc642102f367a7cdaa87301ca8c68bd33797e11cc0edccf1bc9e6306171d16bb843c4beb21025a21ac5b7616c777c70a0b615255139dc43b700201a576acb34ec43c25b87fde53ae"
}
$ bitcoin-cli -named createmultisig nrequired=1 keys='''["'$address1'","'$address2'","'$address3'"]'''
{
  "address": "2MyKi9zjfdJVy7UpS4GUD2od4PhHQNaFPMV",
  "redeemScript": "51210215a369d12f462ad9e909356afad1dc6b8002cf4b59c9609114d0a86654d6bc642102f367a7cdaa87301ca8c68bd33797e11cc0edccf1bc9e6306171d16bb843c4beb21025a21ac5b7616c777c70a0b615255139dc43b700201a576acb34ec43c25b87fde53ae"
}
```

## Record a Multisig Address

## Send to a Multisig Address

## Spend Funds Sent to a Multisig Address

## Summary: Sending a Transaction to a Locktime

_What is the power of multisignatures?_
