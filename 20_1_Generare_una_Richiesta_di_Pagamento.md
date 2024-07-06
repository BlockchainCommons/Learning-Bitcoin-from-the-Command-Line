# 20.1: Generating a Payment Request

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This section describes how payments work on the Lightning Network, how to create a payment request (or _invoice_), and finally how to make sense of it. Issuing invoices depends on your having a second Lightning node, as described in [Accessing a Second Lightning Node](19_2__Interlude_Accessing_a_Second_Lightning_Node.md). These examples will use an LND node as their secondary node, to further demonstrate the possibilities of the Lightning Network. To differentiate between the nodes in these examples, the prompts will be shown as `c$` for the c-lightning node and `lnd$` as the LND node. If you want to reproduce this steps, you should [install your own secondary LND node](19_2__Interlude_Accessing_a_Second_Lightning_Node.md#creating-a-new-lnd-node).

> :book: ***What is an Invoice?** Almost all payments made on the Lightning Network require an invoice, which is nothing more than a **request for payment** made by the recipient of the money and sent by variety of means to the paying user.  All payment requests are single use. Lightning invoices use bech32 encoding, which is already used by Segregated Witness for Bitcoin.

## Create an Invoice

To create a new invoice on c-lightning you would use the `lightning-cli --testnet invoice` command.

Here's how it would work with c-lightning, using arguments of an amount (in millisatoshis), a label, and a description.
```
c$ lightning-cli --testnet invoice 100000 joe-payment "The money you owe me for dinner"
{
   "payment_hash": "07a1c4bd7a38b4dea35f301c173cd8f9aac253b66bd8404d7ad829f226342490",
   "expires_at": 1603305795,
   "bolt11": "lntb1u1p0cw3krpp5q7suf0t68z6dag6lxqwpw0xclx4vy5akd0vyqnt6mq5lyf35yjgqdpj235x2grddahx27fq09hh2gr0wajjqmt9ypnx7u3qv35kumn9wgxqyjw5qcqp2sp5r3puay46tffdyzldjv39fw6tzdgu2hnlszamqhnmgjsuxqxavpgs9qy9qsqatawvx44x5qa22m7td84jau5450v7j6sl5224tlv9k5v7wdygq9qr4drz795lfnl52gklvyvnha5e5lx72lzzmgzcfnp942va5thmhsp5sx7c2",
   "warning_capacity": "No channels",
   "warning_mpp_capacity": "The total incoming capacity is still insufficient even if the payer had MPP capability."
}
```
However, for this example we're going to instead generate an invoice on an LND node, and then pay it on the c-lightning node. This requires LND's slightly different `addinvoice` command.  You can use `--amt` argument to indicate amount to be paid (in millisatoshis) and add a description using the `--memo` argument.

```
lnd$ lncli -n testnet addinvoice --amt 10000 --memo "First LN Payment - Learning Bitcoin and Lightning from the Command line."
{
    "r_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
    "payment_request": "lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd",
    "add_index": "1"
}
```
Note that these invoices don't directly reference the channel you created: that's necessary for payment, but not for requesting payment.

## Understand an Invoice

The `bolt11` `payment_request` that you created is made up of two parts: one is human readable and the other is data.

> :book: **What is a BOLT?** The BOLTs are the individual [specifications for the Lightning Network](https://github.com/lightningnetwork/lightning-rfc).

### Read the Human-Readable Invoice Part

The human readable part of your invoices starts with an `ln`. It is `lnbc` for Bitcoin mainnet, `lntb` for Bitcoin testnet, or `lnbcrt` for Bitcoin regtest.
It then lists the funds requested in the invoice.

For example, look at your invoice from your LND node:
```
lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
```
The human readable part is `ln` + `tb` + `100u`.

`lntb` says that this is a Lightning Network invoice for Testnet bitcoins.

`100u` says that it is for 100 bitcoins times the microsatoshi multiplier. There are four (optional) funds multipliers:

* `m` (milli): multiply by 0.001
* `u` (micro): multiply by 0.000001
* `n` (nano): multiply by 0.000000001
* `p` (pico): multiply by 0.000000000001

100 BTC * .000001 = .0001 BTC, which is the same as 10,000 satoshis.

### Read the Data Invoice Part

The rest of the invoice (`1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd`) contains a timestamp, specifically tagged data, and a signature. You obviously can't read it yourself, but you can ask c-lightning's `lightning-cli` to do so with the `decodepay` command:
```
c$ lightning-cli --testnet decodepay lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
{
   "currency": "tb",
   "created_at": 1602702347,
   "expiry": 3600,
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "msatoshi": 10000000,
   "amount_msat": "10000000msat",
   "description": "First LN Payment - Learning Bitcoin and Lightning from the Command line.",
   "min_final_cltv_expiry": 40,
   "payment_secret": "e946403bcb54764994306c743ed3bf1303af8ae6e7687d3cabcc3ee06dbf904d",
   "features": "028200",
   "payment_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
   "signature": "304402202e73ebd1ef974594eb117e8301be781f2ba289041ab6abc558d432351d8365e902202a8151c3fd13419b9390c2b976503d2d064f2a6748b14cb0db64424cf4e572f4"
}

```
Here's what the most relevent elements mean:

1. `currency`: The currency being paid.
1. `created_at`: Time when the invoice was created.  This is measured in UNIX time, which is seconds since 1970.
1. `expiry`: The time when your node marks the invoice as invalid. Default is 1 hour or 3600 seconds.
1. `payee`: The public key of the person (node) receiving the Lightning Network payment.
1. `msatoshi` and `amount_msat`: The amount of satoshis to be paid.
1. `description`: The user-input description.
1. `payment_hash`: The hash of the preimage that is used to lock the payment. You can only redeem a locked payment with the corresponding preimage to the payment hash. This enables routing on the Lightning Network without trusting third parties, by creating a **Conditional Payment** to be filled.
1. `signature`: The DER-encoded signature.

> :book: ****What are Conditional Payments?*** Although Lightning Channels are created between two participants, multiple channels can be connected together, forming a payment network that allows payments between all the network participants, even those without a direct channel between them. This is done using an smart contract called a **Hashed Time Locked Contract**.

> :book: ***What is a Hashed Time Locked Contract (HTLC)?*** A Hashed Time Locked Contract is a conditional payment that use hashlocks and timelocks to ensure payment security. The receiver must present a payment preimage or generate a cryptographic proof of payment before a given time, otherwise the payer can cancel the contract by spending it. These contracts are created as outputs from the **Commitment Transaction**.

> :book: ***What is a Commitment Transaction?*** A Commitment Transaction is a transaction that spends the original funding transaction. Each peer holds the other peer's signature, meaning that either one can spent his commitment transaction whatever he wants. After each new commitment transaction is created the old one is revoked. The commitment transaction is one way that the funding transaction can be unlocked on the blockchain, as discussed in [§20.3](20_3_Closing_a_Channel.md).

### Check Your Invoice

There are two crucial elements to check in the invoice. The first, obviously, is the payment amount, which you've already examined in the human-readable part. The second is the `payee` value, which is the pubkey of the recipient (node):
```
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
```
You need to check that's the expected recipient.

Looking back at [§20.3](20_3_Closing_a_Channel.md), you can see that's indeed the peer ID that you used when you created your channel. You could also verify it on the opposite node with the `getinfo` command.
```
lnd$ lncli -n testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 1,
    "num_inactive_channels": 0,
    "num_peers": 3,
    "block_height": 1862983,
    "block_hash": "00000000000000c8c2f58f6da2ae2a3884d6e84f55d0e1f585a366f9dfcaa860",
    "best_header_timestamp": "1602702331",
    "synced_to_chain": true,
    "synced_to_graph": true,
    "testnet": true,
    "chains": [
        {
            "chain": "bitcoin",
            "network": "testnet"
        }
    ],
    "uris": [
    ],
    "features": {
        "0": {
            "name": "data-loss-protect",
            "is_required": true,
            "is_known": true
        },
        "5": {
            "name": "upfront-shutdown-script",
            "is_required": false,
            "is_known": true
        },
        "7": {
            "name": "gossip-queries",
            "is_required": false,
            "is_known": true
        },
        "9": {
            "name": "tlv-onion",
            "is_required": false,
            "is_known": true
        },
        "13": {
            "name": "static-remote-key",
            "is_required": false,
            "is_known": true
        },
        "15": {
            "name": "payment-addr",
            "is_required": false,
            "is_known": true
        },
        "17": {
            "name": "multi-path-payments",
            "is_required": false,
            "is_known": true
        }
    }
}
```
However, the `payee` may also be someone brand new, in which case you'll likely need to check with the web site or person who issued the invoice to ensure that it's correct.

## Summary: Generating a Payment Request

In most cases you need to receive an invoice to use Lightning Network payments. In this example we've created one manually, but if you had a production environment, you'd likely have systems automatically doing this whenever someone purchases products or services. Of course, once you've received an invoice, you need to understand how to read it!

## What's Next?

Continue "Using Lightning" with [§20.2: Paying_a_Invoice](20_2_Paying_a_Invoice.md).
