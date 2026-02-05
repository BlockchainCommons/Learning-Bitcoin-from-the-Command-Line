# 3.3: Setting Up Your Wallet

You're now ready to start working with Bitcoin. To begin with, you'll need to create a wallet for sending and receiving funds.

> :warning: **VERSION WARNING:** Bitcoin Core is constantly
evolving. To reflect this, previous versions of this course offered
"VERSION WARNING"s for features that had been recently introduced. For
example, Bitcoin Core v0.21.0 (January 2021) stopped creating wallets
by default and introduced experimental descriptor wallets, which then
became default with Bitcoin v23.0 (April 2022). These WARNINGS have
largely been removed, as they quickly become out of date. The
exception is for interoperable features that may or may not have been
adopted by remote nodes (e.g., new address types). But for your own
machine, just make sure you are using at least the version of Bitcoin
Core that is flagged in [Chapter
2](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) and you'll have access to
all the functions herein. (Though be aware that sometimes functions
are deprecated and removed, so if you are using a newer version
there's a small chance that something may no longer be available.)

## Create a Wallet

If you check your `wallets` directory, you'll see that it's currently empty.
```
$ ls ~/.bitcoin/signet/wallets
$
```

That's because Bitcoin Core doesn't create a wallet by default, but creating one is quite simple. You just run `bitcoin-cli createwallet` and choose a name for your wallet, such as `"mainwallet"`.
```
$ bitcoin-cli createwallet "mainwallet"
{
  "name": "mainwallet"
}
```

Now, your `wallets` directory should be populated.
```
$ ls ~/.bitcoin/signet/wallets
mainwallet
$ ls ~/.bitcoin/signet/wallets/mainwallet
wallet.dat  wallet.dat-journal
```

As this indicates, each wallet will have its own directory. (The main directory is used if you choose `""` for your wallet name.) Each directory will then have a `wallet.dat` file (which is a SQLite database) and a `wallet.dat-journal` file (which is an SQLite rollback journal).

> :book: ***What is a Bitcoin wallet?*** A Bitcoin wallet is the digital equivalent of a physical wallet. It stores information on the amount of bitcoins you have and where it's located (addresses), as well as the ways you can use to spend it. Spending physical money is intuitive, but to spend Bitcoin, users need to provide the correct _private key_. We will explain this in more detail throughout the course, but what you should know for now is that this public-private key dynamic is part of what makes Bitcoin secure and trustless. Your key pair information is saved in the `wallet.dat` file, in addition to data about preferences and transactions. For the most part, you won't have to worry about that private key: `bitcoind` will use it when it's needed. However, this makes the `wallet.dat` file extremely important: if you lose it, you lose your private keys, and if you lose your private keys, you lose your funds!

You now have a Bitcoin wallet. But you can't receive funds with a wallet. For that you need an address, which is a specific repository for funds, derived from the private key information in your wallet: its one private key can generate many addresses.

## Create an Address

The next thing you need to do is create an address for receiving payments. This is done with the `bitcoin-cli getnewaddress` command. Remember that if you want more information on this command, you should type `bitcoin-cli help getnewaddress`. There are a variety of types of addresses, due to Bitcoin's evolution over the years. [§4.1](04_1_Sending_Coins_The_Easy_Way.md) covers them all. For now, though we're just going to create an address of the default type, which is Bech32.

```
$ bitcoin-cli getnewaddress
tb1qmuqycfqfffvkys2h8rwqpgw6n0v6s0uhd235sj
```
Note that this address begins with an "tb1", which [means](https://en.bitcoin.it/wiki/List_of_address_prefixes) that it's a Bech32 address on either signet or testnet. The discussion of different address types in §4.1 will also talk about all of their identifying prefixes.

> :link: **SIGNET vs MAINNET vs TESTNET:** The equivalent mainnet address would start with a "bc1".

Take careful note of the address. You'll need to give it to whomever will be sending you funds.

> :book: ***What is a Bitcoin address?*** A Bitcoin address is literally where you receive money. It's like an email address, but for funds. It's based on a public key, though different address schemes adjust that in different ways. However unlike an email address, a Bitcoin address should be considered single use: use it to receive funds just _once_. When you want to receive funds from someone else or at some other time, generate a new address. This is suggested in large part to improve your privacy. The whole blockchain is immutable, which means that explorers can look at long chains of transactions over time, making it possible to statistically determine who you and your contacts are, no matter how careful you are. If you keep reusing the same address, then this becomes even easier.

By creating your first Bitcoin address, you've also begun to fill in your Bitcoin wallet. More precisely, you've begun to fill the `wallet.dat` file in your `~/.bitcoin/signet /wallets/{walletname}` directory. With a single address in hand, you could jump straight [§3.5: Receiving a Transaction](03_5_Receiving_a_Transaction.md) and begin receiving funds. However, before we get there, we're going to briefly discuss a few optional wallet commands that you might want to use in the future.

## Optional: Sign a Message

Sometimes you'll need to prove that you control a Bitcoin address (or rather, that you control its private key). This is important because it lets people know that they're sending funds to the right person. This can be done by creating a signature with the `bitcoin-cli signmessage` command, in the form `bitcoin-cli signmessage [address] [message]`. For example:
```
$ bitcoin-cli signmessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "Hello, World"
HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=
```
You'll get the signature as a return.

> :book: ***What is a signature?*** A digital signature is a combination of a message and a private key that can then be unlocked with a public key. Since there's a one-to-one correspendence between the elements of a keypair, unlocking with a public key proves that the signer controlled the corresponding private key.

Another person can then use the `bitcoin-cli verifymessage` command to verify the signature. He inputs the address in question, the signature, and the message:
```
$ bitcoin-cli verifymessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
true
```
If they all match up, then the other person knows that he can safely transfer funds to the person who signed the message by sending to the address.

If some black hat was making up signatures, this would instead produce a negative result:
```
$ bitcoin-cli verifymessage "FAKEV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
error code: -3
error message:
Invalid address
```

## Optional: Dump Your Wallet

It might seem dangerous having all of your irreplaceable private keys in a single file. That's what `bitcoin-cli dumpwallet` is for. It lets you make a copy of your wallet.dat:
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
```
The `mywallet.txt` file in your home directory will have a long list of private keys, addresses, and other information. Mind you, you'd never want to put this data out in a plain text file on a Bitcoin setup with real funds!

You can then recover it with `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet ~/mywallet.txt
```
But note this requires an unpruned node!
```
$ bitcoin-cli importwallet ~/mywallet.txt
error code: -4
error message:
Importing wallets is disabled when blocks are pruned
```

## Optional: View Your Private Keys

Sometimes, you might want to actually look at the private keys associated with your Bitcoin addresses. Perhaps you want to be able to sign a message or spend bitcoins from a different machine. Perhaps you just want to back up certain important private keys. You can also do this with your dump file, since it's human readable.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/standup/mywallet.txt"
}
```
More likely, you just want to look at the private key associated with a specific address. This can be done with the `bitcoin-cli dumpprivkey` command.
```
$ bitcoin-cli dumpprivkey "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B"
cTv75T4B3NsG92tdSxSfzhuaGrzrmc1rJjLKscoQZXqNRs5tpYhH
```
You can then save that key somewhere safe, preferably somewhere not connected to the internet.

You can also import any private key, from a wallet dump or an individual key dump, as follows:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Again, expect this to require an unpruned node. Expect this to take a while, as `bitcoind` needs to reread all past transactions, to see if there are any new ones that it should pay attention to.

> :information_source: **NOTE:** Many modern wallets prefer [mnemonic codes](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) to generate the seeds necessary to create the private keys. This methodology is not used `bitcoin-cli`, so you won't be able to generate handy word lists to remember your private keys.

_You've been typing that Bitcoin address you generated a _lot_, while you were signing messages and now dumping keys. If you think it's a pain, we agree. It's also prone to errors, a topic that we'll address in the very next section._

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. Your address is stored in a wallet, which you can back up. You can also do lots more with an address, like dumping its private key or using it to sign messages. But really, creating that address is _all_ you need to do in order to receive Bitcoin funds.

## What's Next?

Step back from "Understanding Your Bitcoin Setup" with [Interlude: Using Command-Line Variables](03_3__Interlude_Using_Command-Line_Variables.md).
