# 20.1: Verifying Your Tor Setup

If you did a standard installation with [Bitcoin
Standup](https://github.com/BlockchainCommons/Bitcoin-Standup) then
you should have Tor set up as part of your Bitcoin node: Tor is
installed and has created hidden services for the Bitcoin RPC ports;
while an onion address has also been created for `bitcoind`. This
section talks about what all of that is and what to do with it.

> 📖 ***What is Tor?*** Tor is a low-latency anonymity and overlay
network based on onion routing and path-building design for enabling
anonymous communication. It's free and open-source software with the
name derived from the acronym for the original software project name:
"The Onion Router".

> 📖 ***Why Use Tor for Bitcoin?*** The Bitcoin network is a
peer-to-peer network that listens for transactions and propagates them
using a public IP address.  When connecting to the network not using
Tor, you would share your IP address, which could expose your
location, your uptime, and others details to third parties — which is
an undesirable privacy practice. To protect yourself online you should
use tools like Tor to hide your connection details. Tor allows you to
improve your privacy online as your data is cryptographically encoded
and goes through different nodes, each one decoding a single layer
(hence the onion metaphor).

## Understand Tor

So how does Tor work?

When a user wants to connect to an Internet server, Tor tries to build
a path formed by at least three Tor relay nodes, called Guard, Middle,
and Exit. While building this path, symmetric encryption keys are
negotiated; when a message moves along the path, each relay then
strips off its layer of encryption (the metaphorical layer of an
onion). In this way, the message arrives at the final destination in
its original form, and each party only knows the previous and the next
hop and cannot determine origin or destination.

Here's what a connection looks like without Tor:
```
20:58:03.804787 IP bitcoin.36300 > lb-140-82-114-25-iad.github.com.443: Flags [P.], seq 1:30, ack 25, win 501, options [nop,nop,TS val 3087919981 ecr 802303366], length 29
```
Contrariwise, with Tor much less information about the actual machines is transmitted:
```
21:06:52.744602 IP bitcoin.58776 > 195-xxx-xxx-x.rev.pxxxxxm.eu.9999: Flags [P.], seq 264139:265189, ack 3519373, win 3410, options [nop,nop,TS val 209009853 ecr 3018177498], length 1050
21:06:52.776968 IP 195-xxx-xxx-x.rev.pxxxxxm.eu.9999 > bitcoin.58776: Flags [.], ack 265189, win 501, options [nop,nop,TS val 3018177533 ecr 209009853], length 0
```

Bottom line: Tor encrypts your data in such a way that it hides your
origin, your destination, and what services you're using, whereas a
standard encryption protocol like TLS *only* protects what your data
contains.

### Understand the Tor Network Architecture

The basic architecture of the Tor network is made up of the following
components:

* **Tor Client (OP or Onion Proxy).** A Tor client installs local software that acts as an onion proxy. It packages application data into cells that are all the same size (512 bytes), which it then sends to the Tor network. A cell is the basic unit of Tor transmission.
* **Onion Node (OR or Onion Router).** An onion node transmits cells coming from the Tor client and from online servers. There are three types of onion nodes: input (Guard), intermediate nodes (Middle), and output nodes (Exit).
*  **Directory Server.** A Directory server stores information about onion routers and onion servers (hidden services), such as their public keys.
*  **Onion Server (hidden server).** An onion server supports TCP applications such as web pages or IRC as services.

### Understand the Limitations of Tor

Tor isn't a perfect tool. Because information from the Tor network is
decrypted at the exit nodes before being sent to its final
destinations, theoretically an observer could collect sufficient
metadata to compromise anonymity and potentially identify users.

There are also studies that suggest that possible exploits of
Bitcoin's anti-DoS protection could allow an attacker to force other
users who use Tor to connect exclusively through his Tor Exit nodes or
to his Bitcoin peers, isolating the client from the rest of the
Bitcoin network and exposing them to censorship, correlation, and
other attacks.

Similarly, Bitcoin Tor users could be fingerprint-attacked by setting
an address cookie on their nodes. This would also allow correlation
and thus deanonymization.

Meanwhile, even over Tor, Bitcoin is only a pseudoanonymous service
due to the many dangers of correlation that stem from the permanent
ledger itself. This means that Bitcoin usage over Tor is actually more
likely to be _deanonymized_ than other services (and could lead to the
deanonymization of other activities).

With that said, Tor is generally considered safer than the alternative, which is non-anonymous browsing.

## Verify Your Tor Setup

So how do you verify that you've enabled Tor? If you installed with
Bitcoin Standup, the following will verify that Tor is running on your
system.

```
$ sudo -u debian-tor tor --verify-config
```

If Tor is installed correctly you should output like this:
```
Oct 29 12:10:58.346 [notice] Tor 0.4.9.14 running on Linux with Libevent 2.1.13-stable, OpenSSL 3.5.7, Zlib 1.3.1, Liblzma 5.8.1, Libzstd 1.5.7 and Glibc 2.41 as libc.
Oct 29 12:10:58.346 [notice] Tor can't help you if you use it wrong! Learn how to be safe at https://support.torproject.org/faq/staying-anonymous/
Oct 29 12:10:58.346 [notice] Read configuration file "/etc/tor/torrc".
Configuration was valid
```
> ⚠️ **Verification is Limited:** This just means that Tor is running, not that its being used for all (or any) connections.

At this point you should be able to use Tor in two ways. First, you
should be able to send RPC commands via Tor (rather than unprotected
methods such as plain `bitcoin-cli` or `curl`). Second, other nodes
should be able to connect to your server via Tor. We'll test each of
these out in turn.

### Verify Your Tor Setup for RPC

The most important purpose of Tor, as installed by Bitcoin Standup, is
to offer hidden services for the RPC ports that are used to send
command-line style commands to `bitcoind`.

> 📖 ***What is a Tor Hidden Service?*** A hidden service (aka "an
onion service") is a service that is accessible via Tor. Connection
made to that service _using the Onion Network_ will be anonymized.

The Tor config file is found at `/etc/tor/torrc`. If you look at it,
you should see the following services to protect your RPC ports:

```
HiddenServiceDir /var/lib/tor/standup/
HiddenServiceVersion 3
HiddenServicePort 38332 127.0.0.1:38332
HiddenServicePort 18332 127.0.0.1:18332
HiddenServicePort 18443 127.0.0.1:18443
HiddenServicePort 8332 127.0.0.1:8332
```
> 🔗 **SIGNET vs MAINNET:** Mainnet RPC is run on port 8332, testnet on port 18332, regtest on port 18443, signet on port 38332.

> ℹ️ **Note the Directory:** The `HiddenServiceDir` is where all the
files are kept for this particular service. If you need to lookup your
onion address, access keys, or add authorized clients, this is where
to do so! (It will require root/sudo access on the default Standup
setup.)

The easy way to test your RPC Hidden Service is to use the
[QuickConnect
API](https://github.com/BlockchainCommons/Bitcoin-Standup/blob/master/Docs/Quick-Connect-API.md)
built into Bitcoin Standup. Just download the QR code found at
`/qrcode.png` and scan it using a wallet or node that support
QuickConnect, such as [The Gordian
Wallet](https://github.com/BlockchainCommons/FullyNoded-2). When you
scan the QR, you should see the wallet sync up with your node; it's
doing so using the RPC hidden services.

The hard way to test your RPC Hidden Service is to send a
`bitcoin-cli` command with `torify`, which allows you to translate a
normal UNIX command to a Tor-protected command. It's difficult because
you need to grab three pieces of information.

1. **Your Hidden Service Port.** This comes from `/etc/tor/torrc/`. By default, all of the Tor ports are set to mirror the regular TCP/IP ports, so the Signet server is at 38332.
2. **Your Tor Address.** This is in the `hostname` file in the `HiddenServiceDir` directory defined in `/etc/tor/torrc`. By default the file is thus `/var/lib/tor/standup/hostname`. It's protected, so you'll need to `sudo` to access it:
```
sudo more /var/lib/tor/standup/hostname

| w3ibwqdeshamshemws242opykpjo2a7wduk6xrmsp6vuovgpjelo7uqd.onion

```
3. **Your RPC User & Password.** This is in `~/.bitcoin/bitcoin.conf`

```
cat ~/.bitcoin/bitcoin.conf | grep -E 'rpcuser|rpcpassword'

| rpcuser=StandUp
| rpcpassword=6da3cf0a3534a7b44aabf0f5ab240f34
```

When you have all of that information you can issue a `bitcoin-cli`
command using `torify` and specifying the `-rpcconnect` as your onion
address, the `-rpcport` as your hidden service port, the `-rpcuser` as
your user, and the `-rpcpassword` as your password:

```
torify bitcoin-cli -rpcconnect=w3ibwqdeshamshemws242opykpjo2a7wduk6xrmsp6vuovgpjelo7uqd.onion -rpcport=38332 -rpcuser=StandUp -rpcpassword=6da3cf0a3534a7b44aabf0f5ab240f34 getblockcount
```

> ⚠️ **ISPs May Block You:** Unfortunately, ISPs and cloud providers
increasingly are blocking ports. Wallets and `bitcoin-cli` may both be
unable to connect to the torified RPC ports that you have set up. 

### Verify Your Tor Setup for Bitcoind

Bitcoin Standup also ensures that your `bitcoind` is set up to
optionally communicate on an onion address.

You can verify the initial setup of Tor for `bitcoind` by grepping for "tor" in the `debug.log` in your data directory:

```
grep "tor" ~/.bitcoin/signet/debug.log

...

| 2026-10-29T19:32:16Z [tor] ADD_ONION successful (PoW defenses enabled)
| 2026-10-29T19:32:16Z Got tor service ID gpp7dfgrhttrz25mysiamgfrkti4oizvdnzu7zjo67lupbomp6etxsqd, advertising service gpp7dfgrhttrz25mysiamgfrkti4oizvdnzu7zjo67lupbomp6etxsqd.onion:38333
| 2026-10-29T19:32:16Z [tor] Cached service private key to /home/standup/.bitcoin/signet/onion_v3_private_key
```
> 📖 **v3 Address or Better!** Bitcoin Core does not support v2 addresses anymore. Tor v2 support was removed in [#22050](https://github.com/bitcoin/bitcoin/pull/22050)

You can verify that a Tor hidden service has been created for Bitcoin with the `getnetworkinfo` RPC call:

```
bitcoin-cli getnetworkinfo

| {
| 
| ...
| 
|   "localaddresses": [
|     {
|       "address": "gpp7dfgrhttrz25mysiamgfrkti4oizvdnzu7zjo67lupbomp6etxsqd.onion",
|       "port": 38333,
|       "score": 4
|     }
| 
| ...
| 
| }
```

This shows the Tor address and port
(`gpp7dfgrhttrz25mysiamgfrkti4oizvdnzu7zjo67lupbomp6etxsqd.onion:38333`)
necessary to access your server.

> 🔗 **TESTNET vs MAINNET:** Mainnet `bitcoind` responds on port 8333, testnet on port 18333, signet on 38333.


> ⚠️ **Keep Your Tor Addresses Private!:** Obviously: never reveal your Tor address in a way that's associated with your name or other PII!

You can see similar information with `getnetworkinfo`.

```
bitcoin-cli getnetworkinfo

| {
|   "version": 320100,
|   "subversion": "/Satoshi:32.1.0/",
|   "protocolversion": 70017,
|   "localservices": "0000000000000c08",
|   "localservicesnames": [
|     "WITNESS",
|     "NETWORK_LIMITED",
|     "P2P_V2"
|   ],
|   "localrelay": true,
|   "timeoffset": 0,
|   "tx_send_rate": 14,
|   "inv_buckets": {
|     "inbound": {
|       "backlog": 0,
|       "count_tok": 420,
|       "size_tok": 49999991.4386
|     },
|     "outbound": {
|       "backlog": 0,
|       "count_tok": 420,
|       "size_tok": 50000000
|     }
|   },
|   "networkactive": true,
|   "connections": 10,
|   "connections_in": 0,
|   "connections_out": 10,
|   "networks": [
|     {
|       "name": "ipv4",
|       "limited": false,
|       "reachable": true,
|       "proxy": "",
|       "proxy_randomize_credentials": false
|     },
|     {
|       "name": "ipv6",
|       "limited": false,
|       "reachable": true,
|       "proxy": "",
|       "proxy_randomize_credentials": false
|     },
|     {
|       "name": "onion",
|       "limited": false,
|       "reachable": true,
|       "proxy": "127.0.0.1:9050",
|       "proxy_randomize_credentials": true
|     },
|     {
|       "name": "i2p",
|       "limited": true,
|       "reachable": false,
|       "proxy": "",
|       "proxy_randomize_credentials": false
|     },
|     {
|       "name": "cjdns",
|       "limited": true,
|       "reachable": false,
|       "proxy": "",
|       "proxy_randomize_credentials": false
|     }
|   ],
|   "relayfee": 0.00000100,
|   "incrementalfee": 0.00000100,
|   "localaddresses": [
|     {
|       "address": "gpp7dfgrhttrz25mysiamgfrkti4oizvdnzu7zjo67lupbomp6etxsqd.onion",
|       "port": 38333,
|       "score": 4
|     }
|   ],
|   "warnings": [
|   ]
| }
```

This hidden service will allow anonymous connections to your
`bitcoind` over the Bitcoin Network.

> ⚠️: **Tor is Optional:** Running Tor and having a Tor hidden service
doesn't force either you or your peers to use Tor.

### Verify Your Tor Setup for Peers

Using the RPC command `getpeerinfo`, you can see what nodes are
connected to your node and check whether they are connected with Tor.

```
bitcoin-cli getpeerinfo
```
Some might be connected via Tor:
```
|   {
|     "id": 6,
|     "addr": "do4fhdpof3pzawpw5giszcdajp654vl5seypksca4jokmre5qwzd73yd.onion:38333",
|     "addrbind": "127.0.0.1:43472",
|     "network": "onion",
|     "services": "0000000000000c09",
|     "servicesnames": [
|       "NETWORK",
|       "WITNESS",
|       "NETWORK_LIMITED",
|       "P2P_V2"
|     ],
| 
|     ...
| 
|   }
| 
```

Some might not, such as this IPv6 connection:

```
|  {
|     "id": 9,
|     "addr": "[2401:2500:102:3007:153:126:143:201]:38333",
|     "addrbind": "[2600:3c01::2000:d9ff:fe05:a3b6]:48710",
|     "addrlocal": "[2600:3c01::2000:d9ff:fe05:a3b6]:48710",
|     "network": "ipv6",
|     "services": "0000000000000c09",
|     "servicesnames": [
|       "NETWORK",
|       "WITNESS",
|       "NETWORK_LIMITED",
|       "P2P_V2"
|     ],
| 
|     ...
|     
|   }
...

```

Having a Tor address for your `bitcoind` is probably somewhat less
useful than having a Tor address for your RPC connections. That's in
part because it's not recommended to try and send all your Bitcoin
connections via Tor, and in part because protecting your RPC commands
is really what's important: you're much more likely to be doing that
remotely, from a software wallet on a mobile device while your server
itself is more likely to be sitting in your office, basement, or
bunker.

Nonetheless, there are ways to make `bitcoind` use Tor more, as
discussed in the next section.

## Summary: Verifying Your Tor Setup

Tor is a software package installed at part of Bitcoin Standup that
allows you to exchange communications anonymously. It will protect
both your RPC ports (8332, 18332, 38332) and your `bitcoind` ports
(8333, 18333, 3833) — but you have to actively connect to the onion
address to use them! Tor is a building stone of privacy and security
for your Bitcoin setup, and you can verify it's available and linked
to Bitcoin with a few simple commands.

> 🔥 ***What is the power of Tor?*** Many attacks on Bitcoin users
depend on knowing who the victim is and that they're transacting
Bitcoins. Tor can protect you from that by hiding both where you are
and what you're doing. It's particularly important if you want to
connect to your own node remotely via a software wallet, and can be
crucial if you do so in some country where you might not feel that
your Bitcoin usage is appreciated or protected. If you must take your
Bitcoin services on the road, make sure that your wallet fully
supports Tor and exchanges all RPC commands with your server using
that protocol.

## What's Next?

Continue "Understanding Tor" with [§20.2: Changing Your Bitcoin Hidden Services](20_2_Changing_Your_Bitcoin_Hidden_Services.md).
