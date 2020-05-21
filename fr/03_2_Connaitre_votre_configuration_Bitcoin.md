# 3.2: Connaître votre configuration Bitcoin

> :information_source: **NOTE:** Il s'agit d'un projet en cours, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Avant de commencer à jouer avec Bitcoin, vous voudrez peut-être toujours mieux comprendre votre configuration.

## Connaître votre répertoire Bitcoin

Pour commencer, vous devez comprendre où tout est conservé : le répertoire ~/.bitcoin.

Le répertoire principal ne contient que votre fichier de configuration et le répertoire testnet :
```
$ ls ~/.bitcoin
bitcoin.conf  testnet3
```
Les guides de configuration du [chapitre 2 : Mise en place d'un VPS Bitcoin-Core](02_0_Mise_en_place_d_un_VPS_Bitcoin-Core.md) présentent un fichier de configuration standardisé. [§3.1 : Vérification de votre configuration Bitcoin](03_1_Verification_de_votre_configuration_Bitcoin.md) a suggéré comment la modifier pour prendre en charge des configurations plus avancées. Si vous souhaitez en savoir plus sur le fichier de configuration, vous pouvez consulter le [Bitcoin Core Config Generator de Jameson Lopp](https://jlopp.github.io/bitcoin-core-config-generator/).

En retournant à votre répertoire ~/.bitcoin, vous constaterez que le répertoire testnet3 contient "tout le matériel" :
```
$ ls ~/.bitcoin/testnet3
banlist.dat   blocks	  database  debug.log  wallet.dat
bitcoind.pid  chainstate  db.log    peers.dat
```
Vous ne devriez pas toucher à la plupart de ces fichiers et répertoires - surtout pas les répertoires blocks et chainstate, qui contiennent toutes les données des chaînes de blocs, et le fichier wallet.dat, qui contient votre portefeuille personnel. Cependant, prenez soigneusement note des fichiers db.log et debug.log, auxquels vous devez vous référer si vous avez des problèmes avec votre installation.

> :link: **TESTNET vs MAINNET:** Si vous utilisez le réseau principal, alors _tout_ sera placé dans le répertoire principal ~/.bitcoin. Ces différentes configurations _doivent_ s'empiler élégamment, donc si vous utilisez mainnet, testnet et regtest, vous trouverez que ~/.bitcoin contient votre fichier de configuration et vos données mainnet, ~/.bitcoin/testnet3 contient vos données testnet, et ~/.bitcoin/regtest contient vos données regtest .

## Connaissez les commandes de votre Bitcoin-cli

La plupart de vos premiers travaux seront réalisés avec la commande `bitcoin-cli`, qui offre une interface facile à `bitcoind`. Si vous souhaitez obtenir plus d'informations sur son utilisation, il vous suffit de l'exécuter avec l'argument `help`. Sans aucun autre argument, il vous montre toutes les commandes possibles :

```
$ bitcoin-cli help
== Blockchain ==
getbestblockhash
getblock "blockhash" ( verbosity )
getblockchaininfo
getblockcount
getblockhash height
getblockheader "hash" ( verbose )
getchaintips
getchaintxstats ( nblocks blockhash )
getdifficulty
getmempoolancestors txid (verbose)
getmempooldescendants txid (verbose)
getmempoolentry txid
getmempoolinfo
getrawmempool ( verbose )
gettxout "txid" n ( include_mempool )
gettxoutproof ["txid",...] ( blockhash )
gettxoutsetinfo
preciousblock "blockhash"
pruneblockchain
verifychain ( checklevel nblocks )
verifytxoutproof "proof"

== Control ==
getmemoryinfo ("mode")
getnetworkinginfo
help ( "command" )
stop
uptime

== Generating ==
generate nblocks ( maxtries )
generatetoaddress nblocks address (maxtries)

== Mining ==
getblocktemplate ( TemplateRequest )
getmininginfo
getnetworkhashps ( nblocks height )
prioritisetransaction <txid> <dummy value> <fee delta>
submitblock "hexdata"  ( "dummy" )

== Network ==
addnode "node" "add|remove|onetry"
clearbanned
disconnectnode "[address]" [nodeid]
getaddednodeinfo ( "node" )
getconnectioncount
getnettotals
getnetworkinfo
getpeerinfo
listbanned
ping
setban "subnet" "add|remove" (bantime) (absolute)
setnetworkactive true|false

== Rawtransactions ==
combinerawtransaction ["hexstring",...]
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime ) ( replaceable )
decoderawtransaction "hexstring"
decodescript "hexstring"
fundrawtransaction "hexstring" ( options )
getrawtransaction "txid" ( verbose )
sendrawtransaction "hexstring" ( allowhighfees )
signrawtransaction "hexstring" ( [{"txid":"id","vout":n,"scriptPubKey":"hex","redeemScript":"hex"},...] ["privatekey1",...] sighashtype )

== Util ==
createmultisig nrequired ["key",...]
estimatefee nblocks
estimatesmartfee conf_target ("estimate_mode")
signmessagewithprivkey "privkey" "message"
validateaddress "address"
verifymessage "address" "signature" "message"

== Wallet ==
abandontransaction "txid"
abortrescan
addmultisigaddress nrequired ["key",...] ( "account" )
addwitnessaddress "address"
backupwallet "destination"
bumpfee "txid" ( options )
dumpprivkey "address"
dumpwallet "filename"
encryptwallet "passphrase"
getaccount "address"
getaccountaddress "account"
getaddressesbyaccount "account"
getbalance ( "account" minconf include_watchonly )
getnewaddress ( "account" )
getrawchangeaddress
getreceivedbyaccount "account" ( minconf )
getreceivedbyaddress "address" ( minconf )
gettransaction "txid" ( include_watchonly )
getunconfirmedbalance
getwalletinfo
importaddress "address" ( "label" rescan p2sh )
importmulti "requests" ( "options" )
importprivkey "privkey" ( "label" ) ( rescan )
importprunedfunds
importpubkey "pubkey" ( "label" rescan )
importwallet "filename"
keypoolrefill ( newsize )
listaccounts ( minconf include_watchonly)
listaddressgroupings
listlockunspent
listreceivedbyaccount ( minconf include_empty include_watchonly)
listreceivedbyaddress ( minconf include_empty include_watchonly)
listsinceblock ( "blockhash" target_confirmations include_watchonly include_removed )
listtransactions ( "account" count skip include_watchonly)
listunspent ( minconf maxconf  ["addresses",...] [include_unsafe] [query_options])
listwallets
lockunspent unlock ([{"txid":"txid","vout":n},...])
move "fromaccount" "toaccount" amount ( minconf "comment" )
removeprunedfunds "txid"
sendfrom "fromaccount" "toaddress" amount ( minconf "comment" "comment_to" )
sendmany "fromaccount" {"address":amount,...} ( minconf "comment" ["address",...] replaceable conf_target "estimate_mode")
sendtoaddress "address" amount ( "comment" "comment_to" subtractfeefromamount replaceable conf_target "estimate_mode")
setaccount "address" "account"
settxfee amount
signmessage "address" "message"
```
Vous pouvez également taper `bitcoin-cli help [commande]` pour obtenir des informations encore plus détaillées sur cette commande. Par exemple :

```
$ bitcoin-cli help getmininginfo
getmininginfo

Returns a json object containing mining-related information.
Result:
{
  "blocks": nnn,             (numeric) The current block
  "currentblocksize": nnn,   (numeric) The last block size
  "currentblockweight": nnn, (numeric) The last block weight
  "currentblocktx": nnn,     (numeric) The last block transaction
  "difficulty": xxx.xxxxx    (numeric) The current difficulty
  "errors": "..."            (string) Current errors
  "networkhashps": nnn,      (numeric) The network hashes per second
  "pooledtx": n              (numeric) The size of the mempool
  "testnet": true|false      (boolean) If using testnet or not
  "chain": "xxxx",           (string) current network name as defined in BIP70 (main, test, regtest)
}

Examples:
> bitcoin-cli getmininginfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
_Qu'est-ce que le RPC ?_`bitcoin-cli` est juste une interface pratique qui vous permet d'envoyer des commandes au `bitcoind`. Plus précisément, c'est une interface qui vous permet d'envoyer des commandes RPC (Remote Procedure Protocol=Protocole de procédure à distance) au `bitcoind`. Souvent, la commande `bitcoin-cli` et la commande RPC ont des noms et des interfaces identiques, mais certaines commandes `bitcoin-cli` fournissent plutôt des raccourcis pour les requêtes RPC plus complexes. En général, l'interface `bitcoin-cli` est beaucoup plus propre et plus simple que d'essayer d'envoyer des commandes RPC à la main, en utilisant `curl` ou une autre méthode. Toutefois, elle présente également des limites quant à ce que vous pouvez faire en fin de compte.

## Facultatif : Connaître vos informations sur les bitcoins

Diverses commandes `bitcoin-cli` peuvent vous donner des informations supplémentaires sur vos données bitcoin. Les plus générales sont les suivantes :
```
$ bitcoin-cli getblockchaininfo
$ bitcoin-cli getmininginfo
$ bitcoin-cli getnetworkinfo
$ bitcoin-cli getnettotals
$ bitcoin-cli getwalletinfo
```
Par exemple, `bitcoin-cli getnetworkinfo` vous donne toute une série d'informations sur votre installation et son accès à différents réseaux :

```
$ bitcoin-cli getnetworkinfo
{
  "version": 150100,
  "subversion": "/Satoshi:0.15.1/",
  "protocolversion": 70015,
  "localservices": "000000000000000d",
  "localrelay": false,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 9,
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
      "limited": true,
      "reachable": false,
      "proxy": "",
      "proxy_randomize_credentials": false
    }
  ],
  "relayfee": 0.00001000,
  "incrementalfee": 0.00001000,
  "localaddresses": [
    {
      "address": "192.168.1.17",
      "port": 18333,
      "score": 52
    }
  ],
  "warnings": "Warning: unknown
```
N'hésitez pas à faire référence à l'un d'entre eux et à utiliser "bitcoin-cli help" si vous voulez plus d'informations sur ce qu'ils font.

## Résumé : Connaître votre configuration Bitcoin

Le répertoire ~/.bitcoin contient tous vos fichiers, tandis que l'aide `bitcoin-cli` et diverses commandes peuvent être utilisées pour obtenir plus d'informations sur le fonctionnement de votre installation et de Bitcoin.

## Quelle est la prochaine étape ?

Continuez "Connaître votre configuration Bitcoin" avec [le §3.3: Configurer votre portefeuille](03_3_Configurer_votre_portefeuille.md).

