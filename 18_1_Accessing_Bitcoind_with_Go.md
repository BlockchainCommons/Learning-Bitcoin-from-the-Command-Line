# 18.1: Accessing Bitcoind with Go

## Set up Go

First, install curl if you haven't already:

```
$ sudo apt install curl
```

Then, look at the [Go downloads page](https://golang.org/dl/) and get the link for the latest download:

```
$ curl -O https://dl.google.com/go/go1.14.4.linux-amd64.tar.gz
```

Once it finishes downloading, compare the hash of the download to the hash on the [Go downloads page](https://golang.org/dl/):

```
$ sha256sum go1.14.4.linux-amd64.tar.gz
aed845e4185a0b2a3c3d5e1d0a35491702c55889192bb9c30e67a3de6849c067  /home/user1/go1.14.4.linux-amd64.tar.gz
```

The hashes should match. Now we can extract the tarball and install Go on our system:

```
$ tar xvf go1.12.5.linux-amd64.tar.gz
$ sudo chown -R root:root ./go
$ sudo mv go /usr/local
```

Now we need to create a Go path to specify our environment. Open the ```~/.profile``` file with an editor of your choice and add the following to the end of it:

```
export GOPATH=$HOME/work
export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
```

Then, refresh your profile:

```
$ source ~/.profile
```

Lastly, create the directory for your Go workspace:

```
$ mkdir $HOME/work
```

## Setup btcd rpcclient

We will be using the rpcclient that comes with btcd, a Bitcoin implementation written in Go. Although rpcclient was originally designed to work with the btcd Bitcoin full node, it also works with Bitcoin core. It has some quirks which we will be looking at. We can use ```go get``` to download it:

```
$ go get github.com/btcsuite/btcd/rpcclient
```

To test that it works, navigate to the directory with the Bitcoin core examples:

```
$ cd $GOPATH/src/github.com/btcsuite/btcd/rpcclient/examples/bitcoincorehttp
```

Modify the ```main.go``` file and enter the details associated with your Bitcoin core setup:

```
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "d8340efbcd34e312044c8431c59c792c",
```
> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

Now run:

```
$ go run main.go
```

You should see the block count printed:

```
2020/06/24 20:41:01 Block count: 1773552
```

### Print the latest block

Leave the btcd directory and create a new Go project:

```
$ mkdir -p ~/work/src/myproject/bitcoin
$ cd ~/work/src/myproject/bitcoin
```

In this directory, create a file called ```main.go``` and enter the following in it:

```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	blockCount, err := client.GetBlockCount()
	if err != nil {
		log.Fatal(err)
	}
	blockHash, err := client.GetBlockHash(blockCount)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("%d\n", blockCount)
	fmt.Printf("%s\n", blockHash.String())
}
```

Make sure to change your username and password to those from your ```~/.bitcoin/bitcoin.conf``` file. 

The btcd rpcclient works by calling bitcoin-cli commands through in PascalCase. For example, ```bitcoin-cli getblockcount``` is ```client.GetBlockCount``` in Go. The ```connCfg``` parameters allow you to choose the Bitcoin RPC port, username, password and whether you are on testnet or mainnet. Using ```rpcclient.New(connCfg, nil)``` we configure ```client``` to connect to our Bitcoin node.

The ```import``` declaration at the top allows you to import relevant libraries. For every example here we will need to import ```"log", "fmt" and "github.com/btcsuite/btcd/rpcclient"```. We may need to import additional libraries for some examples. ```log``` is used for printing out error messages. After each time our Bitcoin node is called, we have an if statement checking if there are any errors. If there are errors, ```log``` is used to print them out. ```fmt``` is used for printing out our output. Since the examples in this document have different libraries to import, I will be showing the entire Go file, not just the specific function.

A quirk with hashes in rpcclient is that they will typically print in a different encoding if you were to print it normally with ```blockHash```. In order to print them as a string, we need to use ```blockHash.String()```.

We can run Go code with ```go run main.go```:

```
$ go run main.go
1773561
00000000000000346bab4126f418a5820692c9a7de7ef79717bebfccebacad61
```

The latest block number along with its hash should be printed out.

### Getting your wallet balance

Since the btcd rpcclient has some limits, we can't make a use of the ```getwalletinfo``` function, however we can get the balance of our wallet:

```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	wallet, err := client.GetBalance("*")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
}
```

You should get an output similar to this:

```
0.000689 BTC
```

```client.GetBalance("*")``` requires the ```"*"``` input, due to a quirk with btcd. The asterisk signifies that we want to get the balance of all our wallets.

### Generate an address

We can generate addresses, but we can't specify the address type:

```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
		Params: chaincfg.TestNet3Params.Name,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	address, err := client.GetNewAddress("")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(address)
}
```
> **MAINNET VS TESTNET:** ```Params: chaincfg.TestNet3Params.Name,``` should be ```Params: chaincfg.MainNetParams.Name,``` on mainnet.

You may notice that the ```connCfg``` clause is different in this example. ```Params: chaincfg.TestNet3Params.Name,``` is added here, so that the rpcclient knows that we are dealing with testnet addresses. You would change ```chaincfg.TestNet3Params.Name``` to ```chaincfg.MainNetParams.Name``` if you were on mainnet. This specification is only required during address generation, which is why I only kept it in this example. You can include this in other examples as well, but it isn't necessary. Make sure to import ```"github.com/btcsuite/btcd/chaincfg"``` if you are using it. 

A quirk with ```client.GetNewAddress("")``` is that an empty string needs to be included for it to work.

### Get amount received by an address

We can retrieve the amount of Bitcoin received by a specific address:

```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
	wallet, err := client.GetReceivedByAddress(addr)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
}
```
> **MAINNET VS TESTNET:** ```&chaincfg.TestNet3Params``` should be ```&chaincfg.TestNet3Params``` on mainnet.

You should get an output similar to this:

```
0.000089 BTC
```

The defaultNet variable is used to specify whether our Bitcoin node is on testnet or on mainnet. This example requires us to import the ```"github.com/btcsuite/btcutil"``` and ```"github.com/btcsuite/btcd/chaincfg"``` libraries. ```btcutil``` allows for a Bitcoin address to be decoded in a way that the rpcclient can understand. This is necessary when working with addresses in rpcclient. ```chaincfg``` is used to configure our chain as the Testnet chain. This is necessary for address decoding since the addresses used on Mainnet and Testnet are different.

### Sending Bitcoin to an address

```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
	sent, err := client.SendToAddress(addr, btcutil.Amount(1e4))
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(sent)
}
```

After the transaction is sent, the txid of the transaction is outputted: 

```
9aa4cd6559e0d69059eae142c35bfe78b71a8084e1fcc2c74e2a9675e9e7489d
```

### Lookup a transaction

```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg/chainhash"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	chash, err := chainhash.NewHashFromStr("1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df")
	if err != nil {
		log.Fatal(err)
	}
	transactions, err := client.GetTransaction(chash)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(transactions)
}
```

This prints out the details associated with a transaction, such as its amount and how many times it has been confirmed:

```
{
  "amount": 0.00100000,
  "confirmations": 4817,
  "blockhash": "000000006628870b0a8a66abea9cf0d4e815c491f079e3fa9e658a87b5dc863a",
  "blockindex": 117,
  "blocktime": 1591857418,
  "txid": "1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df",
  "walletconflicts": [
  ],
  "time": 1591857343,
  "timereceived": 1591857343,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha",
      "category": "receive",
      "amount": 0.00100000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "02000000000101e9e8c3bd057d54e73baadc60c166860163b0e7aa60cab33a03e89fb44321f8d5010000001716001435c2aa3fc09ea53c3e23925c5b2e93b9119b2568feffffff02a0860100000000001976a914600c8c6a4abb0a502ea4de01681fe4fa1ca7800688ac65ec1c000000000017a91425b920efb2fde1a0277d3df11d0fd7249e17cf8587024730440220403a863d312946aae3f3ef0a57206197bc67f71536fb5f4b9ca71a7e226b6dc50220329646cf786cfef79d60de3ef54f702ab1073694022f0618731902d926918c3e012103e6feac9d7a8ad1ac6b36fb4c91c1c9f7fff1e7f63f0340e5253a0e4478b7b13f41fd1a00"
}
```

```"github.com/btcsuite/btcd/chaincfg/chainhash"``` is imported in order to allow hashes to be stored in the Go code. ```chainhash.NewHashFromStr("hash")``` converts a hash in a string, to a format that works with rpcclient.

## Summary: Accessing Bitcoind with Go

Although the btcd rpcclient has some limits, we can still perform the main rpc commands in Go. The documentation for rpcclient is available on [Godoc](https://godoc.org/github.com/btcsuite/btcd/rpcclient). If the documentation doesn't have what your looking for, have a look at the [btcd repository](https://github.com/btcsuite/btcd). It is generally well documented and easy to read. Based on these examples you should be able to incorporate Bitcoin in a Go project and do things like sending and receiving coins.