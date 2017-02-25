# 3 - Playing with Bitcoin

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to begin using Bitcoin from the command land. It presumes that you have a VPS that you installed bitcoin on and that is running bitcoind. It also presumes that you are connected to testnet, allowing for access to bitcoin without using real funds. We explained how to do this either by hand in [2A - Setting up a Bitcoin-Core VPS by Hand](./2A_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md) or by using a Linode StackScript at Linode.com in [2B - Setting up a Bitcoin-Core VPS with StackScript](./2B_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

## Part Zero: Getting Started with Bitcoin

Before you start playing with Bitcoin, you should ensure that everything is setup correctly.

### Check Your Aliases

The Bitcoin setup docs suggest creating a set of aliases. In case you didn't run through those docs, you can create them for your main Bitcoin user with the following command:

```
cat >> ~/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
        # alias btcdir="cd ~/Library/Application\ Support/Bitcoin/" #mac default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getinfo | egrep "\"version\"|balance|blocks|connections|errors"'
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev\\\`"
EOF
```

> **WARNING:** The btcblock alias will not work correctly if you try to place it in your .bash_profile by hand, rather than using the "cat" command as suggested. To enter it by hand, you need to adjust the number of backslashes (usually from three each to one each), so make sure you know what you're doing if you aren't entering the commands exactly as shown.

Note that there are shortcuts for running 'bitcoin-cli', for running 'bitcoind', and for going to the Bitcoin directory. These aliases are mainly meant to make your life easier. We suggest you create other aliases to ease your use of frequent commands or strings and to minimize errors. Aliases of this sort can even more useful if you have a complex setup where you regularly run commands associated with Mainnet, with Testnet, _and_ with Regtest.

With that said, the only aliases directly used by this tutorial are 'btcinfo' and 'btcblock', because they encapsulate much longer and more complex commands.

> **TESTNET vs MAINNET:** Remember that this tutorial generally assumes that you are using testnet. Notes like this will comment on how things might be different over on Mainnet. In this case, the 'btcblock' alias needs to be slightly different. On testnet, you can look up the current block count with the complex command "wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev"; on mainnet, you use the much simpler 'wget -O - http://blockchain.info/q/getblockcount 2>/dev/null'

### Know Your Setup Types

When you setup your node, you choose to create it as either a Mainnet, Testnet, or Regtest node. As noted, this document presumes you are using testnet, as that's the easiest type of setup that allows you to test simple Bitcoin commands without spending real money. 

The type of setup is mainly controlled through the ~/.bitcoin/bitcoin.conf file. If you're running testnet, it probably contains this line:
```
testnet=1
```
While if you're running regtest, it probably contains this line:
```
regtest=1
```
However, if you are running several different sorts of nodes, you may decide to leave the testnet (or regtest) flag out of your configuration file. In this case, you can choose whether you're using the mainnet, the testnet, our your regtest every time you run bitcoind or bitcoin-cli.

Here's a set of aliases that would make that easier by creating a specific alias for starting and stopping the bitcoind, for going to the bitcoin directory, and for running bitcoin-cli, for each of the mainnet (which has no extra flags), the testnet (which is -testnet), or the regtest (which is -regtest).
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
For even more complexity, you could have each of your 'start' aliases use the -conf flag to load configuration from a different file. However, this complexity goes far beyond the scope of this tutorial, but we offer it as a starting point for when your explorations of Bitcoin reach the next level.

### Run Bitcoind

Bitcoind _must_ be running to use bitcoin-cli. If you used our standard setup, it should be. You can double check by looking at the process table.
```
$ ps auxww | grep bitcoind
user1    29360 11.5 39.6 2676812 1601416 ?     SLsl Feb23 163:42 /usr/local/bin/bitcoind -daemon
```

### Verify Your Blocks

You should have the whole blockchain (or the pruned blockchain) ready before you start playing. Just run the 'btcblock' alias to see if it's all loaded. You'll see two numbers, which tell you how many blocks have loaded out of how many total.

If the two numbers aren't the same, as is the case for this Pruned Mainnet, you should wait:
```
$ btcblock
427950/454544
```
Total time can take several hours for a pruned testnet, a day for a pruned mainnet or a non-pruned testnet, and longer for a non-pruned mainnet.

If the two numbers are the same, as is the case for this non-Pruned Testnet, you're ready to go:
```
$ btcblock
1090099/1090099
```

### Check Your Directory

Finally, before you get started, you should take a look at your ~/.bitcoin directory, just to get familiar with it.

The main directory just contains your config file and the testnet directory:
```
$ ls ~/.bitcoin
bitcoin.conf  testnet3
```
The testnet3 directory then contains all of the guts:
```
$ ls ~/.bitcoin/testnet3
banlist.dat   blocks	  database  debug.log  wallet.dat
bitcoind.pid  chainstate  db.log    peers.dat
```
You shouldn't mess with most of these directories, particularly the blocks and chainstate, which contain all of the blockchain data. However, do take careful note of the db.log and debug.log file, which you should reer to if you ever have problems with your setup.

> **TESTNET vs MAINNET:** If you're using mainnet, then _everything_ will instead be stuck in the main ~/.bitcoin directory. These various setups _do_ elegantly stack, so if you are using mainnet, testnet, and regtest, you'll find that ~/.bitcoin contains your config file and your mainnet data, ~/.bitcoin/testnet3 contains your testnet data, and ~/.bitcoin/regtest contains your regtest data.

### Get Help

Most of your work will be done with the "bitcoin-cli" command. Need more help on it? Just use the help argument. Without any other arguments, it shows you ever possible command:
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
$ bitcoin-cli help getinfo
getinfo
Returns an object containing various state info.

Result:
{
  "version": xxxxx,           (numeric) the server version
  "protocolversion": xxxxx,   (numeric) the protocol version
  "walletversion": xxxxx,     (numeric) the wallet version
  "balance": xxxxxxx,         (numeric) the total bitcoin balance of the wallet
  "blocks": xxxxxx,           (numeric) the current number of blocks processed in the server
  "timeoffset": xxxxx,        (numeric) the time offset
  "connections": xxxxx,       (numeric) the number of connections
  "proxy": "host:port",     (string, optional) the proxy used by the server
  "difficulty": xxxxxx,       (numeric) the current difficulty
  "testnet": true|false,      (boolean) if the server is using testnet or not
  "keypoololdest": xxxxxx,    (numeric) the timestamp (seconds since Unix epoch) of the oldest pre-generated key in the key pool
  "keypoolsize": xxxx,        (numeric) how many new keys are pre-generated
  "unlocked_until": ttt,      (numeric) the timestamp in seconds since epoch (midnight Jan 1 1970 GMT) that the wallet is unlocked for transfers, or 0 if the wallet is locked
  "paytxfee": x.xxxx,         (numeric) the transaction fee set in BTC/kB
  "relayfee": x.xxxx,         (numeric) minimum relay fee for non-free transactions in BTC/kB
  "errors": "..."           (string) any error messages
}

Examples:
> bitcoin-cli getinfo 
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getinfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

## Part One: The State of the Chain and the Wallet

You're now ready to work directly with the blockchain and your own wallet. To start with, you should learn a bit about the state of these elements.

### Create an Address

The first thing you need to do is create an address for receiving payments. This is done with the "bitcoin-cli getnewaddress" command. If you want more information on it, you should type "bitcoin-cli help getnewaddress".

Theoretically, you could run it just by typing it on the command line:
```
$ bitcoin-cli getnewaddress
miD9uGeAd2Akuf1bFVKCtE2qxfPBjVyy4p
```
However, this isn't best practice; if you retype that or cut and paste it, it would be easy to make a mistake. And then you're telling people to send money to somewhere else (or to nowhere!). So instead we suggest a best practice, which is meant to minimize address mistakes.

#### BEST PRACTICES: Use Variables to Capture Addresses

Instead, use your shell's built in variables to capture your address.
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress)
```
These commands clear the NEW_ADDRESS_1 variable, then fill it with the results of the "bitcoin-cli getnewaddress" command.

You can use your shell's "echo" command to look at your (new) address:
```
$ echo $NEW_ADDRESS_1
mxn1qAvM6wfnZbJAVM2DimUAQUHoGnNb9e
```
Note that this address (and the earlier example) begins with m. This signifies that this is a testnet address. 

> **TESTNET vs MAINNET:** The equivalent mainnet address would start with a 1.

We'll use this same technique when dealing with future address; note that you could do it all by hand, instead of piping stuff in and out of variables ... but we really don't suggest it.

### Capture the Private Key

The address lets you receive bitcoins, but to spend them, you'll need the addresses private address. That's achieved with the "bitcoin-cli dumpprivkey" command. The following extracts the address from the variable created by the best practices, above, them dumps the private key into another variable:
```
$ unset NEW_PRIV_KEY_1
$ NEW_PRIV_KEY_1=$(bitcoin-cli dumpprivkey "$NEW_ADDRESS_1")
```
As usual, an echo will show you what you have:
```
$ echo $NEW_PRIV_KEY_1
cUPty98cDC6aYfN1ba8SCbqSh2twyAh5voFEQu8q5io7LmMANnpK
```
Usually you would not want to share this with anyone! 

### Sign a Message

Alternatively, you can just directly sign a message using your address (and your bitcoind) without saving the private key. You do with with "bitcoin-cli signmessage [address] [message]". For example:
```
$ bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World"
HzF7EWicHjDBf9faRTF5O7Q1+iSOunAYyV2880Yz0T/eaUELCukg2hGlJXj9/kptMBu2wf4DEuS8fnoq2QpvSo8=
```
This verifies that the message for the address was signed by the person who knew the address' private key. A recipient can verify it:
```
$ bitcoin-cli verifymessage $NEW_ADDRESS_1 "HzF7EWicHjDBf9faRTF5O7Q1+iSOunAYyV2880Yz0T/eaUELCukg2hGlJXj9/kptMBu2wf4DEuS8fnoq2QpvSo8=" "Hello, World"
true
```
If some black hat was making up signatures, they'd instead get a negative result:
```
$ bitcoin-cli verifymessage $NEW_ADDRESS_1 "FAKEEWicHjDBf9faRTF5O7Q1+iSOunAYyV2880Yz0T/eaUELCukg2hGlJXj9/kptMBu2wf4DEuS8fnoq2QpvSo8=" "Hello, World"
false
```


