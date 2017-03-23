# 3.2: Knowing Your bitcoin-cli Setup

## Get Help

Most of your work will be done with the "bitcoin-cli" command. If you ever want more information on its usage, just run the help argument. Without any other arguments, it shows you ever possible command:
```
$ bitcoin-cli help
== Blockchain ==
getbestblockhash
getblock "hash" ( verbose )
getblockchaininfo
getblockcount
getblockhash index
getblockheader "hash" ( verbose )
getchaintips
getdifficulty
getmempoolancestors txid (verbose)
getmempooldescendants txid (verbose)
getmempoolentry txid
getmempoolinfo
getrawmempool ( verbose )
gettxout "txid" n ( includemempool )
gettxoutproof ["txid",...] ( blockhash )
gettxoutsetinfo
verifychain ( checklevel numblocks )
verifytxoutproof "proof"

== Control ==
getinfo
help ( "command" )
stop

== Generating ==
generate numblocks ( maxtries )
generatetoaddress numblocks address (maxtries)

== Mining ==
getblocktemplate ( TemplateRequest )
getmininginfo
getnetworkhashps ( blocks height )
prioritisetransaction <txid> <priority delta> <fee delta>
submitblock "hexdata" ( "jsonparametersobject" )

== Network ==
addnode "node" "add|remove|onetry"
clearbanned
disconnectnode "node" 
getaddednodeinfo dummy ( "node" )
getconnectioncount
getnettotals
getnetworkinfo
getpeerinfo
listbanned
ping
setban "ip(/netmask)" "add|remove" (bantime) (absolute)

== Rawtransactions ==
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime )
decoderawtransaction "hexstring"
decodescript "hex"
fundrawtransaction "hexstring" ( options )
getrawtransaction "txid" ( verbose )
sendrawtransaction "hexstring" ( allowhighfees )
signrawtransaction "hexstring" ( [{"txid":"id","vout":n,"scriptPubKey":"hex","redeemScript":"hex"},...] ["privatekey1",...] sighashtype )

== Util ==
createmultisig nrequired ["key",...]
estimatefee nblocks
estimatepriority nblocks
estimatesmartfee nblocks
estimatesmartpriority nblocks
signmessagewithprivkey "privkey" "message"
validateaddress "bitcoinaddress"
verifymessage "bitcoinaddress" "signature" "message"

== Wallet ==
abandontransaction "txid"
addmultisigaddress nrequired ["key",...] ( "account" )
addwitnessaddress "address"
backupwallet "destination"
dumpprivkey "bitcoinaddress"
dumpwallet "filename"
encryptwallet "passphrase"
getaccount "bitcoinaddress"
getaccountaddress "account"
getaddressesbyaccount "account"
getbalance ( "account" minconf includeWatchonly )
getnewaddress ( "account" )
getrawchangeaddress
getreceivedbyaccount "account" ( minconf )
getreceivedbyaddress "bitcoinaddress" ( minconf )
gettransaction "txid" ( includeWatchonly )
getunconfirmedbalance
getwalletinfo
importaddress "address" ( "label" rescan p2sh )
importprivkey "bitcoinprivkey" ( "label" rescan )
importprunedfunds
importpubkey "pubkey" ( "label" rescan )
importwallet "filename"
keypoolrefill ( newsize )
listaccounts ( minconf includeWatchonly)
listaddressgroupings
listlockunspent
listreceivedbyaccount ( minconf includeempty includeWatchonly)
listreceivedbyaddress ( minconf includeempty includeWatchonly)
listsinceblock ( "blockhash" target-confirmations includeWatchonly)
listtransactions ( "account" count from includeWatchonly)
listunspent ( minconf maxconf  ["address",...] )
lockunspent unlock ([{"txid":"txid","vout":n},...])
move "fromaccount" "toaccount" amount ( minconf "comment" )
removeprunedfunds "txid"
sendfrom "fromaccount" "tobitcoinaddress" amount ( minconf "comment" "comment-to" )
sendmany "fromaccount" {"address":amount,...} ( minconf "comment" ["address",...] )
sendtoaddress "bitcoinaddress" amount ( "comment" "comment-to" subtractfeefromamount )
setaccount "bitcoinaddress" "account"
settxfee amount
signmessage "bitcoinaddress" "message"
```
You can also type "bitcoin help [command]" to get even more extensive info on that command. For example:
```
$ bitcoin-cli help getmininginfo
getmininginfo

Returns a json object containing mining-related information.
Result:
{
  "blocks": nnn,             (numeric) The current block
  "currentblocksize": nnn,   (numeric) The last block size
  "currentblockweight": nnn, (numeric) The last block weight
  "currentblocktx": nnn,     (numeric) The last block transaction
  "difficulty": xxx.xxxxx    (numeric) The current difficulty
  "errors": "..."            (string) Current errors
  "networkhashps": nnn,      (numeric) The network hashes per second
  "pooledtx": n              (numeric) The size of the mempool
  "testnet": true|false      (boolean) If using testnet or not
  "chain": "xxxx",           (string) current network name as defined in BIP70 (main, test, regtest)
}

Examples:
> bitcoin-cli getmininginfo 
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
## Optional: Get More Info

A variety of commands will give oyu move info on your setup:
```
$ bitcoin-cli getblockchaininfo
$ bitcoin-cli getmininginfo
$ bitcoin-cli getnetworkinfo
$ bitcoin-cli getnettotals
$ bitcoin-cli getwalletinfo
```
Feel free to play with any of these, and use "bitcoin-cli help" if you want more information on what any of them do.

## Optional: Know Your Server Types

> **TESTNET vs MAINNET:** When you set up your node, you choose to create it as either a Mainnet, Testnet, or Regtest node. Though this document presumes a testnet setup, it's worth understanding how you might access and use the other setup types — even all on the same machine!

The type of setup is mainly controlled through the ~/.bitcoin/bitcoin.conf file. If you're running testnet, it probably contains this line:
```
testnet=1
```
While if you're running regtest, it probably contains this line:
```
regtest=1
```
However, if you want to run several different sorts of nodes simultaneously, you should leave the testnet (or regtest) flag out of your configuration file. You can then choose whether you're using the mainnet, the testnet, or your regtest every time you run bitcoind or bitcoin-cli.

Here's a set of aliases that would make that easier by creating a specific alias for starting and stopping the bitcoind, for going to the bitcoin directory, and for running bitcoin-cli, for each of the mainnet (which has no extra flags), the testnet (which is -testnet), or your regtest (which is -regtest).
```
alias bcstart="bitcoind -daemon"
alias btstart="bitcoind -testnet -daemon"
alias brstart="bitcoind -regtest -daemon"

alias bcstop="bitcoin-cli stop"
alias btstop="bitcoin-cli -testnet stop"
alias brstop="bitcoin-cli -regtest -stop"

alias bcdir="cd ~/.bitcoin/" #linux default bitcoin path
alias btdir="cd ~/.bitcoin/testnet" #linux default bitcoin testnet path
alias brdir="cd ~/.bitcoin/regtest" #linux default bitcoin regtest path

alias bc="bitcoin-cli"
alias bt="bitcoin-cli -testnet"
alias br="bitcoin-cli -regtest"
```
For even more complexity, you could have each of your 'start' aliases use the -conf flag to load configuration from a different file. This goes far beyond the scope of this tutorial, but we offer it as a starting point for when your explorations of Bitcoin reach the next level.
## Summary: Verifying Your bitcoin-cli Setup

Before you start playing with bitcoin, you should make sure that the bitcoind is running and that all the blocks have been downloaded. You might get additional help from the `bitcoin-cli help` command or from files in the ~/.bitcoin directory.

