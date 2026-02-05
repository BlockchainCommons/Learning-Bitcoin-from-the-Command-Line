TODO:

1. Revise Section: Understanding the Descriptor (3.5)
   * New Content: Descriptor Wallets
   * New Content: Complex Descriptors
   * Consider: Breaking into Two Sections

Legacy Wallets vs Descriptor Wallets

Legacy wallets are wallet files in the BDB file format, created by old versions of Bitcoin Core (and can still be opened by new versions, for now). They're contrasted with descriptor wallets, which use the SQLite file format, which are created by newer versions of Bitcoin Core.

# 3.4: Understanding the Descriptor

You've got your wallet set up, but before we go further we're going to take a moment to really understand it.

## Know about HD Wallets

Private keys are what make the Bitcoin world go round. They're used to generate public keys, which are the foundation of addresses, and they're also used to control those addresses. One private key creates one public key which creates one address. Once upon a time, the Bitcoin Core wallet managed this by holding on to a "bag of keys". But a bag of keys can be big, inefficient, and prone to loss. That's where the HD wallet came in.

The HD wallet, which is short for the Hierarchical Deterministic Wallet was defined in [BIP-32](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki). It's a hierarchical design where a single "seed" generates an extended private key, which includes not just the private key but also a "chain code" that can be used to create descendents. That in turn can be used to determinstically generate chains of keys (and therefore addresses) for a variety of purposes. A Bitcoin HD wallet will typically have individual chains of keys and addresses for a variety of different address types. But, they can all be restored from that seed (or from that master extended private key) because of their determinism: the addresses are always created in the same way provided that you have the same starting points (your master extended private key and a specific index [0,1,...,n] for a specific type of address).

> :book: ***What is a BIP?*** A BIP is a Bitcoin Improvement Proposal. It's an in-depth suggestion for a change to the Bitcoin Core code. Often, when a BIP has been sufficiently discussed and updated, it will become an actual part of the Bitcoin Core code. BIP-32 is one of many examples.

## Know about Descriptor Wallets

Most of this course presumes that you're working entirely from a single node where you manage your own wallet, sending and receiving payments with the addresses created by that wallet. However, that's not necessarily how the larger Bitcoin ecosystem works. There, you're more likely to be moving addresses between wallets and even setting up wallets to watch over funds controlled by different wallets. 

HD wallets offered a big step forward with managing this sort of interoperability, because now you could load a single seed (or master extended private key) into a new wallet rather than having to move over a whole bag of keys. Seed phrases and `xpub` and `xprv` formats were introduced to define these master secrets. But they quickly proved inadequate. The `xprv` (and `xpub`) defined the root of an HD tree, but they were very specific to a certain type of address. When a new address type was created, a new format was needed, `yprv` and `ypub`. Then a third address type resulted in the creation of `zprv` and `zpub`. The system was quickly growing unmanageable. A new system was needed that described not just the extended keys, but also which address types they would be used to create. Because if you didn't have that, a new wallet would have to test a master extended public key against _every possible type of address_ and that was either going to be very inefficient and time-consuming, or else it was going to introduce the likelihood of losing funds.

> :book: **What is a seed phase?** A collection of words that define a seed. The seed is in turn used to generate an extended private keys. Seed phrases are not currently used by Bitcoin Core, but they are in wide use in the larger Bitcoin ecosystem.
 
> :book: ***What is xprv?*** An extended private key. This is the combination of a private key and a chain code. It's a private key that a whole sequence of children private keys can be derived from.

> :book: ***What is xpub?*** An extended public key. This is the combination of a public key and a chain code. It's a public key that a whole sequence of children public keys can be derived from.

Enter, at last, the descriptor wallet. A descriptor wallet collects together "output descriptors" (sometimes called "wallet descriptors") which each define one or more Bitcoin addresses. They do so by organizing a standardized way to put together: a function (which defines how to unlock the Bitcoins), a derivation path (which defines the specific standard that the address adheres to and then also provides space for there to be many addresses following that standard), the master extended key, and a checksum to make sure that nothing has been corrupted.

> :book: ***What is a Derivation Path?*** When you have hierarchical keys, you need to be able to define individual keys as descendents of a seed. For example `[0]` is the 0th key, `[0/1]` is the first son of the 0th key, `[1/0/1]` is the first grandson of the zeroth son of the 1st key. Some keys also contain a `'` or `h` after the number, to show they're hardened, which protects them from a specific attack that could otherwise be used to derive an `xprv` from an `xpub`. You don't need to worry about the specifics, other than the fact that a derivation path like `[1/0/1/0/0]` depicts a hierarchy tree and that descriptor wallets run specific calculations to deterministically determine the right address for a specific position in a tree.

> :information_source: **NOTE:** a derivation path defines a key, which means that a key represents a derivation path. They're equivalent. In the case of a descriptor, the derivation path lets `bitcoind` know where the key that follows in the descriptor came from!

The derivation path allows you to calculate the right key from the master extended keys, but it's the introduction of functions into descriptors that makes them particularly powerful, because it allows them to serve a number of different types of past, present, and future address (which we'll meet in the next chapter).


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
