
========
Note to Shannon, the following transactions is what we eventually want to teach people how
to do by hand in Part Two on raw transactions (part one just being chain status and simple
wallet commands) initially on testnet3 (this OP_RETURN is a mainnet transaction).

For what the transaction looks like, see
see https://live.blockcypher.com/btc/tx/93d8f6a2e66aaabf12a136a9b5f7a4f947796030e2906303d1a0e21a29301b03/

Basically is a P2PHK (Pay to Public Key Hash) transaction that has an OP_RETURN in it.

There is an existing tutorial on how to do some of this at
https://bitcoin.org/en/developer-examples#transaction-tutorial but not the OP_RETURN part.

We also want to do this with a full testnet with txindex=1 rather than regtest (which is
useful for another section later to demonstrate some blocks and mining features).

BTW, if the the below OP_RETURN had a URL or IPFS hash it could serve as a root DID for
an identity. Double purpose!

```
$ bc getrawtransaction 93d8f6a2e66aaabf12a136a9b5f7a4f947796030e2906303d1a0e21a29301b03
0100000001a862008ab5d3509ca2137c26b0339defbe0d45f691678d551e8d3392d8e4585d000000008b48304502210082a5b6fbeb48bfde2d6cc1243a345ec6a13b2c372d976418be250ca411380c39022047dac47d4dcf7ee74ac2c2640f09e0621437454163fb618e2674ff2fd41f9df001410433e05b29670f19cbc499f207f11abe1c69f77f00d5cbb9dbec5b5fe7527e2f41fa1e90f10a05e9c0a34d255988082e190c9ee7ea05f62297d4f76d9b61d7561bffffffff0336150000000000001976a9140fcbb35c04a57663a974c2cbae6554956503f11688ac00000000000000001e6a1c0d0ccf307c4594a8e55bcd09e4aaf2c8bfe77430b09e76f9e05506e410270000000000001976a91405b5fca463964fc8af5ce743e1cfc1102601ecfc88ac00000000

$ bc getrawtransaction 93d8f6a2e66aaabf12a136a9b5f7a4f947796030e2906303d1a0e21a29301b03 1
{
  "hex": "0100000001a862008ab5d3509ca2137c26b0339defbe0d45f691678d551e8d3392d8e4585d000000008b48304502210082a5b6fbeb48bfde2d6cc1243a345ec6a13b2c372d976418be250ca411380c39022047dac47d4dcf7ee74ac2c2640f09e0621437454163fb618e2674ff2fd41f9df001410433e05b29670f19cbc499f207f11abe1c69f77f00d5cbb9dbec5b5fe7527e2f41fa1e90f10a05e9c0a34d255988082e190c9ee7ea05f62297d4f76d9b61d7561bffffffff0336150000000000001976a9140fcbb35c04a57663a974c2cbae6554956503f11688ac00000000000000001e6a1c0d0ccf307c4594a8e55bcd09e4aaf2c8bfe77430b09e76f9e05506e410270000000000001976a91405b5fca463964fc8af5ce743e1cfc1102601ecfc88ac00000000",
  "txid": "93d8f6a2e66aaabf12a136a9b5f7a4f947796030e2906303d1a0e21a29301b03",
  "hash": "93d8f6a2e66aaabf12a136a9b5f7a4f947796030e2906303d1a0e21a29301b03",
  "size": 297,
  "vsize": 297,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "5d58e4d892338d1e558d6791f6450dbeef9d33b0267c13a29c50d3b58a0062a8",
      "vout": 0,
      "scriptSig": {
        "asm": "304502210082a5b6fbeb48bfde2d6cc1243a345ec6a13b2c372d976418be250ca411380c39022047dac47d4dcf7ee74ac2c2640f09e0621437454163fb618e2674ff2fd41f9df0[ALL] 0433e05b29670f19cbc499f207f11abe1c69f77f00d5cbb9dbec5b5fe7527e2f41fa1e90f10a05e9c0a34d255988082e190c9ee7ea05f62297d4f76d9b61d7561b",
        "hex": "48304502210082a5b6fbeb48bfde2d6cc1243a345ec6a13b2c372d976418be250ca411380c39022047dac47d4dcf7ee74ac2c2640f09e0621437454163fb618e2674ff2fd41f9df001410433e05b29670f19cbc499f207f11abe1c69f77f00d5cbb9dbec5b5fe7527e2f41fa1e90f10a05e9c0a34d255988082e190c9ee7ea05f62297d4f76d9b61d7561b"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00005430,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 0fcbb35c04a57663a974c2cbae6554956503f116 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a9140fcbb35c04a57663a974c2cbae6554956503f11688ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "12SXAFChN9A9uwWKN7nYVtVudEmn9tkf4V"
        ]
      }
    },
    {
      "value": 0.00000000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_RETURN 0d0ccf307c4594a8e55bcd09e4aaf2c8bfe77430b09e76f9e05506e4",
        "hex": "6a1c0d0ccf307c4594a8e55bcd09e4aaf2c8bfe77430b09e76f9e05506e4",
        "type": "nulldata"
      }
    },
    {
      "value": 0.00010000,
      "n": 2,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 05b5fca463964fc8af5ce743e1cfc1102601ecfc OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91405b5fca463964fc8af5ce743e1cfc1102601ecfc88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "1XCPdWb6kk7PGfvbdRbRuNh51aPc4vqC7"
        ]
      }
    }
  ],
  "blockhash": "000000000000000002a6c3823505ec646d223ea64b576f12cf5a0de90bf73b53",
  "confirmations": 2081,
  "time": 1486150869,
  "blocktime": 1486150869
}
```

End of Part 2 is doing a simple P2PKH transaction with an op_return. Part 3 are multisig
transactions excluding P2SH. Part 4 is P2SH and its implications. Part 5 is the witness
variants of the standard transactions. Part 5 will about the possbilities for various
new scripts using the new opcodes OP_CHECKSEQUENCEVERIFY and OP_CHECKLOCKTIMEVERIFY,
and a little bit about non-standard scripts
https://medium.com/@alcio/a-look-at-bitcoin-non-standard-outputs-c97f65cccbb6#.1150nicbx
https://medium.com/@alcio/non-standard-p2sh-scripts-508fa6292df5#.2xzcllsr4
http://www.quantabytes.com/articles/a-survey-of-bitcoin-transaction-types



================

# Bitcoin-CLI Part One: The state of the chain and wallet

## Bitcoin related aliases

First we want to set up some aliases to make it easier to work with testnet.

bc* aliases are for real bitcoin on mainnet
bt* aliases are for experimenting with testnet coins
br* aliases are for regtest, great for experimenting with blocks and mining


```
alias bd="bitcoind"

alias bcstart="bitcoind -daemon -conf='~/Library/Application\ Support/Bitcoin'""
alias btstart="bitcoind -testnet -daemon -conf='~/Library/Application\ Support/Bitcoin/testnet3/bitcoin.conf' -datadir='~/Library/Application\ Support/Bitcoin/testnet3/bitcoin.conf'"
alias brstart="bitcoind -regtest -daemon"

alias bcstop="bitcoin-cli stop"
alias btstop="bitcoin-cli -testnet stop"
alias brstop="bitcoin-cli -regtest -stop"

alias bcdir="cd ~/.bitcoin/" #linux default bitcoin path
#alias btdir="cd ~/.bitcoin/testnet" #linux default bitcoin testnet path
#alias brdir="cd ~/.bitcoin/regtest" #linux default bitcoin regtest path

# alias bcdir="cd ~/Library/Application\ Support/Bitcoin/" #mac default bitcoind path
# alias btdir="cd ~/Library/Application\ Support/Bitcoin/testnet/" #mac default testnet path
# alias brdir="cd ~/Library/Application\ Support/Bitcoin/regtest/" #mac default regtest path

alias bc="bitcoin-cli"
#alias bt="bitcoin-cli -testnet"
#alias br="bitcoin-cli -regtest"

alias bcinfo='bitcoin-cli getinfo | egrep "\"version\"|balance|blocks|connections|errors"'
alias btinfo='bitcoin-cli -testnet getinfo | egrep "\"version\"|balance|blocks|connections|errors"'
alias brinfo='bitcoin-cli -regtest getinfo | egrep "\"version\"|balance|blocks|connections|errors"'

alias bcblock="echo \`bitcoin-cli getblockcount 2>&1\`/\`wget -O - http://blockchain.info/q/getblockcount 2>/dev/null\`"
alias btblock="echo \`bitcoin-cli -testnet getblockcount 2>&1\`/\`wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev\`"
alias brblock="bitcoin-cli -regtest getblockcount 2>&1"
```

In general, before using a bitcoin-cli command, take a look at help on it. It tells you
more about how the command works, the options and annotated results, often includes a
number of examples, including how to call the command using CURL using the RPC interface.

```
$ bt help getinfo
getinfo
Returns an object containing various state info.

Result:
{
  "version": xxxxx,           (numeric) the server version
  "protocolversion": xxxxx,   (numeric) the protocol version
  "walletversion": xxxxx,     (numeric) the wallet version
  "balance": xxxxxxx,         (numeric) the total bitcoin balance of the wallet
  "blocks": xxxxxx,           (numeric) the current number of blocks processed in the server
  "timeoffset": xxxxx,        (numeric) the time offset
  "connections": xxxxx,       (numeric) the number of connections
  "proxy": "host:port",     (string, optional) the proxy used by the server
  "difficulty": xxxxxx,       (numeric) the current difficulty
  "testnet": true|false,      (boolean) if the server is using testnet or not
  "keypoololdest": xxxxxx,    (numeric) the timestamp (seconds since Unix epoch) of the oldest pre-generated key in the key pool
  "keypoolsize": xxxx,        (numeric) how many new keys are pre-generated
  "unlocked_until": ttt,      (numeric) the timestamp in seconds since epoch (midnight Jan 1 1970 GMT) that the wallet is unlocked for transfers, or 0 if the wallet is locked
  "paytxfee": x.xxxx,         (numeric) the transaction fee set in BTC/kB
  "relayfee": x.xxxx,         (numeric) minimum relay fee for non-free transactions in BTC/kB
  "errors": "..."           (string) any error messages
}

Examples:
> bitcoin-cli getinfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getinfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/

$ bt getinfo
{
  "version": 130200,
  "protocolversion": 70015,
  "walletversion": 130000,
  "balance": 7.62651233,
  "blocks": 1089336,
  "timeoffset": 0,
  "connections": 8,
  "proxy": "",
  "difficulty": 1,
  "testnet": true,
  "keypoololdest": 1487117744,
  "keypoolsize": 100,
  "paytxfee": 0.00000000,
  "relayfee": 0.00010800,
  "errors": "Warning: unknown new rules activated (versionbit 28)"
}
```

TBD: explain a few of the more important getinfo fields

Next, we are going to look at our wallet, and going to demonstrate a variety of wallet features

```
$ bt help getwalletinfo
getwalletinfo
Returns an object containing various wallet state info.

Result:
{
  "walletversion": xxxxx,       (numeric) the wallet version
  "balance": xxxxxxx,           (numeric) the total confirmed balance of the wallet in BTC
  "unconfirmed_balance": xxx,   (numeric) the total unconfirmed balance of the wallet in BTC
  "immature_balance": xxxxxx,   (numeric) the total immature balance of the wallet in BTC
  "txcount": xxxxxxx,           (numeric) the total number of transactions in the wallet
  "keypoololdest": xxxxxx,      (numeric) the timestamp (seconds since Unix epoch) of the oldest pre-generated key in the key pool
  "keypoolsize": xxxx,          (numeric) how many new keys are pre-generated
  "unlocked_until": ttt,        (numeric) the timestamp in seconds since epoch (midnight Jan 1 1970 GMT) that the wallet is unlocked for transfers, or 0 if the wallet is locked
  "paytxfee": x.xxxx,           (numeric) the transaction fee configuration, set in BTC/kB
  "hdmasterkeyid": "<hash160>", (string) the Hash160 of the HD master pubkey
}

Examples:
> bitcoin-cli getwalletinfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getwalletinfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/

$ bt getwalletinfo
{
  "walletversion": 130000,
  "balance": 0.00000000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 0,
  "keypoololdest": 1487117744,
  "keypoolsize": 100,
  "paytxfee": 0.00000000,
  "hdmasterkeyid": "1ca5426f74d261e90b196ca12ed358fff10ab38e"
}
```

From this we see that we have a new wallet with a root HD master key of "", 100 pregenerated
coins from that root, and there is no balance of testnet coins in your wallet. (Also, your
wallet was generated on 1487117744 (TBD: date conversion)). TBD: explain some more

There are a couple of other useful general wallet commands:

(TBD)
== Wallet ==
backupwallet "destination"
dumpwallet "filename"
encryptwallet "passphrase"
importwallet "filename"




# Bitcoin-CLI Part One: The state of the chain and wallet



Next we are going to get a new address, and introduce a command line trick
to make future work in the command line interface easier.

```
bt help getnewaddress
getnewaddress ( "account" )

Returns a new Bitcoin address for receiving payments.
If 'account' is specified (DEPRECATED), it is added to the address book
so payments received with the address will be credited to 'account'.

Arguments:
1. "account"        (string, optional) DEPRECATED. The account name for the address to be linked to. If not provided, the default account "" is used. It can also be set to the empty string "" to represent the default account. The account does not need to exist, it will be created if there is no account by the given name.

Result:
"bitcoinaddress"    (string) The new bitcoin address

Examples:
> bitcoin-cli getnewaddress
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getnewaddress", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

Before we use this command, as we are going to be using this new address many times, we want
to wrap it so that we can save it to a shell variable that will work until we exit this
terminal window.

```
$ unset NEW_ADDRESS_1 # clear the variable just in case

$ NEW_ADDRESS_1=$(bt getnewaddress)

$ echo "$NEW_ADDRESS_1"
mjZHLvZfR33EbHh29o9XdSUF2ssgq48vzv

```

Note that this address begins with m — this signifies that this is a testnet address. The
equivalent bitcoin mainnet address would start with a 1.

Once we have the address, it is useful to get it's private key

```
$ dumpprivkey "bitcoinaddress"

Reveals the private key corresponding to 'bitcoinaddress'.
Then the importprivkey can be used with this output

Arguments:
1. "bitcoinaddress"   (string, required) The bitcoin address for the private key

Result:
"key"                (string) The private key

Examples:
> bitcoin-cli dumpprivkey "myaddress"
> bitcoin-cli importprivkey "mykey"
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "dumpprivkey", "params": ["myaddress"] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

Like before, we are going to capture this key into a shell variable

```
$ unset NEW_PRIV_KEY_1

$ NEW_PRIV_KEY_1=$(bt dumpprivkey "$NEW_ADDRESS_1")

$ echo "$NEW_PRIV_KEY_1"
cNyw1qsFdNDzMBMz1JXcR7vU2RzMYkpWMxcR4jtd6KscUtCaVNYh

```

TBD: Let's sign a message with bt signmessage "bitcoinaddress" "message"

Now we need to get some testnet coins. We can get these from a testnet faucet.

Here are a few faucets.

For ease of this tutorial, I suggest you request only one transaction for each new
address. You only need one right now. (Shannon I didn't do this which I regret now, so the
below examples have multiple inputs. I think someone should just do one testnet coin,
then see it not sent, then unconfirmed, then confirmed, all through CLI)

Once you have requested the coins, take a look at the transaction in an explorer.

I like blockcypher because they offer a testnet explorer in addition to mainnet, and
have a useful visual look at the transaction.

```
http://tpfaucet.appspot.com/ 0.96 btc
https://live.blockcypher.com/btc-testnet/tx/a7aeb59b9279df3f4f8214648e6a4f4c952566cec3d129a80c93c39f409c7cc5

http://tpfaucet.appspot.com/ 0.97 btc
https://live.blockcypher.com/btc-testnet/tx/3e59ec01dac60763a92c108db70ca00e6dd75a74e8c6ba715c57f524161e2a69/

https://testnet.manu.backend.hamburg/faucet 2.03125 btc
https://live.blockcypher.com/btc-testnet/tx/e644110281d3c49ee0680a7cead22177f878b372ae1bc6844ae34ee04127de33/

https://testnet.coinfaucet.eu/en/ 1.79676616 btc
https://live.blockcypher.com/btc-testnet/tx/f2f83463cc613020b2bfbd84992afd472ff27b314c12687f771ccfc9f9b6b765/
```


Next lets take a look at the your testnet coin balance

```
$ bt help getbalance
getbalance ( "account" minconf includeWatchonly )

If account is not specified, returns the server's total available balance.
If account is specified (DEPRECATED), returns the balance in the account.
Note that the account "" is not the same as leaving the parameter out.
The server total may be different to the balance in the default "" account.

Arguments:
1. "account"      (string, optional) DEPRECATED. The selected account, or "*" for entire wallet. It may be the default account using "".
2. minconf          (numeric, optional, default=1) Only include transactions confirmed at least this many times.
3. includeWatchonly (bool, optional, default=false) Also include balance in watchonly addresses (see 'importaddress')

Result:
amount              (numeric) The total amount in BTC received for this account.

Examples:

The total amount in the wallet
> bitcoin-cli getbalance

The total amount in the wallet at least 5 blocks confirmed
> bitcoin-cli getbalance "*" 6

As a json rpc call
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getbalance", "params": ["*", 6] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

```
$ bt getbalance
0.00000000
```

The reason why you have no coins is that your coins have not been confirmed yet.

```
$ bt help getunconfirmedbalance
getunconfirmedbalance
Returns the server's total unconfirmed balance
```

```
$ bt getunconfirmedbalance
0.96000000
```

If this is still 0.00000000 then your transaction has not been transmitted yet.
Let's look at the transaction

```
$ bt help getrawtransaction
getrawtransaction "txid" ( verbose )

NOTE: By default this function only works sometimes. This is when the tx is in the mempool
or there is an unspent output in the utxo for this transaction. To make it always work,
you need to maintain a transaction index, using the -txindex command line option.

Return the raw transaction data.

If verbose=0, returns a string that is serialized, hex-encoded data for 'txid'.
If verbose is non-zero, returns an Object with information about 'txid'.

Arguments:
1. "txid"      (string, required) The transaction id
2. verbose       (numeric, optional, default=0) If 0, return a string, other return a json object

Result (if verbose is not set or set to 0):
"data"      (string) The serialized, hex-encoded data for 'txid'

Result (if verbose > 0):
{
  "hex" : "data",       (string) The serialized, hex-encoded data for 'txid'
  "txid" : "id",        (string) The transaction id (same as provided)
  "hash" : "id",        (string) The transaction hash (differs from txid for witness transactions)
  "size" : n,             (numeric) The serialized transaction size
  "vsize" : n,            (numeric) The virtual transaction size (differs from size for witness transactions)
  "version" : n,          (numeric) The version
  "locktime" : ttt,       (numeric) The lock time
  "vin" : [               (array of json objects)
     {
       "txid": "id",    (string) The transaction id
       "vout": n,         (numeric)
       "scriptSig": {     (json object) The script
         "asm": "asm",  (string) asm
         "hex": "hex"   (string) hex
       },
       "sequence": n      (numeric) The script sequence number
       "txinwitness": ["hex", ...] (array of string) hex-encoded witness data (if any)
     }
     ,...
  ],
  "vout" : [              (array of json objects)
     {
       "value" : x.xxx,            (numeric) The value in BTC
       "n" : n,                    (numeric) index
       "scriptPubKey" : {          (json object)
         "asm" : "asm",          (string) the asm
         "hex" : "hex",          (string) the hex
         "reqSigs" : n,            (numeric) The required sigs
         "type" : "pubkeyhash",  (string) The type, eg 'pubkeyhash'
         "addresses" : [           (json array of string)
           "bitcoinaddress"        (string) bitcoin address
           ,...
         ]
       }
     }
     ,...
  ],
  "blockhash" : "hash",   (string) the block hash
  "confirmations" : n,      (numeric) The confirmations
  "time" : ttt,             (numeric) The transaction time in seconds since epoch (Jan 1 1970 GMT)
  "blocktime" : ttt         (numeric) The block time in seconds since epoch (Jan 1 1970 GMT)
}

Examples:
> bitcoin-cli getrawtransaction "mytxid"
> bitcoin-cli getrawtransaction "mytxid" 1
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawtransaction", "params": ["mytxid", 1] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

```
$ bt getrawtransaction "e644110281d3c49ee0680a7cead22177f878b372ae1bc6844ae34ee04127de33"
error code: -5
error message:
No information available about transaction
```

Your trasaction still hasn't been transmitted. Wait a few.

```
$ bt getrawtransaction "e644110281d3c49ee0680a7cead22177f878b372ae1bc6844ae34ee04127de33"
0100000004532fbd765e04a376747d8e4dbb5265f2d6cd0d5dd59987d8239ffe6868ebe8eb010000006b483045022100921753679996c775607a6b820db1d967c750d697af14ea8949df567eb9bc6d49022036ee66656f2f1e695eb816aadbb78947a3b5b75bded51c16e52fd20a34390658012102efddfbdc5926df16a597459f35ca8501f81c33b93a40b9d92519e1067a7e2bacffffffff29fae2bf7c80f4158bbfc974dc7a01f04f88e3a57a1c38cce15877620e06b920010000006a4730440220373aace80645894984e2ed4873ceb5404de1edcabe4a2fd2eba7e5cda58a82a3022045417aa5c21f3afa3543ccd3bd9f5cfc2ab434e12c355480c64633ae30177700012102bb7f1c8be7781c3c9e04c5781f34ef0944fc0cb7392a9b6586da27f7e6606daeffffffff1573b35088b9558a4b505d91ce3e8ed24f2fe6d70e19572e3445321c19894e89010000006a47304402204f031b3d90c6daed058b85c6664adae100052c6440b4bc92b78f23012183521c02202971007e70d8e99b02d4c09d42dce5e5d19d4ae6070e690146726d8313f7fd440121033d4bac7c166af9a0032e417de87a194c16c0eebf44550c1dae045b56f273339affffffffa0a612bd47c2359b0b23016b022848079a02f16bd16ba8ffaf295a59b65db61c010000006a47304402205d2a61f615aa227756ee696bd802ef06419a6549043619418332a5c5878e602102207b4a1ffd8c4839921d592fec4da15443b32cd38f612582903da12427622b047e0121026f8798a09e850608469298785d2e6d620aff52e809dceb1feb7664163e3f05c8ffffffff0208711b0c000000001976a9142c513c9cfbd73565803b1ce314b257ca2d9a6d5888ac169cc710010000001976a914c6a7e76f45dfae348673ac00a20ebeb2351cc91d88ac00000000
```

It has arrived!

We can look at it in more detail:

```
$ bt getrawtransaction "e644110281d3c49ee0680a7cead22177f878b372ae1bc6844ae34ee04127de33" 1
{
  "hex": "0100000004532fbd765e04a376747d8e4dbb5265f2d6cd0d5dd59987d8239ffe6868ebe8eb010000006b483045022100921753679996c775607a6b820db1d967c750d697af14ea8949df567eb9bc6d49022036ee66656f2f1e695eb816aadbb78947a3b5b75bded51c16e52fd20a34390658012102efddfbdc5926df16a597459f35ca8501f81c33b93a40b9d92519e1067a7e2bacffffffff29fae2bf7c80f4158bbfc974dc7a01f04f88e3a57a1c38cce15877620e06b920010000006a4730440220373aace80645894984e2ed4873ceb5404de1edcabe4a2fd2eba7e5cda58a82a3022045417aa5c21f3afa3543ccd3bd9f5cfc2ab434e12c355480c64633ae30177700012102bb7f1c8be7781c3c9e04c5781f34ef0944fc0cb7392a9b6586da27f7e6606daeffffffff1573b35088b9558a4b505d91ce3e8ed24f2fe6d70e19572e3445321c19894e89010000006a47304402204f031b3d90c6daed058b85c6664adae100052c6440b4bc92b78f23012183521c02202971007e70d8e99b02d4c09d42dce5e5d19d4ae6070e690146726d8313f7fd440121033d4bac7c166af9a0032e417de87a194c16c0eebf44550c1dae045b56f273339affffffffa0a612bd47c2359b0b23016b022848079a02f16bd16ba8ffaf295a59b65db61c010000006a47304402205d2a61f615aa227756ee696bd802ef06419a6549043619418332a5c5878e602102207b4a1ffd8c4839921d592fec4da15443b32cd38f612582903da12427622b047e0121026f8798a09e850608469298785d2e6d620aff52e809dceb1feb7664163e3f05c8ffffffff0208711b0c000000001976a9142c513c9cfbd73565803b1ce314b257ca2d9a6d5888ac169cc710010000001976a914c6a7e76f45dfae348673ac00a20ebeb2351cc91d88ac00000000",
  "txid": "e644110281d3c49ee0680a7cead22177f878b372ae1bc6844ae34ee04127de33",
  "hash": "e644110281d3c49ee0680a7cead22177f878b372ae1bc6844ae34ee04127de33",
  "size": 667,
  "vsize": 667,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "ebe8eb6868fe9f23d88799d55d0dcdd6f26552bb4d8e7d7476a3045e76bd2f53",
      "vout": 1,
      "scriptSig": {
        "asm": "3045022100921753679996c775607a6b820db1d967c750d697af14ea8949df567eb9bc6d49022036ee66656f2f1e695eb816aadbb78947a3b5b75bded51c16e52fd20a34390658[ALL] 02efddfbdc5926df16a597459f35ca8501f81c33b93a40b9d92519e1067a7e2bac",
        "hex": "483045022100921753679996c775607a6b820db1d967c750d697af14ea8949df567eb9bc6d49022036ee66656f2f1e695eb816aadbb78947a3b5b75bded51c16e52fd20a34390658012102efddfbdc5926df16a597459f35ca8501f81c33b93a40b9d92519e1067a7e2bac"
      },
      "sequence": 4294967295
    },
    {
      "txid": "20b9060e627758e1cc381c7aa5e3884ff0017adc74c9bf8b15f4807cbfe2fa29",
      "vout": 1,
      "scriptSig": {
        "asm": "30440220373aace80645894984e2ed4873ceb5404de1edcabe4a2fd2eba7e5cda58a82a3022045417aa5c21f3afa3543ccd3bd9f5cfc2ab434e12c355480c64633ae30177700[ALL] 02bb7f1c8be7781c3c9e04c5781f34ef0944fc0cb7392a9b6586da27f7e6606dae",
        "hex": "4730440220373aace80645894984e2ed4873ceb5404de1edcabe4a2fd2eba7e5cda58a82a3022045417aa5c21f3afa3543ccd3bd9f5cfc2ab434e12c355480c64633ae30177700012102bb7f1c8be7781c3c9e04c5781f34ef0944fc0cb7392a9b6586da27f7e6606dae"
      },
      "sequence": 4294967295
    },
    {
      "txid": "894e89191c3245342e57190ed7e62f4fd28e3ece915d504b8a55b98850b37315",
      "vout": 1,
      "scriptSig": {
        "asm": "304402204f031b3d90c6daed058b85c6664adae100052c6440b4bc92b78f23012183521c02202971007e70d8e99b02d4c09d42dce5e5d19d4ae6070e690146726d8313f7fd44[ALL] 033d4bac7c166af9a0032e417de87a194c16c0eebf44550c1dae045b56f273339a",
        "hex": "47304402204f031b3d90c6daed058b85c6664adae100052c6440b4bc92b78f23012183521c02202971007e70d8e99b02d4c09d42dce5e5d19d4ae6070e690146726d8313f7fd440121033d4bac7c166af9a0032e417de87a194c16c0eebf44550c1dae045b56f273339a"
      },
      "sequence": 4294967295
    },
    {
      "txid": "1cb65db6595a29afffa86bd16bf1029a074828026b01230b9b35c247bd12a6a0",
      "vout": 1,
      "scriptSig": {
        "asm": "304402205d2a61f615aa227756ee696bd802ef06419a6549043619418332a5c5878e602102207b4a1ffd8c4839921d592fec4da15443b32cd38f612582903da12427622b047e[ALL] 026f8798a09e850608469298785d2e6d620aff52e809dceb1feb7664163e3f05c8",
        "hex": "47304402205d2a61f615aa227756ee696bd802ef06419a6549043619418332a5c5878e602102207b4a1ffd8c4839921d592fec4da15443b32cd38f612582903da12427622b047e0121026f8798a09e850608469298785d2e6d620aff52e809dceb1feb7664163e3f05c8"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 2.03125000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 2c513c9cfbd73565803b1ce314b257ca2d9a6d58 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a9142c513c9cfbd73565803b1ce314b257ca2d9a6d5888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mjZHLvZfR33EbHh29o9XdSUF2ssgq48vzv"
        ]
      }
    },
    {
      "value": 45.76484374,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 c6a7e76f45dfae348673ac00a20ebeb2351cc91d OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914c6a7e76f45dfae348673ac00a20ebeb2351cc91d88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mydMEDMQXoTajULoM958PdBBt925d72Gb1"
        ]
      }
    }
  ],
  "blockhash": "0000000000008b91c284d4d185e8f87a4d9888d7e1df1db7d8697fbd765e4391",
  "confirmations": 374,
  "time": 1487143122,
  "blocktime": 1487143122
}
```

TBD - take a look at how it looks in blockcyper explorer vs looking at the raw transaction.

TBD — show how balance, unconfirmed balance change over time, show getwalletinfo changes.

```
$ bt getwalletinfo
{
  "walletversion": 130000,
  "balance": 3.96125000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 3,
  "keypoololdest": 1487117744,
  "keypoolsize": 100,
  "paytxfee": 0.00000000,
  "hdmasterkeyid": "1ca5426f74d261e90b196ca12ed358fff10ab38e"
}

$ bt getwalletinfo
{
  "walletversion": 130000,
  "balance": 5.75801616,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 4,
  "keypoololdest": 1487117744,
  "keypoolsize": 100,
  "paytxfee": 0.00000000,
  "hdmasterkeyid": "1ca5426f74d261e90b196ca12ed358fff10ab38e"
}

```

TBD - demonstrate listunspent $NEW_ADDRESS_1 — this one is a bad early example as it has
too many inputs for ease of use, as I got different test coins from different services.
Choose one that is the most reliable and fast to use for tutorial, footnote the others
in case it is down.

```
$ bt listunspent
[
  {
    "txid": "e644110281d3c49ee0680a7cead22177f878b372ae1bc6844ae34ee04127de33",
    "vout": 0,
    "address": "mjZHLvZfR33EbHh29o9XdSUF2ssgq48vzv",
    "account": "",
    "scriptPubKey": "76a9142c513c9cfbd73565803b1ce314b257ca2d9a6d5888ac",
    "amount": 2.03125000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "f2f83463cc613020b2bfbd84992afd472ff27b314c12687f771ccfc9f9b6b765",
    "vout": 0,
    "address": "mjZHLvZfR33EbHh29o9XdSUF2ssgq48vzv",
    "account": "",
    "scriptPubKey": "76a9142c513c9cfbd73565803b1ce314b257ca2d9a6d5888ac",
    "amount": 1.79676616,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "3e59ec01dac60763a92c108db70ca00e6dd75a74e8c6ba715c57f524161e2a69",
    "vout": 0,
    "address": "mjZHLvZfR33EbHh29o9XdSUF2ssgq48vzv",
    "account": "",
    "scriptPubKey": "76a9142c513c9cfbd73565803b1ce314b257ca2d9a6d5888ac",
    "amount": 0.97000000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "a7aeb59b9279df3f4f8214648e6a4f4c952566cec3d129a80c93c39f409c7cc5",
    "vout": 1,
    "address": "mjZHLvZfR33EbHh29o9XdSUF2ssgq48vzv",
    "account": "",
    "scriptPubKey": "76a9142c513c9cfbd73565803b1ce314b257ca2d9a6d5888ac",
    "amount": 0.96000000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true
  }
]
```

TBD: maybe a few more wallet commands - see bt help

TBD: This is the end of Part One, so we should repeat what we learned. (and maybe say
what we are going to learn in this section at the top.) In Part Two we will explain how
to to a simple raw transaction and spend it. We want to follow
https://bitcoin.org/en/developer-examples#transaction-tutorial
but make it easier (the sample JSONS are rather overly abbreviated for new users)
except that in the middle we want to demonstrate an OP_RETURN version.

=======================

jq is a useful package — we may want to add it. Basically a command line grepper for json.

```
curl -s http://api.coindesk.com/v1/bpi/currentprice.json | jq .bpi.USD.rate

bt getrawtransaction "f2f83463cc613020b2bfbd84992afd472ff27b314c12687f771ccfc9f9b6b765" 1 | jq '{txid: .txid, vout: .vin.vout}'
```
