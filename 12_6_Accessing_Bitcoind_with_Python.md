# 12.6: Accessing Bitcoind with Python

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Though command-line `curl` is the easiest way to access the `bitcoind` directly, there are many other options for doing so and most of them support more fully featured programming languages. One package for doing so in Python is [Bitcoin authproxy](https://github.com/bitcoin/bitcoin/blob/master/test/functional/test_framework/authproxy.py). It was originally [written by Jeff Garzik](https://github.com/jgarzik/python-bitcoinrpc) but is now maintained by Bitcoin Core as the primary interface for the QA testing suite. It uses an http client module to access the `bitcoind` RPC server.

## Set Up Bitcoin AuthProxy

The latest Bitcoin Core version of `authproxy` requires Python 3, but you can still use jgarzik's original script to run in Python 2. Download the script to your working directory, start Python, and import the module. This example uses Python 3 running on Linux:
```
$ python3
>>> from authproxy import AuthServiceProxy, JSONRPCException
```
Instantiate an `AuthServiceProxy` object with the same localhost IP address, port, username and password you set in `bitcoin.conf`. For exmaple if these are the lines in `bitcoin.conf`:
```
rpcuser=bitcoinrpc
rpcpassword=1d446e2e45e96aaba6dea515871cc497
```
your Python command would be:
```
>>> rpc_connection = AuthServiceProxy("http://%s:%s@127.0.0.1:8332"%("bitcoinrpc","1d446e2e45e96aaba6dea515871cc497"))

```

### Make an RPC Call

Now that we have our RPC connection set up via the proxy, RPC calls as easy to make as if you were just calling `bitcoin-cli` form the command line. AuthProxy returns a Python dict that may contain decimals, so they are not super easy to convert directly to JSON. We'll make a few generic calls to `bitcoind` to get some network statistics:
```
>>> rpc_connection.getmempoolinfo()
{'usage': 42593984, 'bytes': 21357852, 'size': 17809, 'mempoolminfee': Decimal('0.00078966'), 'maxmempool': 50000000}
>>> rpc_connection.getbestblockhash()
'00000000000000000033a05c84da138f2a6779f34671082cfd4b9f586054c65c'
```
You can even use this interface to run wallet commands, and add paramaters to the RPC call. Here we ask `bitcoind` for a new address, then send that address back to `bitcoind` to get the associated private key. (In this example, the wallet is not encrypted!):
```
>>> rpc_connection.getnewaddress()
'1BFcCbjDPBoCwR76PCFEeTRZEVfKAwP11v'
>>> rpc_connection.dumpprivkey('1BFcCbjDPBoCwR76PCFEeTRZEVfKAwP11v')
'L4dnpfiZ69PKsKyym5o7HvoftvHjw1z2QkojQBwYRPUuaWgzTnNS'
```
If your wallet was encrypted, you would have to unlock it first by sending it your wallet passphrase and a length of time (in seconds) that `bitcoind` will keep the wallet unlocked in memory:
```
>>> rpc_connection.walletpassphrase('TOTALLY-SECRET-PASSPHRASE',60)
```

## Appendix I: Monitoring the mempool size in real-time with a loop

Here's the complete code for a Python script that will get the mempool size from `bitcoind` every second and print the output to the console.
```
import os
import time
from authproxy import AuthServiceProxy, JSONRPCException

rpc_connection = AuthServiceProxy("http://%s:%s@127.0.0.1:8332"%("bitcoinrpc","1d446e2e45e96aaba6dea515871cc497"))

while True:
  info =  rpc_connection.getmempoolinfo()
  os.system('clear')
  print ("The current mempool size in bytes is:", info['bytes'])
  print ("There are", info['size'], "transactions in the mempool")
  time.sleep(1)
```
Save the above code to a file called "MempoolMonitor.py" and run it as follows:
```
$ python3 MempoolMonitor.py
```
(you can exit by interupting Python with Ctrl+C) 


## Appendix II: Using `blocknotify` to run a python script for every new block

`bitcoind` has a feature that will run a system command every time tip of the blockchain is updated (every time a new block is mined and added to the chain). You can start `bitcoind` with a `-blocknotify=...` command, but it is easier to just include it in your conf file at `~/.bitcoin/bitcoin.conf`. The `%s` is replaced by the new block's hash:
```
blocknotify = python3 /home/USERNAME/FUN-WITH-BITCOIN/block.py %s
```
Maybe you just want to keep a text file of all the new block hashes? Edit the file at `/home/USERNAME/FUN-WITH-BITCOIN/block.py` to include:
```
from sys import argv

command, hash = argv

with open("hash_list.txt", "a") as logfile:
    logfile.write(hash)
```
