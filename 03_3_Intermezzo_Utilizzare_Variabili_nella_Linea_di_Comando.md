# Interludio: utilizzare le variabili nella riga di comando

La sezione precedente ha dimostrato una serie di comandi della riga di comando utilizzati senza offuscamenti o interferenze. Tuttavia, spesso non è il modo migliore per eseguire Bitcoin dalla riga di comando. Poiché hai a che fare con variabili lunghe, complesse e illeggibili, è facile commettere un errore se le stai copiando (o, satoshi forfend, se le stai digitando a mano). Poiché queste variabili possono fare la differenza tra ricevere e perdere denaro reale, non vuoi commettere errori. Per questi motivi, consigliamo vivamente di utilizzare variabili della riga di comando per salvare indirizzi, firme o altre lunghe stringhe di informazioni ogni volta che è ragionevole farlo.

Se utilizzi `bash`, puoi salvare le informazioni in una variabile come questa:
```
$ VARIABLE=$(command)
```
Questa è una semplice sostituzione di comando, l'equivalente di ``VARIABILE=`comando` ``. Viene eseguito il comando tra parentesi, quindi assegnato alla VARIABILE.

Per creare un nuovo indirizzo sarebbe quindi simile a questo:
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress "" legacy)
```
Questo comando cancella la variabile `NEW_ADDRESS_1` per sicurezza, quindi la riempiono con i risultati del comando `bitcoin-cli getnewaddress`.

Dalla tua Shell puoi utilizzare il comando `echo` per controllare il tuo (nuovo) indirizzo:

```
$ echo $NEW_ADDRESS_1
mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE
```
Poiché hai il tuo indirizzo in una variabile, ora puoi firmare facilmente un messaggio per quell'indirizzo, senza preoccuparti di digitarlo in modo errato. Ovviamente salverai anche quella firma in una variabile!
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
$ echo $NEW_SIG_1
IPYIzgj+Rg4bxDwCyoPiFiNNcxWHYxgVcklhmN8aB2XRRJqV731Xu9XkfZ6oxj+QGCRmTe80X81EpXtmGUpXOM4=
```
Per praticità nella tutorial utilizzeremo questo tipo di salvataggio delle informazioni nelle variabili.

> :book: ***Quando non è pratico utilizzare le variabili della riga di comando?*** Le variabili della riga di comando non sono pratiche se è necessario utilizzare le informazioni in un posto diverso dalla riga di comando. Ad esempio, salvare la tua firma potrebbe non essere utile se devi semplicemente inviarla a qualcun altro tramite email. Inoltre, alcuni comandi futuri restituiranno oggetti JSON invece di semplici informazioni e le variabili non potranno essere utilizzate per acquisire tali informazioni... almeno non senza _poco_ lavoro in più.

## Riepilogo: utilizzare le variabili nella riga di comando

Le variabili di shell possono essere utilizzate per contenere lunghe _Bitcoin strings_, riducendo al minimo le possibilità di errori.

## Cosa viene dopo?

[Capitolo 3.4: Ricevere una Transazione](03_4_Ricevere_una_Transazione.md).
