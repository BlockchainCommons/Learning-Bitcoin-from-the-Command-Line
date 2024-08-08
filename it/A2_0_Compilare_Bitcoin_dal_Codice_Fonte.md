# Appendice 2: Compilare Bitcoin dal Codice Sorgente

Questo corso presuppone che tu utilizzi uno script per creare un ambiente Bitcoin, utilizzando Bitcoin Standup per Linode per [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), o tramite altri mezzi per [§2.2](02_2_Setting_Up_Bitcoin_Core_Other.md). Tuttavia, potresti preferire compilare Bitcoin manualmente.

Questo ha i seguenti vantaggi:

1. Sarai sempre aggiornato con l'ultima release. Avvertenza: Essere sempre aggiornati non è necessario per Bitcoin Core poiché il software è sempre retrocompatibile, il che significa che una vecchia versione di Bitcoin Core sarà ancora in grado di partecipare alla rete Bitcoin, anche se potresti non avere le ultime funzionalità. Dovresti sempre controllare le funzionalità di una nuova release prima di aggiornare.
2. Non dovrai dipendere dai binari precompilati di Bitcoin Core. Questo richiede meno fiducia. Anche se i manutentori di Bitcoin Core fanno un ottimo lavoro nel mantenere l'integrità del codice, un binario precompilato è a qualche passo di distanza dal codice sorgente. Quando compili dal codice sorgente, il codice può essere ispezionato prima della compilazione.
3. Puoi personalizzare la build, facendo cose come disabilitare il wallet o la GUI.

## Prepara il tuo Ambiente

Questo tutorial utilizza il sistema operativo Debian 10.4.kv0 su architettura amd64 (computer a 64 bit), ma puoi utilizzare questo tutorial su qualsiasi sistema basato su Debian (ad es. Ubuntu, Mint, ecc.). Per altri sistemi Linux, puoi adattare i seguenti passaggi con il gestore di pacchetti per quel sistema.

Puoi avere una familiarità di base o nessuna familiarità con la riga di comando purché tu abbia entusiasmo. Il terminale è il tuo alleato più potente, non qualcosa da temere. Puoi semplicemente copiare e incollare i seguenti comandi per compilare bitcoin. (Un comando con "$" è un comando dell'utente normale, e uno con "#" è un comando del superutente/root.)

Se il tuo utente non è nella lista dei sudoers, allora fai quanto segue:



```
$ su root
<enter root passwd>
# apt-get install sudo
# usermod -aG sudo <username>
# reboot
```


## Installa Bitcoin

### Passo 1: Aggiorna il tuo Sistema

Prima, aggiorna il sistema usando:

```
$ sudo apt-get update
```
### Passo 2: Installa Git e le Dipendenze

Installa `git`, che ti permetterà di scaricare il codice sorgente, e `build-essential`, che compila il codice:

```
$ sudo apt-get install git build-essential -y
```

Successivamente, installa le dipendenze rimanenti:

```
$ sudo apt-get install libtool autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler ccache -y
```

### Passo 3: Scarica il Codice Sorgente

Una volta installate le dipendenze, scarica il repository (repo) contenente il codice sorgente di Bitcoin da github:

```
$ git clone https://github.com/bitcoin/bitcoin.git
```

Controlla il contenuto del repo:

```
$ ls bitcoin
````
Dovrebbe approssimativamente corrispondere al seguente contenuto:

![cloning the repo](./public/LBftCLI-compiling_bitcoin-git.png)

### Passo 4: Installa Berkley DB v4.8

1. Entra nella directory `contrib`: `$ cd bitcoin/contrib/`
2. Esegui il seguente comando: ```$ ./install_db4.sh `pwd` ```

Una volta scaricato, dovresti vedere il seguente output. Prendi nota dell'output, lo userai per configurare bitcoin durante la compilazione:

![db4](./public/LBftCLI-compiling_bitcoin-db4.png)

### Passo 5: Compila Bitcoin Core

Si consiglia di compilare da un branch taggato, che è più stabile, a meno che tu non voglia provare l'avanguardia dello sviluppo di bitcoin. Esegui il seguente comando per ottenere l'elenco dei tag, ordinati dal più recente:
```
$ git tag -n | sort -V
```
Poi scegli un tag come `v0.20.0`:
```
$ git checkout <TAG>
```

Una volta selezionato un branch tag, esegui i seguenti comandi all'interno della directory `bitcoin`. Il `<PATH-TO>` dovrebbe essere l'output dello script `install_db4.sh`.

```
$ ./autogen.sh
$ export BDB_PREFIX='<PATH-TO>/db4'
$ ./configure BDB_LIBS="-L${BDB_PREFIX}/lib -ldb_cxx-4.8" BDB_CFLAGS="-I${BDB_PREFIX}/include"
$ make  # build bitcoin core
```

### Passo 6: Testa la Build

Se vuoi controllare la tua build (cosa che è una buona idea), esegui i seguenti test:

1. `$ make check` eseguirà i Test Unitari, che dovrebbero tutti restituire `PASS`.
2. `$ test/functional/test_runner.py --extended` eseguirà test funzionali estesi. Ometti il flag `--extended` se vuoi saltare alcuni test. Questo richiederà un po' di tempo.

### Passo 7: Esegui o Installa Bitcoin Core

Ora che hai compilato Bitcoin Core dal codice sorgente, puoi iniziare a usarlo o installarlo per una disponibilità globale.

#### Esegui Bitcoin Core senza Installarlo

Per eseguire solo Bitcoin Core:

`$ src/qt/bitcoin-qt` per avviare la GUI.
`$ src/bitcoind` per eseguire bitcoin sulla riga di comando.

### Installa Bitcoin Core

Per installare:

`$ sudo make install` installerà bitcoin core globalmente. Una volta installato, puoi quindi eseguire bitcoin da qualsiasi parte nella riga di comando, proprio come qualsiasi altro software, così: `$ bitcoin-qt` per la GUI o `bitcoind` e poi `bitcoin-cli` per la riga di comando.

## Finalizza il tuo Sistema

Compilando Bitcoin dal codice sorgente, hai aumentato la fiducia nel tuo setup. Tuttavia, sei ben lontano dalla sicurezza aggiuntiva fornita da un setup Bitcoin Standup. Per risolvere questo, potresti voler seguire l'intero [Linode Stackscript](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) e passo dopo passo eseguire tutti i comandi. L'unico punto in cui devi fare attenzione è nel Passo 6, che installa Bitcoin. Salta appena dopo aver verificato i tuoi binari e continua da lì.

## Sommario: Compilare Bitcoin dal Codice Sorgente

Se volevi la sicurezza aumentata di installare Bitcoin dal codice sorgente, ora dovresti averla. Speriamo che tu abbia anche seguito il Linode Stackscript per configurare un server più sicuro.

## Cosa segue?

* Se eri nel processo di creazione di un nodo Bitcoin per l'uso in questo corso, dovresti continuare con [Capitolo 3: Comprendere la Configurazione di Bitcoin](03_0_Comprendere_la_Configurazione_di_Bitcoin.md).

* Se stai leggendo le appendici, continua con [Appendice 3: Usare Bitcoin Regtest ](A3_0_Usare_Bitcoin_Regtest.md).
