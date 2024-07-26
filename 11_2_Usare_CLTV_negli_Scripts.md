# 11.2: Utilizzo di CLTV negli Script

`OP_CHECKLOCKTIMEVERIFY` (o CLTV) è il complemento naturale di `nLockTime`. Porta l'idea di bloccare le transazioni per un tempo assoluto o altezza di blocco nel regno degli opcode, permettendo il blocco di singoli UTXO.

> :warning: **AVVISO VERSIONE:** CLTV è diventato disponibile con Bitcoin Core 0.11.2, ma dovrebbe essere abbastanza ampiamente distribuito a questo punto.

## Ricorda nLockTime

Prima di approfondire CLTV, dovremmo prima ricordare come funziona `nLockTime`.

Come dettagliato in [§8.1: Invio di una Transazione con un Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md), il locktime è abilitato impostando due variabili, `nLockTime` e `nSequence`. Il `nSequence` deve essere impostato a meno di 0xffffffff (di solito: 0xffffffff-1), poi il `nLockTime` è interpretato come segue:

* Se il `nLockTime` è inferiore a 500 milioni, è interpretato come altezza di blocco.
* Se il `nLockTime` è 500 milioni o più, è interpretato come un timestamp UNIX.

Una transazione con `nLockTime` impostato non può essere spesa (o anche inserita nella blockchain) fino a quando non viene raggiunta l'altezza del blocco o il tempo. Nel frattempo, la transazione può essere annullata spendendo nuovamente uno qualsiasi degli UTXO che compongono la transazione.

## Comprendere l'Opcode CLTV

`OP_CHECKLOCKTIMEVERIFY` funziona nello stesso paradigma delle altezze di blocco assolute o dei tempi UNIX assoluti, ma viene eseguito come parte di uno Script Bitcoin. Legge un argomento, che può essere un'altezza di blocco o un tempo UNIX assoluto. Attraverso una metodologia alquanto complicata, confronta quell'argomento con il tempo corrente. Se è troppo presto, lo script fallisce; se la condizione temporale è stata soddisfatta, lo script continua.

Poiché CLTV è solo parte di uno script (e presumibilmente parte di una transazione P2SH), una transazione CLTV non è tenuta fuori dalla mempool come una transazione `nLockTime`; non appena viene verificata, entra nella blockchain e i fondi sono considerati spesi. Il trucco è che tutti gli output bloccati con il CLTV non sono disponibili per _spendere nuovamente_ fino a quando il CLTV non lo consente.

### Comprendere un Tempo Assoluto CLTV

Ecco come `OP_CHECKLOCKTIMEVERIFY` sarebbe usato per verificare contro il 24 maggio 2017:

```
1495652013 OP_CHECKLOCKTIMEVERIFY
```
Ma di solito lo rappresenteremo in un'astrazione così:
```
<May24> OP_CHECKLOCKTIMEVERIFY
```
O cosi:
```
<AbsoluteTime> OP_CHECKLOCKTIMEVERIFY
```


### Comprendere un'Altezza di Blocco Assoluta CLTV

Ecco come `OP_CHECKLOCKTIMEVERIFY` verificherebbe contro un'altezza di blocco raggiunta il 24 maggio 2017:

```
467951 OP_CHECKLOCKTIMEVERIFY
```

Ma di solito lo astrarremo così:

```
<AbsoluteBlockHeight> OP_CHECKLOCKTIMEVERIFY
```


### Comprendere Come Funziona Davvero CLTV

La spiegazione sopra è sufficiente per usare e comprendere CLTV. Tuttavia, [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) espone tutti i dettagli.

Uno script di blocco permetterà di spendere nuovamente un UTXO bloccato con un CLTV solo se `OP_CHECKLOCKTIMEVERIFY` verifica tutte le seguenti condizioni:

* Il campo `nSequence` deve essere impostato a meno di 0xffffffff, di solito 0xffffffff-1 per evitare conflitti con i timelock relativi.
* CLTV deve estrarre un operando dallo stack e deve essere 0 o maggiore.
* Sia l'operando dello stack che il valore di `nLockTime` devono essere sopra o sotto i 500 milioni, per rappresentare lo stesso tipo di timelock assoluto.
* Il valore di `nLockTime` deve essere maggiore o uguale all'operando dello stack.

Quindi la prima cosa da notare qui è che `nLockTime` è ancora usato con CLTV. Per essere precisi, è richiesto nella transazione che cerca di _spendere nuovamente_ un UTXO bloccato con CLTV. Ciò significa che non fa parte dei requisiti dello script. È solo il timer usato per rilasciare i fondi, _come definito nello script_.

Questo è gestito attraverso una comprensione intelligente di come funziona `nLockTime`: un valore per `nLockTime` deve sempre essere scelto che sia inferiore o uguale al tempo presente (o altezza del blocco), in modo che la transazione di spesa possa essere inserita nella blockchain. Tuttavia, a causa dei requisiti di CLTV, deve anche essere scelto un valore maggiore o uguale all'operando di CLTV. L'unione di questi due insiemi è `NULL` fino a quando il tempo presente non corrisponde all'operando di CLTV. Successivamente, può essere scelto qualsiasi valore tra l'operando di CLTV e il tempo presente. Di solito, lo imposteresti semplicemente al tempo presente (o al blocco).

## Scrivere uno Script CLTV

`OP_CHECKLOCKTIMEVERIFY` include un `OP_VERIFY`, il che significa che interromperà immediatamente lo script se la sua verifica non riesce. Ha un'altra peculiarità: a differenza della maggior parte dei comandi "verify", lascia ciò che sta testando sullo stack (nel caso in cui tu voglia fare altri controlli contro il tempo). Ciò significa che un `OP_CHECKLOCKTIMEVERIFY` è di solito seguito da un `OP_DROP` per pulire lo stack.

Il seguente semplice script di blocco potrebbe essere usato per trasformare un output P2PKH in una transazione P2PKH bloccata nel tempo:

```
<NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```


### Codificare uno Script CLTV

Ovviamente, come per qualsiasi script Bitcoin complesso, questo script CLTV sarebbe effettivamente codificato in uno script P2SH, come spiegato in [§10.1: Comprendere le Basi di P2SH](10_1_Understanding_the_Foundation_of_P2SH.md) e [§10.2: Costruire la Struttura di P2SH](10_2_Building_the_Structure_of_P2SH.md).

Supponendo che `<NextYear>` fosse l'intero "1546288031" (hex little-endian: 0x9f7b2a5c) e `<pubKeyHash>` fosse "371c20fb2e9899338ce5e99908e64fd30b789313", questo `redeemScript` sarebbe costruito come:

```
OP_PUSHDATA (4 bytes) 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 OP_PUSHDATA (20 bytes) 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
```
Che si traduce in hex come:
```
04 9f7b2a5c b1 75 76 a9 14 371c20fb2e9899338ce5e99908e64fd30b789313 88 ac
```
Oppure, se preferisci:
```
$ btcc 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
049f7b2a5cb17576a914371c20fb2e9899338ce5e99908e64fd30b78931388ac
```
L'RPC `decodescript` può verificare che abbiamo fatto tutto correttamente:
```
{
  "asm": "1546288031 OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG",
  "type": "nonstandard",
  "p2sh": "2MxANZMPo1b2jGaeKTv9rwcBEiXcXYCc3x9",
  "segwit": {
    "asm": "0 07e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "hex": "002007e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qqlj4hu02ah6ra3f274ah0ttnxpgxcgy6wrghlghps5esf25wfedse4yw4w"
    ],
    "p2sh-segwit": "2N4HTwMjVdm38bdaQ5h3X3VktLY74D2qBoK"
  }
}
```


Non mostreremo continuamente come tutti gli Script Bitcoin vengono codificati in transazioni P2SH, ma offriremo invece queste abbreviazioni: quando descriviamo uno script, sarà un `redeemScript`, che sarebbe normalmente serializzato e hashato in uno script di blocco e serializzato nello script di sblocco; quando mostriamo una procedura di sblocco, sarà il secondo turno di convalida, dopo la conferma dell'hash dello script di blocco.

## Spendere un UTXO CLTV

Per spendere un UTXO bloccato con un CLTV, è necessario impostare `nLockTime` sulla nuova transazione. Di solito, vuoi solo impostarlo al tempo presente o al blocco presente, come appropriato. Finché il tempo CLTV o l'altezza del blocco è nel passato, e finché fornisci tutti gli altri dati richiesti dallo script di sblocco, sarai in grado di elaborare l'UTXO.

Nel caso dell'esempio sopra, il seguente script di sblocco sarebbe sufficiente, a condizione che `nLockTime` fosse impostato a un momento successivo alla data `<NextYear>`, e a condizione che fosse effettivamente almeno `<NextYear>`:

```
<signature> <pubKey>
```


### Eseguire uno Script CLTV

Per eseguire lo Script, dovresti prima concatenare gli script di sblocco e blocco:

```
Script: <signature> <pubKey> <NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]
```
I tre costanti verrebbero spinti nello stack:
```
Script: OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <NextYear> ]
```
Poi, `OP_CHECKLOCKTIMEVERIFY` viene eseguito. Trova qualcosa nello stack e verifica che `nSequence` non sia 0xffffffff. Infine, confronta `<NextYear>` con `nLockTime`. Se sono entrambi dello stesso tipo di rappresentazione e se `nLockTime ≥ <NextYear>`, allora viene elaborato con successo (altrimenti, termina lo script):
```
Script: OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <NextYear> OP_CHECKLOCKTIMEVERIFY
Stack: [ <signature> <pubKey> <NextYear> ]
```
Poi, `OP_DROP` elimina quel `<NextYear>` rimasto:
```
Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <NextYear> OP_DROP
Stack: [ <signature> <pubKey> ]
```
FInfine, il resto dello script viene eseguito, che è un controllo normale di una firma e una chiave pubblica.

## Riepilogo: Utilizzo di CLTV negli Script

`OP_CHECKLOCKTIMEVERIFY` è un semplice opcode che guarda un singolo argomento, lo interpreta come un'altezza di blocco o un timestamp UNIX, e permette solo che il suo UTXO venga sbloccato se quell'altezza di blocco o timestamp UNIX è nel passato. Impostare `nLockTime` sulla transazione di spesa è ciò che permette a Bitcoin di fare questo calcolo.

> :fire: ***Qual è il Potere di CLTV?*** Hai già visto che i semplici locktime erano una delle basi dei Contratti Intelligenti. CLTV fa il passo successivo. Ora puoi sia garantire che un UTXO non possa essere speso prima di un certo momento _sia_ garantire che non venga speso diversamente. Nella sua forma più semplice, questo potrebbe essere usato per creare un trust che qualcuno potrebbe accedere solo quando raggiunge i 18 anni o un fondo pensione che potrebbe essere accessibile solo quando compie 50 anni. Tuttavia, il suo vero potere si manifesta quando combinato con i condizionali, dove il CLTV si attiva solo in determinate situazioni.

## Cosa Succede Dopo?

Continua a "Potenziare Timelock" col [Capitolo 11.3: Usare CSV negli Scripts](11_3_Usare_CSV_negli_Scripts.md).
