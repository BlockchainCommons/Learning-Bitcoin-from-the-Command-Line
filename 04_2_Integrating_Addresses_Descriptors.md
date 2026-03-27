# 4.2: Integrating Addresses and Descriptors

You now have an understanding of your descriptor wallet and the
variety of addresses that it can create. You've even seen how your
descriptor wallet contains ranged descriptors for four sorts of
addresses.

But you can also have _non-ranged_ descriptors for individual
addresses. This section look at them.

## Examine an Address' Descriptor

You created a set of three addresses in the previous section. You can
see the details of any individual address with the `getaddressinfo`
command, including its individual descriptor:

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

This reveals the descriptor for this individual address:

```
  "desc": "wpkh([e18dae20/84h/1h/0h/0/2]02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba)#dqt0983r",
```

You can compare that to the `parent_desc`, which contains the ranged
descriptor that this address descriptor is descended from (and that
you also saw when you listed out all of your descriptors):

```
  "parent_desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
```

They're in slightly different formats as the non-ranged address has
the derivation path all together rather than it being split in
two. But other than that, there are just two changes:

* The wallet descriptor has a range of addresses `0/*`, while the address descriptor displays one specific index in that range `0/2`.
* The checksums are different, as you'd expect due to the differences in the index number.

That's all that's different between a wallet descriptor and an address
descriptor (and that similarity is how the one is used to derive
hundreds or thousands of the other).

## Derive Addresses from a Descriptor

In fact, you can derive addresses from a descriptor on your own,
without having to use the `getnewaddress` command again and
again. This is done with the `deriveaddresses` command, which you give
a ranged descriptor, then tell it how far to derive to:

```
$ bitcoin-cli deriveaddresses "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn" 2
[
  "tb1q05ua6g7njnjrtsjc0t9w3jc6g2leeznasf4ny9",
  "tb1q0psqqqgy0fv5928wmk86ntu7hlax8dva7nl82p",
  "tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4"
]
```

This example shows the derivation of addresses from the BIP-84 ranged
ndescriptor up through index "2". If you check this against the
addresses created in in the previous section, you'll see they're just
the same. Which is of course the whole point of descriptors! They are
deterministically derived in the same way every time.

The main purpose of this function would be to export addresses to
other services (for example, if you wanted to export watch-only
addresses to another wallet-app of if you wanted to watch a multisig
address, as we will in chapter 7).

> 📖 ***What is a watch-only address?*** A watch-only address allows
you to watch for transactions related to an address (or to a whole
family of addresses if you used a ranged descriptor), but not to spend
funds on those addresses.

## Create a Descriptor by Hand

Not only can you derive addresses from a descriptor by hand, but you
can also create a descriptor by hand.

There is an extensive list of descriptors that you can create on the
[Bitcoin Core
GitHub](https://github.com/bitcoin/bitcoin/blob/master/doc/descriptors.md). Following
are a few examples.

Once you've created any descriptor, you can then import it with the
`importdescriptors` command that you used in
[§3.4](03_4_Understanding_the_Descriptor_Wallet.md).

Here's what it looked like to import a ranged desccriptor to become one of the `active` descriptors used to generate addresses in your wallet:
```
$ bitcoin-cli importdescriptors '[{ "desc": "wpkh(tprv8ZgxMBicQKsPd1dP4NpsFDpsLUCnZ7oyn4UEbYLw7if1EDVCxMgfSzAwP3aCr1YeRvX9GtGvHsCLdrM7zaDyh33jEj7joQoEeNEyJaSYm5p/84h/1h/0h/0/*)#grdqnase", "timestamp":1770329126, "active": true, "range": [0,10] }]'
```

You will change some of the variables when you import the new
descriptors you're creating below:

```
* **`active`** is not set if this is not a ranged descriptor meant to become one of defaults for creating new addresses.
* **`range`* is not set if it's a descriptor for a single address.

### Create a Watch-Only Wallet

Your default wallet (`""`) is set to hold private keys. You can use
that if you're importing descriptors where you have the private
key. However, if you want to import descriptors without private keys,
you need to create a special watchonly wallet:

```
$ bitcoin-cli createwallet "watchonly" true true
{
  "name": "watchonly"
}
```

The two `true`s are the magic sauce as shown in the help file:

```
1. wallet_name             (string, required) The name for the new wallet. If this is a path, the wallet will be created at the path location.
2. disable_private_keys    (boolean, optional, default=false) Disable the possibility of private keys (only watchonlys are possible in this mode).
3. blank                   (boolean, optional, default=false) Create a blank wallet. A blank wallet has no keys.
```

The first `true` disables the use of private keys, the second `true` tells the wallet not to create keys of its own.

Remember that you're going to have to use `loadwallet` and
`unloadwallet` to cycle to right wallet, or else use a `-rpcwallet`
flag with every command to make sure you're using the wallet. (We'll
do the latter in the following examples.)

### Create an Address Descriptor

An address descriptor takes the form:
```
addr(ADDR)
```

Using this address descriptor would allow you to import one of the addresses you'd already
created into another wallet.

Creating a descriptor and importing it into Bitcoin Core is always a three-step process:

1. Create the descriptor.

```
addr(tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4)
```

2. Feed the descriptor into `getdescriptorinfo` to get a checksum.

```
$ bitcoin-cli getdescriptorinfo "addr(tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4)"
{
  "descriptor": "addr(tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4)#4vmsvy3l",
  "checksum": "4vmsvy3l",
  "isrange": false,
  "issolvable": false,
  "hasprivatekeys": false
}
```

3. Import the descriptor with checksum.

```
$ bitcoin-cli -rpcwallet=watchonly importdescriptors '[{ "desc": "addr(tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4)#4vmsvy3l", "timestamp":1770329126 }]'
[
  {
    "success": true
  }
]
```

Looking at the newly imported address reveals that the metadata is somewhat different from what was in the original wallet:
```
$ bitcoin-cli -rpcwallet=watchonly getaddressinfo tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4
{
  "address": "tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4",
  "scriptPubKey": "00142a4f27c78470206e1a5948b47478d5b37991aac4",
  "ismine": true,
  "solvable": false,
  "parent_desc": "addr(tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4)#4vmsvy3l",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "2a4f27c78470206e1a5948b47478d5b37991aac4",
  "ischange": false,
  "labels": [
    ""
  ]
}
```

For example, we no longer have the ranged `parent_desc` and this one
is not `solvable` (we don't have the private key!). Nonetheless it's
the same address, which means that it's unlocked in the same way (as
shown by the identical `scriptPubKey`, a topic we'll return to).

### Create a Keyed Descriptor

The `pk`, `pkh`, and `wpkh` descriptors are all equally easy to
create, since they just the form of `function(key)`.

If we go back to our original `getaddressinfo`, we can find that the
public key for the address
`tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4` is:

```
  "pubkey": "02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba",
```

That means the wpkh descriptor would be:

```
wpkh(02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba)
```

We retrieve a checksum for it:

```
$ bitcoin-cli getdescriptorinfo "wpkh(02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba)"
{
  "descriptor": "wpkh(02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba)#3303qrm5",
  "checksum": "3303qrm5",
  "isrange": false,
  "issolvable": true,
  "hasprivatekeys": false
}
```

Then we import it:

```
bitcoin-cli -rpcwallet=watchonly importdescriptors '[{ "desc": "wpkh(02040bf9b12e48bbbcbf72ef5197bc18067db378411ae6220f1d0a77da2ee7dbba)#3303qrm5", "timestamp":1770329126 }]'
[
  {
    "success": true
  }
]
```

Voila, we have round-tripped the address via a descriptor with the public key:

```
$ bitcoin-cli -rpcwallet=watchonly getaddressesbylabel ""
{
  "tb1q9f8j03uywqsxuxjefz68g7x4kduer2ky6shsf4": {
    "purpose": "receive"
  }
}
```

> 📖 **Why didn't we supply a derivation path?** Derivation paths
derive child keys from master keys. If we wanted to create a ranged
descriptor we'd need a derivation path so that all the indexed keys
could be created. Similarly, if we knew the master key but not the
address key, we'd need a derivation path. In this case, we had the
specific key for this specific address, and so no derivation path was
needed.

## Create Other Descriptors

This process could be repeated in a number of different ways. You
could create a descriptor with a private key instead of a public key,
and import it into non-watchonly wallet. You could create a ranged
descriptor by hand and import a whole set of addresses. Although it's
not best practice, you could even use the same key to create different
types of addresses. (Try it out: just replace the "wpkh" above with
"pkh", get a new checksum, and import and you'll have a P2PKH address
instead of a P2WPKH address, unlocked by the same key.)

The main purpose here is to show how descriptors work in practice, so
that the link between descriptors and address is clear, and so you can
easily create addresses from descriptors when it's helpful in the
future, such as when we create multisigs.

## Summary: Integrating Addresses and Descriptors

In the modern Bitcoin ecosystem, addresses and descriptors go hand in
hand—and it's not just that you use ranged descriptors to create sets
of address.

* You can view descriptors from individual addresses.
* You can derive addresses directly from descriptors.
* You can generate descriptors by hand and turn them into addresses.

These are powerful techniques that we may not use a lot on the command
line, but which are crucial to an overall understand of how Bitcoin
works.

## What's Next?

Continue "Preparing Your Bitcoin Addresses" with [§4.3: Creating QR
Codes for Address](04_3_Creating_QR_Codes_for_Addresses.md).
