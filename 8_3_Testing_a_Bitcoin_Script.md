# 8.3: Testing a Bitcoin Script

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Bitcoin Scripting allows for considerable new control over Bitcoin transactions, but it's all somewhat abstract. How do you actually turn that theoretical Bitcoin Script into functional Bitcoin code? The first answer is to test it.

## Get the Code Right

The object of testing your Bitcoin Scripts is to get the code right. Don't worry about the whole rigamarole of P2SH scripting (yet). Just worry about producing the Bitcoin Script code itself and making sure it runs and does what you expect.

This is a critically important aspect of any Bitcoin Scripting, because there's _no other_ check on your Script. Bitcoin will often protect you from trying to send bad transactions, but it _won't_ protect you from writing bad locking scripts. That's because the P2SH methodology actually keeps Bitcoin from knowing about the actual Script until it tries to unlock it ... and if your script doesn't run at that point, your funds are gone.

So the onus is on you to get that code right.

## Retrieve Public Keys & Signatures

Most of the Bitcoin code that you're testing will be simple operators. However, you'll also need have some Bitcoin addresses, public keys, and signatures to play with. Here's a quick reminder on how to retrieve this information:

_Create an address:_
```
$ address=$(bitcoin-cli getnewaddress)
$ echo $address
mxwLPpo2X1VXXAiLHNVb55H4JASeLMDm7A
```
_Retrieve the public key:_
```
$ bitcoin-cli -named validateaddress address=$address | jq -r '.pubkey'
03e9ddbea8336e53a7125e5be016411ffae4df06026fc3f947337ea08b74cb8476
```

_Create a signature:_
bitcoin-cli signmessage $address "Secure"
IO2w2knDwAig4eQxMnq+FwAoj3W1gCO/GMyzmuyTs3n3YADPdrIhfD9PVtBhCIkP0M+6NermgGz23qs86XT8nHs=

[[[MAYBE]]

## Test a Script Online

A few online resources exist for looking at scripts, each with some of their own quirks.

### The Script Playground

Charlie Marsh has built an excellent [Script Playground](http://www.crmarsh.com/script-playground/). Just put together your unlocking script and your locking script and run them. The Script Playground evaluates Bitcoin Script as if it were unlocking a transaction. That means that, just like Bitcoin, it does an OP_VERIFY at the end. As a result, `51 48 OP_ADD` won't produce anything useful (other than a green checkmark), but you can run `51 48 OP_ADD 99 OP_EQUAL` and `51 47 OP_ADD 99 OP_EQUAL` and it'll tell you which of those would verify.

The Playground tries to be smart about converting constants without requiring you to messily enter them by hand as bytecode, but values over 100 don't appear to work.

_Bitcoin Realism:_ Runs an `OP_VERIFY` at the end of every transaction.

_Keys & Signatures:_ 


## Test a Script with Java

## Test a Script with Bitcoin

## Summary: Testing a Bitcoin Script
