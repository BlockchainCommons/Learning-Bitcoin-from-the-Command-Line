# Interludio: Introduzione a Bitcoin

Prima di iniziare a programmare Bitcoin (e Lightning), è necessario avere una conoscenza di base di cosa sono e come funzionano. Questa sezione fornisce una panoramica. All'interno del documento troverete ulteriori definizioni; questa ha solo lo scopo di gettare le basi.

## Riguardo a Bitcoin

Bitcoin è un sistema programmatico che consente il trasferimento della valuta bitcoin. È abilitato da un sistema decentralizzato e peer-to-peer di nodi, che comprendono nodi completi, portafogli e minatori. Lavorando insieme, garantiscono che le transazioni in bitcoin siano veloci e non ripudiabili. Grazie alla natura decentralizzata del sistema, queste transazioni sono anche resistenti alla censura e, se ben utilizzate, possono offrire altri vantaggi come la pseudonimia e la non correlazione.

Ovviamente Bitcoin è il cuore di questo libro, ha dato inizio a molti altri sistemi tra cui la blockchain e Lightning, entrambi sono descritti in dettaglio in questo tutorial. Sono escluse da questo tutorial molte altre criptovalute come Ethereum o Litecoin.

**_Come vengono trasferite le monete?_** La valuta Bitcoin non è costituita da monete fisiche. È una serie infinita di riassegnazioni di proprietà. Quando una persona invia monete a un'altra, il trasferimento viene memorizzato come una transazione. È la transazione che registra effettivamente la proprietà del denaro, non un token locale nel portafoglio del proprietario o nel suo computer.

**_A chi si possono inviare monete?_** La maggior parte delle transazioni in bitcoin prevede l'invio di monete a singole persone (o almeno a singoli indirizzi Bitcoin). Tuttavia, è possibile utilizzare metodologie più complesse per inviare bitcoin a gruppi di persone o a script. Queste metodologie hanno nomi come P2PKH, multisig e P2SH.

**_Come sono archiviate le transazioni?_** Le transazioni vengono raggruppate in blocchi di dati più grandi, e poi scritti sul libro mastro della blockchain. Un blocco è costruito in modo tale da non poter essere sostituito o riscritto una volta che diversi blocchi sono stati costruiti sopra (dopo) di esso. Questo è ciò che rende le transazioni in bitcoin non ripudiabili: il libro mastro globale decentralizzato in cui tutto viene registrato è di fatto un database permanente e immutabile.

Tuttavia, il processo di costruzione di questi blocchi è stocastico: è in qualche modo casuale, quindi non si può mai essere certi che una transazione sarà inserita in un blocco specifico. Teoricamente, e in rarissime circonstanze, potrebbero esseci dei cambiamenti nei blocchi, ma solo se sono _molto_ recenti. Quindi, le transazioni diventano non ripudiabili, permanenti e immutabili dopo pochissimo tempo.

**_Come sono protette le transazioni?_** I fondi contenuti in una transazione Bitcoin sono bloccati da un puzzle crittografico. Questi puzzle sono progettati in modo da poter essere facilmente risolti dalla persona a cui sono stati inviati i fondi. Ciò avviene grazie alla potenza della crittografia a chiave pubblica. Tecnicamente, una transazione è protetta da una firma che dimostra che si è il proprietario della chiave pubblica a cui è stata inviata la transazione: Il rompicapo crittografico risolto è la prova di proprietà.

I fondi sono ulteriormente protetti dall'uso degli hash. Le chiavi pubbliche non vengono memorizzate nella blockchain fino a quando i fondi non vengono spesi, ma solo gli hash delle chiavi pubbliche. Ciò significa che anche se dovesse arrivare un computer quantistico, le transazioni Bitcoin rimarrebbero protette da questo secondo livello di crittografia.

**_Come vengono create le transazioni?_** Il cuore di ogni transazione Bitcoin è un linguaggio di scripting simile a FORTH che viene utilizzato per bloccare la transazione. Per spendere nuovamente il denaro, il destinatario fornisce allo script informazioni specifiche che dimostrano che è il destinatario previsto.

Tuttavia, questi script Bitcoin rappresentano il livello più basso di funzionalità Bitcoin. Gran parte del lavoro di Bitcoin viene svolto attraverso il _demone_ Bitcoin `bitcoind`, che viene controllato attraverso comandi RPC. Molti inviano questi comandi RPC attraverso il programma `bitcoin-cli`, che fornisce un'interfaccia ancora più semplice. I non programmatori non si preoccupano di queste minuzie, ma utilizzano portafogli già programmati con interfacce più semplici.

### Bitcoin - In breve

Si potrebbe pensare a Bitcoin come _una sequenza di transazioni atomiche_. Ogni transazione è autenticata da un mittente con la soluzione di un precedente puzzle crittografico memorizzato come script. La nuova transazione viene bloccata dal destinatario con un nuovo enigma crittografico, anch'esso memorizzato come script. Ogni transazione è registrata in un libro mastro globale immutabile.

## La crittografia a chiave pubblica

La crittografia a chiave pubblica è un sistema matematico per proteggere i dati e dimostrarne la proprietà attraverso una coppia asimmetrica di chiavi collegate: la chiave pubblica e la chiave privata.

È importante per Bitcoin (e per la maggior parte dei sistemi blockchain) perché è la base di gran parte della crittografia che protegge i fondi della criptovaluta. Una transazione Bitcoin viene tipicamente inviata a un indirizzo che è una chiave pubblica con hash. Il destinatario è quindi in grado di recuperare il denaro rivelando sia la chiave pubblica che la chiave privata.

**_Cos'è una chiave pubblica?_** Una chiave pubblica è la chiave che viene data ad altre persone. In un tipico sistema a chiave pubblica, un utente genera una chiave pubblica e una chiave privata, poi dà la chiave pubblica a tutti. I destinatari possono criptare le informazioni con la chiave pubblica, ma non possono decifrarle con la stessa chiave pubblica a causa dell'asimmetria della coppia di chiavi.

**_Cos'è una chiave privata?_** Una chiave privata è collegata a una chiave pubblica in una coppia di chiavi. In un tipico sistema a chiave pubblica, un utente tiene al sicuro la propria chiave privata e la usa per decifrare i messaggi che sono stati crittografati con la sua chiave pubblica prima di essergli inviati.

**_Cos'è una firma?_** Un messaggio (o più comunemente un hash di un messaggio) può essere firmato con una chiave privata, creando una firma. Chiunque possieda la chiave pubblica corrispondente può quindi convalidare la firma, verificando che il firmatario possiede la chiave privata associata alla chiave pubblica in questione. _SegWit_ è un formato specifico per la memorizzazione di una firma sulla rete Bitcoin che incontreremo più avanti.

**_Cos'è una funzione hash?_** Una funzione di hash è un algoritmo spesso utilizzato nella crittografia. È un modo per mappare una grande quantità arbitraria di dati in una piccola quantità fissa di dati. Le funzioni hash utilizzate in crittografia sono unidirezionali e resistenti alle collisioni, il che significa che un hash può essere collegato in modo affidabile ai dati originali, ma i dati originali non possono essere rigenerati dall'hash. Gli hash consentono quindi la trasmissione di piccole quantità di dati per rappresentare grandi quantità di dati, il che può essere importante per l'efficienza e i requisiti di archiviazione..

Bitcoin sfrutta la capacità di un hash di mascherare i dati originali, il che consente di nascondere l'effettiva chiave pubblica dell'utente, rendendo le transazioni resistenti al calcolo quantistico.

### Crittografia a chiave pubblica - In breve

Un modo di descrivere la crittografia a chiave pubblica è: _Un modo per proteggere i dati in modo che solo una persona autorizzata possa accedervi e che la persona autorizzata possa dimostrare di avere tale accesso_.

## Riguardo alle ECC

ECC è l'acronimo di crittografia a curve ellittiche. È un campo specifico della crittografia a chiave pubblica che dipende da calcoli matematici eseguiti con curve ellittiche definite su campi finiti. È più complessa e più difficile da spiegare rispetto alla crittografia a chiave pubblica classica (che utilizzava numeri primi), ma presenta alcuni vantaggi.

L'ECC non riceve molta attenzione in questo tutorial. Questo perché il tutorial è incentrato sull'integrazione con i server Bitcoin Core e Lightning, che si sono già occupati della crittografia. In effetti, l'intento di questo tutorial è quello di non farvi preoccupare affatto della crittografia, perché è qualcosa di cui volete _davvero_ che si occupino gli esperti.

**_Cos'è una curva ellittica?_** Una curva ellittica è una curva geometrica che assume la forma `y`<sup>`2`</sup> = `x`<sup>`3`</sup>` + ax + b`. Una curva ellittica specifica viene scelta selezionando valori specifici di `a` e `b`. La curva deve quindi essere esaminata attentamente per determinare se funziona bene per la crittografia. Ad esempio, la curva secp256k1 utilizzata da Bitcoin è definita come `a=0` e `b=7`.

Qualsiasi linea che interseca una curva ellittica lo farà in 1 o 3 punti... e questa è la base della crittografia a curva ellittica.

**_Cosa sono i campi finiti?_** Un campo finito è un insieme finito di numeri, in cui tutte le addizioni, sottrazioni, moltiplicazioni e divisioni sono definite in modo che risultino anche altri numeri nello stesso campo finito. Un modo semplice per creare un campo finito è attraverso l'uso di una funzione modulo.

**_Come viene definita una curva ellittica su un campo finito?_** Una curva ellittica definita su un campo finito ha tutti i punti sulla sua curva disegnati da uno specifico campo finito. Questo assume la forma: `y`<sup>`2`</sup> `% field-size = (x`<sup>`3`</sup>` + ax + b) % field-size` Il campo finito usato per secp256k1 è `2`<sup>`256`</sup>` - 2`<sup>`32`</sup>` - 2`<sup>`9`</sup>` - 2`<sup>`8`</sup>` - 2`<sup>`7`</sup>` - 2`<sup>`6`</sup>` - 2`<sup>`4`</sup>` - 1`.

**_Come vengono utilizzate le curve ellittiche in crittografia?_** Nella crittografia a curva ellittica, un utente seleziona un numero molto grande (256 bit) come chiave privata. Quindi aggiunge un punto base impostato sulla curva a se stessa tante volte. (Nel secp256k1, il punto base è `G = 04 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798 483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8`, che fa precedere le due parti della tupla da uno "04" per indicare che il punto dati è in formato non compresso. Se preferisci una definizione geometrica diretta, questo è il punto "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8") Il numero risultante è la chiave pubblica. È quindi possibile utilizzare varie formule matematiche per dimostrare la proprietà della chiave pubblica, data la chiave privata. Come ogni funzione crittografica, questa è una trappola: è facile passare dalla chiave privata alla chiave pubblica e in gran parte impossibile passare dalla chiave pubblica alla chiave privata.

Questa particolare metodologia spiega anche perché i campi finiti vengono utilizzati nelle curve ellittiche: garantisce che la chiave privata non diventi troppo grande. Si noti che il campo finito per secp256k1 è leggermente inferiore a 256 bit, il che significa che tutte le chiavi pubbliche saranno lunghe 256 bit, proprio come lo sono le chiavi private.

**_Quali sono i vantaggi della ECC?_** Il vantaggio principale dell’ECC è che consente la stessa sicurezza della classica crittografia a chiave pubblica con una chiave molto più piccola. Una chiave pubblica a curva ellittica da 256 bit corrisponde a una chiave pubblica tradizionale (RSA) da 3072 bit.

### ECC - In breve

Un modo di descrivere la ECC è: _un modo per abilitare la crittografia a chiave pubblica che utilizza chiavi molto piccole e matematiche molto oscure._

## Cosa sono le Blockchain

Blockchain è la generalizzazione della metodologia utilizzata da Bitcoin per creare un registro globale distribuito. Bitcoin è una blockchain come lo sono molte alt-coin, ognuna delle quali vive sulla propria rete e scrive sulla propria catena. Anche le sidechain come Liquid sono blockchain. Le blockchain non hanno nemmeno bisogno di avere nulla a che fare con le finanze. Ad esempio, ci sono state molte discussioni sull’utilizzo delle blockchain per proteggere le identità auto-sovrane.

Sebbene sia necessario comprendere le basi del funzionamento di una blockchain per capire come funzionano le transazioni in Bitcoin, non sarà necessario andare oltre. Poiché le blockchain sono diventate una vasta categoria di tecnologia, è probabile che questi concetti di base siano applicabili a molti altri progetti in questo settore tecnologico in crescita. I comandi di programmazione appresi in questo libro sono specifici per Bitcoin (e Lightning).

**_Perché si chiama catena?_** Ogni blocco nella blockchain memorizza un hash del blocco precedente. Questo collega il blocco attuale fino al "blocco genesi" originale attraverso una catena ininterrotta. È un modo per creare un ordine assoluto tra dati potenzialmente contrastanti. Ciò fornisce anche la sicurezza della blockchain, poiché ogni blocco è impilato sopra un vecchio blocco, rendendo più difficile ricreare il vecchio blocco a causa degli algoritmi di prova di lavoro (proof-of-work) utilizzati nella creazione dei blocchi. Una volta che diversi blocchi sono stati costruiti sopra un blocco nella catena, la catena stessa diventa essenzialmente irreversibile.

**_Cos'è un Fork?_** Occasionalmente vengono creati due blocchi nello stesso periodo. Questo crea temporaneamente un fork di un blocco, dove i blocchi attuali potrebbero essere quelli "reali". Di tanto in tanto, una biFORCazione potrebbe espandersi fino a diventare lunga due blocchi, tre blocchi o anche quattro blocchi, ma abbastanza rapidamente un lato della biforcazione viene determinato come quello reale e l'altro rimane "orfano". Questo fa parte del processo stocastico di creazione dei blocchi e dimostra perché diversi blocchi devono essere costruiti sopra un blocco prima che possa essere considerato veramente affidabile e non ripudiabile.

### Blockchain — In breve

Un modo di descrivere la blockchain è: _una serie collegata di blocchi di dati immutabili, che vanno indietro nel tempo. Un altro modo è: _una serie collegata di blocchi per ordinare in modo assoluto i dati che potrebbero essere in conflitto _.

## La Blockchain è adatta a me?

Se volete effettuare transazioni in bitcoin, ovviamente la blockchain di Bitcoin fa al caso vostro. Tuttavia, più in generale, la blockchain è diventata una parola popolare, anche se non è una bachetta magica per tutti i problemi tecnici. Detto questo, ci sono molte situazioni specifiche in cui la blockchain è una tecnologia superiore.

Le blockchain probabilmente saranno utili se:

   * Gli utenti non si fidano l'uno dell'altro.
     * Oppure: gli utenti sono oltre diversi confini.
   * Gli utenti non si fidano delle autorità centralizzate.
     * E: gli utenti vogliono controllare il proprio destino.
   * Gli utenti desiderano una tecnologia trasparente.
   * Gli utenti vogliono condividere qualcosa.
     * E: gli utenti desiderano che ciò che viene condiviso venga registrato in modo permanente.
   * Gli utenti desiderano transazioni definitive e veloci.
     * Ma: gli utenti non hanno bisogno della definitività immediata della transazione.

Probabilmente le blockchain _non_ saranno utili se:

   * Gli utenti sono fidati:
     * Ad esempio: le transazioni avvengono all'interno di un'azienda o di un'organizzazione.
     * Ad esempio: le transazioni sono supervisionate da un'autorità centrale.
   * È richiesta la segretezza:
     * Ad esempio: le informazioni dovrebbero essere segrete.
     * Ad esempio: le transazioni dovrebbero essere segrete.
     * Ad esempio: gli operatori dovrebbero essere segreti.
     * A meno che: una metodologia per la segretezza crittografica venga attentamente considerata, analizzata e testata.
   * Gli utenti necessitano di una transazione definitiva immediata.
     * es.: in meno di 10 minuti su una rete tipo Bitcoin, in meno di 2,5 minuti su una rete tipo Litecoin, in meno di 15 secondi su una rete tipo Ethereum

Tieni presente che potrebbero esserci ancora soluzioni per alcune di queste situazioni all'interno dell'ecosistema Bitcoin. Ad esempio, i canali di pagamento stanno affrontando rapidamente le questioni relative alla liquidità e alla definitività dei pagamenti.

## Riguardo a Lightning

Lightning è un protocollo di livello 2 (Layer-2) che interagisce con Bitcoin per consentire agli utenti di scambiare i propri bitcoin "off-chain". Presenta sia vantaggi che svantaggi rispetto all’utilizzo di Bitcoin "on-chain".

Lightning è anche l'obiettivo secondario di questo tutorial. Anche se si tratta principalmente di interagire direttamente con Bitcoin (e il `bitcoind`), presta una certa attenzione a Lightning perché è una tecnologia imminente che probabilmente diventerà un'alternativa popolare a Bitcoin nel prossimo futuro. Questo libro adotta lo stesso approccio a Lightning e Bitcoin: insegna come interagire direttamente dalla riga di comando con un daemon Lightning affidabile.

A differenza di Bitcoin, in realtà esistono diverse varianti di Lightning. Questo tutorial utilizza l'implementazione [c-lightning](https://github.com/ElementsProject/lightning) conforme allo standard come server Lightning affidabile.

**_Che cos'è un protocollo Layer-2?_** Un protocollo Bitcoin di layer-2 funziona su Bitcoin. In questo caso, Lightning funziona un livello sopra Bitcoin interagendo con Bitcoin attraverso contratti intelligenti.

**_Che cos'è un canale Lightning?_** Un canale Lightning è una connessione tra due utenti Lightning. Ciascuno degli utenti blocca un certo numero di bitcoin sulla blockchain utilizzando entrambi una firma multi-sig. I due utenti possono quindi scambiare bitcoin attraverso il loro canale Lightning senza mai scrivere sulla blockchain di Bitcoin. Solo quando vogliono chiudere il loro canale regolano i conti in bitcoin, in base al conteggio finale delle monete.

**_Che cos'è la Lightning Network?_** Mettendo insieme una serie di canali Lightning si crea "la rete Lightning". Questo permette a due utenti che non hanno creato un canale tra loro di scambiare bitcoin utilizzando Lightning: il protocollo forma una catena di Canali tra i due utenti, poi scambia le monete attraverso la catena utilizzando transazioni a tempo (time-locked).

**_Quali sono i vantaggi di Lightning?_** Lightning consente transazioni più veloci con commissioni inferiori. Ciò crea la reale possibilità di micropagamenti in bitcoin. Offre anche una migliore privacy, poiché è off-chain e solo il primo e l'ultimo stato della transazione vengono scritti nell registro immutabile di Bitcoin.

**_Quali sono gli svantaggi di Lightning?_** Lightning è ancora una tecnologia molto nuova e non è stata testata così approfonditamente come Bitcoin. Non è solo una questione di implementazione tecnologica, ma anche se il design stesso può essere modificato in modi inaspettati.

### Lightning - In breve

Un modo di descrivere Lightning è: _un modo per effettuare transazioni bitcoin utilizzando canali off-chain tra coppie di persone, in modo che solo lo stato iniziale e finale debba essere scritto sulla blockchain_.

## Riepilogo: Presentazione di Bitcoin

Bitcoin è un sistema peer-to-peer che consente il trasferimento di fondi attraverso transazioni bloccate con enigmi. Questi enigmi dipendono dalla crittografia a curva ellittica a chiave pubblica. Quando generalizzi le idee alla base di Bitcoin, ottieni blockchain, una tecnologia che attualmente sta crescendo e innovando. Quando espandi le idee dietro Bitcoin, ottieni protocolli di livello 2 come Lightning, che espandono il potenziale della valuta.

## Cosa viene dopo?

Prosegui nella "Preparatevi per Bitcoin" con il [Capitolo due: Configurare Bitcoin Core VPS](02_0_Configurare_Bitcoin-Core_VPS.md).
