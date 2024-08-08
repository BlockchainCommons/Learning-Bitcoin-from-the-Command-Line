# 9.5: Script di un P2WPKH

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettor avvisato.

I P2PKH vanno bene per spiegare il modo fondamentale in cui funzionano gli script Bitcoin, ma che dire degli script nativi SegWit P2WPKH, che stanno diventando sempre più la maggior parte delle transazioni Bitcoin? A quanto pare, gli indirizzi P2WPKH non utilizzano script Bitcoin come fanno gli indirizzi Bitcoin tradizionali, quindi questa sezione è in realtà una digressione dallo scripting di questo capitolo, ma importante, perché delinea l'_altro_ modo principale in cui i Bitcoin possono essere transati.

## Visualizzare uno script P2WPKH

È abbastanza facile vedere come appare uno script P2WPKH. La seguente transazione grezza è stata creata spendendo un UTXO P2WPKH e quindi inviando il denaro a un indirizzo di resto P2WPKH, proprio come abbiamo fatto con un indirizzo legacy nel [Capitolo 9.1](09_1_Le_basi_delle_transazioni.md).
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "bdf8f12768a9870d41ac280f8bb4f8ecd9d2fa66fffc75606811f5751c17cb3a",
  "hash": "ec09c84cae48694bec7fd3461b3c5b38a76829c56e9d876037bf2484d443174b",
  "version": 2,
  "size": 191,
  "vsize": 110,
  "weight": 437,
  "locktime": 0,
  "vin": [
    {
      "txid": "3f5417bc7a3a4144d715f3f006d35ea2b405f06091cbb9ce492e04ccefe02b18",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001",
        "03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 92a0db923b3a13eb576a40c4b35515aa30206cba",
        "hex": "001492a0db923b3a13eb576a40c4b35515aa30206cba",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qj2sdhy3m8gf7k4m2grztx4g44gczqm96y6sszv"
        ]
      }
    }
  ]
}
```
There are probably two surprising things here: (1) There's no `scriptSig` to unlock the previous transaction; and (2) the `scriptPubKey` to lock the new transaction is just `0 92a0db923b3a13eb576a40c4b35515aa30206cb`.

Questo è molto semplice perché P2WPKH funziona diversamente!

## Comprendere una transazione P2WPKH

Una transazione P2WPKH contiene tutte le stesse informazioni di una transazione P2PKH classica, ma le colloca in posti strani, non all'interno di un tradizionale Bitcoin Script - e, questo è lo scopo esatto delle transazioni SegWit, per estrarre le informazioni `testimone`, cioè chiavi pubbliche e firme, fuori dalla transazione.

Ma, se guardi attentamente, vedrai che il vuoto `scriptSig` è stato sostituito con due voci in una nuova sezione `txinwitness`. Se ne esamini le dimensioni e la formattazione, dovrebbero sembrarti familiari: sono una firma e una chiave pubblica. Allo stesso modo, se guardi "scriptPubKey", vedrai che è composto da uno "0" (in realtà: "OP_0", è il numero di versione di SegWit) e un altro numero lungo, che è l'hash della chiave pubblica.

Ecco un confronto tra i nostri due esempi:

| Type | PubKeyHash | PubKey | Signature |
|----------------|----------|-------------|---------|
| SegWit | 92a0db923b3a13eb576a40c4b35515aa30206cba | 03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903 | 3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001 |
| non-SegWit | 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c |

Allora come funziona? Dipende dal vecchio codice che lo interpreta come una transazione valida e dal nuovo codice che sa controllare le nuove informazioni "testimone".

### Leggere uno script SegWit su un nodo non aggiornato

Se un nodo non è stato aggiornato per supportare SegWit, esegue il solito trucco di concatenare `scriptSig` e `scriptPubKey`. Questo produce: `0 92a0db923b3a13eb576a40c4b35515aa30206cba` (perché c'è solo uno `scriptPubKey`). L'esecuzione produrrà una pila con tutto ciò che contiene in ordine inverso:
```
$ btcdeb '[0 92a0db923b3a13eb576a40c4b35515aa30206cba]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
miniscript failed to parse script; miniscript support disabled
valid script
2 op script loaded. type `help` for usage information
script                                   |  stack 
-----------------------------------------+--------
0                                        | 
92a0db923b3a13eb576a40c4b35515aa30206cba | 
#0000 0
btcdeb> step
		<> PUSH stack 
script                                   |  stack 
-----------------------------------------+--------
92a0db923b3a13eb576a40c4b35515aa30206cba | 0x
#0001 92a0db923b3a13eb576a40c4b35515aa30206cba
btcdeb> step
		<> PUSH stack 92a0db923b3a13eb576a40c4b35515aa30206cba
script                                   |                                   stack 
-----------------------------------------+-----------------------------------------
                                         | 92a0db923b3a13eb576a40c4b35515aa30206cba
                                         | 0x
```
Gli script Bitcoin sono considerati riusciti se c'è qualcosa nello stack ed è diverso da zero, quindi gli script SegWit riescono automaticamente sui vecchi nodi purché `scriptPubKey` venga creato correttamente con un hash pub-key diverso da zero. Questa è chiamata transazione "chiunque può spendere", perché i vecchi nodi le hanno verificate come corrette senza bisogno di firme.

> :book: ***Perché i vecchi nodi non possono rubare gli UTXO SegWit?*** SegWit è stato abilitato sulla rete Bitcoin quando il 95% dei minatori ha segnalato di essere pronto per iniziare a utilizzarlo. Ciò significa che solo il 5% dei nodi a quel punto avrebbe potuto registrare come valide le transazioni SegWit “chiunque può spendere” senza passare attraverso il lavoro adeguato di controllo del `txinwitness`. Se incorporassero erroneamente un UTXO non valido che chiunque può spendere in un blocco, il restante 95% dei nodi si rifiuterebbe di convalidare quel blocco, e quindi rimarrebbe rapidamente orfano anziché essere aggiunto alla blockchain "principale". (Certamente, il 51% dei nodi potrebbe scegliere di smettere di interpretare correttamente le transazioni SegWit, ma il 51% dei nodi può fare qualsiasi cosa su una rete di consenso come una blockchain.)

Poiché i vecchi nodi vedono sempre gli script SegWit come corretti, li verificheranno sempre, anche senza comprenderne il contenuto.

### Leggere uno script SegWit su un nodo aggiornato

Un nodo che capisce come funziona SegWit fa esattamente le stesse cose che farebbe con un vecchio script P2PKH, ma non usa uno script di per sé: sa solo che deve eseguire l'hashing della chiave pubblica nel `txinwitness`, controlla quello contro la chiave con hash dopo il numero di versione in "scriptPubKey" e quindi eseguire "OP_CHECKSIG" sulla firma e sulla chiave pubblica in "txinwitness".

Quindi, è un altro modo di fare la stessa cosa, ma senza avere gli script integrati nelle transazioni. (Il processo è invece integrato nel software del nodo.)

## Riepilogo: Programmare un transazione Pay to Witness Public Key Hash

In larga misura _non_ scrivi un P2WPKH. Invece, Bitcoin Core crea la transazione in un modo diverso, posizionando le informazioni del testimone in un posto diverso rispetto al tradizionale `scriptSig`. Ciò significa che i P2WPKH sono una digressione dagli script Bitcoin di questa parte del libro, perché sono un'espansione di Bitcoin che si allontana dagli script tradizionali.

Tuttavia, SegWit è stato anche un uso intelligente degli script Bitcoin. Sapendo che ci sarebbero stati nodi che non si sarebbero aggiornati e che avrebbero dovuto rimanere compatibili con le versioni precedenti, gli sviluppatori hanno creato il formato P2WPKH in modo da generare uno script sempre convalidato sui vecchi nodi (pur mantenendo lo script che fornisce informazioni ai nuovi nodi nel formato di un numero di versione e una chiave pubblica hashata).

Quando programmi dalla riga di comando, fondamentalmente non devi preoccuparti di questo, a parte sapere che non troverai script tradizionali nelle transazioni SegWit grezze (che, ancora una volta, era il punto).

## Che segue ?

Continua "Programmando Bitcoin" col [Capitolo 10: Inserire Scripts di Bitcoin in Transazioni P2SH](10_0_Inserire_Scripts_di_Bitcoin_in_Transazioni_P2SH.md).
