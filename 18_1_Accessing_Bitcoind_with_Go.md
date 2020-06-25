# 18.1: Accessing Bitcoind with NodeJS

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

We will be using the rpcclient that comes with btcd, a Bitcoin implementation written in Go. We can use ```go get``` to download it:

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

Make sure to change your username and password to those from your ```~/.bitcoin/bitcoin.conf``` file. When you run this:

```
$ go run main.go
1773561
00000000000000346bab4126f418a5820692c9a7de7ef79717bebfccebacad61
```

The latest block number along with its hash should be printed out.

### Getting your wallet balance

Since the btcd rpcclient is quite limited, we can't make a use the ```getwalletinfo``` functino, however we can get the balance of our wallet:

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

	fmt.Println(wallet)
}
```

The defaultNet variable is used to specify whether our Bitcoin node is on testnet or on mainnet.

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

This outputs the txid of the transaction.

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

## Summary: Accessing Bitcoind with Node

Although the btcd rpcclient is quite limited, we can still perform a lot of the main rpc commands in Go. 