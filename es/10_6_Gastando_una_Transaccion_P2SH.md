# 10.6: Gastando una Transacción P2SH

Antes de cerrar esta descripcion general de las transacciones P2SH, veremos cómo gastarlas. Esta sección es principalmente una descripción general, que hace referencia a una sección anterior en la que _ya_ gastamos una transacción P2SH.

## Utilice el Guión de Canjear

Como vimos en [§6.2: Gastando una Transacción con una Multifirma](06_2_Gastando_una_Transaccion_con_una_Direccion_Multifirma.md), gastar una transacción P2SH se trata de tener esa versión serializada del script de bloqueo, el llamado _redeemScript_. Entonces, el primer paso para poder gastar una transacción P2SH es asegurarse de guardar el _redeemScript_ antes de dar la dirección P2SH a todos. 

### Recoge Sus Variables

Debido a que las direcciones P2SH distintas de las direcciones segwit anidadas y mulifirmas especiales no están integradas en `bitcoin-cli` no habrá atajos para el gasto P2SH como viste en [§6.3: Enviando una Multifirma Automatizada](06_3_Enviando_una_Multifirma_Automatizada.md). ¡Necesitará recopilar todas las variables más complejas por su cuenta!

Esto significa que debe recopilar:

   * El `hex` de la `scriptPubKey` para la transacción que está gastando
   * El `redeemScript` serializado
   * Cualquier clave privada, ya que estará firmando a mano
   * Todos los `txids`, `vouts`, y `direcciones` regulares que necesitaría

## Crear la Transacción

Como vimos en §6.2, la creación de una transacción es bastante estándar:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```
Sin embargo, la firma requiere ingresar información adicional para (1) `scriptPubKey`; (2) el `redeemScript`; y (3) cualquier clave privada requerida.

Este es el ejemplo de cómo hacerlo para esa multifirma incrustada en P2SH en §6.2:
```
$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
```
Con cualquier otro tipo de P2SH, incluirá un `redeemscript` diferente, pero por lo demás, la práctica es exactamente la misma. La única diferencia es que después de dos capítulos de trabajo en Scripts, ahora comprende qué es el `scriptPubKey` y qué es el `redeemScript`, así que con suerte lo que eran elementos misteriosos hace cuatro capítulos ahora es viejo sombrero.

## Resumen: Gastar una Transacción P2SH

Ya gastó un P2SH en el Capítulo 6, cuando reenvió una transacción multifirma de la manera difícil, lo que requirió alinear la información de `scriptPubKey` y `redeemScript`. Ahora sabe que el `scriptPubKey` es un script de bloqueo P2SH estandarizado, mientras que el `redeemScript` coincide con un hash en ese script de bloqueo y que necesita poder ejecutarlo con las variables adecuadas para recibir un resultado `True`. Pero aparte de saber más, no hay nada nuevo en gastar una transacciónn P2SH, porque ya lo hizo!

## Que Sigue?

Avanzar a través de "Bitcoin Scripting" con el [Capítulo Once: Potenciando Bloqueos de Tiempo con Bitcoin Scripts](11_0_Potenciando_Bloqueos_de_Tiempo_con_Bitcoin_Scripts.md).
