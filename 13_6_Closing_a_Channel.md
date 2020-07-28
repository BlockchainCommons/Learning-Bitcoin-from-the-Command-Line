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


