# 14.1: Verifying Your Tor Setup

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

If you did a standard installation with [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup) then you should have Tor set up as part of your Bitcoin node: Tor is installed and has created hidden services for the Bitcoin RPC ports; while an onion address has also been created for `bitcoind`. This section talks about what all of that is and what to do with it.

> :book: ***What is Tor?*** Tor is a low-latency anonymity and overlay network based on onion routing and path-building design for enabling anonymous communication. It's free and open-source software with the name derived from the acronym for the original software project name: "The Onion Router".

> :book: ***Why Use Tor for Bitcoin?*** The Bitcoin network is a peer-to-peer network that listens for transactions and propagates them using a public IP address.  When connecting to the network not using Tor, you would share your IP address, which could expose your location, your uptime, and others details to third parties — which is an undesirable privacy practice. To protect yourself online you should use tools like Tor to hide your connection details. Tor allows you to improve your privacy online as your data is cryptographically encoded and goes through different nodes, each one decoding a single layer (hence the onion metaphor).

## Understand Tor

So how does Tor work?

When a user wants to connect to an Internet server, Tor tries to build a path formed by at least three Tor relay nodes, called Guard, Middle, and Exit. While building this path, symmetric encryption keys are negotiated; when a message moves along the path, each relay then strips off its layer of encryption. In this way, the message arrives at the final destination in its original form, and each party only knows the previous and the next hop and cannot determine origin or destination.

Here's what a connection looks like without Tor:
```
20:58:03.804787 IP bitcoin.36300 > lb-140-82-114-25-iad.github.com.443: Flags [P.], seq 1:30, ack 25, win 501, options [nop,nop,TS val 3087919981 ecr 802303366], length 29
```
Contrariwise, with Tor much less information about the actual machines is transmitted:
```
21:06:52.744602 IP bitcoin.58776 > 195-xxx-xxx-x.rev.pxxxxxm.eu.9999: Flags [P.], seq 264139:265189, ack 3519373, win 3410, options [nop,nop,TS val 209009853 ecr 3018177498], length 1050
21:06:52.776968 IP 195-xxx-xxx-x.rev.pxxxxxm.eu.9999 > bitcoin.58776: Flags [.], ack 265189, win 501, options [nop,nop,TS val 3018177533 ecr 209009853], length 0
```
Bottom line: Tor encrypts your data in such a way that it hides your origin, your destination, and what services you're using, whereas a standard encryption protocol like TLS *only* protects what your data contains.

### Understand the Tor Network Architecture

The basic architecture of the Tor network is made up of the following components:

* **Tor Client (OP or Onion Proxy).** A Tor client installs local software that acts as an onion proxy. It packages application data into cells that are all the same size (512 bytes), which it then sends to the Tor network. A cell is the basic unit of Tor transmission.
* **Onion Node (OR or Onion Router).** An onion node transmits cells coming from the Tor client and from online servers. There are three types of onion nodes: input (Guard), intermediate nodes (Middle), and output nodes (Exit).
*  **Directory Server.** A Directory server stores information about onion routers and onion servers (hidden services), such as their public keys.
*  **Onion Server (hidden server).** An onion server supports TCP applications such as web pages or IRC as services.

### Understand the Limitations of Tor

Tor isn't a perfect tool. Because information from the Tor network is decrypted at the exit nodes before being sent to its final destinations, theoretically an observer could collect sufficient metadata to compromise anonymity and potentially identify users.

There are also studies that suggest that possible exploits of Bitcoin's anti-DoS protection could allow an attacker to force other users who use Tor to connect exclusively through his Tor Exit nodes or to his Bitcoin peers,  isolating the client from the rest of the Bitcoin network and exposing them to censorship, correlation, and other attacks.

Similarly, Bitcoin Tor users could be fingerprint-attacked by setting an address cookie on their nodes. This would also allow correlation and thus deanonymization.

Meanwhile, even over Tor, Bitcoin is only a pseudoanonymous service due to the many dangers of correlation that stem from the permanent ledger itself. This means that Bitcoin usage over Tor is actually more likely to be _deanonymized_ than other services (and could lead to the deanonymization of other activities).

With that said, Tor is generally considered safer than the alternative, which is non-anonymous browsing.

## Verify Your Tor Setup

So how do you verify that you've enabled Tor? If you installed with Bitcoin Standup, the following will verify that Tor is running on your system
```
$ sudo -u debian-tor tor --verify-config
```

If Tor is installed correctly you should output like this:
```
Jun 26 21:52:09.230 [notice] Tor 0.4.3.5 running on Linux with Libevent 2.0.21-stable, OpenSSL 1.0.2n, Zlib 1.2.11, Liblzma 5.2.2, and Libzstd N/A.
Jun 26 21:52:09.230 [notice] Tor can't help you if you use it wrong! Learn how to be safe at https://www.torproject.org/download/download#warning
Jun 26 21:52:09.230 [notice] Read configuration file "/etc/tor/torrc".
Configuration was valid
```
> :warning: **WARNING:** This just means that Tor is running, not that its being used for all (or any) connections.

### Verify Your Tor Setup for RPC

The most important purpose of Tor, as installed by Bitcoin Standup, is to offer hidden services for the RPC ports that are used to send command-line style commands to `bitcoind`.

> :book: ***What is a Tor Hidden Service?*** A hidden service (aka "an onion service") is a service that is accessible via Tor. Connection made to that service _using the Onion Network_ will be anonymized.

The Tor config file is found at `/etc/tor/torrc`. If you look at it, you should see the following services to protect your RPC ports:
```
HiddenServiceDir /var/lib/tor/standup/
HiddenServiceVersion 3
HiddenServicePort 1309 127.0.0.1:18332
HiddenServicePort 1309 127.0.0.1:18443
HiddenServicePort 1309 127.0.0.1:8332
```
> :link: **TESTNET vs MAINNET:** Mainnet RPC is run on port 8332, testnet on port 18332.

> :information_source: **NOTE:** The `HiddenServiceDir` is where all the files are kept for this particular service. If you need to lookup your onion address, access keys, or add authorized clients, this is where to do so!

The easy way to test your RPC Hidden Service is to use the [QuickConnect API](https://github.com/BlockchainCommons/Bitcoin-Standup/blob/master/Docs/Quick-Connect-API.md) built into Bitcoin Standup. Just download the QR code found at `/qrcode.png` and scan it using a wallet or node that support QuickConnect, such as [The Gordian Wallet](https://github.com/BlockchainCommons/FullyNoded-2). When you scan the QR, you should see the wallet sync up with your node; it's doing so using the RPC hidden services.

The hard way to test your RPC Hidden Service is to send a `bitcoin-cli` command with `torify`, which allows you to translate a normal UNIX command to a Tor-protected command. It's difficult because you need to grab three pieces of information.

1. **Your Hidden Service Port.** This comes from `/etc/tor/torrc/`. By default, it's port 1309.
2. **Your Tor Address.** This is in the `hostname` file in the `HiddenServiceDir` directory defined in `/etc/tor/torrc`. By default the file is thus `/var/lib/tor/standup/hostname`. It's protected, so you'll need to `sudo` to access it:
```
$ sudo more /var/lib/tor/standup/hostname
mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion
```
3. **Your RPC Password.** This is in `~/.bitcoin/bitcoin.conf`

When you have all of that information you can issue a `bitcoin-cli` command using `torify` and specifying the `-rpcconnect` as your onion address, the `-rpcport` as your hidden service port, and the `-rpcpassword` as your password:
```
$ torify bitcoin-cli -rpcconnect=mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion -rpcport=1309 -rpcuser=StandUp -rpcpassword=685316cc239c24ba71fd0969fa55634f getblockcount
```

### Verify Your Tor Setup for Bitcoind

Bitcoin Standup also ensures that your `bitcoind` is set up to optionally communicate on an onion address.

You can verify the initial setup of Tor for `bitcoind` by grepping for "tor" in the `debug.log` in your data directory:
```
$ grep "tor:" ~/.bitcoin/testnet3/debug.log
2021-06-09T14:07:04Z tor: ADD_ONION successful
2021-06-09T14:07:04Z tor: Got service ID vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd, advertising service vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion:18333
2021-06-09T14:07:04Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_v3_private_key
```
> :information_source: **NOTE:** Bitcoin Core does not support v2 addresses anymore. Tor v2 support was removed in [#22050](https://github.com/bitcoin/bitcoin/pull/22050)

> **TESTNET vs MAINNET:** Mainnet `bitcoind` responds on port 8333, testnet on port 18333.

You can verify that a Tor hidden service has been created for Bitcoin with the `getnetworkinfo` RPC call:

```
$ bitcoin-cli getnetworkinfo
...
 "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
...
```
This shows three addresses to access your Bitcoin server, an IPv4 address (`173.255.245.83`), an IPv6 address (`2600:3c01::f03c:92ff:fe86:f26`), and a Tor address (`vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion`).

> :warning: **WARNING:** Obviously: never reveal your Tor address in a way that's associated with your name or other PII!

You can see similar information with `getnetworkinfo`.
```
 bitcoin-cli getnetworkinfo
{
  "version": 200000,
  "subversion": "/Satoshi:0.20.0/",
  "protocolversion": 70015,
  "localservices": "0000000000000408",
  "localservicesnames": [
    "WITNESS",
    "NETWORK_LIMITED"
  ],
  "localrelay": true,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 10,
  "networks": [
    {
      "name": "ipv4",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "ipv6",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "onion",
      "limited": false,
      "reachable": true,
      "proxy": "127.0.0.1:9050",
      "proxy_randomize_credentials": true
    }
  ],
  "relayfee": 0.00001000,
  "incrementalfee": 0.00001000,
  "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
  "warnings": ""
}
```
This hidden service will allow anonymous connections to your `bitcoind` over the Bitcoin Network.

> :warning: **WARNING:** Running Tor and having a Tor hidden service doesn't force either you or your peers to use Tor.

### Verify Your Tor Setup for Peers

Using the RPC command `getpeerinfo`, you can see what nodes are connected to your node and check whether they are connected with Tor.

```
$ bitcoin-cli getpeerinfo
```
Some might be connected via Tor:
```
...
{
    "id": 9,
    "addr": "nkv.......xxx.onion:8333",
    "addrbind": "127.0.0.1:51716",
    "services": "000000000000040d",
    "servicesnames": [
      "NETWORK",
      "BLOOM",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1593981053,
    "lastrecv": 1593981057,
    "bytessent": 1748,
    "bytesrecv": 41376,
    "conntime": 1593980917,
    "timeoffset": -38,
    "pingwait": 81.649295,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 637875,
    "banscore": 0,
    "synced_headers": -1,
    "synced_blocks": -1,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00000000,
    "bytessent_per_msg": {
      "addr": 55,
      "feefilter": 32,
      "getaddr": 24,
      "getheaders": 1053,
      "inv": 280,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 30082,
      "feefilter": 32,
      "getdata": 280,
      "getheaders": 1053,
      "headers": 106,
      "inv": 9519,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    }
  }
...
```
Some might not, such as this IPv6 connection:
```
...
  {
    "id": 17,
    "addr": "[2001:638:a000:4140::ffff:191]:18333",
    "addrlocal": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "addrbind": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "services": "0000000000000409",
    "servicesnames": [
      "NETWORK",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1595447081,
    "lastrecv": 1595447067,
    "bytessent": 12250453,
    "bytesrecv": 2298711417,
    "conntime": 1594836414,
    "timeoffset": -1,
    "pingtime": 0.165518,
    "minping": 0.156638,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 1780784,
    "banscore": 0,
    "synced_headers": 1781391,
    "synced_blocks": 1781391,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00001000,
    "bytessent_per_msg": {
      "addr": 4760,
      "feefilter": 32,
      "getaddr": 24,
      "getdata": 8151183,
      "getheaders": 1085,
      "headers": 62858,
      "inv": 3559475,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 132,
      "sendheaders": 24,
      "tx": 145098,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 33877,
      "block": 2291124374,
      "feefilter": 32,
      "getdata": 9430,
      "getheaders": 1085,
      "headers": 60950,
      "inv": 2019175,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 66,
      "sendheaders": 24,
      "tx": 5136622,
      "verack": 24,
      "version": 126
    }
  }
...
```
Having a Tor address for your `bitcoind` is probably somewhat less useful than having a Tor address for your RPC connections. That's in part because it's not recommended to try and send all your Bitcoin connections via Tor, and in part because protecting your RPC commands is really what's important: you're much more likely to be doing that remotely, from a software wallet like The Gordian Wallet, while your server itself is more likely to be sitting in your office, basement, or bunker.

Nonetheless, there are ways to make `bitcoind` use Tor more, as discussed in the next section.

## Summary: Verifying Your Tor Setup

Tor is a software package installed at part of Bitcoin Standup that allows you to exchange communications anonymously. It will protect both your RPC ports (8332 or 18332) and your `bitcoind` ports (8333 or 18333) — but you have to actively connect to the onion address to use them! Tor is a building stone of privacy and security for your Bitcoin setup, and you can verify it's available and linked to Bitcoin with a few simple commands.

> :fire: ***What is the power of Tor?*** Many attacks on Bitcoin users depend on knowing who the victim is and that they're transacting Bitcoins. Tor can protect you from that by hiding both where you are and what you're doing. It's particularly important if you want to connect to your own node remotely via a software wallet, and can be crucial if you do so in some country where you might not feel that your Bitcoin usage is appreciated or protected. If you must take your Bitcoin services on the road, make sure that your wallet fully supports Tor and exchanges all RPC commands with your server using that protocol.

## What's Next?

Continue "Understanding Tor" with [§14.2: Changing Your Bitcoin Hidden Services](14_2_Changing_Your_Bitcoin_Hidden_Services.md).
