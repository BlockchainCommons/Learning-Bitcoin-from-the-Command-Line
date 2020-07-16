# 13.3: Setting Up a Channel

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You're now ready to your first lightning network channel. To begin with, you'll need to understand what is and how it's created an c-lightning channel.

> :book: ***What is a channel

In simple terms a lightning channel is a money tube that allows fast, cheap and private transfers of money without sending transactions to the blockchain. 
More technically a channel is a 2-of-2 multisignature bitcoin onchain transaction that establishes a financial relationship between two agents without trust.
Channels on the Lightning Network always are created between two nodes. Although a Lightning channel only allows payment between two users, channels can be connected together to form a network that allows payments between members that doesn't have a direct channel between them. The channel mantains a local database with bitcoin balance for both parts keeping track of how much money they each have.  The two users can then exchange bitcoins through their Lightning channel without ever writing to the Bitcoin blockchain. Only when they want to close out their channel do they settle their bitcoins, based on the final division of coins.

In this chapter we will use testnet network and will use c-lightning as **primary node** to show all processes related.

### Steps to create a channel

* Fund your c-lightning wallet with some satoshis.
* Connect to remote node as a peer.
* Open channel.

#### Fund you c-lightning wallet.

The first thing you need to do is send some satoshis to your c-lightning wallet.  You can create a new address using  `lightning-cli newaddr` command to use it later. The newaddr RPC command generates a new address which can subsequently be used to fund channels managed by the c-lightning node.  This last transaction is called the [funding transaction](https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#funding-transaction-output) and it needs to be confirmed before funds can be used.  You can specify the type of address wanted,  if not specified the address generated will be a bech32.

```
c$ lightning-cli --network=testnet newaddr
{
   "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
   "bech32": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6"
}
```       
We send some sats to this address in this transaction [11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02](https://blockstream.info/testnet/tx/11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02)

To check you local balance you should use `lightning-cli listfunds` command:

```       
c$ lightning-cli --network=testnet listfunds
{
   "outputs": [],
   "channels": []
}
```       

Since we still do not have 6 confirmations we do not have balance available,  after 6 confirmations we should see balance:

```       
c$ lightning-cli --network=testnet listfunds
{
   "outputs": [
      {
         "txid": "11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02",
         "output": 0,
         "value": 300000,
         "amount_msat": "300000000msat",
         "scriptpubkey": "0014ca79fcc63cf72c9e0adb3044f559d997cf23b574",
         "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
         "status": "confirmed",
         "blockheight": 1780680,
         "reserved": false
      }
   ],
   "channels": []
}

```       

Now that we have funded our c-lightning wallet we will get information about remote node to start creating channel process.  Using  `lightning-cli --network testnet listchannels` command you can list all active channels and select one remote node and it's public key:

```       
c$ lightning-cli --network listchannels
 {
         "source": "03eef6610d26489b897d81eb142f28ad5cd48a6b3e5c4e42a697cd00d5eb059313",
         "destination": "020b1052c80eb9b2af1126e377640ff9534d8e27556ed78af9329c13c3d932c45f",
         "short_channel_id": "1780579x3x1",
         "public": true,
         "satoshis": 120000,
         "amount_msat": "120000000msat",
         "message_flags": 1,
         "channel_flags": 3,
         "active": false,
         "last_update": 1594650095,
         "base_fee_millisatoshi": 1000,
         "fee_per_millionth": 1,
         "delay": 40,
         "htlc_minimum_msat": "1000msat",
         "htlc_maximum_msat": "118800000msat",
         "features": ""
      },
      {
         "source": "038863cf8ab91046230f561cd5b386cbff8309fa02e3f0c3ed161a3aeb64a643b9",
         "destination": "03f60f7369dd4dcff6a13d401b159e0bfc6aca34f05a93a8a897b75c7940a55bb9",
         "short_channel_id": "1780610x42x1",
         "public": true,
         "satoshis": 50000,
         "amount_msat": "50000000msat",
         "message_flags": 1,
         "channel_flags": 0,
         "active": true,
         "last_update": 1594658630,
         "base_fee_millisatoshi": 1000,
         "fee_per_millionth": 1,
         "delay": 40,
         "htlc_minimum_msat": "1000msat",
         "htlc_maximum_msat": "49500000msat",
         "features": ""
      },
```       

#### Connect to remote node

The first thing you need to do is connect your node to a peer. This is done with the `lightning-cli connect` command. Remember that if you want more information on this command, you should type `lightning-cli help connect`.   The connect RPC command establishes a new connection with another node in the Lightning Network.

To connect your node to a remote peer you need it's id that represents the target node’s public key. As a convenience, id may be of the form id@host or id@host:port.  Using `lightning-cli listnodes` command you obtain all nodes available on the network and choose one.

```       
c$ lightning-cli --network testnet listnodes
```       
Output
```       
  {
         "nodeid": "033c2c5eb5cc514b54264a838048b4e7194281e2dcd4ad03bab0198259df2dcbc7",
         "alias": "shangoa0c8225d-d86b-4",
         "color": "e20f00",
         "last_timestamp": 1533416062,
         "features": "",
         "addresses": [
            {
               "type": "ipv4",
               "address": "54.158.207.111",
               "port": 9735
            }
         ]
      },
      {
         "nodeid": "03dc1ad7b657c4d7a042f1847ffcb953cd353bcfe818bce008c35abdcdc25a5257"
      },
      {
         "nodeid": "030b3a8efb847f1c267172d7afbfe93bf501c44a76c6ac6294a8b6b59335d5cdcd",
         "alias": "030b3a8efb847f1c2671",
         "color": "3399ff",
         "last_timestamp": 1558873572,
         "features": "",
         "addresses": [
            {
               "type": "ipv4",
               "address": "167.99.231.18",
               "port": 9735
            }
         ]
      },
      {
         **"nodeid": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",**
         "alias": "0302d48972ba7eef8b40",
         "color": "3399ff",
         "last_timestamp": 1594828492,
         "features": "02a2a1",
         "addresses": []
      },
     
```       
We've selected node with public key 0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84 and we'll connect as a peer with `lightning-cli connect` command:

```       
c$ lightning-cli --network=testnet connect 0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84@127.0.0.1:9736
{
   "id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
   "features": "02a2a1"
}
```       

To check out:

```       
c$ lightning-cli --network=testnet listpeers
{
   "peers": [
      {
         "id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
         "connected": true,
         "netaddr": [
            "127.0.0.1:9736"
         ],
         "features": "02a2a1",
         "channels": []
      }
   ]
}
```       
On success, an object with a “peers” key is returned containing a list distinct objects. Object features are bit flags showing supported features.

#### Open a channel

The fundchannel RPC command opens a payment channel with a peer by committing a funding transaction to the blockchain.  You should use `lightning-cli fundchannel` command that receives this parameters:

* **id** is the peer id obtained from connect.

* **amount** is the amount in satoshis taken from the internal wallet to fund the channel.  The value cannot be less than the dust limit, currently set to 546, nor more than 16777215 satoshi (unless large channels were negotiated with the peer).

* **feerate** is an optional feerate used for the opening transaction and as initial feerate for commitment and HTLC transactions. 

* **announce** is an optional flag that triggers whether to announce this channel or not. Defaults to true. If you want to create an unannounced private channel put to false.

* **minconf** specifies the minimum number of confirmations that used outputs should have. Default is 1.

* **utxos** specifies the utxos to be used to fund the channel, as an array of “txid:vout”.

Now we open the channel like this:

```
c$ lightning-cli --network=testnet fundchannel 0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84 280000 urgent true 1
{
   "tx": "0200000000010102eb2fbf64dfcd0ff9bf725bf232e16a06c2aae4a0e5f1f95ace29acb94b09110000000000feffffff02264b000000000000160014aa572371f29310cd677d039cdcd054156c1a9545c045040000000000220020c1ebc407d32cd1fdcd7c0deb6817243b2b982cdaf3c70413f9d3ead29c36f11f024730440220676592b102ee659dfe5ac3acddbe35885140a8476ae7dbbb2f53a939cc815ac0022057a66de1ea16644008791d5cd510439bac00def02cbaa46d04febfe1d5e7e1e001210284368eca82346929a1c3ee2625077571b434b4c8111b81a715dfce5ea86dce1f1f2c1b00",
   "txid": "9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6",
   "channel_id": "a67ad9b15cb10b74a584a284f059866dd2114e45f2a997b260464af537c04399"
}
```
To confirm channel status use `lightning-cli listfunds` command:

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
         "channel_total_sat": 280000,
         "amount_msat": "280000000msat",
         "funding_txid": "9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6",
         "funding_output": 1
      }
   ]
}
```
While the channel with 280.000 satoshis (Channel capacity) is confirmed its state will be CHANNELD_AWAITING_LOCKIN and we got an change output with 19238 sats.   
As we're using testnet network these values are used as an example to show the different states that a channel can have. In a channel with real funds, the actual mining circumstances of the mainnet network must be taken into account.

The funding_txid onchain is [9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6](https://blockstream.info/testnet/tx/9843c037f54a4660b297a9f2454e11d26d8659f084a284a5740bb15cb1d97aa6)

#### Channel capacity

As we said before both sides of the channel own a portion of its capacity. The amount on your side of the channel is called *local balance* and the amount on your peer’s side is called *remote balance*. Both balances can be updated many times without closing the channel (sending final balance to the blockchain), but the channel capacity cannot change without closing or splicing it.   Next chapter we will deep creating and paying invoices.


## Summary: Setting up a channel

You need to create a channel with remote nodes to be able to receive and send money over the lightning network.    Further a maintenance task is required to the channels to keep them balanced.

## What's Next?

Continue "Understanding Your Lightning Setup" with [§13.4: Paying_a_Invoice](13_4_Paying_a_Invoice.md).


