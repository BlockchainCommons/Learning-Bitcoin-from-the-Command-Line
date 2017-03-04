
# Interlude: Sending Coins the Easy Way

You're now ready to send some coins, and that's actually quite simple via the command line. You just type `bitcoin-cli sendtoaddress [address] [amount]`. So, to send a little coinage to the address `msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef` just requires:
```
$ bitcoin-cli sendtoaddress msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef 0.1
6ad295c280798e9746dcdf7e5a60dfb6219d93bf31aab9b540ce892537c41e0c
```
Make sure the address you write in is where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back. 

You'll receive a txid back when you issue this command.

> **WARNING:** As noted previously, the bitcoin-cli command generates JSON-RPC commands to talk to the bitcoind. They can be really picky. This is an example: if you list the bitcoin amount without the leading zero (i.e. ".1" instead of "0.1"), then bitcoin-cli will fail with a mysterious message.

You'll be able to see the transaction in your list immediately. 
```
$ bitcoin-cli listtransactions
[
  {
    "account": "",
    "address": "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
    "category": "receive",
    "amount": 0.47000000,
    "label": "",
    "vout": 0,
    "confirmations": 15,
    "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
    "blockindex": 45,
    "blocktime": 1488307692,
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "walletconflicts": [
    ],
    "time": 1488307692,
    "timereceived": 1488307696,
    "bip125-replaceable": "no"
  }, 
 
  {
    "account": "",
    "address": "msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef",
    "category": "send",
    "amount": -0.10000000,
    "label": "",
    "vout": 0,
    "fee": -0.00004520,
    "confirmations": 0,
    "trusted": false,
    "txid": "6ad295c280798e9746dcdf7e5a60dfb6219d93bf31aab9b540ce892537c41e0c",
    "walletconflicts": [
    ],
    "time": 1488321652,
    "timereceived": 1488321652,
    "bip125-replaceable": "unknown",
    "abandoned": false
  }
]
```
However, note that as always it'll take a while for your balances to settle. Be aware that the default transaction fee for `sendtoaddress` is quite low and should probably be increased, as we note in later sections. This means that your transaction might not placed into the first several blocks, until it's reached enough priority. This is usually fine, if you're sending money to someone. It's less fine if you're working through a tutorial, wanting to get to the next step. 

Using the `sendtoaddress` command isn't necessarily that interesting, if you're planning to write your own raw transactions. However, it's a great test so that you can successfully see a transaction leave your machine, taking some of your money with it.
