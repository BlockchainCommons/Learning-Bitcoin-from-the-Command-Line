
# 13.4: Generate a Payment request.

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This chapter shows what is and how to create a payment request.    For this example we've created a second lightning node using LND implementation to create an invoice to be payed later using our main c-lighting installation in next chapter.  We will refer to this instance using `lnd$ ` prompt shell to indicate place where we'll generate LND commands.   If you want to reproduce this steps you should use Bitcoin Standup to create a second machine and install LND.

> :book: ***What is an invoice

Almost all payments made on the Lightning Network require an invoice, which is nothing more than a **request for payment** made by the recipient of the money and sent by different means to the user who will pay.  The format for a Lightning invoice uses bech32 encoding, which is already used for Bitcoin Segregated Witness and all payment requests are single use.

An invoice is made up of two parts,  one is human readable and other is 

- Human readable part: `ln` + (`lnbc` for Bitcoin mainnet, `lntb` for Bitcoin testnet, and `lnbcrt` for Bitcoin regtest) + data amount
      
- Data part : UNIX Timestamp + tagged parts include a payment hash, the pubkey of the payee node, an optional description of the payment, an expiration time, and some extra routing information.


### Node information

In this second instance we refer before you may get information about it using `lnd$lncli -n testnet getinfo` command.

```
lnd$ lncli -n testnet getinfo
{
    "version": "0.10.99-beta commit=clock/v1.0.0-171-g8cb1276dbf0bfd9fcbf599df87a43238e599eaac",
    "commit_hash": "8cb1276dbf0bfd9fcbf599df87a43238e599eaac",
    "identity_pubkey": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
    "alias": "0302d48972ba7eef8b40",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 1,
    "num_inactive_channels": 0,
    "num_peers": 3,
    "block_height": 1781444,
    "block_hash": "0000000018655b27a1801e976c800be99c6f42c9bfcf122695a7ef5c0fa12e73",
    "best_header_timestamp": "1595509101",
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
        "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84@192.168.0.22:9736"
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

### Generate a Payment request.

Now you can create a payment request using using `lnd$lncli -n testnet addinvoice` command.    You can use --amt argument to indicate amount to be payed and add a description using --memo argument.

```
lnd$ lncli -n testnet addinvoice --amt 100000 --memo "First LN Payment - Learning Bitcoin and Lightning from the Command line."
{
    "r_hash": "72fbf5c2baadc780b17a27d983bd685f7d6ddf682b028c6dc13a50976e9e6e6c",
    "payment_request": "lntb1m1p03ft7lpp5zve4dsgwgdxekqqq39vhgcnv6gfa2g2ktqy9lf0aph60d0388xmqdqqcqzpgsp545a9fphd8m5ayplcu8m5845cr4m0zcnyxddwv4g3zm32yprkfd4q9qy9qsq3s4y6cmyvh0qw9qm0sf80llxyyjy9xwrjds7lpkqhzv247jsm6q5me8t9e6ftquma664gz5u4a2rvs0yf4f0mlwtwfs6as5uj5djzhcqpnqlcj",
    "add_index": "1"
}
```

## Summary: Generating a Payment request.

In most cases you need to receive an invoice to use Lightning Network payments. In this example we've created on manually but in real cases you can buy products and services that will generate a respective invoice for it.

## What's Next?

Continue "Understanding Your Lightning Setup" with [§13.5: Paying_a_Invoice](13_5_Paying_a_Invoice.md).




