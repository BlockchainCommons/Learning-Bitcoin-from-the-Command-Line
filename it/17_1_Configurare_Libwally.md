# 17.1: Configurare Libwally

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe ancora essere in attesa di revisione. Lettore avvisato.

Questa prima sezione spiegherà come scaricare la libreria C Libwally e farla funzionare.

> :book: ***Cos'è Libwally?*** Libwally è una libreria di primitive utili per la creazione di portafogli che è cross-platform e cross-language, così che le stesse funzioni possano essere utilizzate ovunque. Ci sono [documenti online](https://wally.readthedocs.io/en/latest/). Libwally è resa disponibile come parte del [Elements Project](https://github.com/ElementsProject) di Blockstream.

## Installare Libwally

Come al solito, avrai bisogno di alcuni pacchetti sul tuo sistema:

```
$ sudo apt-get install git
$ sudo apt-get install dh-autoreconf
```
Puoi quindi scaricare Libwally dal suo repository Git:
```
$ git clone https://github.com/ElementsProject/libwally-core
```
Successivamente, puoi iniziare il processo di configurazione:
```
$ ./tools/autogen.sh
```
Come con `libbitcoinrpc`, potresti voler installarlo in `/usr/include` e `/usr/lib` per facilità di utilizzo. Basta modificare la linea appropriata nel programma `configure`:
```
< ac_default_prefix=/usr
---
> ac_default_prefix=/usr/local
```
Successivamente, puoi completare la preparazione:
```
$ ./configure
$ make
```
Puoi quindi verificare che i test funzionino:
```
$ make check
Making check in src
make[1]: Entering directory '/home/standup/libwally-core/src'
Making check in secp256k1
make[2]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make  check-TESTS
make[3]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make[4]: Entering directory '/home/standup/libwally-core/src/secp256k1'
============================================================================
Testsuite summary for libsecp256k1 0.1
============================================================================
# TOTAL: 0
# PASS:  0
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[3]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Entering directory '/home/standup/libwally-core/src'
make  check-TESTS check-local
make[3]: Entering directory '/home/standup/libwally-core/src'
make[4]: Entering directory '/home/standup/libwally-core/src'
PASS: test_bech32
PASS: test_psbt
PASS: test_psbt_limits
PASS: test_tx
============================================================================
Testsuite summary for libwallycore 0.7.8
============================================================================
# TOTAL: 4
# PASS:  4
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src'
make[3]: Nothing to be done for 'check-local'.
make[3]: Leaving directory '/home/standup/libwally-core/src'
make[2]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Entering directory '/home/standup/libwally-core'
make[1]: Nothing to be done for 'check-am'.
make[1]: Leaving directory '/home/standup/libwally-core'
```
Infine, puoi installare:
```
$ sudo make install
```


## Prepararsi per Libwally

Quindi, come si usa Libwally in un programma? Come al solito, dovrai includere i file appropriati e collegare le librerie appropriate per il tuo codice.

### Includere i File

Ci sono un numero considerevole di file di inclusione possibili:


```
$ ls /usr/include/wally*
/usr/include/wally_address.h  /usr/include/wally_bip39.h   /usr/include/wally_elements.h  /usr/include/wally_script.h
/usr/include/wally_bip32.h    /usr/include/wally_core.h    /usr/include/wally.hpp	  /usr/include/wally_symmetric.h
/usr/include/wally_bip38.h    /usr/include/wally_crypto.h  /usr/include/wally_psbt.h	  /usr/include/wally_transaction.h
```
Fortunatamente, i nomi dei file corrispondono in gran parte alle sezioni nei [documenti](https://wally.readthedocs.io/en/latest/), quindi dovresti essere in grado di includere i file corretti in base a ciò che stai facendo, dopo aver incluso l'onnipresente `wally_core.h`.

### Collegare le Librerie

Dovrai anche collegare le librerie appropriate:

```
$ ls /usr/lib/libsecp* /usr/lib/libwally*
/usr/lib/libsecp256k1.a   /usr/lib/libwallycore.la  /usr/lib/libwallycore.so.0
/usr/lib/libsecp256k1.la  /usr/lib/libwallycore.so  /usr/lib/libwallycore.so.0.0.0
```
Principalmente, utilizzerai `libwallycore`.

## Configurare un Programma Libwally

Rispetto ad alcune delle librerie precedenti, Libwally è ridicolmente facile da inizializzare:

```
lw_response = wally_init(0);
```
E poi, quando hai finito, c'è una comoda funzione per ripulire tutta la memoria allocata:
```
wally_cleanup(0);
```
In entrambi i casi, l'argomento è per i flag, ma è attualmente impostato su `0`.

## Testare un Programma di Test Libwally

La directory src contiene [testwally.c](src/17_1_testwally.c), che mostra semplicemente come funzionano le funzioni di inizializzazione e pulizia.

Puoi compilarlo come segue:

```
$ cc testwally.c -lwallycore -o testwally
```
Dopo puoi eseguirlo:
```
$ ./testwally
Startup: 0
```
Il valore "Startup" è il ritorno di `wally_init`. Il valore `0` può inizialmente sembrare scoraggiante, ma è quello che vuoi vedere:
```
include/wally_core.h:#define WALLY_OK      0 /** Success */
```

## Install Libsodium


## Installare Libsodium

Dovresti anche installare Libsodium per avere accesso a un generatore di numeri casuali di alta qualità per scopi di test.

> :warning: **ATTENZIONE:** La generazione di numeri casuali può essere uno dei maggiori punti di vulnerabilità in qualsiasi software Bitcoin. Se lo fai male, esponi i tuoi utenti ad attacchi perché finiscono con chiavi Bitcoin non sicure, e questo non è un [problema teorico](https://github.com/BlockchainCommons/SmartCustodyBook/blob/master/manuscript/03-adversaries.md#adversary-systemic-key-compromise). BlockchainInfo una volta generò erroneamente lo 0,0002% delle loro chiavi, il che portò alla perdita temporanea di 250 Bitcoin. In breve: assicurati di essere completamente a tuo agio con la generazione dei numeri casuali. Potrebbe essere Libsodium, o potrebbe essere un metodo TRNG ancora più robusto.

Puoi scaricare un [tarball Libsodium](https://download.libsodium.org/libsodium/releases/) e poi seguire le istruzioni su [installazione di Libsodium](https://doc.libsodium.org/installation) per installarlo.

Prima, decomprimi:

```
$ tar xzfv /tmp/libsodium-1.0.18-stable.tar.gz 
```
Poi, aggiusta il file `configure` esattamente come hai fatto con le altre librerie finora:
```
< ac_default_prefix=/usr
---
> ac_default_prefix=/usr/local
```
Infine, `make`, `check` e `install`:
```
$ make
$ make check
...
============================================================================
Testsuite summary for libsodium 1.0.18
============================================================================
# TOTAL: 77
# PASS:  77
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
...
$ sudo make install
```
Questo corso userà `libsodium` solo per un piccolo (ma cruciale!) bit di generazione di entropia, ma tienilo d'occhio nella sezione successiva.

## Riepilogo: Configurare Libwally

Installando gli include e le librerie Libwally (e Libsodium), ottieni accesso a un numero di funzioni crittografiche e di portafoglio, che possono completare le tue librerie RPC e ZMQ (o il tuo `bitcoin-cli` da riga di comando).

Quindi, cosa puoi fare esattamente ora? È di questo che tratta il resto di questo capitolo.

## Cosa c'è dopo?

Scopri di più su "Programmare Bitcoin con Libwally" nel [Capitolo 17.2: Usare BIP39 in Libwally](17_2_Usare_BIP39_in_Libwally.md).


