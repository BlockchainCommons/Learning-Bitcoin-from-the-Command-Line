# Appendix III: Using Bitcoin Regtest 

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

The majority of this course presumes that you will either use the Mainnet or Testnet. However, those aren't the only choices. While developing Bitcoin applications, you might want to keep your applications isolated from these public blockchains. To do so, you can create a blockchain from scratch using the Regtest, which has one other major advantage over Testnet: you choose when to create new blocks, so you have complete control over the environment.

## Start Bitcoind on Regtest

After [setting up your Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) or [compiling from source](A2_0_Compiling_Bitcoin_from_Source.md), you are now able to use regtest. To start your `bitcoind` on regtest and create a private Blockchain, use the following command:
```
$ bitcoind -regtest -daemon -fallbackfee=1.0 -maxtxfee=1.1
```

The arguments `-fallbackfee=1.0 -maxtxfee=1.1` will prevent the `Fee estimation failed. Fallbackfee is disabled` error. 

On regtest, usually there are not enough transactions so bitcoind cannot give a reliable estimate and, without it, the wallet will not create transactions unless it is explicitly set the fee.

### Reset the Regtest Blockchain

If you wish, you can later restart your Regtest with a new blockchain. 

Regtest wallets and blockchain state (chainstate) are saved in the regtest subdirectory of the Bitcoin configuration directory:
```
user@mybtc:~/.bitcoin# ls
bitcoin.conf  regtest  testnet3
```

To start a brand new Blockchain using regtest, all you have to do is delete the `regtest` folder and restart the Bitcoind:
```
$ rm -rf regtest
```
## Generate Regtest Wallet

Before generating blocks, it is necessary to load a wallet using `loadwallet` or create a new one with `createwallet`. Since version 0.21, Bitcoin Core does not automatically create new wallets on startup.

The argument `descriptors=true` creates a native descriptor wallet, that stores scriptPubKey information using output descriptors. If it is `false`, it will create a legacy wallet, where keys are used to implicitly generate scriptPubKeys and addresses.
```
$ bitcoin-cli -regtest -named createwallet wallet_name="regtest_desc_wallet" descriptors=true
```

## Generate Blocks

You can generate (mine) new blocks on a regtest chain using the RPC method `generate` with an argument for how many blocks to generate. It only makes sense to use this method on regtest; due to the high difficulty it's very unlikely that it will yield to new blocks on the mainnet or testnet:
```
$ bitcoin-cli -regtest -generate 101
[
  "57f17afccf28b9296048b6370312678b6d8e48dc3a7b4ef7681d18ed3d91c122",
  "631ff7b8135ce633c774828be3b8505726459eb65c339aab981b10363befe5a7",
  ...
  "1162dbfe025c7da94ee1128dc26d518a94508f532c19edc0de6bc673a909d02c",
  "20cb2e815c3d42d6a117a204a0b5e726ab641c826e441b5b3417aca33f2aba48"
]
```

> :warning: WARNING. Note that you must add the `-regtest` argument after each `bitcoin-cli` command to correctly access your Regtest environment. If you prefer, you can include a `regtest=1` command in your `~/.bitcoin/bitcoin.conf` file.

Because a block must have 100 confirmations before that reward can be spent, you generate 101 blocks, providing access to the coinbase transaction from block #1. Because this is a new blockchain using Bitcoin’s default rules, the first blocks pay a block reward of 50 bitcoins. Unlike mainnet, in regtest mode only the first 150 blocks pay a reward of 50 bitcoins. The reward halves after 150 blocks, so it pays 25, 12.5, and so on...

The output is the block hash of every block generated.

> :book: ***What is a coinbase transaction?*** A coinbase is the inputless transaction created when a new block is mined and given to the miner. It's how new bitcoins enter the ecosystem. The value of coinbase transactions decay over time. On the mainnet, it halves every 210,000 blocks and ends entirely with the 6,929,999th block, which is currently predicted for the 22nd century. As of May 2020, the coinbase reward is 6.25 BTC.

### Verify Your Balance

After mining blocks and getting the rewards, you can verify the balance on your wallet:
```
$ bitcoin-cli -regtest getbalance
50.00000000
```

## Use the Regtest

Now you should be able to use this balance for any type of interaction on your private Blockchain, such as sending Bitcoin transactions according to [Chapter 4]((04_0_Sending_Bitcoin_Transactions.md)).

It is important to note that for any transactions to complete, you will have to generate (mine) new blocks, so that the transactions can be included.

For example, to create a transaction and include it in a block, you should first use the `sendtoaddress` command:
```
$ bitcoin-cli -regtest sendtoaddress [address] 15.1
e834a4ac6ef754164c8e3f0be4f34531b74b768199ffb244ab9f6cb1bbc7465a
```

The output is the transaction hash included in the blockchain. You can verify the details using the `gettransaction`:
```
$ bitcoin-cli -regtest gettransaction e834a4ac6ef754164c8e3f0be4f34531b74b768199ffb244ab9f6cb1bbc7465a
{
  "amount": 0.00000000,
  "fee": -0.00178800,
  "confirmations": 0,
  "trusted": false,
  "txid": "e834a4ac6ef754164c8e3f0be4f34531b74b768199ffb244ab9f6cb1bbc7465a",
  "walletconflicts": [
  ],
  "time": 1513204730,
  "timereceived": 1513204730,
  "bip125-replaceable": "unknown",
  "details": [
    {
      "account": "",
      "address": "mjtN3C97kuWMgeBbxdB7hG1bjz24Grx2vA",
      "category": "send",
      "amount": -15.10000000,
      "label": "",
      "vout": 1,
      "fee": -0.00178800,
      "abandoned": false
    },
    {
      "account": "",
      "address": "mjtN3C97kuWMgeBbxdB7hG1bjz24Grx2vA",
      "category": "receive",
      "amount": 15.10000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "020000000f00fe2c7b70b925d0d40011ce96f8991fee5aba9537bd1b6913b37c37b041a57c00000000494830450221009ad02bfeee2a49196a99811ace20e2e7fefd16d33d525884edbc64bf6e2b1db502200b94f4000556391b0998932edde3033ba2517733c7ddffb87d91f6b756629fe201feffffff06a9301a2b39875b68f8058b8e2ad0b658f505e44a67e1e1d039140ae186ed1f0000000049483045022100c65cd13a85af6fcfba74d2852276a37076c89a7642429aa111b7986eea7fd6c7022012bbcb633d392ed469d5befda8df0a6b96e1acfa342f559877edebc2af7cb93401feffffff434b6f67e5e068401553e89f739a3edc667504597f29feb8edafc2b081cc32d90000000049483045022100b86ecc43e602180c787c36465da7fc8d1e8bfba23d6f49c37190c20889f2dfa0022032c3aec3ceefbb7a33c040ef19090cacbfd6bc9c5cd8e94252eb864891c6f34501feffffff4c65b43f8568ce58fc4c55d24ba0742e9878a031fdfae0fadac7247f42cc1f8e0000000049483045022100d055acfce852259dde051dc61792f94277d094c5da96752f925582b8e739868f02205e69add76e6b001073ad6b7df5f32a681fc8513ee0f6e126ee1c2d45149bd91d01feffffff5a72d60b58300974c5d4731e29b437ea61b87b6733bb3ca6ce5548ef8887d05b0000000049483045022100a7f5b2ee656a5a904fb27f982210de6858dfb165777ec969a77ea1c2c82975a4022001a1a563dbc3714047ec855f7aee901e756b851e255f35435e85c2ba7b0abd8401feffffff60d68e9d5650d55bc9e0b2a65ed27a3b9bceac4955760aa1560408854c3b148d000000004948304502210081a6f0c8232c52f3eaca825965077e88b816e503834989be4afb3f44f87eb98202207ae8becb99efe379fb269f477e7bb70d117dcb83e106c53b7addaa9715029da101feffffff63e2239425aad544f6e1157d5ee245d2500d4e9e9daf8049e0a38add6246da890000000049483045022100e0ab1752e8fbb244b63f7dd5649f2222e0dc42fae293b758e0c28082f77560b60220013f72fbe50acf4af197890b4d18fa89094055ed66f9226a6b461cc4ff560f8e01feffffff6aad4151087f4209ace714193dd64f770305dfb89470b79cca538b88253fbbef0000000049483045022100fee4a5f7ec6e8b55bd6aa0e93b5399af724039171d998b926e8095b70953d5f202203db0d4ef9d1bd57aeff0fe3d47d4358ec0559135dac8107507741eef0638279201feffffff7ddbca5854e25e6a2dfeacfe5828267cd1ef5d86e1da573fe2c2b21b45ecd6ce0000000049483045022100bf45241525592df4625642972dbc940ef74771139dd844bc6a9517197d01488c02203c99ca98892cc2693e8fbb9a600962eec84494fb8596acf0d670822624e497c901feffffff8672949de559e76601684c4ac3731599fd965d0c412e7df9f8ec16038d4420a60000000049483045022100b5a9bd3c6718c6bd2a8300bbd1d9de0ff1c5d02aeb6a659c52bb88958e7e3b0302207f710db1ef975c22edf54e063169aae31bbe470166cc0e5c34fd27b730b8e7d001feffffff8e006b0bb8cef2c5c2a11c8c2aa7d3ba01cb4386c7f780c45bc1014142b425f00000000048473044022046dc9db8daeb09b7c0b9f48013c8af2d0a71f688adaa8d91b40891768c852d4a02204fa15da6d58851191344a56c63bf51a540ec03f73117a3446230bb58a8a4bcce01feffffffbad05b8f86182b9b7c9c5aaa9ce3dc8d08a76848e49a2d9b8dcfb0f764bb26ca000000004847304402200682379dc36cb486309eac4913f41ac19638525677edad45ca8d9a2b0728b12f02203fb44f8a46cbc4c02f5699d7d4d9cd810bdf7e7c981b421218ccbcb7b73845f501feffffffd35228fe9ef0a742eacffc4a13f15ed7ba23854e6cb49d5010810ac11b5bdf690000000048473044022030045b882500808bd707f4654becc63de070818c82716310d39576decdd724e3022034d3b41cb5e939f0011bb5251be7941b6077fde5f4eff59afd8e49a2844288f701fefffffff5ae4cbd4ae8d68b5a34be3231cdc88b660447175f39cf7a86397f37641d4aa70000000049483045022100afe16f0de96a8629d6148f93520d690f30126c37e7f7f05300745a1273d7eb7202200933f6b371c4ea522570f3ec2aee9be2b59730b634e828f543bcdb019cf4749901fefffffff633f61ac61683221cc3d2665cf4bcf193af1c8ffe9d3d756ba83cc5eb7643250000000049483045022100ef0b8853c94d60634eff2fc1d4d75872aacb0a2d3242308b7ee256b24739c614022069fe9be8288bdd635871c263c46be710c001729d43f6fbc1350ed1a693c4646301feffffff0250780000000000001976a91464ed7fb2fe0b06f4cad0d731b122222e3e91088a88ac80c5005a000000001976a9142fed0f02d008f89f6a874168e506e2d4f9bcbfb888acd32b0000"
}
```

However, you must now finalize it by creating blocks on the blockchain.
Most applications require six block confirmations to consider the transaction as irreversible. If that is your case, you can mine additional six blocks into your regtest chain:
```
$ bitcoin-cli -regtest -generate 6
[
  "33549b2aa249f0a814db4a2ba102194881c14a2ac041c23dcc463b9e4e128e9f",
  "2cc5c2012e2cacf118f9db4cdd79582735257f0ec564418867d6821edb55715e",
  "128aaa99e7149a520080d90fa989c62caeda11b7d06ed1965e3fa7c76fa1d407",
  "6037cc562d97eb3984cca50d8c37c7c19bae8d79b8232b92bec6dcc9708104d3",
  "2cb276f5ed251bf629dd52fd108163703473f57c24eac94e169514ce04899581",
  "57193ba8fd2761abf4a5ebcb4ed1a9ec2e873d67485a7cb41e75e13c65928bf3"
]
```


## Test with NodeJS

When you are on regtest, you are able to simulate edge cases and attacks that might happen in the real world, such as double spend.

As discussed elsewhere in this course, using software libraries might give you more sophisticated access to some RPC commands. In this case, [bitcointest by dgarage](https://github.com/dgarage/bitcointest) for NodeJS can be used to simulate a transaction from one wallet to another; you can check [their guide](https://www.npmjs.com/package/bitcointest) for more specific attack simulations, such as double spend.

See [§18.3](18_3_Accessing_Bitcoind_with_NodeJS.md) for the most up-to-date info on install NodeJS, then add `bitcointest`:
```
$ npm install -g bitcointest
```

After installing `bitcointest`, you can create a `test.js` file with the following content:
```javascript
file: test.js

const { BitcoinNet, BitcoinGraph } = require('bitcointest');
const net = new BitcoinNet('/usr/local/bin', '/tmp/bitcointest/', 22001, 22002);
const graph = new BitcoinGraph(net);

try {

  console.log('Launching nodes...');
  
  const nodes = net.launchBatchS(4);
  const [ n1, n2 ] = nodes;
  net.waitForNodesS(nodes, 20000);

  console.log('Connected!');
  const blocks = n1.generateBlocksS(110);
  console.info('Generated 110 blocks');

  console.log(`n2.balance (before) = ${n2.getBalanceS()}`);

  const sometxid = n1.sendToNodeS(n2, 100);
  console.log(`Generated transaction = ${sometxid}`);
  n1.generateBlocksS(110);
  n2.waitForBalanceChangeS(0);

  const sometx = n2.getTransactionS(sometxid);
  console.log(`n2.balance (after) = ${n2.getBalanceS()}`);


} catch (e) {
  console.error(e);
  net.shutdownS();
  throw e;
}
```

As shown, this will generate blocks and a transaction:
```
$ node test.js
Launching nodes...
Connected!
Generated 110 blocks
n2.balance (before) = 0
Generated transaction = 91e0040c26fc18312efb80bad6ec3b00202a83465872ecf495c392a0b6afce35
n2.after (before) = 100

```

## Summary: Using Bitcoin Regtest

A regtest environment for Bitcoin works just like any testnet environment, except for the fact that you have the ability to easily and quickly generate blocks.

> :fire: ***What is the power of regtest?*** The biggest power of regtest is that you can quickly mine blocks, allowing you to rush the blockchain along, to test transactions, timelocks, and other features that you'd otherwise have to sit around and wait on. However, the other power is that you can run it privately, without connecting to a public blockchain, allowing you to test our proprietary ideas before releasing them into the world.

## What's Next?

If you visited this Appendix while working on some other part of the course, you should get back there.

But otherwise, you've reached the end! Other people who have worked their way through this course have become professional Bitcoin developers and engineers, including some of whom have contributed to [Blockchain Commons](https://www.blockchaincommons.com/). We encourage you to do the same! Just get out there are start working on some of your own Bitcoin code with what you learned.
