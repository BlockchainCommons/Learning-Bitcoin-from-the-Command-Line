# 3 - Playing with Bitcoin

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to begin using Bitcoin from the command line. It presumes that you have a VPS that you installed bitcoin on and that is running bitcoind. It also presumes that you are connected to testnet, allowing for access to bitcoin without using real funds. We explained how to do this either by hand in [2A - Setting up a Bitcoin-Core VPS by Hand](./2A_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md) or by using a Linode StackScript at Linode.com in [2B - Setting up a Bitcoin-Core VPS with StackScript](./2B_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

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

Note that these aliases includes shortcuts for running 'bitcoin-cli', for running 'bitcoind', and for going to the Bitcoin directory. These aliases are mainly meant to make your life easier. We suggest you create other aliases to ease your use of frequent commands or strings and to minimize errors. Aliases of this sort can even more useful if you have a complex setup where you regularly run commands associated with Mainnet, with Testnet, _and_ with Regtest.

With that said, use of these aliases in _this_ document might accidentally obscure the core lessons being taught about Bitcoin, so the only aliases directly used here are 'btcinfo' and 'btcblock', because they encapsulate much longer and more complex commands. Otherwise, we show the full commands; adjust for your own use as appropriate.

> **TESTNET vs MAINNET:** Remember that this tutorial generally assumes that you are using testnet. Notes like this will comment on how things might be different over on Mainnet. In this case, the 'btcblock' alias needs to be slightly different. On testnet, you can look up the current block count with the complex command "wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev"; on mainnet, you use the much simpler 'wget -O - http://blockchain.info/q/getblockcount 2>/dev/null'

### Run Bitcoind

You'll be accessing the Bitcoin network with the bitcoin-cli command. However, bitcoind _must_ be running to use bitcoin-cli. If you used our standard setup, it should be. You can double check by looking at the process table.
```
$ ps auxww | grep bitcoind
user1    29360 11.5 39.6 2676812 1601416 ?     SLsl Feb23 163:42 /usr/local/bin/bitcoind -daemon
```
If it's not running, you'll want to run '/usr/local/bin/bitcoind -daemon' by hand and also place it in your crontab, as explained in [2A - Setting up a Bitcoin-Core VPS by Hand](./2A_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md).

### Verify Your Blocks

You should have the whole blockchain (or the whole pruned blockchain) ready before you start playing. Just run the 'btcblock' alias to see if it's all loaded. You'll see two numbers, which tell you how many blocks have loaded out of how many total.

If the two numbers aren't the same, as shown in this testnet example, you should wait:
```
$ btcblock
973212/1090099
```
Total time can take several hours for a pruned testnet, a day for a pruned mainnet or a non-pruned testnet, and longer for a non-pruned mainnet.

If the two numbers are the same, as shown in this testnet example, you're ready to go:
```
$ btcblock
1090099/1090099
```

### Check Your Directory

If your bitcoind is running and you've downloaded all the blocks, you ready to go. You could move right on to Part 1. However, you may want to be aware of other Bitcoin resources. To start with, take a look at your ~/.bitcoin directory.

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
You shouldn't mess with most of these files and directories — particularly not the blocks and chainstate directories, which contain all of the blockchain data. However, do take careful note of the db.log and debug.log file, which you should refer to if you ever have problems with your setup.

> **TESTNET vs MAINNET:** If you're using mainnet, then _everything_ will instead be stuck in the main ~/.bitcoin directory. These various setups _do_ elegantly stack, so if you are using mainnet, testnet, and regtest, you'll find that ~/.bitcoin contains your config file and your mainnet data, ~/.bitcoin/testnet3 contains your testnet data, and ~/.bitcoin/regtest contains your regtest data.

### Get Help

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

### Optional: Know Your Server Types

> **TESTNET vs MAINNET:** When you set up your node, you choose to create it as either a Mainnet, Testnet, or Regtest node. Though this document presumes a testnet setup, it's worth understanding how you might access and use the other setup types — even all on the same machine!

The type of setup is mainly controlled through the ~/.bitcoin/bitcoin.conf file. If you're running testnet, it probably contains this line:
```
testnet=1
```
While if you're running regtest, it probably contains this line:
```
regtest=1
```
However, if you want to run several different sorts of nodes, you will probably decide to leave the testnet (or regtest) flag out of your configuration file. In this case, you can choose whether you're using the mainnet, the testnet, or your regtest every time you run bitcoind or bitcoin-cli.

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

### Summary: Getting Started

Before you start playing with bitcoin, you should make sure that the bitcoind is running and that all the blocks have been downloaded. You might get additional help from the 'bitcoin-cli help' command or from files in the ~/.bitcoin directory.

## Part One: Setting Up Your Wallet

You're now ready to start working with Bitcoin. To begin with, you'll need to initiate your wallet with an address for receiving funds.

### Create an Address

The first thing you need to do is create an address for receiving payments. This is done with the "bitcoin-cli getnewaddress" command. Remember that if you want more information on this command, you should type "bitcoin-cli help getnewaddress".

Theoretically, you could run it just by typing it on the command line:
```
$ bitcoin-cli getnewaddress
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
However, this isn't best practice; if you retype that address or cut and paste it, it would be easy to make a mistake. And then you're telling people to send money to somewhere else (or to nowhere!). So instead we suggest a best practice, which is meant to minimize address mistakes.

#### BEST PRACTICES: Use Variables to Capture Addresses

Instead, use your shell's built in variables to capture your address.
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress)
```
These commands clear the NEW_ADDRESS_1 variable, then fill it with the results of the "bitcoin-cli getnewaddress" command.

You can then use your shell's "echo" command to look at your (new) address:
```
$ echo $NEW_ADDRESS_1
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
Note that this address begins with an "n" (but sometimes an "m"). This signifies that this is a testnet address. 

> **TESTNET vs MAINNET:** The equivalent mainnet address would start with a 1.

We'll use this same technique for other bitcoin-cli results in the future; note that you could do it all by hand, instead of piping stuff in and out of variables ... but we really don't suggest it.

### Capture the Private Key

The address lets you receive bitcoins, but to spend them, you'll need the address' private key. That's achieved with the "bitcoin-cli dumpprivkey" command. The following extracts the address from the variable created by the best practices, above, them dumps the private key into another variable:
```
$ unset NEW_PRIV_KEY_1
$ NEW_PRIV_KEY_1=$(bitcoin-cli dumpprivkey "$NEW_ADDRESS_1")
```
As usual, an echo will show you what you have:
```
$ echo $NEW_PRIV_KEY_1
cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Usually you would not want to share this with anyone! And, in fact, you might need to use other techniques to save it when you have real addresses, receiving real money in the future!

### Sign a Message

You can also sign a message using your address (and your bitcoind) without needing your private key on hand. You do this with "bitcoin-cli signmessage [address] [message]". For example:
```
$ bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World"
H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=
```
This verifies that the message for the address was signed by the person who knew the address' private key. A recipient can verify it if he inputs the address, the signature, and the message.
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
true
```
If some black hat was making up signatures, they'd instead get a negative result:
```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "FAKEBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
false
```

### Summary: Setting Up Your Wallet

You need to create an address to receive funds. We suggest using variables to capture your address and ensure that you receive your funds. Based on that address, you can also access a private key and sign messages.

## Receiving a Transaction

You're now ready to receive some money at the new address you set up.

### Get Some Money

To do anything more, you need to get some money. On testnet this is done through faucets. Since the money is all pretend, you just go to a faucet, request some money, and it's sent over to you. We suggest using the faucet at http://tpfaucet.appspot.com/. If it's not available for some reason, search for "bitcoin testnet faucet", and you should find others.

To use a faucet, you'll usually need to go to a URL and enter your address. Yes, this violates our Best Practices, but that's how the faucets tend to work.

> **TESTNET vs MAINNET:** Sadly, there are no faucets in real life. If you were playing on the mainnet, you'd need to go and actually buy bitcoins at a bitcoin exchange or ATM or you'd need to get someone to send them to you. Testnet life is much easier.

### Verify Your Money

After you've requested your money, you should be able to verify it with the 'bitcoin-cli getbalance' command:
```
$ bitcoin-cli getbalance
0.00000000
```
But wait, there's no balance yet!? Welcome to the world of Bitcoin latency. Transactions are transmitted across the network and gathered into blocks by miners. If you don't see a balance, your block hasn't been made yet. However, "bitcoin-cli get unconfirmedbalance" should still show it as long as the initial transaction has been created:
```
$ bitcoin-cli getunconfirmedbalance
0.47000000
```
If that's still showing a zero, you're probably moving through this tutorial too fast. Wait a second. The coins should show up unconfirmed, then rapidly move to confirmed. However, if your "getbalance" and your "getunconfirmedbalance" both still show zero in ten minutes, then there's probably something wrong with the faucet, and you'll need to pick another. Do note that a coin can move from unconfirmedbalance to confirmedbalance almost immediately, though, so make sure you check both.

After a block is built and confirmed, another block is built on top of it, and another ... Because this is a stochastic process, there's some chance for reversal when a block is still new. Thus, a block has to be buried several blocks deep in a chain before you can feel total confident in your funds. Each of those blocks tends to be built in an average of 10 minutes ... so it usually takes about an hour for a confirmed transaction to receive full confidence.

You can use 'bitcoin-cli getbalance "\*" [n]' to see if a confirmed balance is 'n' blocks deep.

The following shows that our transaction has been confirmed one time, but not twice:
```
$ bitcoin-cli getbalance "*" 1
0.47000000
$ bitcoin-cli getbalance "*" 2
0.00000000
```
Obviously, every ten minutes or so this depth will increase.

#### Verify Your Wallet

You can also access all of this information with the 'bitcoin-cli getwalletinfo' command:
```
$ bitcoin-cli getwalletinfo
{
  "walletversion": 130000,
  "balance": 0.47000000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 1,
  "keypoololdest": 1488216266,
  "keypoolsize": 100,
  "paytxfee": 0.00000000,
  "hdmasterkeyid": "b91d5ec57d5ae3e90fff50d12e819429b6496b94"
}
```

### Discover Your Transaction ID

Your money came into you via a transaction. You can discover that transactionid (txid) with the 'bitcoin-cli listtransactions' command:
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
    "confirmations": 2,
    "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
    "blockindex": 45,
    "blocktime": 1488307692,
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "walletconflicts": [
    ],
    "time": 1488307692,
    "timereceived": 1488307696,
    "bip125-replaceable": "no"
  }
]
```
This shows one transaction ("88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2") that was received ("receive") by a specific address in my wallet ("n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf") for a specific amount ("0.47000000").

You can access similar information with the 'bitcoin-cli listunspent' command, but it only shows the transactions for the money that you haven't sent back out:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "vout": 0,
    "address": "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
    "account": "",
    "scriptPubKey": "76a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac",
    "amount": 0.47000000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true
  }
]
```
Note that bitcoins are not just a homogeneous mess of cash jammed into your pocket. Each individual transaction that you receive or that you send is placed in the immutable blockchain ledger, in a block. You can see all of those when you look at your transactions. Note also that this means that bitcoin spending isn't quite as anonymous as you'd think. Though the addresses are fairly private, transactions can be examined as they go in and out of them. This makes the funds ultimately fungible and makes the privacy vulnerable to statistical analysis. 

> **TESTNET vs MAINNET:** Why are all of these bitcoin amounts in fractions? Bitcoins are produced slowly, and so there are relatively few in circulation. As a result, each bitcoin over on the mainnet is worth quite a bit (~ $1,200 at the time of this writing). This means that people usually work in fractions. In fact, .47 BTC would be quite a lot in the real-world. You'll often be dealing with even smaller fractions on mainnet. For this reason, names have appeared for smaller amounts of bitcoins, including millibitcoins or mBTCs (one-thousandth of a bitcoin), microbitcoins or or bits μBTCs (one-millionth of a bitcoin), and satoshis (one hundred millionth of a bitcoin).

### Examine Your Transaction

You can get more information on a transaction with the 'bitcoin-cli getrawtransaction' command:
```
$ bitcoin-cli getrawtransaction "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2"
010000000133261a25b44689bab2c6a207381ca21d243de9bbf21f0fa40c3a26ba7282a87b000000006b483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75feffffff02c029cd02000000001976a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac17791703000000001976a914e176ee39c642344df2180863e27e2e936307273c88ac07a41000
```
> **WARNING:** This command will not work in some cases. Generally, some of the money must be unspent, or it must still be in your mempool (which means that it'll work fine for the money you've just received). Otherwise, if you have a non-pruned node, then you can maintain a set of all transactions with the txindex=1 configuration, which is what our scripts suggest for all non-pruned instances.

Granted, this isn't super useful, because it's the hex-encoded transaction data. Fortunately, you can get a more verbose description just by adding a '1' to your command:
```
$ bitcoin-cli getrawtransaction "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2" 1
{
  "hex": "010000000133261a25b44689bab2c6a207381ca21d243de9bbf21f0fa40c3a26ba7282a87b000000006b483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75feffffff02c029cd02000000001976a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac17791703000000001976a914e176ee39c642344df2180863e27e2e936307273c88ac07a41000",
  "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
  "hash": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
  "size": 226,
  "vsize": 226,
  "version": 1,
  "locktime": 1090567,
  "vin": [
    {
      "txid": "7ba88272ba263a0ca40f1ff2bbe93d241da21c3807a2c6b2ba8946b4251a2633",
      "vout": 0,
      "scriptSig": {
        "asm": "3045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c8[ALL] 034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75",
        "hex": "483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75"
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.47000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 fd67e8a7c7813e7a5c376eb71074f373d924d968 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf"
        ]
      }
    }, 
    {
      "value": 0.51869975,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e176ee39c642344df2180863e27e2e936307273c OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e176ee39c642344df2180863e27e2e936307273c88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n256of3JH1A6X8AQUU7LYkcaRcmrfGjGKC"
        ]
      }
    }
  ],
  "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
  "confirmations": 3,
  "time": 1488307692,
  "blocktime": 1488307692
}
```
Now you can see the full information on the transaction, including all of the inputs ("vin") and all the outputs ("vout). One of the interesting things to note is that though I received .47 BTC in the transaction, another .51869975 was sent to another address. It is quite typical for a transaction to have multiple inputs and/or multiple outputs.

### Optional: Use a Block Explorer

Even looking at the verbose information for a transaction can be a little intimidating. The main goal of this tutorial is to teach about dealing with rawtransactions for Bitcoin from the command line. But, we're happy to talk about other tools when they're applicable. One of those tools is a block explorer, which you can use to look at transactions from a web browser in a much friendlier format.

Currently, our preferred block explorer is https://live.blockcypher.com/. 

You can use it to look up transactions for an address:
```
https://live.blockcypher.com/btc-testnet/address/n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf/
```
You can also use it to look at individual transactions:
```
https://live.blockcypher.com/btc-testnet/tx/88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2/
```
A block explorer doesn't generally provide any more information than a command line look at a raw transaction; it just does a good job of highlighting the important information and putting together the puzzle pieces, so that you know what you're seeing.

### Summary: Receiving a Transactions

Faucets will give you money on the testnet. They come in as rawtransactions, which can be examined with 'getrawtransaction' or a block explorer. Once you've receive a transaction, you can see it in your balance and your wallet.

### Interlude: Sending Coins the Easy Way

You're nowready to send some coins, and that's actually quite simple via the command line. You just type `bitcoin-cli sendtoaddress [address] [amount]`. So, to send a little coinage to the address `msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef` just requires:
```
$  bitcoin-cli sendtoaddress msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef 0.1
6ad295c280798e9746dcdf7e5a60dfb6219d93bf31aab9b540ce892537c41e0c
```
Make sure the address you write is in where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back. 

You'll receive a txid back when you issue this command.

> **WARNING:** The bitcoin-cli command generates JSON RPC commands to talk to the bitcoind. They can be really picky. This is an example: if you list the bitcoin amount without the leading zero (i.e. ".1" instead of "0.1"), then bitcoin-cli will fail with a mysterious message.

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
However, note that as always it'll take a while for your balances to settle. Be aware that the default transaction fee for 'sendtoaddress' is quite low, which means that it might not placed into the first several blocks, until it's reached enough priority. This is usually fine, if you're sending money to someone. It's less fine if you're working through a tutorial, wanting to get to the next step. The 'settxfee' can be used to set a different fee (per kB). But you'll get _much_ more control once you're actually writing raw transactions.

Mind you, this all isn't necessarily that interesting if you're planning to write your own rawtransactions. But, it's a great test so that you can successfully see a transaction leave your machine, taking some of your money with it.

## Part Three: Sending a Raw Transaction

We're now ready to create Bitcoin rawtransactions. This allows you to send money (as in the interlude above) but to craft the transactions accidentally as you want ... and to do more complex things, as we'll see.

## Understand the Bitcoin Transaction

Before we dive into actually creating rawtransactions, you should make sure you understand how a Bitcoin transaction works. 

When you receive cash in your Bitcoin wallet, it appears as an individual transaction. Each of these transactions is called a Unspent Transaction Output (UTXO). It doesn't matter if multiple bits of money came into the same address or to multiple addresses: each UTXO remains distinct in your wallet.

When you create a transaction, you gather together one or more UTXOs, each of which represents a clump of money that you received. Together their amount must equal what you want to spend _or more_. You use these as inputs into the transaction. Then, you generate one or more outputs, which give the money represented by the inputs to one or more people. This creates new UTXOs for the recipients, which may then be used to fund future transactions.

Here's the trick: _all of the UTXOs that you gather are spent in full!_ That means that if you want to send just part of the money in a UTXO to someone else, then you also have to generate an additional output that sends the rest back to you!

## List Your Unspent Transactions

In order to create a new raw transaction, you must know what UTXOs you have on-hand to spent. You can determine this information with the `bitcoin-cli listunspent` command:
```
[
  {
    "txid": "ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72",
    "vout": 0,
    "address": "mrS1ypy2pCweh2nBpkMD7r2T3Zj344wxaY",
    "account": "",
    "scriptPubKey": "76a91477ba616a2778b05a5fd73c7449964050fd1a6fd288ac",
    "amount": 0.08000000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  }, 
  {
    "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
    "vout": 1,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  }, 
  {
    "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
    "vout": 0,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  }
]
```
Here we see a listing of three different UTXOs, worth .08, .078, and .078 BTC. Note that each has its own distinct txi and remains distinct in our wallet, even though two of them were sent to the same address.

These UTXOs will be the foundation of any raw transactions we create.

### Write a Raw Transaction with One Ouput

### Create a Change Address

### Write a Raw Transacrion with Two Outputs

### Write a Raw Transaction with Two Outputs and an OP_RETURN

### Summary: Sending a Transaction



