# 20.1: Generating a Payment Request

> :information_source: **NOTE:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Questa sezione descrive come funzionano i pagamenti sulla rete Lightning, come creare una richiesta di pagamento (o _fattura_) e infine come interpretarla. L'emissione delle fatture dipende dall'avere un secondo nodo Lightning, come descritto in [Accedere ad un Secondo Nodo Lightning](19_2_Intermezzo_Accedere_ad_un_Secondo_Nodo_Lightning.md). Questi esempi utilizzeranno un nodo LND come nodo secondario, per dimostrare ulteriormente le possibilità della rete Lightning. Per differenziare tra i nodi in questi esempi, i prompt saranno mostrati come `c$` per il nodo c-lightning e `lnd$` come il nodo LND. Se vuoi riprodurre questi passaggi, dovresti [installare il tuo nodo LND secondario](19_2_Intermezzo_Accedere_ad_un_Secondo_Nodo_Lightning.md#creare-un-nuovo-nodo-lnd).

> :book: ***Cos'è una Fattura?*** Quasi tutti i pagamenti effettuati sulla rete Lightning richiedono una fattura, che non è altro che una **richiesta di pagamento** fatta dal destinatario del denaro e inviata in vari modi all'utente pagante. Tutte le richieste di pagamento sono a uso singolo. Le fatture Lightning utilizzano la codifica bech32, già utilizzata da Segregated Witness per Bitcoin.

## Creare una Fattura

Per creare una nuova fattura su c-lightning dovresti usare il comando `lightning-cli --testnet invoice`.

Ecco come funzionerebbe con c-lightning, utilizzando argomenti di un importo (in millisatoshi), un'etichetta e una descrizione.



```
c$ lightning-cli --testnet invoice 100000 joe-payment "The money you owe me for dinner"
{
   "payment_hash": "07a1c4bd7a38b4dea35f301c173cd8f9aac253b66bd8404d7ad829f226342490",
   "expires_at": 1603305795,
   "bolt11": "lntb1u1p0cw3krpp5q7suf0t68z6dag6lxqwpw0xclx4vy5akd0vyqnt6mq5lyf35yjgqdpj235x2grddahx27fq09hh2gr0wajjqmt9ypnx7u3qv35kumn9wgxqyjw5qcqp2sp5r3puay46tffdyzldjv39fw6tzdgu2hnlszamqhnmgjsuxqxavpgs9qy9qsqatawvx44x5qa22m7td84jau5450v7j6sl5224tlv9k5v7wdygq9qr4drz795lfnl52gklvyvnha5e5lx72lzzmgzcfnp942va5thmhsp5sx7c2",
   "warning_capacity": "No channels",
   "warning_mpp_capacity": "The total incoming capacity is still insufficient even if the payer had MPP capability."
}
```


Tuttavia, per questo esempio genereremo invece una fattura su un nodo LND e poi la pagheremo sul nodo c-lightning. Questo richiede il comando leggermente diverso `addinvoice` di LND. Puoi usare l'argomento `--amt` per indicare l'importo da pagare (in millisatoshi) e aggiungere una descrizione usando l'argomento `--memo`.


```
lnd$ lncli -n testnet addinvoice --amt 10000 --memo "First LN Payment - Learning Bitcoin and Lightning from the Command line."
{
    "r_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
    "payment_request": "lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd",
    "add_index": "1"
}
```


Nota che queste fatture non fanno direttamente riferimento al canale che hai creato: è necessario per il pagamento, ma non per richiedere il pagamento.

## Capire una Fattura

La `bolt11` `payment_request` che hai creato è composta da due parti: una è leggibile dall'uomo e l'altra è costituita da dati.

> :book: **Cos'è un BOLT?** I BOLT sono le singole [specifiche per la rete Lightning](https://github.com/lightningnetwork/lightning-rfc).

### Leggere la Parte Leggibile della Fattura

La parte leggibile della fattura inizia con `ln`. È `lnbc` per Bitcoin mainnet, `lntb` per Bitcoin testnet o `lnbcrt` per Bitcoin regtest.
Poi elenca i fondi richiesti nella fattura.

Ad esempio, guarda la tua fattura dal nodo LND:



```
lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
```


La parte leggibile è `ln` + `tb` + `100u`.

`lntb` dice che questa è una fattura della rete Lightning per Bitcoin Testnet.

`100u` dice che è per 100 bitcoin moltiplicati per il moltiplicatore microsatoshi. Ci sono quattro moltiplicatori di fondi (facoltativi):

* `m` (milli): moltiplica per 0,001
* `u` (micro): moltiplica per 0,000001
* `n` (nano): moltiplica per 0,000000001
* `p` (pico): moltiplica per 0,000000000001

100 BTC * 0,000001 = 0,0001 BTC, che è lo stesso di 10.000 satoshi.

### Leggere la Parte Dati della Fattura

Il resto della fattura (`1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd`) contiene un timestamp, dati specificamente taggati e una firma. Ovviamente non puoi leggerlo da solo, ma puoi chiedere a `lightning-cli` di c-lightning di farlo con il comando `decodepay`:



```
c$ lightning-cli --testnet decodepay lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
{
   "currency": "tb",
   "created_at": 1602702347,
   "expiry": 3600,
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "msatoshi": 10000000,
   "amount_msat": "10000000msat",
   "description": "First LN Payment - Learning Bitcoin and Lightning from the Command line.",
   "min_final_cltv_expiry": 40,
   "payment_secret": "e946403bcb54764994306c743ed3bf1303af8ae6e7687d3cabcc3ee06dbf904d",
   "features": "028200",
   "payment_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
   "signature": "304402202e73ebd1ef974594eb117e8301be781f2ba289041ab6abc558d432351d8365e902202a8151c3fd13419b9390c2b976503d2d064f2a6748b14cb0db64424cf4e572f4"
}

```


Ecco cosa significano gli elementi più rilevanti:

1. `currency`: La valuta che viene pagata.
2. `created_at`: Tempo in cui è stata creata la fattura. Questo è misurato in tempo UNIX, che sono i secondi dal 1970.
3. `expiry`: Il tempo in cui il tuo nodo segna la fattura come non valida. Il valore predefinito è 1 ora o 3600 secondi.
4. `payee`: La chiave pubblica della persona (nodo) che riceve il pagamento della rete Lightning.
5. `msatoshi` e `amount_msat`: La quantità di satoshi da pagare.
6. `description`: La descrizione inserita dall'utente.
7. `payment_hash`: L'hash del preimage che viene utilizzato per bloccare il pagamento. Puoi riscattare un pagamento bloccato solo con il preimage corrispondente all'hash del pagamento. Questo consente il routing sulla rete Lightning senza dover fidarsi di terze parti, creando un **Pagamento Condizionale** da completare.
8. `signature`: La firma codificata DER.

> :book: ***Cosa sono i Pagamenti Condizionali?*** Anche se i canali Lightning sono creati tra due partecipanti, più canali possono essere collegati insieme, formando una rete di pagamento che consente pagamenti tra tutti i partecipanti della rete, anche quelli senza un canale diretto tra loro. Questo viene fatto utilizzando un contratto intelligente chiamato **Hashed Time Locked Contract**.

> :book: ***Cos'è un Hashed Time Locked Contract (HTLC)?*** Un Hashed Time Locked Contract è un pagamento condizionato che utilizza hashlock e timelock per garantire la sicurezza del pagamento. Il destinatario deve presentare un preimage di pagamento o generare una prova crittografica del pagamento prima di un determinato tempo, altrimenti il pagatore può annullare il contratto spendendolo. Questi contratti sono creati come output dalla **Commitment Transaction**.

> :book: ***Cos'è una Commitment Transaction?*** Una Commitment Transaction è una transazione che spende la transazione di finanziamento originale. Ogni peer detiene la firma dell'altro peer, il che significa che entrambi possono spendere la loro commitment transaction quando vogliono. Dopo che ogni nuova commitment transaction viene creata, quella vecchia viene revocata. La commitment transaction è un modo in cui la transazione di finanziamento può essere sbloccata sulla blockchain, come discusso nel [Capitolo 20.3](20_3_Chiudere_un_Canale_Lightning.md).

### Controlla la Tua Fattura

Ci sono due elementi cruciali da controllare nella fattura. Il primo, ovviamente, è l'importo del pagamento, che hai già esaminato nella parte leggibile. Il secondo è il valore `payee`, che è il pubkey del destinatario (nodo):



```
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
```


Devi verificare che sia il destinatario previsto.

Guardando indietro nel [Capitolo 20.3](20_3_Chiudere_un_Canale_Lightning.md), puoi vedere che è effettivamente l'ID del peer che hai utilizzato quando hai creato il tuo canale. Potresti anche verificarlo sul nodo opposto con il comando `getinfo`.



```
lnd$ lncli -n testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 1,
    "num_inactive_channels": 0,
    "num_peers": 3,
    "block_height": 1862983,
    "block_hash": "00000000000000c8c2f58f6da2ae2a3884d6e84f55d0e1f585a366f9dfcaa860",
    "best_header_timestamp": "1602702331",
    "synced_to_chain": true,
    "synced_to_graph": true,
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

Tuttavia, il `payee` potrebbe anche essere qualcuno di nuovo, nel qual caso probabilmente dovrai verificare con il sito web o la persona che ha emesso la fattura per assicurarti che sia corretta.

## Sommario: Generare una Richiesta di Pagamento

Nella maggior parte dei casi è necessario ricevere una fattura per utilizzare i pagamenti della rete Lightning. In questo esempio ne abbiamo creata una manualmente, ma se avessi un ambiente di produzione, probabilmente avresti sistemi che lo fanno automaticamente ogni volta che qualcuno acquista prodotti o servizi. Ovviamente, una volta ricevuta una fattura, devi sapere come leggerla!

## Cosa Succede Dopo?

Continua a "Using Lightning" col [Capitolo 20.2: Pagare una Fattura](20_2_Pagare_una_Fattura.md).
