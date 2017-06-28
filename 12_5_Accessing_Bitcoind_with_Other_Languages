# 12.5: Accessing Bitcoind with Other Languages

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

[[REVISE TO BE ABOUT MULTIPLE LANGUAGES]]
[[GIVE EXTENSIVE DETAILS FOR NODE, AND LIST SOME OTHERS]]

Accessing the `bitcoind` doesn't require `curl`. All you need is simple RPC functionality. That's what's provided by [BCRPC](https://github.com/dgarage/bcrpc) by kallewoof of Digital Garage.

## Set Up Node.js

BCRPC is built on node.js. Thus, you'll first need to install the `node.js` and `npm` (node package manager) packages for your system. 

If you're using a Ubuntu machine, you can run the following commands to get a new version of `node.js` (as opposed to the horribly out-of-date version in the Ubuntu package system).
```
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
sudo apt-get install -y nodejs
sudo apt-get install mocha -g
```
### Set Up BCRPC

You should next download BCRPC from the [BCRPC Repository](https://github.com/dgarage/bcrpc). Clone it or download it, as you prefer.

Once you've done that, you can finish setting it up:
```
$ unzip bcrpc-master.zip
$ cd bcrpc-master
$ npm install
```

### Test BCRPC

To test the BCRPC package, you must first set environmental variables for your rpcuser and rpcpassword. As noted in [§12.1: Accessing Bitcoind with Curl](12_1_Accessing_Bitcoind_with_Curl.md), these come from `~/.bitcoin/bitcoin.conf`.
```
$ export BITCOIND_USER=bitcoinrpc
$ export BITCOIND_PASS=d8340efbcd34e312044c8431c59c792c
```
> **WARNING:** Obviously, you'd never put set your password in an environmental variable in a production environment.

You can now verify everything is working correctly:
```
$ npm test

> bcrpc@0.0.5 test /home/user1/bcrpc-master
> mocha tests.js

  BitcoinD
    ✓ is running

  bcrpc
    ✓ can get info

  2 passing (36ms)
```
Congratulations, you now have a Bitcoin-ready RPC wrapper for node.js.

## Manipulate Your Wallet

### Look Up Addresses

### Look Up Funds

### Create an Address

## Create a Transaction
 
## Summary: Accessing Bitcoind with Node
