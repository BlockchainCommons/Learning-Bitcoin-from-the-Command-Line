# 18.3: Accessing Bitcoind with NodeJS

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This section explains how to interact with `bitcoind` using the NodeJS programming language and the [BCRPC package](https://github.com/dgarage/bcrpc).

## Set Up Node.js

BCRPC is built on node.js. Thus, you'll first need to install the `node.js` and `npm` (the node package manager) packages for your system. 

If you're using a Ubuntu machine, you can run the following commands to get a new version of `node.js` (as opposed to the horribly out-of-date version in the Ubuntu package system).

```
$ curl -sL https://deb.nodesource.com/setup_14.x | sudo bash -
$ sudo apt-get install -y nodejs
$ sudo npm install mocha -g
```

### Set Up BCRPC

You can now clone the BCRPC package from GitHub and install its dependencies.

```
$ git clone https://github.com/dgarage/bcrpc.git
$ cd bcrpc
$ npm install
```

To test the BCRPC package, you must first set environmental variables for your rpcuser and rpcpassword. As usual, these come from `~/.bitcoin/bitcoin.conf`. You must also set the RPC port to 18332 which should be correct for the standard testnet setup described in these documents.

```
$ export BITCOIND_USER=StandUp
$ export BITCOIND_PASS=d8340efbcd34e312044c8431c59c792c
$ export BITCOIND_PORT=18332
```

> :warning: **WARNING:** Obviously, you'd never put your password in an environmental variable in a production environment.

> :link: **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

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

Congratulations, you now have a Bitcoin-ready RPC wrapper for Node.js that is working with your Bitcoin setup.

### Create a BCRPC Project

You can now create a new Node.js project and install BCRPC via npm.

```
$ cd ~
$ mkdir myproject
$ cd myproject
$ npm init
  [continue with default options]
$ npm install bcrpc
```

## Build Your Connection

In your ```myproject``` directory, create a `.js` file where you JavaScript code will be executed. 

You can initiate an RPC connection by creating an `RpcAgent`:
```
const RpcAgent = require('bcrpc');
agent = new RpcAgent({port: 18332, user: 'StandUp', pass: 'd8340efbcd34e312044c8431c59c792c'});
```
Obviously, your `user` and `pass` should again match what's in your `~/.bitcoin/bitcoin.conf`, and you use `port 18332` if you're on testnet.

### Make an RPC Call

Using BCRPC, you can use the same RPC commands you would usually use via ```bitcoin-cli``` with your `RpcAgent`, except they need to be in camelCase. For example, ```getblockhash``` would be ```getBlockHash``` instead.

To print the newest block number, you just call `getBlockCount` thourgh your `RpcAgent`:

```
agent.getBlockCount(function (err, blockCount) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(blockCount.result);
});
```

### Make an RPC Call with Arguments

The BCRPC functions can accept arguments. For example, ```getBlockHash``` takes ```blockCount.result``` as an input. 

```  
  agent.getBlockHash(blockCount.result, function (err, hash) {
    if (err)
      throw Error(JSON.stringify(err));
    console.log(hash.result);
  })
```

The result of the BCRPC functions is a JSON object containing information about any errors and the id of the request. When accessing your result, you add ```.result``` to the end of it to specify that you are interested in the actual result, not information about errors. 

### Run Your Code

You can find the `getinfo` code in [the src directory](src/18_3_getinfo.js).
```
$ node getinfo.js
1831094
00000000000002bf8b522a830180ad3a93b8eed33121f54b3842d8838580a53c
```
This is what output of the above example would look like if you replaced ```console.log(blockCount.result);``` and ```console.log(hash.result);``` with ```console.log(blockCount);``` and ```console.log(hash);```, respectively:
```
{ result: 1774686, error: null, id: null }
{
  result: '00000000000000d980c495a2b7addf09bb0a9c78b5b199c8e965ee54753fa5da',
  error: null,
  id: null
}
```

## Look Up Funds

It's useful when accepting Bitcoin to check the received Bitcoin on a specific address in your wallet. For example, if you were running an online store accepting Bitcoin, for each payment from a customer, you would generate a new address, show that address to the customer, then check the balance of the address after some time, to make sure the correct amount has been received:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

> :information_source: **NOTE:** Obviously, you'll need to enter an address recognized by your machine.

By default this functions checks the transactions that have been confirmed once, however you can increase this to a higher number such as 6:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', 6, function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

### Look Up Wallet Information

You can also look up additional information about your wallet and view your balance, transaction count, et cetera:

```
agent.getWalletInfo(function (err, walletInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(walletInfo.result);
});
```

The source is available as [walletinfo.js](src/18_3_walletinfo.js).
```
$ node walletinfo.js
0.008498
{
  walletname: '',
  walletversion: 169900,
  balance: 0.010438,
  unconfirmed_balance: 0,
  immature_balance: 0,
  txcount: 4,
  keypoololdest: 1596567843,
  keypoolsize: 999,
  hdseedid: 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
  keypoolsize_hd_internal: 1000,
  paytxfee: 0,
  private_keys_enabled: true,
  avoid_reuse: false,
  scanning: false
}
```
Instead of printing all the details associated with your wallet, you can print specific information, such as your balance. Since a JSON object is being accessed, this can be done by changing the line ```console.log(walletInfo.result);``` to ```console.log(walletInfo.result.balance);```:

## Create an Address

You can also pass additional arguments to RPC commands. For example, the following generates a new legacy address, with the ```-addresstype``` flag.

```
agent.getNewAddress('-addresstype', 'legacy', function (err, newAddress) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(newAddress.result);
});
```
This is the same as running the following from the command line:
```
$ bitcoin-cli getnewaddress -addresstype legacy
mtGPcBvRPZFEHo2YX8un9qqPBydhG82uuZ
```

In BCRPC, you can generally use the same flags as in ```bitcoin-cli``` in BCRPC. Though you use camelCase (```getNewAddress```) for the methods, the flags, which are normally separated by spaces on the command line, are instead placed in strings and separated by commas.

## Send a Transaction

You can send coins to an address most easily using the ```sendToAddress``` function:

```
agent.sendToAddress(newAddress.result, 0.00001, function(err, txid) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(txid.result);
});
```

This should print the txid of the transaction:

```
1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9
```

### Look Up a Transaction

You may now wish to view a transaction, such as the one you just sent.
```
agent.getTransaction(txid.result, function (err, transaction) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(transaction.result);
});
```

You should get an output similar to this:

```
{
  amount: 0.001,
  confirmations: 4776,
  blockhash: '000000006628870b0a8a66abea9cf0d4e815c491f079e3fa9e658a87b5dc863a',
  blockindex: 117,
  blocktime: 1591857418,
  txid: '1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df',
  walletconflicts: [],
  time: 1591857343,
  timereceived: 1591857343,
  'bip125-replaceable': 'no',
  details: [
    {
      address: 'mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha',
      category: 'receive',
      amount: 0.001,
      label: '',
      vout: 0
    }
  ],
  hex: '02000000000101e9e8c3bd057d54e73baadc60c166860163b0e7aa60cab33a03e89fb44321f8d5010000001716001435c2aa3fc09ea53c3e23925c5b2e93b9119b2568feffffff02a0860100000000001976a914600c8c6a4abb0a502ea4de01681fe4fa1ca7800688ac65ec1c000000000017a91425b920efb2fde1a0277d3df11d0fd7249e17cf8587024730440220403a863d312946aae3f3ef0a57206197bc67f71536fb5f4b9ca71a7e226b6dc50220329646cf786cfef79d60de3ef54f702ab1073694022f0618731902d926918c3e012103e6feac9d7a8ad1ac6b36fb4c91c1c9f7fff1e7f63f0340e5253a0e4478b7b13f41fd1a00'
}
```

The full code is available as [sendtx.js](src/18_3_sendtx.js).

## Summary: Accessing Bitcoind with Node

With BCRPC you can access all the RPC commands available through ```bitcoin-cli```, in JavaScript. The [BCRPC README](https://github.com/dgarage/bcrpc) has some examples which use promises (the examples in this document use callbacks). The [JavaScript behind it](https://github.com/dgarage/bcrpc/blob/master/index.js) is short and readable. 

Based on these examples you should be able to incorporate Bitcoin in a Node.js project and do things like sending and receiving coins.

## What's Next?

Learn more about "Talking to Bitcoin in Other Languages" in [18.4: Accessing Bitcoin with Python](18_4_Accessing_Bitcoind_with_Python.md).
