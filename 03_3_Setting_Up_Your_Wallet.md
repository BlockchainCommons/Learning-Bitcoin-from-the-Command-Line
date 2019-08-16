# 3.3: Setting Up Your Wallet

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You're now ready to start working with Bitcoin. To begin with, you'll need to create an address for receiving funds.

## Create an Address

The first thing you need to do is create an address for receiving payments. This is done with the `bitcoin-cli getnewaddress` command. Remember that if you want more information on this command, you should type `bitcoin-cli help getnewaddress`.
```
$ bitcoin-cli getnewaddress "" legacy
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
Note that this address begins with an "n" (or sometimes an "m"). This signifies that this is a testnet address.

The "legacy" flag is necessary to generate a traditional address, rather than a p2sh-segwit or bech32 address. The legacy address is currently required from the command line to make sure that signing works correctly.

> **TESTNET vs MAINNET:** The equivalent mainnet address would start with a 1.

Take careful note of the address. You'll need to give it to whomever will be sending you funds.

_What is a Bitcoin address?_ A Bitcoin address is literally where you receive money. It's like an email address, but for funds. However unlike an email address, a Bitcoin address should be considered single use: use it to receive funds just _once_. When you want to receive funds from someone else or at some other time, generate a new address. This is suggested in large part to improve your privacy. The whole blockchain is immutable, which means that explorers can look at long chains of transactions over time, making it possible to statistically determine who you and your contacts are, no matter how careful you are. However, if you keep reusing the same address, then this becomes even easier.

_What is a P2PKH address?_ A Bitcoin address is also something else: a public key (or more precisely, the 160-bit hash of a public key). For this reason it's called a Pay to PubKey Hash (or P2PKH) address. This public key of your key pair allows you to receive money, while an associated private key lets you spend that money. However, bitcoins may be sent to other sorts of addresses: Pay to Script Hash (P2SH) addresses feature prominently in the latter part of this tutorial.

_What is a Bitcoin wallet?_ By creating your first Bitcoin address, you've also begun to fill in your Bitcoin wallet. More precisely, you've begun to fill the `wallet.dat` file in your ~/.bitcoin/testnet3 directory. The `wallet.dat` file contains data about preferences and transactions, but more importantly it contains all of the key pairs that you create: both the public key (which is the source of the address where you receive funds) and the private key (which is how you spend those funds). For the most part, you won't have to worry about that private key: `bitcoind` will use it when it's needed. However, this makes the `wallet.dat` file extremely important: if you lose it, you lose your private keys, and if you lose your private keys, you lose your funds!

With a single address in hand, you could jump straight to the next section and begin receiving funds. However, before we get there, we're going to talk about a few other wallet commands that you might want to use in the future.

## Optional: Sign a Message

Sometimes you'll need to prove that you control a Bitcoin address (or rather, that you control its private key). This is important because it lets people know that they're sending funds to the right person. This can be done by creating a signature with the `bitcoin-cli signmessage` command, in the form `bitcoin-cli signmessage [address] [message]`. For example:
```
$ bitcoin-cli signmessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "Hello, World"
H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=
```
You'll get the signature as a return.

_What is a signature?_ A digital signature is a combination of a message and a private key that can then be unlocked with a public key. Since there's a one-to-one correspendence between the elements of a keypair, unlocking with a public key proves that the signer controlled the corresponding private key.

Another person can then use the `bitcoin-cli verifymessage` command to verify the signature. He inputs the address in question, the signature, and the message:
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
true
```
If they all match up, then the other person knows that he can safely transfer funds to the person who signed the message by sending to the address.

If some black hat was making up signatures, this would instead produce a negative result:
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "FAKEBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
false
```

## Optional: Dump Your Wallet

It might seem dangerous having all of your irreplaceable private keys in a single file. That's what `bitcoin-cli backupwallet` is for. It lets you make a copy of your wallet.dat:
```
$ bitcoin-cli backupwallet backup.dat
```
You can then recover it with `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet backup.dat
```

## Optional: View Your Private Keys

Sometimes, you might want to actually look at the private keys associated with your Bitcoin addresses. Perhaps you want to be able to sign a message or spend bitcoins from a different machine. Perhaps you just want to back up certain important private keys.

To look at _all_ the keys in your wallet, type `bitcoin-cli dumpwallet ~/mywallet.txt`.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/user1/mywallet.txt"
}
```
This will create a mywallet.txt file in your home directory with a long list of private keys, addresses, and other information. Mind you, you'd never want to put this data out in a plain text file on a Bitcoin setup with real funds!

More likely, you just want to look at the private key associated with a specific address. This can be done with the `bitcoin-cli dumpprivkey` command.
```
$ bitcoin-cli dumpprivkey "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf"
cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
You can then save that key somewhere safe, preferably somewhere not connected to the internet.

You can import any private key, from a wallet dump or an individual key dump, as follows:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Expect this to take a while, as `bitcoind` needs to reread all past transactions, to see if there are any new ones that it should pay attention to.

> **NOTE:** Many modern wallets prefer [mnemonic codes](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) to generate the seeds necessary to create the private keys. This methodology is not used `bitcoin-cli`, so you won't be able to generate handy word lists to remember your private keys.

_You've been typing that Bitcoin address you generated a _lot_, while you were signing messages and now dumping keys. If you think it's a pain, we agree. It's also prone to errors, a topic that we'll address in the very next section._

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. Your address is stored in a wallet, which you can back up. You can also do lots more with an address, like dumping its private key or using it to sign messages. But really, creating that address is _all_ you need to do in order to receive Bitcoin funds.

## What's Next?

Step back from "Understanding Your Bitcoin Setup" with [Interlude: Using Command-Line Variables](03_3__Interlude_Using_Command-Line_Variables.md).
