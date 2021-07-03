# Interlude: Accessing a Second Lightning Node

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

When you played with Bitcoin you were accessing an existing network, and that made it relatively easy to work with: you just turned on `bitcoind` and you were immediately interacting with the network. That's now how Lightning works: it's fundamentally a peer-to-peer network, built up from the connections between any two individual nodes. In other words, to interact with the Lightning Network, you'll need to first find a node to connect to.

There are four ways to do so (the first three of which are possible for your first connection):

## Ask for Information on a Node

If someone else already has a Lightning node on the network of your choice, just ask them for their ID. 

If they are are running c-lightning, they just need to use the `getinfo` command:
```
$ lightning-cli getinfo
lightning-cli: WARNING: default network changing in 2020: please set network=testnet in config!
   "id": "03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687",
   "alias": "VIOLETGLEE",
   "color": "03240a",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      }
   ],
   "binding": [
      {
         "type": "ipv6",
         "address": "::",
         "port": 9735
      },
      {
         "type": "ipv4",
         "address": "0.0.0.0",
         "port": 9735
      }
   ],
   "version": "v0.9.1-96-g6f870df",
   "blockheight": 1862854,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```
They can then tell you their `id` (`03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687`). They will also need to tell you their IP address (`74.207.240.32`) and port (`9735`).

## Create a New c-lightning Node

However, for testing purposes, you probably want to have a second node under you own control. The easiest way to do so is to create a second c-lightning node on a new machine, using either Bitcoin Standup, per [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) or compiling it by hand, per [§19.1](19_1_Verifying_Your_Lightning_Setup.md).

Once you have your node running, you can run `getinfo` to retrieve your information, as shown above.

## Create a New LND Node

However, for our examples in the next chapter, we're instead going to create an LND node. This will allow us to demonstrate a bit of the depth of the Lightning ecosystem by showing how similar commands work on the two different platforms.

One way to create an LND node is to run the Bitcoin Standup Scripts again on a new machine, but this time to choose LND, per [§2.1](2_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

Another is to compile LND from source code on a machine where you'rea already running a Bitcoin node, as follows.

### Compile the LND Source Code 

First, you need to download and install Go:
```
$ wget --progress=bar:force https://dl.google.com/go/"go1.14.4"."linux"-"amd64".tar.gz -O ~standup/"go1.14.4"."linux"-"amd64".tar.gz
$ /bin/tar xzf ~standup/"go1.14.4"."linux"-"amd64".tar.gz -C ~standup
$ sudo mv ~standup/go /usr/local
```
Be sure that the Go version is the most up to date (it's `go1.14.4` at the current time), and the platform and architecture are right for your machine. (The above will work for Debian.)

Update your path:
```
$ export GOPATH=~standup/gocode
$ export PATH="$PATH":/usr/local/go/bin:"$GOPATH"/bin
```
Then be sure that `go` works:
```
$ go version
go version go1.14.4 linux/amd64
```
You'll also need `git` and `make`:
```
$ sudo apt-get install git
$ sudo apt-get install build-essential
```
You're now ready to retrieve LND. Be sure to get the current verison (currently `v0.11.0-beta.rc4`).
```
$ go get -d github.com/lightningnetwork/lnd
```
And now you can compile:
```
$ cd "$GOPATH"/src/github.com/lightningnetwork/lnd
$ git checkout v0.11.0-beta.rc4
$ make
$ make install
```
This will install to `~/gocode/bin`, which is `$GOPATH/bin`.

You should move it to global directories:
```
$ sudo cp $GOPATH/bin/lnd $GOPATH/bin/lncli /usr/bin
```

### Create an LND Config File

Unlike with c-lightning, you will need to create a default config file for LND.

However first, you need to enable ZMQ on your Bitcoind, if you didn't already in [§16.3](16_3_Receiving_Bitcoind_Notifications_with_C.md).

This requires adding the following to your `~/.bitcoin/bitcoin.conf` file if it's not already there:
```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```
If you're using a Bitcoin config file from Standup or some other specialized `conf`, be sure you're putting your new commands in the correct section. Ideally, they should go near the top of the file, otherwise in the `[test]` section (assuming, as usual, that you're testing on testnet).

You must then restart bitcoin (or just reboot your machine). You can test that it's working as follows:
```
$ bitcoin-cli getzmqnotifications
[
  {
    "type": "pubrawblock",
    "address": "tcp://127.0.0.1:28332",
    "hwm": 1000
  },
  {
    "type": "pubrawtx",
    "address": "tcp://127.0.0.1:28333",
    "hwm": 1000
  }
]
```
Now you're ready to create a config file.

First, you need to retrieve your rpcuser and rpcpassword. Here's an automated way to do so:
```
$ BITCOINRPC_USER=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcuser | awk -F = '{print $2}')
$ BITCOINRPC_PASS=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcpassword | awk -F = '{print $2}')
```

> :warning: **WARNING:** Obviously, never store your RPC password in a shell variable in a production environment.

Then, you can write the file:
```
$ mkdir ~/.lnd
$ cat > ~/.lnd/lnd.conf << EOF
[Application Options]
maxlogfiles=3
maxlogfilesize=10
#externalip=1.1.1.1 # change to your public IP address if required.
alias=StandUp
listen=0.0.0.0:9735
debuglevel=debug
[Bitcoin]
bitcoin.active=1
bitcoin.node=bitcoind
bitcoin.testnet=true
[Bitcoind]
bitcoind.rpchost=localhost
bitcoind.rpcuser=$BITCOINRPC_USER
bitcoind.rpcpass=$BITCOINRPC_PASS
bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332
bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333
EOF
```

### Create an LND Service

Finally, you can create an LND service to automatically run `lnd`:
```
$ cat > ~/lnd.service << EOF
# It is not recommended to modify this file in-place, because it will
# be overwritten during package upgrades. If you want to add further
# options or overwrite existing ones then use
# $ systemctl edit lnd.service
# See "man systemd.service" for details.
# Note that almost all daemon options could be specified in
# /etc/lnd/lnd.conf, except for those explicitly specified as arguments
# in ExecStart=
[Unit]
Description=LND Lightning Network Daemon
Requires=bitcoind.service
After=bitcoind.service
[Service]
ExecStart=/usr/bin/lnd
ExecStop=/usr/bin/lncli --lnddir /var/lib/lnd stop
PIDFile=/run/lnd/lnd.pid
User=standup
Type=simple
KillMode=process
TimeoutStartSec=60
TimeoutStopSec=60
Restart=always
RestartSec=60
[Install]
WantedBy=multi-user.target
EOF
```
You'll then need to install that and start things up:
```
$ sudo cp ~/lnd.service /etc/systemd/system
$ sudo systemctl enable lnd
$ sudo systemctl start lnd
```
(Expect this to take a minute the first time.)

### Enable Remote Connections

Just as with c-lightning, you're going to need to make LND accessible to other nodes. Here's how to do so if you use `ufw`, as per the Bitcoin Standup setups:
```
$ sudo ufw allow 9735
```

### Create a Wallet

The first time you run LND, you must create a wallet:
```
$ lncli --network=testnet create
```
LND will ask you for a password and then ask if you want to enter an existing mnemonic (just hit `n` for the latter one).

You should now have a functioning `lnd`, which you can verify with `getinfo`:
```
$ lncli --network=testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 0,
    "num_inactive_channels": 0,
    "num_peers": 2,
    "block_height": 1862848,
    "block_hash": "000000000000000ecb6fd95e1f486283d48683aa3111b6c23144a2056f5a1532",
    "best_header_timestamp": "1602632294",
    "synced_to_chain": true,
    "synced_to_graph": false,
    "testnet": true,
    "chains": [
        {
            "chain": "bitcoin",
            "network": "testnet"
        }
    ],
    "uris": [
    ],
    "features": {
        "0": {
            "name": "data-loss-protect",
            "is_required": true,
            "is_known": true
        },
        "5": {
            "name": "upfront-shutdown-script",
            "is_required": false,
            "is_known": true
        },
        "7": {
            "name": "gossip-queries",
            "is_required": false,
            "is_known": true
        },
        "9": {
            "name": "tlv-onion",
            "is_required": false,
            "is_known": true
        },
        "13": {
            "name": "static-remote-key",
            "is_required": false,
            "is_known": true
        },
        "15": {
            "name": "payment-addr",
            "is_required": false,
            "is_known": true
        },
        "17": {
            "name": "multi-path-payments",
            "is_required": false,
            "is_known": true
        }
    }
}
```
This node's ID is `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`. Although this command doesn't show you the IP address and port, they should be the IP address for your machine and port `9735`. 

## Listen to Gossip

If you were already connected to the Lightning Network, and were "gossipping" with peers, you might also be able to find information on peers automatically, through the `listpeers` command:
```       
c$ lightning-cli --network=testnet listpeers
{
   "peers": [
      {
         "id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
         "connected": true,
         "netaddr": [
            "127.0.0.1:9736"
         ],
         "features": "02a2a1",
         "channels": []
      }
   ]
}
```       
However, that definitely won't be the case for your first interaction with the Lightning Network.

## Summary: Accessing a Second Lightning Node

You always need two Lightning nodes to form a channel. If you don't have someone else who is testing things out with you, you're going to need to create a second one, either using c-lightning or (as we will in our examples) LND.

## What's Next?

Though you've possibly created an LND, c-lightning will remain the heart of our examples until we need to start using both of them, in [Chapter 19](19_0_Understanding_Your_Lightning_Setup.md).

Continue "Understanding Your Lightning Setup" with [§19.3: Setting Up_a_Channel](19_3_Setting_Up_a_Channel.md).
