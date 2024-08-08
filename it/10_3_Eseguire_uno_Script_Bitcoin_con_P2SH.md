# 10.3: Eseguire uno Script Bitcoin con P2SH

Ora che conosci la teoria e la pratica dietro gli indirizzi P2SH, sei pronto a trasformare uno Script Bitcoin non standard in una transazione effettiva. Riutilizzeremo lo script di blocco semplice da [Capitolo 9.2: Eseguire uno Script di Bitcoin](09_2_Eseguire_uno_Script_di_Bitcoin.md), `OP_ADD 99 OP_EQUAL`.

## Creare una Transazione P2SH

Per bloccare una transazione con questo Script, fai quanto segue:

1. Serializzare `OP_ADD 99 OP_EQUAL`:
   1. OP_ADD = 0x93 — una semplice traduzione dell'opcode
   2. 99 = 0x01, 0x63 — questo opcode spinge un byte nello stack, 99 (hex: 0x63)
      * Nessuna preoccupazione per la conversione endian poiché è solo un byte
   3. OP_EQUAL = 0x87 — una semplice traduzione dell'opcode
   4. `<serialized99Equal>` = "93016387" 
   

   
```
$ btcc OP_ADD 99 OP_EQUAL
93016387
```


2. Salva `<serialized99Equal>` per riferimento futuro come `redeemScript`.
   1. `<redeemScript>` = "93016387"
3. Esegui l'hash SHA-256 e RIPEMD-160 dello script serializzato.
   1. `<hashed99Equal>` = "3f58b4f7b14847a9083694b9b3b52a4cea2569ed"
4. Produci uno script di blocco P2SH che includa il `<hashed99Equal>`.
   1. `scriptPubKey` = "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87"

Puoi quindi creare una transazione usando questo `scriptPubKey`, probabilmente tramite un'API.

## Sbloccare la Transazione P2SH

Per sbloccare questa transazione è necessario che il destinatario produca un `scriptSig` che premetta due costanti per un totale di novantanove allo script serializzato: `1 98 <serialized99Equal>`.

### Eseguire il Primo Ciclo di Validazione

Il processo di sblocco della transazione P2SH inizia con un primo ciclo di validazione, che verifica che il redeem script corrisponda al valore hashato nello script di blocco.

Concatena `scriptSig` e `scriptPubKey` ed eseguili, come di consueto:

```
Script: 1 98 <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: []

Script: 98 <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 ]

Script: <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 98 ]

Script: OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 98 <serialized99Equal> ]

Script: <hashed99Equal> OP_EQUAL
Running: <serialized99Equal> OP_HASH160
Stack: [ 1 98 <hashed99Equal> ]

Script: OP_EQUAL
Stack: [ 1 98 <hashed99Equal> <hashed99Equal> ]

Script: 
Running: <hashed99Equal> <hashed99Equal> OP_EQUAL
Stack: [ 1 98 True ]
```
Lo Script termina con un `True` in cima allo stack, quindi ha successo... anche se ci sono altre informazioni inutili sotto.

Tuttavia, poiché questo era uno script P2SH, l'esecuzione non è finita. 

### Eseguire il Secondo Ciclo di Validazione

Per il secondo ciclo di validazione, verifica che i valori nello script di sblocco soddisfino il `redeemScript`: deserializza il `redeemScript` ("93016387" = "OP_ADD 99 OP_EQUAL"), quindi eseguilo usando gli elementi nel `scriptSig` prima dello script serializzato:

```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: [ ]

Script: 98 OP_ADD 99 OP_EQUAL
Stack: [ 1 ]

Script: OP_ADD 99 OP_EQUAL
Stack: [ 1 98 ]

Script: 99 OP_EQUAL
Running: 1 98 OP_ADD
Stack: [ 99 ]

Script: OP_EQUAL
Stack: [ 99 99 ]

Script: 
Running: 99 99 OP_EQUAL
Stack: [ True ]
```
Con quella seconda validazione _anche_ vera, il UTXO può ora essere speso!

## Riepilogo: Costruire uno Script Bitcoin con P2SH

Una volta che conosci la tecnica per costruire P2SH, qualsiasi Script può essere incorporato in una transazione Bitcoin; e una volta che comprendi la tecnica di validazione dei P2SH, è facile eseguire gli script in due cicli.

## Cosa c'è Dopo?

Continua a "Incorporare Bitcoin Scripts" col [Capitolo 10.4 Programmare una Multifirma](10_4_Programmare_una_Multifirma.md).
