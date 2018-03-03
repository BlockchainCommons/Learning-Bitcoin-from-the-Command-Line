# 3.2: Knowing Your Bitcoin Setup

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Before you start playing with Bitcoin, you may always want to come to a better understanding of your setup.

## Know Your Bitcoin Directory

To start with, you should understand where everything is kept: the ~/.bitcoin directory.

The main directory just contains your config file and the testnet directory:
```
$ ls ~/.bitcoin
bitcoin.conf  testnet3
```
The setup guides in [Chapter Two: Creating a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) laid out a standardized config file. [§3.1: Verifying Your Bitcoin Setup](03_1_Verifying_Your_Bitcoin_Setup.md) suggested how to change it to support more advanced setups. If you're interested in learning even more about the config file, you may wish to consult [Jameson Lopp's Bitcoin Core Config Generator](https://jlopp.github.io/bitcoin-core-config-generator/).

Moving back to your ~/.bitcoin directory, you'll find that the testnet3 directory contains all of the guts:
```
$ ls ~/.bitcoin/testnet3
banlist.dat   blocks	  database  debug.log  wallet.dat
bitcoind.pid  chainstate  db.log    peers.dat
```
You shouldn't mess with most of these files and directories — particularly not the blocks and chainstate directories, which contain all of the blockchain data, and the wallet.dat file, which contains your personal wallet. However, do take careful note of the db.log and debug.log file, which you should refer to if you ever have problems with your setup.

> **TESTNET vs MAINNET:** If you're using mainnet, then _everything_ will instead be placed in the main ~/.bitcoin directory. These various setups _do_ elegantly stack, so if you are using mainnet, testnet, and regtest, you'll find that ~/.bitcoin contains your config file and your mainnet data, ~/.bitcoin/testnet3 contains your testnet data, and ~/.bitcoin/regtest contains your regtest data.

## Know Your Bitcoin-cli Commands

Most of your early work will be done with the `bitcoin-cli` command, which offers an easy interface to `bitcoind`. If you ever want more information on its usage, just run it with the `help` argument. Without any other arguments, it shows you ever possible command:
```
$ bitcoin-cli help
== Blockchain ==
getbestblockhash
getblock "blockhash" ( verbosity )
getblockchaininfo
getblockcount
getblockhash height
getblockheader "hash" ( verbose )
getchaintips
getchaintxstats ( nblocks blockhash )
getdifficulty
getmempoolancestors txid (verbose)
getmempooldescendants txid (verbose)
getmempoolentry txid
getmempoolinfo
getrawmempool ( verbose )
gettxout "txid" n ( include_mempool )
gettxoutproof ["txid",...] ( blockhash )
gettxoutsetinfo
preciousblock "blockhash"
pruneblockchain
savemempool
verifychain ( checklevel nblocks )
verifytxoutproof "proof"

== Control ==
getmemoryinfo ("mode")
help ( "command" )
logging ( <include> <exclude> )
stop
uptime

== Generating ==
generate nblocks ( maxtries )
generatetoaddress nblocks address (maxtries)

== Mining ==
getblocktemplate ( TemplateRequest )
getmininginfo
getnetworkhashps ( nblocks height )
prioritisetransaction <txid> <dummy value> <fee delta>
submitblock "hexdata"  ( "dummy" )

== Network ==
addnode "node" "add|remove|onetry"
clearbanned
disconnectnode "[address]" [nodeid]
getaddednodeinfo ( "node" )
getconnectioncount
getnettotals
getnetworkinfo
getpeerinfo
listbanned
ping
setban "subnet" "add|remove" (bantime) (absolute)
setnetworkactive true|false

== Rawtransactions ==
combinerawtransaction ["hexstring",...]
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime ) ( replaceable )
decoderawtransaction "hexstring" ( iswitness )
decodescript "hexstring"
fundrawtransaction "hexstring" ( options iswitness )
getrawtransaction "txid" ( verbose "blockhash" )
sendrawtransaction "hexstring" ( allowhighfees )
signrawtransaction "hexstring" ( [{"txid":"id","vout":n,"scriptPubKey":"hex","redeemScript":"hex"},...] ["privatekey1",...] sighashtype )
signrawtransactionwithkey "hexstring" ["privatekey1",...] ( [{"txid":"id","vout":n,"scriptPubKey":"hex","redeemScript":"hex"},...] sighashtype )

== Util ==
createmultisig nrequired ["key",...]
estimatesmartfee conf_target ("estimate_mode")
signmessagewithprivkey "privkey" "message"
validateaddress "address"
verifymessage "address" "signature" "message"

== Wallet ==
abandontransaction "txid"
abortrescan
addmultisigaddress nrequired ["key",...] ( "account" "address_type" )
backupwallet "destination"
bumpfee "txid" ( options )
dumpprivkey "address"
dumpwallet "filename"
encryptwallet "passphrase"
getaccount "address"
getaccountaddress "account"
getaddressesbyaccount "account"
getaddressinfo "address"
getbalance ( "account" minconf include_watchonly )
getnewaddress ( "account" "address_type" )
getrawchangeaddress ( "address_type" )
getreceivedbyaccount "account" ( minconf )
getreceivedbyaddress "address" ( minconf )
gettransaction "txid" ( include_watchonly )
getunconfirmedbalance
getwalletinfo
importaddress "address" ( "label" rescan p2sh )
importmulti "requests" ( "options" )
importprivkey "privkey" ( "label" ) ( rescan )
importprunedfunds
importpubkey "pubkey" ( "label" rescan )
importwallet "filename"
keypoolrefill ( newsize )
listaccounts ( minconf include_watchonly)
listaddressgroupings
listlockunspent
listreceivedbyaccount ( minconf include_empty include_watchonly)
listreceivedbyaddress ( minconf include_empty include_watchonly)
listsinceblock ( "blockhash" target_confirmations include_watchonly include_removed )
listtransactions ( "account" count skip include_watchonly)
listunspent ( minconf maxconf  ["addresses",...] [include_unsafe] [query_options])
listwallets
lockunspent unlock ([{"txid":"txid","vout":n},...])
move "fromaccount" "toaccount" amount ( minconf "comment" )
removeprunedfunds "txid"
rescanblockchain ("start_height") ("stop_height")
sendfrom "fromaccount" "toaddress" amount ( minconf "comment" "comment_to" )
sendmany "fromaccount" {"address":amount,...} ( minconf "comment" ["address",...] replaceable conf_target "estimate_mode")
sendtoaddress "address" amount ( "comment" "comment_to" subtractfeefromamount replaceable conf_target "estimate_mode")
setaccount "address" "account"
settxfee amount
signmessage "address" "message"
signrawtransactionwithwallet "hexstring" ( [{"txid":"id","vout":n,"scriptPubKey":"hex","redeemScript":"hex"},...] sighashtype )
walletlock
walletpassphrase "passphrase" timeout
walletpassphrasechange "oldpassphrase" "newpassphrase"
```
You can also type `bitcoin help [command]` to get even more extensive info on that command. For example:
```
$ bitcoin-cli help getmininginfo
getmininginfo

Returns a json object containing mining-related information.
Result:
{
  "blocks": nnn,             (numeric) The current block
  "currentblockweight": nnn, (numeric) The last block weight
  "currentblocktx": nnn,     (numeric) The last block transaction
  "difficulty": xxx.xxxxx    (numeric) The current difficulty
  "networkhashps": nnn,      (numeric) The network hashes per second
  "pooledtx": n              (numeric) The size of the mempool
  "chain": "xxxx",           (string) current network name as defined in BIP70 (main, test, regtest)
  "warnings": "..."          (string) any network and blockchain warnings
}

Examples:
> bitcoin-cli getmininginfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
_What is RPC?_ `bitcoin-cli` is just a handy interface that lets you send commands to the `bitcoind`. More specifically, it's an interface that lets you send RPC (or Remote Procedure Protocol) commands to the `bitcoind`. Often, the `bitcoin-cli` command and the RPC command have identical names and interfaces, but some `bitcoin-cli` commands instead provide shortcuts for more complex RPC requests. Generally, the `bitcoin-cli` interface is much cleaner and simpler than trying to send RPC commands by hand, using `curl` or some other method. However, it also has limitations as to what you can ultimately do.

## Optional: Know Your Bitcoin Info

A variety of bitcoin-cli commands can give you additional information on your bitcoin data. The most general ones are:
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
  "version": 150100,
  "subversion": "/Satoshi:0.15.1/",
  "protocolversion": 70015,
  "localservices": "000000000000000d",
  "localrelay": false,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 9,
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
      "limited": true,
      "reachable": false,
      "proxy": "",
      "proxy_randomize_credentials": false
    }
  ],
  "relayfee": 0.00001000,
  "incrementalfee": 0.00001000,
  "localaddresses": [
    {
      "address": "192.168.1.17",
      "port": 18333,
      "score": 52
    }
  ],
  "warnings": "Warning: unknown
```
Feel free to references any of these and to use "bitcoin-cli help" if you want more information on what any of them do.

## Summary: Knowing Your Bitcoin Setup

The ~/.bitcoin directory contains all of your files, while `bitcoin-cli help` and a variety of info commands can be used to get more information on how your setup and Bitcoin work.

## What's Next?

Continue "Understanding Your Bitcoin Setup" with [§3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md).
