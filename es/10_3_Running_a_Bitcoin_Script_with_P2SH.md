# 10.3: Ejecución de un Bitcoin Script con P2SH

Ahora que concoce la teoría y la práctica detrás de las direcciones P2SH, está listo para convertir un script de Bitcoin no estándar en una transacción real. Reutilizaremos la secuencia de comandos de bloqueo simple de [§9.2: Ejecución de un Bitcoin Script](09_2_Running_a_Bitcoin_Script.md), `OP_ADD 99 OP_EQUAL`.

## Crear una Transacción P2SH

Para bloquear una transacción con este script, haga lo siguiente:

1. Serializar `OP_ADD 99 OP_EQUAL`:
   1. OP_ADD = 0x93 — una traducción simple de un opcode
   2. 99 = 0x01, 0x63 — este opcode empuja un byte a la pila, 99 (hex: 0x63)
      * No se preocupe por la conversión endian porque es solo un byte
   3. OP_EQUAL = 0x87 — una tradducion simple de un opcode
   4. `<serialized99Equal>` = "93016387" 
   
```
$ btcc OP_ADD 99 OP_EQUAL
93016387
```

2. Guarde `<serialized99Equal>` para referencia futura como `redeemScript`.
   1. `<redeemScript>` = "93016387"
3. SHA-256 y RIPEMD-160 hash el script serializado.
   1. `<hashed99Equal>` = "3f58b4f7b14847a9083694b9b3b52a4cea2569ed"
4. Produzca un script de bloqueo P2SH que incluya el `<hashed99Equal>`.
   1. `scriptPubKey` = "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87"

Luego puede crear una transacción usando esta `scriptPubKey`, probablemente a través de una API.

## Desbloquear la Transacción P2SH

Para desbloquear esta transacción se requiere que el destinatario produzca un `scriptSig` que anteponga dos constantes que sumen noventa y nueve al script serializado: `1 98 <serialized99Equal>`.

### Ejecute la Primera Ronda de Validación

El proceso de desbloqueo de la transacción P2SH comienza con una primera ronda de validación, que verifica que el script de canje coincida con el valor hash en el script de bloqueo. 

Concatenar `scriptSig` y `scriptPubKey` y ejecutarlos, como de costumbre:
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
La secuencia de comandos termina con un `True` en la parte superior de la pila, por lo que tiene éxito ... a pesar de que hay otros cruft debajo de él.

Sin embargo, debido a que se trataba de un script P2SH, la ejecución no se realiza. 

### Ejecute la Segunda Ronda de Validación

Para la segunda ronda de validación, verifique que los valores en el script de desbloqueo satisfagan el `redeemScript`: deserialice el `redeemScript` ("93016387" = "OP_ADD 99 OP_EQUAL"), luego ejecútelo usando los elementos del `scriptSig` antes del script serializado:

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
Con esa segunda validación _también_ verdadera, el UTXO ahora se puede gastar!

## Resumen: Creación de un Bitcoin Script con P2SH

Una vez que conozca la técnica de construcción de P2SH, cualquier script se puede incrustar en una transacción de Bitcoin; y una vez que comprenda la técnica de validación de P2SH, es fácil ejecutar los scripts en dos rondas.

## Que Sigue?

Continúe "Incrustando Bitcoin Scripts" con [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md).
