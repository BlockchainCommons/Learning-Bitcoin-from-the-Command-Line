# Chapter 12: Using Tor

In this section will talk about Tor network and how to use its services that are now available courtesy of [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts)

## 1. Tor network

Tor is a low-latency anonymity and overlay network based on onion routing and path-building design for enabling anonymous communication.  Tor is free and open-source software and the name derived from the acronym for the original software project name "The Onion Router".  The Tor Project, Inc. is a Massachusetts-based  research-education nonprofit organization founded by computer scientists Roger Dingledine, Nick Mathewson and others. The Tor Project is primarily responsible for maintaining software for the [Tor anonymity network](https://www.torproject.org/)

## 2. ¿Why use Bitcoin over Tor? 

The Bitcoin network is a peer-to-peer network that listen for transactions and propagates them using an IP public address.  When you use default configuration you share your ip address that could expose to a third party your location, your uptime and others details that becomes a undesirable privacy practice.   To protect you online you should use tools like Tor to hidden your connection details.   Tor allows improve your privacy online as your data goes through different nodes using cryptography and decoding one layer at a time at the different nodes.

### Weaknesses

Tor isn't a perfect tool and given that at the exit nodes the information of the Tor network is decrypted and sent to its final destinations, theoretically if someone is monitoring this traffic, and the data is no longer encrypted or observer can collect sufficiently metadata the anonymity is compromised and could potentially identify users.

There are some studies about possible exploits on Bitcoin's anti-DoS protection that could allow an attacker to force other users which uses Tor to connect exclusively through his Tor Exit nodes or to his Bitcoin peers,  isolating the client from the rest of the Bitcoin network. 
This could expose users to attacks in which the attacker controls which Bitcoin blocks and transactions receives and lost some level of anonymity.

Fingerprint attack Bitcoin Tor users by setting an address cookie on their nodes. This cookie  allows correlate different transactions of the user exposing transactions propagated without Tor and correlating to his IP address, and later deanonymizing all his transactions sent previously through Tor. 

## 3. How it works

When a user wants to connect to an Internet server Tor tries to build a path formed by at least three Tor nodes relays called Guard, Middle and Exit.  In this building path creates a circuit that negotiates encrypted symmetric keys,that while the message goes along the circuit, each relay strips off its layer of encryption. In this way the message arrives at the final destination in its original form and each party only knows the previous and the next hop and cannot determinate origin or destination.

The basic architecture of the Tor network is made up of the following components:
* Tor Clients (OP or Onion Proxy): A Tor client installs local software considered as an onion proxy, which packages the application data into cells the same size (512 bytes) that it sends to the Tor network. A cell is the basic unit of Tor transmission.
* Onion node (OR or Onion Router): Onion nodes transmit cells coming from the Tor client and server. There are three types of onion nodes:
input (Guard), intermediate nodes (Middle), and output nodes (Exit).
*  Directory servers: Directory servers store the information of onion routers and onion servers (hidden services), such as their
public keys.
*  Onion servers (hidden servers): They support TCP applications as a service web or IRC service.

### Clear internet connection

When a user connects to an Internet Server it shares it's ip address, location, operating system and other details with the server.   Using a data-network packet analyzer like tcpdump we see how connection is established.

```
$ tcpdump
```
Output

```
20:58:03.804787 IP bitcoin.36300 > lb-140-82-114-25-iad.github.com.443: Flags [P.], seq 1:30, ack 25, win 501, options [nop,nop,TS val 3087919981 ecr 802303366], length 29
```
When a user connects to an Internet Server using tor it create a circuit using three relay nodes that only knows previous and the next hop.   This is the output first server or Guard node.

```
21:06:52.744602 IP bitcoin.58776 > 195-xxx-xxx-x.rev.pxxxxxm.eu.9999: Flags [P.], seq 264139:265189, ack 3519373, win 3410, options [nop,nop,TS val 209009853 ecr 3018177498], length 1050
21:06:52.776968 IP 195-xxx-xxx-x.rev.pxxxxxm.eu.9999 > bitcoin.58776: Flags [.], ack 265189, win 501, options [nop,nop,TS val 3018177533 ecr 209009853], length 0
```
### Bitcoin tor connections

Using bitcoin-cli parameter getpeerinfo you can see what nodes are connected to your node and check that it swaps connections over tor.

```
$ bitcoin-cli getpeerinfo
```
Output

```
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
```





  * [12.1: Verifying Your Tor Setup](12_1_Verifying_Your_Tor_Setup.md)
  * [12.2: Changing Your Bitcoin Hidden Services](12_2_Changing_Your_Bitcoin_Hidden_Services.md)
  * [12.3: Adding SSH Hidden Services](12_3_Adding_SSH_Hidden_Services.md)
