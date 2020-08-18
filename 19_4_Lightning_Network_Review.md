## Lightning Network Review

In this brief review we add some important features of C-lightning and we list the most widely used implementations in the lightning network Today, although there are more.     All implementations follow the Basis of Lightning Technology (BOLT) documents describe a layer-2 protocol for off-chain bitcoin transfers. The specifications are currently a work-in-progress and currently being drafted.

| Name  | Description | BitcoinStandup | Language | Repository |
| ------------- | ------------- | :---: | ------------- | ------------- |
| C-lighting  | Blockstream  | X | C | [Download](https://github.com/ElementsProject/lightning) |
| LND  | Lightning Labs  | X | Go | [Download](https://github.com/lightningnetwork/lnd) |
| Eclair  | ACINQ  | - | Scala | [Download](https://github.com/ACINQ/eclair) |


### Plugins

c-lightning is a lightweight, highly customizable and standard compliant implementation of the Lightning Network protocol.   The way c-lightning extend the functionality is using Plugins, that are a powerful and easy way to do it.  Mainly they are subprocesses that are initiated by lightningd daemon and can interact with lightningd in a different ways:

* Command line option that allows plugins to register their own command line options that are exposed through lightningd.
* JSON-RPC command passthrough allows plugins to add their own commands to the JSON-RPC interface.
* Event stream subscriptions provide plugins with a push-based notification mechanism about events from the lightningd.
* Hooks are a primitive option that allows plugins to be notified about events in lightningd daemon and modify its behavior or pass custom behaviors.

A plugin may be written in any language, and communicates with lightningd through the plugin’s stdin and stdout. JSON-RPCv2 is used as protocol on top of the two streams, with the plugin acting as server and lightningd acting as client. 

Here is a updated list of [plugins](https://github.com/lightningd/plugins) available.

### Backup

As we mentioned on Closing a channel chapter your node needs to be online all the time otherwise your counterparty could send a previous channel status and steals your funds.   However, there is another scenario in which funds can be lost, and that is when a hardware failure occurs that prevents the node from establishing a cooperative closure with the counterparty. This will probably imply that if you do not have an exact copy of the state of the channel before the failure, you will have an invalid state that could cause the other node to assume it as an attempted fraud and use the penalty transaction. In this case all funds will be lost. To avoid this undesirable situation exists a solution based on the high availability of postgresQL database [here](https://github.com/gabridome/docs/blob/master/c-lightning_with_postgresql_reliability.md).    We haven't tested this solution.

### Mobile wallets.

Today we know about two mobile lightning wallets that support c-lightning implementation.   For iOS devices FullyNoded which is an open source iOS Bitcoin wallet that connects via Tor V3 authenticated service to your own full node. FullyNoded  functionality is currently under active development and in early beta testing phase.

SparkWallet is a minimalistic wallet GUI for c-lightning, accessible over the web or through mobile and desktop apps for Android. 

*  [FullyNoded](https://github.com/Fonta1n3/FullyNoded/blob/master/Docs/Lightning.md) in TestFlight
*  [SparkWallet](https://github.com/shesek/spark-wallet)


