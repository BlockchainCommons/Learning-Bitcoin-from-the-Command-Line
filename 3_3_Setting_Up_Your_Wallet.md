# 3.3: Setting Up Your Wallet

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You're now ready to start working with Bitcoin. To begin with, you'll need to create an address for receiving funds.

## Create an Address

The first thing you need to do is create an address for receiving payments. This is done with the `bitcoin-cli getnewaddress` command. Remember that if you want more information on this command, you should type `bitcoin-cli help getnewaddress`.

Theoretically, you could run it just by typing it on the command line:
```
$ bitcoin-cli getnewaddress
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
However, this isn't best practice. Though your address _is_ saved away in your wallet for your future use, you could easily make a mistake if you were retyping or cutting it somewhere else. And then you're telling people to send money to somewhere else (or to nowhere!). So instead we suggest a best practice, which is meant to minimize address mistakes.

[[move]]
## Best Practices: Use Variables to Capture Addresses

Instead, use your shell's built-in variables to capture your address.
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress)
```
These commands clear the NEW_ADDRESS_1 variable, then fill it with the results of the `bitcoin-cli getnewaddress` command.

You can then use your shell's `echo` command to look at your (new) address:
```
$ echo $NEW_ADDRESS_1
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
Note that this address begins with an "n" (or sometimes an "m"). This signifies that this is a testnet address. 

> **TESTNET vs MAINNET:** The equivalent mainnet address would start with a 1.

We'll use this same technique for other bitcoin-cli results in the future; note that you could do it all by hand, instead of piping stuff in and out of variables ... but we really don't suggest it.

## Optional: Capture the Private Key

The address lets you receive bitcoins, but to spend them, you'll need the address' private key. Again, this is all stored in your wallet, and it's not something that you usually need to worry about. But, if you do need it for some purpose (such as proving ownership from some other machine), then you can access the private key with the `bitcoin-cli dumpprivkey` command.
```
$ bitcoin-cli dumpprivkey "$NEW_ADDRESS_1"
cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
We opted not to put this in a variable, because it's not something you want floating around ...

## Optional: Sign a Message

You can also sign a message using your address. This verifies that the message for the address was signed by the person who knew the address' private key. You do this with `bitcoin-cli signmessage [address] [message]`. For example:
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
$ echo $NEW_SIG_1
H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=
```
 A recipient can verify the signature if he inputs the address, the signature, and the message.
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
true
```
If some black hat was making up signatures, they'd instead get a negative result:
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "FAKEBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
false
```

## Optional: Dump Your Wallet

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. We suggest using variables to capture your address, to ensure that you give out the correct address in the future. Based on that address, you can also access a private key and sign messages.
