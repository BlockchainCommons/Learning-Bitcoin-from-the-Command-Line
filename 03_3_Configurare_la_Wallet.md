# 3.3: Configurare la Wallet

Ora sei pronto per iniziare a lavorare con Bitcoin. Per cominciare, dovrai creare una wallet, un portafoglio digitale per inviare e ricevere fondi.

## Crea una wallet

> :warning: **AVVERTIMENTO VERSIONE:** Le versioni più recenti di Bitcoin Core, a partire dalla v0.21.0, non creeranno più automaticamente un portafoglio predefinito all'avvio. Quindi, dovrai crearne uno manualmente. Ma se stai utilizzando una versione precedente di Bitcoin Core, è già stato creato un nuovo portafoglio per te, in quel caso puoi passare a [Creare un indirizzo](#Creare-un-indirizzo).

La prima cosa che devi fare è creare un nuovo portafoglio, cosa che può essere fatta con il comando `bitcoin-cli createwallet`. Creando un nuovo portafoglio, creerai la tua coppia di chiavi pubblica-privata. La tua chiave pubblica è la fonte da cui verranno creati i tuoi indirizzi e la tua chiave privata è ciò che ti consentirà di spendere tutti i fondi che ricevi nei tuoi indirizzi. Bitcoin Core salverà automaticamente tali informazioni in un file `wallet.dat` nella tua directory `~/.bitcoin/testnet3/wallets`.

Se controlli la directory `wallets`, vedrai che al momento è vuota.

```
$ ls ~/.bitcoin/testnet3/wallets
$
```
Anche se Bitcoin Core non creerà un nuovo portafoglio per te, comunque caricherà all'avvio, per impostazione predefinita, una wallet senza nome (""). Quindi puoi trarne vantaggio creando un nuovo portafoglio senza nome.

```
$ bitcoin-cli -named createwallet wallet_name="" descriptors=false

{
  "name": "",
  "warning": ""
}
```

Ora, la tua directory  `wallets` si è riempito.
```
$ ls ~/.bitcoin/testnet3/wallets
database  db.log  wallet.dat
```
> :book: ***Cos'è una wallet Bitcoin?*** Un portafoglio Bitcoin è l'equivalente digitale di un portafoglio fisico sulla rete Bitcoin. Memorizza informazioni sulla quantità di bitcoin che possiedi e dove si trovano (indirizzi), nonché i modi in cui puoi utilizzarli per spenderli. Spendere denaro fisico è intuitivo, ma per spendere bitcoin gli utenti devono fornire la _chiave privata_ corretta. Lo spiegheremo in modo più dettagliato piu avanti nel corso, ma quello che dovresti sapere per ora è che questa dinamica della chiave pubblica-privata fa parte di ciò che rende Bitcoin sicuro e senza bisogno di fiducia in terzi. Le informazioni sulla tua coppia di chiavi vengono salvate nel file `wallet.dat`, oltre ai dati sulle preferenze e sulle transazioni. Nella maggior parte dei casi, non dovrai preoccuparti della chiave privata: `bitcoind` la utilizzerà quando sarà necessaria. Tuttavia, questo rende il file `wallet.dat` estremamente importante: se lo perdi, perdi le tue chiavi private e se perdi le chiavi private, perdi i tuoi fondi!

Ottimo, ora hai un portafoglio Bitcoin. Ma un portafoglio sarà di scarsa utilità per ricevere bitcoin se prima non crei un indirizzo.

> :warning: **AVVERTIMENTO VERSIONE:** A partire da Bitcoin Core v 23.0, i portafogli `Descriptor` sono diventati l'impostazione predefinita. È fantastico, perché i portafogli descrittori sono molto potenti, tranne per il fatto che attualmente non funzionano con multisig! Quindi li disattiviamo con l'argomento `descrptor=false`. Vedi [Capitolo 3.5: Comprendere il Descriptor](03_5_Comprendere_il_Descriptor.md) per ulteriori informazioni.

## Creare un indirizzo

La prossima cosa che devi fare è creare un indirizzo per ricevere i pagamenti. Questo viene fatto con il comando `bitcoin-cli getnewaddress`. Ricorda che se desideri maggiori informazioni su questo comando, devi digitare `bitcoin-cli help getnewaddress`. Attualmente esistono tre tipi di indirizzi: un tipo "legacy" , e due tipi SegWit, cioè "p2sh-segwit" e "bech32". Se non specifichi diversamente, otterrai il valore predefinito, che attualmente è "bech32".

Tuttavia, nelle prossime sezioni utilizzeremo invece indirizzi "legacy", sia perché "bitcoin-cli" ha avuto alcuni problemi iniziali con le sue prime versioni degli indirizzi SegWit, sia perché altre persone potrebbero non essere in grado di inviare a indirizzi "bech32". È improbabile che tutto ciò costituisca un problema per te adesso, ma per il momento vogliamo iniziare con esempi di transazioni che sono (per lo più) garantiti per funzionare.

Innanzitutto, riavvia `bitcoind` in modo che il tuo nuovo portafoglio senza nome ("") venga impostato come predefinito e caricato automaticamente.


```
$ bitcoin-cli stop
Bitcoin Core stopping # wait a minute so it stops completely
$ bitcoind -daemon
Bitcoin Core starting # wait a minute so it starts completely
```

Ora puoi creare un indirizzo. Puoi richiedere un indirizzo `legacy` con il secondo argomento di `getnewaddress` o con l'argomento denominato `addresstype`.

```
$ bitcoin-cli getnewaddress -addresstype legacy
moKVV6XEhfrITAnQCYq6ppT7AaliCOsZ1B
```

Tieni presente che questo indirizzo inizia con una "m" (o talvolta una "n") per indicare un indirizzo Legacy testnet. Sarebbe un "2" per un indirizzo P2SH o un "tb1" per un indirizzo Bech32.

> :link: **TESTNET vs MAINNET:** L'indirizzo mainnet equivalente inizierebbe con "1" (per Legacy), "3" (per P2SH) o "bc1" (per Bech32).

Prendi nota attentamente dell'indirizzo. Dovrai darlo a chi ti invierà i fondi.

> :book: ***Che cos'è un indirizzo Bitcoin?*** Un indirizzo Bitcoin è letteralmente il luogo in cui ricevi denaro. È come un indirizzo email, ma per i fondi. Tecnicamente, è una chiave pubblica, che viene adattata a seconda dei diversi schemi di indirizzi. Tuttavia, a differenza di un indirizzo email, un indirizzo Bitcoin dovrebbe essere considerato monouso: usalo per ricevere fondi solo _una volta_. Quando desideri ricevere fondi da qualcun altro o in un altro momento, genera un nuovo indirizzo. Questo è suggerito in gran parte per migliorare la tua privacy. L'intera blockchain è immutabile, il che significa che gli esploratori possono osservare lunghe catene di transazioni nel tempo, consentendo di determinare statisticamente chi sei tu e i tuoi contatti, non importa quanto tu sia attento. Tuttavia, se continui a riutilizzare lo stesso indirizzo, la cosa diventa ancora più semplice. Creando il tuo primo indirizzo Bitcoin, hai anche iniziato a riempire il tuo portafoglio Bitcoin. Più precisamente, hai iniziato a riempire il file `wallet.dat` nella tua directory `~/.bitcoin/testnet3 /wallets`.

Con un solo indirizzo in mano, puoi passare direttamente alla sezione successiva e iniziare a ricevere fondi. Tuttavia, prima di arrivare a questo, discuteremo brevemente degli altri tipi di indirizzi che incontrerai in futuro e parleremo di alcuni altri comandi della wallet che potresti voler utilizzare in futuro.

### Conoscere i tuoi indirizzi Bitcoin

Esistono tre tipi di indirizzi Bitcoin che puoi creare con il comando RPC `getnewaddress`. Qui utilizzerai un indirizzo "legacy" (P2PKH), mentre passerai a un indirizzo SegWit (P2SH-SegWit) o ​​Bech32 nel [Capitolo 4.6: Creare una Transazione Segwit](04_6_Creare_una_Transazione_Segwit.md).

Come notato sopra, la base di un indirizzo Bitcoin è una chiave pubblica: qualcuno invia fondi alla tua chiave pubblica e poi usi la tua chiave privata per riscattarla. Facile? Tranne che mettere la tua chiave pubblica là fuori non è del tutto sicuro. Al momento, se qualcuno ha la tua chiave pubblica, non può recuperare la tua chiave privata (e quindi i tuoi fondi); questa è la base della crittografia, che utilizza una funzione trap-door per garantire che si possa passare solo dalla chiave privata a quella pubblica e non viceversa. Ma il problema è che non sappiamo cosa ci riserverà il futuro. Solo che sappiamo che i sistemi di crittografia prima o poi vengono danneggiati dall'inarrestabile progresso della tecnologia, quindi è meglio non mettere chiavi pubbliche grezze in rete, per rendere le tue transazioni a prova di futuro.

Le transazioni Bitcoin classiche hanno creato indirizzi P2PKH che hanno aggiunto un ulteriore passaggio crittografico per proteggere le chiavi pubbliche.

> :book: ***Che cos'è un indirizzo Legacy (P2PKH)?*** Questo è un indirizzo Legacy del tipo utilizzato dalla prima rete Bitcoin. Lo useremo negli esempi per le prossime sezioni. Si chiama indirizzo _Pay to PubKey Hash_ (o P2PKH) perché l'indirizzo è un hash a 160 bit di una chiave pubblica. L'utilizzo di un hash della tua chiave pubblica come indirizzo crea un processo in due fasi in cui è necessario rivelare sia la chiave privata che quella pubblica per spendere i fondi, aumenta di conseguenza la sicurezza futura. Questo tipo di indirizzo rimane importante per ricevere fondi da persone con software wallet non aggiornato.

Come descritto più dettagliatamente nel [Capitolo 4.6: Creare una Transazione Segwit](04_6_Creating_a_Segwit_Transaction.md), le guerre sulle dimensioni dei blocchi della fine degli anni 2010 hanno portato a un nuovo tipo di indirizzo: *SegWit*. Questo è il tipo di indirizzo preferito, oggi è completamente integrato in Bitcoin-Core, ma i dettagli li vedremo nel Capitolo 4.6.

SegWit significa semplicemente "testimone separato" ed è un modo per separare le firme delle transazioni dal resto della transazione per ridurre le dimensioni e il costo della transazione. Alcuni indirizzi SegWit si insinueranno in alcuni dei nostri esempi precedenti al Capitolo 4.6 come indirizzi di devoluzione del resto, "change", che vedrai come indirizzi che iniziano con "tb". Questo va bene perché `bitcoin-cli` supporta interamente il loro utilizzo. 

Esistono due indirizzi di questo tipo:

> :book: ***Che cos'è un indirizzo P2SH-SegWit (noto anche come Nested SegWit)?*** Questa è la prima generazione di SegWit. Avvolge l'indirizzo SegWit in un scripthash per garantire la compatibilità con le versioni precedenti. Il risultato crea transazioni più piccole di circa il 25% di Bytes in meno (con corrispondenti riduzioni delle commissioni di transazione).

> :book: ***Cos'è un indirizzo Bech32 (noto anche come Native SegWit, alias P2WPKH)?*** Questa è la seconda generazione di SegWit. È completamente descritto in [BIP 173](https://en.bitcoin.it/wiki/BIP_0173). Crea transazioni ancora più piccole ma, soprattutto, presenta anche alcuni vantaggi nella creazione di indirizzi meno soggetti a errori umani e con una correzione implicita degli errori oltre a ciò. Bech32 *Non* è compatibile con le versioni precedenti come lo era P2SH-SegWit, quindi alcune persone potrebbero non essere in grado di inviar sats a qesti indirizzi.

Esistono altri tipi di indirizzi Bitcoin, come P2PK (che paga a una semplice chiave pubblica ed è deprecato a causa della sua futura insicurezza) e P2SH (che paga a uno Script Hash e che viene utilizzato dal Nested SegWit di prima generazione indirizzi; lo affronteremo più approfonditamente tra qualche capitolo).


## Facoltativo: firma un messaggio

A volte dovrai dimostrare di verificare un indirizzo Bitcoin (o meglio, di verificare la chiave privata). Questo è importante perché fa sapere alle persone che stanno inviando fondi alla persona giusta. Questo può essere fatto creando una firma con il comando `bitcoin-cli signmessage`, nella forma `bitcoin-cli signmessage [indirizzo] [messaggio]`. Per esempio:

```
$ bitcoin-cli signmessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "Hello, World"
HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=
```
Riceverai la firma in cambio.

> :book: ***Cos'è una firma?*** Una firma digitale è una combinazione di un messaggio e di una chiave privata che può poi essere sbloccata con una chiave pubblica. Poiché esiste una corrispondenza uno a uno tra gli elementi di una coppia di chiavi, lo sblocco con una chiave pubblica dimostra che il firmatario controllava la chiave privata corrispondente.

Un'altra persona può quindi utilizzare il comando "bitcoin-cli verifymessage" per verificare la firma. Inserisce l'indirizzo in questione, la firma e il messaggio:

```
$ bitcoin-cli verifymessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
true
```
Se corrispondono tutti, l'altra persona sa che può trasferire in sicurezza i fondi alla persona che ha firmato il messaggio inviandolo all'indirizzo.

Se qualche black hat stesse inventando firme, ciò produrrebbe invece un risultato negativo:
```
$ bitcoin-cli verifymessage "FAKEV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
error code: -3
error message:
Invalid address
```

## Opzionale: scarica il tuo portafoglio

Potrebbe sembrare pericoloso avere tutte le tue insostituibili chiavi private in un unico file. Ecco a cosa serve `bitcoin-cli dumpwallet`. Ti consente di creare una copia del tuo wallet.dat:
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
```
Il file `mywallet.txt` nella tua directory home conterrà un lungo elenco di chiavi private, indirizzi e altre informazioni. Intendiamoci, non vorresti MAI inserire questi dati in un semplice file di testo su una configurazione Bitcoin con fondi reali!

È quindi possibile recuperarlo con `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet ~/mywallet.txt
```
Ma nota che questo richiede un nodo non potato!
```
$ bitcoin-cli importwallet ~/mywallet.txt
error code: -4
error message:
Importing wallets is disabled when blocks are pruned
```

## Facoltativo: visualizza le tue chiavi private

A volte, potresti voler effettivamente guardare le chiavi private associate ai tuoi indirizzi Bitcoin. Forse vuoi poter firmare un messaggio o spendere bitcoin da una macchina diversa. Forse vuoi solo eseguire il backup di alcune importanti chiavi private. Puoi farlo anche con il tuo file di dump, poiché è leggibile dall'uomo.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/standup/mywallet.txt"
}
```
Più probabilmente, vuoi solo guardare la chiave privata associata a un indirizzo specifico. Questo può essere fatto con il comando "bitcoin-cli dumpprivkey".

```
$ bitcoin-cli dumpprivkey "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B"
cTv75T4B3NsG92tdSxSfzhuaGrzrmc1rJjLKscoQZXqNRs5tpYhH
```
Puoi quindi salvare la chiave in un posto sicuro, preferibilmente in un posto non connesso a Internet.

Puoi anche importare qualsiasi chiave privata, da dumpwallet o da un dump della chiave individuale, come segue:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Ancora una volta, aspettatevi che ciò richieda un nodo non potato. Aspettatevi che ciò richieda un po' di tempo, poiché "bitcoind" deve rileggere tutte le transazioni passate, per vedere se ce ne sono di nuove a cui prestare attenzione.

> :information_source: **NOTA:** Molti wallet moderni preferiscono [codici mnemonici](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) per generare i seed necessari per creare il privato chiavi. Questa metodologia non viene utilizzata "bitcoin-cli", quindi non sarai in grado di generare pratici elenchi di parole per ricordare le tue chiavi private.

_Hai digitato quell'indirizzo Bitcoin e firmavi i messaggi e ora scaricavi le chiavi, è *troppo*. Se pensi che sia una seccatura, siamo d'accordo. Ed è anche soggetto a errori, un argomento che affronteremo nella sezione successiva._

## Riepilogo: configurazione la wallet

È necessario creare un indirizzo per ricevere fondi. Il tuo indirizzo è memorizzato in un wallet di cui puoi eseguire il backup. Puoi anche fare molto di più con un indirizzo, come scaricare la sua chiave privata o usarlo per firmare messaggi. Ma in realtà, creare quell'indirizzo è _tutto_ ciò che devi fare per ricevere fondi Bitcoin.

## E ora?

Prenditi un attimo e da un occhiata a le variabili nella linea di comando, qui: [Utilizzare Variabili nella Linea di Comando](03_3_Intermezzo_Utilizzare_Variabili_nella_Linea_di_Comando.md).
