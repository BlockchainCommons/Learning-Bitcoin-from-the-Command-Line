# 3.5: Understanding the Address

You understand your wallet now, but there's still the question of the addresses that it generates. Because, as it turns out, there are many sorts of addresses out there, some totally deprecated, some fairly bleeding edge, and some that are the default.

## Understand Addresses 

As you know, each Bitcoin address is a storage area for Bitcoin funds. Funds are sent from one address to another address as a _transaction_. But an address is more than just a storage area: it's a lockbox. The specifics of the address define how that lockbox is actually opened up. The default is to use a private key associated with the public key that was used to create the address, but that's not the only way that Bitcoin addresses work.

Different types of Bitcoin addresses change all of this. In general they may change:

* How the Bitcoin address is derived from the public key.
* How the Bitcoin address is encoded.
* How the address is unlocked.
* Whether additional scripts are necessary for unlocking.
* How transactions sent from the address must be formed.

### The Segwit Address(es)

All Bitcoin addresses have a "Pay-to" name, and Segwit addresses are called P2WPKH, which stands for "Pay to Witness Public Key Hash," which is a mouthful that will make more sense when you've met the older address types. It's also called a Bech32 address (to describe its encoding method) or a SegWit address (to describe how its signature is stored), but there are other types of addresses that are also Bech32-encoded or SegWit.

SegWit came out of the Blocksize War of the '10s, where Bitcoin developers and users were trying to figure out a way to speed up the Bitcoin blockchain by allowing more transactions to be sent in less time. The result was the SegWit soft fork. It solved the problem by moving the signatures (which are the key that you use to unlock most Bitcoin transactions) from the middle of the transaction to the end, and then making that end space into increased space to store transactions, but space that could also be ignored by older servers. This allowed for a "blocksize increase" that was phased in gradually. The original version of SegWit used by P2WPKH is now called SegWit v0, with SegWit standing for "Segregated Witness", which describes that separation between the main part of the transaction and the signature.

The specifics aren't that important. What is important is that P2WPKH, and a few other SegWit addresses that we'll meet shortly, are the default type of address; when you created an address in [§3.3](03_3_Setting_Up_Your_Wallet.md), you created a P2WPKH.

> :book: **What is a fork?** A fork is a change to the Bitcoin protocol. This isn't a change to how one application (like Bitcoin Core) works, it's a change to how all of the members of the Bitcoin network agree to interact with each other. A hard fork makes new transactions incompatible with old transactions. It essentially creates two versions of Bitcoin (and most often has been used to actually create a new cryptocurrency). A soft fork ensures that old transactions remain compatible.

### The Rest of the Addresses

Here's a comprehensive listing of all the address types that have existed in Bitcoin's history, with the ❌ addreses largely deprecated (meaning don't worry about them!) and the ✅ addresses being the current ones (meaning that they will be relevant to your use of Bitcoin). Don't be overwhelmed by this list! If it looks like too much, just scan the ✅ addresses, which will be the only one used in this course, and come back and look at some of the other varieties if they come up and you want to figure out what they do exactly.

The addresseses commonly in use for new Bitcoin addresses today are:

* **✅ P2WPKH (Pay to Witness Public Hash).** P2WPKH is a SegWit address that can be unlocked with a single key. This is the default address type for Bitcoin Core, and what you'll mostly be using. P2WPKH deprecates P2PKH.
* **✅ P2WSH (Pay to Witness Script Hash).** P2WSH is a SegWit address that can be unlocked by a script (rather than one or more private keys doing so). Scripts are described in [Chapter 11](11_0_Introducing_Bitcoin_Scripts.md) (and afterward). P2WSH deprecates P2SH.
* **✅ P2TR.** Taproot is the newest Bitcoin address type, but it was soft forked back in 2021, so it should have wide acceptance at this point. It's considered to be SegWit v1. Due to its use of Schnorr signatures, it can produce smaller transactions for multisigs, has privacy advantages, and can also combine singular signatures with scripts. It hasn't been widely adopted yet. We'll examing Taproot transcations starting in [Chapter 9](09_0_Expanding_Bitcoin_Transactions_with_Schnorr.md).

The following two address types are not in wide use, but still hold large amounts of old funds:

* **❌ P2PKH (Pay to Public Key Hash).** The P2PKH address was the predominent address type prior to the introduction of SegWit, which replaced it with P2WKH. Most new addresses are now created with P2WPKH, since their transactions are cheaper, but many older P2PKH addresses still hold funds, since it was in use for roughly a decade.
* **❌ P2SH (Pay to Script Hash).** P2SH was the first type of scripting address, before signatures got revamped for SegWit, resulting in P2WSH.

These last four address types are more totally deprecated:

* **❌ P2PK (Pay to Public Key).** This address type was used in the earliest days of Bitcoin and soon phased out. It pays directly to a public key, rather than the hash of a public key. Revealing your public key can actually be dangerous since quantum computing or other future processor improvements might be able to brute force a private key given a public key. That's why modern addresses moved to those hashes, which can't be reversed.
* **❌ P2MS (Pay to Multisig).** Most Bitcoin transactions are signed by a single key, which allows a single person to decide to send the Bitcoin. Multisigs instead allow multiple people to do so. P2MS was the initial attempt to support this, but it exposed public keys (like P2PK did) and only allowed a maximum of three signatures. It was quickly replaced by P2SH scripts, and more recently by P2WSH and P2TR. 
* **❌ P2SH-P2WPKH and ❌P2SH-P2WSH.** When SegWit was being deployed, people wanted to send to it even before their wallets had been upgraded. These two "Nested Segwit" or "Wrapped Address" types offered the opportunity to do so by using a classic P2SH (scripting) address to incorporate the SegWit mechanics. These are actually just P2SH addresses with specific scripts, not a proper address type, and they're no longer needed since the SegWit upgrade is long past.

[Unchained Capital](https://www.unchained.com/blog/bitcoin-address-types-compared) conducted a survey of address types back in 2023. Besides giving more details on all of the address types, it also lists how much of the Bitcoin supply is held in each address type. Though some deprecated addresses (particularly P2PKH and P2SH) still hold large amounts of Bitcoin, that's a historic artifact. Some of those funds may be forever lost (due to lost keys) while others may be being held for the long term.
  
| Type | Description | Fund % |
|------|-------------|--------|
| P2PK | Pay to Public Key | 9% |
| P2MS | Pay to Multisig | 0%+ |
| P2PKH | Pay to Public Key Hash | 43% |
| P2SH | Pay to Script hash | 24% |
| P2WPKH | Pay to Witness Public Key Hash | 20% |
| P2WSH | Pay to Witness Script Hash | 4% |
| P2TR | Pay to Taproot | 0.1% |

## Understand Address Prefixes

Each different type of address is formed in a different way. An address can be derived in a different way, encoded in a different way, or formatted in a different way. As a result, addresses have different lengths and look different. Part of that formatting tends to be a prefix, which helps to identify an address type at a glance. The prefix is always different for mainnet and for the various testing networks (signet, testnet, regtest) so that you don't confuse fake funds and real funds.

The following chart lists out the prefixes and encoding methods for each address type:

| Type | Mainnet | Testnet | Encoding |
|------|-------------|--------|----|
| P2PK | N/A | N/A | public key |
| P2MS | N/A | N/A | public keys |
| P2PKH | 1... | m...<br>n...| base58 |
| P2SH<br>P2SH-P2WPKH<br>P2SH-P2WSH | 3... | 2... | base58 |
| P2WPKH | bc1q... | tb1q...| bech32 |
| P2WSH | bc1q... | tb1q...| bech32 |
| P2TR | bc1p... | tb1p...| bech32m |

## Create More Addresses

Though you'll stick with the default P2PWKH addresses most of the time, you may want to create other sorts of addresses. The `getnewaddress` RPC actually offers four types:

| Flag | Type | Notes |
|------|------|-------|
| bech32 | P2PKH | Default |
| bech32m | P2TR | |
| legacy | P2PK | Deprecated |
| p2sh-segwit | P2SH-PWPKH | Deprecated |

Missing from this is P2WSH, but that's because it has its own methods for creation, as are described later in the book.

The following commands use `getnewaddress` to generate an address with no label `""` for each of the four types:
```
$ bitcoin-cli getnewaddress "" bech32
tb1quqv4shd6ts0t6ew5l8kcqvvxgvr8drtr7ajew2

$ bitcoin-cli getnewaddress "" bech32m
tb1pps29f3evn5lscg82a2wceyl8p6s0wckjvp3zllw2myn6xg6xxvxqcekkxd

$ bitcoin-cli getnewaddress "" legacy
mueEcK86JTvMrge7qibsRdMes6ET5oAGgd

$ bitcoin-cli getnewaddress "" p2sh-segwit
2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq
```
Note that the address prefixes match each of the expected types, with `p2sh-segwit` revealing itself as actually being a P2SH transaction.

It seems likely that the `legacy` and `p2sh-segwit` flags will be sunset some time in the future, as they were added to support the transition to SegWit that began in 2017.


## Summary: Understanding the Address

Addresses come in many types, and there have been several over the history of Bitcoin. However, you really only need to know about three:

* **P2WPKH.** Pay to Witness Public Key Hash. A SegWit v0 Bech32 address. This is the standard address for receiving funds, and what you'll use through most of this course.
* **P2WSH.** Pay to Witness Script Hash. A SegWit v0 Bech32 address. This is an address for receiving more complex transactions that can be spent by meeting the conditions of a script.
* **P2TR.** Pay to Tap Root. A Segwit v1 Bech32m address. Besides using Schnorr signatures, this is a sort of hybrid script that allows both single signing and the use of a Merkelized Alternative Script Tree.

## What's Next?

Continue "Understanding Your Bitcoin Setup" with [§3.6: Creating QR Codes for Addresses](03_6_Creating_QR_Codes_for_Addresses.md).

