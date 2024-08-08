# 3.5: Comprendere il Descrittore

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvertito.

Potresti aver notato uno strano campo `desc:` nel comando `listunspent` della sezione precedente. Ecco di cosa si tratta (e come può essere utilizzato per trasferire indirizzi).

> :avviso: **AVVERTIMENTO VERSIONE:** Questa è un'innovazione di Bitcoin Core v 0.17.0 che ha continuato ad essere ampliata attraverso Bitcoin Core 0.20.0. La maggior parte dei comandi in questa sezione provengono dalla versione 0.17.0, ma l'aggiornamento `importmulti` che supporta i descrittori proviene dalla versione 0.18.0.

## Informazioni sul trasferimento degli indirizzi

La maggior parte di questo corso presuppone che tu stia lavorando interamente da un singolo nodo in cui gestisci il tuo portafoglio, inviando e ricevendo pagamenti con gli indirizzi creati da quel portafoglio. Tuttavia, non è necessariamente così che funziona il piu ampio ecosistema Bitcoin. Lì, è più probabile che sposti indirizzi tra portafogli e addirittura configuri portafogli watchonly per sorvegliare i fondi controllati da portafogli diversi.

È qui che entrano in gioco i descrittori. Sono molto utili se stai interagendo con software _diverso_ da Bitcoin Core e hai davvero bisogno di appoggiarti a questo tipo di funzione di compatibilità: vedi [Capitolo 6.1](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md) per un esempio reale di come sia utilizzare i descrittori.

Lo spostamento degli indirizzi tra portafogli si concentrava su "xpub" e "xprv", e questi sono ancora supportati.

> :book: ***Cos'è xprv?*** Una chiave privata estesa. Questa è la combinazione di una chiave privata e un codice a catena. È una chiave privata da cui può essere derivata un'intera sequenza di chiavi private figlie.

> :book: ***Cos'è xpub?*** Una chiave pubblica estesa. Questa è la combinazione di una chiave pubblica e un codice a catena. È una chiave pubblica da cui può essere derivata un'intera sequenza di chiavi pubbliche figlie.

Il fatto che tu possa avere una "intera sequenza di chiavi figlie" rivela il fatto che "xpub" e "xprv" non sono chiavi standard come abbiamo parlato finora. Sono invece chiavi gerarchiche che possono essere utilizzate per creare intere famiglie di chiavi, basate sull'idea degli HD Wallet.

> :book: ***Che cos'è un portafoglio HD?*** La maggior parte dei portafogli moderni è basata su [BIP32: portafogli deterministici gerarchici](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki). Si tratta di un progetto gerarchico in cui un singolo seme può essere utilizzato per generare un'intera sequenza di chiavi. L'intero portafoglio può quindi essere ripristinato da quel seme, invece di richiedere il ripristino di ogni singola chiave privata.

> :book: ***Cos'è un derivation path?*** Quando disponi di chiavi gerarchiche, devi essere in grado di definire chiavi individuali come discendenti di un seme. Ad esempio, "[0]" è la chiave 0, "[0/1]" è il primo figlio della chiave 0, "[1/0/1]" è il primo nipote del figlio zero della chiave 0. Alcune chiavi contengono anche un `'` dopo il numero, per indicare che sono rinforzate, cosa che le protegge da un attacco specifico che può essere utilizzato per derivare un `'xprv` da un `'xpub`. Non devi preoccuparti dei dettagli, a parte il fatto che questi `'` ti causeranno problemi di formattazione quando lavori dalla riga di comando.

> :information_source: **NOTA:** un derivation path definisce una chiave, il che significa che una chiave rappresenta un percorso di derivazione. Sono equivalenti. Nel caso di un descrittore, il percorso di derivazione consente a `bitcoind` di sapere da dove proviene la chiave che segue nel descrittore!

"xpubs" e "xprvs" si sono rivelati insufficienti quando i tipi di chiavi pubbliche si sono moltiplicati sotto l'[espansione SegWit](04_6_Creare_una_Transazione_Segwit.md), ecco quindi la necessità di "output descriptors".

> :book: ***Output descriptors? cosa sono?*** Una descrizione precisa di come derivare un indirizzo Bitcoin da una combinazione di una funzione e uno o più input per quella funzione.

L'introduzione di funzioni nei descrittori è ciò che li rende potenti, perché possono essere utilizzati per trasferire tutti i tipi di indirizzi, dagli indirizzi Legacy con cui stiamo lavorando ora agli indirizzi Segwit e multisig che incontreremo più avanti. Una singola funzione corrisponde a un particolare tipo di indirizzo ed è correlata a regole specifiche per generare quell'indirizzo.

## Cattura un descrittore

I descrittori sono visibili in diversi comandi come `listunspent` e `getaddressinfo`:
```
$ bitcoin-cli getaddressinfo ms7ruzvL4atCu77n47dStMb3of6iScS8kZ
{
  "address": "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ",
  "scriptPubKey": "76a9147f437379bcc66c40745edc1891ea6b3830e1975d88ac",
  "ismine": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": false,
  "pubkey": "03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388",
  "iscompressed": true,
  "ischange": false,
  "timestamp": 1592335136,
  "hdkeypath": "m/0'/0'/18'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
    ""
  ]
}
```
Qui il descriptor è questo `pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk`.

## Comprendere il descriptor

Un descrittore è suddiviso in più parti:
```
function([derivation-path]key)#checksum
```
Ecco cosa significa tutto ciò:
* **Function.** La funzione utilizzata per creare un indirizzo da quella chiave. In questi casi si tratta di `pkh`, che è l'indirizzo legacy P2PKH standard che hai incontrato in [Capitolo 3.3: Configurare la Wallet](03_3_Configurare_la_Wallet.md). Allo stesso modo, un indirizzo SegWit P2WSH utilizzerebbe "wsh" e un indirizzo P2WPKH utilizzerebbe "wpkh".
* **Derivation Path.** Descrive quale parte di un portafoglio HD viene esportata. In questo caso è un seme con l'impronta digitale `d6043800` e quindi il 18° figlio dello 0° figlio dello 0° figlio (`0'/0'/18'`) di quel seme. Potrebbe esserci anche un'ulteriore derivazione dopo la chiave: `function([derivation-path]key/more-derivation)#checksum`
 * Vale la pena notare qui che se mai ottieni un percorso di derivazione senza impronta digitale, puoi inventarlo. È solo che se ce n'è una esistente, dovresti abbinarla, perché se mai dovessi tornare al dispositivo che ha creato l'impronta digitale, dovrai avere la stessa.
* **key**. La chiave o le chiavi che vengono trasferite. Potrebbe trattarsi di qualcosa di tradizionale come "xpub" o "xprv", potrebbe essere semplicemente una chiave pubblica per un indirizzo come in questo caso, potrebbe essere un insieme di indirizzi per una firma multipla o potrebbe essere qualcos'altro. Questi sono i dati fondamentali: la funzione spiega cosa farne.
* **#checksum**. I descrittori sono pensati per essere trasferibili dall'uomo. Questo checksum assicura che sia corretto.

Vedi [Informazioni di Bitcoin Core sul supporto dei descrittori](https://github.com/bitcoin/bitcoin/blob/master/doc/descriptors.md) per ulteriori informazioni.

## Esamina un descrittore

Puoi guardare un descrittore con l'RPC `getdescriptorinfo`:
```
$ bitcoin-cli getdescriptorinfo "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk"
{
  "descriptor": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "checksum": "4ahsl9pk",
  "isrange": false,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Tieni presente che restituisce un checksum. Se ti viene mai fornito un descrittore senza checksum, puoi ottenerlo con questo comando:
```
$ bitcoin-cli getdescriptorinfo "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)"
{
  "descriptor": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "checksum": "4ahsl9pk",
  "isrange": false,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Oltre a fornirti il ​​checksum, questo comando verifica anche la validità del descrittore e fornisce informazioni utili come se un descrittore contiene chiavi private.

Uno dei poteri di un descrittore è quello di poter derivare un indirizzo in modo regolare. Questo viene fatto con l'RPC `deriveaddresses`.

```
$ bitcoin-cli deriveaddresses "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk"
[
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ"
]
```
Noterai che ritorna all'indirizzo con cui abbiamo iniziato (come dovrebbe).

## Importa un descrittore

Ma la cosa veramente importante di un descrittore è che puoi portarlo su un'altra macchina (remota) e importarlo. Questo viene fatto con l'RPC `importmulti` utilizzando l'opzione `desc`:

```
remote$ bitcoin-cli importmulti '[{"desc": "pkh([d6043800/0'"'"'/0'"'"'/18'"'"']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk", "timestamp": "now", "watchonly": true}]'
[
  {
    "success": true
  }
]
```
Innanzitutto noterai il nostro primo uso davvero brutto delle virgolette. Ogni `'` nel percorso di derivazione doveva essere sostituito con `'"'"'`. Aspettati solo di doverlo fare se stai manipolando un descrittore che contiene un percorso di derivazione. (L'altra opzione è scambiare `'` con una `h` per hardened, ma questo cambierà il tuo checksum, quindi se lo preferisci per la sua facilità d'uso, dovrai ottenere un nuovo checksum con `getdescriptorinfo` .)

In secondo luogo, noterai che lo abbiamo contrassegnato come `watchonly`. Questo perché sappiamo che è una chiave pubblica, quindi non possiamo spendere con essa. Se non avessimo inserito questo flag, `importmulti` ci avrebbe detto qualcosa del tipo: `Mancano alcune chiavi private, gli output saranno considerati watchonly. Se ciò è intenzionale, specifica il flag watchonly.`.


> :book: ***Che cos'è un indirizzo watch only?*** Un indirizzo di solo controllo ti consente di controllare le transazioni relative a un indirizzo (o a un'intera famiglia di indirizzi se hai utilizzato un `xpub`), ma non spendere fondi per quegli indirizzi.

Utilizzando `getaddressesbylabel`, ora possiamo vedere che il nostro indirizzo è stato importato correttamente nel nostro computer remoto!
```
remote$ bitcoin-cli getaddressesbylabel ""
{
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ": {
    "purpose": "receive"
  }
}
```
## Sommario: Comprendere il Descrittore

I descrittori ti consentono di passare chiavi pubbliche e private tra portafogli, ma soprattutto ti consentono di definire indirizzi in modo preciso e corretto e di derivare indirizzi di molti tipi diversi da un formato di descrizione standardizzato.

> :fire: ***Qual è il potere dei descrittori?*** I descrittori ti permettono di importare ed esportare semi e chiavi. È fantastico se vuoi spostarti tra diversi portafogli. Come sviluppatore, ti consentono anche di creare il tipo preciso di indirizzi che ti interessa creare. Ad esempio, lo utilizziamo in [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2/blob/master/Docs/How-it-works.md) per generare un multi-sig da tre seed.

Faremo un uso reale dei descrittori nel [Capitolo 7.3: Integrazione con Hardware Wallets](07_3_Integrazione_con_Hardware_Wallets.md), quando importeremo indirizzi da un portafoglio hardware.

## Qual è il prossimo?

Avanza attraverso "bitcoin-cli" con il [Capitolo 4: Inviare Transazioni Bitcoin](04_0_Inviare_Transazioni_Bitcoin.md).
