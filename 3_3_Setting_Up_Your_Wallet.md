# 3.3: Setting Up Your Wallet

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You're now ready to start working with Bitcoin. To begin with, you'll need to create an address for receiving funds.

## Create an Address

The first thing you need to do is create an address for receiving payments. This is done with the `bitcoin-cli getnewaddress` command. Remember that if you want more information on this command, you should type `bitcoin-cli help getnewaddress`.
```
$ bitcoin-cli getnewaddress
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
Note that this address begins with an "n" (or sometimes an "m"). This signifies that this is a testnet address. 

> **TESTNET vs MAINNET:** The equivalent mainnet address would start with a 1.

_What is a Bitcoin address?_ A Bitcoin address is literally where you receive money. It's like an email address, but for funds. However unlike an email address, a Bitcoin address should be considered a single-use token: use it to receive funds just _once_. When you want to receive funds from someone else, generate a new address. This is suggested in large part to improve your privacy. The whole blockchain is immutable, which means that explorers can look at long chains of transactions over time. This can make it possibly to statistically figure out who you and your contacts are, no matter how careful you are. However, if you keep reusing the same address, then it becomes pretty easy to see who you.

However a Bitcoin address is also something else: a public key (or more precisely, the 160-bit has of a public key). The public key (or address) allows you to receive money, while an associated private key lets you spend money.

_What is a Bitcoin wallet?_ By creating your first Bitcoin address, you've also begun to fill in your Bitcoin wallet. More precisely, you've begun to fill the `wallet.dat` file in your ~/.bitcoin/testnet3 directory. The `wallet.dat` file contains a considerable amount of data about prefeences and transactions, but most importantly, it contains all of the keypairs that you create on your local instance of Bitcoin: both the public key (which is to say the address, which you give people so that you can receive coins) and the private key (which is what you use to spend those coins). For the most part, you won't have to worry about that private key: `bitcoind` will use it as appropriate. However, this makes the `wallet.dat` file extremely important: if you lose it, you lose your private keys, and if you lose your private keys, you lose your funds!

With a single address in hand, you could jump straight to the next section, and begin receiving funds. However, before we get there, we're going to talk about a few other wallet commands that you might want to use in the future.


## Optional: Sign a Message

Sometimes you'll need to prove that you control a Bitcoin address (or rather, that you control its private key). This is important because it lets people know that they're sending funds to the right persons. This can be done by using the `bitcoin-cli signmessage` command, in the form `bitcoin-cli signmessage [address] [message]`. For example:
```
$ bitcoin-cli signmessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "Hello, World"
H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=
```
Another person can then use the `bitcoin-cli verifymessage` command to verify the signature. He inputs the address in question, the signature, and the message:
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
true
```
If they all match up, then the other person knows that he can safely transfer funds to the person who signed the message by sending to the address:.

If some black hat was making up signatures, this would instead produce a negative result:
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "FAKEBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
false
```

## Optional: Capture the Private Key

The address lets you receive bitcoins, but to spend them, you'll need the address' private key. Again, this is all stored in your wallet, and it's not something that you usually need to worry about. But, if you do need it for some purpose (such as proving ownership from some other machine), then you can access the private key with the `bitcoin-cli dumpprivkey` command.
```
$ bitcoin-cli dumpprivkey "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf"
cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
If you think it's a pain to have to constantly retype (or recopy) that public address, we agree. It's also prone to errors, a topic that we'll address in the next section. IN the meantime, as long as you were careful, you should now have a private key.


## Optional: Dump Your Wallet

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. We suggest using variables to capture your address, to ensure that you give out the correct address in the future. Based on that address, you can also access a private key and sign messages.
