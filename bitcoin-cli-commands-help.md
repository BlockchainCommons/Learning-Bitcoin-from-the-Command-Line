
### WORK IN PROGRESS

Goal to capture all the bitcoin-cli help text in a single file, then later annotate with
more links and such.

$ bitcoin-cli help
== Blockchain ==
getbestblockhash
getblock "hash" ( verbose )
getblockchaininfo
getblockcount
getblockhash index
getblockheader "hash" ( verbose )
getchaintips
getdifficulty
getmempoolancestors txid (verbose)
getmempooldescendants txid (verbose)
getmempoolentry txid
getmempoolinfo
getrawmempool ( verbose )
gettxout "txid" n ( includemempool )
gettxoutproof ["txid",...] ( blockhash )
gettxoutsetinfo
verifychain ( checklevel numblocks )
verifytxoutproof "proof"

## Control ##

### getinfo ###
getinfo

Returns an object containing various state info.

Result:
```
{
  "version": xxxxx,           (numeric) the server version
  "protocolversion": xxxxx,   (numeric) the protocol version
  "walletversion": xxxxx,     (numeric) the wallet version
  "balance": xxxxxxx,         (numeric) the total bitcoin balance of the wallet
  "blocks": xxxxxx,           (numeric) the current number of blocks processed in the server
  "timeoffset": xxxxx,        (numeric) the time offset
  "connections": xxxxx,       (numeric) the number of connections
  "proxy": "host:port",     (string, optional) the proxy used by the server
  "difficulty": xxxxxx,       (numeric) the current difficulty
  "testnet": true|false,      (boolean) if the server is using testnet or not
  "keypoololdest": xxxxxx,    (numeric) the timestamp (seconds since Unix epoch) of the oldest pre-generated key in the key pool
  "keypoolsize": xxxx,        (numeric) how many new keys are pre-generated
  "unlocked_until": ttt,      (numeric) the timestamp in seconds since epoch (midnight Jan 1 1970 GMT) that the wallet is unlocked for transfers, or 0 if the wallet is locked
  "paytxfee": x.xxxx,         (numeric) the transaction fee set in BTC/kB
  "relayfee": x.xxxx,         (numeric) minimum relay fee for non-free transactions in BTC/kB
  "errors": "..."           (string) any error messages
}
```
Examples:
```
> bitcoin-cli getinfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getinfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

help ( "command" )
stop

== Generating ==
generate numblocks ( maxtries )
generatetoaddress numblocks address (maxtries)

== Mining ==
getblocktemplate ( TemplateRequest )
getmininginfo
getnetworkhashps ( blocks height )
prioritisetransaction <txid> <priority delta> <fee delta>
submitblock "hexdata" ( "jsonparametersobject" )

== Network ==
addnode "node" "add|remove|onetry"
clearbanned
disconnectnode "node"
getaddednodeinfo dummy ( "node" )
getconnectioncount
getnettotals
getnetworkinfo
getpeerinfo
listbanned
ping
setban "ip(/netmask)" "add|remove" (bantime) (absolute)

== Rawtransactions ==
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime )
decoderawtransaction "hexstring"
decodescript "hex"
fundrawtransaction "hexstring" ( options )

### getrawtransaction "txid" ( verbose )

NOTE: By default this function only works sometimes. This is when the tx is in
the mempool or there is an unspent output in the utxo for this transaction. To
make it always work, you need to maintain a transaction index, using the
-txindex command line option.

Return the raw transaction data.

If verbose=0, returns a string that is serialized, hex-encoded data for 'txid'.
If verbose is non-zero, returns an Object with information about 'txid'.

Arguments:

1. "txid"      (string, required) The transaction id
2. verbose       (numeric, optional, default=0) If 0, return a string, other return a json object

Result (if verbose is not set or set to 0):

"data"      (string) The serialized, hex-encoded data for 'txid'

Result (if verbose > 0):

```
{
  "hex" : "data",       (string) The serialized, hex-encoded data for 'txid'
  "txid" : "id",        (string) The transaction id (same as provided)
  "hash" : "id",        (string) The transaction hash (differs from txid for witness transactions)
  "size" : n,             (numeric) The serialized transaction size
  "vsize" : n,            (numeric) The virtual transaction size (differs from size for witness transactions)
  "version" : n,          (numeric) The version
  "locktime" : ttt,       (numeric) The lock time
  "vin" : [               (array of json objects)
     {
       "txid": "id",    (string) The transaction id
       "vout": n,         (numeric)
       "scriptSig": {     (json object) The script
         "asm": "asm",  (string) asm
         "hex": "hex"   (string) hex
       },
       "sequence": n      (numeric) The script sequence number
       "txinwitness": ["hex", ...] (array of string) hex-encoded witness data (if any)
     }
     ,...
  ],
  "vout" : [              (array of json objects)
     {
       "value" : x.xxx,            (numeric) The value in BTC
       "n" : n,                    (numeric) index
       "scriptPubKey" : {          (json object)
         "asm" : "asm",          (string) the asm
         "hex" : "hex",          (string) the hex
         "reqSigs" : n,            (numeric) The required sigs
         "type" : "pubkeyhash",  (string) The type, eg 'pubkeyhash'
         "addresses" : [           (json array of string)
           "bitcoinaddress"        (string) bitcoin address
           ,...
         ]
       }
     }
     ,...
  ],
  "blockhash" : "hash",   (string) the block hash
  "confirmations" : n,      (numeric) The confirmations
  "time" : ttt,             (numeric) The transaction time in seconds since epoch (Jan 1 1970 GMT)
  "blocktime" : ttt         (numeric) The block time in seconds since epoch (Jan 1 1970 GMT)
}
```

Examples:
```
> bitcoin-cli getrawtransaction "mytxid"
> bitcoin-cli getrawtransaction "mytxid" 1
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawtransaction", "params": ["mytxid", 1] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

sendrawtransaction "hexstring" ( allowhighfees )
signrawtransaction "hexstring" ( [{"txid":"id","vout":n,"scriptPubKey":"hex","redeemScript":"hex"},...] ["privatekey1",...] sighashtype )

== Util ==
createmultisig nrequired ["key",...]
estimatefee nblocks
estimatepriority nblocks
estimatesmartfee nblocks
estimatesmartpriority nblocks
signmessagewithprivkey "privkey" "message"
validateaddress "bitcoinaddress"
verifymessage "bitcoinaddress" "signature" "message"

== Wallet ==
abandontransaction "txid"
addmultisigaddress nrequired ["key",...] ( "account" )
addwitnessaddress "address"
backupwallet "destination"

### dumpprivkey "bitcoinaddress"

Reveals the private key corresponding to 'bitcoinaddress'.

Then the importprivkey can be used with this output

Arguments:

1. "bitcoinaddress"   (string, required) The bitcoin address for the private key

Result:

"key"                (string) The private key

Examples:
```
> bitcoin-cli dumpprivkey "myaddress"
> bitcoin-cli importprivkey "mykey"
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "dumpprivkey", "params": ["myaddress"] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

dumpwallet "filename"
encryptwallet "passphrase"
getaccount "bitcoinaddress"
getaccountaddress "account"
getaddressesbyaccount "account"
###
getbalance ( "account" minconf includeWatchonly )

If account is not specified, returns the server's total available balance.
If account is specified (DEPRECATED), returns the balance in the account.
Note that the account "" is not the same as leaving the parameter out.
The server total may be different to the balance in the default "" account.

Arguments:
1. "account"      (string, optional) DEPRECATED. The selected account, or "\*" for entire wallet. It may be the default account using "".
2. minconf          (numeric, optional, default=1) Only include transactions confirmed at least this many times.
3. includeWatchonly (bool, optional, default=false) Also include balance in watchonly addresses (see 'importaddress')

Result:
```
amount              (numeric) The total amount in BTC received for this account.
```
Examples:

The total amount in the wallet
```
> bitcoin-cli getbalance
```

The total amount in the wallet at least 5 blocks confirmed
```
> bitcoin-cli getbalance "*" 6
```

As a json rpc call
```
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getbalance", "params": ["*", 6] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```


### getnewaddress ( "account" )

Returns a new Bitcoin address for receiving payments.

If 'account' is specified (DEPRECATED), it is added to the address book
so payments received with the address will be credited to 'account'.

Arguments:

1. "account"        (string, optional) DEPRECATED. The account name for the
address to be linked to. If not provided, the default account "" is used. It
can also be set to the empty string "" to represent the default account. The
account does not need to exist, it will be created if there is no account by
the given name.

Result:

"bitcoinaddress"    (string) The new bitcoin address

Examples:
```
> bitcoin-cli getnewaddress
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getnewaddress", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

getrawchangeaddress
getreceivedbyaccount "account" ( minconf )
getreceivedbyaddress "bitcoinaddress" ( minconf )

## gettransaction "txid" ( includeWatchonly )

Get detailed information about in-wallet transaction <txid>

Arguments:

1. "txid"    (string, required) The transaction id
2. "includeWatchonly"    (bool, optional, default=false) Whether to include watchonly addresses in balance calculation and details[]

Result:
```
{
  "amount" : x.xxx,        (numeric) The transaction amount in BTC
  "confirmations" : n,     (numeric) The number of confirmations
  "blockhash" : "hash",  (string) The block hash
  "blockindex" : xx,       (numeric) The index of the transaction in the block that includes it
  "blocktime" : ttt,       (numeric) The time in seconds since epoch (1 Jan 1970 GMT)
  "txid" : "transactionid",   (string) The transaction id.
  "time" : ttt,            (numeric) The transaction time in seconds since epoch (1 Jan 1970 GMT)
  "timereceived" : ttt,    (numeric) The time received in seconds since epoch (1 Jan 1970 GMT)
  "bip125-replaceable": "yes|no|unknown"  (string) Whether this transaction could be replaced due to BIP125 (replace-by-fee);
                                                   may be unknown for unconfirmed transactions not in the mempool
  "details" : [
    {
      "account" : "accountname",  (string) DEPRECATED. The account name involved in the transaction, can be "" for the default account.
      "address" : "bitcoinaddress",   (string) The bitcoin address involved in the transaction
      "category" : "send|receive",    (string) The category, either 'send' or 'receive'
      "amount" : x.xxx,                 (numeric) The amount in BTC
      "label" : "label",              (string) A comment for the address/transaction, if any
      "vout" : n,                       (numeric) the vout value
    }
    ,...
  ],
  "hex" : "data"         (string) Raw data for transaction
}
```
Examples:
```
> bitcoin-cli gettransaction "1075db55d416d3ca199f55b6084e2115b9345e16c5cf302fc80e9d5fbf5d48d"
> bitcoin-cli gettransaction "1075db55d416d3ca199f55b6084e2115b9345e16c5cf302fc80e9d5fbf5d48d" true
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "gettransaction", "params": ["1075db55d416d3ca199f55b6084e2115b9345e16c5cf302fc80e9d5fbf5d48d"] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
getunconfirmedbalance

## getwalletinfo
Returns an object containing various wallet state info.

Result:
```
{
  "walletversion": xxxxx,       (numeric) the wallet version
  "balance": xxxxxxx,           (numeric) the total confirmed balance of the wallet in BTC
  "unconfirmed_balance": xxx,   (numeric) the total unconfirmed balance of the wallet in BTC
  "immature_balance": xxxxxx,   (numeric) the total immature balance of the wallet in BTC
  "txcount": xxxxxxx,           (numeric) the total number of transactions in the wallet
  "keypoololdest": xxxxxx,      (numeric) the timestamp (seconds since Unix epoch) of the oldest pre-generated key in the key pool
  "keypoolsize": xxxx,          (numeric) how many new keys are pre-generated
  "unlocked_until": ttt,        (numeric) the timestamp in seconds since epoch (midnight Jan 1 1970 GMT) that the wallet is unlocked for transfers, or 0 if the wallet is locked
  "paytxfee": x.xxxx,           (numeric) the transaction fee configuration, set in BTC/kB
  "hdmasterkeyid": "<hash160>", (string) the Hash160 of the HD master pubkey
}
```

Examples:
```
> bitcoin-cli getwalletinfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getwalletinfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

importaddress "address" ( "label" rescan p2sh )
importprivkey "bitcoinprivkey" ( "label" rescan )
importprunedfunds
importpubkey "pubkey" ( "label" rescan )
importwallet "filename"
keypoolrefill ( newsize )
listaccounts ( minconf includeWatchonly)
listaddressgroupings
listlockunspent
listreceivedbyaccount ( minconf includeempty includeWatchonly)
listreceivedbyaddress ( minconf includeempty includeWatchonly)
listsinceblock ( "blockhash" target-confirmations includeWatchonly)
listtransactions ( "account" count from includeWatchonly)

### listunspent ( minconf maxconf  ["address",...] )

Returns array of unspent transaction outputs
with between minconf and maxconf (inclusive) confirmations.
Optionally filter to only include txouts paid to specified addresses.

Arguments:

1. minconf          (numeric, optional, default=1) The minimum confirmations to filter
2. maxconf          (numeric, optional, default=9999999) The maximum confirmations to filter
3. "addresses"    (string) A json array of bitcoin addresses to filter
```
    [
      "address"   (string) bitcoin address
      ,...
    ]
```

Result:
```
[                   (array of json object)
  {
    "txid" : "txid",          (string) the transaction id
    "vout" : n,               (numeric) the vout value
    "address" : "address",    (string) the bitcoin address
    "account" : "account",    (string) DEPRECATED. The associated account, or "" for the default account
    "scriptPubKey" : "key",   (string) the script key
    "amount" : x.xxx,         (numeric) the transaction amount in BTC
    "confirmations" : n,      (numeric) The number of confirmations
    "redeemScript" : n        (string) The redeemScript if scriptPubKey is P2SH
    "spendable" : xxx,        (bool) Whether we have the private keys to spend this output
    "solvable" : xxx          (bool) Whether we know how to spend this output, ignoring the lack of keys
  }
  ,...
]
```

Examples:
```
> bitcoin-cli listunspent
> bitcoin-cli listunspent 6 9999999 "[\"1PGFqEzfmQch1gKD3ra4k18PNj3tTUUSqg\",\"1LtvqCaApEdUGFkpKMM4MstjcaL4dKg8SP\"]"
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [6, 9999999 "[\"1PGFqEzfmQch1gKD3ra4k18PNj3tTUUSqg\",\"1LtvqCaApEdUGFkpKMM4MstjcaL4dKg8SP\"]"] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```

lockunspent unlock ([{"txid":"txid","vout":n},...])
move "fromaccount" "toaccount" amount ( minconf "comment" )
removeprunedfunds "txid"
sendfrom "fromaccount" "tobitcoinaddress" amount ( minconf "comment" "comment-to" )
sendmany "fromaccount" {"address":amount,...} ( minconf "comment" ["address",...] )
sendtoaddress "bitcoinaddress" amount ( "comment" "comment-to" subtractfeefromamount )
setaccount "bitcoinaddress" "account"
settxfee amount
signmessage "bitcoinaddress" "message"




=====




-?
Print this help message and exit
-version
Print version and exit
-alertnotify=<cmd>
Execute command when a relevant alert is received or we see a really long fork (%s in cmd is replaced by message)
-blocknotify=<cmd>
Execute command when the best block changes (%s in cmd is replaced by block hash)
-checkblocks=<n>
How many blocks to check at startup (default: 6, 0 = all)
-checklevel=<n>
How thorough the block verification of -checkblocks is (0-4, default: 3)
-conf=<file>
Specify configuration file (default: bitcoin.conf)
-datadir=<dir>
Specify data directory
-dbcache=<n>
Set database cache size in megabytes (4 to 16384, default: 300)
-loadblock=<file>
Imports blocks from external blk000??.dat file on startup
-maxorphantx=<n>
Keep at most <n> unconnectable transactions in memory (default: 100)
-maxmempool=<n>
Keep the transaction memory pool below <n> megabytes (default: 300)
-mempoolexpiry=<n>
Do not keep transactions in the mempool longer than <n> hours (default: 72)
-par=<n>
Set the number of script verification threads (-4 to 16, 0 = auto, <0 = leave that many cores free, default: 0)
-pid=<file>
Specify pid file (default: bitcoind.pid)
-prune=<n>
Reduce storage requirements by pruning (deleting) old blocks. This mode is incompatible with -txindex and -rescan. Warning: Reverting this setting requires re-downloading the entire blockchain. (default: 0 = disable pruning blocks, >550 = target size in MiB to use for block files)
-reindex-chainstate
Rebuild chain state from the currently indexed blocks
-reindex
Rebuild chain state and block index from the blk*.dat files on disk
-sysperms
Create new files with system default permissions, instead of umask 077 (only effective with disabled wallet functionality)
-txindex
Maintain a full transaction index, used by the getrawtransaction rpc call (default: 0)


Connection options:


-addnode=<ip>
Add a node to connect to and attempt to keep the connection open
-banscore=<n>
Threshold for disconnecting misbehaving peers (default: 100)
-bantime=<n>
Number of seconds to keep misbehaving peers from reconnecting (default: 86400)
-bind=<addr>
Bind to given address and always listen on it. Use [host]:port notation for IPv6
-connect=<ip>
Connect only to the specified node(s)
-discover
Discover own IP addresses (default: 1 when listening and no -externalip or -proxy)
-dns
Allow DNS lookups for -addnode, -seednode and -connect (default: 1)
-dnsseed
Query for peer addresses via DNS lookup, if low on addresses (default: 1 unless -connect)
-externalip=<ip>
Specify your own public address
-forcednsseed
Always query for peer addresses via DNS lookup (default: 0)
-listen
Accept connections from outside (default: 1 if no -proxy or -connect)
-listenonion
Automatically create Tor hidden service (default: 1)
-maxconnections=<n>
Maintain at most <n> connections to peers (default: 125)
-maxreceivebuffer=<n>
Maximum per-connection receive buffer, <n>\*1000 bytes (default: 5000)
-maxsendbuffer=<n>
Maximum per-connection send buffer, <n>\*1000 bytes (default: 1000)
-maxtimeadjustment
Maximum allowed median peer time offset adjustment. Local perspective of time may be influenced by peers forward or backward by this amount. (default: 4200 seconds)
-onion=<ip:port>
Use separate SOCKS5 proxy to reach peers via Tor hidden services (default: -proxy)
-onlynet=<net>
Only connect to nodes in network <net> (ipv4, ipv6 or onion)
-permitbaremultisig
Relay non-P2SH multisig (default: 1)
-peerbloomfilters
Support filtering of blocks and transaction with bloom filters (default: 1)
-port=<port>
Listen for connections on <port> (default: 8333 or testnet: 18333)
-proxy=<ip:port>
Connect through SOCKS5 proxy
-proxyrandomize
Randomize credentials for every proxy connection. This enables Tor stream isolation (default: 1)
-rpcserialversion
Sets the serialization of raw transaction or block hex returned in non-verbose mode, non-segwit(0) or segwit(1) (default: 1)
-seednode=<ip>
Connect to a node to retrieve peer addresses, and disconnect
-timeout=<n>
Specify connection timeout in milliseconds (minimum: 1, default: 5000)
-torcontrol=<ip>:<port>
Tor control port to use if onion listening enabled (default: 127.0.0.1:9051)
-torpassword=<pass>
Tor control port password (default: empty)
-upnp
Use UPnP to map the listening port (default: 0)
-whitebind=<addr>
Bind to given address and whitelist peers connecting to it. Use [host]:port notation for IPv6
-whitelist=<IP address or network>
Whitelist peers connecting from the given IP address (e.g. 1.2.3.4) or CIDR notated network (e.g. 1.2.3.0/24). Can be specified multiple times. Whitelisted peers cannot be DoS banned and their transactions are always relayed, even if they are already in the mempool, useful e.g. for a gateway
-whitelistrelay
Accept relayed transactions received from whitelisted peers even when not relaying transactions (default: 1)
-whitelistforcerelay
Force relay of transactions from whitelisted peers even if they violate local relay policy (default: 1)
-maxuploadtarget=<n>
Tries to keep outbound traffic under the given target (in MiB per 24h), 0 = no limit (default: 0)


Wallet options:


-disablewallet
Do not load the wallet and disable wallet RPC calls
-keypool=<n>
Set key pool size to <n> (default: 100)
-fallbackfee=<amt>
A fee rate (in BTC/kB) that will be used when fee estimation has insufficient data (default: 0.0002)
-mintxfee=<amt>
Fees (in BTC/kB) smaller than this are considered zero fee for transaction creation (default: 0.00001)
-paytxfee=<amt>
Fee (in BTC/kB) to add to transactions you send (default: 0.00)
-rescan
Rescan the block chain for missing wallet transactions on startup
-salvagewallet
Attempt to recover private keys from a corrupt wallet on startup
-spendzeroconfchange
Spend unconfirmed change when sending transactions (default: 1)
-txconfirmtarget=<n>
If paytxfee is not set, include enough fee so transactions begin confirmation on average within n blocks (default: 2)
-usehd
Use hierarchical deterministic key generation (HD) after BIP32. Only has effect during wallet creation/first start (default: 1)
-upgradewallet
Upgrade wallet to latest format on startup
-wallet=<file>
Specify wallet file (within data directory) (default: wallet.dat)
-walletbroadcast
Make the wallet broadcast transactions (default: 1)
-walletnotify=<cmd>
Execute command when a wallet transaction changes (%s in cmd is replaced by TxID)
-zapwallettxes=<mode>
Delete all wallet transactions and only recover those parts of the blockchain through -rescan on startup (1 = keep tx meta data e.g. account owner and payment request information, 2 = drop tx meta data)


ZeroMQ notification options:


-zmqpubhashblock=<address>
Enable publish hash block in <address>
-zmqpubhashtx=<address>
Enable publish hash transaction in <address>
-zmqpubrawblock=<address>
Enable publish raw block in <address>
-zmqpubrawtx=<address>
Enable publish raw transaction in <address>


Debugging/Testing options:


-uacomment=<cmt>
Append comment to the user agent string
-debug=<category>
Output debugging information (default: 0, supplying <category> is optional). If <category> is not supplied or if <category> = 1, output all debugging information.<category> can be: addrman, alert, bench, cmpctblock, coindb, db, http, libevent, lock, mempool, mempoolrej, net, proxy, prune, rand, reindex, rpc, selectcoins, tor, zmq, qt.
-help-debug
Show all debugging options (usage: --help -help-debug)
-logips
Include IP addresses in debug output (default: 0)
-logtimestamps
Prepend debug output with timestamp (default: 1)
-minrelaytxfee=<amt>
Fees (in BTC/kB) smaller than this are considered zero fee for relaying, mining and transaction creation (default: 0.00001)
-maxtxfee=<amt>
Maximum total fees (in BTC) to use in a single wallet transaction or raw transaction; setting this too low may abort large transactions (default: 0.10)
-printtoconsole
Send trace/debug info to console instead of debug.log file
-shrinkdebugfile
Shrink debug.log file on client startup (default: 1 when no -debug)


Chain selection options:


-testnet
Use the test chain


Node relay options:


-bytespersigop
Equivalent bytes per sigop in transactions for relay and mining (default: 20)
-datacarrier
Relay and mine data carrier transactions (default: 1)
-datacarriersize
Maximum size of data in data carrier transactions we relay and mine (default: 83)
-mempoolreplacement
Enable transaction replacement in the memory pool (default: 1)


Block creation options:


-blockmaxweight=<n>
Set maximum BIP141 block weight (default: 3000000)
-blockmaxsize=<n>
Set maximum block size in bytes (default: 750000)
-blockprioritysize=<n>
Set maximum size of high-priority/low-fee transactions in bytes (default: 0)


RPC server options:


-server
Accept command line and JSON-RPC commands
-rest
Accept public REST requests (default: 0)
-rpcbind=<addr>
Bind to given address to listen for JSON-RPC connections. Use [host]:port notation for IPv6. This option can be specified multiple times (default: bind to all interfaces)
-rpccookiefile=<loc>
Location of the auth cookie (default: data dir)
-rpcuser=<user>
Username for JSON-RPC connections
-rpcpassword=<pw>
Password for JSON-RPC connections
-rpcauth=<userpw>
Username and hashed password for JSON-RPC connections. The field <userpw> comes in the format: <USERNAME>:<SALT>$<HASH>. A canonical python script is included in share/rpcuser. This option can be specified multiple times
-rpcport=<port>
Listen for JSON-RPC connections on <port> (default: 8332 or testnet: 18332)
-rpcallowip=<ip>
Allow JSON-RPC connections from specified source. Valid for <ip> are a single IP (e.g. 1.2.3.4), a network/netmask (e.g. 1.2.3.4/255.255.255.0) or a network/CIDR (e.g. 1.2.3.4/24). This option can be specified multiple times
-rpcthreads=<n>
Set the number of threads to service RPC calls (default: 4)


UI Options:


-choosedatadir
Choose data directory on startup (default: 0)
-lang=<lang>
Set language, for example "de_DE" (default: system locale)
-min
Start minimized
-rootcertificates=<file>
Set SSL root certificates for payment request (default: -system-)
-splash
Show splash screen on startup (default: 1)
-resetguisettings
Reset all settings changed in the GUI
