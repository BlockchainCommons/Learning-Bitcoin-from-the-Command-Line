# 18.4: Accesso a Bitcoind con Python

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Questa sezione spiega come interagire con `bitcoind` utilizzando il linguaggio di programmazione Python e il [Python-BitcoinRPC](https://github.com/jgarzik/python-bitcoinrpc).

## Configurare Python

Se hai già installato Bitcoin Core, dovresti avere anche Python 3 disponibile.
Puoi verificarlo eseguendo:

`$ python3 --version`

Se restituisce un numero di versione (ad es., `3.7.3` o `3.8.3`) allora hai python3 installato.

Tuttavia, se in qualche modo non hai Python installato, dovrai compilarlo da sorgente come segue. Consulta la variante ["Compilare Python da Sorgente"](17_4_Accessing_Bitcoind_with_Python.md#variant-build-python-from-source) prima di continuare.

### Configurare BitcoinRPC

Sia che tu abbia utilizzato un Python esistente o che lo abbia compilato da sorgente, ora sei pronto per installare la libreria `python-bitcoinrpc`:

```
$ pip3 install python-bitcoinrpc
```

Se non hai `pip` installato, dovrai eseguire il seguente comando:

```
$ sudo apt install python3-pip
```

(Poi ripeti le istruzioni `pip3 install python-bitcoinrpc`.)

### Creare un Progetto BitcoinRPC

Generalmente dovrai includere le dichiarazioni appropriate da `bitcoinrpc` nei tuoi progetti Bitcoin in Python. Quanto segue ti darà accesso ai comandi basati su RPC:

```
from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
```

Potresti anche trovare utili i seguenti:

```
from pprint import pprint
import logging
```

`pprint` stamperà in modo leggibile la risposta `json` da `bitcoind`.

`logging` stamperà la chiamata che fai a `bitcoind` e la risposta di `bitcoind`, il che è utile quando fai molte chiamate insieme. Se non vuoi l'output eccessivo nel terminale, commenta semplicemente il blocco `logging`.

## Costruire la Tua Connessione

Ora sei pronto per iniziare a interagire con `bitcoind` stabilendo una connessione. Crea un file chiamato `btcrpc.py` e digita quanto segue:

```
logging.basicConfig()
logging.getLogger("BitcoinRPC").setLevel(logging.DEBUG)
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = "StandUp"
rpc_pass = "6305f1b2dbb3bc5a16cd0f4aac7e1eba"
rpc_host = "127.0.0.1"
rpc_client = AuthServiceProxy(f"http://{rpc_user}:{rpc_pass}@{rpc_host}:18332", timeout=120)
```

Gli argomenti nell'URL sono `<rpc_username>:<rpc_password>@<host_IP_address>:<port>`. Come al solito, l'`user` e il `pass` si trovano nel tuo `~/.bitcoin/bitcoin.conf`, mentre l'`host` è il tuo localhost e la porta è `18332` per testnet. L'argomento `timeout` è specificato poiché i socket vanno in timeout sotto carico pesante su mainnet. Se ottieni una risposta `socket.timeout: timed out`, sii paziente e aumenta il `timeout`.

> :link: **MAINNET VS TESTNET:** La porta sarebbe 8332 per una configurazione mainnet.

### Effettuare una Chiamata RPC

Se `rpc_client` è stato inizializzato con successo, sarai in grado di inviare comandi RPC al tuo nodo bitcoin.

Per utilizzare un metodo RPC da `python-bitcoinrpc`, utilizzerai l'oggetto `rpc_client` che hai creato, il quale fornisce la maggior parte delle funzionalità accessibili tramite `bitcoin-cli`, utilizzando gli stessi nomi dei metodi.

Ad esempio, quanto segue recupererà il blockcount del tuo nodo:

```
block_count = rpc_client.getblockcount()
print("---------------------------------------------------------------")
print("Block Count:", block_count)
print("---------------------------------------------------------------\n")
```

Dovresti vedere il seguente output con `logging` abilitato:

```
DEBUG:BitcoinRPC:-3-> getblockcount []
DEBUG:BitcoinRPC:<-3- 1773020
---------------------------------------------------------------
Block Count: 1773020
---------------------------------------------------------------
```

### Effettuare una Chiamata RPC con Argomenti

Puoi utilizzare quel blockcount come argomento per recuperare l'hash del blocco di un blocco e anche per recuperare i dettagli di quel blocco.

Questo viene fatto inviando comandi all'oggetto `rpc_client` con un argomento:

```
blockhash = rpc_client.getblockhash(block_count)
block = rpc_client.getblock(blockhash)
```

Il `getblockhash` restituirà un singolo valore, mentre il `getblock` restituirà un array associativo di informazioni sul blocco, che include un array sotto `block['tx']` che fornisce dettagli su ogni transazione all'interno del blocco:

```
nTx = block['nTx']
if nTx > 10:
    it_txs = 10
    list_tx_heading = "First 10 transactions: "
else:
    it_txs = nTx
    list_tx_heading = f"All the {it_txs} transactions: "
print("---------------------------------------------------------------")
print("BLOCK: ", block_count)
print("-------------")
print("Block Hash...: ", blockhash)
print("Merkle Root..: ", block['merkleroot'])
print("Block Size...: ", block['size'])
print("Block Weight.: ", block['weight'])
print("Nonce........: ", block['nonce'])
print("Difficulty...: ", block['difficulty'])
print("Number of Tx.: ", nTx)
print(list_tx_heading)
print("---------------------")
i = 0
while i < it_txs:
    print(i, ":", block['tx'][i])
    i += 1
print("---------------------------------------------------------------\n")
```

### Eseguire il Codice

Puoi recuperare [il codice sorgente](src/18_4_getinfo.py) ed eseguirlo con `python3`:

```
$ python3 getinfo.py
---------------------------------------------------------------
Block Count: 1831106
---------------------------------------------------------------
---------------------------------------------------------------
BLOCK:  1831106
-------------
Block Hash...:  00000000000003b2ea7c2cdfffd86156ad1f5606ab58e128940a2534d1348b04
Merkle Root..:  056a547fe59208167eef86fa694263728fb684119254b340c1f86bdd423a8082
Block Size...:  52079
Block Weight.:  128594
Nonce........:  1775583700
Difficulty...:  4194304
Number of Tx.:  155
First 10 transactions: 
---------------------
0 : d228d55112e3aa26265b0118cfdc98345c229d20fe074b9afb87107c03ce11b5
1 : 92822e8e34fafb472b87c99ea3f3e16440452b3f361ed86c6fa62175173fb750
2 : fa7c67600c14d4aa350a9674688f1429577954f4a6c5e4639d06c8964824f647
3 : 3a91d1527e308e5603dafde7ab17824f441a73a779d2571d073466dc9e8451b2
4 : 30fd0e5527b1522e7b26a4818b9edac80fe47c0c39fc34705478a49e684708d0
5 : 24c5372b38c78cbaf5b0b305925502a491bc0c1b5758f50c0bd335abb6ae85f5
6 : be70e125a5793efc5e32051fecba0668df971bdf371138c8261201c2a46b2d38
7 : 41ebf52c847a59ba0aeb4425c74e89a01e91defa86a82785ff53ed4668054561
8 : dc8211b4ce122f87692e7c203672e3eb1ffc44c0a307eafcc560323fcc5fae78
9 : 59e2d8e11cad287eacf3207e64a373f65059286b803ef0981510193ae29cbc8c
---------------------------------------------------------------
```

## Cercare Fondi

Puoi recuperare in modo simile le informazioni del tuo wallet con l'RPC `getwalletinfo`:

```
wallet_info = rpc_client.getwalletinfo()
print("---------------------------------------------------------------")
print("Wallet Info:")
print("-----------")
pprint(wallet_info)
print("---------------------------------------------------------------\n")
```

Dovresti avere un output simile al seguente con `logging` disabilitato:

```
---------------------------------------------------------------
#Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.07160443'),
 'hdseedid': '6dko666b1cc0d69b7eb0539l89eba7b6390kdj02',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1542245729,
 'keypoolsize': 999,
 'keypoolsize_hd_internal': 1000,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 9,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------
```

Altri comandi informativi come `getblockchaininfo`, `getnetworkinfo`, `getpeerinfo` e `getblockchaininfo` funzioneranno in modo simile.

Altri comandi possono darti informazioni specifiche su elementi selezionati all'interno del tuo wallet.

### Recuperare un Array

L'RPC `listtransactions` ti consente di visualizzare le ultime 10 transazioni sul tuo sistema (o un insieme arbitrario di transazioni utilizzando gli argomenti `count` e `skip`). Mostra come un comando RPC possa restituire un array facile da manipolare:

```
tx_list = rpc_client.listtransactions()
pprint(tx_list)
```

### Esplorare un UTXO

Puoi utilizzare in modo simile `listunspent` per ottenere un array di UTXO sul tuo sistema:

```
print("Exploring UTXOs")
## List UTXOs
utxos = rpc_client.listunspent()
print("Utxos: ")
print("-----")
pprint(utxos)
print("------------------------------------------\n")
```

Per manipolare un array come quello restituito da `listtransactions` o `listunspent`, devi semplicemente prendere l'elemento appropriato dall'array appropriato:

```
## Select a UTXO - first one selected here
utxo_txid = utxos[0]['txid']
```

Per `listunspent`, ottieni un `txid`. Puoi recuperare informazioni al riguardo con `gettransaction`, quindi decodificarle con `decoderawtransaction`:

```
utxo_hex = rpc_client.gettransaction(utxo_txid)['hex']
utxo_tx_details = rpc_client.decoderawtransaction(utxo_hex)
print("Details of Utxo with txid:", utxo_txid)
print("---------------------------------------------------------------")
print("UTXO Details:")
print("------------")
pprint(utxo_tx_details)
print("---------------------------------------------------------------\n")
```

Questo codice è disponibile su [walletinfo.py](src/18_4_walletinfo.py).

```
$ python3 walletinfo.py 
---------------------------------------------------------------
#Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.01031734'),
 'hdseedid': 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1596567843,
 'keypoolsize': 1000,
 'keypoolsize_hd_internal': 999,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 6,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------

Utxos: 
-----
[{'address': 'mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg',
  'amount': Decimal('0.00001000'),
  'confirmations': 1180,
  'desc': "pkh([ce0c7e14/0'/0'/25']02d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472)#n06pq9q5",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 0},
 {'address': 'tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4',
  'amount': Decimal('0.01029734'),
  'confirmations': 1180,
  'desc': "wpkh([ce0c7e14/0'/1'/26']02c581259ba7e6aef6d7ea23adb08f7c7f10c4c678f2e097a4074639e7685d4805)#j3pctfhf",
  'safe': True,
  'scriptPubKey': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 1},
 {'address': 'mzDxbtYY3LBBBJ6HhaBAtnHv6c51BRBTLE',
  'amount': Decimal('0.00001000'),
  'confirmations': 1181,
  'desc': "pkh([ce0c7e14/0'/0'/23']0377bdd176f985b4af2f6bdbb22c2925b6007b6c07ba171f75e65990c002615e98)#3y6ef6vu",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914cd339342b06042bb986a45e73d56db46acc1e01488ac',
  'solvable': True,
  'spendable': True,
  'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
  'vout': 1}]
------------------------------------------

Details of Utxo with txid: 84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
---------------------------------------------------------------
UTXO Details:
------------
{'hash': '0c6c27f58f122329bbc53a91f290b35ce23bd2708706b21a04cdc387dc8e2fd9',
 'locktime': 1831103,
 'size': 225,
 'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
 'version': 2,
 'vin': [{'scriptSig': {'asm': '', 'hex': ''},
          'sequence': 4294967294,
          'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
          'txinwitness': ['3044022014b3e2359fb46d8cbc4cd30fa991b455edfa4b419a4c64a53fcdfc79e3ca89db022010cefc3268bc252d55f1982c426328b709b47d02332def9e2efb3b12de2cf0d301',
                          '0351b470e87b44e8e9607acf09b8d4543c51c93c17dc741176319e60202091f2be'],
          'vout': 0}],
 'vout': [{'n': 0,
           'scriptPubKey': {'addresses': ['mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg'],
                            'asm': 'OP_DUP OP_HASH160 '
                                   'a080d1a10f5e7a02d0a291f118982ed19e8cfcd7 '
                                   'OP_EQUALVERIFY OP_CHECKSIG',
                            'hex': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
                            'reqSigs': 1,
                            'type': 'pubkeyhash'},
           'value': Decimal('0.00001000')},
          {'n': 1,
           'scriptPubKey': {'addresses': ['tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4'],
                            'asm': '0 1e127c28a5d696c01aee1adaa8662396a5902fff',
                            'hex': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
                            'reqSigs': 1,
                            'type': 'witness_v0_keyhash'},
           'value': Decimal('0.01029734')}],
 'vsize': 144,
 'weight': 573}
---------------------------------------------------------------
```

## Creare un Indirizzo

Creare un nuovo indirizzo con Python 3 richiede solo l'uso di un RPC come `getnewaddress` o `getrawchangeaddress`.

```
new_address = rpc_client.getnewaddress("Learning-Bitcoin-from-the-Command-Line")
new_change_address = rpc_client.getrawchangeaddress()
```

In questo esempio, dai il comando `getnewaddress` con un argomento: l'etichetta `Learning-Bitcoin-from-the-Command-Line`.

## Inviare una Transazione

Creare una transazione in Python 3 richiede la combinazione di alcuni degli esempi precedenti (creare indirizzi e recuperare UTXO) con alcuni nuovi comandi RPC per creare, firmare e inviare una transazione, proprio come hai fatto in precedenza dalla riga di comando.

Ci sono cinque passaggi:

0. Creare due indirizzi, uno che fungerà da destinatario e l'altro per il resto.
1. Selezionare un UTXO e impostare i dettagli della transazione.
2. Creare una transazione grezza.
3. Firmare la transazione grezza con la chiave privata dell'UTXO.
4. Trasmettere la transazione sulla testnet bitcoin.

### 1. Selezionare UTXO e Impostare i Dettagli della Transazione

Nel seguente frammento di codice, selezioni prima l'UTXO che vuoi spendere. Poi ottieni il suo indirizzo, l'ID della transazione e l'indice del vettore dell'output.

```
utxos = rpc_client.listunspent()
selected_utxo = utxos[0]  # again, selecting the first utxo here
utxo_address = selected_utxo['address']
utxo_txid = selected_utxo['txid']
utxo_vout = selected_utxo['vout']
utxo_amt = float(selected_utxo['amount'])
```

Successivamente, recuperi anche l'indirizzo del destinatario a cui vuoi inviare i bitcoin, calcoli la quantità di bitcoin che vuoi inviare e calcoli la commissione per i miner e l'importo del resto. Qui, l'importo è diviso arbitrariamente in due e una commissione per i miner è impostata arbitrariamente.

```
recipient_address = new_address
recipient_amt = utxo_amt / 2  # sending half coins to recipient
miner_fee = 0.00000300        # choose appropriate fee based on your tx size
change_address = new_change_address
change_amt = float('%.8f'%((utxo_amt - recipient_amt) - miner_fee))
```

> :warning: **ATTENZIONE:** Ovviamente un programma reale farebbe scelte più sofisticate su quale UTXO utilizzare, cosa fare con i fondi e quale commissione per i miner pagare.

### 2. Creare Transazione Grezza

Ora hai tutte le informazioni per inviare una transazione, ma prima di poterlo fare, devi creare una transazione.

```
txids_vouts = [{"txid": utxo_txid, "vout": utxo_vout}]
addresses_amts = {f"{recipient_address}": recipient_amt, f"{change_address}": change_amt}
unsigned_tx_hex = rpc_client.createrawtransaction(txids_vouts, addresses_amts)
```

Ricorda che il formato del comando `createrawtransaction` è:

`$ bitcoin-cli createrawtransaction '[{"txid": <utxo_txid>, "vout": <vector_id>}]' '{"<address>": <amount>}'`

Il `txids_vouts` è quindi un elenco e `addresses_amts` è un dizionario python, per corrispondere al formato di `createrawtransaction`.

Se vuoi vedere di più sui dettagli della transazione che hai creato, puoi usare `decoderawtransaction`, sia in Python 3 che con `bitcoin-cli`.

### 3. Firmare la Transazione Grezza

Firmare una transazione è spesso la parte più complicata dell'invio di una transazione programmaticamente. Qui recuperi una chiave privata da un indirizzo con `dumpprivkey` e la inserisci in un array:

```
address_priv_key = []  # list of priv keys of each utxo
address_priv_key.append(rpc_client.dumpprivkey(utxo_address))
```

Puoi quindi utilizzare quell'array (che dovrebbe contenere le chiavi private di ogni UTXO che viene speso) per firmare il tuo `unsigned_tx_hex`:

```
signed_tx = rpc_client.signrawtransactionwithkey(unsigned_tx_hex, address_priv_key)
```

Questo restituisce un oggetto JSON con l'hex della transazione firmata e se è stata firmata completamente o meno.

### 4. Trasmettere la Transazione

Infine, sei pronto per trasmettere la transazione firmata sulla rete bitcoin:

```
send_tx = rpc_client.sendrawtransaction(signed_tx['hex'])
```

### Eseguire il Codice

Il [codice di esempio](src/18_4_sendtx.py) è pieno di istruzioni `print` per dimostrare tutti i dati disponibili in ogni punto:

```
$ python3 sendtx.py 
Creating a Transaction
---------------------------------------------------------------
Transaction Details:
-------------------
UTXO Address.......:  mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg
UTXO Txid..........:  84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
Vector ID of Output:  0
UTXO Amount........:  1e-05
Tx Amount..........:  5e-06
Recipient Address..:  tb1qca0elxxqzw5xc0s3yq5qhapzzj90ka0zartu6y
Change Address.....:  tb1qrveukqrvqm9h6fua99xvcxgnvdx507dg8e8hrt
Miner Fee..........:  3e-06
Change Amount......:  2e-06
---------------------------------------------------------------

---------------------------------------------------------------
Unsigned Transaction Hex:  02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f20840000000000ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000
---------------------------------------------------------------

---------------------------------------------------------------
Signed Transaction: 
----------------------
{'complete': True,
 'hex': '02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f2084000000006a47304402205da9b2234ea057c9ef3b7794958db6c650c72dedff1a90d2915147a5f6413f2802203756552aba0dd8ebd71b0f28341becc01b28d8b28af063d7c8ce89f9c69167f8012102d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000'}
---------------------------------------------------------------

---------------------------------------------------------------
TXID of sent transaction:  187f8baa222f9f37841d966b6bad59b8131cfacca861cbe9bfc8656bd16a44cc
```

## Sommario: Accesso a Bitcoind con Python

Accedere a Bitcoind con Python è molto facile utilizzando la libreria `python-bitcoinrpc`. La prima cosa da fare è sempre stabilire una connessione con la tua istanza `bitcoind`, poi puoi chiamare tutte le API bitcoin come descritto nella documentazione di bitcoin-core. Questo rende facile creare programmi piccoli o grandi per gestire il tuo nodo, controllare i saldi o creare applicazioni interessanti sopra, accedendo alla piena potenza di `bitcoin-cli`.

## Cosa c'è Dopo?

Scopri di più su "Parlare con Bitcoin in Altri Linguaggi" in [18.5: Accesso a Bitcoind con Rust](18_5_Accessing_Bitcoind_with_Rust.md).

## Variante: Compilare Python da Sorgente

Se hai bisogno di installare Python 3 da sorgente, segui queste istruzioni, poi continua con ["Creare un Progetto BitcoinRPC"](18_4_Accessing_Bitcoind_with_Python.md#create-a-bitcoinrpc-project).

### 1. Installare le Dipendenze

```
$ sudo apt-get install build-essential checkinstall
$ sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev
```

### 2. Scaricare e Estrarre Python

```
$ wget https://www.python.org/ftp/python/3.8.3/Python-3.8.3.tgz
$ tar -xzf Python-3.8.3.tgz
```

### 3. Compilare il Sorgente Python e Controllare l'Installazione:

```
$ cd Python-3.8.3
$ sudo ./configure --enable-optimizations
$ sudo make -j 8  # enter the number of cores of your system you want to use to speed up the build process.
$ sudo make altinstall
$ python3.8 --version
```

Dopo aver ottenuto l'output della versione, rimuovi il file sorgente:

```
$ rm Python-3.8.3.tgz
```
