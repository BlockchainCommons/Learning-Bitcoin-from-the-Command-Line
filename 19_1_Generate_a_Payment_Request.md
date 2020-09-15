
# 19.1: Generate a Payment request.

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This chapter shows what is and how to create a payment request.  We will explain briefly how works payments on the Lightning Network.
For this example we've created a second lightning node using LND implementation to create an invoice to be payed later using our main c-lighting installation in next chapter.  We will refer to this instance using `lnd$ ` prompt shell to indicate place where we'll generate LND commands.   If you want to reproduce this steps you should use Bitcoin Standup to create a second machine and install LND.

> :book: ***What is an invoice

Almost all payments made on the Lightning Network require an invoice, which is nothing more than a **request for payment** made by the recipient of the money and sent by different means to the user who will pay.  The format for a Lightning invoice uses bech32 encoding, which is already used for Bitcoin Segregated Witness and all payment requests are single use.

An invoice is made up of two parts,  one is human readable and other is data part.

- Human readable part: `ln` + (`lnbc` for Bitcoin mainnet, `lntb` for Bitcoin testnet, and `lnbcrt` for Bitcoin regtest) + data amount
      
- Data part : UNIX Timestamp + tagged parts include a payment hash, the pubkey of the payee node, an optional description of the payment, an expiration time, and some extra routing information.

1. Destination: The public key of the person receiving the LN payment.
2. payment_hash: The hash of the preimage that is used to lock the payment. You can only redeem a locked payment with the corresponding preimage to the payment hash. This enables routing on the Lightning Network without trusted third parties creating a conditional payment to be filled.
3. num_satoshis: The amount of satoshis to be paid.
4. expiry: Time when your node marks the invoice as invalid. Default is 1 hour or 3600 seconds.
5. timestamp: Time when the invoice was created.  This is measured in seconds since 1970.
6. cltv_expiry: Delta to use for time-lock of CLTV extended to the final hop.

> :book: ***Conditional payments.

As we said in Setting a channel chapter and although the channels are created between two participants, the channels can be connected to each other forming a payment network to allow payments between all the network participants without having a direct channel between them using an smart contract called **Hashed Time Locked Contract**.

> :book: ***Hashed Time Locked Contract.

A Hashed Time Locked Contract is a conditional payment that use hashlocks and timelocks to ensure payment security. This means that receiver must presents a payment preimage or generate a cryptographic proof of payment before a given time otherwise the payer can cancel the contract by spending it.   This contracts are created as outputs from the Commitment transaction.

> :book: ***Commitment transaction.

Commitment transaction is a transaction that spends the funding transaction. Each peer holds the other peer's signature meaning that either one can spent his commitment transaction whatever he wants.  After each new commitment transaction is created the old one is revoked.

### Node information

In this second instance that we refer before you may get information about it using `lnd$lncli -n testnet getinfo` command.   Remember identity_pubkey field,  due it will be the same value when you pay the invoice. 

**"identity_pubkey": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84"**

> :warning: **WARNING:**   Notice this example is using an LND instance,   if you want to reproduce this steps you should use Bitcoin Standup Script and install a new LND instance.

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

To create a new invoice on c-lightning you should use invoice `lightning-cli --network=testnet invoice` command.
It receives amount expresed on milisatoshis and description invoices texts.

```
c$ lightning-cli --network=testnet invoice 100000 test22 test22
{
   "payment_hash": "743487bca87bb39a8f07314864e80d02f7686d97693ada32c69ee3a5e06132f7",
   "expires_at": 1600684714,
   "bolt11": "lntb1u1p047jp2pp5ws6g009g0wee4rc8x9yxf6qdqtmksmvhdyad5vkxnm36tcrpxtmsdq2w3jhxapjxgxqyjw5qcqp2sp5s3pyvvknhnaej6ukssvxlwc4sdqpcc2prrwue8jreke587ms68qq9qy9qsqhf5uhxk0mfw3sl87ukx8e5d757pkla6zlyu07p0wc666juzm5unhlaxeqmnl8f7v7vs3wz2thme9szs3f7whzr9uewz0kexqkl9jxgsq5r752l",
   "warning_offline": "No channel with a peer that is currently connected has sufficient incoming capacity"
}
```

For this example we'll create a payment request using `lnd$lncli -n testnet addinvoice` command.    You can use --amt argument to indicate amount to be payed and add a description using --memo argument.

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

Continue "Understanding Your Lightning Setup" with [§19.2: Paying_a_Invoice](19_2_Paying_a_Invoice.md).




