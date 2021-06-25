# 3.2: Conhecendo a configuração do seu node Bitcoin

Antes de começar o jogo com Bitcoin, você pode sempre querer entender melhor sua configuração.

## Conhecendo o seu diretório Bitcoin

Para começar, você deve entender onde tudo é guardado: o diretório `~/.bitcoin`.

O diretório principal contém apenas seu arquivo de configuração e o diretório testnet:
```
$ ls ~/.bitcoin
bitcoin.conf  testnet3
```
Os guias de configuração em [Capítulo dois: Criando um VPS Bitcoin-Core](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) estabelecem um arquivo de configuração padronizado. [3.1: Verificando sua configuração do seu node Bitcoin](03_1_Verifying_Your_Bitcoin_Setup.md) Sugeri como alterá-lo para oferecer suporte a configurações mais avançadas. Se você estiver interessado em aprender ainda mais sobre o arquivo de configuração, você pode consultar [Jameson Lopp's Bitcoin Core gerador de configuração](https://jlopp.github.io/bitcoin-core-config-generator/).

Voltando para o diretório `~/.bitcoin`, você descobrirá que o diretório testnet3 contém todos os elementos:
```
$ ls ~/.bitcoin/testnet3
banlist.dat   blocks	  debug.log	     mempool.dat	peers.dat
bitcoind.pid  chainstate  fee_estimates.dat  onion_private_key	wallets
```
Você não deve mexer com a maioria desses arquivos e diretórios - particularmente não com os diretórios `blocks` e` chainstate`, que contêm todos os dados do blockchain, e as informações em seu diretório `wallets`, que contém sua carteira pessoal. No entanto, preste atenção ao arquivo `debug.log`, que você deve consultar se tiver problemas com sua configuração.

> :link: **TESTNET vs MAINNET:** Se você estiver usando mainnet, então _tudo_ será colocado no diretório principal `~/.bitcoin`. Então se você estiver usando mainnet, testnet e regtest, você verá que `~/.bitcoin` contém seu arquivo de configuração e seus dados mainnet, o diretório` ~/.bitcoin/testnet3` contém seus dados testnet, e o diretório `~/.bitcoin/regtest` contém seus dados de regtest.

## Conheça os comandos do Bitcoin-cli

A maior parte do seu trabalho inicial será feito com o comando `bitcoin-cli`, que oferece uma interface fácil para o `bitcoind`. Se você quiser mais informações sobre seu uso, basta executá-lo com o argumento `help`. Sem nenhum outro argumento, ele mostrara todos os comandos possíveis:
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
Você também pode digitar `bitcoin-cli help [command]` para obter informações ainda mais detalhadas sobre aquele comando. Por exemplo:
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
> :book: ***What is RPC?*** `bitcoin-cli` is just a handy interface that lets you send commands to the `bitcoind`. More specifically, it's an interface that lets you send RPC (or Remote Procedure Protocol) commands to the `bitcoind`. Often, the `bitcoin-cli` command and the RPC command have identical names and interfaces, but some `bitcoin-cli` commands instead provide shortcuts for more complex RPC requests. Generally, the `bitcoin-cli` interface is much cleaner and simpler than trying to send RPC commands by hand, using `curl` or some other method. However, it also has limitations as to what you can ultimately do.

## Opcional: Conheça as informações do seu node Bitcoin

Uma variedade de comandos bitcoin-cli podem fornecer informações adicionais sobre seus node Bitcoin. Os mais gerais são:

`bitcoin-cli -getinfo` retorna informações diferentes do RPC

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

Outros comandos para obter informações sobre blockchain, mineração, rede, carteira etc.

```
$ bitcoin-cli getblockchaininfo
$ bitcoin-cli getmininginfo
$ bitcoin-cli getnetworkinfo
$ bitcoin-cli getnettotals
$ bitcoin-cli getwalletinfo
```
Por exemplo, `bitcoin-cli getnetworkinfo` fornece uma variedade de informações sobre sua configuração e seu acesso a outras redes:
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

Sinta-se à vontade para consultar qualquer um deles e usar `bitcoin-cli help` se quiser mais informações sobre o que qualquer um deles faz.

## Resumo: Conhecendo a configuração do seu node Bitcoin

O diretório `~ /.bitcoin` contém todos os seus arquivos, enquanto `bitcoin-cli help` te retorna uma variedade de comandos, info podem ser usados para obter mais informações sobre como sua configuração e o Bitcoin funcionam.

## Mas o que vem a seguir?

Continue "Compreendendo sua configuração do seu node Bitcoin" com [3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md).