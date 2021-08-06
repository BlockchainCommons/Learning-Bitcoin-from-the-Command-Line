# 15.1: Bitcoin Core as an I2P (Invisible Internet Project) service

Users should consider different trade-offs involved in using i2p with other networks in Bitcoin Core for better privacy:

- Sybil Attacks and Network Partitioning is possible with `onlynet=i2p`
- Running Onion service with i2p service is experimental for now.
- For maximum privacy, it is preferable to disable accepting incoming connections.

Read more: https://bitcoin.stackexchange.com/questions/107060/tor-and-i2p-tradeoffs-in-bitcoin-core

Follow the below steps to run Bitcoin Core i2p service:

1. You can install `i2pd` on Ubuntu with below commands:

   ```
   sudo add-apt-repository ppa:purplei2p/i2pd
   sudo apt-get update
   sudo apt-get install i2pd
   ```

   For installing on other OS: https://i2pd.readthedocs.io/en/latest/user-guide/install/

2. Then [run](https://i2pd.readthedocs.io/en/latest/user-guide/run/) it and check if its running:

   ```
   $ sudo systemctl start i2pd.service
   ```

   You should see it running on port 7656:

   ```
   $ ss -nlt

   State   Recv-Q   Send-Q     Local Address:Port      Peer Address:Port  Process

   LISTEN  0        4096           127.0.0.1:7656           0.0.0.0:*
   ```

3. Add the following lines in `bitcoin.conf`. Config logging option `debug=i2p` is used to have additional information in the debug log about your I2P configuration and connections (Default location on Linux: ~/.bitcoin/bitcoin.conf):

   ```
   i2psam=127.0.0.1:7656
   debug=i2p
   ```

5. Restart `bitcoind`

   ```
   $ bitcoind
   ```

6. Check `debug.log`if i2p was setup correctly or any errors printed in logs.  I2P address is mentioned in the logs and ends with _b32.i2p_:
   ```
   2021-06-15T20:36:16Z i2paccept thread start
   2021-06-15T20:36:16Z I2P: Creating SAM session with 127.0.0.1:7656

   2021-06-15T20:36:56Z I2P: SAM session created: session id=3e0f35228b, my address=bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333
   2021-06-15T20:36:56Z AddLocal(bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333,4)
   ```

7. Confirm `i2p_private_key` was created in Bitcoin Core data directory. The first time Bitcoin Core connects to the I2P router, its I2P address (and corresponding private key) will be automatically generated and saved in a file named *i2p_private_key*:
   ```
   ~/.bitcoin/testnet3$ ls

   anchors.dat  chainstate         i2p_private_key  settings.json
   banlist.dat  debug.log          mempool.dat      wallets
   blocks       fee_estimates.dat  peers.dat
   ```

8. `bitcoin-cli -netinfo` or `bitcoin-cli getnetworkinfo`returns i2p address:

   ```
   Local addresses
   bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p     port  18333    score      4
   ```

## Summary: Bitcoin Core as an I2P (Invisible Internet Project) service

It is always good to have alternatives for privacy and not depend only on Tor to run Bitcoin Core as hidden service. Since i2p was recently added in Bitcoin Core, not many people use it. Experiment with it and report bugs if you find any issues.

> :information_source: **NOTE:** _i2pd_ (C++) is different from _i2prouter_ (Java), you will need `i2pd` for Bitcoin Core.

Move on to "Programming with RPC" with [Chapter Sixteen: Talking to Bitcoind with C](16_0_Talking_to_Bitcoind.md).

Or, if you're not a programmer, you can skip to [Chapter Nineteen: Understanding Your Lightning Seutp](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/19_0_Understanding_Your_Lightning_Setup.md) to continue your command-line education with the Lightning Network.
