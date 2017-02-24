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


## Part One: The State of the Chain and the Wallet

