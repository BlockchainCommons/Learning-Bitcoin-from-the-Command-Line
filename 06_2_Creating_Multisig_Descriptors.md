# 6.2: Creating a Multisig from Descriptors

You can improve your control over the creation of a multisig by creating it by hand with a descriptor.

## Collect the Keys

The basic process is much the same as in [§6.1](06_1_Creating_Multisig_Public_Keys.md): it starts with collecting keys from two machines:

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
026137216df604722b973bb43d8ebffd10761a22b3dd98892904eb1163de8206ee

machine1$ pubkey4=026137216df604722b973bb43d8ebffd10761a22b3dd98892904eb1163de8206ee
```

## Create the Descriptor

Next you create the descriptor. This requires two questions:

1. What address type do you want to use? `sh` (P2SH) or `wsh` (P2WSH)
2. What script function do you wan to use? `multi` (Bitcoin Core default) or `sortedmulti` (safer, but non-default)

The following descriptor uses the `sortedmulti` function in a P2WSH address:
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
wsh(multi(2,021c04d13d7047bb8413ecf2f3a7f3c7172eaffff00040a67eb6b10cd57046b958,026137216df604722b973bb43d8ebffd10761a22b3dd98892904eb1163de8206ee))#nvydugpc
```

## Import the Descriptor

You're now ready to import the descriptor.

You should reload the watch-only wallet you created in the previous section:
```
$ bitcoin-cli loadwallet "watch_multi"
{
  "name": "watch_multi"
}
standup@lbtc:~$
```

Afterward, you can import the descriptor just like you did in the previous section:
```
$  bitcoin-cli -rpcwallet=watch_multi importdescriptors '''[{ "desc": "'$msdescwithcs'", "timestamp": 1770329126 }]'''
[
  {
    "success": true
  }
]
```

You should now see two addresses in your watch-only wallet: the one you create with `createmultisig` and the one you created by hand using a descriptor:

```
$ bitcoin-cli -rpcwallet=watch_multi getaddressesbylabel ""
{
  "tb1qj885390pmsaggamryky7w67ax8xq3dk673epgsyjy7d3gvmvevvqmq6jzy": {
    "purpose": "receive"
  },
  "tb1qnx6ns5mn2435tqvu76nurwc2p2zahh6tcgh92hqn800z04ka7v7sggc0j8": {
    "purpose": "receive"
  }
}
```

## Summary: Creating a Multisig from Descriptors

You don't have to depend on the Bitcoin Core commands! As you grow increasingly confident with Bitcoin, there will be many things you can do on your own. Descriptors are one of the best tools for this, because they let you create a variety of addresses just by knowing how they're constructed. Even when you're creating complex multisig addresses, you can fall back on descriptors ... and the results may be even better than using `bitcoin-cli`, because you can choose to sort the addresses, which is not the default for Bitcoin Core.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§6.3: Spending a
Transaction to a
Multisig](06_3_Spending_a_Transaction_to_a_Multisig.md).
