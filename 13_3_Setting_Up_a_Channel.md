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
lightning-cli --network=testnet listfunds
{
   "outputs": [],
   "channels": []
}
```       

Since we still do not have the confirmed transaction we do not have a balance,  after 6 confirmations we should see balance available:

```       
lightning-cli --network=testnet listfunds
{
   "outputs": [],
   "channels": []
}
```       


Later we send some sats to this address in this transaction [11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02](https://blockstream.info/testnet/tx/11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02)


#### Connect to remote node

The first thing you need to do is connect your node to a peer. This is done with the `lightning-cli connect` command. Remember that if you want more information on this command, you should type `lightning-cli help connect`. 

#### Open channel


In this example we will use two lightning nodes running c-lightning and LND implmentations.   

```
$ lightning-cli getinfo
moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B
```
