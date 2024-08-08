# 13.2: Scrivere Script Multisig Complessi

Finora, i multisig descritti in questi documenti sono stati completamente semplici, della forma m-di-n o n-di-n. Tuttavia, potresti desiderare multisig più complessi, dove i cosigner variano o dove diverse opzioni potrebbero diventare disponibili nel tempo.

## Scrivere un Multisig Variabile

Un multisig variabile richiede numeri diversi di persone per firmare a seconda di chi sta firmando.

### Scrivere un Multisig con un Singolo Firmatario o Co-Firmatari

Immagina una corporazione dove sia il presidente o due dei tre vice-presidenti potrebbero concordare l'uso dei fondi.

Puoi scriverlo creando un'istruzione `IF`/`ELSE`/`ENDIF` che ha due blocchi, uno per il presidente e la sua firma uno-di-uno e uno per i vice-presidenti e le loro firme due-di-tre. Puoi quindi determinare quale blocco usare in base a quante firme ci sono nello script di sblocco. Usare `OP_DEPTH 1 OP_EQUAL` ti dirà se c'è un elemento nello stack, e da lì procedi.

Lo script di blocco completo sarebbe `OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF`

Se eseguito dal presidente, apparirebbe così:

```
Script: <sigPres> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: <SigPres> OP_DEPTH
Stack: [ <sigPres> 1 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> 1 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 1 1 OP_EQUAL
Stack: [ <sigPres> True ]
```
Poiché il risultato è `True`, lo Script ora si riduce all'istruzione `IF`:
```
Script: <pubKeyPres> OP_CHECKSIGNATURE
Running: True IF
Stack: [ <sigPres> ]

Script: OP_CHECKSIGNATURE
Stack: [ <sigPres> <pubKeyPres> ]

Script: 
Running: <sigPres> <pubKeyPres> OP_CHECKSIGNATURE
Stack: [ True ]
```
Se eseguito da due vice-presidenti, apparirebbe così:
```
Script: 0 <sigVPA> <sigVPB> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 0 <sigVPA> <sigVPB> OP_DEPTH
Stack: [ 0 <sigVPA> <sigVPB> 3 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> 3 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 3 1 OP_EQUAL
Stack: [ 0 <sigVPA> <sigVPB> False ]
```
Poiché il risultato è `False`, lo Script ora si riduce all'istruzione `ELSE`:
```
Script: 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Running: False IF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: OP_CHECKMULTISIG
Stack: [ 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 ]

Script: 
Running: 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Stack: [ ]
```
Potresti notare che la firma del Presidente utilizza solo un semplice `OP_CHECKSIGNATURE` piuttosto che il codice più complesso solitamente richiesto per un P2PKH. Possiamo permetterci di includere la chiave pubblica nello script di blocco, evitando la solita trafila, perché è hashata e non sarà rivelata (attraverso il `redeemScript`) fino a quando la transazione non sarà sbloccata. Questo consente anche a tutti i firmatari possibili di firmare utilizzando la stessa metodologia.

L'unico possibile problema è se il Presidente è smemorato e firma accidentalmente una transazione con uno dei suoi Vice-Presidenti, perché ricorda che si tratta di un multisig 2-di-3. Un'opzione è decidere che è una condizione di errore accettabile, perché il Presidente sta usando il multisig in modo errato. Un'altra opzione è trasformare il multisig 2-di-3 in un multisig 2-di-4, nel caso in cui il Presidente non tolleri errori: `OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> <pubKeyPres> 4 OP_CHECKMULTISIG ENDIF`. Questo permetterebbe al Presidente di firmare erroneamente con qualsiasi Vice-Presidente, ma non influirebbe se due Vice-Presidenti volessero (correttamente) firmare.

### Scrivere un Multisig con un Firmatario Richiesto

Un'altra possibilità multisig prevede un multisig m-di-n dove uno dei firmatari è richiesto. Questo può solitamente essere gestito scomponendo il multisig in più multisig m di n-1. Ad esempio, un multisig 2-di-3 dove uno dei firmatari è richiesto sarebbe in realtà due multisig 2-di-2, ciascuno includendo il firmatario richiesto.

Ecco un modo semplice per scriverlo:

```
OP_3DUP
2 <pubKeyRequired> <pubKeyA> 2  OP_CHECKMULTISIG
NOTIF

  2 <pubKeyRequired> <pubKeyB> 2  OP_CHECKMULTISIG

ENDIF
```
Lo script di sblocco sarebbe `0 <pubKeyRequired> <pubKeyA>` o `0 <pubKeyRequired> <pubKeyB>`.

Prima lo Script controllerebbe le firme contro `<pubKeyRequired> <pubKeyA>`. Se fallisce, controllerebbe contro `<pubKeyRequired> <pubKeyB>`.

Il risultato dell'ultimo `OP_CHECKMULTISIG` che è stato eseguito sarà lasciato in cima allo stack (anche se ci sarà spazzatura sotto di esso se il primo è riuscito).

## Scrivere un Multisig Escrow

Abbiamo parlato molto di escrow. I multisig complessi combinati con i timelock offrono un modo automatizzato per crearli in modo robusto.

Immagina l'acquirente di una casa Alice e il venditore della casa Bob che lavorano con un agente escrow. Il modo semplice per scrivere questo sarebbe come un multisig dove qualsiasi due delle tre parti potrebbero rilasciare il denaro: o il venditore e l'acquirente sono d'accordo o l'agente escrow prende il controllo e concorda con una delle parti: `2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISIG`.

Tuttavia, questo indebolisce il potere dell'agente escrow e consente al venditore e all'acquirente di prendere accidentalmente una decisione sbagliata tra loro — che è una delle cose che un sistema escrow è progettato per evitare. Quindi potrebbe essere che ciò che vogliamo veramente è il sistema che abbiamo appena esposto, dove l'agente escrow è una parte richiesta nel multisig 2-di-3: `OP_3DUP 2 <pubKeyEscrow> <pubKeyA> 2 OP_CHECKMULTISIG NOTIF 2 <pubKeyEscrow> <pubKeyB> 2 OP_CHECKMULTISIG ENDIF`.

Tuttavia, questo non passa il test del "camminare davanti a un autobus". Se l'agente escrow muore o fugge alle Bahamas durante l'escrow, l'acquirente e il venditore perderanno molti soldi. Qui entra in gioco un timelock. Puoi creare un test aggiuntivo che verrà eseguito solo se abbiamo superato il periodo di escrow. In questa situazione, permetti all'acquirente e al venditore di firmare insieme:

```
OP_3DUP
2 <pubKeyEscrow> <pubKeyA> 2 OP_CHECKMULTISIG
NOTIF

  OP_3DUP
  2 <pubKeyEscrow> <pubKeyB> 2 OP_CHECKMULTISIG
  NOTIF

    <+30Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    2 <pubKeyA> <pubKeyB> 2 OP_CHECKMULTISIG
    
  ENDIF
ENDIF
```
Prima, testi una firma per l'acquirente e l'agente escrow, poi una firma per il venditore e l'agente escrow. Se entrambe falliscono e sono passati 30 giorni, permetti anche una firma per l'acquirente e il venditore.

### Scrivere un Multisig Escrow Centrato sull'Acquirente

[BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki#Escrow_with_Timeout) offre un esempio diverso di questo tipo di escrow che non ha le protezioni extra per evitare di aggirare l'agente escrow, ma che dà ad Alice il controllo totale se l'escrow fallisce.

```
IF

    2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISIG 

ELSE

    <+30Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    <pubKeyA> OP_CHECKSIGNATURE

ENDIF
```
Qui, qualsiasi due dei tre firmatari possono rilasciare il denaro in qualsiasi momento, ma dopo 30 giorni Alice può recuperare i suoi soldi da sola.

Nota che questo Script richiede un `True` o `False` da passare per identificare quale ramo viene utilizzato. Questo è un modo più semplice e meno intensivo dal punto di vista computazionale per supportare i rami in un Bitcoin Script; è abbastanza comune.

All'inizio, il seguente `sigScript` sarebbe consentito: `0 <signer1> <signer2> True`. Dopo 30 giorni, Alice potrebbe produrre un `sigScript` come questo: `<sigA> False`.

## Riepilogo: Scrivere Script Multisig Complessi

Le multisignature più complesse possono essere solitamente create combinando firme o multisignature con condizionali e test. I multisig risultanti possono essere variabili, richiedendo numeri diversi di firmatari in base a chi sono e quando stanno firmando.

> :fire: ***Qual è la potenza degli script multisig complessi?*** Più di qualsiasi cosa vista finora, gli script multisig complessi sono veri e propri contratti intelligenti. Possono essere molto precisi su chi è autorizzato a firmare e quando. Possono supportare corporazioni multi-livello, partnership e escrow. Usare altre funzionalità potenti come i timelock può ulteriormente proteggere questi fondi, permettendo che siano rilasciati o addirittura restituiti in determinati momenti.

## Cosa c'è dopo?

Continua con "Progettare Script Bitcoin Reali" col [Capitolo 13.3: Potenziare Bitcoin con Scripts](13_3_Potenziare_Bitcoin_con_Scripts.md).
