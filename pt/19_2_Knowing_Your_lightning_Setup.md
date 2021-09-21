# 19.2: Conhecendo Nossa Configuração da c-lightning

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Antes de começar a acessar a Lightning Network, devemos compreender melhor a nossa configuração.

## Conhecendo o Diretório da c-lightning

Ao usar a c-lightning, tudo será mantindo dentro do diretório `~/.lightning`.

O diretório principal contém apenas os diretórios para as redes configuradas, neste caso da Testnet:
```
$ ls ~/.lightning
testnet
```
O diretório `~/.lightning/testnet` irá então conter a essência da nossa configuração:
```
$ ls ~/.lightning/testnet3
config  gossip_store  hsm_secret  lightningd.sqlite3  lightningd.sqlite3-journal  lightning-rpc
```

> :link: **TESTNET vs MAINNET:** Se estivermos usando a Mainnet, então _tudo_ será colocado no diretório principal `~/.lightning/bitcoin`. Essas várias configurações _empilham-se_ elegantemente, então se estivermos usando a Mainnet, Testnet e Regtest, descobriremos que `~/.lightning/bitcoin` contém os arquivos de configuração e os dados da Mainnet, o diretório `~/.lightning/testnet` contém os dados da Testnet, e o diretório `~/.lightning/regtest` contém os dados Regtest.

## Conhecendo os Comandos lightning-cli

A maior parte do nosso trabalho inicial será feito com o comando `lightning-cli`, que oferece uma interface fácil para o `lightningd`, assim como o `bitcoin-cli`.

Já vimos que o comando `help` nos dará uma lista de outros comandos:

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

## Conhecendo Informações da Lightning

Uma variedade de comandos `lightning-cli` podem fornecer informações adicionais sobre nosso node Lightning. Os mais comuns são:
```
$ lightning-cli --testnet listconfigs
$ lightning-cli --testnet listfunds
$ lightning-cli --testnet listtransactions
$ lightning-cli --testnet listinvoices
$ lightning-cli --testnet listnodes   
```
* listconfigs: O comando RPC `listconfigs` lista todas as opções de configuração;
* listfunds: O comando RPC `listfunds` exibe todos os fundos disponíveis, seja em saídas não gastas (UTXOs) na carteira interna ou fundos bloqueados em canais abertos no momento;
* listtransactions: O comando RPC `listtransactions` retorna as transações rastreadas na carteira. Isso inclui depósitos, retiradas e transações relacionadas a canais;
* listinvoices: O comando RPC `listinvoices` recupera o status de um invoice específico, se houver, ou o status de todos os invoices, se não houver nenhum argumento;
* listnodes: O comando RPC `listnodes` retorna nodes que nosso servidor aprendeu através da comunicação com outros nodes, ou um específico se o id do node foi informado.

Por exemplo, `lightning-cli listconfigs` fornece uma variedade de informações sobre nossa configuração:

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

## Resumo: Conhecendo Nossa Configuração da c-lightning

O diretório `~/.lightning` contém todos os arquivos, enquanto o comando `lightning-cli help` mostra uma variedade de informações dos comandos que podem ser usados para obter mais informações sobre a configuração e o funcionamento da Lightning Network.

## O Que Vem Depois?

Precisaremos de um segundo node para testar o pagamento dos invoices. Se precisarmos de suporte para configurar um, podemos ler o [Prefácio: Acessando um Segundo Node Lightning](19_2__Interlude_Accessing_a_Second_Lightning_Node.md).

Caso contrário, vamos continuar "Compreendendo a Configuração da Lightning" na seção [§19.3: Criando um Canal na Lightning](19_3_Setting_Up_a_Channel.md).