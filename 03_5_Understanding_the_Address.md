# 3.5: Understanding the Address

You understand your wallet now, but there's still the question of the addresses that it generates. Because, as it turns out, there are many sorts of addresses out there, some totally deprecated, some fairly bleeding edge, and some that are the default.

## Understand Addresses 

As you know, each Bitcoin address is a storage area for Bitcoin funds. Funds are sent from one address to another address as a _transaction_; the transaction matches the address that it was sent from. But an address is more than just a storage area: it's a lockbox. The specifics of the address define how that lockbox is actually opened up. The default is to use a private key associated with the public key that was used to create the address, but that's not the only way that Bitcoin addresses work.

Different types of Bitcoin addresses change all of this. In general they may change:

* How the Bitcoin address is derived from the public key.
* How the Bitcoin address is encoded.
* How the address is unlocked.
* Whether additional scripts are necessary for unlocking.
* How transactions sent from the address must be formed.

### The Segwit Address(es)

The most common sort of address actively used in the modern Bitcoin world is called by two different names that both mean the same thing: the Bech32 address (which names its encoding method, and technically includes other address types that are also encoded that way) or the SegWit address (which stands for "Segregated Witness" and describes how transactions sent to it must be formed). All Bitcoin addresses also have a "Pay-to" name, and Segwit addresses are called P2WPKH, which stands for "Pay to Witness Public Key Hash," which is a mouthful that will make more sense when you've met the older address types.

Segwit came out of the Blocksize War of the '10s, where Bitcoin developers and users were trying to figure out a way to speed up the Bitcoin blockchain by allowing more transactions to be sent in less time. The result was the segwit soft fork. It solved the problem by moving the signatures (which are the key you use to unlock most Bitcoin transactions) from the middle of the transaction to the end, and then making that end space into increased space to store transactions, that could also be ignored by older servers. This allowed for an increase in the space to store transactions that was phased in gradually.

The specifics aren't that important. What is important is that Segwit addresses (or Bech32 addresses or P2WPKH address) are the default type of address; when you created an address in [§3.3](03_3_Setting_Up_Your_Wallet.md), this is what you created.

> :book: **What is a fork?** A fork is a change to the Bitcoin protocol. This isn't just a change to how one application (like Bitcoin Core) works, it's a change to how all of the members of the Bitcoin network agree to interact with each other. A hard fork makes new transactions incompatible with old transactions. It essentially two versions of Bitcoin (and most often has been used to actually create a new cryptocurrency). A soft fork ensures that old transactions remain compatible.

### The Rest of the Addresses

Here's a comprehensive listing of the address types, with the ❌ addreses largely deprecated (meaning don't worry about them!) and the ✅ addresses being the current ones (meaning that they will be relevant to your use of Bitcoin).

Don't be overwhelmed by this list! If it looks like too much, just scan the ✅ addresses, which will be the only one used in this course, and come back and look at some of the other varieties if they come up and you want to figure out what they do exactly.

The addresseses commonly in use for Bitcoin today are:

* **✅ P2WPKH (Pay to Witness Public Hash).** P2WPKH is a SegWit address that can be unlocked with a single key. This is the default address type for Bitcoin core, and what you'll mostly be using. P2WPKH deprecates P2PKH.
* **✅ P2WSH (Pay to Witness Script Hash).** P2WSH is a SegWit address that can be unlocked a script (rather than one or more private keys doing so). Scripts are described in [Chapter 11](11_0_Introducing_Bitcoin_Scripts.md) (and afterward). P2WSH deprecates P2SH
* **✅ P2TR.** Taproot is the newest Bitcoin address type, but it was soft forked back in 2021, so it should have wide acceptance at this point. It's considered the next iteration of SegWit. It can produce smaller transactions for multisigs, has privacy advantages, and can also combine singular signatures with scripts. It hasn't been widely adopted yet.

The addresses that have been deprecated are:

* **❌ P2PK (Pay to Public Key).** This address type was used in the earliest days of Bitcoin and quickly deprecated. Revealing your public key can actually be dangerous since quantum computing or other future processor improvements might be able to brute force a private key given a public key. That's why modern addresses are based on a hash of your public key, instead of the public key itself, and why P2PK was quickly phased out.
* **❌ P2MS (Pay to Multisig).** Most Bitcoin transactions are signed by a single key, which allows a single person to decide to send the Bitcoin. Multisigs instead allow multiple people to do so. P2MS was the initial attempt to support this, but it exposed public keys (like P2PK). It was quickly replaced by P2SH scripts, and more recently by P2WSH and P2TR.
* **❌ P2PKH (Pay to Public Key Hash).** The P2PKH address was the predominent address type prior to the introduction of SegWit, which replaced it with P2WKH. Most new addresses are now created with P2WPKH, since their transactions are cheaper, but many older P2PKH addresses still hold funds, since it was in use for roughly a decade.
* **❌ P2SH (Pay to Script Hash).** P2SH was the first type of scripting address, before signatures got reached for SegWit, resulting in P2WSH.
* **❌ P2SH-P2WPKH and ❌P2SH-P2WSH.** When SegWit was being deployed, people wanted to send to it even before their wallets had been upgraded. These two "Nested Segwit" or "Wrapped Address" types offered the opportunity to do so by using a classic P2SH (scripting) address to incorporate the SegWit mechanics. These are actually just P2SH addresses with specific scripts, not a proper address type, and they're no longer needed since the SegWit upgrade is long past.

[Unchained Capital](https://www.unchained.com/blog/bitcoin-address-types-compared) has conducted a survey of address types that they most recently updated in 2025. Besides giving more details on all of address types, it also listed how much of the Bitcoin supply was held in each address type. Though some deprecated addresses (mainly P2PKH) still hold large amounts of Bitcoin, that's a historic artifact. Some funds may be forever lost (due to lost keys) while others may be being held for the long term.
  
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

Each different type of address is formed in a different way. An address can be derived in a different way, encoded in a different way, or formatted in a different way. As a result, addresses have different lengths and look different. Part of that formatting tends to be a prefix, which helps to identify an address type at a glance. Sometimes that prefix is different for mainnet and for the various testing networks (signet, testnet, regtest) so that you don't confuse fake funds and real funds.

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
