# 7.2: Creating a Multisig from Descriptors

You can improve your control over the creation of a multisig by creating it by hand with a descriptor.

## Collect the Keys

The basic process is much the same as in [§7.1](07_1_Creating_Multisig_Public_Keys.md): it starts with collecting keys from two machines:

```
machine1$ address3=$(bitcoin-cli getnewaddress)
machine1$ pubkey3=$(bitcoin-cli -named getaddressinfo address=$address3 | jq -r '.pubkey')
```

And:

```
machine2$ address4=$(bitcoin-cli getnewaddress)
machine2$ pubkey4=$(bitcoin-cli -named getaddressinfo address=$address4 | jq -r '.pubkey')
```

Afterward, you consolidate your info on to a single machine:

```
machine2$ echo $pubkey4
027f5c103ada4029ff7b821c40a1ccff98704d87ace8caaf24fa52ee94b7e6de4b

machine1$ pubkey4=027f5c103ada4029ff7b821c40a1ccff98704d87ace8caaf24fa52ee94b7e6de4b
```

## Create the Descriptor

Next you create the descriptor. This uses the same technique as you applied in [§4.2](04_2_Integrating_Addresses_Descriptors.md), except you'll be creating a slightly more complex multisignature descriptor.

Doing so requires you answer four questions:

1. What address type do you want to use? `sh` (P2SH) or `wsh` (P2WSH)
2. What script function do you wan to use? `multi` (Bitcoin Core default) or `sortedmulti` (safer, but non-default)
3. What is your threshold? (how many keys are required to unlock?)
4. How many keys total are there? (how many keys could be used to meet the threshold?)

You then write a descriptor of the form:
```
address-type(script-function(threshold,key1,key2,...))
```

The following descriptor uses the `sortedmulti` function in a P2WSH address with a threshold of 2 and 2 keys (e.g., a 2-of-2 multisig):
```
machine1$ msdesc="wsh(sortedmulti(2,$pubkey3,$pubkey4))"
```

Bitcoin Core always requires a checksum for its descriptors, with you can retrieve with `getdescriptorinfo`.

```
machine1$ mscs=$(bitcoin-cli getdescriptorinfo $msdesc | jq -r '.checksum')
```

You can then create a descriptor with the checksum:

```
machine1$ msdescwithcs=$msdesc#$mscs

machine1$ echo $msdescwithcs
wsh(sortedmulti(2,03ae6bee76f15dcce5c67a910a484d2482270daa7f1ea38d0db3fc5ec31fe78ed9,027f5c103ada4029ff7b821c40a1ccff98704d87ace8caaf24fa52ee94b7e6de4b))#amajqzxw
```

## Import the Descriptor

You've skipped right past the nead to use `createmultisig` to create your address. Instead, you're now ready to import the descriptor.

You should reload the watch-only wallet you created in the previous section:
```
$ bitcoin-cli loadwallet "watchmulti"
{
  "name": "watchmulti"
}
```

Afterward, you can import the descriptor just like you did in the previous section:
```
$ bitcoin-cli -rpcwallet=watchmulti importdescriptors '''[{ "desc": "'$msdescwithcs'", "timestamp": 1770329126 }]'''
[
  {
    "success": true
  }
]
```

You should now see two addresses in your watch-only wallet: the one you create with `createmultisig` and the one you created by hand using a descriptor:

```
$ bitcoin-cli -rpcwallet=watchmulti getaddressesbylabel ""
{
  "tb1q8cg6qwhhv58zp005w6qnpfx8g6606awkjmf5yzlkulg0sc9phx8sqkltdd": {
    "purpose": "receive"
  },
  "tb1q8lapgtw2fez7xxn4nwtrkzxzaajt5ju52vn4vzws58aexpxzsw9qu2fmc3": {
    "purpose": "receive"
  }
}
```

## Summary: Creating a Multisig from Descriptors

You don't have to depend on the Bitcoin Core commands! As you grow increasingly confident with Bitcoin, there will be many things you can do on your own. Descriptors are one of the best tools for this, because they let you create a variety of addresses just by knowing how they're constructed. Even when you're creating complex multisig addresses, you can fall back on descriptors ... and the results may be even better than using `bitcoin-cli`, because you can choose to sort the addresses, which is not the default for Bitcoin Core.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§7.3: Spending a
Transaction to a
Multisig](07_3_Spending_a_Transaction_to_a_Multisig.md).
