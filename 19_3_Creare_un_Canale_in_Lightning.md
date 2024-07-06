# 19.3: Creating a Lightning Channel

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

You now understand the basics of your Lightning setup, and hopefully have either created or been given info on a second Lightning node. You're ready to create your first Lightning Network channel. Of course, you'll need to understand what is, and how it's created using c-lightning.

> :book: ***What is a Lighting Channel?*** Simply, a lightning channel is a money tube that allows fast, cheap and private transfers of money without sending transactions to the blockchain.  More technically a channel is a 2-of-2 multisignature on-chain Bitcoin transaction that establishes a trustless financial relationship between two people or two agents. A certain amount of money is deposited into the channel, when then mantains a local database with bitcoin balance for both parties, keeping track of how much money they each have from the initial amount. The two users can then exchange bitcoins through their Lightning channel without ever writing to the Bitcoin blockchain. Only when they want to close out their channel do they settle their bitcoins to the blockchain, based on the final division of coins.

> :book: ***How do Lightning Channels Create a Lightning Network?*** Although a Lightning channel only allows payment between two users, channels can be connected together to form a network that allows payments between members that doesn't have a direct channel between them. This creates a network among multiple people built from pairwise connections.

In this section, we will continue using our c-lightning setup as our primary node.

## Create a Channel

Creating a Lightning channel requires the following steps:

* Fund your c-lightning wallet with some satoshis.
* Connect to a remote node as a peer.
* Open a channel.

### Fund Your c-lightning Wallet

In order to move funds to a Lightning channel first requires funding your c-lightning wallet.

> :book: ***What is a c-lightning wallet?*** C-lightning's standard implementation comes with a integrated Bitcoin wallet that allows you send and receive on-chain bitcoin transactions. This wallet will be used to create new channels.

The first thing you need to do is send some satoshis to your c-lightning wallet. You can create a new address using  `lightning-cli newaddr` command. This generates a new address that can subsequently be used to fund channels managed by the c-lightning node. You can specify the type of address wanted; if not specified, the address generated will be a bech32.

```
$ lightning-cli --testnet newaddr
{
   "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
   "bech32": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6"
}
```       
You can then send funds to this address using `bitcoin-cli sendtoaddress` (or any other methodlogy you prefer). For this example, we have done so in the transaction [11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02](https://blockstream.info/testnet/tx/11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02).

This transaction is called the [funding transaction](https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#funding-transaction-output), and it needs to be confirmed before funds can be used.  

> :book: ***What is a Funding Transaction?*** A funding transaction is a Bitcoin transaction that places money into a Lightning channel. It may be single-funded (by one participant) or dual-funded (by both). From there on, Lightning transactions are all about reallocating the ownership of the funding transaction, but they only settle to the blockchain when the channel is closed.

To check you local balance you should use `lightning-cli listfunds` command:

```       
c$ lightning-cli --testnet listfunds
{
   "outputs": [],
   "channels": []
}
```       

Since the funds do not yet have six confirmations, there is no balance available. After six confirmations you should see a balance:
```       
c$ lightning-cli --testnet listfunds
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
Note that the value is listed in satoshis or microsatoshis, not Bitcoin! 

> :book: ***What are satoshis and msat?*** You already met satoshis way back in [§3.4](03_4_Receiving_a_Transaction.md). One satoshi is one hundred millionth of a bitcoin, so 300,000 satoshi = 0.003 BTC. A satoshi is the smallest unit of currency on the Bitcoin network. But, the Lightning network can go smaller, so 1,000 msat, or millisatoshis, equal one satoshi. That means that 1 msat is one hundred billionth of a bitcoin, and 300,000,000 msat = 0.003 BTC.

Now that you have funded your c-lightning wallet you will need information about a remote node to start creating channel process. 

### Connect to a Remote Node

The next thing you need to do is connect your node to a peer. This is done with the `lightning-cli connect` command. Remember that if you want more information on this command, you should type `lightning-cli help connect`.

To connect your node to a remote peer you need its id, which represents the target node’s public key. As a convenience, `id` may be of the form `id@host` or `id@host:port`.  You may have retrieved this with `lightning-cli getinfo` (on c-lightning) or `lncli --network=testnet getinfo` (on LND) as discussed in the [previous interlude](19_2__Interlude_Accessing_a_Second_Lightning_Node.md). 

We've selected the LND node, `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`, which is located at IP address `45.33.35.151`, which we're going to connect to from our c-lightning node:

```       
$ lightning-cli --network=testnet connect 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543@45.33.35.151
{
   "id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "features": "02a2a1"
}
```     

### Open a Channel

The fundchannel RPC command opens a payment channel with a peer by committing a funding transaction to the blockchain.  You should use `lightning-cli fundchannel` command to do so, with the following parameters:

* **id** is the peer id return from connect.
* **amount** is the amount in satoshis taken from the internal wallet to fund the channel.  The value cannot be less than the dust limit, currently set to 546, nor more than 16.777.215 satoshi (unless large channels were negotiated with the peer).
* **feerate** is an optional feerate used for the opening transaction and as initial feerate for commitment and HTLC transactions. 
* **announce** is an optional flag that triggers whether to announce this channel or not. It defaults to true. If you want to create an unannounced private channel set it to false.
* **minconf** specifies the minimum number of confirmations that used outputs on the channel opening processe should have. Default is 1.
* **utxos** specifies the utxos to be used to fund the channel, as an array of “txid:vout”.

Now you can open the channel like this:

```
$ lightning-cli --testnet fundchannel 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543 100000 urgent true 1
{
   "tx": "0200000000010193dc3337837f091718f47b71f2eae8b745ec307231471f6a6aab953c3ea0e3b50100000000fdffffff02a0860100000000002200202e30365fe321a435e5f66962492163302f118c13e215ea8928de88cc46666c1d07860100000000001600142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c024730440220668a7c253c9fd83fc1b45e4a52823fb6bc5fad30da36240d4604f0d6981a6f4502202aeb1da5fbbc8790791ef72b3378005fe98d485d22ffeb35e54a6fbc73178fb2012103b3efe051712e9fa6d90008186e96320491cfe1ef1922d74af5bc6d3307843327c76c1c00",
   "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
   "channel_id": "1d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d6966",
   "outnum": 0
}
```
To confirm channel status use `lightning-cli listfunds` command:

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
         "status": "unconfirmed",
         "reserved": false
      },
      {
         "txid": "b5e3a03e3c95ab6a6a1f47317230ec45b7e8eaf2717bf41817097f833733dc93",
         "output": 1,
         "value": 200000,
         "amount_msat": "200000000msat",
         "scriptpubkey": "0014ed54b65eae3da99b23a48bf8827c9acd78079469",
         "address": "tb1qa42tvh4w8k5ekgay30ugyly6e4uq09rfpqf9md",
         "status": "confirmed",
         "blockheight": 1862831,
         "reserved": true
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_AWAITING_LOCKIN",
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
While this new channel with 100,000 satoshis is unconfirmed, its state will be `CHANNELD_AWAITING_LOCKIN`. Note that unconfirmed change of `99847` satoshis is also showing as a new transaction in the wallet. After all six confirmations are completed, the channel will change to `CHANNELD_NORMAL` state,  which will be its permanent state. At this time, a `short_channel_id` will also appear, such as:
```
         "short_channel_id": "1862856x29x0",
```
These values denote where the funding transaction can be found on the blockchain. It appears in the form `block x txid x vout`.

In this case, `1862856x29x0` means:

* Created on the 1862856th block;
* with a `txid` of 29; and
* an `vout` of 0.

You may need to use this `short_channel_id` for certain commands in Lightning.

This funding transaction can also be found onchain at [66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d](https://blockstream.info/testnet/tx/66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d)

> :book: ***What is Channel Capacity?*** In a Lightning Channel, both sides of the channel own a portion of its capacity. The amount on your side of the channel is called *local balance* and the amount on your peer’s side is called *remote balance*. Both balances can be updated many times without closing the channel (when the final balance is sent to the blockchain), but the channel capacity cannot change without closing or splicing it. The total capacity of a channel is the sum of the balance held by each participant in the channel.

## Summary: Setting up a channel

You need to create a channel with a remote node to be able to receive and send money over the Lightning Network.   

## What's Next?

You're ready to go! Move on to [Chapter 20: Using Lightning](20_0_Using_Lightning.md).
