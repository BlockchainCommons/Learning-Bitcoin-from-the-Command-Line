21.1: Runing Bitcoin as an i2p Service

Rather than using the proxy-based Tor service to ensure the privacy of
your Bitcoin communications, you may instead wish to use I2P, which is
designed to act as a private network within the internet, rather than
simply offering private access to internet services.

## Understand the Differences

Tor and I2P both offer private access to online services, but with
different routing and databases, and with different architectures for
relays. Since hidden services (such as Bitcoin access) are core to the
design of I2P, they have also been better optimized:

|  | Tor | I2P |
| :---         |     :---:      |          ---: |
| Routing   | [Onion](https://www.onion-router.net/)     | [Garlic](https://geti2p.net/en/docs/how/garlic-routing)    |
| Network Database     | Trusted [Directory Servers](https://blog.torproject.org/possible-upcoming-attempts-disable-tor-network)       | [Distributed network database](https://geti2p.net/en/docs/how/network-database)      |
| Relay     | **Two-way** encrypted connections between each Relay       | **One-way** connections between every server in its tunnels      |
| Hidden services     | Slow       | Fast      |

A more detailed comparison may be found at [geti2p.net](https://geti2p.net/en/comparison/tor).

### Understand Tradeoffs for Limiting Outgoing Connections

There are
[tradeoffs](https://bitcoin.stackexchange.com/questions/107060/tor-and-i2p-tradeoffs-in-bitcoin-core)
if you choose to support only I2P, only Tor, or both. These
configurations, which limit outgoing clearnet connections, are made in
Bitcoin Core using the `onlynet` argument in your `bitcoin.conf`.

* `onlynet=onion`, which limits outgoing connections to Tor, can expose a node to Sybil attacks and can create network partitioning, because of limited connections between Tornet and the clearnet.
* `onlynet=onion` and `onlynet=i2p` in conjunction, which runs Onion service with I2P service is experimental for now.

## Install I2P

To install I2P, you should make sure your ports are correctly set up and then you can continue with your setup process.

### Prepare Ports

To use I2P, you will need to open the following ports, which are required by I2P:

1. **Outbound (Internet facing):** a random port between 9000 and 31000 is selected. It is best if all these ports are open for outbound connections, which doesn't affect your security. 
- You can check firewall status using `sudo ufw status verbose`, which shouldn't deny outgoing connections by default.
2. Inbound (Internet facing): optional. A variety of inbound ports are listed in the [I2P docs](https://geti2p.net/en/faq#ports).
- For maximum privacy, it is preferable to disable accepting incoming connections.

### Run I2P

The following will run Bitcoin Core I2P services:

1. Install `i2pd` on Ubuntu:

   ```sh
   sudo apt-get install i2pd
   ```

   For installing on other OSes, see [these docs](https://i2pd.readthedocs.io/en/latest/user-guide/install/)

2. [Run](https://i2pd.readthedocs.io/en/latest/user-guide/run/) the I2P service:

   ```sh
   sudo systemctl start i2pd.service
   ```

3. Add the following lines in `bitcoin.conf`: 

   ```sh
   i2psam=127.0.0.1:7656
   debug=i2p
   ```
   The logging option, `debug=i2p`, is used to record additional information in the debug log about your I2P configuration and connections. The default location for this debugging file on Linux is: `~/.bitcoin/debug.log`:

4. Restart `bitcoind`

   ```sh
   kill [IP]
   /usr/local/bin/bitcoind -conf=/home/standup/.bitcoin/bitcoin.conf -daemon
   ```

5. Check `debug.log` to see if I2P was setup correctly, or if any errors appeared in the logs.  
   ```sh
   grep i2p ~/.bitcoin/signet/debug.log 

   | 2026-10-30T00:32:24Z Config file arg: debug="i2p"
   | 2026-10-30T00:32:24Z Config file arg: i2psam="127.0.0.1:7656"
   | 2026-10-30T00:32:26Z i2paccept thread start
   | 2026-10-30T00:32:26Z [i2p] Creating persistent I2P SAM session b8ce018cdd with 127.0.0.1:7656
   ```
   
6. Confirm `i2p_private_key` was created in the Bitcoin Core data directory. The first time Bitcoin Core connects to the I2P router, its I2P address (and corresponding private key) will be automatically generated and saved in a file named *i2p_private_key*:

   ```sh
   ls ~/.bitcoin/signet/
   
   | banlist.json  blocks      debug.log  i2p_private_key  onion_v3_private_key  settings.json
   | bitcoind.pid  chainstate  fees       mempool.dat      peers.dat             wallets
   ```

7. Check that `bitcoin-cli getnetworkinfo` show that i2p is reachable:

   ```sh
   bitcoin-cli getnetworkinfo

   | {
   |
   | ...
   |
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
   |       "limited": false,
   |       "reachable": true,
   |       "proxy": "127.0.0.1:7656",
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
   |
   | ...
   |
   | }

   ```
  
You now have your Bitcoin server accessible through the I2P network at your new local address.

## Summary: Bitcoin Core as an I2P (Invisible Internet Project) service

It is always good to have alternatives for privacy and not depend
solely on Tor for running Bitcoin Core as a hidden service. Since I2P
was recently added in Bitcoin Core, not many people use it. Experiment
with it and report bugs if you find any issues.

> ℹ️ **For more ...** For the official i2prouter implementation in
Java, visit the [I2P download page](https://geti2p.net/en/download)
and follow the instructions for your Operating System. Once installed,
open a terminal window and type `i2prouter start`. Then visit
`127.0.0.1:7657` in your browser to enable SAM. To do so, select:
"Configure Homepage", then "Clients", and finally select the "Play
Button" next to SAM application Bridge. On the left side of the page,
there should be a green light next to "Shared Clients".

