# 12.1: Accessing Bitcoind with Curl

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

`bitcoin-cli`, the heart of chapters 3-6, is ultimately just a wrapper. It's a way to interface with `bitcoind` from the command line, providing simplified access to its many RPC commands. But RPC can, of course, be accessed directly. That's what this chapter is about: directly connecting to RPC with the `curl` command.

## Know Your Curl

`curl`, short for "see URL", is a command-line tool that allows you to directly access URLs in a programmatic way. It's an easy way to interact with servers that listen to ports on the internet and that speak a variety of protocols, among them `bitcoind`. It also comes as a library for many programming languages, such as C, Java, PHP, and Python. So, once you know how to work with Curl, you'll have a strong foundation for using a lot of different APIs — some of which we'll touch on in the next few chapters.

In order to use `curl` with `bitcoind`, you must know three things: the standard format, the user name and password, and the correct port.

### Know Your Format

The `bitcoin-cli` commands are all linked to the RPC commands in `bitcoind`. That makes the transition from using `bitcoin-cli` to using `curl` very simply. In fact, if you look at any of the help pages for `bitcoin-cli`, you'll see that they list not only the `bitcoin-cli` command, but also a parallel `curl` command. For example, here is `bitcoin-cli getinfo help`:
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
And there's the `curl` command, at the end of the help screen! This somewhat lengthy command has four major parts: (1) a listing of your user name; (2) a `--data-binary` flag; (3) a JSON object that tells `bitcoind` what to do; and (4) an HTTP header that includes the URL to talk to. When you are working with `curl`, most of this will stay the same from command to command, other than the `method` and `params` entries in the JSON array. However, you need to know how to fill in your username and your URL address in order to make it work in the first place!

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
rpcpassword=d8340efbcd34e312044c8431c59c792c
txindex=1
testnet=1
```
Clearly, our user is `bitcoinrpc` and our password is `d8340efbcd34e312044c8431c59c792c`.

The secure way to access your user name is as follows:
```
$ curl --user bitcoinrpc --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
Enter host password for user 'bitcoinrpc':
```
As noted, you will be prompted for your password.

The insecure way to do so is as follows:
```
$ curl --user bitcoinrpc:d8340efbcd34e312044c8431c59c792c --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
> **WARNING:** Entering your password on the command line may put your password into the process table and/or save it into a history. It's not recommended, except for testing on testnet. If you want to do it anywhere else, make sure you know what you're doing!

### Know Your Port

The port should be easy, it's the `http://127.0.0.1:8332/` shown in the tutorials, right? Not exactly. It depends on whether you're using testnet or mainnet.

> **MAINNET VS. TESTNET.** To access RPC commands on the mainnet, use port 8332, but to do so on testnet, use port 18332.

However, with the right port number in hand, you should now be able to `curl` out to the `bitcoind`:
```
$ curl --user bitcoinrpc:d8340efbcd34e312044c8431c59c792c --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
{"result":{"blocks":1128599,"currentblocksize":0,"currentblockweight":0,"currentblocktx":0,"difficulty":1,"errors":"Warning: unknown new rules activated (versionbit 28)","networkhashps":8658807967387.751,"pooledtx":0,"chain":"test"},"error":null,"id":"curltest"}
```
You'll note that the result is another JSON array, which is kind of ugly to read if you're using `curl` by hand. That's why `bitcoin-cli` and `curl` libraries exist. But, for the moment, we'll struggle through the JSON inputs and outputs, to understand this as the low-level.

> **WARNING:** If you get a result like "Failed to connect to 127.0.0.1 port 8332: Connection refused", be sure that a line like `rpcallowip=127.0.0.1` is in your ~/.bitcoin/bitcoin.conf. If things still don't work, be sure that you're allowing access to port 18332 (or 8332) from localhost. Our standard setup from [Chapter Two: Creating a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) should do all of this.

## Understand the Limits of RPC

In each of these three chapters about accessing `bitcoind` via RPC, we're going to quickly run through lessons already learned, about wallet access and about sending raw transactions, as a guide to getting you started if you choose to use this message of access. Expect all of these access methods to be _harder_ than when you were working directly with `bitcoin-cli`.

There's a reason for this: `bitcoin-cli` actually provides two sorts of functionality. First, it gives you access to the RPC commands of `bitcoind`; and second, it proides you with basic wallet functionality while accessing those RPC commands. Now that you're accessing RPC commands directly, those wallet shortcuts are going to go away, leaving you on your own to deal with the wilds of Bitcoin. Mind you, this isn't necessarily a bad thing. When a programmer starts working with Bitcoin, often one of the first things he does is write a new set wallet functionality. Because he _needs_ it when working with Bitcoin more directly.

So, welcome to the club.

## Manipulate Your Wallet


## Create a Transaction

### Adjust the nVersion

## Summary: Accessing Bitcoind with Curl


