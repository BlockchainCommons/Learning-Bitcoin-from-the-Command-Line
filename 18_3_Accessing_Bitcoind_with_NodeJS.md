# 18.3: Accessing Bitcoind with NodeJS

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

In this ```myproject``` directory, create a file called ```server.js``` in which the JavaScript code can be executed. Enter the following at the top of the file to connect to your Bitcoin node:

```
const RpcAgent = require('bcrpc');
agent = new RpcAgent({port: 18332, user: 'bitcoinrpc', pass: 'd8340efbcd34e312044c8431c59c792c'});
```

Now we can start writing programs in JavaScript that access Bitcoind. You can use the same RPC commands you would usually use via ```bitcoin-cli```, except they need to be in camelCase. For example, ```getblockhash``` would be ```getBlockHash``` instead.

### Print the Newest Block

To print the newest block number along with its hash, we can first call ```getBlockCount```, print it, then pass the block number to ```getBlockHash``` and then print the latest hash. Add this code to ```server.js```:

```
agent.getBlockCount(function (err, blockCount) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(blockCount.result);
  agent.getBlockHash(blockCount.result, function (err, hash) {
    if (err)
      throw Error(JSON.stringify(err));
    console.log(hash.result);
  })
});
```

You can run it with ```$ node server.js```. You should get an output similar to this:

```
1773373
0000000000000083d29c524d4cfc257adfab8fa9b6f0d207d1d0f1b63e1de11e
```

## Look Up Your Wallet

You can also lookup your wallet and view your balance, transaction count et cetera:

```
agent.getWalletInfo(function (err, walletInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(walletInfo.result);
});
```

When you run it, you should get an output similar to this:

```
{
  walletname: '',
  walletversion: 169900,
  balance: 0.0011,
  unconfirmed_balance: 0,
  immature_balance: 0,
  txcount: 2,
  keypoololdest: 1591844503,
  keypoolsize: 999,
  keypoolsize_hd_internal: 1000,
  paytxfee: 0,
  hdseedid: '6b9ccb7d96c45a3ca407a3f3b0e9b42501f05c49',
  private_keys_enabled: true
}
```

Instead of printing all the details associated with your wallet, you can print specific information such as your balance. Since a JSON object is being accessed, this can be done by changing the line ```console.log(walletInfo.result);``` to ```console.log(walletInfo.result.balance);```:

```
0.0011
```

### Create an Address

We can pass additional arguments to RPC commands as well. Here is an example for generating a new legacy address, with the ```-addresstype``` flag.

```
agent.getNewAddress('-addresstype', 'legacy', function (err, newAddress) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(newAddress.result);
});
```

Output:
```
mtGPcBvRPZFEHo2YX8un9qqPBydhG82uuZ
```

### List transactions

We can list our previous transactions and view information about transactions such as amount and number of confirmations:

```
agent.listTransactions(function (err, transactions) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(transactions.result);
});
```

We can also examine a transaction based on its txid:

```
agent.getTransaction('1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df', function (err, transaction) {
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

### Get Amount Received by an Address

A useful function when accepting Bitcoin is checking the received Bitcoin of a specific address in your wallet. For example, if you were running an online store accepting Bitcoin, for each payment from a customer, you would generate a new address (as we did with ```getNewAddress``` function), show that address to the customer, then check the balance of the address after some time, to make sure the correct amount has been received:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

By default this functions checks the transactions that have been confirmed once, however we can increase this to a higher number such as 6:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', 6, function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

Output:

```
0.0011
```

## Sending coins

We can send coins to an address using the ```sendToAddress``` function:

```
agent.sendToAddress('n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi', 0.00001, function(err, txid) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(txid.result);
});
```

This should print the txid of the transaction:

```
6172d60d154cd4bbb5b6adeaffa191866f3904dd3f525c7a079154aea906b723
```

We can also use the ```getTransaction``` function to view how many confirmations it has, what fee we paid, et cetera.

## Summary: Accessing Bitcoind with Node

With BCRPC we can do all the commands available through ```bitcoin-cli```, in JavaScript. The [BCRPC README](https://github.com/dgarage/bcrpc) has some examples which use promises (the examples in this document use callbacks). The [JavaScript behind it](https://github.com/dgarage/bcrpc/blob/master/index.js) is short and readable. Based on these examples you should be able to incorporate Bitcoin in a Node.js project and do things like sending and receiving coins.
