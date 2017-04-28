# 6.1: Sending a Transaction with a Multisig

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The first way to vary how you send a basic transaction is to use a multisig. This gives you the ability to require that multiple people (or at least multiple private keys) authorize the use of funds.

## Understand How Multisigs Work

[m of n]

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
