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

### Verify Your Blocks

### Check Your Directory

## Part One: The State of the Chain and the Wallet

