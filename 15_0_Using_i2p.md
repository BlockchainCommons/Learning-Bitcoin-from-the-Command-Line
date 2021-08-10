# Chapter 15: Using i2p

The Invisible Internet Project (I2P) is a fully encrypted private network layer. It uses a distributed [network database](https://geti2p.net/en/docs/how/network-database) and encrypted unidirectional tunnels between your and your peers.

Basic differences between Tor and i2p:

|  | Tor | i2p |
| :---         |     :---:      |          ---: |
| Routing   | [Onion](https://www.onion-router.net/)     | [Garlic](https://geti2p.net/en/docs/how/garlic-routing)    |
| Network Database     | Trusted [Directory Servers](https://blog.torproject.org/possible-upcoming-attempts-disable-tor-network)       | [Distributed network database](https://geti2p.net/en/docs/how/network-database)      |
| Relay     | **Two-way** encrypted connections between each Relay       | **One-way** connections between every server in its tunnels      |
| Hidden services     | Slow       | Fast      |

Comparison in detail: https://geti2p.net/en/comparison/tor

Ports required by i2p:

1. Outbound (Internet facing): a random port between 9000 and 31000 is selected however its better if all ports are open and it doesn't affect your security. You can check firewall status using `sudo ufw status verbose` which shouldn't deny outgoing by default.

2. Inbound (Internet facing): Optional

Local ports: https://geti2p.net/en/faq#ports

It is not installed by [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts) right now as i2p support was recently added in Bitcoin core. However, you can try it manually by following the steps mentioned in [Section One](15_1_i2p_service.md).

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

  * Run Bitcoin Core as an I2P (Invisible Internet Project) service

Supporting objectives include the ability to:

  * Understand the i2p Network
  * Learn difference between Tor and i2p

## Table of Contents

* [Section One: Bitcoin Core as an I2P (Invisible Internet Project) service](15_1_i2p_service.md)