# 10.4: Guioniendo una Multifirma

Antes de cerrar esta introducción a las secuencias de comandos P2SH, vale la pena examinar un ejemplo más realista. Desde [§6.1](06_1_Sending_a_Transaction_to_a_Multisig.md), hemos estado diciendo casualmente que la `bitcoin-cli` envuelve su transacción multifirma en una transacción P2SH. De hecho, esta es la metodologiá estándar para crear multifirmas en Blockchain. Así es como funciona eso en profundidad.

## Entender el Código Multifirma

Las transacciones multifirma se crean en Bitcoin utlizando el código `OP_CHECKMULTISIG`. `OP_CHECKMULTISIG` espera una larga cadena de argumentos con este aspecto: `0 ... sigs ... <m> ... addresses ... <n> OP_CHECKMULTISIG`. Cuando se ejecuta `OP_CHECKMULTISIG` hace lo siguiente:

1. Saque el primer valor de la pila (`<n>`).
2. Saque los valores "n" de la pila como direcciones de Bitcoin (claves públicas con hash).
3. Saque el siguiente valor de la pila (`<m>`).
4. Saque los valores "m" de la pila como posible firmas.
5. Saque un `0` de la pila debido a un error en la codificación.
6. Compare las firmas con las direcciones de Bitcoin.
7. Pulse `True` o `False` según el resultado.

Los operandos de `OP_MULTISIG` se dividen típicamente, con el `0` y las firmas provenientes del script de desbloqueo y la "m", "n", y las direcciones se detallan en el script de bloqueo.

El requisito de que `0` como primer operando de `OP_CHECKMULTISIG` es una regla de consenso. Debido a que la versión original de `OP_CHECKMULTISIG` sacó accidentalmente un elemento adicional de la pila, Bitcoin debe seguir para siempre ese estándar, para que no se rompan accidentalmente los complejos scripts de canje de ese período de tiempo, haciendo que los fondos antiguos no se puedan canjear. 

> :book: ***¿Qué es una regla de consenso?*** Estas son las reglas que siguen los nodos de Bitcoin para trabajar juntos. En gran parte, están definidos por el código Bitcoin Core. Estas reglas incluyen muchos mandatos obvios como el límite de la cantidad de Bitcoins que se crean para cada bloque y las reglas sobre cómo se pueden respetar las transacciones. Sin embargo, también incluyen correciones para errores que han aparecido a lo largo de los años, porque una vez que se ha introducido un error en la base de código de Bitcoin, debe recibir soporte continuo para evitar que los antiguos Bitcoins se vuelvan insostenibles. 

## Crear una Multifima Sin Procesar

Como se explica en [§10.1: Creación de un Bitcoin Script con P2SH](10_1_Building_a_Bitcoin_Script_with_P2SH.md), las multifirmas son uno de los tipos de transacciones estándar de Bitcoin. Se puede crear una transacción con un script de bloqueo que utiliza el comando `OP_CHECKMULTISIG` sin procesar, y se aceptará en un bloque. Esta es la metodología clásica para usar multifirmas en Bitcoin.

Como ejemplo, volveremos a visitar la multifirma creado en [§8.1](08_1_Sending_a_Transaction_to_a_Multisig.md) una útltima vez y crearemos un nuevo script de bloqueo para él utilizando esta metodología. Como recordará, se trataba de una multifirma 2 de 2 construida a partir de `$address1` y `$address2`. 

Como el script de bloquo `OP_CHECKMULTISIG` requiere la "m" (`2`), las direcciones y la "n" (`2`), puede escribir la siguiente `scriptPubKey`:
```
2 $address1 $address2 2 OP_CHECKMULTISIG
```
Si esto le parece familiar, es porque es la firma múltiple que deserializó en [§8.2: Construcción de la Estructura de P2SH](08_2_Building_the_Structure_of_P2SH.md).
```
2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG
```

> **ADVERTENCIA:** Para las firmas clásicas `OP_CHECKMULTISIG`, "n" debe ser ≤ 3 para que la transacción sea estándar.

## Desbloquear un Multifirma Sin Procesar

El `scriptSig` para una dirección estándar mutlfirma debe enviar los operandos que faltan para `OP_CHECKMULTISIG`: un `0` seguido de firmas "m". Por ejemplo:
```
0 $signature1 $signature2
```

### Ejecutar una Guion de Multifirmas Sin Procesar

Para gastar un UTXO multifirma, ejecute `scriptSig` y `scriptPubKey` de la siguiente manera:
```
Script: 0 $signature1 $signature2 2 $address1 $address2 2 OP_CHECKMULTISIG
Stack: [ ]
```
Primero, coloca todas las constantes en la pila:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 2 ]
```
Entonces, el `OP_CHECKMULTISIG` comienza a ejecutarse. Primero se saca el "2":
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 ]
```
Luego, el "2" le dice a `OP_CHECKMULTISIG ` que muestre dos direcciones:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 ]
```
Luego, el siguiente "2" se saque:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 ]
```
Luego, el "2" le dice a `OP_CHECKMULTISIG` que saque dos firmas:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 ]
```
Luego, se saque un elemento más por error:
```
Running: OP_CHECKMULTISIG
Stack: [ ]
```
Luego, `OP_CHECKMULTISIG` completa su operación comparando las firmas "m" con las direcciones "n":
```
Script:
Stack: [ True ]
```
## Entender las Limitaciones de los Scripts de Multifirmas Sin Procesar

Desafortunadamente, la técnica de incrustar una multifirma sin procesar en una transacción tiene algunos inconvenientes notables:

1. Debido a que no existe un formato de dirección estándar para multifirma, cada remitente debe: ingresar un script multifirma largo y engorroso; tener un software que permita esto; y ser de confianza para no estropearlo.
2. Debido a que las múltiples funciones pueden ser mucho más largas que los script de bloqueo típicos, la cadena de bloques incurre en más costos. Esto requiere tarifas de transacción más altas por parte del remitente y crea más molestias para cada nodo.

En general, se trataba de problemas con cualquier tipo de script de Bitcoin complejo, pero rápidamente se convirtieron en problemas muy reales cuando se aplicaron a multifirmas, que fueron algunos de los primeros scripts complejos que se utilizaron ampliamente en la red de Bitcoin. Las transacciones P2SH se crearon para resolver estos problemas a partir de 2012. 

> :book: ***¿Qué es un P2SH multifirma?*** Las multifirmas P2SH fueron la primera implementación de transacciones P2SH. Simplemente empaquetan una transacción estándar de multifirmas en una transacción P2SH estándar. Simplemente empaquetan una transacción estándar de multifirma en una transacción P2SH estándar. Esto permite la estandarización de direcciones; reduce el almacenamiento de datos; y aumenta los recuentos de "m" y "n".

## Crear un P2SH Multifirma

Las multifirmas P2SH son la metodología moderna para crear multifirmas en los Blockchains. Se pueden crear de forma muy sencilla, utilizando el mismo proceso visto en las secciones anteriores.

### Crear la Cerradura para P2SH Multifirma

Para crear una multifirma P2SH, siga los pasos estándar para crear un script de bloqueo P2SH:

1. Serialice `2 $address1 $address2 2 OP_CHECKMULTISIG`.
   1. `<serializedMultiSig>` = "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
2. Guarde `<serializedMultiSig>` para referencia futura como redeemScript.
   1. `<redeemScript>` = "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
3. SHA-256 y RIPEMD-160 procesan el script serializado.
   1. `<hashedMultiSig>` = "a5d106eb8ee51b23cf60d8bd98bc285695f233f3"
4. Produzca un script de bloqueo P2SH Multifirma que incluya el script hash (`OP_HASH160 <hashedMultisig> OP_EQUAL`).
   1. `scriptPubKey` = "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387"
   
Luego puede crear una transacción usando esa `scriptPubKey`.

## Desbloquear el P2SH Multifirma

Para desbloquear esta transacción multifirma es necessario que el destinatario produzca un scriptSig que incluya las dos firmas y el `redeemScript`.

### Ejecute la Primera Ronda de Validación P2SH

Para desbloquear el P2SH multifirma, primero confirme el script:

1. Producir un script de desbloqueo de `0 $signature1 $signature2 <serializedMultiSig>`.
2. Concatenar eso con el script de bloqueo de `OP_HASH160 <hashedMultisig> OP_EQUAL`.
3. Validar `0 $signature1 $signature2 <serializedMultiSig> OP_HASH160 <hashedMultisig> OP_EQUAL`.
4. Tener éxito si el `<serializedMultisig>` coincide con el `<hashedMultisig>`.

### Ejecute la Segunda Ronda de Validación P2SH

Luego, ejecute el script multifirma:

1. Deserializar `<serializedMultiSig>` a `2 $address1 $address2 2 OP_CHECKMULTISIG`.
2. Concatenar eso con los operandos anteriores en el script de desbloqueo, `0 $signature1 $signature2`.
3. Validar `0 $signature1 $signature2 2 $address1 $address2 2 OP_CHECKMULTISIG`.
4. Tener éxito si los operandos cumplen con la deserialización `redeemScript`.

Ahora sabe cómo se creó realmente la transacción multifirma en [§6.1](06_1_Sending_a_Transaction_to_a_Multisig.md), cómo se validó para el gasto y por qué ese `redeemScript` era tan importante.

## Resumen: Creación de Guiones de Multifirmas

Las multifirmas son un tipo de transacción estándar, pero su uso es un poco complicado, por lo que se incorporan regularmente en las transacciones P2SH, como fue el caso en [§6.1](06_1_Sending_a_Transaction_to_a_Multisig.md) cuando creamos nuestras primeras multifirmas. El resultado es más limpio, más pequeño y más estandarizado — pero lo que es más importante, es un gran ejemplo del mundo real cómo funcionan realmente los scripts P2SH.

## Que Sigue?

Continúe "Incrustando Bitcoin Scripts" con [§10.5: Scripting a Segwit Script](10_5_Scripting_a_Segwit_Script.md)
