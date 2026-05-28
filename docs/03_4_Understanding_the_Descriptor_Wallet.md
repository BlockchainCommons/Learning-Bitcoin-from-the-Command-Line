# 3.4: Understanding the Descriptor Wallet

You've got your wallet set up, but before we go further we're going to take a moment to really understand it.

## Understand HD Wallets

Private keys are what make the Bitcoin world go round. They're used to
generate public keys, which are the foundation of addresses, and
they're also used to control those addresses. One private key creates
one public key which creates one address. Once upon a time, the
Bitcoin Core wallet managed this by holding on to a "bag of keys". A
new, unrelated private key would be created every time a new address
was desired. But bags of keys are big, inefficient, and prone to
loss. That's where the HD wallet came in.

The HD wallet, which is short for the Hierarchical Deterministic
Wallet, was defined in
[BIP-32](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki). It's
a hierarchical design where a single "seed" generates an extended
private key, which includes not just the private key but also a "chain
code" that can be used to create descendents of that key. The key and
chain code can be used to deterministically generate chains of keys
(and therefore addresses) for a variety of purposes. A Bitcoin HD
wallet will typically have individual accounts, which are chains of
keys (and addresses) for a variety of different address types. But,
they can all be restored from that seed (or from that master extended
private key) because of their determinism: the addresses are always
created in the same way provided that you have the same starting
points (which include your master extended private key and a specific
index [0,1,...,n] for a specific type of address).

> 📖 ***What is a BIP?*** A BIP is a Bitcoin Improvement
Proposal. It's an in-depth suggestion for a change to the Bitcoin Core
code. Often, when a BIP has been sufficiently discussed and updated,
it will become an actual part of the Bitcoin Core code. BIP-32 is one
of many examples.

The higher levels of secrets, the seeds and the master keys, are
largely hidden from you in Bitcoin Core, but we'll talk about how to
import them from the larger ecosystem in [chapter
10](10_0_Working_with_Secrets.md).

## Understand Descriptor Wallets

Most of this course presumes that you're working entirely from a
single node where you manage your own wallet, sending and receiving
payments with the addresses created by that wallet. However, that's
not necessarily how the larger Bitcoin ecosystem works. There, you're
more likely to be moving addresses between wallets (often on different
devices) and even setting up wallets to watch over funds controlled by
different wallets.

HD wallets offered a big step forward for managing this sort of
interoperability, because they allowed you to load a single seed (or
master extended private key) into a new wallet rather than having to
move over a whole bag of keys. A few formats were quickly introduced
to make it easy to move HD wallets, starting with seed phrases and the
`xpub` and `xprv` formats. But they quickly proved inadequate. Seed
phrases only defined the seed, not what it was used for. The `xprv`
(and `xpub`) improved on that by defining the root of an HD tree, but
they were very specific to a certain type of address. When a new
address type was created, a new format was needed, resulting in the
`yprv` and `ypub` formats. Then a third address type resulted in the
creation of `zprv` and `zpub`. The system was quickly growing
unmanageable.

A new format was needed that described not just the extended keys, but
also which address types they would be used to create. Because if you
didn't have that, a new wallet would have to test a master extended
public key against _every possible type of address_ and that was going
to be very inefficient and time-consuming and still introduced the
likelihood of losing funds.
 
> 📖 ***What is xprv?*** Xprv stands for extended private key. This is
the combination of a private key and a chain code. It's a private key
that a whole sequence of children private keys can be derived from.

> 📖 ***What is xpub?*** Xpub stands for extended public key. This is
the combination of a public key and a chain code. It's a public key
that a whole sequence of children public keys can be derived from.

Enter, at last, the descriptor wallet. A descriptor wallet collects
together "output descriptors" (sometimes called "wallet descriptors"),
which each either define one address or, with a special "ranged
descriptor", a whole array of addresses, each at a separate
index. They do so through the specification of a format that includes:
one or more nested functions (which define how to unlock the Bitcoin
at the address), a derivation path (which not only derives a specific
account or address key from the master key but also provides
information by defining the purpose of an address, which mostly links
it to a specific standard), either the account public key or the
account private key, and a checksum to make sure that nothing has been
corrupted.

> 📖 ***What is a Derivation Path?*** When you have hierarchical keys,
you need to be able to define individual keys as descendents of the
master key. For example `[0]` is the 0th key of the master key,
`[0/1]` is the first son of the 0th key, `[0/1/1]` is the first son of
the first son of the 0th key. Some keys also contain a `'` or `h`
after the number, to show they're hardened, which protects them from a
specific attack that could otherwise be used to derive a private key
from a public key. You don't need to worry about the specifics, other
than the fact that a derivation path like `[0/1/1/0/0]` describes a
path down through a hierarchical tree and that descriptor wallets run
specific calculations to deterministically determine the right address
for a specific position in a tree. A derivation path defines a key,
which means that a key represents a derivation path. They're
equivalent.

The derivation path allows you to calculate the right key from the
master extended key, but it's the introduction of functions into
descriptors that makes them particularly powerful, because they allow
descriptors to serve a number of different types of past, present, and
future addresses (which we'll meet in
[§4.1](04_1_Understanding_the_Address.md)).

> ⚠️ **VERSION WARNING:** Modern Bitcoin wallets use descriptor wallets
stored in SQLite. Older, "classic" wallets were instead bags of keys,
stored in BDB (Berkeley Database) format. The classic files can
currently still be opened by `bitcoin-cli` but you wouldn't want to
create something new in that format.

## Examine Descriptors with `listdescriptors`

Your Bitcoin Core wallet is a descriptor wallet because it creates its
Bitcoin addresses from "ranged descriptors".  You can look at them
with `bitcoin-cli listdescriptors`:

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

Wow, that's a lot! But it's really just a listing of eight descriptors
(`desc`) with a bunch of additional information on each. And those
eight descriptors actually represent a large number of addresses that
can deterministically be created on the fly.

* There are four types of addresses, which we'll meet in the next chapter (`pkh`, `sh`, `tr`, and `wpkh`).
* Each type of address supports external addresses (for receiving funds from other wallets) and internal addresses (for sending change back to this wallet), as defined by `internal`.
* Each type of address has its own account key (e.g., `tpubDCsocyjrtJLXKJ3atFwKf6FiPheuVNV27B1swsGiNvk4cuPhVTsCDvusSNcH8thnS68FPFotgHqo9FHNGrvhtx6ZqdbWBQTWrCgK9xous17`).
* Each type of address supports a range of addresses, initially running 0 to 999.

In other words, even though you have eight descriptors, that's
replacing thousands of addresses.  That's a huge boon for backups
(when you want to protect your funds) and for moving control of your
funds from one wallet-app to another.

With that understood, we can look more closely at one of the descriptors:

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
* **`active`:** Is the descriptor still in use for creating new addresses? (It could have been superseded by a new master extended key, for example when encrypting the wallet.)
* **`internal`:** Is this a descriptor for internal addresses (for change)?
* **`range`:** For ranged descriptors, what's the range?
* **`next`, `next_index`:** What is the next address to create for this descriptor? In this example, the next one is `3` because three addresses have been created, as we'll see in the next chapter.

As for the descriptor itself, let's break that down further:

* **Function: `wpkh`.** The function that is used to create an address from that key. In this cases it's `wpkh`. That stands for "Witness Public Key Hash," which is one of the methods used to unlock a Bech32 address, which we'll meet in the next chapter.
* **Fingerprint: `e18dae20`.** This is a fingerprint of the master extended public key. It tells you which secret was used to generate this address. The fingerprint is *not* necessary to generate the keys and addresses for a derivation, it's just helpful if you need to go back and find the secret that generated your extended keys.
* **Derivation Path for Key: `/84h/1h/0h`.** This describes the derivation path used to create the key in the desciptor from the fingerprinted master key. This is the 0th child key of the 1st child of the 84th child in the HD tree. The various levels in the derivation path have very specific meanings as defined in [BIP-44](https://en.bitcoin.it/wiki/BIP_0044): `/purpose/coin_type/account/`. The purpose of this derivation path is "84", which means that it follows [BIP-84](https://github.com/bitcoin/bips/blob/master/bip-0084.mediawiki), which describes WPKH derivation. The coin type is "1", which means that it's a testnet or signet coin. (A mainnet coin could would be "0".) The account is "0", as it's the only account in our wallet.
* **Key: `tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu`.** This is key derived from the fingerprinted master key using the derivation path. In this case, since the derivation path was `84h/1h/0h`, it's an account key (to be precise, an account public key for testnet). A private key could be here instead: a public key would demonstrate how to watch this series of addresses, while a private key would show to control them.
* **Derivation Path for Addresses: `/0/*`.** This is the rest of the five-part derivation path. What's in the `[brackets]` shows how to derive the key in the descriptor from the fingerprinted master key, while what's here shows you how to derive addresses from that key. Again, BIP-44 defines these levels of the derivation path: they're `change/address_index`. The "0" says it's an external address. (An internal or change address would be "1".) The `*` says it's a ranged address, which means that it's defining a whole set of WPKH addresses that could be created. Each of those addresses has its own "index" (`0`, `1`, etc).
* **`#3658f8sn"`.** This is a checksum showing the descriptor isn't corrupted.

So that's what everything means in a descriptor.

> 📖 **Private Keys Possible.** You could instead run `bitcoin-cli listdescriptors true` if you want your descriptor list to include the private keys instead of the public keys.

## Examine Descriptors with `getdescriptorinfo`

Descriptors can be also be examined with another command, `bitcoin-cli getdescriptorinfo`:
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
This is a pretty sparse description that includes the `descriptor` and its `checksum` and confirms that it's a ranged descriptor (`isrange`), that our wallet has the private key (`issolvable`), and that the private key isn't included in the descriptor and that there's instead a public key (`hasprivatekeys`). Beyond that information, `getdescriptorinfo` serves two other purposes:

1. If you don't have the checksum (which is required for other descriptor-related `bitcoin-cli` commands), you can enter the descriptor into `getdescriptorinfo` without it, and it'll be calculated for you.

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

## Import a Descriptor

The really important feature of descriptors is that you can take them to another (remote) machine and import them. This is done with the `importdescriptors` command. The following example shows the import of a BIP-84 ranged descriptor containing a master private key into another wallet:
```
$ bitcoin-cli importdescriptors '[{ "desc": "wpkh(tprv8ZgxMBicQKsPd1dP4NpsFDpsLUCnZ7oyn4UEbYLw7if1EDVCxMgfSzAwP3aCr1YeRvX9GtGvHsCLdrM7zaDyh33jEj7joQoEeNEyJaSYm5p/84h/1h/0h/0/*)#grdqnase", "timestamp":1770329126, "active": true, "range": [0,10] }]'
[
  {
    "success": true
  }
]
```

You'll note that this is a much more complex `bitcoin-cli` command
than anything we've used before. It requires the input of a JSON array
with a variety of different variables. (Which is a pain.) The `desc`
is that master-private-key descriptor, the `timestamp` says how much of the
blockchain to rescan, the `range` says how much of the range to
import, and the `active` says that this descriptor can be used to
create new addresses. After importing it, this descriptor becomes the
main (`active`) one for your wallet, which will be used to derive new addresses.

## Summary: Understanding the Descriptor

Descriptor wallets define large numbers of keys with a single simple
description for each type of address in your wallet. Currently, there are eight ranged descriptors in each Bitcoin Core Wallet, defining internal and external addresses for the four in-use types of Bitcoin addresses.

> 🔥 ***What is the power of descriptors?*** Descriptors allow you to
import and export sets of keys and addresses. That's great if you want
to move between different wallets. As a developer, they also allow you
to build up the precise sort of addresses that you're interested in
creating.

## What's Next?

Move on to "addresses" with [Chapter Three: Preparing Your Bitcoin Addresses](04_0_Preparing_Your_Bitcoin_Addresses.md).
