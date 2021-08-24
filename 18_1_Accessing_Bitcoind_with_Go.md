# 18.1: Accessing Bitcoind with Go

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This section explains how to interact with `bitcoind` using the Go programming language and the [btcd rpcclient](https://github.com/btcsuite/btcd/tree/master/rpcclient). Note that it has some quirks and some limitations.

## Set Up Go

To prepare for Go usage on your UNIX machine,  first install curl if you haven't already:

```
$ sudo apt install curl
```

Then, look at the [Go downloads page](https://golang.org/dl/), get the link for the latest download, and download it using `curl`. For a Debian setup, you will want to use the `linux-amd64` version:

```
$ curl -O https://dl.google.com/go/go1.15.1.linux-amd64.tar.gz
```

Once it finishes downloading, compare the hash of the download to the hash on the [Go downloads page](https://golang.org/dl/):

```
$ sha256sum go1.15.1.linux-amd64.tar.gz 
70ac0dbf60a8ee9236f337ed0daa7a4c3b98f6186d4497826f68e97c0c0413f6  go1.15.1.linux-amd64.tar.gz
```

The hashes should match. If so, extract the tarball and install Go on your system:
```
$ tar xfv go1.15.1.linux-amd64.tar.gz 
$ sudo chown -R root:root ./go
$ sudo mv go /usr/local
```

Now you need to create a Go path to specify your environment. Open the ```~/.profile``` file with an editor of your choice and add the following to the end of it:

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

### Set Up `btcd` `rpcclient`

You'll be using the `rpcclient` that comes with `btcd,` a Bitcoin implementation written in Go. Although `rpcclient` was originally designed to work with the `btcd` Bitcoin full node, it also works with Bitcoin Core. It has some quirks which we will be looking at. 

You can use ```go get``` to download it:

```
$ go get github.com/btcsuite/btcd/rpcclient
```

To test that it works, navigate to the directory with the Bitcoin Core examples:

```
$ cd $GOPATH/src/github.com/btcsuite/btcd/rpcclient/examples/bitcoincorehttp
```

Modify the ```main.go``` file and enter the details associated with your Bitcoin core setup, which can be found in ` ~/.bitcoin/bitcoin.conf`:

```
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "6305f1b2dbb3bc5a16cd0f4aac7e1eba",
```
> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

You can now run a test:
```
$ go run main.go
```

You should see the block count printed:

```
2020/09/01 11:41:24 Block count: 1830861
```

### Create a `rpcclient` Project

You will typically be creating projects in your `~/work/src/myproject/bitcoin` directory:
```
$ mkdir -p ~/work/src/myproject/bitcoin
$ cd ~/work/src/myproject/bitcoin
```
Each project should have the following `imports`:
```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
)
```
This ```import``` declaration allows you to import relevant libraries. For every example here, you will need to import ```"log", "fmt"``` and ```"github.com/btcsuite/btcd/rpcclient"```. You may need to import additional libraries for some examples. 
   * ```log``` is used for printing out error messages. After each time the Bitcoin node is called, an `if` statement will check if there are any errors. If there are errors, ```log``` is used to print them out. 
   * ```fmt``` is used for printing out output. 
   * ```rpcclient``` is obviously the `rpcclient` library

## Build Your Connection

Every `bitcoind` function in Go begins with creating the RPC connection, using the `ConnConfig` function:
```
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()
```
The ```connCfg``` parameters allow you to choose the Bitcoin RPC port, username, password and whether you are on testnet or mainnet. 

> **NOTE:** Again, be sure to substitute the `User` and `Pass` with the one found in your `~/.bitcoin/bitcon.conf`. 

The```rpcclient.New(connCfg, nil)``` function then configures ```client``` to connect to your Bitcoin node. 

The ```defer client.Shutdown()``` line is for disconnecting from your Bitcoin node, once the ```main()``` function finishes executing. After the ```defer client.Shutdown()``` line is where the exciting stuff goes — and it will be pretty easy to use. That's's because `rpcclient` helpfully turns the `bitcoin-cli` commands into functions using PascalCase. For example, ```bitcoin-cli getblockcount``` will be ```client.GetBlockCount``` in Go.

### Make an RPC Call

All that's required now is to make an informational call like `GetBlockCount` or `GetBlockHash` using your `client`:
```
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
```

### Make an RPC Call with Arguments

The `rpcclient` functions can take inputs as well; for example ```client.GetBlockHash(blockCount)``` takes the block count as an input. The ```client.GetBlockHash(blockCount)``` from above would look like this as a ```bitcoin-cli``` command:
```
$ bitcoin-cli getblockhash 1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```
However, a quirk with hashes in `rpcclient` is that they will typically print in a different encoding if you were to print then normally with ```blockHash```. In order to print them as a string, you need to use ```blockHash.String()```.

### Run Your Code

You can download the complete code from the [src directory](src/18_1_blockinfo.go).

You can then run:
```
$ go run blockinfo.go 
1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```

The latest block number along with its hash should be printed out.

## Look up Funds

Due to limitations of the `btcd` `rpcclient`, you can't make a use of the ```getwalletinfo``` function. However, you can make use of the `getbalance` RPC:

```
	wallet, err := client.GetBalance("*")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
```
```client.GetBalance("*")``` requires the ```"*"``` input, due to a quirk with `btcd`. The asterisk signifies that you want to get the balance of all of your wallets.

If you run [the src code](src/18_1_getbalance.go), you should get an output similar to this:
```
$ go run getbalance.go 
0.000689 BTC
```

## Create an Address

You can generate addresses in Go, but you can't specify the address type:

This requires the use of a special `chaincfg` function, to specify which network the addresses are being created for. This specification is only required during address generation, which is why it is only used in this example. You can include this in other examples as well, but it isn't necessary.

Be sure to import ```"github.com/btcsuite/btcd/chaincfg"```:
```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg"
)
```
Then call `connCfG` with the `chaincfg.TestNet3Params.Name` parameter:
```
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
```
> **MAINNET VS TESTNET:** ```Params: chaincfg.TestNet3Params.Name,``` should be ```Params: chaincfg.MainNetParams.Name,``` on mainnet.

You can then create your address:
```
	address, err := client.GetNewAddress("")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(address)
```
A quirk with ```client.GetNewAddress("")``` is that an empty string needs to be included for it to work.

Running [the source](17_1_getaddress.go) produces the following results:
```
$ go run getaddress.go 
tb1qutkcj34pw0aq7n9wgp3ktmz780szlycwddfmza
```

### Decode an Address

Creating an address took a look extra work, in specifying the appropiate chain. Using an address also will because you'll have to decode it prior to use. 

The means that you'll have to import both the ```"github.com/btcsuite/btcutil"``` and ```"github.com/btcsuite/btcd/chaincfg"``` libraries.
   * ```btcutil``` allows for a Bitcoin address to be decoded in a way that the`rpcclient` can understand. This is necessary when working with addresses in `rpcclient`. 
   * ```chaincfg``` is (again) used to configure your chain as the Testnet chain. This is necessary for address decoding since the addresses used on Mainnet and Testnet are different.
```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)
```
The defaultNet variable is now used to specify whether your Bitcoin node is on testnet or on mainnet. That information (and the `btcutil` object) is then used to decode the address.

> **MAINNET VS TESTNET:** ```&chaincfg.TestNet3Params``` should be ```&chaincfg.MainNetParams``` on mainnet.

```
	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
```

> **NOTE:** Change the address (`mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha`) for one actually your wallet; you can use `bitcoin-cli listunspent` to find some addresses with funds for this test. If you want to be really fancy, modify the Go code to take an argument, then write a script that runs `listunspent`, saves the info to a variable, and runs the Go code on that.

Only afterward do you use the `getreceivedbyaddress` RPC, on your decoded address:
```
	wallet, err := client.GetReceivedByAddress(addr)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
```
When you run [the code](src/18_1_getamountreceived.go), you should get output similar to:
```
$ go run getamountreceived.go 
0.0085 BTC
```

## Send a Transaction

You've now got all the puzzle pieces in place to send a transaction. You're going to want to:

1. Import the correct libraries, including `chaincfg` to specify a network and `btcutil` to decode an address.
2. Choose an address to send to.
3. Decode that address.
4. Run `sendtoaddress` to send funds the easy way.
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
		User:         "StandUp",
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
When you run [the code](src/18_1_sendtransaction.go), the txid of the transaction is outputted: 

```
$ go run sendtransaction.go
9aa4cd6559e0d69059eae142c35bfe78b71a8084e1fcc2c74e2a9675e9e7489d
```

### Look Up a Transaction

To lookup a transaction, such as the one you just sent, you'll need to once again do some conversions, this time of txid. ```"github.com/btcsuite/btcd/chaincfg/chainhash"``` is imported in order to allow hashes to be stored in the Go code. ```chainhash.NewHashFromStr("hash")``` converts a hash in a string to a format that works with rpcclient.

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
		User:         "StandUp",
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
> **NOTE:** Again, you'll want to change out the txid for one actually recognized by your system.

When you run [the code](17_1_lookuptransaction.go) it will print out the details associated with a transaction, such as its amount and how many times it has been confirmed:

```
$ go run lookuptransaction.go
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

## Summary: Accessing Bitcoind with Go

Although the `btcd` `rpcclient` has some limits, you can still perform the main RPC commands in Go. The documentation for `rpcclient` is available on [Godoc](https://godoc.org/github.com/btcsuite/btcd/rpcclient). If the documentation doesn't have what you're looking for, also consult the [btcd repository](https://github.com/btcsuite/btcd). It is generally well documented and easy to read. Based on these examples you should be able to incorporate Bitcoin in a Go project and do things like send and receive coins.

## What's Next?

Learn more about "Talking to Bitcoin in Other Languages" in [18.2: Accessing Bitcoin with Java](18_2_Accessing_Bitcoind_with_Java.md).
