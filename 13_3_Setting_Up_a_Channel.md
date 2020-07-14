# 13.3: Setting Up a Channel

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You're now ready to your first lightning network channel. To begin with, you'll need to understand what is and how it's created an c-lightning channel.

## What is a channel

In simple terms a lightning channel is a money tube that allows fast, cheap and private transfers of money without sending transactions to the blockchain. 
More technically a channel is a 2-of-2 multisignature bitcoin onchain transaction that establishes a financial relationship between two agents without trust.
As we seen in [§6.1:Sending a transaction to a multisig](06_1_Sending_a_Transaction_to_a_Multisig.md) chapter multisignatures are generally described as being "m of n". That means that the transaction is locked with a group of "n" keys, but only "m" of them are required to unlock the transaction.   

Channels on the Lightning Network always are created between two nodes. The channel mantains a local database with bitcoin balance for both parts keeping track of how much money they each have.

In this chapter we will use testnet network and will use c-lightning as primary node to show all processes related but we will use a lnd node too to create a channel with it.  If you're running our BitcoinStandup setup you could choose either of the two implementations at the time of installing.

### Steps to create a channel

* Fund your c-lightning wallet with some satoshis.
* Connect to remote node as a peer.
* Open channel.

#### Fund you c-lightning wallet.

The first thing you need to do is send some satoshis to your c-lightning wallet.  You can create a new address using this command an send it money.  This is done with the `lightning-cli newaddr` command. The newaddr RPC command generates a new address which can subsequently be used to fund channels managed by the c-lightning node.  This transaction is called the [funding transaction](https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#funding-transaction-output) and it needs to be confirmed before funds can be used.  You can specify the type of address wanted,  if not specified the address generated will be a bench32.

```
$ lightning-cli --network=testnet newaddr
{
   "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
   "bech32": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6"
}
```       

To check you local balance you should use `lightning-cli listfunds` command:

```       
$ lightning-cli --network=testnet listfunds
{
   "outputs": [],
   "channels": []
}
```       

Since we still do not have the confirmed transaction we do not have a balance,  after 6 confirmations we should see balance available:

```       
$ lightning-cli --network=testnet listfunds
{
   "outputs": [],
   "channels": []
}
```       

Later we send some sats to this address in this transaction [11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02](https://blockstream.info/testnet/tx/11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02)

Now that we have funded our c-lightning wallet we will get information about remote node to start creating channel process.  On LND nodes you can get information about your node using `lncli -n testnet getinfo`:

```       
lncli -n testnet getinfo
{
    "version": "0.10.99-beta commit=clock/v1.0.0-171-g8cb1276dbf0bfd9fcbf599df87a43238e599eaac",
    "commit_hash": "8cb1276dbf0bfd9fcbf599df87a43238e599eaac",
    "identity_pubkey": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
    "alias": "0302d48972ba7eef8b40",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 0,
    "num_inactive_channels": 0,
    "num_peers": 0,
    "block_height": 1780686,
    "block_hash": "000000000000000beb29fa5d3afb713a253f949b12f5f5be25935bb26764e321",
    "best_header_timestamp": "1594737322",
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

#### Connect to remote node

The first thing you need to do is connect your node to a peer. This is done with the `lightning-cli connect` command. Remember that if you want more information on this command, you should type `lightning-cli help connect`.   The connect RPC command establishes a new connection with another node in the Lightning Network.

To connect your node to a remote peer you need it's id that represents the target node’s public key. As a convenience, id may be of the form id@host or id@host:port. In this case, the port parameter must be changed due we're running two nodes on the same machine,  c-lightning node it's running on 9735 default port and we've started LND node on 9736 port with this parameter.

```       
$ lnd --listen=0.0.0.0:9736 
```       
We can check it's listening on 9736 port using this command:

```       
$ netstat -aon | grep 9736
tcp6       0      0 :::9736                 :::*                    LISTEN      off (0.00/0/0)
$ 
```       
Now we can use `lightning-cli connect` command to reach node as a peer:

```       
$ lightning-cli --network=testnet connect 0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84@127.0.0.1:9736
{
   "id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
   "features": "02a2a1"
}
```       

To check out:

```       
$ lightning-cli --network=testnet listpeers
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
On success, an object with a “peers” key is returned containing a list distinct objects. Object features are bit flags showing supported features (BOLT #9).

#### Open channel


In this example we will use two lightning nodes running c-lightning and LND implmentations.   

```
$ lightning-cli getinfo

```
