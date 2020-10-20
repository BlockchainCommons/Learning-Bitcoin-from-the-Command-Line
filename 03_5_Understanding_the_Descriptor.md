# 3.5: Understanding the Descriptor

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

You may have noticed a weird `desc:` field in the `listunspent` command of the previous section. Here's what's all about (and how it can be used to transfer addresses).

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.17.0 that had continued to be expanded through Bitcoin Core 0.20.0. Most of the commands in this section are from 0.17.0, but the updated `importmulti` that support descriptors is from 0.18.0.

## Know about Transferring Addresses

Most of this course presumes that you're working entirely from a single node where you manage your own wallet, sending and receiving payments with the addresses created by that wallet. However, that's not necessarily how the larger Bitcoin ecosystem works. There, you're more likely to be moving addresses between wallets and even setting up wallets to watch over funds controlled by different wallets. 

That's where descriptors come in. They're most useful if you're interacting with software _other_ than Bitcoin Core, and really need to lean on this sort of compatibility function: see [§6.1](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/06_1_Sending_a_Transaction_to_a_Multisig.md) for a real-world example of how having the capability of descriptors is critical.

Moving addresses between wallets used to focus on `xpub` and `xprv`, and those are still supported. 

> :book: ***What is xprv?*** An extended private key. This is the combination of a private key and a chain code. It's a private key that a whole sequence of children private keys can be derived from.

> :book: ***What is xpub?*** An extended public key. This is the combination of a public key and a chain code. It's a public key that a whole sequence of children public keys can be derived from.

The fact that you can have a "whole sequence of children ... keys" reveals the fact that "xpub" and "xprv" aren't standard keys like we've been talking about so far. They're instead hierarchical keys that can be used to create whole families of keys, built on the idea of HD Wallets.

> :book: ***What is an HD Wallet?*** Most modern wallets are built on [BIP32: Hierarchical Deterministic Wallets](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki). This is a hierarchical design where a single seed can be used to generate a whole sequence of keys. The entire wallet may then be restored from that seed, rather than requiring the restoring of every single private key.

> :book: ***What is a Derivation Path?*** When you have hierarchical keys, you need to be able to define individual keys as descendents of a seed. For example `[0]` is the 0th key, `[0/1]` is the first son of the 0th key, `[1/0/1]` is the first grandson of the zeroth son of the 1st key. Some keys also contain a `'` after the number, to show they're hardened, which protects them from a specific attack that can be used to derive an `xprv` from an `xpub`. You don't need to worry about the specifics, other than the fact that those `'`s will cause you formatting troubles when working from the command line.

> :information_source: **NOTE:** a derivation path defines a key, which means that a key represents a derivation path. They're equivalent. In the case of a descriptor, the derivation path lets `bitcoind` know where the key that follows in the descriptor came from!

`xpubs` and `xprvs` proved insufficient when the types of public keys multiplied under the [SegWit expansion](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/04_6_Creating_a_Segwit_Transaction.md), thus the need for "output descriptors".

> :book: ***What is an output descriptor?*** A precise description of how to derive a Bitcoin address from a combination of a function and one or more inputs to that function.

The introduction of functions into descriptors is what makes them powerful, because they can be used to transfer all sorts of addresses, from the Legacy addresses that we're working with now to the Segwit and multisig addresses that we'll meet down the road. An individual function matches a particular type of address and correlates with specific rules to generate that address.

## Capture a Descriptor

Descriptors are visible in several commands such as `listunspent` and `getaddressinfo`:
```
$ bitcoin-cli getaddressinfo ms7ruzvL4atCu77n47dStMb3of6iScS8kZ
{
  "address": "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ",
  "scriptPubKey": "76a9147f437379bcc66c40745edc1891ea6b3830e1975d88ac",
  "ismine": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": false,
  "pubkey": "03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388",
  "iscompressed": true,
  "ischange": false,
  "timestamp": 1592335136,
  "hdkeypath": "m/0'/0'/18'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
    ""
  ]
}
```
Here the descriptor is `pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk`.

## Understand a Descriptor

A descriptor is broken into several parts:
```
function([derivation-path]key)#checksum
```
Here's what that all means:
* **Function.** The function that is used to create an address from that key. In this cases it's `pkh`, which is the standard P2PKH legacy address that you met in [§3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md). Similarly, a P2WSH SegWit address would use `wsh` and a P2WPKH address would use `wpkh`.
* **Derivation Path.** This describes what part of an HD wallet is being exported. In this case it's a seed with the fingerprint `d6043800` and then the 18th child of the 0th child of the 0th child (`0'/0'/18'`) of that seed. There may also be a further derivation after the key: `function([derivation-path]key/more-derivation)#checksum`
   * It's worth noting here that if you ever get a derivation path without a fingerprint, you can make it up. It's just that if there's an existing one, you should match it, because if you ever go back to the device that created the fingerprint, you'll need to have the same one.
* **Key**. The key or keys that are being transferred. This could be something traditional like an `xpub` or `xprv`, it could just be a public key for an address as in this case, it could be a set of addresses for a multi-signature, or it could be something else. This is the core data: the function explains what to do with it.
* **Checksum**. Descriptors are meant to be human transferrable. This checksum makes sure you got it right.

See [Bitcoin Core's Info on Descriptor Support](https://github.com/bitcoin/bitcoin/blob/master/doc/descriptors.md) for more information.

## Examine a Descriptor

You can look at a descriptor with the `getdescriptorinfo` RPC:
```
$ bitcoin-cli getdescriptorinfo "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk"
{
  "descriptor": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "checksum": "4ahsl9pk",
  "isrange": false,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Note that it returns a checksum. If you're ever given a descriptor without a checksum, you can learn it with this command:
```
$ bitcoin-cli getdescriptorinfo "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)"
{
  "descriptor": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "checksum": "4ahsl9pk",
  "isrange": false,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Besides giving you the checksum, this command also verifies the validity of the descriptor and provides useful information like whether a descriptor contains private keys.

One of the powers of a descriptor is being able to derive an address in a regular way. This is done with the `deriveaddresses` RPC.
```
$ bitcoin-cli deriveaddresses "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk"
[
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ"
]
```
You'll note it loops back to the address we started with (as it should).

## Import a Descriptor

But, the really important thing about a descriptor is that you can take it to another (remote) machine and import it. This is done with the `importmulti` RPC using the `desc` option:
```
remote$ bitcoin-cli importmulti '[{"desc": "pkh([d6043800/0'"'"'/0'"'"'/18'"'"']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk", "timestamp": "now", "watchonly": true}]'
[
  {
    "success": true
  }
]
```
First, you'll note our first really ugly use of quotes. Every `'` in the derivation path had to be replaced with `'"'"'`. Just expect to have to do that if you're manipulating a descriptor that contains a derivation path. (The other option is to exchange the `'` with a `h` for hardened, but that will change you checksum, so if you prefer that for its ease of use, you'll need to get a new checksum with `getdescriptorinfo`.)

Second, you'll note that we flagged this as `watchonly`. That's because we know that it's a public key, so we can't spend with it. If we'd failed to enter this flag, `importmulti` would helpfully have told us something like: `Some private keys are missing, outputs will be considered watchonly. If this is intentional, specify the watchonly flag.`.

> :book: ***What is a watch-only address?*** A watch-only address allows you to watch for transactions related to an address (or to a whole family of addresses if you used an `xpub`), but not to spend funds on those addresses.

Using `getaddressesbylabel`, we can now see that our address has correctly been imported into our remote machine!
```
remote$ bitcoin-cli getaddressesbylabel ""
{
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ": {
    "purpose": "receive"
  }
}
```
## Summary: Understanding the Descriptor

Descriptors let you pass public keys and private keys among wallets, but more than that, they allow you to precisely and correctly to define addresses and to derive addresses of a lot of different sorts from a standardized description format.

> :fire: ***What is the power of descriptors?*** Descriptors allow you to import and export seeds and keys. That's great if you want to move between different wallets. As a developer, they also allow you to build up the precise sort of addresses that you're interested in creating. For example, we use it in [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2/blob/master/Docs/How-it-works.md) to generate a multi-sig from three seeds. 

We'll make real use of descriptors in [§7.3](07_3_Integrating_with_Hardware_Wallets.md), when we're importing addresses from a hardware wallet.

## What's Next?

Advance through "bitcoin-cli" with [Chapter Four: Sending Bitcoin Transactions](04_0_Sending_Bitcoin_Transactions.md).
