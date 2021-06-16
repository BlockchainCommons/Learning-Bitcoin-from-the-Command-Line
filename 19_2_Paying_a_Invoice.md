# 19.2: Paying an Invoice

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

In this chapter you will learn how to pay an invoice using `lightning-cli pay` command. It assumes that you've already looked over the invoice, per [§19.1](19_1_Generate_a_Payment_Request.md) and determined it was valid.

## Check your Balance

Obviously, the first thing you need to do is make sure that you have enough funds to pay an invoice. In this case, the channel set up previously with `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543` contains 100,000 satoshis. This will be the channel used to pay the invoice.

```
c$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "output": 1,
         "value": 99847,
         "amount_msat": "99847000msat",
         "scriptpubkey": "00142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c",
         "address": "tb1q9lszuklf9qlgck7tjwhxzssm47xtvnuu4jslf8",
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1862856x29x0",
         "channel_sat": 100000,
         "our_amount_msat": "100000000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}
```
If you didn't have enough funds, you'd need to create a new channel.

## Pay Your Invoice

You use `lightning-cli pay` command to pay an invoice. It will attempt to find a route to the given destination and send the funds requested. Here that's very simple because there's a direct channel between the payer and the recipient:
```
c$ lightning-cli --testnet pay lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
{
   "destination": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "payment_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
   "created_at": 1602704828.948,
   "parts": 1,
   "msatoshi": 10000000,
   "amount_msat": "10000000msat",
   "msatoshi_sent": 10000000,
   "amount_sent_msat": "10000000msat",
   "payment_preimage": "1af4a9bb830e49b6bc8f0bef980630e189e3794ad1705f06ad1b9c71571dce0c",
   "status": "complete"
}
```
Note that here all the amounts are in `msats`, not `sats`!

### Pay Your Invoice Across the Network

However, you do _not_ need to have a channel with a node in order to pay them. There just needs to be a reasonable route across the Lightning Network.

Imagine that you received this teeny payment request for 11,111 msat:
```
c$ lightning-cli --testnet decodepay lntb111110p1p0cw43ppp5u0ngjytlw6ywec3x784jale4xd7h058g9u4mthcaf9rl2f7g8zxsdp2t9hh2gr0wajjqmt9ypnx7u3qv35kumn9wgs8gmm0yyxqyjw5qcqp2sp5kj4xhrthmfgcgyl84zaqpl9vvdjwm5x368kr09fu5nym74setw4s9qy9qsq8hxjr73ee77vat0ay603e4w9aa8ag9sa2n55xznk5lsfrjffxxdj2k0wznvcfa98l4a57s80j7dhg0cc03vwqdwehkujlzxgm0xyynqqslwhvl
{
   "currency": "tb",
   "created_at": 1602704929,
   "expiry": 604800,
   "payee": "02f3d74746934494fa378235e5bc44cfdbb5b8779d839263fb7f9218be032f6f61",
   "msatoshi": 11111,
   "amount_msat": "11111msat",
   "description": "You owe me for dinner too!",
   "min_final_cltv_expiry": 10,
   "payment_secret": "b4aa6b8d77da518413e7a8ba00fcac6364edd0d1d1ec37953ca4c9bf56195bab",
   "features": "028200",
   "payment_hash": "e3e689117f7688ece226f1eb2eff35337d77d0e82f2bb5df1d4947f527c8388d",
   "signature": "304402203dcd21fa39cfbcceadfd269f1cd5c5ef4fd4161d54e9430a76a7e091c929319b02202559ee14d984f4a7fd7b4f40ef979b743f187c58e035d9bdb92f88c8dbcc424c"
}
```
If you tried to pay it, and you didn't have a route to the recipient through the Lightning Network, you could expect an error like this:
```
c$ lightning-cli --testnet pay lntb111110p1p0cw43ppp5u0ngjytlw6ywec3x784jale4xd7h058g9u4mthcaf9rl2f7g8zxsdp2t9hh2gr0wajjqmt9ypnx7u3qv35kumn9wgs8gmm0yyxqyjw5qcqp2sp5kj4xhrthmfgcgyl84zaqpl9vvdjwm5x368kr09fu5nym74setw4s9qy9qsq8hxjr73ee77vat0ay603e4w9aa8ag9sa2n55xznk5lsfrjffxxdj2k0wznvcfa98l4a57s80j7dhg0cc03vwqdwehkujlzxgm0xyynqqslwhvl
{
   "code": 210,
   "message": "Ran out of routes to try after 11 attempts: see `paystatus`",
   "attempts": [
      {
         "status": "failed",
         "failreason": "Error computing a route to 02f3d74746934494fa378235e5bc44cfdbb5b8779d839263fb7f9218be032f6f61: \"Could not find a route\" (205)",
         "partid": 1,
         "amount": "11111msat"
      },
...
```
But what if a host that you had a channel with opened a channel with the intended recipient?

In that case, when you go to pay the invoice, it will _automatically work_!
```
c$ lightning-cli --testnet pay lntb111110p1p0cw43ppp5u0ngjytlw6ywec3x784jale4xd7h058g9u4mthcaf9rl2f7g8zxsdp2t9hh2gr0wajjqmt9ypnx7u3qv35kumn9wgs8gmm0yyxqyjw5qcqp2sp5kj4xhrthmfgcgyl84zaqpl9vvdjwm5x368kr09fu5nym74setw4s9qy9qsq8hxjr73ee77vat0ay603e4w9aa8ag9sa2n55xznk5lsfrjffxxdj2k0wznvcfa98l4a57s80j7dhg0cc03vwqdwehkujlzxgm0xyynqqslwhvl
{
   "destination": "02f3d74746934494fa378235e5bc44cfdbb5b8779d839263fb7f9218be032f6f61",
   "payment_hash": "e3e689117f7688ece226f1eb2eff35337d77d0e82f2bb5df1d4947f527c8388d",
   "created_at": 1602709081.324,
   "parts": 1,
   "msatoshi": 11111,
   "amount_msat": "11111msat",
   "msatoshi_sent": 12111,
   "amount_sent_msat": "12111msat",
   "payment_preimage": "ec7d1b28a7b877cd92b83be396899e8bfc3ecb0b4f944f65afb4be7d0ee72617",
   "status": "complete"
}
```
That's the true beauty of the Lightning Network there: with no effort from the peer-to-peer participants, their individual channels become a network!

> :book: ***How Do Payments Work Across the Network?*** Say that node A has a channel open with node B, node B has a channel open with node C, and node A receives an invoice from node C for 11,111 msat. Node A pays node B the 11,111 msat, plus a small fee, and then node B pays the 11,111 msat to node C. Easy enough. Except remember that all channels actually are just records of who owns how much of the Funding Transaction. So what really happens is 11,111 msat of the Funding Transaction on channel A-B shifts from A to B, and then 11,111 msat of the Funding Transaction on channel B-C shifts from B to C. This means that two things are required for this payment to work: first, each channel must have sufficient capacity for the payment; and second, the payer on each channel must own enough of the capacity to make the payment.  

Note that in this example, 12,111 msat were sent to pay an invoice of 11,111 msat: the extra being a very small, flat fee (not a percentage) that was paid to the intermediary.

## Check your Balance

Having successfully made a payment, you should see that your funds have changed accordingly.

Here's what funds looked like for the paying node following the initial payment of 10,000 satoshis:
```
c$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "output": 1,
         "value": 99847,
         "amount_msat": "99847000msat",
         "scriptpubkey": "00142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c",
         "address": "tb1q9lszuklf9qlgck7tjwhxzssm47xtvnuu4jslf8",
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1862856x29x0",
         "channel_sat": 90000,
         "our_amount_msat": "90000000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}
```
Note that the channel capacity remains at 100,000 satoshis (it never changes!), but that `our_amount` is now just 90,000 satoshis (or 90,000,000 msat).

After paying the second invoice, for 11,111 msat, the funds change again accordingly:
```
$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "output": 1,
         "value": 99847,
         "amount_msat": "99847000msat",
         "scriptpubkey": "00142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c",
         "address": "tb1q9lszuklf9qlgck7tjwhxzssm47xtvnuu4jslf8",
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1862856x29x0",
         "channel_sat": 89987,
         "our_amount_msat": "89987000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}
```
`our_amount` is now just 89,987 satoshis, having paid 11,111 msat plus a 1,000 msat fee.

## Summary: Paying a Invoice

Once you've got an invoice, it's easy enough to pay with a single command in Lightning. Even if you don't have a channel to a recipient, payment is that simple, provided that there's a route between you and the destination node.

## What's Next?

Continue "Using Lighting" with [§19.3: Closing a Channel](19_3_Closing_a_Channel.md).
