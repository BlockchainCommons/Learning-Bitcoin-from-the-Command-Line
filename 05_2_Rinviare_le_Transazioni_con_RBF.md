# 5.2: Nuovo invio di una transazione con RBF

Se la tua transazione Bitcoin è ferma nella mempool e tu sei il mittente, puoi rinviarla nuovamente utilizzando RBF (replace-by-fee). Tuttavia, non è tutto ciò che RBF può fare: è generalmente una funzionalità potente e multiuso che consente ai mittenti Bitcoin di ricreare transazioni per una serie di motivi.

> :warning: **AVVISO DI VERSIONE:** Questa è un'innovazione di _Bitcoin Core v 0.12.0_, che ha raggiunto la piena maturità nella wallet di Bitcoin Core con la versione v 0.14.0. Ovviamente, la maggior parte delle persone dovrebbe averlo già utilizzato.

## Opzione di uso di RBF

RBF è una funzionalità Bitcoin opzionale. Le transazioni sono idonee all'utilizzo di RBF solo se sono state create con uno speciale flag RBF. Questo viene fatto impostando nel UTXO della transazione un numero di `sequence` qualsiasi (che in genere vengono impostati automaticamente), in modo che sia maggiore di 0 e minore di 0xffffffff-1 (4294967294).

Ciò si ottiene semplicemente aggiungendo una variabile `sequenza` alle tue entrate dell' UTXO:

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.00007658, "'$changeaddress'": 0.00000001 }''')
```
Dovresti firmare e inviare la transazione come al solito:

```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
```
Ora, quando guardi la tua transazione, dovresti vedere qualcosa di nuovo: la riga `bip125-replaceable`, che prima era sempre contrassegnata con "no", ora è contrassegnata con `yes`:

```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375                                                                     
      
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": true,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Il flag `bip125-replaceable` rimarrà `yes` finché la transazione non riceverà conferme. A quel punto non è più sostituibile.

> :book: ***Devo fidarmi delle transazioni senza conferme?*** No, mai. Questo era vero prima della RBF ed era vero dopo la RBF. Le transazioni devono ricevere conferme prima di essere affidabili. Ciò è particolarmente vero se una transazione è contrassegnata come "bip125-replaceable", perché in tal caso può essere... sostituita.

> :information_source: **NOTA — SEQUENCE:** Questo è il primo utilizzo del valore `nSequence` in Bitcoin. Puoi impostarlo tra 1 e 0xffffffff-2 (4294967293) e abilitare RBF, ma se non stai attento potresti imbatterti nell'uso parallelo di `nSequence` per i timelock relativi. Suggeriamo di impostarlo sempre su "1", che è ciò che fa Bitcoin Core, ma l'altra opzione è impostarlo su un valore compreso tra 0xf0000000 (4026531840) e 0xffffffff-2 (4294967293). Impostandolo su "1" rende effettivamente irrilevanti i relativi timelock, mentre impostandolo su 0xf0000000 o superiore li disattiva. Tutto ciò è spiegato ulteriormente nel [Capitolo 11.3: Usare CSV negli Scripts](11_3_Usare_CSV_negli_Scripts.md) dove si parla di `OP_CHECKSEQUENCEVERIFY`. Per ora, scegli semplicemente uno dei valori non in conflitto per "nSequence".

### Facoltativo: aderire sempre a RBF

Se preferisci, puoi _sempre_ optare per RBF. Fallo eseguendo il tuo `bitcoind` con il comando `-walletrbf`. Una volta fatto questo (e riavviato `bitcoind`), tutti gli UTXO dovrebbero avere un numero di sequenza inferiore e la transazione dovrebbe essere contrassegnata come `bip125-replaceable`.

> :warning: **AVVISO DI VERSIONE:** Il flag walletrbf richiede Bitcoin Core v.0.14.0.


## Scopri come funziona RBF

La funzionalità RBF si basa su [BIP 125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki), che elenca le seguenti regole per l'utilizzo di RBF:

> 1. Le transazioni originali segnalano la sostituibilità esplicitamente o tramite ereditarietà come descritto prima.

Ciò significa che il numero di sequenza deve essere impostato su un valore inferiore a 0xffffffff-1. (4294967294), o lo stesso vale per le transazioni principali non confermate.

> 2. L'operazione sostitutiva prevede una commissione in assoluto superiore a quella pagata con l'operazione originaria.
> 3. La transazione sostitutiva non contiene nuovi input non confermati che non apparivano in precedenza nel mempool. (Gli input non confermati sono input che tentano di spendere output da transazioni attualmente non confermate.)
> 4. La transazione sostitutiva deve pagare per la propria larghezza di banda in aggiunta all'importo pagato dalle transazioni originali pari o superiore alla tariffa stabilita dall'impostazione della tariffa di inoltro minima del nodo. Ad esempio, se la tariffa di inoltro minima è 1 satoshi/byte e la transazione sostitutiva è di 500 byte in totale, la sostituzione dovrà pagare una tariffa superiore di almeno 500 satoshi rispetto alla somma degli originali.
> 5. Il numero di transazioni originali da sostituire e le transazioni discendenti che verranno eliminate dalla mempool non devono superare un totale di 100 transazioni.

> :book: ***Che cos'è un BIP?*** Un BIP è una proposta di miglioramento di Bitcoin. È un suggerimento approfondito per una modifica al codice Bitcoin Core. Spesso, quando un BIP è stato sufficientemente discusso e aggiornato, diventerà parte effettiva del codice Bitcoin Core. Ad esempio, BIP 125 è stato implementato in Bitcoin Core 0.12.0.

L’altra cosa da capire su RBF è che per utilizzarlo è necessario spendere due volte, riutilizzando uno o più degli stessi UTXO. Inviare semplicemente un'altra transazione con un UTXO diverso allo stesso destinatario non risolverà il problema (e probabilmente comporterà una perdita di denaro). Invece, devi creare intenzionalmente un conflitto, in cui lo stesso UTXO viene utilizzato in due transazioni diverse.

Di fronte a questo conflitto, i miner sapranno utilizzare quello con la tariffa più alta e saranno incentivati ​​a farlo da quella tariffa più alta.

> :book: ***Che cos'è una doppia spesa?*** Una doppia spesa si verifica quando qualcuno invia gli stessi fondi elettronici a due persone diverse (o, alla stessa persona due volte, in due transazioni diverse). Questo è un problema centrale per qualsiasi sistema di denaro elettronica. In Bitcoin il problema viene risolto dal registro immutabile: una volta che una transazione è sufficientemente confermata, nessun minatore verificherà le transazioni che riutilizzano lo stesso UTXO. Tuttavia, è possibile spendere due volte _prima_ che una transazione venga confermata, motivo per cui desideri sempre una o più conferme prima di finalizzare una transazione. Nel caso di RBF, effettui una doppia spesa intenzionalmente perché una transazione iniziale si è bloccata e i minatori accettano la tua doppia spesa se soddisfi i criteri specifici stabiliti da BIP 125.

> :warning: **WARNING:** Alcune prime discussioni su questa politica suggerivano che anche il numero `nSequence` fosse aumentato. Questo infatti era l'uso previsto di "nSequence" nella sua forma originale. Questo _non_ fa parte della politica pubblicata in BIP 125. Infatti, l'aumento del numero di sequenza può bloccare accidentalmente la transazione con un relativo blocco temporale, a meno che non si utilizzino numeri di sequenza nell'intervallo da 0xf0000000 (4026531840) a 0xffffffff-2 (4294967293) .

## Sostituisci una transazione nel modo più difficile: manualmente

Per creare manualmente una transazione RBF, tutto ciò che devi fare è creare una transazione grezza che: (1) sostituisce una precedente transazione grezza che ha aderito a RBF e che non è confermata; (2) riutilizza uno o più degli stessi UTXO; (3) aumenta le tariffe; e (4) paga la larghezza di banda minima di entrambe le transazioni [che può già essere gestita da (3)].

L'esempio seguente riutilizza semplicemente le nostre variabili esistenti, ma diminuisce l'importo inviato al cambio di indirizzo, per aumentare la commissione dagli 0 BTC accidentali della transazione originale a 0,01 BTC eccessivamente generosi nella nuova transazione:

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.000075, "'$changeaddress'": 0.00000001 }''')
```
Naturalmente dobbiamo firmarlo nuovamente e rispedirlo:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
```
Ora puoi guardare la tua transazione originale e vedere che presenta `walletconflicts`:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Ciò rappresenta il fatto che due transazioni diverse stanno entrambe tentando di utilizzare lo stesso UTXO.

Alla fine, la transazione con la commissione più alta dovrebbe essere accettata:

```
$ bitcoin-cli -named gettransaction txid=c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
{
  "amount": 0.00000000,
  "fee": -0.00000299,
  "confirmations": 2,
  "blockhash": "0000000000000055ac4b6578d7ffb83b0eccef383ca74500b00f59ddfaa1acab",
  "blockheight": 1773266,
  "blockindex": 9,
  "blocktime": 1592955002,
  "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
  "walletconflicts": [
    "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375"
  ],
  "time": 1592954467,
  "timereceived": 1592954467,
  "bip125-replaceable": "no",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b010000000001000000024c1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077dcdd98d85f6247450185c2b918a0f434d9b2e647330d741944ecae60d6ff790220424f85628cebe0ffe9fa11029b8240d08bdbfcc0c11f799483e63b437841b1cd0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Nel frattempo, la transazione originale con la commissione più bassa inizia a raccogliere conferme negative, per mostrare la sua divergenza dalla blockchain:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": -2,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
I nostri destinatari hanno i loro soldi e la transazione originale non riuscita alla fine uscirà dal mempool.

## Sostituisci una transazione nel modo più semplice: con bumpfee

Le transazioni raw sono molto potenti e puoi fare un sacco di cose interessanti combinandole con RBF. Tuttavia, a volte _tutto_ quello che vuoi fare è liberare una transazione che è rimasta in sospeso. Ora puoi farlo con un semplice comando, `bumpfee`.

Ad esempio, per aumentare la commissione della transazione `4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927` dovresti eseguire:

```
$ bitcoin-cli -named bumpfee txid=4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927
{
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "origfee": 0.00000000,
  "fee": 0.00022600,
  "errors": [
  ]
}
```
Il risultato è la generazione automatica di una nuova transazione con una commissione determinata dal file bitcoin.conf:
```
$ bitcoin-cli -named gettransaction txid=75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f
{
  "amount": -0.10000000,
  "fee": -0.00022600,
  "confirmations": 0,
  "trusted": false,
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "walletconflicts": [
    "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927"
  ],
  "time": 1491605676,
  "timereceived": 1491605676,
  "bip125-replaceable": "yes",
  "replaces_txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.00022600,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0020000006b48304502210094e54afafce093008172768d205d99ee2e9681b498326c077f0b6a845d9bbef702206d90256d5a2edee3cab1017b9b1c30b302530b0dd568e4af6f2d35380bbfaa280121029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac38f25c05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
> :warning: **AVVISO DI VERSIONE:** L'RPC `bumpfee` richiede Bitcoin Core v.0.14.0.

## Riepilogo: Reinviare una transazione con RBF

Se una transazione è ferma e non vuoi aspettare che scada del tutto, se hai aderito a RBF, puoi effettuare una doppia spesa utilizzando RBF per creare una transazione sostitutiva (o semplicemente utilizzare `bumpfee`).

> :fire: ***Qual è il potere di RBF?*** Ovviamente, RBF è molto utile se hai creato una transazione con una commissione troppo bassa e hai bisogno di ottenere quei fondi. Tuttavia, la capacità di sostituire generalmente le transazioni non confermate con quelle aggiornate ha più potere di questo (ed è il motivo per cui potresti voler continuare a utilizzare RBF con transazioni raw, anche dopo l'avvento di `bumpfee`).

> Ad esempio, potresti inviare una transazione e quindi, prima che venga confermata, combinarla con una seconda transazione. Ciò consente di comprimere più transazioni in una sola, riducendo le commissioni complessive. Potrebbe anche offrire vantaggi alla privacy. Ci sono anche altri motivi per utilizzare RBF, per contratti intelligenti o cut-through di transazione, come descritto nelle [Domande frequenti su Opt-in RBF](https://bitcoincore.org/en/faq/optin_rbf/).

## Cosa c'è dopo?

Continua con "Controllo delle transazioni Bitcoin" nel [Capitolo 5.3: Pagare una Transaccion con CPFP](05_3_Pagare_una_Transaccion_con_CPFP.md).
