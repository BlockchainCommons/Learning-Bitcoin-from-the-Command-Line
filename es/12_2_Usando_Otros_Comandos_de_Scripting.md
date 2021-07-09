# 12.2: Uso de otros comandos de secuencia de comandos

Es posible que ya tenga a mano la mayoría de los códigos de operación de Bitcoin Script que utilizará en la mayoría de los scripts. Sin embargo, Bitcoin Script ofrece muchas más opciones, que podrían ser exactamente lo que necesita para crear el instrumento financiero de sus sueños.

Debería consultar la [página de Bitcoin Script](https://en.bitcoin.it/wiki/Script) para obtener una visión más detallada de todos estos y muchos otros comandos. Esta sección solo destaca los códigos de operación más notables.

## Comprender los códigos de operación aritméticos

Los códigos de operación aritméticos manipulan o prueban números.

Manipular un número:

* OP_1ADD (0x8b) - Incremento en uno
* OP_1SUB (0x8c) - disminución en uno
* OP_NEGATE (0x8f) - Da la vuelta al signo del número
* OP_ABS (0x90) - Hacer que el número sea positivo
* OP_NOT (0x91) - Voltea 1 y 0, de lo contrario 0

Ver también: `OP_0NOTEQUAL` (0x92)

Manipula dos números matemáticamente:

* OP_ADD (0x93) - Suma dos números
* OP_SUB (0x94) - Resta dos números
* OP_MIN (0xa3) - Devuelve el menor de dos números
* OP_MAX (0xa4) - Devuelve el mayor de dos números

Manipula dos números de forma lógica:

* OP_BOOLAND (0x9a) - 1 si ambos números no son 0, de lo contrario 0
* OP_BOOLOR (0x9b) - 1 si alguno de los números no es 0, de lo contrario 0

Prueba dos números:

* OP_NUMEQUAL (0x9c) - 1 si ambos números son iguales, de lo contrario 0
* OP_LESSTHAN (0x9f) - 1 si el primer número es menor que el segundo, de lo contrario 0
* OP_GREATERTHAN (0xa0) - 1 si el primer número es mayor que el segundo, de lo contrario 0
* OP_LESSTHANOREQUAL (0xa1) - 1 si el primer número es menor o igual que el segundo, de lo contrario 0
* OP_GREATERTHANOREQUAL (0xa2) - 1 si el primer número es mayor o igual que el segundo, de lo contrario 0

Ver también: `OP_NUMEQUALVERIFY` (0x9d),`OP_NUMNOTEQUAL` (0x9e)

Prueba tres números:

* OP_WITHIN (0xa5) - 1 si un número está en el rango de otros dos números

## Comprender los códigos de operación de pila

Hay una cantidad sorprendente de códigos de operación de pila, pero aparte de `OP_DROP`,`OP_DUP` y, a veces, `OP_SWAP` generalmente no son necesarios si se tiene cuidado con el orden de las pilas. No obstante, aquí hay algunos de los más interesantes:

* OP_DEPTH (0x74) - Empuja el tamaño de la pila
* OP_DROP (0x75) - Aparece el elemento de la pila superior
* OP_DUP (0x76) - duplica el elemento de la pila superior
* OP_PICK (0x79) - Duplica el enésimo elemento de la pila como la parte superior de la pila
* OP_ROLL (0x7a) - Mueve el enésimo elemento de la pila a la parte superior de la pila
* OP_SWAP (0x7c) - intercambia los dos elementos superiores de la pila

Ver también: `OP_TOALTSTACK` (0x6b),`OP_FROMALTSTACK` (0x6c), `OP_2DROP` (0x6d),`OP_2DUP` (0x6e), `OP_3DUP` (0x6f),`OP_2OVER` (0x70), `OP_271 ), `OP_2SWAP` (0x72),`OP_IFDUP` (0x73), `OP_NIP` (0x77),`OP_OVER` (0x78), `OP_ROT` (0x7b) y`OP_TUCK` (0x7d).

## Comprender los códigos de operación criptográficos

Por último, una variedad de códigos de operación admiten hash y verificación de firmas:

Hash:(lo traduce como picadillo)

* OP_RIPEMD160 (0xa6) - RIPEMD-160
* OP_SHA1 (0xa7) - SHA-1
* OP_SHA256 (0xa8) - SHA-256
* OP_HASH160 (0xa9) - SHA-256 + RIPEMD-160
* OP_HASH256 (0xaa) - SHA-256 + SHA-256

Check Signatures:

* OP_CHECKSIG (0xac) - Verifique una firma
* OP_CHECKMULTISIG (0xae) - Verifica un multisig de m de n

Consulte también: `OP_CODESEPARATOR` (0xab),`OP_CHECKSIGVERIFY` (0xad) y `OP_CHECKMULTISIGVERIFY` (0xaf).

## Resumen: uso de otros comandos de secuencia de comandos

Bitcoin Script incluye una amplia variedad de códigos de operación aritméticos, apilados y criptográficos. La mayoría de estos códigos de operación adicionales probablemente no sean tan comunes como los que se discutieron en las secciones anteriores, pero no obstante, están disponibles si son justo lo que necesita para escribir su script.

## ¿Que sigue?

Avance a través de "Scripts de Bitcoin" con el [Capítulo trece: Diseño de scripts de Bitcoin reales](13_0_Disenando_Bitcoin_Scripts_Reales.md).
