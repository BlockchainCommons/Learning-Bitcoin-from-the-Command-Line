# 13.3: Potenziare Bitcoin con gli Script

Gli Script Bitcoin possono andare ben oltre i relativi semplici strumenti finanziari dettagliati fino ad oggi. Sono anche la base della maggior parte degli usi complessi della rete Bitcoin, come dimostrato da questi esempi reali di funzionalità off-chain, tratti dagli esempi della Lightning Network in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

## Blocco per la Lightning Network

La [Lightning Network](https://rusty.ozlabs.org/?p=450) è un canale di pagamento che consente agli utenti di prelevare fondi off-chain e impegnarsi in numerose microtransazioni prima di finalizzare il canale di pagamento e riportare i fondi in Bitcoin. I vantaggi includono commissioni più basse e velocità di transazione più elevate. Viene discussa in maggior dettaglio, con esempi di come utilizzarla dalla riga di comando, a partire dal [Capitolo 19](19_0_Understanding_Your_Lightning_Setup.md).

[BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) contiene alcuni esempi di come queste transazioni off-chain potrebbero essere generate, utilizzando script di blocco Bitcoin.

### Blocco con Transazioni di Impegno Revocabili

Il trucco con Lightning è il fatto che è off-chain. Per utilizzare Lightning, i partecipanti bloccano congiuntamente i fondi sulla blockchain Bitcoin con una multisignature n-di-n. Quindi, si impegnano in una serie di transazioni tra loro. Ogni nuova "transazione di impegno" divide quei fondi congiunti in modo diverso; queste transazioni sono parzialmente firmate ma _non vengono messe sulla blockchain_.

Se hai una massa di transazioni non pubblicate, ognuna delle quali _potrebbe_ essere inserita nella Blockchain, come fai a impedire a uno dei partecipanti di tornare a una vecchia transazione più vantaggiosa per loro? La risposta è _revoca_. Un esempio semplificato in BIP 112, che offre una delle tappe per Lightning, mostra come: dai al partecipante che sarebbe danneggiato dalla reversione a una transazione revocata la possibilità di reclamare i fondi lui stesso se l'altro partecipante cerca illegittimamente di usare la transazione revocata.

Ad esempio, supponiamo che Alice e Bob aggiornino la transazione di impegno per dare più fondi a Bob (effettivamente: Alice ha inviato fondi a Bob tramite la rete Lightning). Firmano parzialmente nuove transazioni, ma offrono anche ciascuno il proprio `revokeCode` per le transazioni precedenti. Questo garantisce effettivamente che non pubblicheranno transazioni precedenti, perché farlo permetterebbe alla loro controparte di reclamare quei fondi precedenti.

Allora, come appariva la vecchia transazione? Era una transazione di impegno che mostrava fondi destinati ad Alice, prima che li desse a Bob. Aveva uno script di blocco come segue:

```
OP_HASH160 
<revokeHash> 
OP_EQUAL
    
IF
        
    <pubKeyBob>
    
ELSE

    <+24Hours> 
    OP_CHECKSEQUENCEVERIFY 
    OP_DROP
    <pubKeyAlice>
    
ENDIF
OP_CHECKSIG
```
Il blocco `ELSE` è dove Alice otteneva i suoi fondi, dopo un ritardo di 24 ore. Tuttavia ora è stato superato; questo è il punto centrale di un canale di pagamento in stile Lightning, dopo tutto. In questa situazione, questa transazione non dovrebbe mai essere pubblicata. Bob non ha alcun incentivo a farlo perché ha una nuova transazione, che lo avvantaggia di più perché ha ricevuto alcuni dei fondi di Alice. Alice non ha neanche incentivo, perché perde i fondi se ci prova a causa di quel `revokeCode`. Quindi nessuno mette la transazione sulla blockchain e le transazioni off-chain continuano.

Vale la pena esplorare come questo script funzionerebbe in una varietà di situazioni, la maggior parte delle quali coinvolge Alice che cerca di barare tornando a questa vecchia transazione, che descrive i fondi _prima_ che Alice ne inviasse alcuni a Bob.

#### Eseguire lo Script di Blocco per Alice che Bara, con Codice di Revoca

Alice potrebbe provare a usare il codice di revoca che ha dato a Bob per reclamare immediatamente i fondi. Scrive uno script di blocco di `<sigAlice> <revokeCode>`:

```
Script: <sigAlice> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <sigAlice> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> True ]
```
L'`OP_EQUAL` alimenta l'istruzione `IF`. Poiché Alice usa il `revokeCode`, entra nel ramo che le permette di riscattare immediatamente i fondi, riducendo il resto dello script a `<pubKeyBob>` (all'interno del condizionale) e `OP_CHECKSIG` (dopo).
```
Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <sigAlice> ]
```
Maledizione! Solo Bob può firmare immediatamente usando il `redeemCode`!
```
Script:  OP_CHECKSIG
Stack: [ <sigAlice> <pubKeyBob> ]

Script:
Running: <sigAlice> <pubKeyBob> OP_CHECKSIG
Stack: [ False ]
```
#### Eseguire lo Script di Blocco per Alice che Bara, senza Codice di Revoca

Quindi cosa succede se Alice invece prova a usare la propria firma, senza il `revokeCode`? Usa uno script di sblocco di `<sigAlice> <notRevokeCode>`.
```
Script: <sigAlice> 0 OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> 0 ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: 0 OP_HASH160 
Stack: [ <sigAlice> <0Hash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <0Hash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <0Hash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> False ]
```
Ora ci riduciamo all'istruzione `ELSE` e a ciò che viene dopo il condizionale:
```
Script: <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: False IF
Stack: [ <sigAlice> ]

Script: OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Stack: [ <sigAlice> <+24Hours> ]
```
E poi Alice viene nuovamente sventata perché non sono passate 24 ore!
```
Script: OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: <+24Hours> OP_CHECKSEQUENCEVERIFY 
Stack: [ <sigAlice> <+24Hours> ] — Script EXITS
```
#### Eseguire lo Script di Blocco per Bob Vittima

Ciò significa che Bob ha 24 ore per reclamare i suoi fondi se Alice tenta mai di barare, usando il `<revokeCode>` e la sua firma come script di sblocco:

```
Script: <SigBob> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <SigBob> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <SigBob> True ]

Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <SigBob> ]

Script:  OP_CHECKSIG
Stack: [ <SigBob> <pubKeyBob> ]

Script:
Running: <SigBob> <pubKeyBob> OP_CHECKSIG
Stack: [ True ]
```
#### Eseguire lo Script di Blocco per Alice Virtuosa

Tutte le transazioni di impegno di Alice sono bloccate con questo stesso script di blocco, siano state revocate o meno. Ciò significa che anche la più recente transazione di impegno, che è quella attualmente valida, è bloccata con essa. Alice non ha mai inviato una nuova transazione a Bob e quindi non gli ha mai inviato il precedente `revokeCode`.

In questa situazione, potrebbe pubblicare virtuosamente la transazione, chiudendo il canale proto-Lightning. Mette la transazione sulla catena e aspetta 24 ore. Bob non può fare nulla al riguardo perché non ha il codice di revoca. Quindi, dopo l'attesa, Alice reclama i suoi fondi. (Bob fa la stessa cosa con la sua ultima transazione di impegno.)

### Blocco con Contratti Hashed Time-Lock

Le Transazioni di Impegno Revocabili erano solo una tappa per Lightning. La vera Lightning Network utilizza un meccanismo più complesso chiamato [contratto hashed timelock](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts), o HTLC.

Lo scopo principale degli HTLC è creare una rete completa di partecipanti. Le transazioni non sono più solo tra una coppia di partecipanti che sono entrati insieme nella rete, ma ora possono essere tra persone precedentemente non associate. Quando i fondi vengono inviati, viene creata una serie di transazioni, ciascuna bloccata con un `secretHash`. Quando viene rivelato il corrispondente `secretCode`, l'intera serie di transazioni può essere spesa. Questo è ciò che consente alle singole transazioni di diventare effettivamente una rete.

C'è anche un po' più di complessità negli script di blocco della Lightning Network. Ci sono blocchi separati per il mittente e il destinatario di ciascuna transazione che sono più divergenti rispetto alle diverse transazioni di impegno menzionate nella sezione precedente. Mostreremo entrambi, per dimostrare la potenza di questi script di blocco, ma non ci soffermeremo su come interagiscono tra loro.

#### Blocco della Transazione del Destinatario

Ancora una volta, iniziamo a guardare la transazione di impegno di Alice, che mostra i fondi che ha ricevuto:
```
OP_HASH160 
OP_DUP 
<secretHash> 
OP_EQUAL
    
IF

    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyAlice>
    
ELSE

    <revokeHash> 
    OP_EQUAL
        
    OP_NOTIF
            
        <Date> 
        OP_CHECKLOCKTIMEVERIFY 
        OP_DROP
        
    ENDIF
        
    <pubKeyBob>
    
ENDIF
    
OP_CHECKSIG
```
La chiave di questi nuovi HTLC è il `secretHash`, che abbiamo detto è ciò che consente a una transazione di estendersi alla rete. Quando la transazione è passata dal suo originatore al destinatario previsto, viene rivelato il `secretCode`, che consente a tutti i partecipanti di creare un `secretHash` e sbloccare l'intera rete di pagamenti.

Dopo che il `secretCode` è stato rivelato, si apre il ramo `IF`: Alice può reclamare i fondi 24 ore dopo che la transazione è stata inserita nella rete Bitcoin.

Tuttavia, c'è anche l'opportunità per Bob di reclamare i suoi fondi, che appare nel ramo `ELSE`. Può farlo se la transazione è stata revocata (ma Alice la mette comunque sulla blockchain), _o se_ si è verificato un timeout assoluto.

#### Blocco della Transazione del Mittente

Ecco il script di blocco alternativo della transazione di impegno utilizzato dal mittente:

```
OP_HASH160 
OP_DUP 
<secretHash>
OP_EQUAL
OP_SWAP 
<revokeHash> 
OP_EQUAL 
OP_ADD
   
IF

    <pubKeyAlice>

ELSE

    <Date>
    OP_CHECKLOCKTIMEVERIFY
    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyBob>

ENDIF
OP_CHECKSIG
```
La parte iniziale del loro Script è piuttosto ingegnosa e quindi vale la pena eseguirla:
```
Initial Script: <suppliedCode> OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ ]

Initial Script: OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ <suppliedCode> ]

Initial Script: OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedCode> OP_HASH160 
Stack: [ <suppliedHash> ]

Initial Script: <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> OP_DUP 
Stack: [ <suppliedHash> <suppliedHash> ]

Initial Script: OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ <suppliedHash> <suppliedHash> <secretHash> ]

Initial Script: OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <secretHash> OP_EQUAL
Stack: [ <suppliedHash> <wasItSecretHash?> ]

Initial Script: <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <wasItSecretHash?> OP_SWAP
Stack: [ <wasItSecretHash?> <suppliedHash> ]

Initial Script: OP_EQUAL OP_ADD
Stack: [ <wasItSecretHash?> <suppliedHash> <revokeHash> ]

Initial Script: OP_ADD
Running: <suppliedHash> <revokeHash> OP_EQUAL
Stack: [ <wasItSecretHash?> <wasItRevokeHash?> ]

Initial Script: 
Running: <wasItSecretHash?> <wasItRevokeHash?> OP_ADD
Stack: [ <wasItSecretOrRevokeHash?> ]
```
Eseguire lo script rivela che i controlli iniziali, sopra il `IF`/`ELSE`/`ENDIF`, determinano se l'hash era _sia_ il `secretCode` _sia_ il `revokeCode`. Se sì, Alice può prendere i fondi nel primo blocco. Se no, Bob può prendere i fondi, ma solo dopo che Alice ha avuto la sua occasione e dopo che entrambi il timeout di 24 ore e il timeout assoluto sono passati.

#### Comprendere gli HTLC

Gli HTLC sono piuttosto complessi, e questa panoramica non cerca di spiegare tutte le loro complessità. La [panoramica](https://rusty.ozlabs.org/?p=462) di Rusty Russell spiega di più, e c'è ancora più dettaglio nel suo documento [Deployable Lightning](https://github.com/ElementsProject/lightning/blob/master/doc/deployable-lightning.pdf). Ma non preoccuparti se alcune delle complessità ti sfuggono ancora, in particolare le interrelazioni dei due script.

Per i fini di questo tutorial, ci sono due lezioni importanti per gli HTLC:

   * Comprendere che una struttura molto complessa come un HTLC può essere creata con Bitcoin Script.
   * Analizzare come eseguire ciascuno dei due script HTLC.
   
Vale la pena eseguire ciascuno dei due script HTLC attraverso ciascuna delle sue permutazioni, un elemento dello stack alla volta.

## Riepilogo: Potenziare Bitcoin con gli Script

Stiamo chiudendo il nostro esame degli Script Bitcoin guardando a quanto possano essere veramente potenti. In 20 opcodes o meno, uno Script Bitcoin può costituire la base di un intero canale di pagamento off-chain. Allo stesso modo, le sidechain a due vie sono il prodotto di meno di venti opcodes, come brevemente notato anche in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

Se hai mai visto funzionalità Bitcoin complesse o sistemi adiacenti a Bitcoin, probabilmente sono stati costruiti su Script Bitcoin. E ora hai tutti gli strumenti per fare lo stesso tu stesso.

## Cosa c'è dopo?

Prosegui con "Usare Tor" col [Capitolo 14: Usare Tor](14_0_Usare_Tor.md).

Oppure, se preferisci, ci sono due percorsi alternativi:

Se vuoi rimanere concentrato su Bitcoin, prosegui con "Programmazione con RPC" col [Capitolo 16.0: Parlare a Bitcoind con C](16_0_Parlare_a_Bitcoind_con_C.md).

Oppure, se vuoi rimanere concentrato sulla riga di comando perché non sei un programmatore, puoi passare al [Capitolo 19.0: Comprendere la Configurazione Lightning](19_0_Comprendere_la_Configurazione_Lightning.md) per continuare la tua formazione sulla riga di comando con Lightning Network.

