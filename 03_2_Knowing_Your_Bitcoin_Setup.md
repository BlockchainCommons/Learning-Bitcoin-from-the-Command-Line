# 3.2: Knowing Your Bitcoin Setup

Before you start playing with Bitcoin, you may always want to come to a better understanding of your setup.

## Know Your Bitcoin Directory

To start with, you should understand where everything is kept: the `~/.bitcoin` directory.

The main directory just contains your config file and the testnet directory:
```
$ ls ~/.bitcoin
bitcoin.conf  signet
```
The setup guides in [Chapter Two: Creating a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) laid out a standardized config file. [§3.1: Verifying Your Bitcoin Setup](03_1_Verifying_Your_Bitcoin_Setup.md) suggested how to change it to support more advanced setups. If you're interested in learning even more about the config file, you may wish to consult [Jameson Lopp's Bitcoin Core Config Generator](https://jlopp.github.io/bitcoin-core-config-generator/).

Moving back to your ~/.bitcoin directory, you'll find that the `signet` directory contains all of the guts:
```
$ ls ~/.bitcoin/signet
banlist.json  blocks      debug.log             peers.dat      wallets
bitcoind.pid  chainstate  onion_v3_private_key  settings.json
```
You shouldn't mess with most of these files and directories — particularly not the `blocks` and `chainstate` directories, which contain all of the blockchain data, and the information in your `wallets` directory, which will contain your personal wallet (when we set it up). However, do take careful note of the `debug.log` file, which you should refer to if you ever have problems with your setup.

> :link: **SIGNET vs MAINNET:** If you're using mainnet, then _everything_ will instead be placed in the main `~/.bitcoin` directory. These various setups _do_ elegantly stack, so if you are using mainnet, signet, testnet, and regtest, you'll find that `~/.bitcoin` contains your config file and your mainnet data, the `~/.bitcoin/signet` directory contains your signet data, the `~/.bitcoin/testnet3` or `~/.bitcoin/testnet4` directory contains your testnet data, and the `~/.bitcoin/regtest` directory contains your regtest data.

## Know Your Bitcoin-cli Commands

Most of your early work will be done with the `bitcoin-cli` command, which offers an easy interface to `bitcoind`. If you ever want more information on its usage, just run it with the `help` argument. Without any other arguments, it shows you every possible command:
```
$ bitcoin-cli help
== Blockchain ==
dumptxoutset "path" ( "type" {"rollback":n,...} )
getbestblockhash
getblock "blockhash" ( verbosity )
getblockchaininfo
getblockcount
getblockfilter "blockhash" ( "filtertype" )
getblockfrompeer "blockhash" peer_id
getblockhash height
getblockheader "blockhash" ( verbose )
getblockstats hash_or_height ( stats )
getchainstates
getchaintips
getchaintxstats ( nblocks "blockhash" )
getdeploymentinfo ( "blockhash" )
getdescriptoractivity ["blockhash",...] [scanobjects,...] ( include_mempool )
getdifficulty
getmempoolancestors "txid" ( verbose )
getmempooldescendants "txid" ( verbose )
getmempoolentry "txid"
getmempoolinfo
getrawmempool ( verbose mempool_sequence )
gettxout "txid" n ( include_mempool )
gettxoutproof ["txid",...] ( "blockhash" )
gettxoutsetinfo ( "hash_type" hash_or_height use_index )
gettxspendingprevout [{"txid":"hex","vout":n},...]
importmempool "filepath" ( options )
loadtxoutset "path"
preciousblock "blockhash"
pruneblockchain height
savemempool
scanblocks "action" ( [scanobjects,...] start_height stop_height "filtertype" options )
scantxoutset "action" ( [scanobjects,...] )
verifychain ( checklevel nblocks )
verifytxoutproof "proof"
waitforblock "blockhash" ( timeout )
waitforblockheight height ( timeout )
waitfornewblock ( timeout "current_tip" )

== Control ==
getmemoryinfo ( "mode" )
getrpcinfo
help ( "command" )
logging ( ["include_category",...] ["exclude_category",...] )
stop
uptime

== Mining ==
getblocktemplate {"mode":"str","capabilities":["str",...],"rules":["segwit","str",...],"longpollid":"str","data":"hex"}
getmininginfo
getnetworkhashps ( nblocks height )
getprioritisedtransactions
prioritisetransaction "txid" ( dummy ) fee_delta
submitblock "hexdata" ( "dummy" )
submitheader "hexdata"

== Network ==
addnode "node" "command" ( v2transport )
clearbanned
disconnectnode ( "address" nodeid )
getaddednodeinfo ( "node" )
getaddrmaninfo
getconnectioncount
getnettotals
getnetworkinfo
getnodeaddresses ( count "network" )
getpeerinfo
listbanned
ping
setban "subnet" "command" ( bantime absolute )
setnetworkactive state

== Rawtransactions ==
analyzepsbt "psbt"
combinepsbt ["psbt",...]
combinerawtransaction ["hexstring",...]
converttopsbt "hexstring" ( permitsigdata iswitness )
createpsbt [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount,...},{"data":"hex"},...] ( locktime replaceable version )
createrawtransaction [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount,...},{"data":"hex"},...] ( locktime replaceable version )
decodepsbt "psbt"
decoderawtransaction "hexstring" ( iswitness )
decodescript "hexstring"
descriptorprocesspsbt "psbt" ["",{"desc":"str","range":n or [n,n]},...] ( "sighashtype" bip32derivs finalize )
finalizepsbt "psbt" ( extract )
fundrawtransaction "hexstring" ( options iswitness )
getrawtransaction "txid" ( verbosity "blockhash" )
joinpsbts ["psbt",...]
sendrawtransaction "hexstring" ( maxfeerate maxburnamount )
signrawtransactionwithkey "hexstring" ["privatekey",...] ( [{"txid":"hex","vout":n,"scriptPubKey":"hex","redeemScript":"hex","witnessScript":"hex","amount":amount},...] "sighashtype" )
submitpackage ["rawtx",...] ( maxfeerate maxburnamount )
testmempoolaccept ["rawtx",...] ( maxfeerate )
utxoupdatepsbt "psbt" ( ["",{"desc":"str","range":n or [n,n]},...] )

== Signer ==
enumeratesigners

== Util ==
createmultisig nrequired ["key",...] ( "address_type" )
deriveaddresses "descriptor" ( range )
estimatesmartfee conf_target ( "estimate_mode" )
getdescriptorinfo "descriptor"
getindexinfo ( "index_name" )
signmessagewithprivkey "privkey" "message"
validateaddress "address"
verifymessage "address" "signature" "message"

== Wallet ==
abandontransaction "txid"
abortrescan
backupwallet "destination"
bumpfee "txid" ( options )
createwallet "wallet_name" ( disable_private_keys blank "passphrase" avoid_reuse descriptors load_on_startup external_signer )
createwalletdescriptor "type" ( {"internal":bool,"hdkey":"str",...} )
encryptwallet "passphrase"
getaddressesbylabel "label"
getaddressinfo "address"
getbalance ( "dummy" minconf include_watchonly avoid_reuse )
getbalances
gethdkeys ( {"active_only":bool,"private":bool,...} )
getnewaddress ( "label" "address_type" )
getrawchangeaddress ( "address_type" )
getreceivedbyaddress "address" ( minconf include_immature_coinbase )
getreceivedbylabel "label" ( minconf include_immature_coinbase )
gettransaction "txid" ( include_watchonly verbose )
getwalletinfo
importdescriptors requests
importprunedfunds "rawtransaction" "txoutproof"
keypoolrefill ( newsize )
listaddressgroupings
listdescriptors ( private )
listlabels ( "purpose" )
listlockunspent
listreceivedbyaddress ( minconf include_empty include_watchonly "address_filter" include_immature_coinbase )
listreceivedbylabel ( minconf include_empty include_watchonly include_immature_coinbase )
listsinceblock ( "blockhash" target_confirmations include_watchonly include_removed include_change "label" )
listtransactions ( "label" count skip include_watchonly )
listunspent ( minconf maxconf ["address",...] include_unsafe query_options )
listwalletdir
listwallets
loadwallet "filename" ( load_on_startup )
lockunspent unlock ( [{"txid":"hex","vout":n},...] persistent )
migratewallet ( "wallet_name" "passphrase" )
psbtbumpfee "txid" ( options )
removeprunedfunds "txid"
rescanblockchain ( start_height stop_height )
restorewallet "wallet_name" "backup_file" ( load_on_startup )
send [{"address":amount,...},{"data":"hex"},...] ( conf_target "estimate_mode" fee_rate options version )
sendall ["address",{"address":amount,...},...] ( conf_target "estimate_mode" fee_rate options )
sendmany ( "" ) {"address":amount,...} ( minconf "comment" ["address",...] replaceable conf_target "estimate_mode" fee_rate verbose )
sendtoaddress "address" amount ( "comment" "comment_to" subtractfeefromamount replaceable conf_target "estimate_mode" avoid_reuse fee_rate verbose )
setlabel "address" "label"
settxfee amount
setwalletflag "flag" ( value )
signmessage "address" "message"
signrawtransactionwithwallet "hexstring" ( [{"txid":"hex","vout":n,"scriptPubKey":"hex","redeemScript":"hex","witnessScript":"hex","amount":amount},...] "sighashtype" )
simulaterawtransaction ( ["rawtx",...] {"include_watchonly":bool,...} )
unloadwallet ( "wallet_name" load_on_startup )
walletcreatefundedpsbt ( [{"txid":"hex","vout":n,"sequence":n,"weight":n},...] ) [{"address":amount,...},{"data":"hex"},...] ( locktime options bip32derivs version )
walletdisplayaddress "address"
walletlock
walletpassphrase "passphrase" timeout
walletpassphrasechange "oldpassphrase" "newpassphrase"
walletprocesspsbt "psbt" ( sign "sighashtype" bip32derivs finalize )

== Zmq ==
getzmqnotifications

```
You can also type `bitcoin-cli help [command]` to get even more extensive info on that command. For example:
```
$ bitcoin-cli help createwallet
...
createwallet "wallet_name" ( disable_private_keys blank "passphrase" avoid_reuse descriptors load_on_startup external_signer )

Creates and loads a new wallet.

Arguments:
1. wallet_name             (string, required) The name for the new wallet. If this is a path, the wallet will be created at the path location.
2. disable_private_keys    (boolean, optional, default=false) Disable the possibility of private keys (only watchonlys are possible in this mode).
3. blank                   (boolean, optional, default=false) Create a blank wallet. A blank wallet has no keys.
4. passphrase              (string, optional) Encrypt the wallet with this passphrase.
5. avoid_reuse             (boolean, optional, default=false) Keep track of coin reuse, and treat dirty and clean coins differently with privacy considerations in mind.
6. descriptors             (boolean, optional, default=true) If set, must be "true"
7. load_on_startup         (boolean, optional) Save wallet name to persistent settings and load on startup. True to add wallet to startup list, false to remove, null to leave unchanged.
8. external_signer         (boolean, optional, default=false) Use an external signer such as a hardware wallet. Requires -signer to be configured. Wallet creation will fail if keys cannot be fetched. Requires disable_private_keys and descriptors set to true.

Result:
{                    (json object)
  "name" : "str",    (string) The wallet name if created successfully. If the wallet was created using a full path, the wallet_name will be the full path.
  "warnings" : [     (json array, optional) Warning messages, if any, related to creating and loading the wallet.
    "str",           (string)
    ...
  ]
}

Examples:
> bitcoin-cli createwallet "testwallet"
> curl --user myusername --data-binary '{"jsonrpc": "2.0", "id": "curltest", "method": "createwallet", "params": ["testwallet"]}' -H 'content-type: application/json' http://127.0.0.1:8332/
> bitcoin-cli -named createwallet wallet_name=descriptors avoid_reuse=true load_on_startup=true
> curl --user myusername --data-binary '{"jsonrpc": "2.0", "id": "curltest", "method": "createwallet", "params": {"wallet_name":"descriptors","avoid_reuse":true,"load_on_startup":true}}' -H 'content-type: application/json' http://127.0.0.1:8332/

```
> :book: ***What is RPC?*** `bitcoin-cli` is just a handy interface that lets you send commands to the `bitcoind`. More specifically, it's an interface that lets you send RPC (or Remote Procedure Protocol) commands to the `bitcoind`. Often, the `bitcoin-cli` command and the RPC command have identical names and interfaces, but some `bitcoin-cli` commands instead provide shortcuts for more complex RPC requests. Generally, the `bitcoin-cli` interface is much cleaner and simpler than trying to send RPC commands by hand, using `curl` or some other method. However, it also has limitations as to what you can ultimately do.

## Optional: Know Your Bitcoin Info

A variety of bitcoin-cli commands can give you additional information on your bitcoin data. The most general ones are:

`bitcoin-cli -getinfo` returns information from different RPCs (user-friendly)

```diff
$ bitcoin-cli -getinfo

! Chain: test
Blocks: 1977694
Headers: 1977694
Verification progress: 0.9999993275374796
Difficulty: 1

+ Network: in 0, out 8, total 8
Version: 219900
Time offset (s): 0
Proxy: N/A
Min tx relay fee rate (BTC/kvB): 0.00001000

@@ Wallet: ""@@
Keypool size: 1000
Unlocked until: 0
Transaction fee rate (-paytxfee) (BTC/kvB): 0.00000000

# Balance: 0.02853102

- Warnings: unknown new rules activated (versionbit 28)

```

Other commands allow you to get information about blockchain, mining, network, wallet etc.

```
$ bitcoin-cli getblockchaininfo
$ bitcoin-cli getmininginfo
$ bitcoin-cli getnetworkinfo
$ bitcoin-cli getnettotals
$ bitcoin-cli getwalletinfo
```
For example `bitcoin-cli getnetworkinfo` gives you a variety of information on your setup and its access to various networks:
```
$ bitcoin-cli getnetworkinfo
{
  "version": 300200,
  "subversion": "/Satoshi:30.2.0/",
  "protocolversion": 70016,
  "localservices": "0000000000000c08",
  "localservicesnames": [
    "WITNESS",
    "NETWORK_LIMITED",
    "P2P_V2"
  ],
  "localrelay": true,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 11,
  "connections_in": 0,
  "connections_out": 11,
  "networks": [
    {
      "name": "ipv4",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "ipv6",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "onion",
      "limited": false,
      "reachable": true,
      "proxy": "127.0.0.1:9050",
      "proxy_randomize_credentials": true
    },
    {
      "name": "i2p",
      "limited": true,
      "reachable": false,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "cjdns",
      "limited": true,
      "reachable": false,
      "proxy": "",
      "proxy_randomize_credentials": false
    }
  ],
  "relayfee": 0.00000100,
  "incrementalfee": 0.00000100,
  "localaddresses": [
    {
      "address": "172.239.66.235",
      "port": 38333,
      "score": 1
    },
    {
      "address": "2a01:7e03::2000:92ff:fe75:3ec",
      "port": 38333,
      "score": 1
    },
    {
      "address": "km4bnrbemttglfbkafav2nf5mcy4x3l5jmi24fwtjjm3uba2ygycutyd.onion",
      "port": 38333,
      "score": 4
    }
  ],
  "warnings": [
  ]
}
```
Feel free to test out any of these and to use "bitcoin-cli help" if you want more information on what any of them do.

## Summary: Knowing Your Bitcoin Setup

The `~/.bitcoin` directory contains all of your files, while `bitcoin-cli help` and a variety of info commands can be used to get more information on how your setup and Bitcoin work.

## What's Next?

Continue "Understanding Your Bitcoin Setup" with [§3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md).
