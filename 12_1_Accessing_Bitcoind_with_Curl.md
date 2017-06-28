# 12.1: Accessing Bitcoind with Curl

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

`bitcoin-cli`, the heart of chapters 3-6, is ultimately just a wrapper. It's a way to interface with `bitcoind` from the command line, providing simplified access to its many RPC commands. But RPC can, of course, be accessed directly. That's what this chapter is about: directly connecting to RPC with the `curl` command.

## Know Your Curl

`curl`, short for "see URL", is a command-line tool that allows you to directly access URLs in a programmatic way. It's an easy way to interact with servers like `bitcoind` that listen to ports on the internet and that speak a variety of protocols. Curl is also available as a library for many programming languages, such as C, Java, PHP, and Python. So, once you know how to work with Curl, you'll have a strong foundation for using a lot of different APIs — as we'll touch on in the next few chapters.

In order to use `curl` with `bitcoind`, you must know three things: the standard format, the user name and password, and the correct port.

### Know Your Format

The `bitcoin-cli` commands are all linked to RPC commands in `bitcoind`. That makes the transition from using `bitcoin-cli` to using `curl` very simple. In fact, if you look at any of the help pages for `bitcoin-cli`, you'll see that they list not only the `bitcoin-cli` commands, but also parallel `curl` commands. For example, here is `bitcoin-cli getinfo help`:
```
$ bitcoin-cli help getmininginfo
getmininginfo

Returns a json object containing mining-related information.
Result:
{
  "blocks": nnn,             (numeric) The current block
  "currentblocksize": nnn,   (numeric) The last block size
  "currentblockweight": nnn, (numeric) The last block weight
  "currentblocktx": nnn,     (numeric) The last block transaction
  "difficulty": xxx.xxxxx    (numeric) The current difficulty
  "errors": "..."            (string) Current errors
  "networkhashps": nnn,      (numeric) The network hashes per second
  "pooledtx": n              (numeric) The size of the mempool
  "chain": "xxxx",           (string) current network name as defined in BIP70 (main, test, regtest)
}

Examples:
> bitcoin-cli getmininginfo 
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
And there's the `curl` command, at the end of the help screen! This somewhat lengthy command has four major parts: (1) a listing of your user name; (2) a `--data-binary` flag; (3) a JSON object that tells `bitcoind` what to do, including a JSON array of parameters; and (4) an HTTP header that includes the `bitcoind` URL. 

When you are working with `curl`, most of these arguments to `curl` will stay the same from command to command; only the `method` and `params` entries in the JSON array will typically change. However, you need to know how to fill in your username and your URL address in order to make it work in the first place!

_Whenever you're unusure about how to curl an RPC command, just look at the bitcoin-cli help and go from there._

### Know Your User Name

In order to speak with the `bitcoind` port, you need a user name and password. These were created as part of your initial Bitcoin setup, and can be found in `~/.bitcoin/bitcoin.conf`. 

For example, here's our current setup:
```
$ more ~/.bitcoin/bitcoin.conf
server=1
dbcache=1536
par=1
blocksonly=1
maxuploadtarget=137
maxconnections=16
rpcuser=bitcoinrpc
rpcpassword=73bd45ba60ab8f9ff9846b6404769487
rpcallowip=127.0.0.1
txindex=1
testnet=1
```
Our user name is `bitcoinrpc` and our password is `73bd45ba60ab8f9ff9846b6404769487`.

> **WARNING:** Clearly, it's not very secure to have this information in a plain text file. As of Bitcoin Core 0.12, you can instead omit the `rpcpassword` from your `bitcoin.conf` file, and have `bitcoind` generate a new cookie whenever it starts up. The downside of this is that it makes use of RPC commands by other applications, such as the ones detailed in this chapter, more difficult. So, we're going to stick with the plain `rpcuser` and `rpcpassword` information for now, but for production software, consider moving to cookies.

The secure way to RPC with `bitcoind` is as follows:
```
$ curl --user bitcoinrpc --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
Enter host password for user 'bitcoinrpc':
```
As noted, you will be prompted for your password.

The insecure way to do so is as follows:
```
$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
> **WARNING:** Entering your password on the command line may put your password into the process table and/or save it into a history. This is even less recommended than putting it in a file, except for testing on testnet. If you want to do it anywhere else, make sure you know what you're doing!

### Know Your Port

The port should be easy, it's the `http://127.0.0.1:8332/` shown in the tutorials, right? Not exactly. It depends on whether you're using testnet or mainnet.

> **MAINNET VS. TESTNET.** To access RPC commands on the mainnet, use port 8332, but to do so on testnet, use port 18332.

### Know Your Command & Parameters

With all of that in hand, you're ready to send off standard RPC commands with `curl` ... but you still need to know how to incorporate the two elements that tend to change in the `curl` command.

The first is `method`, which is the RPC method being used. This should generally match the command names you've been feeding into `bitcoin-cli` for ages. 

The second is `params`, which is a JSON array of parameters. These are the same as the arguments (or named arguments) that you've been using. They're also the most confusing part of `curl`, in large part because they're a structured array rather than a simple list.

Here's what some parameter arrays will look like:

  * `[]` — An empty array
  * `["000b4430a7a2ba60891b01b718747eaf9665cb93fbc0c619c99419b5b5cf3ad2"]` — An array with data
  * `["'$signedhex'"]` — An array with a variable
  * `[6, 9999999]` — An array with two parameters
  * `[''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.298, "'$changeaddress'": 1.0}'']` — An array with an array containing an object and a bare object

## Get Information

You can now send your first `curl` command by accessing the `getmininginfo` RPC:
```
$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
{"result":{"blocks":1128599,"currentblocksize":0,"currentblockweight":0,"currentblocktx":0,"difficulty":1,"errors":"Warning: unknown new rules activated (versionbit 28)","networkhashps":8658807967387.751,"pooledtx":0,"chain":"test"},"error":null,"id":"curltest"}
```
Note that we provided the method, `getmininginfo`, and the parameter, `[]`, but that everything else was the standard `curl` command line.

> **WARNING:** If you get a result like "Failed to connect to 127.0.0.1 port 8332: Connection refused", be sure that a line like `rpcallowip=127.0.0.1` is in your ~/.bitcoin/bitcoin.conf. If things still don't work, be sure that you're allowing access to port 18332 (or 8332) from localhost. Our standard setup from [Chapter Two: Creating a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) should do all of this.

The result is another JSON array, which is unfortunately ugly to read if you're using `curl` by hand. Fortunately, you can clean it up  simply by piping it through `jq`:
```
$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'

% Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   354  100   277  100    77  65546  18220 --:--:-- --:--:-- --:--:-- 92333

{
  "result": {
    "blocks": 1128609,
    "currentblocksize": 0,
    "currentblockweight": 0,
    "currentblocktx": 0,
    "difficulty": 2777757.750498331,
    "errors": "Warning: unknown new rules activated (versionbit 28)",
    "networkhashps": 8795436830406.6,
    "pooledtx": 0,
    "chain": "test"
  },
  "error": null,
  "id": "curltest"
}
```
You'll see a bit of connectivity reporting as the data is downloaded, then when that data hits `jq`, everything will be output in a correctly indented form. (We'll be omiting the download information in future examples.)

## Manipulate Your Wallet

Though you're accessing `bitcoind` directly, you'll still get access to wallet functionality, because that's largely stored in `bitcoind` itself.

### Look Up Addresses

Use the `getaddressesbyaccount` RPC to list all of your current addresses:
```
$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getaddressesbyaccount", "params": [""] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
 
 {
  "result": [
    "mg7YqyvK8HUFvpgZ5iYTfZ5vjfaJWnNTd9",
    "mkMkhbUzcSPdEHUoRQkBKHe8otP1SzWWeb",
    "moXjRArk4rWUYZvAiDHnMXQHmwyf3TtAb8",
    "mp3qqSTEYqhm3iuo9nM5GnzCGuoEvb9KrK",
    "msW5tzCMJRECSATENN11ATmxx3PLwjGEaW",
    "msZ5bMbZC4HQPRuJCoivDWYExAB7ssE5o8",
    "mx3wUEVp526gkqzMteK6NwsPffgtFB3CGX",
    "mxwLPpo2X1VXXAiLHNVb55H4JASeLMDm7A",
    "n2AfUVXq9MzsJohCjCKNXKj2SYHfdbrcqy",
    "n2fGxDXKozkZjpi9DqW93NZ65TnQDqd49V",
    "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
    "2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8",
    "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz",
    "2NDx9HTeLagiEqWrYvBuPkG3gwdhQ32Jz4B"
  ],
  "error": null,
  "id": "curltest"
}
```
This is our first example of a real parameter, `""`. This is the required `account` parameter for `getaddressbyaccount`, but all of our addresses are in the default account, so nothing special was required here.

The result is a list of all the addresses that have been used by this wallet ... some of which presumably contain funds.

### Look Up Funds

Use the `listunspent` RPC to list the funds that you have available:
```
$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'

{
  "result": [
    {
      "txid": "6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e",
      "vout": 0,
      "address": "mnsdQUxpGa6UsroUJhJGupPrPcuvQbPzmV",
      "scriptPubKey": "76a91450b1d90a130c4f3f1e5fbfa7320fd36b7265db0488ac",
      "amount": 1.2985,
      "confirmations": 1260,
      "spendable": true,
      "solvable": true
    },
    {
      "txid": "966099e0fd330007eac0394d97a36985980ff971d64b865c5d474f1931eeec3a",
      "vout": 0,
      "address": "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz",
      "account": "",
      "scriptPubKey": "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287",
      "amount": 1.2985,
      "confirmations": 3681,
      "spendable": false,
      "solvable": false
    },
    {
      "txid": "e31f747cb97a364a1a00b56a3d9d43a97c6e02938a47e599ad54d7cf710f056f",
      "vout": 0,
      "address": "2NDx9HTeLagiEqWrYvBuPkG3gwdhQ32Jz4B",
      "account": "",
      "redeemScript": "52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd81921029bf628adf0698a089137294975c8589cf0cc6be050f92d944faaa8c8a623430352ae",
      "scriptPubKey": "a914e31fa4be1f718e068ba5cbb2f12a59bba66a2da687",
      "amount": 1.3,
      "confirmations": 6515,
      "spendable": false,
      "solvable": true
    },
    {
      "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
      "vout": 0,
      "address": "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
      "account": "",
      "scriptPubKey": "76a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac",
      "amount": 0.47,
      "confirmations": 38043,
      "spendable": true,
      "solvable": true
    },
    {
      "txid": "7e8abac5040a14f74fc8c44300c89cf93ce7f59f1e1c1d1711fdddd7012941c1",
      "vout": 0,
      "address": "moXjRArk4rWUYZvAiDHnMXQHmwyf3TtAb8",
      "account": "",
      "scriptPubKey": "76a91457e6ac3cfbc9c34c292ca59f105a2d7a819db7a488ac",
      "amount": 0.65,
      "confirmations": 6586,
      "spendable": true,
      "solvable": true
    }
  ],
  "error": null,
  "id": "curltest"
}
```
This is almost exactly the same output that you receive when you type `bitcoin-cli listunspent`, showing how closely tied the two interfaces are. If no cleanup or extra help is needed, then `bitcoin-cli` just outputs the RPC. Easy!

### Create an Address

After you know where your funds are, the next step in crafting a transaction is to get a change address. By now you've probably got the hang of this, and you know that for simple RPC commands, all you need to do is adjust the `method` is the `curl` command:
```
$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
 
 {
  "result": "mznccEt2ozGFN6oVaYU5BGgTzcdH8Zj8wf",
  "error": null,
  "id": "curltest"
}
```
At this point, we can even revert to our standard practice of saving results to variables with additional help from `jq`:
```
$ changeaddress=$(curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')

$ echo $changeaddress
mmDWRH3CbeXwCqBwdHCj7E9d3oWTuuizxc
```
No need to worry about the downloading info. It'll go to `STDERR` and be displayed on your screen, while the results go to `STDOUT` and are saved in your variable.

## Create a Transaction

You're now ready to create a transaction with `curl`.

### Ready Your Variables

Just as with `bitcoin-cli`, in order to create a transaction by curling RPC commands, you should first save your variables. The only change here is that `curl` creates a JSON object that includes a `result` key-value, so you always need to pipe through the `.result` tag before you do anything else. 

This example sets up our variables for using the 1.2985 BTC in funds listed in the first unspent transaction above:
```
$ utxo_txid=$(curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .[0] | .txid')
$ utxo_vout=$(curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .[0] | .vout')
$ recipient=mwCwTceJvYV27KXBc3NJZys6CjsgsoeHmf
$ changeaddress=$(curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')

$ echo $utxo_txid
6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e
$ echo $utxo_vout
0
$ echo $recipient
mwCwTceJvYV27KXBc3NJZys6CjsgsoeHmf
$ echo $changeaddress
mmDWRH3CbeXwCqBwdHCj7E9d3oWTuuizxc
```

### Create the Transaction

The transaction created with `curl` is very similar to the transaction created with `bitcoin-cli`, but with a few subtle differences:
```
$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.298, "'$changeaddress'": 1.0}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
 
 {
  "result": "02000000010e7ffd317318792fec2b3d88b303a15326d541f0d631489f1830fa072f4a186a0000000000ffffffff0240b6c601000000001976a914ac19d3fd17710e6b9a331022fe92c693fdf6659588ac00e1f505000000001976a9143e84156731d67c80c3dff6c1cc3b4f58460e642388ac00000000",
  "error": null,
  "id": "curltest"
}
```
The heart of the transaction is, of course, the `params` JSON array, which we're putting to full use for the first time.

Note that the entire `params` is lodged in `[]`s to mark the parameters array.

We've also varied up the quoting from how things worked in `bitcoin-cli`, to start and end each array and object within the `params` array with `''` instead of our traditional `'''`. That's because the entire set of JSON arguments already has a `'` around it. As usual, just take a look at the bizarre shell quoting and get used to it.

However, there's one last thing of note in this example, and it can be _maddening_ if you miss it. When you executed a `createrawtransaction` command with `bitcoin-cli` the JSON array of inputs and the JSON object of outputs were each distinct parameters, so they were separated by a space. Now, because they're part of that `params` JSON array, they're separated by a comma (`,`). Miss that and you'll get a `parse error` without much additional information.

> **WARNING:** Ever having troubles debugging your `curl`? Add the argument `--trace-ascii /tmp/foo`. Full information on what's being sent to the server will be saved in `/tmp/foo` (or whatever file name you provide).

Having verified that things work, you probably want to save the hex code into a variable:
```
$ hexcode=$(curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.298, "'$changeaddress'": 1.0}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')
```

### Sign and Send

Signing and sending your transaction using `curl` is an easy use of the `signrawtransaction` and `sendrawtransaction` RPC:

```
$ signedhex=$(curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "signrawtransaction", "params": ["'$hexcode'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .hex')

$ curl --user bitcoinrpc:73bd45ba60ab8f9ff9846b6404769487 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "sendrawtransaction", "params": ["'$signedhex'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "000b4430a7a2ba60891b01b718747eaf9665cb93fbc0c619c99419b5b5cf3ad2",
  "error": null,
  "id": "curltest"
}
```
## Summary: Accessing Bitcoind with Curl

Having finished this section, you may feel that accessing `bitcoind` via `curl` is very much like accessing it through `bitcoin-cli` ... but more cumbersome. And, you'd be right. `bitcoin-cli` has pretty complete RPC functionality, so anything that you do through `curl` you can probably do through `bitcoin-cli`.

_What is the power of curl?_ Most obviously, `curl` takes out one level of indirection. Instead of working with `bitcoin-cli` which sends RPC commands to `bitcoind`, you're sending those RPC commands directly. This allows for more robust programming, because you don't have to worry about what unexpected things that `bitcoin-cli` might do or how it might change over time. However, you're also taking your first steps toward using a more comprehensive programming language than the poor options offered by a shell script. But for that, you'll need to use a `curl` library within a more familiar language like C. In other words, command-line `curl` was just the first step, the basis to better explain what you're doing as you move forward.

