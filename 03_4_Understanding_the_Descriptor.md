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

> :warning: **VERSION WARNING:** Modern Bitcoin wallets use descriptor wallets stores in SQLite. Older, "classic" wallets were instead bags of keys, stored in BDB (Berkeley Database) format. The classic files can currently still be opened by `bitcoin-cli` but you wouldn't want to create something new in that format.
> 
## Examine Your Wallet's Descriptors

You can look at all of the descriptors contained in your wallet with `bitcoin-cli listdescriptors`:
```
$ bitcoin-cli listdescriptors
{
  "wallet_name": "",
  "descriptors": [
    {
      "desc": "pkh([e18dae20/44h/1h/0h]tpubDCsocyjrtJLXKJ3atFwKf6FiPheuVNV27B1swsGiNvk4cuPhVTsCDvusSNcH8thnS68FPFotgHqo9FHNGrvhtx6ZqdbWBQTWrCgK9xous17/0/*)#5q6np9at",
      "timestamp": 1770329126,
      "active": true,
      "internal": false,
      "range": [
        0,
        999
      ],
      "next": 0,
      "next_index": 0
    },
    {
      "desc": "pkh([e18dae20/44h/1h/0h]tpubDCsocyjrtJLXKJ3atFwKf6FiPheuVNV27B1swsGiNvk4cuPhVTsCDvusSNcH8thnS68FPFotgHqo9FHNGrvhtx6ZqdbWBQTWrCgK9xous17/1/*)#95ljusdn",
      "timestamp": 1770329126,
      "active": true,
      "internal": true,
      "range": [
        0,
        999
      ],
      "next": 0,
      "next_index": 0
    },
    {
      "desc": "sh(wpkh([e18dae20/49h/1h/0h]tpubDCmuiGVie2yYNXiwi6ymC2qamb7RdEXKeo8HZtkLAtaCcLHFGvHBY4W3Fx9rTjxYdxkb1kvkLp86oDvdxmYjvrVXpcVG2XptmYu8rYsAirP/0/*))#jdj8slnk",
      "timestamp": 1770329126,
      "active": true,
      "internal": false,
      "range": [
        0,
        999
      ],
      "next": 0,
      "next_index": 0
    },
    {
      "desc": "sh(wpkh([e18dae20/49h/1h/0h]tpubDCmuiGVie2yYNXiwi6ymC2qamb7RdEXKeo8HZtkLAtaCcLHFGvHBY4W3Fx9rTjxYdxkb1kvkLp86oDvdxmYjvrVXpcVG2XptmYu8rYsAirP/1/*))#8vu3gqxf",
      "timestamp": 1770329127,
      "active": true,
      "internal": true,
      "range": [
        0,
        999
      ],
      "next": 0,
      "next_index": 0
    },
    {
      "desc": "tr([e18dae20/86h/1h/0h]tpubDC5ZrtRv5CURANE3oWS1cK7mE9t93KnZfUApDs3dg8xxDuGMsuSejCLeh4yKeqkvHknRboWEctS2BSGQxXhsCx1dgPCTCwjF4a9mgq999Af/0/*)#h4xmum5n",
      "timestamp": 1770329126,
      "active": true,
      "internal": false,
      "range": [
        0,
        999
      ],
      "next": 0,
      "next_index": 0
    },
    {
      "desc": "tr([e18dae20/86h/1h/0h]tpubDC5ZrtRv5CURANE3oWS1cK7mE9t93KnZfUApDs3dg8xxDuGMsuSejCLeh4yKeqkvHknRboWEctS2BSGQxXhsCx1dgPCTCwjF4a9mgq999Af/1/*)#xpr6pwyt",
      "timestamp": 1770329127,
      "active": true,
      "internal": true,
      "range": [
        0,
        999
      ],
      "next": 0,
      "next_index": 0
    },
    {
      "desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
      "timestamp": 1770329126,
      "active": true,
      "internal": false,
      "range": [
        0,
        1001
      ],
      "next": 3,
      "next_index": 3
    },
    {
      "desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/1/*)#qw3x5jqt",
      "timestamp": 1770329127,
      "active": true,
      "internal": true,
      "range": [
        0,
        999
      ],
      "next": 0,
      "next_index": 0
    }
  ]
}
```
Wow, that's a lot! But it's really just a listing of eight descriptors (`desc`) with a bunch of additional information on each. As it happens, that's descriptors for four different types of addresses (which we'll meet in chapter 4) with both an external address (for sending to other people) and an internal address (for sending change back yourself). (And we'll talk about change in chapter 4 too!)

With that understood, we can look more closely at one of them:
```
{
  "desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
  "timestamp": 1770329126,
  "active": true,
  "internal": false,
  "range": [
    0,
    1001
  ],
  "next": 3,
  "next_index": 3
},
```
This contains:
* **`desc`:** The descriptor.
* **`timestamp`:** When the descriptor was created.
* **`active`:** Is the descriptor still in use for creating new addresses. (It could have been superseded by a new master extended key, for example when encrypting the wallet.
* **`internal`:** Is this a descriptor for internal addresses (for change).
* **`range`:** For ranged descriptors, what's the range?
* **`next`, `next_index`:** What is the next address to create for this descriptor. In this example, the next one `3` because we already created three addresses from this descriptor (`0`, `1`, and `2`) in [§3.3](3_3_Setting_Up_Your_Wallet.md).

As for the descriptor itself, let's break that down further:

* **Function: `wpkh`.** The function that is used to create an address from that key. In this cases it's `wpkh`. That standards for "Witness Public Key Hash," which is one of the methods used to unlock a Bech32 address.
* **Fingerprint: `e18dae20`.** This is a fingerprint of the master extended public key. It tells you which secret is used to generate this address. It is *not* necessary to generate the keys and address for this derivation, it's just helpful for you to go back and find the secret that generated your extended keys.
* **Derivation Path: `/84h/1h/0h`.** This describes what part of an HD wallet is being exported. This is the 0th child key of the 1st child of the 84th child in the HD tree. These various levels have very specific meanings: `/purpose'/ coin_type'/ account'/`. The purpose of this derivation path is "84", which means that it follows [BIP-84](https://github.com/bitcoin/bips/blob/master/bip-0084.mediawiki), which describes WPKH derivation. The coin type is "1", which means testnet or signet coin. (A mainnet could would be "0") The account is "0", as it's the only account in our wallet.
* **Key: `tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu`.** This is the signet or testnet extended master public key that was used to generate this derived key. (A private key could be here instead. With just a public key in place, it demonstrates how to watch this series of addresses, and with a private key in place, it show show to control them)
* **Range: `/0/*`.** These are actually the final two parts of the derivation path, which are defined as `change / address_index`. The "0" says it's an external address. (An internal or change address would be "1".) The `*` says it's a ranged address, which means that it's defining a whole set of WPKH addresses that could be created.
* **`#3658f8sn"`.** As noted, this is a checksum showing the descriptor isn't corrupted.

So that's what everything means in a descriptor. Though they might seem somewhat complex, keep in mind that they take the place of a potentially infinite number of addresses. With this one descriptor, or these eight descriptors as the case might be, you can regenerate every key and addresse that you might have used for these four address types. That's a huge boon for backups (when you want to protect your funds) and for moving control of your funds from one machine to another.

Note that you can also run `bitcoin-cli listdescriptors true` if you want your descriptor list to include the private keys instead of the public keys.

## Examine an Address' Descriptor

You can look at the descriptor for an individual address with `bitcoin-cli getaddressinfo`:
```
$ bitcoin-cli getaddressinfo tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4
{
  "address": "tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4",
  "scriptPubKey": "00142a4f27c78470206e1a5948b47478d5b37991aac4",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([e18dae20/84h/1h/0h/0/2]02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba)#dqt0983r",
  "parent_desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "2a4f27c78470206e1a5948b47478d5b37991aac4",
  "pubkey": "02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba",
  "ischange": false,
  "timestamp": 1770329126,
  "hdkeypath": "m/84h/1h/0h/0/2",
  "hdseedid": "0000000000000000000000000000000000000000",
  "hdmasterfingerprint": "e18dae20",
  "labels": [
    ""
  ]
}
```
That reveals:
```
  "desc": "wpkh([e18dae20/84h/1h/0h/0/2]02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba)#dqt0983r",
```
Which you can compare to the ranged descriptor we just looked at:
```
  "desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
```
They're in slightly different formats as the non-ranged address has the derivation path all together. But other than that, there are just two changes:
* The wallet has a ranged of addresses `0/*`, while the address is one specific index in that range `0/2`.
* The checksums are different, as you'd expect due to the differences in the index number.

That's the only difference between a descriptor in the wallet and a descriptor for a specific address!

You will see descriptors throughout Bitcoin commands! They're a vital element of not just the wallet, but of each address that is used to transfer funds.

## Examine Descriptors Again

Descriptors can be looked at via another command, `bitcoin-cli getdescriptorinfo`:
```
$ bitcoin-cli getdescriptorinfo "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn"
{
  "descriptor": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
  "checksum": "3658f8sn",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": false
}
```
This is a pretty sparse description that includes the descriptor and confirms that it's a ranged descriptor (`isrange`), that our wallet has the private key (`issolvable`) and that the private key isn't included in the descriptor and that there's instead a public key (`hasprivatekeys`). Beyond that information, `getdescriptorinfo` services two other purposes:

1. If you don't have the checksum (which is required for other descriptor-related `bitcoin-cli` commands), you can enter the descriptor without it, and it'll be calculated for you.

```
$ bitcoin-cli getdescriptorinfo "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)"
{
  "descriptor": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
  "checksum": "3658f8sn",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": false
}
```

2. If your descriptor is invalid, `getdescriptorinfo` will tell you:
```
$ bitcoin-cli getdescriptorinfo "cow([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)"
error code: -5
error message:
'cow([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)' is not a valid descriptor function
```

---

## Derive Addresses by Hand

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
