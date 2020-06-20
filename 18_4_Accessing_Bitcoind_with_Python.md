# 18.4: Accessing Bitcoind with Python

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Interacting with the bitcoind directly and using command-line curl can get simple if you understand how it works, but there's a project [Python-BitcoinRPC](https://github.com/jgarzik/python-bitcoinrpc) that provides the functionality in a Python-API level, making it even easier to interact with your Bitcoin Server.

## Setup Python

We are going to be using python 3 for this chapter. Debian 10 installs python3 and if you have bitcoin-core installed, you should have it installed. Let's check, run:

`$ python3 --version`

If it returns the version number then you have python3 installed. If it returns nothing or an error then we will build python from source, do:

1. Install dependencies:
```sh
$ sudo apt-get install build-essential checkinstall
$ sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev
```

2. Download & extract Python:
```sh
$ wget https://www.python.org/ftp/python/3.8.3/Python-3.8.3.tgz
$ tar -xzf Python-3.8.3.tgz
```

3. Compile Python Source & Check Installation:
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

We are now ready to start interacting with `boitcond` but first we need to establish connection. Create a file called `btcrpc.py` and write the following:

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
rpc_client = AuthServiceProxy(f"http://{rpc_user}:{rpc_pass}@{rpc_host}:8332", timeout=240)
```

`pprint` is nifty library to pretty print the `json` response from `bitcoind`.

`logging` will print out the call we make to `bitcoind` and `bitcoind`'s respose, which is useful when we make a bunch of calls together. If you don't want the excessive output in the terminal just comment out the `logging` blok.

The arguments in the URL are `<rpc_username>:<rpc_password>@<host_IP_address>:<port>`. We covered the information with out work with `curl` . The IP address `127.0.0.1` and port `18332` should be correct for the standard testnet setup described in this documents, while you can extract the user and password from `~/.bitcoin/bitcoin.conf`. We specify the `timeout` argument since socket timeouts under heavy load. If you get `socket.timeout: timed out` response, be patient and increase the `timeout`.

> MAINNET VS TESTNET: The port would be 8332 for a mainnet setup.

If `rpc_client` is successfully initialized, you'll be able to send off RPC commands.

In order to use an RPC method using `python-bitcoinrpc`, you'll find that the `rpc_client` provides most of the functionality that can be accessed through `bitcoin-cli` or `curl`, using the same method names. For more details about the commands that you are able to execute and what to expect back, you should refer to [3.2: Knowing Your Bitcoin Setup](https://github.com/jodobear/Learning-Bitcoin-from-the-Command-Line/blob/master/03_2_Knowing_Your_Bitcoin_Setup.md).

Let's start with getting basic information about our node. Enter the following lines under `rpc_client`, save and run it in the terminal as `python3 bitcrpc.py`:

### Get Blockcount

```py
block_count = rpc_client.getblockcount()
print("---------------------------------------------------------------")
print("Node Block Height: ", block_count)
print("---------------------------------------------------------------\n")
```

You should see the following output with `logging` enabled :

```sh
DEBUG:BitcoinRPC:-3-> getblockcount []
DEBUG:BitcoinRPC:<-3- 635440
---------------------------------------------------------------
635440
---------------------------------------------------------------
```

### Get Wallet Info

```py
wallet_info = rpc_client.getwalletinfo()
print("---------------------------------------------------------------")
pprint(wallet_info)
print("---------------------------------------------------------------\n")
```

You should have an output similar to the following with `logging` disabled:

```sh
---------------------------------------------------------------
{'avoid_reuse': False,
 'balance': Decimal('1E-8'),
 'hdseedid': '25bc60acee2f850ed2098ad29z00e275591c4e94',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1539809952,
 'keypoolsize': 1000,
 'keypoolsize_hd_internal': 1000,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 2,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 159900}
---------------------------------------------------------------
```

Try out other commands like `getnetworkinfo`, `getpeerinfo`, `getblockchaininfo`, etc.

### Exploring a Block

Let's explore block `630000`, the first block after the third halving on May 11th.

```py
third_halving_blockhash = rpc_client.getblockhash(630000)
block_630000 = rpc_client.getblock(third_halving_blockhash)
print("---------------------------------------------------------------")
print("BLOCK No. 630000 :")
print("-------------")
print("Block Hash...: ", third_halving_blockhash)
print("Merkle Root..: ", block_630000['merkleroot'])
print("Block Size...: ", block_630000['size'])
print("Block Weight.: ", block_630000['weight'])
print("Nonce........: ", block_630000['nonce'])
print("Difficulty...: ", block_630000['difficulty'])
print("Number of Tx.: ", block_630000['nTx'])
print("First 10 transactions:")
print("---------------------")
i = 0
while i < 10:
    print(i, ":", block_630000['tx'][i])
    i += 1
print("---------------------------------------------------------------\n")
```

You should see the following output:

```sh
---------------------------------------------------------------
BLOCK No. 630000 :
-------------
Block Hash...:  000000000000000000024bead8df69990852c202db0e0097c1a12ea637d7e96d
Merkle Root..:  b191f5f973b9040e81c4f75f99c7e43c92010ba8654718e3dd1a4800851d300d
Block Size...:  1186930
Block Weight.:  3993250
Nonce........:  2302182970
Difficulty...:  16104807485529.38
Number of Tx.:  3134
First 10 transactions:
---------------------
0 : cc2ca076fd04c2aeed6d02151c447ced3d09be6fb4d4ef36cb5ed4e7a3260566
1 : 1e0aa1f5847d88c6ba8a6bd85c169151827ec841799e696f2697269390e005da
2 : 6ab6432a6b7b04ecc335c6e8adccc45c25f46e33752478f0bcacaf3f1b61ad92
3 : 40352adf6fba255e083c60a21f9f85774ce7c97017f542bf22c63be2ef9f366b
4 : bbf7f927234fc718b153e180b43892528437f135c471ba11ebd33390f2cb0dd7
5 : 3ea1a64c550ff91c6faba076aa776faa60aa524b48a54801d458d1c927333c8f
6 : e732ee5bf33069a6aaeb90cb461927e9a77c5132464a2abfbccc74b408604b92
7 : 384fa0ce9b5067e4cf5e8e44e017007ba38f2112bda65c9874f0d17c5e07f5e9
8 : d5458f1b5587e887e95bcf9f8431bc68ebb60b07a1475052bdeeeeb479937ee5
9 : 1438271ce2314944c5b2ce8ed89b7f2b0550ba91935403c240bee064e1af2a9b
---------------------------------------------------------------
```

## Working with Transactions

Let's create a transaction using our interface between python and bitcoind.

### List Unspent

Let's first list the UTXOs in our wallet

```py
utxos = rpc_client.listunspent()
print("---------------------------------------------------------------")
pprint(utxos)
print("---------------------------------------------------------------\n")
```

Your output will be similar to the following
```sh
[{'address': 'bc183409dkt7hqxscatr3u2spdnkv8amdjf850xmla13',
  'amount': Decimal('0.01'),
  'confirmations': 5050,
  'label': 'test',
  'safe': True,
  'scriptPubKey': '8837e44e000ujk3431d58e3c5402d9d987eed93kd0x',
  'solvable': True,
  'spendable': True,
  'txid': '39dskj00086fb9d10d99c05314d59888jmd666623158d755e7218666mmmdjk83',
  'vout': 0}
]
```

### Create a New Address

Now we create a new address for the change output. The `"Learning-Bitcoin-from-the-Command-Line"` is the label for the address. You should always label your addresses.

```py
new_address = rpc_client.getnewaddress("Learning-Bitcoin-from-the-Command-Line")
print("---------------------------------------------------------------")
print("New Address: ", new_address)
print("---------------------------------------------------------------\n")
```

`bitcoind` will generate a new address for you:

```sh
---------------------------------------------------------------
New Address:  3Nh6msiasUsInTEL5eaR9RudVdpqB1q7dI
---------------------------------------------------------------
```

### Setting Transaction Details

In the folowing code snippet we first select the UTXO which we want to spend. Then we get it's address, transaction id, vector index of the output and check the amount of bitcoins the utxo at the `vout`.

Then we add the recipent address to which we want to send the bitcoins, enter the amount of bitcoins we want to send, set miner fee and calculate the change amount.

```py
# Transaction Details
first_utxo = utxos[0]
utxo_address = first_utxo["address"]
utxo_txid = first_utxo["txid"]
vector_id = first_utxo["vout"]
utxo_amt = float(first_utxo["amount"])
recipient_address = "<Enter a recipent address here>" # e.g. bc1k3409dkt7hqxscatr3u3lsdnkv8amdjf85nd9ka13
recipient_amt = 0.0001
miner_fee = 0.00000300
change_address = new_address_02
change_amt = float((utxo_amt - recipient_amt) - miner_fee)
print("---------------------------------------------------------------")
print("Transaction Details:")
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

You should see similar output:

```sh
---------------------------------------------------------------
Transaction Details:
UTXO Txid..........:  39dskj00086fb9d10d99c05314d59888jmd666623158d755e7218666mmmdjk83
Vector ID of Output:  0
UTXO Amount........:  0.01
Tx Amount..........:  0.005
Recipient Address..:  bc1k3409dkt7hqxscatr3u3lsdnkv8amdjf85nd9ka13
Change Address.....:  bc1k3ks8dkt7hqxscatr3u3lsdnkv8amdjf85nd9kkx8
Miner Fee..........:  0.000003
Change Amount......:  0.00499700
---------------------------------------------------------------
```

### Create Raw Transacion

Now we have all the information to send a transaction, but before we can send one, we have to create a transaction, we do that by creating a raw transaction, like so:

```py
txid_vout = {"txid": utxo_txid, "vout": vector_id}
addresses_amts = {f"{recipient_address}": recipient_amt, f"{change_address}": change_amt}
create_raw_tx = rpc_client.createrawtransaction([txid_vout], addresses_amts)
tx_hash = create_raw_tx
print("---------------------------------------------------------------")
print("Transaction Hash: ", create_raw_tx)
print("---------------------------------------------------------------\n")
```

The format of the `createrawtransaction` command is:

`$ bitcoin-cli createrawtransaction '[{"txid": <utxo_txid>, "vout": <vector_id>}]' '{"<address>": <amount>}'`

Hence, we have constructed `txid_vout` and `addresses_amts` in this manner.

