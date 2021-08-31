# 18.4: Accessing Bitcoind with Python

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This section explains how to interact with `bitcoind` using the Python programming language and the [Python-BitcoinRPC](https://github.com/jgarzik/python-bitcoinrpc).

## Set Up Python

If you already have Bitcoin Core installed, you should have Python 3 available as well.
You can check this by running:

`$ python3 --version`

If it returns a version number (e.g., `3.7.3` or `3.8.3`) then you have python3 installed. 

However, if you somehow do not have Python installed, you'll need build it from source as follows. Please see the ["Building Python from Source"](17_4_Accessing_Bitcoind_with_Python.md#variant-build-python-from-source) variant before continuing.

### Set Up BitcoinRPC

Whether you used an existing Python or built it from source, you're now ready to install  the `python-bitcoinrpc` library:

```
$ pip3 install python-bitcoinrpc
```
If you don't have `pip` installed, you'll need to run the following:
```
$ sudo apt install python3-pip
```
(Then repeat the `pip3 install python-bitcoinrpc` instructions.)

### Create a BitcoinRPC Project

You'll generally need to include appropriate declarations from `bitcoinrpc` in your Bitcoin projects in Python. The following will give you access to the RPC based commands:
```py
from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
```
You may also find the following useful:
```py
from pprint import pprint
import logging
```
`pprint` will pretty print the `json` response from `bitcoind`.

`logging` will print out the call you make to `bitcoind` and `bitcoind`'s respose, which is useful when you make a bunch of calls together. If you don't want the excessive output in the terminal just comment out the `logging` block.

## Build Your Connection

You are now ready to start interacting with `bitcoind` by establishing a connection. Create a file called `btcrpc.py` and type the following:

```py
logging.basicConfig()
logging.getLogger("BitcoinRPC").setLevel(logging.DEBUG)
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = "StandUp"
rpc_pass = "6305f1b2dbb3bc5a16cd0f4aac7e1eba"
rpc_host = "127.0.0.1"
rpc_client = AuthServiceProxy(f"http://{rpc_user}:{rpc_pass}@{rpc_host}:18332", timeout=120)
```

The arguments in the URL are `<rpc_username>:<rpc_password>@<host_IP_address>:<port>`. As usual, the `user` and `pass` are found in your `~/.bitcoin/bitcoin.conf`, while the `host` is your localhost, and the port is `18332` for testnet. The `timeout` argument is specified since sockets timeout under heavy load on the mainnet. If you get `socket.timeout: timed out` response, be patient and increase the `timeout`.

> :link: **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

### Make an RPC Call

If `rpc_client` is successfully initialized, you'll be able to send off RPC commands to your bitcoin node.

In order to use an RPC method from `python-bitcoinrpc`, you'll use `rpc_client` object that you created, which provides most of the functionality that can be accessed through `bitcoin-cli`, using the same method names.

For example, the following will retrieve the blockcount of your node:

```py
block_count = rpc_client.getblockcount()
print("---------------------------------------------------------------")
print("Block Count:", block_count)
print("---------------------------------------------------------------\n")
```

You should see the following output with `logging` enabled :

```sh
DEBUG:BitcoinRPC:-3-> getblockcount []
DEBUG:BitcoinRPC:<-3- 1773020
---------------------------------------------------------------
Block Count: 1773020
---------------------------------------------------------------
```
### Make an RPC Call with Arguments

You can use that blockcount as an argument to retrieve the blockhash of a block and also to retrieve details of that block.

This is done by send your `rpc_client` object commands with an argument:
```py
blockhash = rpc_client.getblockhash(block_count)
block = rpc_client.getblock(blockhash)
```

The `getblockhash` will return a single value, while the `getblock` will return an associative array of information about the block, which includes an array under `block['tx']` providing details on each transaction within the block:
```py
nTx = block['nTx']
if nTx > 10:
    it_txs = 10
    list_tx_heading = "First 10 transactions: "
else:
    it_txs = nTx
    list_tx_heading = f"All the {it_txs} transactions: "
print("---------------------------------------------------------------")
print("BLOCK: ", block_count)
print("-------------")
print("Block Hash...: ", blockhash)
print("Merkle Root..: ", block['merkleroot'])
print("Block Size...: ", block['size'])
print("Block Weight.: ", block['weight'])
print("Nonce........: ", block['nonce'])
print("Difficulty...: ", block['difficulty'])
print("Number of Tx.: ", nTx)
print(list_tx_heading)
print("---------------------")
i = 0
while i < it_txs:
    print(i, ":", block['tx'][i])
    i += 1
print("---------------------------------------------------------------\n")
```

### Run Your Code

You can retrieve [the src code](src/18_4_getinfo.py) and run it with `python3`:
```
$ python3 getinfo.py
---------------------------------------------------------------
Block Count: 1831106
---------------------------------------------------------------
---------------------------------------------------------------
BLOCK:  1831106
-------------
Block Hash...:  00000000000003b2ea7c2cdfffd86156ad1f5606ab58e128940a2534d1348b04
Merkle Root..:  056a547fe59208167eef86fa694263728fb684119254b340c1f86bdd423a8082
Block Size...:  52079
Block Weight.:  128594
Nonce........:  1775583700
Difficulty...:  4194304
Number of Tx.:  155
First 10 transactions: 
---------------------
0 : d228d55112e3aa26265b0118cfdc98345c229d20fe074b9afb87107c03ce11b5
1 : 92822e8e34fafb472b87c99ea3f3e16440452b3f361ed86c6fa62175173fb750
2 : fa7c67600c14d4aa350a9674688f1429577954f4a6c5e4639d06c8964824f647
3 : 3a91d1527e308e5603dafde7ab17824f441a73a779d2571d073466dc9e8451b2
4 : 30fd0e5527b1522e7b26a4818b9edac80fe47c0c39fc34705478a49e684708d0
5 : 24c5372b38c78cbaf5b0b305925502a491bc0c1b5758f50c0bd335abb6ae85f5
6 : be70e125a5793efc5e32051fecba0668df971bdf371138c8261201c2a46b2d38
7 : 41ebf52c847a59ba0aeb4425c74e89a01e91defa86a82785ff53ed4668054561
8 : dc8211b4ce122f87692e7c203672e3eb1ffc44c0a307eafcc560323fcc5fae78
9 : 59e2d8e11cad287eacf3207e64a373f65059286b803ef0981510193ae29cbc8c
---------------------------------------------------------------
```

## Look Up Funds

You can similarly retrieve your wallet's information with the `getwalletinfo` RPC:

```py
wallet_info = rpc_client.getwalletinfo()
print("---------------------------------------------------------------")
print("Wallet Info:")
print("-----------")
pprint(wallet_info)
print("---------------------------------------------------------------\n")
```

You should have an output similar to the following with `logging` disabled:

```sh
---------------------------------------------------------------
Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.07160443'),
 'hdseedid': '6dko666b1cc0d69b7eb0539l89eba7b6390kdj02',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1542245729,
 'keypoolsize': 999,
 'keypoolsize_hd_internal': 1000,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 9,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------
```
Other informational commands such as `getblockchaininfo`, `getnetworkinfo`, `getpeerinfo`, and `getblockchaininfo` will work similarly.

Other commands can give you specific information on select elements within your wallet.

### Retrieve an Array

The `listtransactions` RPC allows you to look at the most recent 10 transactions on your system (or some arbitrary set of transactions using the `count` and `skip` arguments). It shows how an RPC command can return an easy-to-manipulate array:

```py
tx_list = rpc_client.listtransactions()
pprint(tx_list)
```

### Explore a UTXO

You can similarly use `listunspent` to get an array of UTXOs on your system:

```py
print("Exploring UTXOs")
## List UTXOs
utxos = rpc_client.listunspent()
print("Utxos: ")
print("-----")
pprint(utxos)
print("------------------------------------------\n")
```

In order to manipulate an array like the one returned from `listtransactions` or `listunpsent`, you just grab the appropriate item from the appropriate element of the array:
```
## Select a UTXO - first one selected here
utxo_txid = utxos[0]['txid']
```

For `listunspent`, you get a `txid`. You can retrieve information about it with `gettransaction`, then decode that with `decoderawtransaction`:

```
utxo_hex = rpc_client.gettransaction(utxo_txid)['hex']

utxo_tx_details = rpc_client.decoderawtransaction(utxo_hex)

print("Details of Utxo with txid:", utxo_txid)
print("---------------------------------------------------------------")
print("UTXO Details:")
print("------------")
pprint(utxo_tx_details)
print("---------------------------------------------------------------\n")
```

This code is available at [walletinfo.py](src/18_4_walletinfo.py).

```
$ python3 walletinfo.py 
---------------------------------------------------------------
Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.01031734'),
 'hdseedid': 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1596567843,
 'keypoolsize': 1000,
 'keypoolsize_hd_internal': 999,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 6,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------

Utxos: 
-----
[{'address': 'mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg',
  'amount': Decimal('0.00001000'),
  'confirmations': 1180,
  'desc': "pkh([ce0c7e14/0'/0'/25']02d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472)#n06pq9q5",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 0},
 {'address': 'tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4',
  'amount': Decimal('0.01029734'),
  'confirmations': 1180,
  'desc': "wpkh([ce0c7e14/0'/1'/26']02c581259ba7e6aef6d7ea23adb08f7c7f10c4c678f2e097a4074639e7685d4805)#j3pctfhf",
  'safe': True,
  'scriptPubKey': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 1},
 {'address': 'mzDxbtYY3LBBBJ6HhaBAtnHv6c51BRBTLE',
  'amount': Decimal('0.00001000'),
  'confirmations': 1181,
  'desc': "pkh([ce0c7e14/0'/0'/23']0377bdd176f985b4af2f6bdbb22c2925b6007b6c07ba171f75e65990c002615e98)#3y6ef6vu",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914cd339342b06042bb986a45e73d56db46acc1e01488ac',
  'solvable': True,
  'spendable': True,
  'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
  'vout': 1}]
------------------------------------------

Details of Utxo with txid: 84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
---------------------------------------------------------------
UTXO Details:
------------
{'hash': '0c6c27f58f122329bbc53a91f290b35ce23bd2708706b21a04cdc387dc8e2fd9',
 'locktime': 1831103,
 'size': 225,
 'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
 'version': 2,
 'vin': [{'scriptSig': {'asm': '', 'hex': ''},
          'sequence': 4294967294,
          'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
          'txinwitness': ['3044022014b3e2359fb46d8cbc4cd30fa991b455edfa4b419a4c64a53fcdfc79e3ca89db022010cefc3268bc252d55f1982c426328b709b47d02332def9e2efb3b12de2cf0d301',
                          '0351b470e87b44e8e9607acf09b8d4543c51c93c17dc741176319e60202091f2be'],
          'vout': 0}],
 'vout': [{'n': 0,
           'scriptPubKey': {'addresses': ['mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg'],
                            'asm': 'OP_DUP OP_HASH160 '
                                   'a080d1a10f5e7a02d0a291f118982ed19e8cfcd7 '
                                   'OP_EQUALVERIFY OP_CHECKSIG',
                            'hex': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
                            'reqSigs': 1,
                            'type': 'pubkeyhash'},
           'value': Decimal('0.00001000')},
          {'n': 1,
           'scriptPubKey': {'addresses': ['tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4'],
                            'asm': '0 1e127c28a5d696c01aee1adaa8662396a5902fff',
                            'hex': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
                            'reqSigs': 1,
                            'type': 'witness_v0_keyhash'},
           'value': Decimal('0.01029734')}],
 'vsize': 144,
 'weight': 573}
---------------------------------------------------------------
```

## Create an Address

Creating a new address with Python 3 just requires the use of an RPC like `getnewaddress` or `getrawchangeaddress`.
```
new_address = rpc_client.getnewaddress("Learning-Bitcoin-from-the-Command-Line")
new_change_address = rpc_client.getrawchangeaddress()
```
In this example, you give the `getnewaddress` command an argument: the `Learning-Bitcoin-from-the-Command-Line` label.

## Send a Transaction

Creating a transaction in Python 3 requires combining some of the previous examples (of creating addresses and retrieving UTXOs) with some new RPC commands for creating, signing, and sending a transaction — much as you've done previously from the command line.

There are five steps:

0. Create two addresses, one that will act as recipient and the other for change.
1. Select a UTXO and set transaction details.
2. Create a raw transaction.
3. Sign the raw transaction with the private key of the UTXO.
4. Broadcast the transaction on the bitcoin testnet.

### 1. Select UTXO & Set Transaction Details

In the folowing code snippet you first select the UTXO which we want to spend. Then you get its address, transaction id, and the vector index of the output. 

```py
utxos = rpc_client.listunspent()
selected_utxo = utxos[0]  # again, selecting the first utxo here
utxo_address = selected_utxo['address']
utxo_txid = selected_utxo['txid']
utxo_vout = selected_utxo['vout']
utxo_amt = float(selected_utxo['amount'])
```
Next, you also retrieve the recipient address to which you want to send the bitcoins, calculate the amount of bitcoins you want to send, and calculate the miner fee and the change amount. Here, the amount is arbitrarily split in two and a miner fee is arbitrarily set.

```py
recipient_address = new_address
recipient_amt = utxo_amt / 2  # sending half coins to recipient
miner_fee = 0.00000300        # choose appropriate fee based on your tx size
change_address = new_change_address
change_amt = float('%.8f'%((utxo_amt - recipient_amt) - miner_fee))
```

> :warning: **WARNING:** Obviously a real program would make more sophisticated choices about what UTXO to use, what to do with the funds, and what miner's fee to pay.

### 2. Create Raw Transacion

Now you have all the information to send a transaction, but before you can send one, you have to create a transaction. 

```py
txids_vouts = [{"txid": utxo_txid, "vout": utxo_vout}]
addresses_amts = {f"{recipient_address}": recipient_amt, f"{change_address}": change_amt}
unsigned_tx_hex = rpc_client.createrawtransaction(txids_vouts, addresses_amts)
```
Remember that the format of the `createrawtransaction` command is:

`$ bitcoin-cli createrawtransaction '[{"txid": <utxo_txid>, "vout": <vector_id>}]' '{"<address>": <amount>}'`

The `txids_vouts` is thus a list and the `addresses_amts` is a python dictionary, to match with the format of `createrawtransaction`.

If you want to see more about the details of the transaction that you've created, you can use `decoderawtransaction`, either in Python 3 or with `bitcoin-cli`.

### 3. Sign Raw Transaction

Signing a transaction is often the trickiest part of sending a transaction programmatically. Here you retrieve a private key from an address with `dumpprivkey` and place it in an array:
```py
address_priv_key = []  # list of priv keys of each utxo
address_priv_key.append(rpc_client.dumpprivkey(utxo_address))
```
You can then use that array (which should contain the private keys of every UTXO that is being spent) to sign your `unsigned_tx_hex`:
```py
signed_tx = rpc_client.signrawtransactionwithkey(unsigned_tx_hex, address_priv_key)
```
This returns a JSON object with the signed transaction's hex, and whether it was signed completely or not:

### 4. Broadcast Transaction

Finally, you are ready to broadcast the signed transaction on the bitcoin network:

```py
send_tx = rpc_client.sendrawtransaction(signed_tx['hex'])
```
### Run Your Code

The [sample code](src/18_4_sendtx.py) is full of `print` statements to demonstrate all of the data available at every point:
```
$ python3 sendtx.py 
Creating a Transaction
---------------------------------------------------------------
Transaction Details:
-------------------
UTXO Address.......:  mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg
UTXO Txid..........:  84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
Vector ID of Output:  0
UTXO Amount........:  1e-05
Tx Amount..........:  5e-06
Recipient Address..:  tb1qca0elxxqzw5xc0s3yq5qhapzzj90ka0zartu6y
Change Address.....:  tb1qrveukqrvqm9h6fua99xvcxgnvdx507dg8e8hrt
Miner Fee..........:  3e-06
Change Amount......:  2e-06
---------------------------------------------------------------

---------------------------------------------------------------
Unsigned Transaction Hex:  02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f20840000000000ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000
---------------------------------------------------------------

---------------------------------------------------------------
Signed Transaction: 
----------------------
{'complete': True,
 'hex': '02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f2084000000006a47304402205da9b2234ea057c9ef3b7794958db6c650c72dedff1a90d2915147a5f6413f2802203756552aba0dd8ebd71b0f28341becc01b28d8b28af063d7c8ce89f9c69167f8012102d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000'}
---------------------------------------------------------------

---------------------------------------------------------------
TXID of sent transaction:  187f8baa222f9f37841d966b6bad59b8131cfacca861cbe9bfc8656bd16a44cc
```

## Summary: Accessing  Bitcoind with Python

Accessing Bitcoind with Python is very easy while using the `python-bitcoinrpc` library. The first thing to always do is to establish a connection with your `bitcoind` instance, then you can call all of the bitcoin API calls as described in the bitcoin-core documentation. This makes it easy to create small or large programs to manage your own node, check balances, or create cool applications on top, as you access the full power of `bitcoin-cli`.

## What's Next?

Learn more about "Talking to Bitcoin in Other Languages" in [18.5: Accessing Bitcoin with Rust](18_5_Accessing_Bitcoind_with_Rust.md).

## Variant: Build Python from Source

If you need to install Python 3 from source, follow these instructions, then continue with ["Creating a BitcoinRPC Project"](17_4_Accessing_Bitcoind_with_Python.md#creating-a-bitcoinrpc-project).

### 1. Install Dependencies
```sh
$ sudo apt-get install build-essential checkinstall
$ sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev
```

### 2. Download & Extract Python
```sh
$ wget https://www.python.org/ftp/python/3.8.3/Python-3.8.3.tgz
$ tar -xzf Python-3.8.3.tgz
```

### 3. Compile Python Source & Check Installation:
```sh
$ cd Python-3.8.3
$ sudo ./configure --enable-optimizations
$ sudo make -j 8  # enter the number of cores of your system you want to use to speed up the build process.
$ sudo make altinstall
$ python3.8 --version
```

After you get the version output, remove the source file:

```sh
$ rm Python-3.8.3.tgz
```
