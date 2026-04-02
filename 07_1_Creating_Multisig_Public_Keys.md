# 7.1: Creating a Multisig from Public Keys

The first way to vary how you send a basic transaction is to use a multisig. This gives you the ability to require that multiple people (or at least multiple private keys) authorize the use of funds.

## Understand How Multisigs Work

In a typical Bitcoin transaction, bitcoins are sent to an address based on your public key, which in turn means that the related private key is required to unlock the transaction in order to solve the cryptographic puzzle and allow you to reuse the funds. But what if you could instead lock a transaction with _multiple_ private keys? This would effectively allow funds to be sent to a group of people, where those people all have to agree to reuse the funds.

> 📖 ***What is a multisignature?*** A multisignature is a methodology that allows more than one person to jointly create a digital signature. It's a general technique for the cryptographic use of keys that goes far beyond Bitcoin.

> 📖 ***What is a multisignature transaction?*** A multisignature transaction is a Bitcoin transaction that has been sent to a multisignature address, thus requiring the signatures of certain people from the multisignature group to reuse the funds.

Technically, a multisignature cryptographic puzzle is created by Bitcoin using the OP_CHECKMULTISIG command, and typically that's encapsulated in a script address. [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md) will detail how that works more precisely. For now, all you need to know is that you can use `bitcoin-cli` command to create multisignature addresses; funds can be sent to these addresses just like any normal address, but multiple private keys will be required for the redemption of the funds.

As with normal addresses, there are multiple types of multisig addresses that have appeared as Bitcoin has evolved over the years. Currently, there is one deprecated address type and three that are in active use. The following shows each, with the flag that you'll be using to create them for the `createmultisig` RPC.

| Flag | Type | Prefix | Notes | 
|------|------|--------|-------|
| | P2MS | N/A | deprecated |
| `bech32` | P2WSH | bc1q...<br>tb1q... | |
| `legacy` | P2SH | 3...<br>2... | default |
| `p2sh-segwit` | P2SH-PWPKH | 3...<br>2... | |

Multisigs have been somewhat neglected by Bitcoin Core since the SegWit transition, which is likely why `legacy` addresses are still the default. Nonetheless, just as with regular addresses, you can choose to create multisig addresses from any stage of the SegWit transition: `legacy` for pre-SegWit addresses; `p2sh-segwit` for adddresses used during the transition; and `bech32` for modern SegWit addresses.

Simple multisignatures require everyone in the group to sign the UTXO when it's spent. However, there's more complexity possible. Multisignatures are generally described as being "m of n". That means that the transaction is locked with a group of "n" keys, but only "m" of them are required to unlock the transaction, where m≤n.

> 📖 ***What is a m-of-n multisignature?*** In a multisignature, "m" signatures out of a group of "n" are required to form the signature, where "m ≤ n".

## Create a Multisig Address

In order to lock a UTXO with multiple private keys, you must first create a multisignature address. 

This requires a number of steps:

1. Generate "n" addresses.
2. Record the public key for each address.
3. Consolidate the public keys onto a single machine.
4. Create the multisig on one of the machines with the `createmultisig` RPC command.
5. Record crucial information about the multisig.

The examples used here show the creation (and usage) of a 2-of-2 multisignature.

### Generate the Address

To create a multisignature address, you must first ready the addresses that the multisig will combine. Best practice suggests that you always create new addresses. This means that the participants will each run the `getnewaddress` command on their own machine:
```
machine1$ address1=$(bitcoin-cli getnewaddress)
```
And:
```
machine2$ address2=$(bitcoin-cli getnewaddress)
```
Afterwards, one of the recipients (or perhaps some third party) will need to combine the addresses. 

### Record the Public Keys

However, you can't create a multi-sig from addresses, as those are the hashes of public keys: you instead need the public keys themselves.

This information is readily available with the `getaddressinfo` command, as shown in this example from `machine`.
```
machine2$ bitcoin-cli -named getaddressinfo address=$address2
{
  "address": "tb1qqw20avmlwe6tw6fsjvm5ghdgdwvaa6plgec0y0",
  "scriptPubKey": "00140394feb37f7674b769309337445da86b99dee83f",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([e18dae20/84h/1h/0h/0/3]03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0)#nhkqpvrv",
  "parent_desc": "wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "0394feb37f7674b769309337445da86b99dee83f",
  "pubkey": "03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0",
  "ischange": false,
  "timestamp": 1770329126,
  "hdkeypath": "m/84h/1h/0h/0/3",
  "hdseedid": "0000000000000000000000000000000000000000",
  "hdmasterfingerprint": "e18dae20",
  "labels": [
    ""
  ]
}
```
The `pubkey` address (`03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0`) is what's required. 

You can retrieve it from both machines with the following commands:

```
machine1$ pubkey1=$(bitcoin-cli -named getaddressinfo address=$address1 | jq -r '.pubkey')
```

And:

```
machine2$ pubkey2=$(bitcoin-cli -named getaddressinfo address=$address2 | jq -r '.pubkey')
```

### Consolidate the Public Keys

Via some safe, _accurate_ means, copy the public keys to a single machine.

> ⚠️ **Public Keys Are Potentially Insecure.** Bitcoin's use of public-key hashes as addresses instead of public keys represents an additional layer of security. Therefore, sending a public key slightly increases the vulnerability of the associated address, for some far-future possibility of a compromise of the elliptic curve. You shouldn't worry about having to occasionally send out a public key for a usage such as this, but you should be aware that the public-key hashes represent security, and so the actual public keys should not be sent around willy nilly.

The following examples presume the `$pubkey2` variable from machine2 was copied to a `$pubkey2` variable on machine1 (but either would work)

### Create the Multisig

A multisig can now be created with the `createmultisig` command:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]''' address_type=bech32
{
  "address": "tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd",
  "redeemScript": "5221039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d3312103c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c052ae",
  "descriptor": "wsh(multi(2,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#k626xmlq"
}
```
When creating the multisignature address, you list how many signatures are required with the `nrequired` argument (that's "m" in a "m-of-n" multisignature), then you list the total set of possible signatures with the `keys` argument (that's "n"). Note that the the `keys` entries likely came from different places. In this case, we included `$pubkey1` from the local machine and `$pubkey2` that we filled in using the public key from a remote machine.

> ℹ️ **m-of-n vs. n-of-n.** This example shows the creation of a simple 2-of-2 ("n-of-n") multisig. If you instead want to create an m-of-n signature where "m < n", you adjust the `nrequired` field and/or the number of signatures in the `keys` JSON object. For a 1-of-2 multisig, you'd set `nrequired=1` and also list two keys, while for a 2-of-3 multisig, you'd leave `nrequired=2`, but add one more public key to the `keys` listing.

When used correctly, `createmultisig` returns three results, all of which are critically important.

* The ***address*** is what you'll give out to people who want to send funds. It will always be a script address instead of a normal public-key-hash address, though for native SegWit address, they all look the same, either `bc1q` or `tb1q`. In any case, as we saw in [§4.6](4_6_Sending_Coins_to_Other_Addresses.md), all the addresses receive coins in the same way, so you don't need to worry about which it us. 
* The ***redeemScript*** is what you need to redeem the funds (along with the private keys for "m" of the "n" addresses). This script is another special feature of P2SH addresses and will be fully explained in [§10.3: Running a Bitcoin Script with P2SH](10_3_Running_a_Bitcoin_Script_with_P2SH.md). For now, just be aware that it's a bit of data that's required to get your money.
* The ***descriptor*** is the standardized description for an address that we met in [§3.4: Understanding the Descriptor](03_4_Understanding_the_Descriptor.md). It provides one way that you could import this address back to the other machine, using the `importdescriptors` RPC.

> 📖 ***What is a script address?*** A script address is a different type of recipient than a standard SegWit address, used for funds whose redemption are based on more complex Bitcoin Scripts. It can be a P2SH, P2WSH, or P2TR address. `bitcoin-cli` uses P2SH encapsulation to help standardize and simplify its multisigs as "P2SH multisigs", just like P2SH-SegWit was using P2SH to standardize its SegWit addresses and make them fully backward compatible.

> ⚠️ **Multisig Size Limits.** Classic multisigs have a limit to 20-of-20 multisigs and the classic `redeemScript` could only fit 15 public keys. P2WSH multisigs, like the ones described in this chapter, can be no bigger than that.

### Record Crucial Info

Here's an important caveat: nothing about your multisig is saved into your wallet using these basic techniques. In order to later redeem money sent to this multisignature address, you're going to need to retain two crucial bits of information:

   * A list of the Bitcoin addresses used in the multisig, _in order_.
   * The type of address created.
   * The `redeemScript` output by `createmultsig`.
   
Technically, the `redeemScript` can be recreated by rerunning `createmultisig` with the complete list of public keys _in the same order_ and with the right m-of-n count and the right address type. But, it's better to hold onto it and save yourself stress and grief.

### Everything Matters

Here's proof that all the defails matter:

If you put your keys in a different order, a different address will be created:

```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]''' address_type=bech32
{
  "address": "tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd",
  "redeemScript": "5221039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d3312103c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c052ae",
  "descriptor": "wsh(multi(2,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#k626xmlq"
}

machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey2'","'$pubkey1'"]''' address_type=bech32
{
  "address": "tb1qmvc8aeaws3q0vpvhltuag7l87xqyykxspe6zmspfj47566ts0avqmq3vsf",
  "redeemScript": "522103c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c021039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d33152ae",
  "descriptor": "wsh(multi(2,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331))#hh0tm58j"
}
```

If you choose a different address type, a different address will be created:

```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]''' address_type=bech32
{
  "address": "tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd",
  "redeemScript": "5221039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d3312103c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c052ae",
  "descriptor": "wsh(multi(2,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#k626xmlq"
}

machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]''' address_type=legacy
{
  "address": "2N22883rSKTuAVafgdxpFT3u2dc4LGCzWQL",
  "redeemScript": "5221039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d3312103c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c052ae",
  "descriptor": "sh(multi(2,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#3n4msv3t"
}
```

To be precise, each ordering and each address type creates a different _redeemScript_. That means that if you used these basic techniques and failed to save the redeemScript as you were instructed, you'll have to walk through an ever-increasing number of variations to find the right one when you try and spend your funds!

[BIP67](https://github.com/bitcoin/bips/blob/master/bip-0067.mediawiki) suggests a way to lexicographically order keys, so that they always generate the same multisignatures. ColdCard and Electrum are among the wallets that already support this. Of course, this can cause troubles on its own if you don't know if a multisig address was created with sorted or unsorted keys. Once more, [descriptors](03_5_Understanding_the_Descriptor.md) come to the rescue. If a multisig is unsorted, it's built with the function `multi` and if it's sorted it's built with the function `sortedmulti`.

If you look at the `desc`riptor for the multisig that you created above, you'll see that Bitcoin Core doesn't currently sort its multisigs:
```
  "descriptor": "wsh(multi(2,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#k626xmlq"
```
However, if it imports an address with type `sortedmulti`, it'll do the right thing, which is the whole point of descriptors! (We'll do that in the next section.)

## Import a Multisig Address

Though you used `bitcoin-cli` to create a multisig address, it won't be in any of your wallets. 

To import it, you first need to create a watch-only wallet (without private keys and without any keys of its own):
```
$ bitcoin-cli -named createwallet wallet_name="watchmulti" disable_private_keys=true blank=true
{
  "name": "watchmulti"
}
```

You can then import the descriptor to that wallet using the `descriptor` line from the `createmultisig` command:
```
$ bitcoin-cli -rpcwallet=watchmulti importdescriptors '[{ "desc": "wsh(multi(2,039395fa19d6512f03043210cd3e9a03a850f7a8d986c8f35d30f2efc281a8d331,03c57ed70775d7a616778514e738fef0946b4be4ee32440b19f65ddd6e345983c0))#k626xmlq", "timestamp":1770329126 }]'
[
  {
    "success": true
  }
]
```

Looking at the addresses in that wallet will now list your multisig address, which will allow you to query when it has funds:
```
$ bitcoin-cli -rpcwallet=watchmulti getaddressesbylabel ""
{
  "tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd": {
    "purpose": "receive"
  }
}
```

Remember per [§3.3](03_3_Setting_Up_Your_Wallet.md#optional-create-multiple-wallets) that you can `loadwallet`, `unloadwallet`, or use the `-rpcwallet` flag when multuple wallets are loaded. In this case, we're using `-rpcwallet` to make it clear that we're accessing something other than our default (`""`) wallet.

## Send to a Multisig Address

Once you've got a multisig, you can send to it normally, either creating a transaction yourself from the command line or tapping a faucet. [§4.6](04_6_Sending_Coins_to_Other_Addresses.md) included a demonstration of sending to a P2SH address, and that's one of the types that is used to create multisig addresses.

## Summary: Sending a Transaction with a Multisig

Multisig addresses lock funds to multiple private keys — possibly requiring all of those private keys for redemption, and possibly requiring just some from the set. They're easy enough to create with `bitcoin-cli` and they're entirely normal to send to. This ease is due in large part to the invisible use of P2SH (pay-to-script-hash) or P2WSH (pay-to-witness-script-hash) addresses, a large topic that wwill get more coverage in the future.

> 🔥 ***What is the power of multisignatures?*** Multisignatures allow the modeling of a variety of financial arrangements such as corporations, partnerships, committees, and other groups. A 1-of-2 multisig might be a married couple's joint bank account, while a 2-of-2 multisig might be used for large expenditures by a Limited Liability Partnership. Multisignatures also form one of the foundations of Smart Contracts. For example, a real estate deal could be closed with a 2-of-3 multisig, where the signatures are submitted by the buyer, the seller, and a licensed escrow agent. Once the escrow agent agrees that all of the conditions have been met, he frees up the funds for the seller; or alternatively, the buyer and seller can jointly free the funds.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§7.2: Creating a Multisig from Descriptors](07_2_Spending_a_Transaction_to_a_Multisig.md).
