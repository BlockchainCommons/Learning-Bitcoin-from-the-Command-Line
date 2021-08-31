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
