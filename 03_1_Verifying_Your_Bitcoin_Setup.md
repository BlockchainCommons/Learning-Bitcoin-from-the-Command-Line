# 3.1: Verifying Your Bitcoin Setup

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Before you start playing with Bitcoin, you should ensure that everything is setup correctly.

## Check Your Aliases

The Bitcoin setup docs suggest creating a set of aliases. In case you didn't run through those docs, you can create them for your main Bitcoin user with the following command:

```
cat >> ~/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev\\\`"
EOF
```

> **WARNING:** The btcblock alias will not work correctly if you try to place it in your .bash_profile by hand, rather than using the "cat" command as suggested. If you want to enter it by hand, you need to adjust the number of backslashes (usually from three each to one each), so make sure you know what you're doing if you aren't entering the commands exactly as suggested.

Note that these aliases includes shortcuts for running `bitcoin-cli`, for running `bitcoind`, and for going to the Bitcoin directory. These aliases are mainly meant to make your life easier. We suggest you create other aliases to ease your use of frequent commands (and arguments) and to minimize errors. Aliases of this sort can be even more useful if you have a complex setup where you regularly run commands associated with Mainnet, with Testnet, _and_ with Regtest, as explained further below.

With that said, use of these aliases in _this_ document might accidentally obscure the core lessons being taught about Bitcoin, so the only aliases directly used here are `btcinfo` and `btcblock`, because they encapsulate much longer and more complex commands. Otherwise, we show the full commands; adjust for your own use as appropriate.

> **TESTNET vs MAINNET:** Remember that this tutorial generally assumes that you are using testnet. The `btcblock` alias needs to be slightly different on mainnet, where you can use the simpler "wget -O - http://blockchain.info/q/getblockcount 2>/dev/null".

## Run Bitcoind

You'll begin your exploration of the Bitcoin network with the `bitcoin-cli` command. However, bitcoind _must_ be running to use bitcoin-cli, as bitcoin-cli sends JSON-RPC commands to the bitcoind. If you used our standard setup, bitcoind should already be up and running. You can double check by looking at the process table.
```
$ ps auxww | grep bitcoind
user1    29360 11.5 39.6 2676812 1601416 ?     SLsl Feb23 163:42 /usr/local/bin/bitcoind -daemon
```
If it's not running, you'll want to run "/usr/local/bin/bitcoind -daemon" by hand and also place it in your crontab, as explained in [§2.1: Setting up a Bitcoin-Core VPS by Hand](02_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md).

## Verify Your Blocks

You should have the whole blockchain downloaded before you start playing. Just run the `btcblock` alias to see if it's all loaded. You'll see two numbers, which tell you how many blocks have loaded out of how many total.

If the two numbers aren't the same, you should wait:
```
$ btcblock
973212/1090099
```
Total download time can take from an hour to several hours, depending on your setup.

If the two numbers are the same, you're fully loaded:
```
$ btcblock
1090099/1090099
```
And that means you're ready to go!

## Optional: Know Your Server Types

> **TESTNET vs MAINNET:** When you set up your node, you choose to create it as either a Mainnet, Testnet, or Regtest node. Though this document presumes a testnet setup, it's worth understanding how you might access and use the other setup types — even all on the same machine! But, if you're a first time user, skip on past this, as it's not necessary for a basic setup.

The type of setup is mainly controlled through the ~/.bitcoin/bitcoin.conf file. If you're running testnet, it probably contains this line:
```
testnet=1
```
If you're running regtest, it probably contains this line:
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
For even more complexity, you could have each of your 'start' aliases use the -conf flag to load configuration from a different file. This goes far beyond the scope of this tutorial, but we offer it as a starting point for when your explorations of Bitcoin reaches the next level.

## Summary: Verifying Your Bitcoin Setup

Before you start playing with bitcoin, you should make sure that your aliases are set up, your bitcoind is running, and your blocks are downloaded. You may also want to set up some access to alternative Bitcoin setups, if you're an advanced user.

## What's Next?

Continue "Understanding Your Bitcoin Setup" with [§3.2: Knowing Your Bitcoin Setup](03_2_Knowing_Your_Bitcoin_Setup.md).
