# 3.2: Knowing Your Bitcoin Setup

Before you start playing with Bitcoin, you may always want to come to a better understanding of your setup.

## Know Your Bitcoin Directory

To start with, you should understand where everything is kept: the `~/.bitcoin` directory.

The main directory just contains your config file and the testnet directory:
```
$ ls ~/.bitcoin
bitcoin.conf  testnet3
```
The setup guides in [Chapter Two: Creating a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) laid out a standardized config file. [§3.1: Verifying Your Bitcoin Setup](03_1_Verifying_Your_Bitcoin_Setup.md) suggested how to change it to support more advanced setups. If you're interested in learning even more about the config file, you may wish to consult [Jameson Lopp's Bitcoin Core Config Generator](https://jlopp.github.io/bitcoin-core-config-generator/).

Moving back to your ~/.bitcoin directory, you'll find that the testnet3 directory contains all of the guts:
```
$ ls ~/.bitcoin/testnet3
banlist.json   blocks	  debug.log	     mempool.dat	peers.dat
bitcoind.pid  chainstate  fee_estimates.dat  onion_private_key	wallets
```
You shouldn't mess with most of these files and directories — particularly not the `blocks` and `chainstate` directories, which contain all of the blockchain data, and the information in your `wallets` directory, which contains your personal wallet. However, do take careful note of the `debug.log` file, which you should refer to if you ever have problems with your setup.

> :link: **TESTNET vs MAINNET:** If you're using mainnet, then _everything_ will instead be placed in the main `~/.bitcoin` directory. These various setups _do_ elegantly stack, so if you are using mainnet, testnet, and regtest, you'll find that `~/.bitcoin` contains your config file and your mainnet data, the `~/.bitcoin/testnet3` directory contains your testnet data, and the `~/.bitcoin/regtest` directory contains your regtest data.

## Know Your Bitcoin-cli Commands

Most of your early work will be done with the `bitcoin-cli` command, which offers an easy interface to `bitcoind`. If you ever want more information on its usage, just run it with the `help` argument. Without any other arguments, it shows you every possible command:


-------------------------------------------------
# 3.2: Capire la configurazione di Bitcoin

Prima di iniziare a giocare con Bitcoin, potresti sempre voler comprendere meglio la tua configurazione.

## Conosci la tua directory Bitcoin

Per cominciare, dovresti capire dove viene conservato tutto: la directory `~/.bitcoin`.

La directory principale contiene solo il file di configurazione e la directory di testnet:
```
$ ls ~/.bitcoin
bitcoin.conf testnet3
```
Le guide di configurazione nel [Capitolo 2: Configurare Bitcoin-Core VPS](02_0_Configurare_Bitcoin-Core_VPS.md) presentano un file di configurazione standardizzato. [Capitolo 3.1: 03_1 Verificare la configurazione Bitcoin](03_1_Verificare_la_configurazione_Bitcoin.md) ha suggerito come modificarlo per supportare configurazioni più avanzate. Se sei interessato a saperne di più sul file di configurazione, potresti consultare [Bitcoin Core Config Generator di Jameson Lopp](https://jlopp.github.io/bitcoin-core-config-generator/).

Tornando alla directory ~/.bitcoin, scoprirai che la directory testnet3 contiene tutto il resto:
```
$ ls ~/.bitcoin/testnet3
banlist.json   blocks	  debug.log	     mempool.dat	peers.dat
bitcoind.pid  chainstate  fee_estimates.dat  onion_private_key	wallets
```
Non dovresti scherzare con la maggior parte di questi file e directory, in particolare non con le directory "blocks" e "chainstate", che contengono tutti i dati blockchain e le informazioni nella directory "wallets", che contiene il tuo portafoglio personale. Tuttavia, prendi attentamente nota del file `debug.log`, a cui dovresti fare riferimento se dovessi avere problemi con la tua configurazione.

> :link: **TESTNET vs MAINNET:** Se stai utilizzando mainnet, allora _tutto_ verrà inserito nella directory principale `~/.bitcoin`. Queste varie configurazioni _si impilano_ in modo elegante, quindi se stai utilizzando mainnet, testnet e regtest, scoprirai che `~/.bitcoin` contiene il tuo file di configurazione e i tuoi dati mainnet, la directory `~/.bitcoin/testnet3` contiene i tuoi dati testnet e la directory "~/.bitcoin/regtest" contiene i tuoi dati regtest.

## Conosci i comandi di Bitcoin-cli

La maggior parte del tuo lavoro iniziale verrà svolto con il comando "bitcoin-cli", che offre un'interfaccia semplice per "bitcoind". Se desideri maggiori informazioni sul suo utilizzo, eseguilo semplicemente con l'argomento "aiuto". Senza altri argomenti, ti mostra tutti i comandi possibili:
-------------------------------------------------



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
Puoi anche scrivere `bitcoin-cli help [command]` per maggiori informazioni sul commando specifico. Per esempio:
```
$ bitcoin-cli help getmininginfo
```
Restituisce un oggetto json contenente informazioni relative al mining.
Risultato:
```
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
```
Esempio:
```
> bitcoin-cli getmininginfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id": "curltest", "method": "getmininginfo", "params": []}' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
> :book: ***Cos'è RPC?*** `bitcoin-cli` è semplicemente una pratica interfaccia che ti consente di inviare comandi a `bitcoind`. Più specificamente, è un'interfaccia che ti consente di inviare comandi RPC (o Remote Procedure Protocol) a `bitcoind`. Spesso, il comando `bitcoin-cli` e il comando RPC hanno nomi e interfacce identici, ma alcuni comandi `bitcoin-cli` forniscono invece scorciatoie per richieste RPC più complesse. In generale, l'interfaccia `bitcoin-cli` è molto più pulita e semplice rispetto al tentativo di inviare comandi RPC manualmente, utilizzando "curl" o qualche altro metodo. Tuttavia, presenta anche dei limiti su ciò che puoi fare.

## Opzionale: conoscere le informazioni su Bitcoin

Una varietà di comandi `bitcoin-cli` può fornirti ulteriori informazioni sui tuoi dati bitcoin. Quelli più generali sono:

`bitcoin-cli -getinfo` restituisce informazioni da diversi RPCs (user-friendly)

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

Altri comandi per ottenere informazioni su blockchain, mining, rete, portafoglio ecc.
```
$ bitcoin-cli getblockchaininfo
$ bitcoin-cli getmininginfo
$ bitcoin-cli getnetworkinfo
$ bitcoin-cli getnettotals
$ bitcoin-cli getwalletinfo
```
Ad esempio `bitcoin-cli getnetworkinfo` ti fornisce una serie di informazioni sulla tua configurazione e sul suo accesso a varie reti:
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
Sentiti libero di fare riferimento a uno di questi e di utilizzare `bitcoin-cli help` se desideri maggiori informazioni su cosa fanno.

## Riepilogo: Capire la configurazione di Bitcoin

La directory `~/.bitcoin` contiene tutti i tuoi file, mentre `bitcoin-cli help` contiene e una varietà di comandi che puoi utilizzare per ottenere maggiori informazioni su come funzionano la tua configurazione e Bitcoin.

## Ora cosa facciamo?

Continua "Comprendere la configurazione di Bitcoin" con [Capitolo 3.3:Configurare la Wallet](03_3_Configurare_la_Wallet.md).
