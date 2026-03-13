# 4.1: Inviare monete in modo semplice

Il comando `bitcoin-cli` offre tre modi principali per inviare monete: un comando semplice; una transazione grezza; e una transazione grezza con calcolo. Ogni modo ha i propri vantaggi e svantaggi. Questo primo metodo per inviare monete è anche il più semplice.

## Imposta la commissione di transazione

Prima di inviare denaro sulla rete Bitcoin, dovresti pensare a quali commissioni di transazione pagherai.

> :book: ***Che cos'è una commissione di transazione?*** _Non esiste un pranzo gratis_. I minatori incorporano le transazioni nei blocchi perché sono pagati per farlo. Non solo vengono pagati dalla rete per effettuare il blocco, ma vengono anche pagati dagli utenti per includere le loro transazioni. Se non paghi una commissione, la tua transazione potrebbe rimanere bloccata... per sempre (o, fino a quando non verrà salvata da alcuni dei trucchi nel [Capitolo 5](05_0_Controllare_Transazioni_Bitcoin.md).

Quando utilizzi metodi semplici e automatizzati per creare transazioni, come delineato qui e nel [Capitolo 4.5: Inviare Monete con Transazione Grezza Automatizzata](04_5_Inviare_Monete_con_Transazione_Grezza_Automatizzata.md), Bitcoin calcolerà le commissioni di transazione per te. Questo viene fatto utilizzando le commissioni variabili, in cui "bitcoind" osserva quanto tempo impiegano le transazioni per essere confermate e calcola automaticamente quanto spendere.

Puoi contribuire a controllarlo inserendo valori razionali nel tuo `~/.bitcoin/bitcoin.conf`. I seguenti valori a basso costo garantirebbero che ci fosse una commissione di transazione minima di 10.000 satoshi per kByte di dati nella transazione e richiederebbero che le commissioni variabili raggiungano un buon importo per portare la transazione da qualche parte nei successivi sei blocchi.
```
mintxfee=0.0001
txconfirmtarget=6
```
Tuttavia, partendo dal presupposto che non vuoi aspettare mentre lavori su un tutorial, abbiamo adottato i seguenti valori più alti:
```
mintxfee=0.001
txconfirmtarget=1
```
Dovresti inserirli in `~/.bitcoin/bitcoin.conf`, nella sezione principale, verso l'inizio del file o, se vuoi essere sicuro di non usarlo mai altrove, nella sezione `[test]`.

Per completare questo tutorial, siamo disposti a spendere 100.000 satoshi per kB su ogni transazione (circa $ 10!) e vogliamo portare ogni transazione al blocco successivo! (Per metterlo in prospettiva, una transazione tipica è compresa tra 0,25 kB e 1 kB, quindi in realtà pagherai più di $ 2,50 circa $ 10... se fossero soldi veri.)

Dopo aver modificato il tuo file `bitcoin.conf`, ti consigliamo di terminare e riavviare bitcoind.


```
$ bitcoin-cli stop
$ bitcoind -daemon
```

## Ottieni un indirizzo

Hai bisogno di un posto dove inviare le tue monete. Di solito, qualcuno ti invia un indirizzo e magari ti dà una firma per dimostrare di possedere quell'indirizzo. In alternativa, potrebbero fornirti un codice QR da scansionare, in modo da non poter commettere errori durante la digitazione dell'indirizzo. Nel nostro caso, invieremo monete a `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi`, che è un indirizzo di ritorno per un vecchio faucet Tesetnet.

> :book: ***Cos'è un codice QR?*** Un codice QR è semplicemente la codifica di un indirizzo Bitcoin. Molti portafogli genereranno codici QR per te, mentre alcuni siti convertiranno un indirizzo in un codice QR (da evitare). Ovviamente, dovresti accettare solo un codice QR da un sito di cui ti fidi assolutamente (nessuno). Potrebbero alterare il codice e ricevere i soldi destinati a te.

## Invia le monete

Ora sei pronto per inviare alcune monete. In realtà questo è abbastanza semplice tramite la riga di comando. Basta usare `bitcoin-cli sendtoaddress [address] [amount]`. Quindi, per inviare una piccola moneta all'indirizzo `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi` è sufficiente:
```
$ txid=$(bitcoin-cli sendtoaddress n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi 0.001)
$ echo $txid
93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8
```
> 🙏 Per mantenere in vita i faucet testnet, prova a utilizzare l'indirizzo di ritorno dello stesso faucet che hai utilizzato nel capitolo precedente sulla ricezione delle transazioni.

Assicurati che l'indirizzo che scrivi sia quello dove vuoi che vadano i soldi. Controlla due volte. Se commetti errori in Bitcoin, non puoi tornare indietro.

Riceverai un txid indietro quando eseguirai questo comando.

> ❕ Potresti ritrovarti con un codice di errore se non disponi di fondi sufficienti nel tuo portafoglio per inviare la transazione. A seconda del tuo saldo attuale `bitcoin-cli getbalance` potresti dover modificare l'importo da inviare per tenere conto dell'importo inviato insieme alla commissione di transazione. Se il tuo saldo attuale è 0,001, potresti provare a inviare 0,0001. In alternativa, sarebbe meglio sottrarre la commissione prevista indicata nel messaggio di errore dal saldo attuale. Questa è una buona pratica poiché molti portafogli si aspettano che tu calcoli il tuo importo + le commissioni quando prelevi, anche tra gli exchange più popolari.

> :warning: **AVVERTENZA:** Il comando `bitcoin-cli` genera effettivamente comandi JSON-RPC quando comunica con bitcoind. Possono essere davvero schizzinosi. Questo è un esempio: se elenchi l'importo del bitcoin senza lo zero iniziale (cioè ".1" invece di "0.1"), `bitcoin-cli fallirà` con un messaggio misterioso.

> :warning: **AVVERTENZA:** Anche se presti attenzione ai tuoi input, potresti visualizzare il messaggio `"Fee estimation failed. Fallbackfee is disabled." `(Stima della tariffa non riuscita. La tariffa di riserva è disabilitata). Fondamentalmente, ciò significa che il tuo `bitcoind` locale non dispone di informazioni sufficienti per stimare le commissioni. Non dovresti mai vederlo se hai aspettato che la tua blockchain si sincronizzi e configuri il tuo sistema con Bitcoin Standup. Ma se non sei completamente sincronizzato, potresti vedere questo. Potrebbe anche darsi che tu non stia utilizzando un `bitcoin.conf` standard: la voce `blocksonly=1` impedirà al tuo `bitcoind` di stimare le commissioni.

## Esamina la tua transazione

Puoi controllare la tua transazione utilizzando il tuo ID transazione:

```
{
  "amount": -0.00100000,
  "fee": -0.00022200,
  "confirmations": 0,
  "trusted": true,
  "txid": "93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8",
  "walletconflicts": [
  ],
  "time": 1592604194,
  "timereceived": 1592604194,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.00100000,
      "vout": 1,
      "fee": -0.00022200,
      "abandoned": false
    }
  ],
  "hex": "0200000001e982921bb0189afc486e20bb05cc5825c71a0ba8868043ed04ece9ab0cb12a8e010000006a47304402200fc493a01c5c9d9574f7c321cee6880f7f1df847be71039e2d996f7f75c17b3d02203057f5baa48745ba7ab5f1d4eed11585bd8beab838b1ca03a4138516fe52b3b8012102fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9bafeffffff02e8640d0000000000160014d37b6ae4a917bcc873f6395741155f565e2dc7c4a0860100000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac780b1b00"
}
```
Puoi vedere non solo l'importo trasferito (.001 BTC) ma anche una commissione di transazione (.000222 BTC), che è circa un quarto della commissione minima di .001 BTC/kB impostata, il che suggerisce che la transazione è stata di circa un dimensione di un quarto di kB.

Mentre aspetti che questa transazione venga cancellata, noterai che `bitcoin-cli getbalance` mostra che tutti i tuoi soldi sono spariti (o, almeno, tutti i tuoi soldi da una singola transazione in entrata). Allo stesso modo, `bitcoin-cli listunspent` mostrerà che un'intera transazione è andata perduta, anche se era superiore a quello che volevi inviare. C'è una ragione per questo: ogni volta che ricevi soldi, devi mandarli tutti insieme, e devi fare un po' di ginnastica se vuoi davvero tenerne una parte! Ancora una volta, `sendtoaddress` si prende cura di tutto questo per te, il che significa che non devi preoccuparti di apportare modifiche finché non invii una transazione grezza. In questo caso, una nuova transazione apparirà con la tua modifica quando la tua spesa verrà incorporata in un blocco.

## Riepilogo: inviare monete in modo semplice

Per inviare monete nel modo più semplice, assicurati che le impostazioni predefinite della transazione siano logiche, ottieni un indirizzo e invia le monete lì. Ecco perché lo chiamano facile!

> :fire: ***Qual è il potere di inviare monete in modo semplice?***

> _I vantaggi._ È facile. Non devi preoccuparti di cose arcane come gli UTXO. Non è necessario calcolare manualmente le commissioni di transazione, quindi non è probabile che tu commetta errori che ti costeranno ingenti somme di denaro. Se il tuo unico obiettivo è sederti al computer e inviare denaro, questa è la strada da percorrere.

> _Gli svantaggi._ Il livello è alto. Hai pochissimo controllo su ciò che sta accadendo e non puoi fare nulla di speciale. Se hai intenzione di scrivere un software Bitcoin più complesso o desideri una comprensione più profonda di come funziona Bitcoin, allora il modo più semplice è solo un noioso diversivo prima di arrivare alle cose vere.

## Con cosa andiamo avanti?

Andiamo avanti col'invio di transazioni Bitcoin con [inviare Monete con Transazione Grezza](04_4_Inviare_Monete_con_Transazione_Grezza.md).
