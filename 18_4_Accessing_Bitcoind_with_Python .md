# 18.4: Accessing Bitcoind with Python

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Interacting with the bitcoind directly and using command-line curl can get simple if you understand how it works, but there's a project [Python-BitcoinRPC](https://github.com/jgarzik/python-bitcoinrpc) that provides the functionality in a Python-API level, making it even easier to interact with your Bitcoin Server.

## Setup Python

We are going to be using python 3 for this chapter. Debian 10 installs python3 and if you have bitcoin-core installed, you should have it installed. Let's check, run:

`$ python3 --version`

If it returns the version number then you have python3 installed. If it returns nothing or an error then we will build python from source, do:

#### 1. Install dependencies:
```sh
$ sudo apt-get install build-essential checkinstall
$ sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev
```

#### 2. Download & extract Python:
```sh
$ wget https://www.python.org/ftp/python/3.8.3/Python-3.8.3.tgz
$ tar -xzf Python-3.8.3.tgz
```

#### 3. Compile Python Source & Check Installation:
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

## Setup Dependency

We are going to install the `python-bitcoinrpc` library, like so:

`python3.8 -m pip install python-bitcoinrpc`

## Build Your Connection

We are now ready to start interacting with `boitcond`, but first we need to establish connection. Create a file called `btcrpc.py` and type the following:

```py
from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
from pprint import pprint
import logging

logging.basicConfig()
logging.getLogger("BitcoinRPC").setLevel(logging.DEBUG)
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = "<ENTER_THE_RPC_USER_FROM_bitcoin.conf>"
rpc_pass = "<ENTER_THE_RPC_PASSWORD_FROM_bitcoin.conf>"
rpc_host = "127.0.0.1"
rpc_client = AuthServiceProxy(f"http://{rpc_user}:{rpc_pass}@{rpc_host}:18332", timeout=120)
```

`pprint` is nifty library to pretty print the `json` response from `bitcoind`.

`logging` will print out the call we make to `bitcoind` and `bitcoind`'s respose, which is useful when we make a bunch of calls together. If you don't want the excessive output in the terminal just comment out the `logging` blok.

The arguments in the URL are `<rpc_username>:<rpc_password>@<host_IP_address>:<port>`. We covered the information in our work with `curl` . The IP address `127.0.0.1` and port `18332` should be correct for the standard testnet setup described in these documents, while you can extract the user and password from `~/.bitcoin/bitcoin.conf`. We specify the `timeout` argument since socket timeouts under heavy load on the mainnet. If you get `socket.timeout: timed out` response, be patient and increase the `timeout`.

> MAINNET VS TESTNET: The port would be 8332 for a mainnet setup.

If `rpc_client` is successfully initialized, you'll be able to send off RPC commands to your bitcoin node.

In order to use an RPC method using `python-bitcoinrpc`, you'll find that the `rpc_client` provides most of the functionality that can be accessed through `bitcoin-cli` or `curl`, using the same method names. For more details about the commands that you are able to execute and what to expect back, you should refer to [3.2: Knowing Your Bitcoin Setup](https://github.com/jodobear/Learning-Bitcoin-from-the-Command-Line/blob/master/03_2_Knowing_Your_Bitcoin_Setup.md).

## Getting Basic Info

Let's start with getting basic information about our node. Enter the following lines under `rpc_client`, save and run it in the terminal as `python3 bitcrpc.py`:

### Get Blockcount

Let's get the blockcount of our node:

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

### Get Wallet Info

Let's get details of our wallet:

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

Try out other commands like `getblockchaininfo`, `getnetworkinfo`, `getpeerinfo`, `getblockchaininfo`, etc.

### Exploring a Block

Let's explore a block on the testnet. First we get the blockhash of a block we want so that we ask bitcoind to return the details of that block. Then we print some details of that block along with a few transactions.

```py
blockhash_630000 = rpc_client.getblockhash(630000)
block_630000 = rpc_client.getblock(blockhash_630000)
nTx = block_630000['nTx']
if nTx > 10:
    it_txs = 10
    list_tx_heading = "First 10 transactions: "
else:
    it_txs = nTx
    list_tx_heading = f"All the {it_txs} transactions: "
print("---------------------------------------------------------------")
print("BLOCK No. 630000 :")
print("-------------")
print("Block Hash...: ", blockhash_630000)
print("Merkle Root..: ", block_630000['merkleroot'])
print("Block Size...: ", block_630000['size'])
print("Block Weight.: ", block_630000['weight'])
print("Nonce........: ", block_630000['nonce'])
print("Difficulty...: ", block_630000['difficulty'])
print("Number of Tx.: ", nTx)
print(list_tx_heading)
print("---------------------")
i = 0
while i < it_txs:
    print(i, ":", block_630000['tx'][i])
    i += 1
print("---------------------------------------------------------------\n")
```

You should see the following output:

```sh
---------------------------------------------------------------
BLOCK No. 630000 :
-------------
Block Hash...:  0000000000007c21a9190d3eb43517032ba0077229cbff4e6a2a43357011bebb
Merkle Root..:  a8dc7bf722a4ca6869d99fea465fc65dbf68df07026a4142f4b8213d3749ed7f
Block Size...:  4519
Block Weight.:  18076
Nonce........:  265038597
Difficulty...:  126320.64
Number of Tx.:  3
All the 3 transactions:
---------------------
0 : b4d2c5442554ebcdfcbf08a71cf2c510d21acac48f93fc2802b8bc52c5cebfeb
1 : c3f8177522be84bc023e61e53b9bbff674e1f825fc9d68c3f4c2e21a2161d5a9
2 : 4ab80958fea242285056ffcf88f7d65b65d357f761796104109839ad7da2255f
---------------------------------------------------------------
```

## Exploring an Address

How about getting a list of all the transactions of your address? Type the following and see what track your address:

```py
track_address = "<your address>"
tx_list = rpc_client.listtransactions()
address_tx_list = []
for tx in tx_list:
    if tx['address'] == track_address:
        address_tx_list.append(tx)
pprint(address_tx_list)
```

## Exploring a UTXO

To get details of a utxo, we first hed to get it's `hex` id, which we can then decode using `decoderawtransaction`.

```py
print("Exploring UTXOs")
## List Utxos
utxos = rpc_client.listunspent()
print("Utxos: ")
print("-----")
pprint(utxos)
print("------------------------------------------\n")
## Select a UTXO - first one selected here
utxo_txid = utxos[0]['txid']
## Get UTXO Hex
utxo_hex = rpc_client.gettransaction(utxo_txid)['hex']
## Get tx Details
utxo_tx_details = rpc_client.decoderawtransaction(utxo_hex)
print("Details of Utxo with txid:", utxo_txid)
print("---------------------------------------------------------------")
print("UTXO Details:")
print("------------")
pprint(utxo_tx_details)
print("---------------------------------------------------------------\n")
```

## Sending Transactions

Now that we have established comfort with our python interface with bitcoind, lets create a transaction. For the purposes of this course we will send test bitcoins to an address generated by us in our testnet wallet. Here's how we will do it:

1. Create 2 addresses, one that will act as recipient and the other for change.
2. Select a UTXO and set transaction details.
3. Create a raw transaction.
4. Sign the raw transaction with private key of the UTXO.
5. Broadcast the transaction on the bitcoin testnet.

### 1. Create New Addresses

```py
print("Creating a Transaction")
## Create New Addresses
new_address_01 = rpc_client.getnewaddress("Learning-Bitcoin-from-the-Command-Line-01")
new_address_02 = rpc_client.getnewaddress("Learning-Bitcoin-from-the-Command-Line-02")
```

### 2. Select UTXO & Set Transaction Details

In the folowing code snippet we first select the UTXO which we want to spend. Then we get it's address, transaction id, vector index of the output and check the amount of bitcoins the utxo at the `vout`.

Then we add the recipent address to which we want to send the bitcoins, enter the amount of bitcoins we want to send, set miner fee and calculate the change amount.

```py
utxos = rpc_client.listunspent()
second_utxo = utxos[1]
utxo_address = second_utxo["address"]
utxo_txid = second_utxo["txid"]
vector_id = second_utxo["vout"]
utxo_amt = float(second_utxo["amount"])
# here we are sending bitcoins to an address generated by us in out own wallet.
recipient_address = new_address_01
recipient_amt = utxo_amt / 2
miner_fee = 0.00000200
change_address = new_address_02
change_amt = float((utxo_amt - recipient_amt) - miner_fee)
print("---------------------------------------------------------------")
print("Transaction Details:")
print("-------------------")
print("UTXO Address.......: ", utxo_address)
print("UTXO Txid..........: ", utxo_txid)
print("Vector ID of Output: ", vector_id)
print("UTXO Amount........: ", utxo_amt)
print("Tx Amount..........: ", recipient_amt)
print("Recipient Address..: ", recipient_address)
print("Change Address.....: ", change_address)
print("Miner Fee..........: ", '%f'%(miner_fee))
print("Change Amount......: ", change_amt)
print("---------------------------------------------------------------\n")
```

Your output will be similar to the following:

```sh
---------------------------------------------------------------
Transaction Details:
-------------------
UTXO Address.......:  2NHkixaDUBQ8qz7oV3obwWbYXeRzsJwYTg
UTXO Txid..........:  b90sl97lfc2bfc0f16761fd1bb5f27f86e88eab440f5578fefe47143b5e4232b
Vector ID of Output:  1
UTXO Amount........:  0.01
Tx Amount..........:  0.005
Recipient Address..:  2NOLScQAYV3QH21my3kXrmn1ykTnTWXBPZ6
Change Address.....:  2NABxEDoVs85b9owihT8HGNFSnPPJTDVsFC
Miner Fee..........:  0.000002
Change Amount......:  0.004998
---------------------------------------------------------------
```

### 3. Create Raw Transacion

Now we have all the information to send a transaction, but before we can send one, we have to create a transaction, we do that by creating a raw transaction, like so:

```py
txid_vout = {"txid": utxo_txid, "vout": vector_id}
addresses_amts = {f"{recipient_address}": recipient_amt, f"{change_address}": change_amt}
create_raw_tx = rpc_client.createrawtransaction([txid_vout], addresses_amts)
unsigned_tx_hex = create_raw_tx
print("---------------------------------------------------------------")
print("Unsigned Transaction Hex: ", unsigned_tx_hex)
print("---------------------------------------------------------------\n")
```

This will return the hex of the raw transaction:

```sh
---------------------------------------------------------------
Unsigned Transaction Hex:  02000000012b23e4b54371e4ef8f57f540b4ea88slk8275fbbd11f76160ffc2bfc10da18b80100000000ffffffff0220a107000000000017a914e573193a39esl9ld930127551fcef86aaa47f82f8758a007000000000017a914ls90d0497b0fef38642f64ba0c8148adf0fb1d0c8700000000
---------------------------------------------------------------
```

The format of the `createrawtransaction` command is:

`$ bitcoin-cli createrawtransaction '[{"txid": <utxo_txid>, "vout": <vector_id>}]' '{"<address>": <amount>}'`

Hence, we have constructed `txid_vout` and `addresses_amts` in this manner.

Try checking the details of the transaction, hint: use `decoderawtransaction`.

### 4. Sign Raw Transaction

Now that we have a raw transaction, we use it's hex and the private key of the utxo we are spending to sign the transaction like so:

```py
address_priv_key = []
address_priv_key.append(rpc_client.dumpprivkey(utxo_address))
sign_tx = rpc_client.signrawtransactionwithkey(unsigned_tx_hex, address_priv_key)
print("---------------------------------------------------------------")
print("Signed Transaction: ")
print("----------------------")
pprint(signed_tx)
print("---------------------------------------------------------------\n")
```
This returns a json object with the signed transaction's hex and whether it was completed successfully or not:

```sh
---------------------------------------------------------------
Signed Transaction:
----------------------
{'complete': True,
 'hex': '020000000001012b23e4b5438904ef8f57f540b4ea886ef8275fbbd11f76160ffc2bfc10da18b80100000017160014c5d04b36b7b45eb9c5e4716272e3c955a6041fc2ffffffff0220a107000000000017a914e573193a39ed74dd930127551fcef86aaa47f82f8758a007000000000017a914e7d0osl97b0fef38642f64ba0c8148adf0fb1d0c8702463043021f4e2855813369bbf08288fa8bc670a48c1b27873646296c5cdabd50ee7790810220516f03c20a72f8985fd3bd5549dkfc68993909c28ffeffb5d277456f65384e60012103fa812ff869d1839e1b1d5c9532413b0ls2dcd21c334e727d9ee221c7a151591a400000000'}
---------------------------------------------------------------
```

### Broadcast Transaction

Finally, we are now ready to broadcast our signed transaction on the bitcoin network like so:

```py
send_tx = rpc_client.sendrawtransaction(signed_tx_hex['hex'])
print("---------------------------------------------------------------")
print("TXID of sent transaction: ", send_tx)
print("---------------------------------------------------------------\n")
```

Once the transaction has been broadcasted, bitcoind will return it's txid:

```sh
---------------------------------------------------------------
TXID of sent transaction: 'k9db702lsd512e2421915dc53clkj28f39a987c9a91cc0514faacfef500c6764'
---------------------------------------------------------------
```

## Summary

In this chapter we learnt how to connect to a node, get some basic information about our node and even sent a transaction over testnet. Accessing Bitcoind with Python is very easy while using the `python-bitcoinrpc` library. The first thing to always do is to establish connection with your bitcoind instance then, you can basically call all the bitcoin API calls as described in the bitcoin-core documentation. This makes it really easy to create small or large scripts to manage your own node, check balances or create cool applications on top as you get the full power of `bitcoin-cli`.

All the source code for this chapter is available in the [src](./src/18_4_accessing_bitcoind_with_python.py) directory of the repo.