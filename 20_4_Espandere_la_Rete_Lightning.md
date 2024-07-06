# 20.4: Expanding the Lightning Network

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

These two chapters have covered just a few of the most important activities with Lightning. There's lots more that can be done, and lots of variety possible. What follows are some pointers forward.

## Use c-lightning Plugins

c-lightning is a lightweight, highly customizable, and standard compliant implementation of the Lightning Network protocol. It extends it functionality using Plugins.  Mainly, these are subprocesses that are initiated by the `lightningd` daemon and can interact with `lightningd` in a variety of ways:

* Command line options allow plugins to register their own command line arguments, which are then exposed through `lightningd`.
* JSON-RPC command passthrough allows plugins to add their own commands to the JSON-RPC interface.
* Event stream subscriptions provide plugins with a push-based notification mechanism for `lightnind`.
* Hooks are a primitive option that allows plugins to be notified about events in the `lightningd` daemon and modify its behavior or pass on custom behaviors.

A plugin may be written in any language and can communicate with `lightningd` through the plugin's stdin and stdout. JSON-RPCv2 is used as protocol on top of the two streams, with the plugin acting as server and `lightningd` acting as client. 

The `lightningd` GitHub repo maintains a updated list of [plugins](https://github.com/lightningd/plugins) available.

## Use Mobile Wallets

We currently know of two mobile lightning wallets that support the c-lightning implementation.

For iOS devices FullyNoded is an open-source iOS Bitcoin wallet that connects via Tor V3 authenticated service to your own full node. FullyNoded  functionality is currently under active development and in early beta testing phase.

*  [FullyNoded](https://github.com/Fonta1n3/FullyNoded/blob/master/Docs/Lightning.md)

SparkWallet is a minimalistic wallet GUI for c-lightning, accessible over the web or through mobile and desktop apps for Android. 

*  [SparkWallet](https://github.com/shesek/spark-wallet)

## Use Different Lightning Implementations

c-lightning isn't your only option. Today there are three widely used implementations of the Lightning Network. All of them follow the [Basis of Lightning Technology (BOLT) documents](https://github.com/lightningnetwork/lightning-rfc), which describe a layer-2 protocol for off-chain bitcoin transfers. The specifications are currently a work-in-progress that is still being drafted.

| Name  | Description | BitcoinStandup | Language | Repository |
| ------------- | ------------- | :---: | ------------- | ------------- |
| C-lighting  | Blockstream  | X | C | [Download](https://github.com/ElementsProject/lightning) |
| LND  | Lightning Labs  | X | Go | [Download](https://github.com/lightningnetwork/lnd) |
| Eclair  | ACINQ  | - | Scala | [Download](https://github.com/ACINQ/eclair) |

## Maintain Backups

Your Lightning node needs to be online all the time, otherwise your counterparty could send a previous channel status and steal your funds.  However, there is another scenario in which funds can be lost, and that is when a hardware failure occurs that prevents the node from establishing a cooperative closure with the counterparty. This will probably mean that if you do not have an exact copy of the state of the channel before the failure, you will have an invalid state that could cause the other node to assume it as an attempted fraud and use the penalty transaction. In this case, all funds will be lost. To avoid this undesirable situation a solution based on the high availability of postgresQL database [exists](https://github.com/gabridome/docs/blob/master/c-lightning_with_postgresql_reliability.md).

We haven't tested this solution.

## Summary: Expanding the Lightning Network

You can use different implementations, plugins, mobile wallets, or backups to expand your Lightning experience. 

## What's Next?

You've completed Learning Bitcoin from the Command Line, though if you never visited the [Appendices](A0_Appendices.md) of alternate setups, you can do so now.

Otherwise, we encourage you to join developer communities, to program, and to put your new knowledge to work.

You can also help us here at Blockchain Commons with Issues or PRs for Learning Bitcoin or for any of our other repos, or you can even become a [Sponsor](https://github.com/sponsors/BlockchainCommons). You can also help out by spreading the word: let people on social media know about the course and what you learned from it!

Now get out there and make the Blockchain community a better place!

