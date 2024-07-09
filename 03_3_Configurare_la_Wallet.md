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





## Optional: Sign a Message

Sometimes you'll need to prove that you control a Bitcoin address (or rather, that you control its private key). This is important because it lets people know that they're sending funds to the right person. This can be done by creating a signature with the `bitcoin-cli signmessage` command, in the form `bitcoin-cli signmessage [address] [message]`. For example:
```
$ bitcoin-cli signmessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "Hello, World"
HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=
```
You'll get the signature as a return.

> :book: ***What is a signature?*** A digital signature is a combination of a message and a private key that can then be unlocked with a public key. Since there's a one-to-one correspendence between the elements of a keypair, unlocking with a public key proves that the signer controlled the corresponding private key.

Another person can then use the `bitcoin-cli verifymessage` command to verify the signature. He inputs the address in question, the signature, and the message:
```
$ bitcoin-cli verifymessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
true
```
If they all match up, then the other person knows that he can safely transfer funds to the person who signed the message by sending to the address.

If some black hat was making up signatures, this would instead produce a negative result:
```
$ bitcoin-cli verifymessage "FAKEV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
error code: -3
error message:
Invalid address
```

## Optional: Dump Your Wallet

It might seem dangerous having all of your irreplaceable private keys in a single file. That's what `bitcoin-cli dumpwallet` is for. It lets you make a copy of your wallet.dat:
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
```
The `mywallet.txt` file in your home directory will have a long list of private keys, addresses, and other information. Mind you, you'd never want to put this data out in a plain text file on a Bitcoin setup with real funds!

You can then recover it with `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet ~/mywallet.txt
```
But note this requires an unpruned node!
```
$ bitcoin-cli importwallet ~/mywallet.txt
error code: -4
error message:
Importing wallets is disabled when blocks are pruned
```

## Optional: View Your Private Keys

Sometimes, you might want to actually look at the private keys associated with your Bitcoin addresses. Perhaps you want to be able to sign a message or spend bitcoins from a different machine. Perhaps you just want to back up certain important private keys. You can also do this with your dump file, since it's human readable.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/standup/mywallet.txt"
}
```
More likely, you just want to look at the private key associated with a specific address. This can be done with the `bitcoin-cli dumpprivkey` command.
```
$ bitcoin-cli dumpprivkey "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B"
cTv75T4B3NsG92tdSxSfzhuaGrzrmc1rJjLKscoQZXqNRs5tpYhH
```
You can then save that key somewhere safe, preferably somewhere not connected to the internet.

You can also import any private key, from a wallet dump or an individual key dump, as follows:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Again, expect this to require an unpruned node. Expect this to take a while, as `bitcoind` needs to reread all past transactions, to see if there are any new ones that it should pay attention to.

> :information_source: **NOTE:** Many modern wallets prefer [mnemonic codes](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) to generate the seeds necessary to create the private keys. This methodology is not used `bitcoin-cli`, so you won't be able to generate handy word lists to remember your private keys.

_You've been typing that Bitcoin address you generated a _lot_, while you were signing messages and now dumping keys. If you think it's a pain, we agree. It's also prone to errors, a topic that we'll address in the very next section._

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. Your address is stored in a wallet, which you can back up. You can also do lots more with an address, like dumping its private key or using it to sign messages. But really, creating that address is _all_ you need to do in order to receive Bitcoin funds.

## What's Next?

Step back from "Understanding Your Bitcoin Setup" with [Interlude: Using Command-Line Variables](03_3__Interlude_Using_Command-Line_Variables.md).
