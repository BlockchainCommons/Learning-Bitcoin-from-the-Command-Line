# 3.3: Setting Up Your Wallet

You're now ready to start working with Bitcoin. To begin with, you'll need to create a wallet for sending and receiving funds.

> :warning: **VERSION WARNING:** Bitcoin Core is constantly
evolving. To reflect this, previous versions of this course offered
"VERSION WARNING"s for features that had been recently introduced. For
example, Bitcoin Core v0.21.0 (January 2021) stopped creating wallets
by default and introduced experimental descriptor wallets, which then
became default with Bitcoin v23.0 (April 2022).
>
> These WARNINGS have
largely been removed in recent versions of this course, as they quickly become out of date. The
exception is for interoperable features that may or may not have been
adopted by remote nodes (e.g., new address types). But for your own
machine, just make sure you are using at least the version of Bitcoin
Core that is flagged in [Chapter
2](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) and you'll have access to
all the functions herein.
>
> Be aware that sometimes functions
are deprecated and removed, so if you are using a newer version
there's a small chance that something may no longer be available.

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

You can also see details on your wallet with the `bitcoin-cli getwalletinfo` command:
```
$ bitcoin-cli getwalletinfo
{
  "walletname": "mainwallet",
  "walletversion": 169900,
  "format": "sqlite",
  "txcount": 0,
  "keypoolsize": 3999,
  "keypoolsize_hd_internal": 4000,
  "paytxfee": 0.00000000,
  "private_keys_enabled": true,
  "avoid_reuse": false,
  "scanning": false,
  "descriptors": true,
  "external_signer": false,
  "blank": false,
  "birthtime": 1770319097,
  "flags": [
    "last_hardened_xpub_cached",
    "descriptor_wallet"
  ],
  "lastprocessedblock": {
    "hash": "00000010ea21796c6e54101bf757ca1e80d656856d1c71aa18282698edb4b49b",
    "height": 290276
  }
}
```
> :book: ***What is a Bitcoin wallet?*** A Bitcoin wallet is the digital equivalent of a physical wallet. It stores information on the amount of bitcoins you have and where it's located (addresses), as well as the ways you can use to spend it. Spending physical money is intuitive, but to spend Bitcoin, users need to provide the correct _private key_. We will explain this in more detail throughout the course, but what you should know for now is that this public-private key dynamic is part of what makes Bitcoin secure and trustless. Your key pair information is saved in the `wallet.dat` file, in addition to data about preferences and transactions. For the most part, you won't have to worry about that private key: `bitcoind` will use it when it's needed. However, this makes the `wallet.dat` file extremely important: if you lose it, you lose your private keys, and if you lose your private keys, you lose your funds!

You now have a Bitcoin wallet. But you can't receive funds with a wallet. For that you need an address, which is a specific repository for funds, derived from the private key information in your wallet: its one private key can generate many addresses.

## Create an Address

The next thing you need to do is create an address for receiving payments. This is done with the `bitcoin-cli getnewaddress` command. Remember that if you want more information on this command, you should type `bitcoin-cli help getnewaddress`. There are a variety of types of addresses, due to Bitcoin's evolution over the years. [§4.1](04_1_Sending_Coins_The_Easy_Way.md) covers them all. For now, though we're just going to create an address of the default type, which is Bech32.

```
$ bitcoin-cli getnewaddress
tb1qhlcqx0rmqctujw94l2rw9r8umx7keerp38lc06
```
Note that this address begins with an "tb1", which [means](https://en.bitcoin.it/wiki/List_of_address_prefixes) that it's a Bech32 address on either signet or testnet. The discussion of different address types in §4.1 will also talk about all of their identifying prefixes.

> :link: **SIGNET vs MAINNET vs TESTNET:** The equivalent mainnet address would start with a "bc1".

Take careful note of the address. You'll need to give it to whomever will be sending you funds.

> :book: ***What is a Bitcoin address?*** A Bitcoin address is literally where you receive money. It's like an email address, but for funds. It's based on a public key, though different address schemes adjust that in different ways. However unlike an email address, a Bitcoin address should be considered single use: use it to receive funds just _once_. When you want to receive funds from someone else or at some other time, generate a new address. This is suggested in large part to improve your privacy. The whole blockchain is immutable, which means that explorers can look at long chains of transactions over time, making it possible to statistically determine who you and your contacts are, no matter how careful you are. If you keep reusing the same address, then this becomes even easier.

By creating your first Bitcoin address, you've also begun to fill in your Bitcoin wallet. More precisely, you've begun to fill the `wallet.dat` file in your `~/.bitcoin/signet /wallets/{walletname}/` directory. With a single address in hand, you could jump straight [§3.5: Receiving a Transaction](03_5_Receiving_a_Transaction.md) and begin receiving funds. However, before we get there, we're going to briefly discuss backing up your wallet and a few optional wallet commands that you might want to use in the future.

## Backup Your Wallet

You can backup your wallet with the `bitcoin-cli backupwallet` command:
```
$ mkdir ~/backups
$ bitcoin-cli backupwallet ~/backups/mainwallet.dat
```
This will create a backup of the SQLite file that you can later restore from with the `bitcoin-cli restorewallet` command. It's probably best used if you'll be backing up your wallet to a secure (encrypted) storage area.

Just having a single backup will be enough to recover your wallet, because it'll contain the seed used to generate the wallet (more on that next chapter). However, it's good to regularly backup your wallet so that you don't lose data about your transactions.

## Optional: Encrypt Your Wallet

You can choose to encrypt your wallet. (It actually just encrypts the private information in your wallet, such as your private keys.) This is done with the `bitcoin-cli encryptwallet` command. 
```
$ bitcoin-cli encryptwallet "your-great-password"
wallet encrypted; The keypool has been flushed and a new HD seed was generated. You need to make a new backup with the backupwallet RPC.
```
You should _definitely_ encrypt your wallet if you are using `bitcoin-cli` to deal with real money, but you probably shouldn't do it for this course (or any other use of test networks) because it'll just add new hoops to jump through test out funds.

Once you have encrypted your wallet, you'll be required to enter a passphrase in order to do many commands:
```
$ bitcoin-cli walletpassphrase "your-great-password" 3600
```
This will unlock your wallet for a set number of seconds (here, 3600 seconds, which is an hour), after which you'll again be required to enter your passphrase.

If you want to lock your wallet back up earlier than that, you can use `bitcoin-cli walletlock`:
```
$ bitcoin-cli walletlock
```

## Optional: Proving Control

In previous versions of Bitcoin Core, you used to be able to prove control of an address with the `bicoin-cli signmessage` command. This is generally a nice feature because it allows you to assure someone sending you funds that they're sending to a place where you will definitely be able to retrieve the funds. Because of the advent of descriptor wallets, which we'll talk about in the next section, this is no longer possible. You still _can_ prove control of an address by deriving a WIF-format private key from the descriptor for a particular address and then using `bitcoin-cli signmessagewithprivkey` with that specific key, but it's a complex process that goes beyond the scope of this course.

Just keep in mind for the moment that proof of control is a nice feature if you can manage it.

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. That address is stored in a wallet, which you can back up. But, there's more to both the wallet and the address: they're supported by an interoperable description system called descriptors. Understanding that will be the topic of the next chapter, before we finally get some funds. We'll also be seeing lots more wallet commands in the future, but they'll be things like checking your balance, which require you to have those funds!

## What's Next?

Step back from "Understanding Your Bitcoin Setup" with [Interlude: Using Command-Line Variables](03_3__Interlude_Using_Command-Line_Variables.md).
