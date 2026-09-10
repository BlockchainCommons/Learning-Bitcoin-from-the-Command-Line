# Interlude: Using Curl

`bitcoin-cli` is ultimately just a wrapper. It's a way to interface with `bitcoind` from the command line, providing simplified access to its many RPC commands. But RPC can, of course, be accessed directly. That's what this interlude is about: directly connecting to RPC with the `curl` command.

It won't be used much in the future chapters, but it's an important
building block that you can see as an alternative way to access to
`bitcoind` is you so prefer.

## Know Your Curl

`curl`, short for "see URL", is a command-line tool that allows you to
directly access URLs in a programmatic way. It's an easy way to
interact with servers like `bitcoind` that listen to ports on the
internet and that speak a variety of protocols. Curl is also available
as a library for many programming languages, such as C, Java, PHP, and
Python. So, once you know how to work with Curl, you'll have a strong
foundation for using a lot of different API.

In order to use `curl` with `bitcoind`, you must know three things: the standard format, the user name and password, and the correct port.

### Know Your Format

The `bitcoin-cli` commands are all linked to RPC commands in
`bitcoind`. That makes the transition from using `bitcoin-cli` to
using `curl` very simple. In fact, if you look at any of the help
pages for `bitcoin-cli`, you'll see that they list not only the
`bitcoin-cli` commands, but also parallel `curl` commands. For
example, here is `bitcoin-cli help getmininginfo`:

```sh
bitcoin-cli help getmininginfo

| getmininginfo
| 
| Returns a json object containing mining-related information.
| 
| Result:
| {                                (json object)
|   "blocks" : n,                  (numeric) The current block
|   "currentblockweight" : n,      (numeric, optional) The block weight (including reserved weight for block header, txs count and coinbase tx) of the last assembled block (only present if a block was ever assembled)
|   "currentblocktx" : n,          (numeric, optional) The number of block transactions (excluding coinbase) of the last assembled block (only present if a block was ever assembled)
|   "bits" : "hex",                (string) The current nBits, compact representation of the block difficulty target
|   "difficulty" : n,              (numeric) The current difficulty
|   "target" : "hex",              (string) The current target
|   "networkhashps" : n,           (numeric) The network hashes per second
|   "pooledtx" : n,                (numeric) The size of the mempool
|   "blockmintxfee" : n,           (numeric) Minimum feerate of packages selected for block inclusion in BTC/kvB
|   "chain" : "str",               (string) current network name (main, test, testnet4, signet, regtest)
|   "signet_challenge" : "hex",    (string, optional) The block challenge (aka. block script), in hexadecimal (only present if the current network is a signet)
|   "next" : {                     (json object) The next block
|     "height" : n,                (numeric) The next height
|     "bits" : "hex",              (string) The next target nBits
|     "difficulty" : n,            (numeric) The next difficulty
|     "target" : "hex"             (string) The next target
|   },
|   "warnings" : [                 (json array) any network and blockchain warnings (run with `-deprecatedrpc=warnings` to return the latest warning as a single string)
|     "str",                       (string) warning
|     ...
|   ]
| }
| 
| Examples:
| > bitcoin-cli getmininginfo 
| > curl --user myusername --data-binary '{"jsonrpc": "2.0", "id": "curltest", "method": "getmininginfo", "params": []}' -H 'content-type: application/json' http://127.0.0.1:8332/
```

There's the `curl` command, at the end of the help screen! This somewhat lengthy command has four major parts: (1) a listing of your user name; (2) a `--data-binary` flag; (3) a JSON object that tells `bitcoind` what to do, including a JSON array of parameters; and (4) an HTTP header that includes the `bitcoind` URL.

When you are working with `curl`, most of these arguments to `curl` will stay the same from command to command; only the `method` and `params` entries in the JSON array will typically change. However, you need to know how to fill in your username and your URL address in order to make it work in the first place!

_Whenever you're unsure about how to curl an RPC command, just look at the bitcoin-cli help and go from there._

### Know Your User Name

In order to speak with the `bitcoind` port, you need an RPC user name
and password. These were created as part of your initial Bitcoin
setup, and can be found in `~/.bitcoin/bitcoin.conf`.

For example, here's our current setup:

```sh
cat ~/.bitcoin/bitcoin.conf

| server=1
| dbcache=1536
| par=1
| maxuploadtarget=137
| maxconnections=16
| rpcuser=StandUp
| rpcpassword=4b42b0447b08784cb41adedac54d7f26
| rpcallowip=127.0.0.1
| debug=tor
| prune=550
| signet=1
| fallbackfee=0.0001
| mintxfee=0.0001
| txconfirmtarget=1
| [test]
| rpcbind=127.0.0.1
| rpcport=18332
| [main]
| rpcbind=127.0.0.1
| rpcport=8332
| [regtest]
| rpcbind=127.0.0.1
| rpcport=18443
```

Our user name is `StandUp` and our password is `4b42b0447b08784cb41adedac54d7f26`.

> ⚠️ **SECURITY WARNING:** Clearly, it's not very secure to have this
information in a plain text file. You can instead omit the
`rpcpassword` from your `bitcoin.conf` file, and have `bitcoind`
generate a new cookie whenever it starts up. The downside of this is
that it makes use of RPC commands by other applications, such as the
ones detailed in this chapter, more difficult. So, we're going to
stick with the plain `rpcuser` and `rpcpassword` information for now,
but for production software, consider moving to cookies.

The secure way to RPC with `bitcoind` is as follows:

```sh
curl --user StandUp --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/

| Enter host password for user 'bitcoinrpc':
```
As noted, you will be prompted for your password.

> 🔗 **SIGNET vs MAINNET:** Signet uses a URL with port 38332 and mainnet uses a URL with port 8332. Take a look in your `bitcoin.conf`, it's mostly laid out there.

The insecure way to do so is as follows:

```sh
curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/
```

> ⚠️ **WARNING: Password Not Secure!** Entering your password on the
command line may put your password into the process table and/or save
it into a history. This is even less recommended than putting it in a
file, except for testing on signet (or testnet). If you want to do it
anywhere else, make sure you know what you're doing!

### Know Your Command & Parameters

With all of that in hand, you're ready to send off standard RPC commands with `curl` ... but you still need to know how to incorporate the two elements that tend to change in the `curl` command.

The first is `method`, which is the RPC method being used. This should
generally match the command names you've previously fed into
`bitcoin-cli`.

The second is `params`, which is a JSON array of parameters. These are the same as the arguments (or named arguments) that you've been using. They're also the most confusing part of `curl`, in large part because they're a structured array rather than a simple list.

Here's what some parameter arrays will look like:

  * `[]` — An empty array
  * `["000b4430a7a2ba60891b01b718747eaf9665cb93fbc0c619c99419b5b5cf3ad2"]` — An array with data
  * `["'$signedhex'"]` — An array with a variable
  * `[6, 9999999]` — An array with two parameters
  * `{}` - An empty object
  * `[''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.298, "'$changeaddress'": 1.0}'']` — An array with an array containing an object and a bare object

## Get Information

You can now send your first `curl` command by accessing the `getmininginfo` RPC:

```sh
curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/

| {"result":{"blocks":293320,"bits":"1d150eea","difficulty":0.0474865840527392,"target":"000000150eea0000000000000000000000000000000000000000000000000000","networkhashps":301893.0621307249,"pooledtx":80,"blockmintxfee":0.00000001,"chain":"signet","next":{"height":293321,"bits":"1d150eea","difficulty":0.0474865840527392,"target":"000000150eea0000000000000000000000000000000000000000000000000000"},"signet_challenge":"512103ad5e0edad18cb1f0fc0d28a3d4f1f3e445640337489abb10404f2d1e086be430210359ef5021964fe22d6f8e05b2463c9540ce96883fe3b278760f048f5189f2e6c452ae","warnings":[]},"error":null,"id":"curltest"}
```

Note that we provided the method, `getmininginfo`, and the parameter, `[]`, but that everything else was the standard `curl` command line.

> ⚠️ **WARNING: RPC May Not Allow.** If you get a result like "Failed
to connect to 127.0.0.1 port 8332: Connection refused", be sure that a
line like `rpcallowip=127.0.0.1` is in your
~/.bitcoin/bitcoin.conf. If things still don't work, be sure that
you're allowing access to port 38332 (or 8332) from localhost. Our
standard setup from [Chapter Two: Creating a Bitcoin-Core
VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) should do all of this.

The result is another JSON array, which is unfortunately ugly to read
if you're using `curl` by hand. Fortunately, you can clean it up
simply by piping it through `jq`:

```sh
curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.'

| {
|   "result": {
|     "blocks": 293320,
|     "bits": "1d150eea",
|     "difficulty": 0.0474865840527392,
|     "target": "000000150eea0000000000000000000000000000000000000000000000000000",
|     "networkhashps": 301893.0621307249,
|     "pooledtx": 88,
|     "blockmintxfee": 1E-8,
|     "chain": "signet",
|     "next": {
|       "height": 293321,
|       "bits": "1d150eea",
|       "difficulty": 0.0474865840527392,
|       "target": "000000150eea0000000000000000000000000000000000000000000000000000"
|     },
|     "signet_challenge": "512103ad5e0edad18cb1f0fc0d28a3d4f1f3e445640337489abb10404f2d1e086be430210359ef5021964fe22d6f8e05b2463c9540ce96883fe3b278760f048f5189f2e6c452ae",
|     "warnings": []
|   },
|   "error": null,
|   "id": "curltest"
| }
```

The '2>' throws out the connectivity reporting that `curl` sends back,
and then we send the rest to `jq`, which ensures that everything will
be output in a correctly indented form.

## Manipulate Your Wallet

Though you're accessing `bitcoind` directly, you'll still get access to wallet functionality, because that's largely stored in `bitcoind` itself.

### Look Up Addresses

Use the `getaddressesbylabel` RPC to list all of your current addresses:

```sh
curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getaddressesbylabel", "params": [""] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.'

| {
|   "result": {
|     "tb1qxe0nn84xxw76jfc0s93g8kuefu2llglcvscy28": {
|       "purpose": "receive"
|     },
|     "tb1qxmx50fc06jt67nw6eqdcsm3rexgn6qvcet3rcm": {
|       "purpose": "receive"
|     },
|     "tb1q8lpqzydarurhsqttvw2qkf2qjxrwdv0kkej2ux": {
|       "purpose": "receive"
|     },
|     "tb1qdu69s5f9nw8x88lvsc8nt5d7gy9rgtnef0upfu": {
|       "purpose": "receive"
|     },
|     "tb1qsnm8mfafey9mlc3wj0qqspfmzdvhy0pnlmw9xm": {
|       "purpose": "receive"
|     },
|     "tb1qnu24edevunfh8lk2a4x74ctn9jhslr83e4nsxx": {
|       "purpose": "receive"
|     },
|     "tb1qkvcgm83wv9np7re8w4w5ayg9tm2kxxpxk99h9p": {
|       "purpose": "receive"
|     },
|     "tb1qmwzvuavdvc6x0kmlrqncau4txhm5qrs7exjnv4": {
|       "purpose": "receive"
|     },
|     "tb1qukwva953qreduzm3mgnjwh5ymxupa4jrrkj8ym": {
|       "purpose": "receive"
|     },
|     "tb1qactkrm2yj2g73s966ptw7m56634zdv4acnhjhx": {
|       "purpose": "receive"
|     },
|     "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl": {
|       "purpose": "receive"
|     },
|     "tb1qlw280hk7cf7r97sm896s83wysat4vrqzpalm7e": {
|       "purpose": "receive"
|     }
|   },
|   "error": null,
|   "id": "curltest"
| }
```

This is our first example of a real parameter, `""`. This is the required `label` parameter for `getaddressesbylabel`, but all of our addresses are under the default label, so nothing special was required here.

The result is a list of all the addresses that have been used by this wallet ... some of which presumably contain funds.

### Look Up Funds

Use the `listunspent` RPC to list the funds that you have available:

```sh
curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.'

| {
|   "result": [
|     {
|       "txid": "80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465",
|       "vout": 1,
|       "address": "tb1qjrs9jr572nfyg5un8f2rzjdngml0mvskxlyd09",
|       "scriptPubKey": "001490e0590e9e54d24453933a543149b346fefdb216",
|       "amount": 0.00194000,
|       "confirmations": 1,
|       "spendable": true,
|       "solvable": true,
|       "desc": "wpkh([b8309bae/84h/1h/0h/1/3]0347d61d5a0d09bf411b059a8ee97225e3430171b303a71ec40c5f414029cd84db)#gl740gxy",
|       "parent_descs": [
|         "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
|       ],
|       "safe": true
|     },
|     {
|       "txid": "80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465",
|       "vout": 0,
|       "address": "tb1qdu69s5f9nw8x88lvsc8nt5d7gy9rgtnef0upfu",
|       "label": "",
|       "scriptPubKey": "00146f345851259b8e639fec860f35d1be410a342e79",
|       "amount": 0.02000000,
|       "confirmations": 1,
|       "spendable": true,
|       "solvable": true,
|       "desc": "wpkh([b8309bae/84h/1h/0h/0/11]03d9cbd48a1238cc2c7371fd0c7169995106db6dec9fb287e4490c08cbd2b35cf1)#n3vc09ew",
|       "parent_descs": [
|         "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|       ],
|       "safe": true
|     },
|     {
|       "txid": "8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2",
|       "vout": 0,
|       "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
|       "label": "",
|       "scriptPubKey": "0014f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
|       "amount": 0.00099000,
|       "confirmations": 9,
|       "spendable": true,
|       "solvable": true,
|       "desc": "wpkh([b8309bae/84h/1h/0h/0/5]027650c44c6f71d50a173f3f18858490cf26d2661a1d9929deba9aca57c76d1e5b)#7ss7jfnn",
|       "parent_descs": [
|         "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|       ],
|       "safe": true
|     }
|   ],
|   "error": null,
|   "id": "curltest"
| }
```

This is almost exactly the same output that you receive when you type `bitcoin-cli listunspent`, showing how closely tied the two interfaces are. If no cleanup or extra help is needed, then `bitcoin-cli` just outputs the RPC. Easy!

### Create an Address

After you know where your funds are, the next step in crafting a transaction is to get a change address. By now you've probably got the hang of this, and you know that for simple RPC commands, all you need to do is adjust the `method` is the `curl` command. This example demonstrates the creation of an older "legacy" address, mainly so that we can demonstrate how an actual parameter works in the RPC interface.

```sh
curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.'

| {
|   "result": "n2cv88ogXyYSiGsVvRuAswetLnf7D7GRM5",
|   "error": null,
|   "id": "curltest"
| }
```

> ⚠️ **WARNING: Parameter Order Matters!** The parameters order is
important when you are sending RPC commands using curl. There's only
one argument for `getrawchangeaddress`, but consider its close cousin
`getnewaddress`. That takes two arguments: first label, then type. If
we sent that same `"params": ["legacy"]` instead of `"params": ["",
"legacy"]`, we would get a `bech32` address with a label of `"legacy"`
instead of a `legacy` address, so pay attention to the order!

At this point, we can even revert to our standard practice of saving results to variables with additional help from `jq`:

```sh
newaddress=$(curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.result')
echo $newaddress

| tb1qmp3jd7s9s0vgnyxtjdf50qua90s825alqrpktv
```

## Create a Transaction

You're now ready to create a transaction with `curl`.

### Ready Your Variables

Just as with `bitcoin-cli`, in order to create a transaction by curling RPC commands, you should first save your variables. The only change here is that `curl` creates a JSON object that includes a `result` key-value, so you always need to pipe through the `.result` tag before you do anything else.

This example sets up our variables for using the 0.00194000 BTC in funds listed in the first unspent transaction above:

```sh
utxo_txid=$(curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.result | .[0] | .txid')
utxo_vout=$(curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.result | .[0] | .vout')
recipient=tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn
changeaddress=$(curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.result')

echo $utxo_txid

| 80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465

echo $utxo_vout

| 1

echo $recipient

| tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn

echo $changeaddress

| tb1q79st28w04n57z6ht8zasfrdy9804qe9u5l4ahf
```

### Create the Transaction

The transaction created with `curl` is very similar to the transaction created with `bitcoin-cli`, but with a few subtle differences:
```
curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.001, "'$changeaddress'": 0.0009}'']}' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.'

| {
|   "result": "02000000016524475f2cb9b7af59f86d25183d6dbe79b41b248a5a40c5e608ce79ca9f61800100000000fdffffff02a086010000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac9905f010000000000160014f160b51dcface9e16aeb38bb048da429df5064bc00000000",
|   "error": null,
|   "id": "curltest"
| }
```

The heart of the transaction is, of course, the `params` JSON array, which we're putting to full use for the first time.

Note that the entire `params` is lodged in `[]`s to mark the parameters array.

We've also varied up the quoting from how things worked in
`bitcoin-cli`, to start and end each array and object within the
`params` array with `''` instead of our traditional `'''`. That's
because the entire set of JSON arguments already has a `'` around
it. As usual, just take a look at the bizarre shell quoting and get
used to it.

However, there's one last thing of note in this example, and it can be _maddening_ if you miss it. When you executed a `createrawtransaction` command with `bitcoin-cli` the JSON array of inputs and the JSON object of outputs were each distinct parameters, so they were separated by a space. Now, because they're part of that `params` JSON array, they're separated by a comma (`,`). Miss that and you'll get a `parse error` without much additional information.

> ⚠️ **WARNING: Logging Ahead.** Ever having troubles debugging your `curl`? Add the argument `--trace-ascii /tmp/foo`. Full information on what's being sent to the server will be saved in `/tmp/foo` (or whatever file name you provide).

Having verified that things work, you probably want to save the hex code into a variable:

```sh
hexcode=$(curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.0003, "'$changeaddress'": 0.0005}'']}' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.result')
```

### Sign and Send

Signing and sending your transaction using `curl` is an easy use of the `signrawtransactionwithwallet` and `sendrawtransaction` RPC:

```sh
signedhex=$(curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "signrawtransactionwithwallet", "params": ["'$hexcode'"] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.result | .hex')

curl --user StandUp:4b42b0447b08784cb41adedac54d7f26 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "sendrawtransaction", "params": ["'$signedhex'"] }' -H 'content-type: text/plain;' http://127.0.0.1:38332/ 2> /dev/null | jq -r '.'

| {
|   "result": "d4ce7ab7db122373567addca4aa5a2866fc617b720db8147748b4108834144ff",
|   "error": null,
|   "id": "curltest"
| }
```

Whew! We're not going to do that again! (Using `bitcoin-cli` is complex enough!)

## Summary: Accessing Bitcoind with Curl

Having finished this section, you may feel that accessing `bitcoind`
via `curl` is very much like accessing it through `bitcoin-cli`
... but more cumbersome. And, you'd be right. `bitcoin-cli` has pretty
complete RPC functionality, so anything that you do through `curl` you
can probably do through `bitcoin-cli`. Which is why we're going to
continue concentrating on `bitcoin-cli` following this digression.

But there are still reasons you'd use `curl` instead of `bitcoin-cli`:

> 🔥 **What is the power of curl?** Most obviously, `curl` takes out one level of indirection. Instead of working with `bitcoin-cli` which sends RPC commands to `bitcoind`, you're sending those RPC commands directly. This allows for more robust programming, because you don't have to worry about what unexpected things that `bitcoin-cli` might do or how it might change over time. However, you're also taking your first steps toward using a more comprehensive programming language than the poor options offered by a shell script. Ultimately, you might use libraries that provide access to curl functionality to access the RPC commands in a variety of programming languages—but that goes beyond the scope of the command line.

## What's Next?

Learn one more way to "Send Bitcoin Transactions" with [§5.5 Sending Coins with Automated Raw Transactions](05_5_Sending_Coins_with_Automated_Raw_Transactions.md).
