# 3.5: Understanding the Address

You understand your wallet now, but there's still the question of the addresses that it generates. Because, as it turns out, there are many sorts of addresses out there, some totally deprecated, some fairly bleeding edge, and some that are the default.

## Understand Addresses 

As you know, each Bitcoin address is a storage area for Bitcoin funds. Funds are sent from one address to another address as a _transaction_; the transaction matches the address that it was sent from. But an address is more than just a storage area: it's a lockbox. The specifics of the address define how that lockbox is actually opened up. The default is to use a private key associated with the public key that was used to create the address, but that's not the only way that Bitcoin addresses work.

Different types of Bitcoin addresses cause changes in all of this. In general they may change:

* How the Bitcoin address is derived from the public key.
* How the Bitcoin address is encoded.
* How the address is unlocked.
* Whether additional scripts are necessary for unlocking.
* How transactions sent from the address must be formed.

### The Segwit Address(es)

The most common sort of address actively used in the modern Bitcoin world is called by two different names that both mean the same thing: the Bech32 address (which names its encoding method) or the SegWit address (which stands for "Segregated Witness" and describes how transactions sent to it must be formed). All Bitcoin addresses also have a "Pay-to" name, and Segwit addresses are called P2WPKH, which stands for "Pay to Witness Public Key Hash," which is a mouthful that will make more sense when you've met the older address types.

Segwit came out of the Blocksize War of the '10s, where Bitcoin developers and users were trying to figure out a way to speed up the Bitcoin blockchain by allowing more transactions to be sent in less time. It solved the problem by moving the signatures (which are the key you use to unlock most Bitcoin transactions) from the middle of the transaction to the end, and then making that end space into increased space to store transactions, that could also be ignored by older servers. This allowed for an increase in the space to store transactions that was phased in gradually.

The specifics aren't that important. What is important is that Segwit addresses (or Bech32 addresses or P2WPKH address) are the default type of address; when you created an address in [§3.3](03_3_Setting_Up_Your_Wallet.md), this is what you created.

### The Rest of the Addresses

## Understand Address Prefixes
