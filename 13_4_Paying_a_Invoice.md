# 13.4: Paying a Invoice

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

> :book: ***What is an invoice

Almost all payments made on the Lightning Network require an invoice, which is nothing more than a request for payment made by the recipient of the money and sent by different means to the user who will pay.  The format for a Lightning invoice uses bech32 encoding, which is already used for Bitcoin Segregated Witness and all payment requests are single use.

Invoices have two parts:

- Human readable part: `ln` + (`lnbc` for Bitcoin mainnet, `lntb` for Bitcoin testnet, and `lnbcrt` for Bitcoin regtest) + data amount
      
- Data part : UNIX Timestamp + tagged parts include a payment hash, the pubkey of the payee node, an optional description of the payment, an expiration time, and some extra routing information.

Conside this ln invoice 

`lntb1m1p03ft7lpp5zve4dsgwgdxekqqq39vhgcnv6gfa2g2ktqy9lf0aph60d0388xmqdqqcqzpgsp545a9fphd8m5ayplcu8m5845cr4m0zcnyxddwv4g3zm32yprkfd4q9qy9qsq3s4y6cmyvh0qw9qm0sf80llxyyjy9xwrjds7lpkqhzv247jsm6q5me8t9e6ftquma664gz5u4a2rvs0yf4f0mlwtwfs6as5uj5djzhcqpnqlcj
`  

This invoice starts with prefix ln+tb+1m that indicates milli, so you should multi by 0.001.   That means we'll pay an invoice by 100000 satoshis.

### Understanding your invoice

If you want to decode your lightning invoice you should use `lightning-cli decodepay ` command that receives a string and checks and parses as specified by the [BOLT 11 specification](https://github.com/lightningnetwork/lightning-rfc/blob/master/11-payment-encoding.md).

```
c$lightning-cli --network=testnet decodepay lntb1m1p03ft7lpp5zve4dsgwgdxekqqq39vhgcnv6gfa2g2ktqy9lf0aph60d0388xmqdqqcqzpgsp545a9fphd8m5ayplcu8m5845cr4m0zcnyxddwv4g3zm32yprkfd4q9qy9qsq3s4y6cmyvh0qw9qm0sf80llxyyjy9xwrjds7lpkqhzv247jsm6q5me8t9e6ftquma664gz5u4a2rvs0yf4f0mlwtwfs6as5uj5djzhcqpnqlcj
{
   "currency": "tb",
   "created_at": 1595191263,
   "expiry": 3600,
   "payee": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
   "msatoshi": 100000000,
   "amount_msat": "100000000msat",
   "description": "First LN Payment - Learning Bitcoin and Lightning from the Command line.",
   "min_final_cltv_expiry": 40,
   "payment_secret": "ad3a5486ed3ee9d207f8e1f743d6981d76f16264335ae6551116e2a204764b6a",
   "features": "028200",
   "payment_hash": "133356c10e434d9b0000895974626cd213d5215658085fa5fd0df4f6be2739b6",
   "signature": "30450221008c2a4d636465de07141b7c1277ffe621244299c39361ef86c0b898aafa50de8102204de4eb2e7495839beeb5540a9caf543641e44d52fdfdcb7261aec29c951b215f"
}
```
### Check your balance

As we did in previous chapter we have established an channel with a total capacity of 280.000 sats.

```
c$ lightning-cli --network=testnet listfunds
{
   "outputs": [
      {
         "txid": "9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6",
         "output": 0,
         "value": 19238,
         "amount_msat": "19238000msat",
         "scriptpubkey": "0014aa572371f29310cd677d039cdcd054156c1a9545",
         "address": "tb1q4ftjxu0jjvgv6emaqwwde5z5z4kp49299gmdpd",
         "status": "confirmed",
         "blockheight": 1780768,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
         "connected": true,
         "state": "CHANNELD_AWAITING_LOCKIN",
         "channel_sat": 280000,
         "our_amount_msat": "280000000msat",
         "channel_total_sat": **280000**,
         "amount_msat": "280000000msat",
         "funding_txid": "9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6",
         "funding_output": 1
      }
   ]
}
```

### Paying your invoice

You should use `lightning-cli pay ` command to pay the invoice.  The pay RPC command attempts to find a route to the given destination, and send the funds asked in amount value.

```
c$lightning-cli --network=testnet pay 
lntb1m1p03ft7lpp5zve4dsgwgdxekqqq39vhgcnv6gfa2g2ktqy9lf0aph60d0388xmqdqqcqzpgsp545a9fphd8m5ayplcu8m5845cr4m0zcnyxddwv4g3zm32yprkfd4q9qy9qsq3s4y6cmyvh0qw9qm0sf80llxyyjy9xwrjds7lpkqhzv247jsm6q5me8t9e6ftquma664gz5u4a2rvs0yf4f0mlwtwfs6as5uj5djzhcqpnqlcj
{
   "id": 1,
   "payment_hash": "133356c10e434d9b0000895974626cd213d5215658085fa5fd0df4f6be2739b6",
   "destination": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
   "msatoshi": 100000000,
   "amount_msat": "100000000msat",
   "msatoshi_sent": 100000000,
   "amount_sent_msat": "100000000msat",
   "created_at": 1595191379,
   "status": "complete",
   "payment_preimage": "8b7549ffd4e648374669d47992c1ab44086501d5dd85d7e3d15766d70e9196e3",
   "bolt11": "lntb1m1p03ft7lpp5zve4dsgwgdxekqqq39vhgcnv6gfa2g2ktqy9lf0aph60d0388xmqdqqcqzpgsp545a9fphd8m5ayplcu8m5845cr4m0zcnyxddwv4g3zm32yprkfd4q9qy9qsq3s4y6cmyvh0qw9qm0sf80llxyyjy9xwrjds7lpkqhzv247jsm6q5me8t9e6ftquma664gz5u4a2rvs0yf4f0mlwtwfs6as5uj5djzhcqpnqlcj"
}
```

### Check your balance

We've performed payment successfully

```
c$ lightning-cli --network=testnet listfunds
{
   "outputs": [
      {
         "txid": "9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6",
         "output": 0,
         "value": 19238,
         "amount_msat": "19238000msat",
         "scriptpubkey": "0014aa572371f29310cd677d039cdcd054156c1a9545",
         "address": "tb1q4ftjxu0jjvgv6emaqwwde5z5z4kp49299gmdpd",
         "status": "confirmed",
         "blockheight": 1780768,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1780768x12x1",
         "channel_sat": 180000,
         "our_amount_msat": "180000000msat",
         "channel_total_sat": 280000,
         "amount_msat": "280000000msat",
         "funding_txid": "9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6",
         "funding_output": 1
      }
   ]
}
```
