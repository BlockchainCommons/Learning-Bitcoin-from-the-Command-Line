# 5.6: Sending Coins to Other Addresses

You've sent coins to P2WPKH addresses by a variety of means. But what if you want to send coins to other address types?

It turns out that it's so simple that you don't have to think about it.

## Send Coins to Supported Addresses

If you want to send a coin to any supported address described in [§4.1](04_1_Understanding_the_Address.md) you can do so with any of the methodologies described to date. There's nothing more: it just works, because Bitcoin is backward compatible and because Bitcoin Core is going to always be on the leading edge of new address types.

Here's some examples, using the simple `sendtoaddress` command from [§5.1](05_1_Sending_Coins_The_Easy_Way.md). (Addresses are drawn from the [recycle page](https://signet257.bublina.eu.org/about.html#recycle) on the [Alt Signet Faucet](https://signet257.bublina.eu.org/).

**P2PKH:**

```sh
bitcoin-cli sendtoaddress mmDZXXiHqydn31zgbUaKiLNBxwNCcX6Eam 0.001

| 2f60126b63260ce814a7d636df4d27d68dc73fbd145877a102d80c7747d89f55
```

**P2SH-P2WPKH:**

```sh
bitcoin-cli sendtoaddress 2NEDviSjc3UP8c4VpUPgjyovY1m6KvyJq1m 0.001

| a3028893d2b5d4843d25abd562023eb05e447b4af163c17811facc69423d02b1
```

**P2TR:**

```sh
bitcoin-cli sendtoaddress tb1p4tp4l6glyr2gs94neqcpr5gha7344nfyznfkc8szkreflscsdkgqsdent4 0.001

| 75884b60ca03b17a5bb68590b375ba676566383d8bd77da304aeb0aa6f040f89
```

However, `bitcoin-cli` will no longer send to some of the most outdated addresses. The following are examples of the oldest P2PK addresses (one compressed, the other not):

```sh
bitcoin-cli sendtoaddress 04a8c3fa3dbc022ca7c9a2214c5e673833317b3cff37c0fc170fc347f1a2f6b6e2a53db4d023387f89209de481cd014a44040e1b09c3226d40fed02c0bc8d0f548 0.001

| error code: -5
| error message:
| Invalid Bitcoin address: 04a8c3fa3dbc022ca7c9a2214c5e673833317b3cff37c0fc170fc347f1a2f6b6e2a53db4d023387f89209de481cd014a44040e1b09c3226d40fed02c0bc8d0f548

bitcoin-cli sendtoaddress 02a8c3fa3dbc022ca7c9a2214c5e673833317b3cff37c0fc170fc347f1a2f6b6e2 0.001

| error code: -5
| error message:
| Invalid Bitcoin address: 02a8c3fa3dbc022ca7c9a2214c5e673833317b3cff37c0fc170fc347f1a2f6b6e2
```
That doesn't meant the addresses aren't valid. These still are, and they contain large amounts of Bitcoin; the Bitcoin Core engineers have just decided that there's no reason to send additional funds to those older addresses that will be insecure in the case of a Quantum Computer breakthrough (and in fact there are [discussions](https://www.coindesk.com/tech/2025/07/16/bitcoin-devs-float-proposal-to-freeze-quantum-vulnerable-addresses-even-satoshi-nakamoto-s) about freezing the funds due to their vulnerability, though that would be a rather dramatic change to traditional Bitcoin consensus).

## Summary: Sending Coins to Other Addresses

Now that you know how to send coins via a variety of means, you can send them to any sort of address. Bitcoin Core and the protocols take care of the specifics. The only exceptions are addresses that are so old that they haven't been used for over a decade, and which are largely considered dead.

## What's Next?

Advance through "bitcoin-cli" with [Chapter Six: Controlling Bitcoin
Transactions](06_0_Controlling_Bitcoin_Transactions.md).
