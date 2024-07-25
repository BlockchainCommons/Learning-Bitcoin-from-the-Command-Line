# 10.4: Scripting di un Multisig

Prima di concludere questa introduzione agli script P2SH, vale la pena esaminare un esempio più realistico. Fin dal [Capitolo 6.1](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md), abbiamo detto casualmente che l'interfaccia `bitcoin-cli` avvolge la sua transazione multisig in una transazione P2SH. In effetti, questa è la metodologia standard per creare multisig sulla Blockchain. Ecco come funziona, in dettaglio.

## Comprendere il Codice Multisig

Le transazioni multisig sono create in Bitcoin usando il codice `OP_CHECKMULTISIG`. `OP_CHECKMULTISIG` si aspetta una lunga stringa di argomenti che assomiglia a questa: `0 ... sigs ... <m> ... addresses ... <n> OP_CHECKMULTISIG`. Quando `OP_CHECKMULTISIG` viene eseguito, fa quanto segue:

1. Estrae il primo valore dallo stack (`<n>`).
2. Estrae "n" valori dallo stack come indirizzi Bitcoin (chiavi pubbliche hashate).
3. Estrae il valore successivo dallo stack (`<m>`).
4. Estrae "m" valori dallo stack come potenziali firme.
5. Estrae uno `0` dallo stack a causa di un errore nella codifica originale.
6. Confronta le firme con gli indirizzi Bitcoin.
7. Spinge un `True` o `False` a seconda del risultato.

Gli operandi di `OP_MULTISIG` sono tipicamente divisi, con lo `0` e le firme provenienti dallo script di sblocco e "m", "n" e gli indirizzi dettagliati dallo script di blocco.

Il requisito di quello `0` come primo operando per `OP_CHECKMULTISIG` è una regola di consenso. Poiché la versione originale di `OP_CHECKMULTISIG` ha accidentalmente estratto un elemento in più dallo stack, Bitcoin deve seguire per sempre quello standard, altrimenti gli script di riscatto complessi di quel periodo rischiano di essere rotti, rendendo i vecchi fondi irrecuperabili.

> :book: ***Cos'è una regola di consenso?*** Queste sono le regole che i nodi Bitcoin seguono per lavorare insieme. In gran parte sono definite dal codice di Bitcoin Core. Queste regole includono molti mandati ovvi, come il limite a quanti Bitcoin sono creati per ogni blocco e le regole su come le transazioni possono essere rispese. Tuttavia, includono anche correzioni per bug che sono apparsi nel corso degli anni, perché una volta che un bug è stato introdotto nel codice di Bitcoin, deve essere continuamente supportato, altrimenti i vecchi Bitcoin diventano non spendibili.

## Creare un Multisig Raw

Come discusso nel [Capitolo 10.1: Comprendere la Base di P2SH](10_1_Comprendere_la_Base_di_P2SH.md), i multisig sono uno dei tipi di transazione standard di Bitcoin. Una transazione può essere creata con uno script di blocco che usa il comando raw `OP_CHECKMULTISIG`, e sarà accettata in un blocco. Questa è la metodologia classica per usare i multisig in Bitcoin.

Come esempio, rivisiteremo il multisig creato nel [Capitolo 6.1](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md) un'ultima volta e costruiremo un nuovo script di blocco per esso usando questa metodologia. Come forse ricorderai, quello era un multisig 2-di-2 costruito da `$address1` e `$address2`.

Poiché lo script di blocco `OP_CHECKMULTISIG` richiede "m" (`2`), gli indirizzi e "n" (`2`), potresti scrivere il seguente `scriptPubKey`:
```
2 $address1 $address2 2 OP_CHECKMULTISIG
```
Se questo ti sembra familiare, è perché è il multisig che hai deserializzato nel [Capitolo 10.2: Construire la Struttura di P2SH](10_2_Construire_la_Struttura_di_P2SH.md).
```
2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG
```


> **AVVISO:** Per le firme `OP_CHECKMULTISIG` classiche, "n" deve essere ≤ 3 affinché la transazione sia standard.

## Sbloccare un Multisig Raw

Lo `scriptSig` per un indirizzo multisig standard deve quindi fornire gli operandi mancanti per `OP_CHECKMULTISIG`: uno `0` seguito da "m" firme. Ad esempio:
:
```
0 $signature1 $signature2
```


### Eseguire uno Script Multisig Raw

Per spendere un UTXO multisig, esegui lo `scriptSig` e lo `scriptPubKey` come segue:

```
Script: 0 $signature1 $signature2 2 $address1 $address2 2 OP_CHECKMULTISIG
Stack: [ ]
```
Prima, metti tutte le costanti nello stack:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 2 ]
```
Poi, l'`OP_CHECKMULTISIG` inizia a funzionare. Prima, viene estratto il "2":
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 ]
```
Poi, il "2" dice a `OP_CHECKMULTISIG` di estrarre due indirizzi:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 ]
```
Poi, viene estratto il prossimo "2":
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 ]
```
Poi, il "2" dice a `OP_CHECKMULTISIG` di estrarre due firme:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 ]
```
Poi, un altro elemento viene erroneamente estratto:
```
Running: OP_CHECKMULTISIG
Stack: [ ]
```
Poi, `OP_CHECKMULTISIG` completa la sua operazione confrontando le "m" firme con gli indirizzi "n":
```
Script:
Stack: [ True ]
```
## Comprendere i Limiti degli Script Multisig Raw

Sfortunatamente, la tecnica di incorporare un multisig raw in una transazione ha alcuni svantaggi notevoli:

1. Poiché non esiste un formato di indirizzo standard per i multisig, ogni mittente deve: inserire uno script multisig lungo e ingombrante; avere un software che lo consenta; e essere affidabile per non sbagliare.
2. Poiché i multisig possono essere molto più lunghi degli script di blocco tipici, la blockchain incorre in maggiori costi. Questo richiede commissioni di transazione più elevate da parte del mittente e crea più fastidi per ogni nodo.

Questi erano generalmente problemi con qualsiasi tipo di script Bitcoin complesso, ma sono diventati rapidamente problemi molto reali quando applicati ai multisig, che erano alcuni dei primi script complessi ad essere ampiamente utilizzati sulla rete Bitcoin. Le transazioni P2SH sono state create per risolvere questi problemi, a partire dal 2012.

> :book: ***Cos'è un multisig P2SH?*** I multisig P2SH sono la prima implementazione delle transazioni P2SH. Semplicemente confezionano una transazione multisig standard in una transazione P2SH standard. Questo consente la standardizzazione degli indirizzi; riduce lo spazio di archiviazione dei dati; e aumenta i conteggi di "m" e "n".

## Creare un Multisig P2SH

I multisig P2SH sono la metodologia moderna per creare multisig sulla Blockchain. Possono essere creati molto semplicemente, usando lo stesso processo visto nelle sezioni precedenti.

### Creare il Blocco per il Multisig P2SH

Per creare un multisig P2SH, segui i passaggi standard per creare uno script di blocco P2SH:

1. Serializzare `2 $address1 $address2 2 OP_CHECKMULTISIG`.
   1. `<serializedMultiSig>` = "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
2. Salva `<serializedMultiSig>` per riferimento futuro come redeemScript.
   1. `<redeemScript>` = "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
3. Esegui l'hash SHA-256 e RIPEMD-160 dello script serializzato.
   1. `<hashedMultiSig>` = "a5d106eb8ee51b23cf60d8bd98bc285695f233f3"
4. Produci uno script di blocco Multisig P2SH che includa lo script hashato (`OP_HASH160 <hashedMultisig> OP_EQUAL`).
   1. `scriptPubKey` = "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387"
   
Puoi quindi creare una transazione usando quel `scriptPubKey`.

## Sbloccare il Multisig P2SH

Per sbloccare questa transazione multisig è necessario che il destinatario produca uno scriptSig che includa le due firme e il `redeemScript`.

### Eseguire il Primo Ciclo di Validazione P2SH

Per sbloccare il multisig P2SH, conferma prima lo script:

1. Produci uno script di sblocco di `0 $signature1 $signature2 <serializedMultiSig>`.
2. Concatenalo con lo script di blocco di `OP_HASH160 <hashedMultisig> OP_EQUAL`.
3. Valida `0 $signature1 $signature2 <serializedMultiSig> OP_HASH160 <hashedMultisig> OP_EQUAL`.
4. Ha successo se il `<serializedMultisig>` corrisponde al `<hashedMultisig>`.

### Eseguire il Secondo Ciclo di Validazione P2SH

Poi, esegui lo script multisig:

1. Deserializza `<serializedMultiSig>` in `2 $address1 $address2 2 OP_CHECKMULTISIG`.
2. Concatenalo con gli operandi precedenti nello script di sblocco, `0 $signature1 $signature2`.
3. Valida `0 $signature1 $signature2 2 $address1 $address2 2 OP_CHECKMULTISIG`.
4. Ha successo se gli operandi soddisfano il `redeemScript` deserializzato.

Ora sai come la transazione multisig nel [Capitolo 6.1](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md) è stata effettivamente creata, come è stata validata per la spesa e perché quel `redeemScript` era così importante.

## Riepilogo: Creare Script Multisig

I multisig sono un tipo di transazione standard, ma sono un po' ingombranti da usare, quindi sono regolarmente incorporati in transazioni P2SH, come nel caso del [Capitolo 6.1](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md) quando abbiamo creato i nostri primi multisig. Il risultato è più pulito, più piccolo e più standardizzato — ma più importante, è un ottimo esempio reale di come funzionano veramente gli script P2SH.

## Cosa c'è Dopo?

Continua "Incorporare Bitcoin Scripts" con [Capitolo 10.5: Programmare uno Script Segwit](10_5_Programmare_uno_Script_Segwit.md)
