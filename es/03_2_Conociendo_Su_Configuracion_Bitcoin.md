# 3.2: Conociendo la confeniguración de Bitcoin

Antes de comenzar a jugar con Bitcoin, siempre desearía tener una mejor compresión de su configuración.

## Conozca su directorio de Bitcoin

Para empezar, debe comprender dónde se guarda todo: el directorio `~/.bitcoin`.

El directorio principal solo contiene su archivo de configuración y el directorio testnet:
```
$ ls ~/.bitcoin
bitcoin.conf  testnet3
```
Las guías de configuración del [Capítulo dos: Creación de un VPS de Bitcoin-Core] presentaron un archivo de configuración estandarizado. [§3.1: Verificación de la configuración de Bitcoin] sugirió cómo cambiarla para admitir configuraciones más avanzadas. Si está interesado en aprender aún más sobre el archivo de configuración, puede consultar el Generador de configuración de [Bitcoin Core de Jameson Lopp](https://jlopp.github.io/bitcoin-core-config-generator/).

Volviendo a su directorio ~ / .bitcoin, encontrará que el directorio testnet3 contiene todas las vísceras:
```
$ ls ~/.bitcoin/testnet3
banlist.dat   blocks	  debug.log	     mempool.dat	peers.dat
bitcoind.pid  chainstate  fee_estimates.dat  onion_private_key	wallets
```
No debería interactuar con la mayoría de estos archivos y directorios, particularmente con los directorios `blocks` y `chainstate`, que contienen todos los datos de la cadena de bloques, y la información en su directorio `wallets`, que contiene su billetera personal. Sin embargo, tome nota del archvo `debug.log`, al que debe consultar si alguna vez tiene problemas con la configuración.

> :link: **TESTNET vs MAINNET:** Si está utilizando mainnet, entonces _todo_ se colocará en el directorio principal `~/.bitcoin`. Estas diversas configuraciones se apilan elegantemente, por lo que si usted está usando MainNet, testnet y Regtest, usted encontrará que `~/.bitcoin` contiene el archivo de configuración y sus datos MainNet, el directorio `~/.bitcoin/testnet3` contiene los datos Testnet, y el directorio `~/.bitcoin/regtest` los datos Regtest.

## Conozca sus comandos Bitcoin-cli

La mayor parte de su trabajo inicial se realizará con el comando `bitcoin-cli`, que ofrece una interfaz sencilla para `bitcoind`. Si alguna vez desea obtener más información sobre su uso, simplemente ejecútelo con el argumento `help`. Sin ningún otro argumento, le muestra todos los comandos posibles:

```
$ bitcoin-cli help
== Blockchain ==
getbestblockhash
getblock "blockhash" ( verbosity )
getblockchaininfo
getblockcount
getblockfilter "blockhash" ( "filtertype" )
getblockhash height
getblockheader "blockhash" ( verbose )
getblockstats hash_or_height ( stats )
getchaintips
getchaintxstats ( nblocks "blockhash" )
getdifficulty
getmempoolancestors "txid" ( verbose )
getmempooldescendants "txid" ( verbose )
getmempoolentry "txid"
getmempoolinfo
getrawmempool ( verbose )
gettxout "txid" n ( include_mempool )
gettxoutproof ["txid",...] ( "blockhash" )
gettxoutsetinfo
preciousblock "blockhash"
pruneblockchain height
savemempool
scantxoutset "action" ( [scanobjects,...] )
verifychain ( checklevel nblocks )
verifytxoutproof "proof"

== Control ==
getmemoryinfo ( "mode" )
getrpcinfo
help ( "command" )
logging ( ["include_category",...] ["exclude_category",...] )
stop
uptime

== Generating ==
generatetoaddress nblocks "address" ( maxtries )
generatetodescriptor num_blocks "descriptor" ( maxtries )

== Mining ==
getblocktemplate ( "template_request" )
getmininginfo
getnetworkhashps ( nblocks height )
prioritisetransaction "txid" ( dummy ) fee_delta
submitblock "hexdata" ( "dummy" )
submitheader "hexdata"

== Network ==
addnode "node" "command"
clearbanned
disconnectnode ( "address" nodeid )
getaddednodeinfo ( "node" )
getconnectioncount
getnettotals
getnetworkinfo
getnodeaddresses ( count )
getpeerinfo
listbanned
ping
setban "subnet" "command" ( bantime absolute )
setnetworkactive state

== Rawtransactions ==
analyzepsbt "psbt"
combinepsbt ["psbt",...]
combinerawtransaction ["hexstring",...]
converttopsbt "hexstring" ( permitsigdata iswitness )
createpsbt [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount},{"data":"hex"},...] ( locktime replaceable )
createrawtransaction [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount},{"data":"hex"},...] ( locktime replaceable )
decodepsbt "psbt"
decoderawtransaction "hexstring" ( iswitness )
decodescript "hexstring"
finalizepsbt "psbt" ( extract )
fundrawtransaction "hexstring" ( options iswitness )
getrawtransaction "txid" ( verbose "blockhash" )
joinpsbts ["psbt",...]
sendrawtransaction "hexstring" ( maxfeerate )
signrawtransactionwithkey "hexstring" ["privatekey",...] ( [{"txid":"hex","vout":n,"scriptPubKey":"hex","redeemScript":"hex","witnessScript":"hex","amount":amount},...] "sighashtype" )
testmempoolaccept ["rawtx",...] ( maxfeerate )
utxoupdatepsbt "psbt" ( ["",{"desc":"str","range":n or [n,n]},...] )

== Util ==
createmultisig nrequired ["key",...] ( "address_type" )
deriveaddresses "descriptor" ( range )
estimatesmartfee conf_target ( "estimate_mode" )
getdescriptorinfo "descriptor"
signmessagewithprivkey "privkey" "message"
validateaddress "address"
verifymessage "address" "signature" "message"

== Wallet ==
abandontransaction "txid"
abortrescan
addmultisigaddress nrequired ["key",...] ( "label" "address_type" )
backupwallet "destination"
bumpfee "txid" ( options )
createwallet "wallet_name" ( disable_private_keys blank "passphrase" avoid_reuse )
dumpprivkey "address"
dumpwallet "filename"
encryptwallet "passphrase"
getaddressesbylabel "label"
getaddressinfo "address"
getbalance ( "dummy" minconf include_watchonly avoid_reuse )
getbalances
getnewaddress ( "label" "address_type" )
getrawchangeaddress ( "address_type" )
getreceivedbyaddress "address" ( minconf )
getreceivedbylabel "label" ( minconf )
gettransaction "txid" ( include_watchonly verbose )
getunconfirmedbalance
getwalletinfo
importaddress "address" ( "label" rescan p2sh )
importmulti "requests" ( "options" )
importprivkey "privkey" ( "label" rescan )
importprunedfunds "rawtransaction" "txoutproof"
importpubkey "pubkey" ( "label" rescan )
importwallet "filename"
keypoolrefill ( newsize )
listaddressgroupings
listlabels ( "purpose" )
listlockunspent
listreceivedbyaddress ( minconf include_empty include_watchonly "address_filter" )
listreceivedbylabel ( minconf include_empty include_watchonly )
listsinceblock ( "blockhash" target_confirmations include_watchonly include_removed )
listtransactions ( "label" count skip include_watchonly )
listunspent ( minconf maxconf ["address",...] include_unsafe query_options )
listwalletdir
listwallets
loadwallet "filename"
lockunspent unlock ( [{"txid":"hex","vout":n},...] )
removeprunedfunds "txid"
rescanblockchain ( start_height stop_height )
sendmany "" {"address":amount} ( minconf "comment" ["address",...] replaceable conf_target "estimate_mode" )
sendtoaddress "address" amount ( "comment" "comment_to" subtractfeefromamount replaceable conf_target "estimate_mode" avoid_reuse )
sethdseed ( newkeypool "seed" )
setlabel "address" "label"
settxfee amount
setwalletflag "flag" ( value )
signmessage "address" "message"
signrawtransactionwithwallet "hexstring" ( [{"txid":"hex","vout":n,"scriptPubKey":"hex","redeemScript":"hex","witnessScript":"hex","amount":amount},...] "sighashtype" )
unloadwallet ( "wallet_name" )
walletcreatefundedpsbt [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount},{"data":"hex"},...] ( locktime options bip32derivs )
walletlock
walletpassphrase "passphrase" timeout
walletpassphrasechange "oldpassphrase" "newpassphrase"
walletprocesspsbt "psbt" ( sign "sighashtype" bip32derivs )

== Zmq ==
getzmqnotifications
```
También puede escribir `bitcoin-cli help [command]` para obtener información aún más extensa sobre ese comando. Por ejemplo:
```
$ bitcoin-cli help getmininginfo
...
Returns a json object containing mining-related information.
Result:
{                              (json object)
  "blocks" : n,                (numeric) The current block
  "currentblockweight" : n,    (numeric, optional) The block weight of the last assembled block (only present if a block was ever assembled)
  "currentblocktx" : n,        (numeric, optional) The number of block transactions of the last assembled block (only present if a block was ever assembled)
  "difficulty" : n,            (numeric) The current difficulty
  "networkhashps" : n,         (numeric) The network hashes per second
  "pooledtx" : n,              (numeric) The size of the mempool
  "chain" : "str",             (string) current network name (main, test, regtest)
  "warnings" : "str"           (string) any network and blockchain warnings
}

Examples:
> bitcoin-cli getmininginfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id": "curltest", "method": "getmininginfo", "params": []}' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
> :book: ***¿Qué es RPC?*** `bitcoin-cli` es solo una interfaz práctica que le permite enviar comandos al `bitcoind`. Más específicamente, es una interfaz que le permite enviar comandos RPC (o Protocolo de procedimiento remoto) al `bitcoind`. A menudo, el comando `bitcoin-cli` y el comando RPC tienen nombres e interfaces idénticos, pero algunos comandos `bitcoin-cli` proporcionan atajos para solicitudes RPC más complejas. Generalmente, la interfaz `bitcoin-cli` es mucho más limpia y simple que intentar enviar comandos RPC a mano, usando `curl` o algún otro método. Sin embargo, también tiene limitaciones en cuanto a lo que puede hacer en última instancia.

## Opcional: conozca su información de Bitcoin

Una variedad de comandos bitcoin-cli pueden brindarle información adicional sobre sus datos bitcoin. Los más generales son:

`bitcoin-cli -getinfo` devuelve información de diferentes RPC (fácil de usar)

```diff
$ bitcoin-cli -getinfo

! Chain: test
Blocks: 1977694
Headers: 1977694
Verification progress: 0.9999993275374796
Difficulty: 1

+ Network: in 0, out 8, total 8
Version: 219900
Time offset (s): 0
Proxy: N/A
Min tx relay fee rate (BTC/kvB): 0.00001000

@@ Wallet: ""@@
Keypool size: 1000
Unlocked until: 0
Transaction fee rate (-paytxfee) (BTC/kvB): 0.00000000

# Balance: 0.02853102

- Warnings: unknown new rules activated (versionbit 28)

```

Otros comandos para obtener información sobre blockchain, minería, red, billetera, etc.

```
$ bitcoin-cli getblockchaininfo
$ bitcoin-cli getmininginfo
$ bitcoin-cli getnetworkinfo
$ bitcoin-cli getnettotals
$ bitcoin-cli getwalletinfo
```
Por ejemplo, `bitcoin-cli getnetworkinfo` le brinda una variedad de información sobre su configuración y su acceso a varias redes:
```
$ bitcoin-cli getnetworkinfo
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
      "address": "45.79.111.171",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fecc:fdb7",
      "port": 18333,
      "score": 1
    },
    {
      "address": "4wrr3ktm6gl4sojx.onion",
      "port": 18333,
      "score": 4
    }
  ],
  "warnings": "Warning: unknown new rules activated (versionbit 28)"
}
```
No dude en hacer referencia a cualquiera de estos y utilizar el comando "bitcoin-cli help" si desea obtener más información sobre lo que hacen.

## Resumen: conociendo la configuración de Bitcoin

El directorio `~/.bitcoin` contiene todos sus archivos, mientras que `bitcoin-cli help` se puede usar con una variedad de comandos informativos para obtener más información sobre cómo funcionan su configuración y Bitcoin.

## Que sigue?

Continúe con "Comprensión de la configuración de Bitcoin" [§3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md).

