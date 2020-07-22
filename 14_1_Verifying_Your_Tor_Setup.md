# Chapter 12: Verifying Your Tor Setup

In this chapter we will verify tor installation and setup.

```
~$ sudo -u debian-tor tor --verify-config
```

If tor is installed correctly you should see an output like this:

```
Jun 26 21:52:09.230 [notice] Tor 0.4.3.5 running on Linux with Libevent 2.0.21-stable, OpenSSL 1.0.2n, Zlib 1.2.11, Liblzma 5.2.2, and Libzstd N/A.
Jun 26 21:52:09.230 [notice] Tor can't help you if you use it wrong! Learn how to be safe at https://www.torproject.org/download/download#warning
Jun 26 21:52:09.230 [notice] Read configuration file "/etc/tor/torrc".
Configuration was valid

~$ 
```
## Verify bitcoind Tor setup

You should see something like this in your debug log file to verify your ID onion address, which confirms you're using an onion hidden service that will bypass firewalls and NAT's and allows you connect to your node remotely using your ID and port.

```
$ grep -e "tor: " debug.log
```
Output

```
2020-06-25T18:16:44Z tor: Thread interrupt
2020-06-25T19:11:12Z tor: Got service ID [YOUR_ONION_ID], advertising service your_onion_id.onion:8333
```
Using bitcoin-cli you should use getnetworkinfo to get your onion id like this:

```
 "localaddresses": [
    {
      "address": "your_onion_id.onion",
      "port": 8333,
      "score": 4
    }
  ],
```

```
$ bitcoin-cli getnetworkinfo
```
Output

```
{
  "version": 200000,
  "subversion": "/Satoshi:0.20.0/",
  "protocolversion": 70015,
  "localservices": "0000000000000409",
  "localservicesnames": [
    "NETWORK",
    "WITNESS",
    "NETWORK_LIMITED"
  ],
  "localrelay": true,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 5,
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
      "address": "your_onion_id.onion",
      "port": 8333,
      "score": 4
    }
  ],
  "warnings": ""
}
```
