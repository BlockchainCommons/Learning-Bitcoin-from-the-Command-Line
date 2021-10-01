# 19.2: Conocer la configuración de su nodo c-lightning

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

Antes de comenzar a acceder a Lightning Network, debe comprender mejor su configuración.

## Conozca su directorio de c-lightning

Cuando se usa c-lightning, todo se guarda en el directorio `~/.lightning`.

El directorio principal solo contiene directorios para las redes que estén configuradas, en este caso testnet:
```
$ ls ~/.lightning
testnet
```

El directorio `~/.lightning/testnet` contendrá todas las entrañas de su configuración:
```
$ ls ~/.lightning/testnet3
config  gossip_store  hsm_secret  lightningd.sqlite3  lightningd.sqlite3-journal  lightning-rpc
```

> :link: **TESTNET vs MAINNET:** Si está utilizando mainnet, entonces _todo_ se colocará en el directorio `~/.lightning/bitcoin`. Estas diversas configuraciones se hacen elegantemente, por lo que si usted está usando MainNet, testnet y Regtest, usted encontrará que `~/.lightning/bitcoin` contiene el archivo de configuración y sus datos MainNet, el directorio `~/.lightning/testnet` contiene los datos Testnet, y el directorio `~/.lightning/regtest` contiene sus datos regtest.

## Conozca sus comandos lightning-cli

La mayor parte de su trabajo inicial se realizará con el comando `lightning-cli`, que ofrece una interfaz fácil contra `lightningd`, tal como lo hace `bitcoin-cli`.

Ya ha visto que el `help` comando le dará una lista de otros comandos:
```
$ lightning-cli help
lightning-cli: WARNING: default network changing in 2020: please set network=testnet in config!
=== bitcoin ===

feerates style
    Return feerate estimates, either satoshi-per-kw ({style} perkw) or satoshi-per-kb ({style} perkb).

newaddr [addresstype]
    Get a new {bech32, p2sh-segwit} (or all) address to fund a channel (default is bech32)

reserveinputs outputs [feerate] [minconf] [utxos]
    Reserve inputs and pass back the resulting psbt

sendpsbt psbt
    Finalize, extract and send a PSBT.

signpsbt psbt
    Sign this wallet's inputs on a provided PSBT.

txdiscard txid
    Abandon a transaction created by txprepare

txprepare outputs [feerate] [minconf] [utxos]
    Create a transaction, with option to spend in future (either txsend and txdiscard)

txsend txid
    Sign and broadcast a transaction created by txprepare

unreserveinputs psbt
    Unreserve inputs, freeing them up to be reused

withdraw destination satoshi [feerate] [minconf] [utxos]
    Send to {destination} address {satoshi} (or 'all') amount via Bitcoin transaction, at optional {feerate}

=== channels ===

close id [unilateraltimeout] [destination] [fee_negotiation_step]
    Close the channel with {id} (either peer ID, channel ID, or short channel ID). Force a unilateral close after {unilateraltimeout} seconds (default 48h). If {destination} address is provided, will be used as output address.

fundchannel_cancel id
    Cancel inflight channel establishment with peer {id}.

fundchannel_complete id txid txout
    Complete channel establishment with peer {id} for funding transactionwith {txid}. Returns true on success, false otherwise.

fundchannel_start id amount [feerate] [announce] [close_to] [push_msat]
    Start fund channel with {id} using {amount} satoshis. Returns a bech32 address to use as an output for a funding transaction.

getroute id msatoshi riskfactor [cltv] [fromid] [fuzzpercent] [exclude] [maxhops]
    Show route to {id} for {msatoshi}, using {riskfactor} and optional {cltv} (default 9). If specified search from {fromid} otherwise use this node as source. Randomize the route with up to {fuzzpercent} (default 5.0). {exclude} an array of short-channel-id/direction (e.g. [ '564334x877x1/0', '564195x1292x0/1' ]) or node-id from consideration. Set the {maxhops} the route can take (default 20).

listchannels [short_channel_id] [source]
    Show channel {short_channel_id} or {source} (or all known channels, if not specified)

listforwards 
    List all forwarded payments and their information

setchannelfee id [base] [ppm]
    Sets specific routing fees for channel with {id} (either peer ID, channel ID, short channel ID or 'all'). Routing fees are defined by a fixed {base} (msat) and a {ppm} (proportional per millionth) value. If values for {base} or {ppm} are left out, defaults will be used. {base} can also be defined in other units, for example '1sat'. If {id} is 'all', the fees will be applied for all channels. 

=== network ===

connect id [host] [port]
    Connect to {id} at {host} (which can end in ':port' if not default). {id} can also be of the form id@host

disconnect id [force]
    Disconnect from {id} that has previously been connected to using connect; with {force} set, even if it has a current channel

listnodes [id]
    Show node {id} (or all, if no {id}), in our local network view

listpeers [id] [level]
    Show current peers, if {level} is set, include logs for {id}

ping id [len] [pongbytes]
    Send peer {id} a ping of length {len} (default 128) asking for {pongbytes} (default 128)

=== payment ===

createonion hops assocdata [session_key]
    Create an onion going through the provided nodes, each with its own payload

decodepay bolt11 [description]
    Decode {bolt11}, using {description} if necessary

delexpiredinvoice [maxexpirytime]
    Delete all expired invoices that expired as of given {maxexpirytime} (a UNIX epoch time), or all expired invoices if not specified

delinvoice label status
    Delete unpaid invoice {label} with {status}

invoice msatoshi label description [expiry] [fallbacks] [preimage] [exposeprivatechannels]
    Create an invoice for {msatoshi} with {label} and {description} with optional {expiry} seconds (default 1 week), optional {fallbacks} address list(default empty list) and optional {preimage} (default autogenerated)

listinvoices [label]
    Show invoice {label} (or all, if no {label})

listsendpays [bolt11] [payment_hash]
    Show sendpay, old and current, optionally limiting to {bolt11} or {payment_hash}.

listtransactions 
    List transactions that we stored in the wallet

sendonion onion first_hop payment_hash [label] [shared_secrets] [partid]
    Send a payment with a pre-computed onion.

sendpay route payment_hash [label] [msatoshi] [bolt11] [payment_secret] [partid]
    Send along {route} in return for preimage of {payment_hash}

waitanyinvoice [lastpay_index] [timeout]
    Wait for the next invoice to be paid, after {lastpay_index} (if supplied).  If {timeout} seconds is reached while waiting, fail with an error.

waitinvoice label
    Wait for an incoming payment matching the invoice with {label}, or if the invoice expires

waitsendpay payment_hash [timeout] [partid]
    Wait for payment attempt on {payment_hash} to succeed or fail, but only up to {timeout} seconds.

=== plugin ===

autocleaninvoice [cycle_seconds] [expired_by]
    Set up autoclean of expired invoices. 

estimatefees 
    Get the urgent, normal and slow Bitcoin feerates as sat/kVB.

fundchannel id amount [feerate] [announce] [minconf] [utxos] [push_msat]
    Fund channel with {id} using {amount} (or 'all'), at optional {feerate}. Only use outputs that have {minconf} confirmations.

getchaininfo 
    Get the chain id, the header count, the block count, and whether this is IBD.

getrawblockbyheight height
    Get the bitcoin block at a given height

getutxout txid vout
    Get informations about an output, identified by a {txid} an a {vout}

listpays [bolt11]
    List result of payment {bolt11}, or all

pay bolt11 [msatoshi] [label] [riskfactor] [maxfeepercent] [retry_for] [maxdelay] [exemptfee]
    Send payment specified by {bolt11} with {amount}

paystatus [bolt11]
    Detail status of attempts to pay {bolt11}, or all

plugin subcommand=start|stop|startdir|rescan|list
    Control plugins (start, stop, startdir, rescan, list)

sendrawtransaction tx
    Send a raw transaction to the Bitcoin network.

=== utility ===

check command_to_check
    Don't run {command_to_check}, just verify parameters.

checkmessage message zbase [pubkey]
    Verify a digital signature {zbase} of {message} signed with {pubkey}

getinfo 
    Show information about this node

getlog [level]
    Show logs, with optional log {level} (info|unusual|debug|io)

getsharedsecret point
    Compute the hash of the Elliptic Curve Diffie Hellman shared secret point from this node private key and an input {point}.

help [command]
    List available commands, or give verbose help on one {command}.

listconfigs [config]
    List all configuration options, or with [config], just that one.

listfunds 
    Show available funds from the internal wallet

signmessage message
    Create a digital signature of {message}

stop 
    Shut down the lightningd process

waitblockheight blockheight [timeout]
    Wait for the blockchain to reach {blockheight}, up to {timeout} seconds.

=== developer ===

dev-listaddrs [bip32_max_index]
    Show addresses list up to derivation {index} (default is the last bip32 index)

dev-rescan-outputs 
    Synchronize the state of our funds with bitcoind

---
run `lightning-cli help <command>` for more information on a specific command
```

## Conozca la información de lightning

Una variedad de comandos `lightning-cli` pueden brindarle información adicional sobre su nodo Lightning. Los más generales son:
```
$ lightning-cli --testnet listconfigs
$ lightning-cli --testnet listfunds
$ lightning-cli --testnet listtransactions
$ lightning-cli --testnet listinvoices
$ lightning-cli --testnet listnodes   
```
* listconfigs:  El comando RPC `listconfigs` enumera todas las opciones de configuración.
* listfunds: El comando RPC `listfunds` muestra todos los fondos disponibles, ya sea en salidas no gastadas (UTXO) en la billetera interna o fondos bloqueados en los canales abiertos actualmente.
* listtransactions: El comando RPC `listtransactions` devuelve transacciones rastreadas en la billetera. Esto incluye depósitos, retiros y transacciones relacionadas con los canales.
* listinvoices: El comando RPC `listinvoices` recupera el estado de una factura específica, si existe, o el estado de todas las facturas si no se proporciona ningún argumento.
* listnodes: El comando RPC `listnodes` devuelve los nodos que su servidor ha aprendido a través de mensajes de gossip, o uno solo si se especificó el ID de nodo.

Por ejemplo `lightning-cli listconfigs` le brinda una variedad de información sobre su configuración:
```
c$ lightning-cli --testnet listconfigs
{
   "# version": "v0.8.2-398-g869fa08",
   "lightning-dir": "/home/standup/.lightning",
   "network": "testnet",
   "allow-deprecated-apis": true,
   "rpc-file": "lightning-rpc",
   "plugin": "/usr/local/bin/../libexec/c-lightning/plugins/fundchannel",
   "plugin": "/usr/local/bin/../libexec/c-lightning/plugins/autoclean",
   "plugin": "/usr/local/bin/../libexec/c-lightning/plugins/bcli",
   "plugin": "/usr/local/bin/../libexec/c-lightning/plugins/pay",
   "plugin": "/usr/local/bin/../libexec/c-lightning/plugins/keysend",
   "plugins": [
      {
         "path": "/usr/local/bin/../libexec/c-lightning/plugins/fundchannel",
         "name": "fundchannel"
      },
      {
         "path": "/usr/local/bin/../libexec/c-lightning/plugins/autoclean",
         "name": "autoclean",
         "options": {
            "autocleaninvoice-cycle": null,
            "autocleaninvoice-expired-by": null
         }
      },
      {
         "path": "/usr/local/bin/../libexec/c-lightning/plugins/bcli",
         "name": "bcli",
         "options": {
            "bitcoin-datadir": null,
            "bitcoin-cli": null,
            "bitcoin-rpcuser": null,
            "bitcoin-rpcpassword": null,
            "bitcoin-rpcconnect": null,
            "bitcoin-rpcport": null,
            "bitcoin-retry-timeout": null,
            "commit-fee": "500"
         }
      },
      {
         "path": "/usr/local/bin/../libexec/c-lightning/plugins/pay",
         "name": "pay"
      },
      {
         "path": "/usr/local/bin/../libexec/c-lightning/plugins/keysend",
         "name": "keysend"
      }
   ],
   "disable-plugin": [],
   "always-use-proxy": false,
   "daemon": "false",
   "wallet": "sqlite3:///home/user/.lightning/testnet/lightningd.sqlite3",
   "wumbo": false,
   "wumbo": false,
   "rgb": "03fce2",
   "alias": "learningBitcoin",
   "pid-file": "/home/user/.lightning/lightningd-testnet.pid",
   "ignore-fee-limits": false,
   "watchtime-blocks": 144,
   "max-locktime-blocks": 720,
   "funding-confirms": 3,
   "commit-fee-min": 200,
   "commit-fee-max": 2000,
   "cltv-delta": 6,
   "cltv-final": 10,
   "commit-time": 10,
   "fee-base": 1,
   "rescan": 15,
   "fee-per-satoshi": 10,
   "max-concurrent-htlcs": 483,
   "min-capacity-sat": 10000,
   "offline": "false",
   "autolisten": true,
   "disable-dns": "false",
   "enable-autotor-v2-mode": "false",
   "encrypted-hsm": false,
   "rpc-file-mode": "0600",
   "log-level": "DEBUG",
   "log-prefix": "lightningd"
}
```

## Resumen: Conozca la configuración de su nodo lightning

El directorio `~/.lightning` contiene todos sus archivos, mientras que el comando `lightning-cli help` se puede usar con una variedad de comandos de ayuda para obtener más información sobre cómo funcionan su configuración y Lightning Network.

## Que sigue?

Necesitará tener un segundo nodo Linode para probar el pago real de facturas. Si necesita ayuda para configurar uno, lea  [Interludio: Acceso a un segundo nodo Lightning.](19_2_Interludio_Accediendo_a_un_Segundo_Nodo_Lightning.md).

De lo contrario, continúe "Comprensión de la configuración de Lightning" con [§19.3: Configurando un Canal](19_3_Creando_un_Canal_en_Lightning.md).
