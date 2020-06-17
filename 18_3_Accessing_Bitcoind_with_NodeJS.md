# 18.3: Accessing Bitcoind with NodeJS

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

## Set Up Node.js

BCRPC is built on node.js. Thus, you'll first need to install the `node.js` and `npm` (node package manager) packages for your system. 

If you're using a Ubuntu machine, you can run the following commands to get a new version of `node.js` (as opposed to the horribly out-of-date version in the Ubuntu package system).

```
curl -sL https://deb.nodesource.com/setup_14.x | sudo bash -
sudo apt-get install -y nodejs
sudo npm install mocha -g
```

### Test BCRPC

Clone the BCRPC package from GitHub and install its dependencies.

```
$ git clone https://github.com/dgarage/bcrpc.git
$ cd bcrpc
$ npm install
```

To test the BCRPC package, you must first set environmental variables for your rpcuser and rpcpassword. As noted in [§12.1: Accessing Bitcoind with Curl](12_1_Accessing_Bitcoind_with_Curl.md), these come from `~/.bitcoin/bitcoin.conf`. You must also set the RPC port to 18332 which should be correct for the standard testnet setup described in these documents.

```
$ export BITCOIND_USER=bitcoinrpc
$ export BITCOIND_PASS=d8340efbcd34e312044c8431c59c792c
$ export BITCOIND_PORT=18332
```

> **WARNING:** Obviously, you'd never put set your password in an environmental variable in a production environment.
> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

You can now verify everything is working correctly:

```
$ npm test

> bcrpc@0.2.2 test /home/user1/bcrpc
> mocha tests.js

  BitcoinD
    ✓ is running

  bcrpc
    ✓ can get info

  2 passing (36ms)
```

Congratulations, you now have a Bitcoin-ready RPC wrapper for Node.js working with your Bitcoin setup.

### Set Up BCRPC

Leave the BCRPC directory and create a new Node.js project with BCRPC installed via npm.

```
$ cd ..
$ mkdir myproject
$ cd myproject
$ npm init
  [continue with default options]
$ npm install bcrpc
```

## Manipulate Your Wallet

### Look Up Addresses

### Look Up Funds

### Create an Address

## Create a Transaction
 
## Summary: Accessing Bitcoind with Node
