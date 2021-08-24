package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
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

	wallet, err := client.GetBalance("*")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
}
