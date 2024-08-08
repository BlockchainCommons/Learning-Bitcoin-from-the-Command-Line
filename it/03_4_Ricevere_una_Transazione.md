# 3.4: Ricevere una Transazione

Ora sei pronto per ricevere denaro al nuovo indirizzo che hai impostato.

## Ottieni dei soldi

Per fare qualcosa di più, devi ottenere dei soldi. Su testnet questo avviene tramite faucet. Dato che i soldi sono tutti finti, basta andare a un rubinetto, chiedere dei soldi e ti verranno inviati. Ti consigliamo di utilizzare il faucet su https://testnet-faucet.mempool.co/, https://bitcoinfaucet.uo1.net/ o https://testnet.coinfaucet.eu/en/. Se per qualche motivo non sono disponibili, cerca "bitcoin testnet faucet" e dovresti trovarne altri.

Per utilizzare un faucet, di solito dovrai accedere a un URL e copiare e incollare il tuo indirizzo. Tieni presente che questo è uno di quei casi in cui non sarai in grado di utilizzare le variabili della riga di comando, ahimè. Successivamente, verrà creata una transazione che ti invierà denaro dal faucet.

> :book: ***Cos'è una transazione?*** Una transazione è uno scambio di bitcoin. Il proprietario di alcuni bitcoin utilizza la sua chiave privata per accedere a quelle monete, quindi blocca la transazione utilizzando la chiave pubblica del destinatario.

> :link: **TESTNET vs MAINNET:** Purtroppo, non ci sono faucet nella vita reale. Se giocassi sulla rete principale, dovresti andare ad acquistare effettivamente bitcoin presso uno scambio bitcoin o un bancomat, oppure dovresti trovare qualcuno che te li invii. La vita su Testnet è molto più semplice.

## Verifica i tuoi soldi

Dopo aver richiesto il tuo denaro, dovresti essere in grado di verificarlo con il comando `bitcoin-cli getbalance`:

```
$ getbalance bitcoin-cli
0.00000000
```
Ma aspetta, non ci sono ancora?

Benvenuti nel mondo della latenza Bitcoin. Il problema è che la tua transazione non è stata ancora registrata in un blocco!

> :book: ***Cos'è un blocco?*** Le transazioni vengono trasmesse attraverso la rete e raccolte in blocchi dai minatori. Questi blocchi sono protetti con una prova di lavoro matematica, che dimostra che la potenza di calcolo è stata spesa come parte della creazione del blocco. È quella prova di lavoro (moltiplicata su molti blocchi, ciascuno costruito sopra l'ultimo) che alla fine mantiene Bitcoin sicuro.

> :book: ***Cos'è un miner?*** Un miner è un partecipante nella rete Bitcoin che lavora per creare blocchi. È un lavoro retribuito: quando un minatore crea con successo un blocco, riceve una ricompensa una tantum più le commissioni per le transazioni nel suo blocco. L’estrazione mineraria è un grande business. I minatori tendono a funzionare su hardware speciale, accelerato in modi che rendono più probabile che siano in grado di creare blocchi. Tendono anche a far parte dei pool minerari, dove i minatori accettano tutti di condividere i premi quando uno di loro crea con successo un blocco.

Fortunatamente, `bitcoin-cli getunconfirmedbalance` dovrebbe comunque mostrare il saldo aggiornato purché sia ​​stata creata la transazione iniziale:
```
$ bitcoin-cli getunconfirmedbalance
0.01010000
```
Se anche questo mostra ancora uno zero, probabilmente stai procedendo troppo velocemente attraverso questo tutorial. Aspetta un secondo. Le monete dovrebbero apparire non confermate, quindi passare rapidamente a confermate. Tieni presente che una moneta può passare dal saldo non confermato al saldo confermato quasi immediatamente, quindi assicurati di controllarli entrambi. Tuttavia, se il tuo `getbalance` e il tuo `getunconfirmedbalance` mostrano ancora zero entro dieci minuti, probabilmente c'è qualcosa che non va con il faucet e dovrai sceglierne un altro.

### Aumenta la confidenza col tuo denaro

Puoi utilizzare `bitcoin-cli getbalance "*" [n]`, dove sostituisci `[n]` con un numero intero, per vedere se un saldo confermato è profondo 'n' blocchi.

> :book: ***Cos'è la profondità del blocco?*** Dopo che un blocco è stato costruito e confermato, un altro blocco viene costruito sopra di esso, e un altro... Poiché si tratta di un processo stocastico, c'è qualche possibilità di inversione quando un blocco è ancora nuovo. Pertanto, un blocco deve essere sepolto diversi blocchi in profondità in una catena prima che tu possa sentirti totalmente sicuro dei tuoi fondi. Ciascuno di questi blocchi tende ad essere costruito in una media di 10 minuti... quindi di solito ci vuole circa un'ora affinché una transazione confermata riceva sei blocchi di profondità, che è la misura della piena fiducia in Bitcoin.

Quanto segue mostra che le nostre transazioni sono state confermate una volta, ma non due volte:
```
$  bitcoin-cli getbalance "*" 1
0.01010000
$  bitcoin-cli getbalance "*" 2
0.00000000
```
Ovviamente, ogni dieci minuti circa questa profondità aumenterà.

Naturalmente, sul testnet, nessuno è così preoccupato dell'affidabilità dei tuoi fondi. Potrai spendere i tuoi soldi non appena sarà confermato.

## Verifica il tuo portafoglio

Il comando `bitcoin-cli getwalletinfo` ti fornisce maggiori informazioni sul saldo del tuo portafoglio:

```
$ bitcoin-cli getwalletinfo
{
  "walletname": "",
  "walletversion": 169900,
  "balance": 0.01010000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 2,
  "keypoololdest": 1592335137,
  "keypoolsize": 999,
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "keypoolsize_hd_internal": 1000,
  "paytxfee": 0.00000000,
  "private_keys_enabled": true,
  "avoid_reuse": false,
  "scanning": false
}
```

## Scopri la ID della transazione

I tuoi soldi sono entrati nel tuo portafoglio tramite una transazione. Puoi scoprire la ID della  transazione (txid) con il comando "bitcoin-cli listtransactions":

```
$ bitcoin-cli listtransactions
[
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.01000000,
    "label": "",
    "vout": 1,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 73,
    "blocktime": 1592600085,
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "walletconflicts": [
    ],
    "time": 1592599884,
    "timereceived": 1592599884,
    "bip125-replaceable": "no"
  },
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.00010000,
    "label": "",
    "vout": 0,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 72,
    "blocktime": 1592600085,
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "walletconflicts": [
    ],
    "time": 1592599938,
    "timereceived": 1592599938,
    "bip125-replaceable": "no"
  }
]

```
Questo mostra due transazioni (`8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9`) e (`ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9 f36`) per un importo specifico (`0.01000000` e `0.00010000`), entrambi ricevuti ("receive") dallo stesso indirizzo nel nostro portafoglio (`mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE`) . A proposito, questa è unapratica sconsigliata: dovresti usare un nuovo indirizzo per ogni singola transazione Bitcoin che ricevi. In questo caso ci siamo spazientiti perché il primo rubinetto sembrava non funzionare.

Puoi accedere a informazioni simili con il comando `bitcoin-cli listunspent`, ma mostra solo le transazioni per i soldi che non hai speso. Questi sono chiamati UTXO e saranno di vitale importanza quando invierai denaro nel mondo Bitcoin:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "vout": 1,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  }
]
```
Tieni presente che i bitcoin non è un disordine di soldi buttati nelle tue tasche. Ogni singola transazione che ricevi o che invii viene inserita nel registro immutabile della blockchain, in un blocco. Puoi vedere queste singole transazioni quando guardi i tuoi soldi non spesi. Ciò significa che la spesa in bitcoin non è così anonima come potresti pensare. Sebbene gli indirizzi siano abbastanza privati, le transazioni possono essere esaminate mentre entrano ed escono dagli indirizzi. Ciò rende la privacy vulnerabile all’analisi statistica. Introduce inoltre una certa potenziale non fungibilità per i bitcoin, poiché è possibile risalire a una serie di transazioni, anche se non è possibile tracciare uno specifico "bitcoin".

> :book: ***Perché tutti questi importi di bitcoin sono frazionati?*** I bitcoin vengono prodotti lentamente, quindi ce ne sono relativamente pochi in circolazione. Di conseguenza, ogni bitcoin sulla rete principale vale parecchio (~ $ 60.000 al momento in cui scriviamo). Ciò significa che le persone di solito lavorano in frazioni. In effetti, il 0.0101 nelle monete Testnet varrebbe circa $ 600 se fossero sulla rete principale. Per questo motivo, sono apparsi nomi per quantità minori di bitcoin, inclusi millibitcoin o mBTC (un millesimo di bitcoin), microbitcoin o bit o μBTC (un milionesimo di bitcoin) e `satoshi` (centomilionesimo di bitcoin). .

## Esamina la tua transazione

Puoi ottenere maggiori informazioni su una transazione con il comando  `bitcoin-cli gettransaction`.:

```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9"
{
  "amount": 0.01000000,
  "confirmations": 1,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00"
}
```
Il comando `gettransaction` dettaglierà le transazioni presenti nel tuo portafoglio, come questa, che ci è stata inviata.

Tieni presente che `gettransaction` ha due argomenti facoltativi:
```
$ bitcoin-cli help gettransaction
gettransaction "txid" ( include_watchonly verbose )

Get detailed information about in-wallet transaction <txid>

Arguments:
1. txid                 (string, required) The transaction id
2. include_watchonly    (boolean, optional, default=true for watch-only wallets, otherwise false) Whether to include watch-only addresses in balance calculation and details[]
3. verbose              (boolean, optional, default=false) Whether to include a `decoded` field containing the decoded transaction (equivalent to RPC decoderawtransaction)
```
Impostando questi due su true o false, possiamo scegliere di includere indirizzi watch only nell'output (cosa che non ci interessa) o guardare un risultato più dettagliato (cosa che facciamo).

Ecco cosa esaminano invece questi dati quando impostiamo `include_watchonly` su `false` e `verbose` su `true`.
```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9" false true
{
  "amount": 0.01000000,
  "confirmations": 3,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00",
  "decoded": {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "hash": "d4ae2b009c43bfe9eba96dcd16e136ceba2842df3d76a67d689fae5975ce49cb",
    "version": 2,
    "size": 249,
    "vsize": 168,
    "weight": 669,
    "locktime": 1772327,
    "vin": [
      {
        "txid": "80f57b9779a769eb8772faf319265a46b94499d7d51df5db7a13b0d17749d014",
        "vout": 1,
        "scriptSig": {
          "asm": "0014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f",
          "hex": "160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f"
        },
        "txinwitness": [
          "304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e13301",
          "022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c"
        ],
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01010143,
        "n": 0,
        "scriptPubKey": {
          "asm": "OP_HASH160 5c3bfb36b03f279967977ca9d1e35185e3991778 OP_EQUAL",
          "hex": "a9145c3bfb36b03f279967977ca9d1e35185e399177887",
          "reqSigs": 1,
          "type": "scripthash",
          "addresses": [
            "2N1ev1WKevSsdmAvRqZf7JjvDg223tPrVCm"
          ]
        }
      },
      {
        "value": 0.01000000,
        "n": 1,
        "scriptPubKey": {
          "asm": "OP_DUP OP_HASH160 1b72503639a13f190bf79acf6d76255d772360b7 OP_EQUALVERIFY OP_CHECKSIG",
          "hex": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
          "reqSigs": 1,
          "type": "pubkeyhash",
          "addresses": [
            "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE"
          ]
        }
      }
    ]
  }
}
```
Ora puoi vedere tutte le informazioni sulla transazione, inclusi tutti gli input (`vin`) e tutti gli output (`vout`). Una delle cose interessanti da notare è che, sebbene abbiamo ricevuto 0,01 BTC nella transazione, un altro .01010143 è stato inviato a un altro indirizzo. Probabilmente si trattava di un indirizzo di cambio o resto, un concetto che verrà esplorato nella sezione successiva. È abbastanza tipico che una transazione abbia più input e/o più output.

Esiste un altro comando, `getrawtransaction`, che ti consente di esaminare le transazioni che non sono nel tuo portafoglio. Tuttavia, richiede che tu abbia un nodo non potato e `txindex=1` nel tuo file `bitcoin.conf`. A meno che tu non abbia un serio bisogno di informazioni che non sono nel tuo portafoglio, probabilmente è meglio usare un Bitcoin Explorer per questo genere di cose...

## Facoltativo: utilizza un Block Explorer

Anche guardare le informazioni dettagliate di una transazione può essere un po’ intimidatorio. L'obiettivo principale di questo tutorial è insegnare come gestire le transazioni grezze dalla riga di comando, ma saremo felici di parlare di altri strumenti quando applicabili. Uno di questi strumenti è un block explorer, che puoi utilizzare per esaminare le transazioni da un browser web in un formato molto più intuitivo.

Attualmente, il nostro block explorer preferito è [mempool.space](https://mempool.space).

Ecco un'altra alternativa,  [https://live.blockcypher.com/](https://live.blockcypher.com/).

Puoi usarlo per cercare le transazioni per un indirizzo:

[https://mempool.space/it/testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE](https://mempool.space/it/testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE)

[https://live.blockcypher.com/btc-testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE/](https://live.blockcypher.com/btc-testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE/)

Puoi anche usarlo per esaminare le singole transazioni:

[https://mempool.space/it/testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9](https://mempool.space/it/testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9)

[https://live.blockcypher.com/btc-testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9/](https://live.blockcypher.com/btc-testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9/)

Un block explorer generalmente non fornisce più informazioni di quelle che una riga di comando esamina in una transazione grezza; fa semplicemente un buon lavoro nell'evidenziare le informazioni importanti e nel mettere insieme i pezzi del puzzle, comprese le commissioni di transazione dietro una transazione, un altro concetto che tratteremo nelle sezioni future.

## Riepilogo: ricezione di una transazione

I rubinetti ti daranno soldi sul testnet. Arrivano come transazioni grezze, che possono essere esaminate con `gettransaction` o un block explorer. Una volta ricevuta una transazione, puoi visualizzare il saldo nel tuo portafoglio.

## Qual è il prossimo?

Per un approfondimento su come vengono descritti gli indirizzi, in modo che possano essere trasferiti o creati in parti in una multisig, vedere [Capitolo 3.5: Comprendere il Descriptor](03_5_Comprendere_il_Descriptor.md).

Ma se è approfondire un po troppo, salta e vai al [Capitolo 4: Inviare Transazioni Bitcoin](04_0_Inviare_Transazioni_Bitcoin.md).
