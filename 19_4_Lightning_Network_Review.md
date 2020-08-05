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
