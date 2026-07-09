# 3.3: Setting Up Your Wallet

You're now ready to start working with Bitcoin. To begin with, you'll
need to create a wallet for sending and receiving funds.

> ⚠️ **Version Warning:** Bitcoin Core is constantly
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

```sh
ls ~/.bitcoin/signet/wallets

| 
```

That's because Bitcoin Core doesn't create a wallet by default, but creating one is quite simple. You just run `bitcoin-cli createwallet` and choose a name for your wallet. If you choose no name (`""`) you're creating a wallet that will be loaded by default (which is helpful!). But you could also choose a real name like `"mainwallet"` if you plan to run multiple wallets. (We suggest the use of a no-name wallet for this tutorial, for reasons of simplicity, as we'll discuss later.)

Here's what the creation looks like:
```sh
bitcoin-cli createwallet ""

| {
|   "name": ""
| }
```

If you created a `""` wallet, your `wallets` directory will have files in it:

```sh
| wallet.dat  wallet.dat-journal
```

If you created a named wallet, the `wallets` directory will instead have a subdirectory by that name, and the subdirectory will have the files.

```sh
ls ~/.bitcoin/signet/wallets

| mainwallet

ls ~/.bitcoin/signet/wallets/mainwallet

| wallet.dat  wallet.dat-journal
```

Each wallet will have a `wallet.dat` file (which is a SQLite database) and a `wallet.dat-journal` file (which is an SQLite rollback journal).

You can see details on your wallet with the `bitcoin-cli getwalletinfo` command:

```sh

bitcoin-cli getwalletinfo
| {
|   "walletname": "",
|   "walletversion": 169900,
|   "format": "sqlite",
|   "txcount": 0,
|   "keypoolsize": 4000,
|   "keypoolsize_hd_internal": 4000,
|   "paytxfee": 0.00000000,
|   "private_keys_enabled": true,
|   "avoid_reuse": false,
|   "scanning": false,
|   "descriptors": true,
|   "external_signer": false,
|   "blank": false,
|   "birthtime": 1770328126,
|   "flags": [
|     "last_hardened_xpub_cached",
|     "descriptor_wallet"
|   ],
|   "lastprocessedblock": {
|     "hash": "0000000bd5046d28c2c02a60be14f2d3f1909277e574fa01279190dbe468a6e0",
|     "height": 290283
|   }
| }
```
> 📖 ***What is a Bitcoin wallet?*** A Bitcoin wallet is the digital equivalent of a physical wallet. It stores information on the amount of bitcoins you have and where it's located (addresses), as well as the ways you can use to spend it. Spending physical money is intuitive, but to spend Bitcoin, users need to provide the correct _private key_. We will explain this in more detail throughout the course, but what you should know for now is that this public-private key dynamic is part of what makes Bitcoin secure and trustless. Your key pair information is saved in the `wallet.dat` file, in addition to data about preferences and transactions. For the most part, you won't have to worry about that private key: `bitcoind` will use it when it's needed. However, this makes the `wallet.dat` file extremely important: if you lose it, you lose your private keys, and if you lose your private keys, you lose your funds!

You now have a Bitcoin wallet. But you can't receive funds with a wallet. For that you need an address, which is a specific repository for funds, derived from the private key information in your wallet: its one private key can generate many addresses.

## Backup Your Wallet

You can backup your wallet with the `bitcoin-cli backupwallet` command:

```sh
mkdir ~/backups
bitcoin-cli backupwallet ~/backups/mainwallet.dat
```
This will create a backup of the SQLite file that you can later restore from with the `bitcoin-cli restorewallet` command. It's probably best used if you'll be backing up your wallet to a secure (encrypted) storage area.

Just having a single backup will usually be enough to recover your wallet, because it'll contain the seed used to generate the wallet (more on that next chapter). However, it's good to regularly backup your wallet so that you don't lose data about your transactions. (A few commands also force the creation of a new seed, and you'd need to backup your wallet again in those cases.)

## Encrypt Your Wallet (Optional)

You can choose to encrypt your wallet. (It actually just encrypts the private information in your wallet, such as your private keys.) This is done with the `bitcoin-cli encryptwallet` command. 

```sh
bitcoin-cli encryptwallet "your-great-password"

| wallet encrypted; The keypool has been flushed and a new HD seed was generated. You need to make a new backup with the backupwallet RPC.
```
You should _definitely_ encrypt your wallet if you are using `bitcoin-cli` to deal with real money, but you probably shouldn't do it for this course (or any other use of test networks) because it'll just add new hoops to jump through test out funds. Note that this is one of those functions that generates an additional seed: make sure you backup your wallet again afterward if you run it.

Once you have encrypted your wallet, you'll be required to enter a passphrase prior to running many commands:

```sh
bitcoin-cli walletpassphrase "your-great-password" 3600
```
This will unlock your wallet for a set number of seconds (here, 3600 seconds, which is an hour), after which you'll again be required to enter your passphrase.

If you want to lock your wallet back up earlier than that, you can use `bitcoin-cli walletlock`:

```sh
bitcoin-cli walletlock
```

## Reload Named Wallets (Optional)

If you created a no-name (`""`) wallet, it will automatically load when you restart `bitcoind`. However, if you instead created a named wallet, it will not reload, forcing you to (initially) reload it by hand when you restart `bitcoind`.

```sh
bitcoin-cli loadwallet mainwallet

| {
|   "name": "mainwallet"
| }
```
You can make a named wallet load automatically on future startups by adding a `true` to the end of the `bitcoin-cli loadwallet` command:

```sh
bitcoin-cli loadwallet mainwallet true

| {
|   "name": "mainwallet"
| }
```
Afterward, the wallet that you must recently flagged in this way should always be the one that appaers at startup.

## Create Multiple Wallets (Optional)

You can create multiple wallets if it's helpful for separating funds or separating different sorts of expenses:

```sh
bitcoin-cli createwallet "gamingfunds"

| {
|   "name": "gamingfunds"
| }
```
`bitcoin-cli listwallets` will list out all of the wallets currently "loaded", which will include your newly created one(s).

```
bitcoin-cli listwallets

| [
|   "mainwallet",
|   "gamingfunds"
| ]
```
For commands that relate to the wallet (which is most of the ones you'll regularly use), you can specify which loaded wallet to use with the `-rpcwallet=` flag:

```sh
bitcoin-cli -rpcwallet=mainwallet getnewaddress

| tb1qdve6cqp05acjpm77d7le5d07haplhkcc3ucr3k
```
If you try to run a wallet-related command when you have multiple wallets loaded without using the `-rpcwallet=` flag, you'll get an error:

```sh
| Multiple wallets are loaded. Please select which wallet to use by requesting the RPC through the /wallet/<walletname> URI path. Or for the CLI, specify the "-rpcwallet=<walletname>" option before the command (run "bitcoin-cli -h" for help or "bitcoin-cli listwallets" to see which wallets are currently loaded).
```

Rather than flagging every command, it's easier to use `bitcoin-cli unloadwallet` to unload the wallets that you're not currently using:

```sh
bitcoin-cli unloadwallet mainwallet

| {
| }

bitcoin-cli listwallets

| [
|   "gamingfunds"
| ]
```

You can then use the `bitcoin-cli loadwallet` to bring a wallet back into active use, and use both commands to swap around what you're using:

```sh
bitcoin-cli loadwallet mainwallet

| {
|   "name": "mainwallet"
| }

bitcoin-cli unloadwallet gamingfunds

| {
| }

bitcoin-cli listwallets

| [
|   "mainwallet"
| ]
```

As discussed above, named wallets will not load by default unless you
use `loadwallet` with `true` as the second argument. In this example
of `mainwallet` and `gamingfunds`, nothing would load when you
restarted `bitcoind` unless you specified otherwise.

In [§7.1](07_1_Creating_Multisig_Public_Keys.md), we'll create a
second wallet to provide watch-only access to multisigs. Be sure to
remember the `loadwallet` and `unloadwallet` commands, or else use
`-rpcwallet` so that you can properly access the right wallet.

## Prove Control (Optional)

In previous versions of Bitcoin Core, you were able to prove control of an address with the `bicoin-cli signmessage` command. This is generally a nice feature because it allows you to offer assurance to someone sending you funds that you'll definitely be able to retrieve those funds (or at least that you can currently). Because of the advent of descriptor wallets, which we'll talk about in the next section, this is no longer possible. You still _can_ prove control of an address by deriving a WIF-format private key from the descriptor for a particular address and then using `bitcoin-cli signmessagewithprivkey` with that specific key, but that's a complex process that goes beyond the scope of this course.

Just keep in mind for the moment that proof of control is a nice feature if you can manage it.

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. That address is stored in a wallet, which you can backup. But, there's more to both the wallet and the address: wallets are supported by an interoperable description system called descriptors; and addresses come in a variety of types. We'll cover those in the next two chapters before we finally get to some funds. We'll also be seeing lots more wallet commands in the future, but they'll be things like checking your balance, which require you to have those funds!

## What's Next?

Complete "Preparing Your Bitcoin Wallet" with [§3.4: Undestanding
the Descriptor Wallet](03_4_Understanding_the_Descriptor_Wallet.md).
