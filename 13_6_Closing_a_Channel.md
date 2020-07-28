# 13.6: Closing a Channel

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

In this chapter you'll be able to close a channel and learn what it means and how to do it using `lightning-cli close` command-line interface.   The close RPC command attempts to close the channel cooperatively with the peer, or unilaterally after unilateraltimeout argument expires measured in seconds and the output will be sent to the address controlled by your c-lightning wallet if you don't specify one.

Close a channel means you and your counterparty will send their agreed-upon channel balance to the blockchain whereby you must pay transaction fees and must wait for the transaction to be mined.

### Listing your channels.

You can use the `lightning-cli listfunds` command to see your channels.  This RPC command displays all funds available, either in unspent outputs (UTXOs) in the internal wallet or funds locked in currently open channels.

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
         "connected": false,
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

## Closing a channel

You should use  `lightning-cli close` command to close the channel.  This RPC command attempts to close the channel cooperatively with the peer, or unilaterally after unilateraltimeout expires, and the to-local output will be sent to the address specified in destination.

### Types of Closing Channels.

Each participant of the channel is able to create as many Lightning payments to their counterparty as their funds they have.  Most of the time there will be no disagreements between the participants, so there will only be two onchain transactions, one opening and the other closing the channel.   However, there may be other scenarios in which you are not online, you do not agree with the last state of the channel or someone tries to steal funds from the other party.

#### Cooperative Close

In this case both channel participants agree to close the channel and settle the final state to the blockchain. Both participants must be online and it's performed by broadcasting an unconditional spend of the funding transaction with an output to each peer. 

#### Force Close

In this case when only one participant is online or if the participants disagree on the last state of the channel,  so one peer can perform an unilateral close of the channel without the cooperation of the other node.   It's performed by broadcasting a commitment transaction that commits to a previous channel state which both parts have agreed upon.
This commitment transaction contains the channel state divided in two parts: the balance of each participant and all the pending payments (HTLCs).

### Node Information

Now we'll show you how to get information about your channel using `lightning-cli listchannels` command.  The listchannels RPC command returns data on channels that are known to the node. Because channels may be bidirectional, up to 2 objects will be returned for each channel (one for each direction).   To query information about own channels we'll use jq tool showed in previous chapters.

First we'll get our own node id public_key in NODEID variable.

```
c$ NODEID=$(lightning-cli --network=testnet getinfo | jq .id)
c$ echo $NODEID
"03fce2a20393a65b9d6cab5425f4cd33ddc621ade458efd69d652917e2b5eaf59c"
c$
```
Later we'll use select to show only data containing public_key id as source or destination.

```
$ lightning-cli listchannels | jq '.channels[] | select(.source == '$NODEID' or .destination == '$NODEID')'
{
  "source": "03fce2a20393a65b9d6cab5425f4cd33ddc621ade458efd69d652917e2b5eaf59c",
  "destination": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
  "short_channel_id": "1780768x12x1",
  "public": true,
  "satoshis": 280000,
  "amount_msat": "280000000msat",
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
### Closing a channel

Finally you should use `lightning-cli close` command to close the channel. The close RPC command attempts to close the channel cooperatively with the peer,  if you want to close it unilaterally set unilateraltimeout argument with number of seconds command will wait.   If you set to 0 and the peer is online command can negotiate a mutual close.   For this example we use an mutual close.

```
lightning-cli --network=testnet close 0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84 0
{
   "tx": "0200000001a67ad9b15cb10b74a584a284f059866dd2114e45f2a997b260464af537c043980100000000ffffffff02a08601000000000016001404e34b25e1310c9b90c7a53a6eba88f4eefe8efb69be020000000000160014865353eaccaa94aa4f90d3a0acdf3903c06c12c400000000",
   "txid": "b4c0a1993dd113081eff5369a22d6afe1af9f0d07b29a590e8772ac7f712736a",
   "type": "mutual"
}
```
The closing transaction onchain is [b4c0a1993dd113081eff5369a22d6afe1af9f0d07b29a590e8772ac7f712736a](https://blockstream.info/testnet/tx/b4c0a1993dd113081eff5369a22d6afe1af9f0d07b29a590e8772ac7f712736a).

This transaction has two outputs, one for remote node and other for local c-lightning wallet.   Output on index 0 corresponds to remote node with a value of 100000.   Output on index 1 correspond to local node with a value of 179817.

```
c$ bitcoin-cli -testnet getrawtransaction b4c0a1993dd113081eff5369a22d6afe1af9f0d07b29a590e8772ac7f712736a 1
{
  "txid": "b4c0a1993dd113081eff5369a22d6afe1af9f0d07b29a590e8772ac7f712736a",
  "hash": "9433409227ca3d7a6999cdcc2856272314aef96f0e869a04711eda4969bbd21f",
  "version": 2,
  "size": 334,
  "vsize": 169,
  "weight": 673,
  "locktime": 0,
  "vin": [
    {
      "txid": "9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "",
        "3045022100ef40a71fc4d4d0e197cd3d503961da90b71cedab25f6f30740b3640664efb617022061f68aaffccf739824668d07519c8cb59ec30117d87beff2ef217e31cb5e628801",
        "304402201a28274f64ec78fecba74ebc2b13582564ab155f83593c65ca1667bb92c42de10220489e7c3adc5be8bac2c5771482e5571abd3f602cb40ece2cfea3b768eb5341ea01",
        "52210364d9b2e600d837aad224702c5f30c0ab73ac499cb67f43899cbf83f6358d422c21036695eadb796fe140434aad02c7da76395a44f62747770bf381fa4a3c52ff279452ae"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00100000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 04e34b25e1310c9b90c7a53a6eba88f4eefe8efb",
        "hex": "001404e34b25e1310c9b90c7a53a6eba88f4eefe8efb",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qqn35kf0pxyxfhyx855axaw5g7nh0arhmxyv3zk"
        ]
      }
    },
    {
      "value": 0.00179817,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 865353eaccaa94aa4f90d3a0acdf3903c06c12c4",
        "hex": "0014865353eaccaa94aa4f90d3a0acdf3903c06c12c4",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qsef486kv42225nus6ws2eheeq0qxcykycqsymn"
        ]
      }
    }
  ],
  "hex": "02000000000101a67ad9b15cb10b74a584a284f059866dd2114e45f2a997b260464af537c043980100000000ffffffff02a08601000000000016001404e34b25e1310c9b90c7a53a6eba88f4eefe8efb69be020000000000160014865353eaccaa94aa4f90d3a0acdf3903c06c12c40400483045022100ef40a71fc4d4d0e197cd3d503961da90b71cedab25f6f30740b3640664efb617022061f68aaffccf739824668d07519c8cb59ec30117d87beff2ef217e31cb5e62880147304402201a28274f64ec78fecba74ebc2b13582564ab155f83593c65ca1667bb92c42de10220489e7c3adc5be8bac2c5771482e5571abd3f602cb40ece2cfea3b768eb5341ea014752210364d9b2e600d837aad224702c5f30c0ab73ac499cb67f43899cbf83f6358d422c21036695eadb796fe140434aad02c7da76395a44f62747770bf381fa4a3c52ff279452ae00000000"
}
```

Listing funds onchain or offchain we get an output with a value of 179817 that results of 280000 minus 183 per fee in 279817.   We have to substract 100000 paid on the invoice of the previous chapter to finally receives 179817 satoshis.

```
lightning-cli --network=testnet listfunds
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
      },
      {
         "txid": "b4c0a1993dd113081eff5369a22d6afe1af9f0d07b29a590e8772ac7f712736a",
         "output": 1,
         "value": 179817,
         "amount_msat": "179817000msat",
         "scriptpubkey": "0014865353eaccaa94aa4f90d3a0acdf3903c06c12c4",
         "address": "tb1qsef486kv42225nus6ws2eheeq0qxcykycqsymn",
         "status": "confirmed",
         "blockheight": 1781830,
         "reserved": false
      }
}
```

## Summary: Closing a channel

When you close a channel you perform an onchain transaction ending your financial relationship with remote node.   To close a channel you must take into account its status and the type of closure you want to execute and decide some arguments to do it.

## What's Next?

Continue "Understanding Your Lightning Setup" with [§13.7: Lightning Network Implementations](13_7_Lightning_Network_Implementations.md).
