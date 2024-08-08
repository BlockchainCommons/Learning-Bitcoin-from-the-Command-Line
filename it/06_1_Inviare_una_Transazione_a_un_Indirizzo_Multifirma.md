# 6.1: Inviare di una transazione con un Multisig

Il primo modo per variare la modalità di invio di una transazione di base è utilizzare un multisig. Ciò ti dà la possibilità di richiedere che più persone (o almeno più chiavi private) autorizzino l'uso dei fondi.

## Come funziona il Multisig:

In una tipica transazione P2PKH o SegWit, i bitcoin vengono inviati a un indirizzo basato sulla tua chiave pubblica, il che a sua volta significa che è necessaria la relativa chiave privata per sbloccare la transazione, risolvendo il puzzle crittografico e permettendoti di riutilizzare i fondi. Ma cosa succederebbe se potessi invece bloccare una transazione con chiavi private _multiple_? Ciò consentirebbe effettivamente di inviare fondi a un gruppo di persone, dove tutte queste persone devono accettare di riutilizzare i fondi.

> :book: ***Cos'è una multifirma?*** Una multifirma è una metodologia che consente a più di una persona di creare congiuntamente una firma digitale. È una tecnica generale per l'uso crittografico delle chiavi che va ben oltre Bitcoin.

Tecnicamente, un puzzle crittografico multifirma viene creato da Bitcoin utilizzando il comando `OP_CHECKMULTISIG` e in genere è incapsulato in un indirizzo P2SH. [Capitolo 10.4 Programmare una Multifirma](10_4_Programmare_una_Multifirma.md) spiegherà in dettaglio come funziona in modo più preciso. Per ora, tutto ciò che devi sapere è che puoi utilizzare il comando `bitcoin-cli` per creare indirizzi multifirma; i fondi possono essere inviati a questi indirizzi proprio come qualsiasi normale indirizzo P2PKH o Segwit, ma saranno necessarie più chiavi private per il riscatto dei fondi.

> :book: ***Cos'è una transazione multifirma?*** Una transazione multifirma è una transazione Bitcoin che è stata inviata a un indirizzo multifirma, richiedendo quindi le firme di alcune persone del gruppo multifirma per riutilizzare i fondi.

Le multifirme semplici richiedono che tutti i membri del gruppo firmino l'UTXO quando è speso. Tuttavia, è possibile una maggiore complessità. Le multifirme sono generalmente descritte come "m di n". Ciò significa che la transazione è bloccata con un gruppo di "n" chiavi, ma solo "m" di esse sono necessarie per sbloccare la transazione.

> :book: ***Cos'è una multifirma m-di-n?*** In una multifirma, sono richieste "m" firme da un gruppo di "n" per formare la firma, dove "m ≤ n".

## Crea un indirizzo multisig

Per bloccare un UTXO con più chiavi private, devi prima creare un indirizzo multifirma. Gli esempi qui utilizzati mostrano la creazione (e l'utilizzo) di una multifirma 2 su 2.

### Creare gli indirizzi

Per creare un indirizzo multifirma, devi prima preparare gli indirizzi che il multisig combinerà. La best practice suggerisce di creare sempre nuovi indirizzi. Ciò significa che i partecipanti eseguiranno ciascuno il comando `getnewaddress` sul proprio computer:
```
machine1$ address1=$(bitcoin-cli getnewaddress)
```
e:
```
machine2$ address2=$(bitcoin-cli getnewaddress)
```
Successivamente, uno dei destinatari (o forse una terza parte) dovrà combinare gli indirizzi.

#### Raccogli le chiavi pubbliche

Tuttavia, non è possibile creare un multi-sig con gli indirizzi, poiché questi sono gli hash delle chiavi pubbliche: sono invece necessarie le chiavi pubbliche stesse.

Queste informazioni sono facilmente disponibili con il comando "getaddressinfo".

Sulla macchina remota, che qui presumiamo sia "macchina2", puoi ottenere le informazioni dall'elenco. 
```
machine2$ bitcoin-cli -named getaddressinfo address=$address2
{
  "address": "tb1qr2tkjh8rs9xn5xaktf5phct0wxqufplawrfd9q",
  "scriptPubKey": "00141a97695ce3814d3a1bb65a681be16f7181c487fd",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([fe6f2292/0'/0'/1']02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3)#zc64l8dw",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "1a97695ce3814d3a1bb65a681be16f7181c487fd",
  "pubkey": "02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3",
  "ischange": false,
  "timestamp": 1592957904,
  "hdkeypath": "m/0'/0'/1'",
  "hdseedid": "1dc70547f2b80e9bb5fde5f34fb3d85f8d8d1dab",
  "hdmasterfingerprint": "fe6f2292",
  "labels": [
    ""
  ]
}
```
L'indirizzo `pubkey` (`02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3`) è ciò che è richiesto. Copialo sul tuo computer locale con qualunque mezzo ritieni più efficiente e _meno soggetto a errori_.

Questo processo deve essere intrapreso per _ogni_ indirizzo da una macchina diversa da quella su cui viene costruito il multisig. Ovviamente, se qualche terza parte sta creando l'indirizzo, dovrai farlo per ogni indirizzo.

> :avviso: **ATTENZIONE:** l'uso da parte di Bitcoin di hash di chiave pubblica come indirizzi, invece di chiavi pubbliche, rappresenta in realtà un ulteriore livello di sicurezza. Pertanto, l'invio di una chiave pubblica aumenta leggermente la vulnerabilità dell'indirizzo associato, per qualche possibilità futura di compromissione della curva ellittica. Non dovresti preoccuparti di dover inviare occasionalmente una chiave pubblica per un utilizzo come questo, ma dovresti essere consapevole che gli hash della chiave pubblica rappresentano la sicurezza e quindi le chiavi pubbliche effettive non dovrebbero essere inviate in giro, volenti o nolenti.

Se uno degli indirizzi è stato creato sul tuo computer locale, che qui presumiamo sia `machine1`, puoi semplicemente scaricare l'indirizzo `pubkey` in una nuova variabile.
```
machine1$ pubkey1=$(bitcoin-cli -named getaddressinfo address=$address1 | jq -r '.pubkey')
```

### Crea l'indirizzo

Ora è possibile creare un indirizzo multisig con il comando `createmultisig`:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3"]'''
{
  "address": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "redeemScript": "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
  "descriptor": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y"
}
```
> :warning: **AVVERTIMENTO VERSIONE:** Alcune versioni di `createmultisig` consentono l'immissione di chiavi o indirizzi pubblici, altre richiedono solo chiavi pubbliche. Attualmente, sembra che entrambi siano consentiti.

Quando crei l'indirizzo multifirma, elenchi quante firme sono richieste con l'argomento `nrequired` (ovvero "m" in una multifirma "m-of-n"), quindi elenchi l'insieme totale di firme possibili con le `keys` argomento (che è "n"). Tieni presente che le voci "chiavi" probabilmente provengono da luoghi diversi. In questo caso, abbiamo incluso `$pubkey1` dal computer locale e `02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3` da un computer remoto.

> :information_source: **NOTA — M-OF-N VS N-OF-N:** Questo esempio mostra la creazione di un semplice multisig 2 di 2. Se invece desideri creare una firma m-of-n dove "m < n", modifica il campo `nrequired` e/o il numero di firme nell'oggetto JSON `keys`. Per un multisig 1 su 2, imposteresti `nrequired=1` ed elencheresti anche due chiavi, mentre per un multisig 2 su 3, lasceresti `nrequired=2`, ma aggiungerai un'altra chiave pubblica all'elenco delle "chiavi".

Se utilizzato correttamente, `createmultisig` restituisce tre risultati, tutti di fondamentale importanza.

L'_indirizzo_ è ciò che darai alle persone che vogliono inviare fondi. Noterai che ha un nuovo prefisso "2", esattamente come gli indirizzi P2SH-SegWit. Questo perché, come loro, `createmultisig` sta in realtà creando un tipo di indirizzo totalmente nuovo chiamato indirizzo P2SH. Funziona esattamente come un indirizzo P2PKH standard per l'invio di fondi, ma poiché questo è stato creato per richiedere più indirizzi, dovrai fare un po' più di lavoro per spenderli.

> :link: **TESTNET vs MAINNET:** Su testnet, il prefisso per gli indirizzi P2SH è "2", mentre su mainnet è "3".

Il _redeemScript_ è ciò di cui hai bisogno per riscattare i fondi (insieme alle chiavi private per "m" degli "n" indirizzi). Questo script è un'altra caratteristica speciale degli indirizzi P2SH e verrà spiegato completamente nel [Capitolo 10.3 Eseguire un Script Bitcoin con P2SH](10_3_Eseguire_un_Script_Bitcoin_con_P2SH.md). Per ora, tieni presente che sono necessari alcuni dati per ottenere i tuoi soldi.

Il _descrittore_ è la descrizione standardizzata di un indirizzo che abbiamo incontrato nel [Capitolo 3.5 Comprendere il Descriptor](03_5_Comprendere_il_Descriptor.md). Fornisce un modo per importare nuovamente questo indirizzo sull'altra macchina, utilizzando l'RPC "importmulti".

> :book: ***Cos'è un indirizzo P2SH?*** P2SH sta per Pay-to-script-hash. Si tratta di un tipo di destinatario diverso da un indirizzo P2PKH standard o anche da un Bech32, utilizzato per fondi il cui riscatto si basa su script Bitcoin più complessi. `bitcoin-cli` utilizza l'incapsulamento P2SH per aiutare a standardizzare e semplificare i suoi multisig come `multisig P2SH`, proprio come P2SH-SegWit utilizzava P2SH per standardizzare i suoi indirizzi SegWit e renderli completamente compatibili con le versioni precedenti.

> :avviso: **ATTENZIONE:** Gli indirizzi multisig P2SH, come quelli creati da `bitcoin-cli`, hanno un limite per "m" e "n" in multisig in base alla dimensione massima dello script di riscatto, che è attualmente 520 byte. In pratica, non lo raggiungerai a meno che tu non stia facendo qualcosa di eccessivo.

### Salva il tuo lavoro

Ecco un avvertimento importante: nulla del tuo multisig viene salvato nel tuo portafoglio utilizzando queste tecniche di base. Per riscattare successivamente il denaro inviato a questo indirizzo multifirma, dovrai conservare due informazioni cruciali:

 * Un elenco degli indirizzi Bitcoin utilizzati nel multisig.
 * L'output `redeemScript` di `createmultsig`.

Tecnicamente, `redeemScript` può essere ricreato eseguendo nuovamente `createmultisig` con l'elenco completo delle chiavi pubbliche _nello stesso ordine_ e con il conteggio m-of-n corretto. Ma è meglio conservarlo e risparmiarti stress e dolore.

### Guarda l'Ordine

C'è una cosa di cui essere molto cauti: _l'ordine conta_. L'ordine delle chiavi utilizzate per creare un multi-sig crea un hash univoco, vale a dire se inserisci le chiavi in ​​un ordine diverso, produrranno un indirizzo diverso, come mostrato:
```
$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]'''
{
  "address": "2NFBQvz57UzKWDr2Vx5D667epVZifjGixkm",
  "redeemScript": "52210342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da0321039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b352ae",
  "descriptor": "sh(multi(2,0342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da03,039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3))#8l6hvjsk"
}
$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey2'","'$pubkey1'"]'''
{
  "address": "2N5bC4Yc5Pqept1y8nPRqvWmFSejkVeRb1k",
  "redeemScript": "5221039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3210342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da0352ae",
  "descriptor": "sh(multi(2,039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3,0342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da03))#audl88kg"
}
```
Più in particolare, ogni ordinamento crea un _redeemScript_ diverso. Ciò significa che se hai utilizzato queste tecniche di base e non sei riuscito a salvare il _redeemScript_ come ti è stato detto, dovrai passare attraverso un numero sempre crescente di varianti per trovare quella giusta quando provi a spendere i tuoi fondi!

[BIP67](https://github.com/bitcoin/bips/blob/master/bip-0067.mediawiki) suggerisce un modo per ordinare lessicograficamente le chiavi, in modo che generino sempre le stesse multifirme. ColdCard ed Electrum sono tra i portafogli che già lo supportano. Naturalmente, questo può causare problemi di per sé se non si sa se un indirizzo multisig è stato creato con chiavi ordinate o non ordinate. Ancora una volta, il [descriptor](03_5_Comprendere_il_Descriptor.md) viene in soccorso. Se un multisig non è ordinato, viene creato con la funzione "multi" e se è ordinato viene creato con la funzione "sortedmulti".

Se guardi il `descriptor` per il multisig che hai creato sopra, vedrai che Bitcoin Core attualmente non ordina i suoi multisig:
```
 "descrittore": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e 78df82f33fa3))#0pazcr4y"
```
Tuttavia, se importa un indirizzo con tipo `sortedmulti`, farà la cosa giusta, che è il punto centrale dei descrittori!

> :warning: **AVVISO VERSIONE:** Bitcoin Core comprende la funzione del descrittore `sortedmulti` dalla versione v 0.20.0 in poi. Prova ad accedere al descrittore su una versione precedente di Bitcoin Core e riceverai un errore come "È necessaria una funzione all'interno di P2WSH".

## Inviare a un indirizzo multisig

Se hai una multifirma in un comodo formato P2SH, come quello generato da `bitcoin-cli`, può essere inviata esattamente come un normale indirizzo.
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.000065}''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "hash": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "version": 2,
  "size": 83,
  "vsize": 83,
  "weight": 332,
  "locktime": 0,
  "vin": [
    {
      "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00006500,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
    }
  ]
}

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521
```
Come puoi vedere, non c'era nulla di insolito nella creazione della transazione, e sembrava del tutto normale, anche se con un indirizzo con un prefisso diverso dal normale (`2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr`). Nessuna sorpresa, poiché allo stesso modo non abbiamo notato alcuna differenza quando abbiamo inviato per la prima volta agli indirizzi Bech32 nel [Capitolo 4.6](04_6_Creare_una_Transazione_Segwit.md).

## Riepilogo: inviare una transazione a un indirizzo Multisig

Gli indirizzi Multisig bloccano i fondi su più chiavi private, possibilmente richiedendo tutte quelle chiavi private per il riscatto e forse richiedendone solo alcune dal set. Sono abbastanza facili da creare con `bitcoin-cli` ed è del tutto normale inviarli. Questa facilità è dovuta in gran parte all'uso invisibile degli indirizzi P2SH (pay-to-script-hash), un argomento ampio che abbiamo toccato due volte, con gli indirizzi P2SH-SegWit e multisig, e che otterrà di più copertura in futuro.

> :fire: ***Qual è il potere delle multifirme?*** Le multifirme consentono la modellazione di una varietà di accordi finanziari come società, partenariati, comitati e altri gruppi. Un multisig 1 su 2 potrebbe essere il conto bancario congiunto di una coppia sposata, mentre un multisig 2 su 2 potrebbe essere utilizzato per grandi spese da parte di una società a responsabilità limitata. Le multifirme costituiscono anche una delle basi degli Smart Contracts. Ad esempio, un affare immobiliare potrebbe essere chiuso con un multisig 2 su 3, in cui le firme vengono inviate dall'acquirente, dal venditore e da un agente di deposito a garanzia autorizzato. Una volta che l'agente di deposito a garanzia concorda che tutte le condizioni sono state soddisfatte, libera i fondi per il venditore; o in alternativa, l'acquirente e il venditore possono liberare congiuntamente i fondi.

## Cosa viene dopo?

Continua "Espansione delle transazioni Bitcoin" con [Capitolo 6.2: Spendere una Transazione Multifirma.md](06_2_Spendere_una_Transazione_con_un_Indirizzo_Multifirma.md).
