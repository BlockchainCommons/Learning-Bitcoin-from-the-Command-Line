# 7.3: Integrazione con hardware wallet

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Uno dei maggiori poteri dei PSBT è la capacità di trasferire le transazioni ai portafogli hardware. Questo sarà un ottimo strumento di sviluppo per te se continui a programmare con Bitcoin. Tuttavia, non puoi testarlo ora se stai utilizzando una delle configurazioni che suggeriamo per questo corso: una VM su Linode secondo il [Capitolo 2.1](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md) o un'opzione ancora più remota come AWS vista nel [Capitolo 2.2](02_2_Configurare_Bitcoin_Core_Altri_Mezzi.md) - perché ovviamente non avrai alcun modo di collegare un portafoglio hardware alla tua macchina virtuale remota.

> :book: ***Che cos'è un portafoglio hardware?*** Un portafoglio hardware è un dispositivo elettronico che migliora la sicurezza di una criptovaluta mantenendo tutte le chiavi private sul dispositivo, anziché metterle mai su un computer direttamente collegato a Internet. I portafogli hardware dispongono di protocolli specifici per fornire interazioni online, solitamente gestiti da un programma che comunica con il dispositivo tramite una porta USB. In questo capitolo gestiremo un portafoglio hardware con `bitcoin-cli` e il programma `hwy.py`.

Hai tre opzioni per muoverti in questo capitolo sui portafogli hardware: (1) leggere senza testare il codice; (2) installa Bitcoin su un computer locale per testare completamente questi comandi; oppure (3) vai direttamente al [Capitolo 8: Ampliare Altre Transazioni Bitcoin](08_0_Ampliare_Altre_Transazioni_Bitcoin.md). Suggeriamo l'opzione n. 1, ma se vuoi davvero sporcarti le mani ti forniremo supporto anche per la n.2 parlando dell'utilizzo di un Macintosh (una piattaforma hardware supportata da 
[Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup)) per i test.

> :avviso: **AVVISO di VERSIONE:** Le PSBT sono un'innovazione di Bitcoin Core v 0.17.0. Le versioni precedenti di Bitcoin Core non saranno in grado di funzionare con il PSBT mentre è in corso (sebbene saranno comunque in grado di riconoscere la transazione finale). L'interfaccia HWI è apparsa in Bitcoin Core v 0.18.0, ma finché utilizzi la nostra configurazione suggerita con Bitcoin Standup, dovrebbe funzionare.

La metodologia descritta in questo capitolo per l'integrazione con un portafoglio hardware dipende dall'[Bitcoin Hardware Wallet Interface](https://github.com/bitcoin-core/HWI) rilasciata tramite Bitcoin Core e si basa sull'[installazione](https://github.com/bitcoin-core/HWI/blob/master/README.md) e le istruzioni d'[utilizzo](https://hwi.readthedocs.io) si trovano lì.

> :avviso: **AVVISO:** L'interfaccia HWI è molto nuova e grezza a partire da Bitcoin Core v 0.20.0. Potrebbe essere difficile installarlo correttamente e potrebbero essere presenti errori non intuitivi. Quella che segue è una descrizione di una configurazione funzionante, ma sono stati necessari diversi tentativi per ottenerla correttamente e la configurazione potrebbe variare.

## Installa Bitcoin Core nel tuo PC

_Se hai intenzione di leggere semplicemente questa sezione e di non testare questi comandi fino a una data futura quando disponi di un ambiente di sviluppo locale, puoi saltare questa sottosezione, che riguarda la creazione di un'installazione Bitcoin Core su una macchina locale come un Mac o meglio una Macchina Linux._

Esistono versioni alternative dello script Bitcoin Standup che hai utilizzato per creare la tua VM che verrà installata su un MacOS o su una macchina Linux non Linode.

Se disponi di MacOS, puoi installare [Bitcoin Standup MacOS](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

Se disponi di una macchina Linux locale, puoi installare [Bitcoin Standup Linux Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/README.md).

Una volta che Bitcoin Standup è in esecuzione sul tuo computer locale, ti consigliamo di sincronizzare la blockchain `Testnet`, supponendo che tu continui a seguire la metodologia standard di questo corso.

Utilizzeremo un Macintosh e Testnet per gli esempi in questa sezione.

### Crea un alias per Bitcoin-CLI

Crea un alias che esegua `bitcoin-cli` dalla directory corretta con eventuali flag appropriati.

Ecco un esempio di alias da un Mac:
```
$ alias bitcoin-cli="~/StandUp/BitcoinCore/bitcoin-0.20.0/bin/bitcoin-cli -testnet"
```
Noterai che non solo ci fornisce il percorso completo, ma garantisce anche che rimaniamo su Testnet.

## Installa HWI su un computer locale

_Le seguenti istruzioni presuppongono nuovamente un Mac e puoi saltare questa sottosezione se stai solo leggendo questo capitolo._

HWI è un programma Bitcoin Core disponibile in Python che può essere utilizzato per interagire con i portafogli hardware.

### Installa Python

Poiché HWI è scritto in `python`, dovrai installarlo insieme ad alcuni programmi ausiliari.

Se non disponi già degli strumenti da riga di comando "xcode", ti serviranno:
```
$ xcode-select --install
```

Se non hai già il gestore pacchetti Homebrew, dovresti installare anche quello. Le istruzioni attuali sono disponibili sul [sito Homebrew](https://brew.sh/). Al momento della stesura di questo articolo, devi semplicemente farlo:
```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

Per la prima installazione, dovresti anche assicurarti che la tua directory `/usr/local/Frameworks` sia creata correttamente:
```
$ sudo mkdir /usr/local/Frameworks
$ sudo chown $(whoami):admin /usr/local/Frameworks
```

Se hai tutto a posto, puoi finalmente installare Python:
```
$ brew install python
$ brew install libusb
```

### Installa l'HWI

Ora sei pronto per installare HWI, che richiede la clonazione di un repository GitHub e l'esecuzione di uno script di installazione.

Se non hai `git` già installato sul tuo Mac, puoi farlo semplicemente provando a eseguirlo: `git --version`.

È quindi possibile clonare il repository HWI:
```
$ cd ~/StandUp
$ git clone https://github.com/bitcoin-core/HWI.git
```
Successivamente, è necessario installare il pacchetto e le sue dipendenze:
```
$ cd HWI
HWI$ python3 setup.py install
```

### Crea un alias per HWI

Ti consigliamo di creare un alias anche qui, variato in base alla posizione di installazione effettiva:
```
$ alias hwi="~/Standup/HWI/hwi.py --chain test"
```
Ancora una volta, abbiamo incluso un riferimento a testnet in questo alias.

## Prepara il tuo Ledger

_Anche noi abbiamo dovuto scegliere una piattaforma hardware-wallet per questa dimostrazione HWI. La nostra scelta è ricaduta sul Ledger, che è stato a lungo il nostro banco di prova per i portafogli hardware. Consulta le [informazioni sul supporto del dispositivo HWI](https://github.com/bitcoin-core/HWI/blob/master/README.md#device-support) per un elenco di altri dispositivi supportati. Se utilizzi un dispositivo diverso da Ledger, dovrai valutare le tue soluzioni per prepararlo all'utilizzo su Testnet, ma per il resto dovresti essere in grado di continuare con il corso come scritto._

Se stai lavorando con Bitcoin sul tuo Ledger, probabilmente non dovrai fare nulla. (Ma non lo suggeriamo per l'uso con questo corso).

Per lavorare con le monete Testnet, come suggerito da questo corso, dovrai apportare alcuni aggiornamenti:

1. Vai su Impostazioni sulla tua app Ledger Live (è l'attrezzatura), vai alla scheda "Funzionalità sperimentali" e attiva "Modalità sviluppatore".
2. Vai su "Manager" e installa "Bitcoin Test". La versione attuale richiede che tu abbia prima installato "Bitcoin".
3. Vai su "Manager", scorri fino al tuo nuovo "Bitcoin Test" e "Aggiungi account"

## Collegamento a un registro

Affinché un Ledger sia accessibile, devi effettuare il login con il tuo PIN e poi richiamare l'app che desideri utilizzare, in questo caso l'app "Bitcoin Test". Potrebbe essere necessario ripeterlo di tanto in tanto se il tuo Ledger si addormenta.

Dopo averlo fatto, puoi chiedere a HWI di accedere al Ledger con il comando "enumerate".:
```
$ hwi enumerate
[{"type": "ledger", "model": "ledger_nano_s", "path": "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/XHC1@14/XHC1@14000000/HS05@14100000/Nano S@14100000/Nano S@0/IOUSBHostHIDDevice@14100000,0", "fingerprint": "9a1d520b", "needs_pin_sent": false, "needs_passphrase_sent": false}]
```
Se ricevi informazioni sul tuo dispositivo, sei a posto! Come puoi vedere, verifica il tuo tipo di portafoglio hardware, fornisce altre informazioni identificative e ti dice come comunicare con il dispositivo. L'impronta digitale (`9a1d520b`) è ciò a cui dovresti prestare particolare attenzione, perché tutte le interazioni con il tuo portafoglio hardware la richiederanno.

Se invece hai ottenuto "[]", allora (1) non hai preparato il tuo dispositivo Ledger inserendo il PIN e scegliendo l'applicazione corretta, oppure (2) c'è qualcosa di sbagliato nella configurazione di Python, probabilmente una dipendenza mancante: dovresti considerare di disinstallarlo e provare da zero.

## Importa indirizzi

L'interazione con un portafoglio hardware di solito avviene in due parti: ricerca di fondi e spesa di fondi.

Puoi controllare i fondi importando gli indirizzi dal tuo portafoglio hardware al tuo nodo completo, utilizzando HWI e `bitcoin-cli`.

### Crea un portafoglio

Per utilizzare il tuo portafoglio hardware con `bitcoin-cli`, ti consigliamo di creare un portafoglio con nome specifico in Bitcoin Core, utilizzando l'RPC `createwallet`, che è un comando di cui non abbiamo discusso in precedenza.
```
$ bitcoin-cli --named createwallet wallet_name="ledger" disable_private_keys="true" descriptors="false"
{
  "name": "ledger",
  "warning": ""
}
```
In questo caso, stai creando un nuovo portafoglio `ledger` senza chiavi private (poiché queste saranno presenti sul dispositivo Ledger).

> :book: ***Perché nominare i portafogli?*** Fino ad oggi, questo corso ha utilizzato il portafoglio predefinito (` "" `) in Bitcoin Core. Questo va bene per molti scopi, ma è inadeguato se hai una situazione più complessa, come quando guardi le chiavi da un portafoglio hardware. Qui, vogliamo essere in grado di distinguere dalle chiavi possedute localmente (che sono conservate nel portafoglio ` "" ` ) e dalle chiavi possedute in remoto (che sono conservate nel portafoglio "ledger").

Ora puoi vedere che il nuovo portafoglio è nell'elenco dei tuoi portafogli:
```
$ bitcoin-cli listwallets
[
  "",
  "ledger"
]
```
Poiché hai creato un secondo portafoglio, alcuni comandi ora richiederanno un flag `-rpcwallet=`, per specificare quale stai utilizzando

### Importa le chiavi

Ora devi importare una watchlist di indirizzi dal portafoglio hardware. Questo viene fatto con il comando `getkeypool` di HWI:
```
$ hwi -f 9a1d520b getkeypool 0 1000
[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]
```
Indirizziamo HWI con l'impronta digitale e chiediamo i primi 1000 indirizzi. Per impostazione predefinita viene utilizzato il tipo di indirizzo WPKH (nativo Segwit). In cambio riceviamo due descrittori per il key pool: uno per ricevere gli indirizzi e uno per cambiare gli indirizzi.

> :book: ***Che cos'è un pool di chiavi?*** Un pool di chiavi è un gruppo di chiavi pregenerate. I moderni portafogli HD creano pool di chiavi continuando a determinare nuovi indirizzi gerarchici basati sul seed originale. L’idea dei pool di chiavi è stata originariamente implementata per facilitare i requisiti di backup dei portafogli. Ciò ha consentito a un utente di generare un pool di chiavi e quindi eseguire immediatamente il backup del portafoglio, anziché richiedere backup dopo la creazione di ogni nuovo indirizzo. Il concetto si è rivelato molto utile anche ai giorni nostri poiché consente di importare un intero set di indirizzi futuri da un dispositivo all'altro.

I valori restituiti da `getkeypool` sono lo stesso tipo di descrittori di cui abbiamo appreso nel [Capitolo3.5: Comprendere il descrittore](03_5_Comprendere_il_Descriptor.md). All'epoca dicevamo che erano utilissimi per spostare indirizzi tra macchine diverse. Ecco l'esempio della vita reale: spostare gli indirizzi da un portafoglio hardware al nodo Bitcoin Core, in modo che la nostra macchina connessa alla rete sia in grado di sorvegliare le chiavi possedute dal portafoglio hardware offline.

Proprio come hai imparato nel [Capitolo 3.5](03_5_Comprendere_il_Descriptor.md), puoi esaminare questi descrittori con l'RPC `getdescriptorinfo`:
```
$ bitcoin-cli getdescriptorinfo "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592"
{
  "descriptor": "wpkh([9a1d520b/84'/1'/0']tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#n65e7wjf",
  "checksum": "qttxy592",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Come ci si aspetterebbe, _non_ hai le "chiavi private", perché i portafogli hardware le conservano.

Con il descrittore in mano, puoi importare le chiavi nel tuo nuovo portafoglio `ledger` utilizzando l'RPC `importmulti` che hai incontrato anche nel [Capitolo 3.5](03_5_Comprendere_il_Descriptor.md). In questo caso, inserisci semplicemente l'intera risposta ricevuta da HWI in `'`.
```
$ bitcoin-cli -rpcwallet=ledger importmulti '[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]'
[
  {
    "success": true
  },
  {
    "success": true
  }
]
```
(Si noti che HWI restituisce utilmente il percorso di derivazione con `h` per mostrare le derivazioni rafforzate anziché `'` e calcola il suo checksum di conseguenza, in modo da non dover fare citazioni massicce come abbiamo fatto nel Capitolo 3.5.)

Ora _potresti_ elencare tutti gli indirizzi di watch-okly che hai ricevuto utilizzando il comando `getaddressesbylabel`. Tutti i 1000 indirizzi di ricezione sono proprio lì, nel portafoglio `ledger`!

```
$ bitcoin-cli -rpcwallet=ledger getaddressesbylabel "" | more
{
  "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y": {
    "purpose": "receive"
  },
  "tb1qqzvrm6hujdt93qctuuev5qc4499tq9fdk0prwf": {
    "purpose": "receive"
  },
...
}
```
## Ricevi una transazione

Ovviamente, ricevere una transazione è semplice. Utilizzi "getnewaddress" per richiedere uno di quegli indirizzi importati:
```
$ bitcoin-cli -rpcwallet=ledger getnewaddress
tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y
```
Quindi gli invii denaro.

Il potere di HWI è che puoi guardare i pagamenti dal tuo nodo Bitcoin Core, invece di dover collegare il tuo portafoglio hardware e interrogarlo.
```
$ bitcoin-cli -rpcwallet=ledger listunspent
[
  {
    "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
    "vout": 1,
    "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr",
    "label": "",
    "scriptPubKey": "00142d4f139faa885304d15616a166d2d51574af4a5d",
    "amount": 0.01000000,
    "confirmations": 12,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/0]02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab)#9za8hlvk",
    "safe": true
  },
  {
    "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
    "vout": 0,
    "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y",
    "label": "",
    "scriptPubKey": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/569]030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5)#sx9haake",
    "safe": true
  }
]
```
## Crea una transazione con PSBT

Controllare e ricevere pagamenti è solo metà dell'opera. Potresti anche voler effettuare pagamenti utilizzando gli account detenuti dal tuo portafoglio hardware. Questo è il quarto esempio di vita reale per l'utilizzo di PSBT, secondo il processo delineato in [Capitolo 7.1: Creare una transazione Bitcoin parzialmente firmata](07_1_Creare_una_Transazione_Bitcoin_Parzialmente_Firmata.md).

I comandi funzionano esattamente allo stesso modo. In questo caso, usa `walletcreatefundedpsbt` per formare il tuo PSBT perché questa è una situazione in cui non ti interessa quali UTXO vengono utilizzati:
```
$ bitcoin-cli -named -rpcwallet=ledger walletcreatefundedpsbt inputs='''[]''' outputs='''[{"tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd":0.015}]'''
{
  "psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==",
  "fee": 0.00000209,
  "changepos": 1
}
```

Puoi dare un'occhiata al PSBT e verificare che sembri razionale:

```
$ psbt="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA=="

$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "hash": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01500000,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      },
      {
        "value": 0.00499791,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 f4e8dde5db370898b57c84566e3f76098850817d",
          "hex": "0014f4e8dde5db370898b57c84566e3f76098850817d",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q7n5dmewmxuyf3dtus3txu0mkpxy9pqtacuprak"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 00193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "hex": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "type": "witness_v0_keyhash",
          "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/569"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2d4f139faa885304d15616a166d2d51574af4a5d",
          "hex": "00142d4f139faa885304d15616a166d2d51574af4a5d",
          "type": "witness_v0_keyhash",
          "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/0"
        }
      ]
    }
  ],
  "outputs": [
    {
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "03d942b59eea527d70bcd67981eb95d9fa9625269fd6eb0364e452ede59092c7a9",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/1/1"
        }
      ]
    }
  ],
  "fee": 0.00000209
}
```
E come al solito, `analyzepsbt` mostrerà quanto lontano sei arrivato:
```
$ bitcoin-cli analyzepsbt $psbt
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "00193c8bf25ef056f8d4571a1c3f65123e5fe788"
        ]
      }
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "2d4f139faa885304d15616a166d2d51574af4a5d"
        ]
      }
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "signer"
}
```
Poiché hai importato quel pool di chiavi, `bitcoin-cli` ha tutte le informazioni necessarie per compilare gli input, semplicemente non può firmare perché le chiavi private sono conservate nel portafoglio hardware.

È qui che entra in gioco HWI, con il comando `signtx`. Basta inviare lungo il PSBT:
```
$ hwi -f 9a1d520b signtx $psbt
```
Aspettatevi di dover armeggiare con il vostro portafoglio hardware a questo punto. Probabilmente il dispositivo ti chiederà di confermare gli ingressi, le uscite e la tariffa. Al termine, dovrebbe restituire un nuovo PSBT.

```
{"psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="}
$ psbt_f="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="
```

Quando lo analizzi, vedrai che è pronto per essere finalizzato:

```
$ bitcoin-cli analyzepsbt $psbt_f
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "finalizer"
}
```

A questo punto sei di nuovo nel territorio standard:
```
$ bitcoin-cli finalizepsbt $psbt_f
{
  "hex": "02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000",
  "complete": true
}
$ hex=02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000
$ bitcoin-cli sendrawtransaction $hex
45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d
```
Hai inviato fondi con successo utilizzando le chiavi private conservate nel tuo portafoglio hardware!

## Scopri altri comandi HWI

Sono disponibili numerosi altri comandi da utilizzare con HWI. Al momento in cui scrivo, ci sono:
```
numerate,getmasterxpub,signtx,getxpub,signmessage,getkeypool,getdescriptors,displayaddress,setup,wipe,restore,backup,promptpin,togglepassphrase,sendpin
```

## Riepilogo: Integrazione con hardware wallet

I hardware wallet possono offrire una migliore protezione mantenendo le tue chiavi private offline, protette nell'hardware. Fortunatamente, c'è ancora un modo per interagire con loro utilizzando "bitcoin-cli". Basta installare HWI e ti consentirà quindi di (1) importare chiavi pubbliche e guardarle; e (2) firmare le transazioni utilizzando il tuo hardware wallet.

> :fire: ***Qual è il potere di HWI?*** HWI ti consente di interagire con i portafogli hardware utilizzando tutti i comandi di `bitcoin-cli` che hai imparato fino ad oggi. Puoi effettuare transazioni grezze di qualsiasi tipo, quindi inviare PSBT ai portafogli hardware per la firma. Pertanto, hai tutta la potenza di Bitcoin Core, ma hai anche la sicurezza di un dispositivo hardware.

## Qual è il prossimoargomento?

Espandi maggiormente le transazioni Bitcoin con il [Capitolo 8: Ampliare Altre Transazioni Bitcoin](08_0_Ampliare_Altre_Transazioni_Bitcoin.md).
