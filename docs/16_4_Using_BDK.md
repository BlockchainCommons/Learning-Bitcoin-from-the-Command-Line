# 16.4: Using BDK with Miniscript

[BDK](https://github.com/bitcoindevkit), the Bitcoin Dev Kit, is a
powerful set of libraries based on `rust-bitcoin` and `rust-minscript`
that can be used for coding Bitcoin functionality in Rust. It includes
[`bdk-cli`](https://github.com/bitcoindevkit/bdk-cli), a command-line
tool built on the BDK libraries that mirrors and expands much of the
functionality of `bitcoin-cli`. `bdk-cli` could support a "Learning"
course of its own; this chapter just shows how to install it, how to
use it for some very basic functionality, and most importantly how to
use it as a Policy compiler.

## Install BDK

`bdk-cli` is a Rust package, so it can be installed with cargo. You'll
want to add either the `electrum or the `rpc` feature (to talk to a
`bitcoind` server) and the `compiler` feature (to get access to BDK's
Policy compiler.

```
cargo install bdk-cli --features electrum,compiler
```

## Work with Bitcoind

### Choose Your Server

`bdk-cli` can be built to work with Esplora, Electrum, Kyoto, or
RPC. Usually, we'd choose RPC, so that we can use our own private
server ... except `bdk-cli` doesn't sync properly with a pruned node.

So we're instead going to use `electrum`, as
`ssl://mempool.space:60602` provides access to a public Signet
Electrum server.

### Config a Wallet

All commands in `bdk-cli` are hierarchical, so for example you have a
`wallet` command and under that a `config` command that allows you to
create a wallet from a descriptor (much like `importdescriptors` for
`bitcoin-cli`).

In this case, we're going to demonstrate basic `bdk-cli` functionality
(and also offer a real-world example of how descriptors are great for
moving funds between wallets) by copying a descriptor from
`bitcoin-cli` to `bdk-cli`.

Here's our Segwit descriptors from `bitcoin-cli`, with their private keys:

```sh
bitcoin-cli listdescriptors true

| {
|    ...
|     {
|       "desc": "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/0/*)#9fxgzcuj",
|       "timestamp": 1784828880,
|       "active": true,
|       "internal": false,
|       "range": [
|         0,
|         1000
|       ],
|       "next": 1,
|       "next_index": 1
|     },
|     {
|       "desc": "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/1/*)#5arfldv2",
|       "timestamp": 1784828881,
|       "active": true,
|       "internal": true,
|       "range": [
|         0,
|         999
|       ],
|       "next": 0,
|       "next_index": 0
|     }
|   ]
| }
```

The following command will then import those descriptors into
`bdk-cli`:

```sh
bdk-cli \
    --network signet \
    wallet --wallet bitcoincore config \
    --ext-descriptor "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/0/*)" \
    --int-descriptor "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/1/*)" \
    --url "ssl://mempool.space:60602" \
    --client-type electrum \
    --database-type sqlite 
```

(There are shorter versions of all these flags, and some of them
default to command-line variables, but here we're showing them in
their full glory to demonstrate how they work.)

After some warnings, you should see:

```
| {
|   "message": "Wallet 'bitcoincore' initialized successfully in \"/home/standup/.bdk-bitcoin/config.toml\""
| }
```

Commands like `wallets` and `descriptor` will now show you what's
available in your `bdk-cli` wallet.

```
bdk-cli descriptor

| {
|   "public_descriptors": {
|     "external": "wpkh([3fa9af92/84'/1'/0']tpubDD1TyyMFnYyV7wq9627FgdxHJ8gwHNXQyuJDunMnR1UQqiqw3rV7hnW1TfPbFbupm6K6SdxPhr8kyMvXbP2gAs22J6LQknwpGGH5JeYyC2S/0/*)#890undlk",
|     "internal": "wpkh([3fa9af92/84'/1'/0']tpubDD1TyyMFnYyV7wq9627FgdxHJ8gwHNXQyuJDunMnR1UQqiqw3rV7hnW1TfPbFbupm6K6SdxPhr8kyMvXbP2gAs22J6LQknwpGGH5JeYyC2S/1/*)#k32awc0w"
|   },
|   "private_descriptors": {
|     "external": "wpkh(tprv8ZgxMBicQKsPet8NfGxLAnqfd4WNwKLgtnaJrYaEvLHvbvMaEq7dR2nv1EFS8V2GCqVATmor9CueNkoHo26NtyEPAXbaazKWjf92UxczoMQ/84'/1'/0'/0/*)#cruklgqk",
|     "internal": "wpkh(tprv8ZgxMBicQKsPet8NfGxLAnqfd4WNwKLgtnaJrYaEvLHvbvMaEq7dR2nv1EFS8V2GCqVATmor9CueNkoHo26NtyEPAXbaazKWjf92UxczoMQ/84'/1'/0'/1/*)#fhehzasw"
|   },
|   "mnemonic": "orphan quote seek ankle raccoon major ivory lunch axis piece ostrich stem",
|   "fingerprint": "3fa9af92"
| }

bdk-cli wallets

| {
|   "bitcoincore": {
|     "wallet": "bitcoincore",
|     "network": "signet",
|     "ext_descriptor": "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/0/*)",
|     "int_descriptor": "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/1/*)",
|     "database_type": "sqlite",
|     "client_type": "electrum",
|     "server_url": "ssl://mempool.space:60602",
|     "batch_size": 10,
|     "proxy": null,
|     "proxy_auth": null,
|     "proxy_retries": 5,
|     "proxy_timeout": null
|   }
| }
```

You'll note wallet has saved information like your server and your
network so that you don't have to keep typing it in!

At this point, you'd usually want to check addresses. Here's the first
two addresses with `bdk-cli`:

```
bdk-cli wallet --wallet bitcoincore new_address

| {
|   "address": "tb1q7aecrwahqkewhhupt8ru5pumtnla65lhc9yuj2",
|   "index": 0
| }

bdk-cli wallet --wallet bitcoincore new_address

| {
|   "address": "tb1qg6pqcgltff4edp79hhzxc928l28lj7d2vqtxl4",
|   "index": 1
| }
```

Happily, they match with `bitcoin-cli`, where this descriptor originated:

```
bitcoin-cli getnewaddress

| tb1q7aecrwahqkewhhupt8ru5pumtnla65lhc9yuj2

bitcoin-cli getnewaddress

| tb1qg6pqcgltff4edp79hhzxc928l28lj7d2vqtxl4
```

You don't actually _need_ a network for `bdk-cli` to work. From here,
you could generate addresses, create transactions, and work with
PSBTs.

But if you want to know about transactions, balances, and UTXOs, then
you need to sync `bdk-cli` with a server, either an electrum or
Bitcoin server. This will take a long time, as it'll need to pull down
the blockchain (as normal).

```
bdk-cli wallet --wallet bitcoincore sync

| Chain tip advanced from height 0 to 314500
| {
|   "message": "Wallet synced successfully."
| }
```

Afterward you can start working with commands like `wallet balance`
and `wallet transactions` that work with the current state of the
blockchain, but that goes beyond the scope of this quick intro. For
now, we just want to see how BDK works with miniscript!

## Compile Policy with BDK

For our current purposes, the really exciting part of BDK is its
Policy compiler, because it's the only command-line Miniscript Policy
that we know about.

You access it with the `compile` command (which was one of
`--features` that you enabled when you installed `bdk-cli`). Not only
will it compile from Policy to Miniscript, but it'll also embed your
Miniscript as a Minidescriptor.

Here's an example of compiling the executive policy from previous
sections.

```sh
bdk-cli compile "or(pk(president),thresh(2,pk(vp1),pk(vp2),pk(vp3)))"

| {
|   "descriptor": "wsh(t:or_c(pk(president),v:multi(2,vp1,vp2,vp3)))#jvnnrg8k"
| }
```

It's interesting to note that `bdk-cli` chose a different compilation
that `sipa.be`, which instead picked
`or_d(pk(president),multi(2,vp1,vp2,vp3))`, but as we said Policy is
not fully specified: it's the encoding from Miniscript to Bitcoin
Script that is meant to be full analyzable and consistent.

Beyond that, we see that `bdk-cli` indeed added on both the `wsh()`
and the checksum. This is a descriptor that is fully ready to import
into `bitcoin-cli`, `bdk-cli`, or the other wallet of your choice.

Or it would be if you'd use the keys, but that's easy to do too, using
the `seedtool` and `keytool` apps from [chapter
10](10_0_Working_with_Secrets.md).:

```
SEEDPP=$(seedtool)
KEYPP=$(keytool --seed $SEEDPP --account-derivation-path m/84h/1h/0h/0h --network testnet account-key-base58)

SEEDVP1P=$(seedtool)
KEYVP1P=$(keytool --seed $SEEDVP1P --account-derivation-path m/84h/1h/0h/0h --network testnet account-key-base58)

SEEDVP2P=$(seedtool)
KEYVP2P=$(keytool --seed $SEEDVP2P --account-derivation-path m/84h/1h/0h/0h --network testnet account-key-base58)

SEEDVP3P=$(seedtool)
KEYVP3P=$(keytool --seed $SEEDVP3P --account-derivation-path m/84h/1h/0h/0h --network testnet account-key-base58)

EXEC_PRIVATE=$(echo "or(pk($KEYPP/*),thresh(2,pk($KEYVP1P/*),pk($KEYVP2P/*),pk($KEYVP3P/*)))")
EXEC_PRIVATE_DESC=$(bdk-cli compile $EXEC_PRIVATE | jq -r '.descriptor')
echo $EXEC_PRIVATE_DESC

| wsh(t:or_c(pk(tprv8hmuNrAoEFnCr2MaRNjX94GjAxDiKxG5FVhPjJE6jqbEBCY5o3FEvvC3hRqjEHbAR1CTSUhcHCHTmDsCqo27YpyAMVokseWzzjSBan2zYrN/*),v:multi(2,tprv8inKZ567XARkdHGUzvyH741df7ZsVTTs2cg6yCgMJPxxhN7GKgpb2LYbg4h9fGGnnu5BXZMW2EeD89g5kuAMZXK9GCizPySjv7jefHaYsNp/*,tprv8iPGcCodCT5SrQoq1uXNQUV1iBjpgEXH6eQgV5EavAsUTw5wVa8eaCMgNpmQECX6C6jTSjpJs3tQZyZXBEhJ9FxmK4t5jVwLdheaKPzn6yA/*,tprv8hZzKjjEA4e9tCW2XVsUud17Ftbn71NBjVCW275K3gvKVwxbNDrPvoEuDFiScjtAEr4TLGiEn2L3tJixuLxdEWbNppLPNnbuhECK22BpHGr/*)))#2lzcceyf
```

## Import a BDK Miniscript

This descriptor can't currently be imported into `bitcoin-cli`
(because it includes private keys), but it'll work fine in `bdk-cli`,
which has put more focus into Miniscript:

```sh
bdk-cli \
    --network signet \
    wallet --wallet execs config \
    --ext-descriptor "$EXEC_PRIVATE_DESC" \
    --url "ssl://mempool.space:60602" \
    --client-type electrum \
    --database-type sqlite
```

You'll immediately be able to see it in your list of wallets:

```sh
bdk-cli wallets

| {
|   "bitcoincore": {
|     "wallet": "bitcoincore",
|     "network": "signet",
|     "ext_descriptor": "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/0/*)",
|     "int_descriptor": "wpkh(tprv8ZgxMBicQKsPfCYFA9DANn81pjg3GPfAzLEdtywSxJAL3pGz41CgVZ8ChvdULoetthwZmd7nYvghRBaY2wDvg1WoStSumpGcdqRW3ZdpY1b/84h/1h/0h/1/*)",
|     "database_type": "sqlite",
|     "client_type": "electrum",
|     "server_url": "ssl://mempool.space:60602",
|     "batch_size": 10,
|     "proxy": null,
|     "proxy_auth": null,
|     "proxy_retries": 5,
|     "proxy_timeout": null
|   },
|   "execs": {
|     "wallet": "execs",
|     "network": "signet",
|     "ext_descriptor": "wsh(t:or_c(pk(tprv8hmuNrAoEFnCr2MaRNjX94GjAxDiKxG5FVhPjJE6jqbEBCY5o3FEvvC3hRqjEHbAR1CTSUhcHCHTmDsCqo27YpyAMVokseWzzjSBan2zYrN/*),v:multi(2,tprv8inKZ567XARkdHGUzvyH741df7ZsVTTs2cg6yCgMJPxxhN7GKgpb2LYbg4h9fGGnnu5BXZMW2EeD89g5kuAMZXK9GCizPySjv7jefHaYsNp/*,tprv8iPGcCodCT5SrQoq1uXNQUV1iBjpgEXH6eQgV5EavAsUTw5wVa8eaCMgNpmQECX6C6jTSjpJs3tQZyZXBEhJ9FxmK4t5jVwLdheaKPzn6yA/*,tprv8hZzKjjEA4e9tCW2XVsUud17Ftbn71NBjVCW275K3gvKVwxbNDrPvoEuDFiScjtAEr4TLGiEn2L3tJixuLxdEWbNppLPNnbuhECK22BpHGr/*)))#2lzcceyf",
|     "int_descriptor": null,
|     "database_type": "sqlite",
|     "client_type": "electrum",
|     "server_url": "ssl://mempool.space:60602",
|     "batch_size": 10,
|     "proxy": null,
|     "proxy_auth": null,
|     "proxy_retries": 5,
|     "proxy_timeout": null
|   }
| }
```

And you can generate addresses from it:


```sh
bdk-cli wallet --wallet execs new_address

| {
|   "address": "tb1qdsap579txagtk2mj66l4ccj73xn4hkfpf9k4ymlc26q30tx8p3wsr8ckzl",
|   "index": 0
| }
```

## Summary: Using BDK with Miniscript

BDK is a powerful set of Rust libraries for Bitcoin that has its own
CLI, `bdk-cli`. It can be used as an alternative to `bitcoin-cli`, but
for our current work on Miniscript it has two exciting features: it
will compile Policy to Miniscript from the command line, and it will
import Miniscript containing private keys.

## What's Next?

Move on to "Using Taproot" with [Chapter Seventeen: Introducing
Tapscript](17_0_Introducing_Taproot.md).
