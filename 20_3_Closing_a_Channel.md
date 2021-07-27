# 20.3: Closing a Channel

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

In this chapter you'll learn how to close a channel using `lightning-cli close` command-line interface. Closing a channel means you and your counterparty will send their agreed-upon channel balance to the blockchain, whereby you must pay blockchain transaction fees and wait for the transaction to be mined. A closure can be cooperative or non-cooperative, but it works either way.

In order to close a channel, you first need to know the ID of the remote node; you can retrieve it in one of two ways. 

## Find your Channels by Funds

You can use the `lightning-cli listfunds` command to see your channels.  This RPC command displays all funds available, either in unspent `outputs` (UTXOs) in the internal wallet or locked up in currently open `channels`.
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
         "channel_sat": 89987,
         "our_amount_msat": "89987000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}

  "message_flags": 1,
  "channel_flags": 2,
  "active": true,
  "last_update": 1595508075,
  "base_fee_millisatoshi": 1000,
  "fee_per_millionth": 1,
  "delay": 40,
  "htlc_minimum_msat": "1000msat",
  "htlc_maximum_msat": "280000000msat",
  "features": ""
}
```


You could retrieve the ID of the 0th channel into a variable like this:
```
c$ nodeidremote=$(lightning-cli --testnet listfunds | jq '.channels[0] | .peer_id')
```

## Find your Channels with JQ

The other way to find channels to close is to the use the `listchannels` command. It returns data on channels that are known to the node. Because channels may be bidirectional, up to two nodes will be returned for each channel (one for each direction). 

However, Lightning's gossip network is very effective, and so in a short time you will come to know about thousands of channels. That's great for sending payments across the Lightning Network, but less useful for discovering your own channels. To do so requires a bit of `jq` work.

First, you need to know your own node ID, which can be retrieved with `getinfo`:
```
c$ nodeid=$(lightning-cli --testnet getinfo | jq .id)
c$ echo $nodeid
"03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687"
c$
```
You can then use that to look through `listchannels` for any channels where your node is either the source or the destination:
```
c$ lightning-cli --testnet listchannels | jq '.channels[] | select(.source == '$nodeid' or .destination == '$nodeid')'
{
  "source": "03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687",
  "destination": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
  "short_channel_id": "1862856x29x0",
  "public": true,
  "satoshis": 100000,
  "amount_msat": "100000000msat",
  "message_flags": 1,
  "channel_flags": 0,
  "active": true,
  "last_update": 1602639570,
  "base_fee_millisatoshi": 1,
  "fee_per_millionth": 10,
  "delay": 6,
  "htlc_minimum_msat": "1msat",
  "htlc_maximum_msat": "99000000msat",
  "features": ""
}
```
There's our old favorite `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543` again, as the destination.

Once you know what you've got, you can store it in a variable:
```
c$ nodeidremote=$(lightning-cli --testnet listchannels | jq '.channels[] | select(.source == '$nodeid' or .destination == '$nodeid') | .destination')
```

## Close a Channel

Now that you have a remote node ID, you're ready to use the `lightning-cli close` command to close a channel. By default, it will attempt to close the channel cooperatively with the peer;  if you want to close it unilaterally set the `unilateraltimeout` argument with the number of seconds to wait. (If you set it to 0 and the peer is online, a mutual close is still attempted.) For this example, you will attempt a mutual close.

```
c$ lightning-cli --testnet close $nodeidremote 0
{
   "tx": "02000000011d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d69660000000000ffffffff021c27000000000000160014d39feb57a663803da116402d6cb0ac050bf051d9cc5e01000000000016001451c88b44420940c52a384bd8a03888e3676c150900000000",
   "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
   "type": "mutual"
}
```
The closing transaction on-chain is [f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f](https://blockstream.info/testnet/tx/f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f).

It's this closing transaction that actually disburses the funds that were traded back and forth through Lightning transactions. This can be seen by examining the transaction:
```
$ bitcoin-cli --named getrawtransaction txid=f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f verbose=1
{
  "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
  "hash": "3a6b3994932ae781bab80e159314bad06fc55d3d33453a1d663f9f9415c9719c",
  "version": 2,
  "size": 334,
  "vsize": 169,
  "weight": 673,
  "locktime": 0,
  "vin": [
    {
      "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "",
        "304402207f8048e29192ec86019bc83be8b4cac5d1fc682374538bed0707f58192d41c390220512ebcde122d53747feedd70c09153a40c56d09a5fec02e47642afdbb20aa2ac01",
        "3045022100d686a16084b60800fa0f6b14c25dca1c13d10a55c5fb7c6a3eb1c5f4a2fb20360220555f5b6e672cf9ef82941f7d46ee03dd52e0e848b9f094a41ff299deb8207cab01",
        "522102f7589fd8366252cdbb37827dff65e3304abd5d17bbab57460eff71a9e32bc00b210343b980dff4f2723e0db99ac72d0841aad934b51cbe556ce3a1b257b34059a17052ae"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00010012,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 d39feb57a663803da116402d6cb0ac050bf051d9",
        "hex": "0014d39feb57a663803da116402d6cb0ac050bf051d9",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q6w07k4axvwqrmggkgqkkev9vq59lq5we5fcrzn"
        ]
      }
    },
    {
      "value": 0.00089804,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 51c88b44420940c52a384bd8a03888e3676c1509",
        "hex": "001451c88b44420940c52a384bd8a03888e3676c1509",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q28ygk3zzp9qv223cf0v2qwygudnkc9gfp30ud4"
        ]
      }
    }
  ],
  "hex": "020000000001011d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d69660000000000ffffffff021c27000000000000160014d39feb57a663803da116402d6cb0ac050bf051d9cc5e01000000000016001451c88b44420940c52a384bd8a03888e3676c1509040047304402207f8048e29192ec86019bc83be8b4cac5d1fc682374538bed0707f58192d41c390220512ebcde122d53747feedd70c09153a40c56d09a5fec02e47642afdbb20aa2ac01483045022100d686a16084b60800fa0f6b14c25dca1c13d10a55c5fb7c6a3eb1c5f4a2fb20360220555f5b6e672cf9ef82941f7d46ee03dd52e0e848b9f094a41ff299deb8207cab0147522102f7589fd8366252cdbb37827dff65e3304abd5d17bbab57460eff71a9e32bc00b210343b980dff4f2723e0db99ac72d0841aad934b51cbe556ce3a1b257b34059a17052ae00000000",
  "blockhash": "000000000000002a214b1ffc3a67c64deda838dd24d12154c15d3a6f1137e94d",
  "confirmations": 1,
  "time": 1602713519,
  "blocktime": 1602713519
}
```
The input of the transaction is `66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d`, which was the funding transaction in [§19.3](19_3_Setting_Up_a_Channel.md). The transaction then has two outputs, one for the remote node and the other for the local c-lightning wallet. The output on index 0 corresponds to the remote node with a value of 0.00010012 BTC; and the output on index 1 corresponds to the local node with a value of 0.00089804 BTC.

Lightning will similarly show 89804 satoshis returned as a new UTXO in its wallet:
```
$ lightning-cli --network=testnet listfunds
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
      },
      {
         "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
         "output": 1,
         "value": 89804,
         "amount_msat": "89804000msat",
         "scriptpubkey": "001451c88b44420940c52a384bd8a03888e3676c1509",
         "address": "tb1q28ygk3zzp9qv223cf0v2qwygudnkc9gfp30ud4",
         "status": "confirmed",
         "blockheight": 1863006,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": false,
         "state": "ONCHAIN",
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

### Understand the Types of Closing Channels.

The `close` RPC command attempts to close a channel cooperatively with its peer or unilaterally after the `unilateraltimeout` argument expires. This bears some additional discussion, as it goes to the heart of Lightning's trustless design:

Each participant of a channel is able to create as many Lightning payments to their counterparty as their funds allow.  Most of the time there will be no disagreements between the participants, so there will only be two on-chain transactions, one opening and the other closing the channel. However, there may be scenarios in which one peer is not online or does not agree with the final state of the channel or where someone tries to steal funds from the other party. This is why there are both cooperative and forced closes.

#### Cooperative Close

In the case of a cooperative close, both channel participants agree to close the channel and settle the final state to the blockchain. Both participants must be online; the close is performed by broadcasting an unconditional spend of the funding transaction with an output to each peer. 

#### Force Close

In the case of a force close, only one participant is online or the participants disagree on the final state of the channel. In this situation, one peer can perform an unilateral close of the channel without the cooperation of the other node.  It's performed by broadcasting a commitment transaction that commits to a previous channel state that both parties have agreed upon. This commitment transaction contains the channel state divided in two parts: the balance for each participant and all the pending payments (HTLCs).

To perform this kind of close, you must specify an `unilateraltimeout` argument. If this value is not zero, the close command will unilaterally close the channel when that number of seconds is reached:
```
c$ lightning-cli --network=testnet close $newidremote 60
{
   "tx": "0200000001a1091f727e6041cc93fead2ea46b8402133f53e6ab89ab106b49638c11f27cba00000000006a40aa8001df85010000000000160014d22818913daf3b4f86e0bcb302a5a812d1ef6b91c6772d20",
   "txid": "02cc4c647eb3e06f37fcbde39871ebae4333b7581954ea86b27b85ced6a5c4f7",
   "type": "unilateral"
}

```
## Summary: Closing a Channel

When you close a channel you perform an on-chain transaction ending your financial relationship with the remote node  To close a channel, you must take into account its status and the type of closure you want to execute.

## What's Next?

Continue "Using Lightning" with [§20.4: Expanding the Lightning Network](20_4_Lightning_Network_Review.md).
