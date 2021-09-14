# 10.1: Entendiendo la Base de P2SH

Sabe que los scripts de Bitcoin se pueden usar para controlar el canje de UTXOs. El siguiente paso es crear sus propios scripts ... pero eso requiere una técnica muy específica.

## Conozca los Estándares de Bitcoin

Aquí está la trampa para usar los scripts de Bitcoin: por razones de seguridad, la mayoría de los nodos de Bitcoin solo aceptarán seis tipos de transacciones de Bitcoin "estándar".

* __Pagar a Clave Pública (Pay to Public Key, P2PK)__ — Una transacción antigua y obsoleta (`<pubKey> OP_CHECKSIG`) que ha sido reemplazada por la mejor seguridad de P2PKH.
* __Pagar al Testigo del Hash de la Clave Pública (Pay to Public Key Hash, P2PKH)__ — Una transacción (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) que paga el hash de una clave pública.
* __Pagar Para Ser Testigo de Hash de Clave Pública (Pay to Witness Public Key Hash, P2WPKH)__ — El tipo más nuevo de transacción de clave pública. Es solo (`OP_0 <pubKeyHash`) porque depende del consenso del minero para funcionar, como se describe en [§9.5](09_5_Scripting_a_P2WPKH.md).
* __Multifirma (Multisig)__ — Una transacción para un grupo de claves, como se explica con más detalle en [§8.4](08_4_Scripting_a_Multisig.md).
* __Datos Nulos (Null Data)__ — Una transacción invencible (`OP_RETURN Data`).
* __Pagar a Script Hash (Pay to Script Hash, P2SH)__ — Una transacción que paga a un script específico, como se explica con más detalle aqui.

Entonces, ¿cómo se escribe un script de Bitcoin más complejo? La respuesta está en ese último tipo de transacción estándar, el P2SH. Puede poner cualquier tipo de script largo y complejo en una transacción P2SH, y siempre que siga las reglas estándar para incrustar su script y canjear los fondos, obtendrá todos los beneficios de Bitcoin Scripting.

> :warning: **ADVERTENCIA DE VERSIÓN:** Los scripts P2SH arbitrarios solo se convirtieron en estándar a partir de Bitcoin Core 0.10.0. Antes de eso, solo se permitían P2SH Multifirmas.

## Entender el P2SH Script

Ya vio una transacción P2SH cuando creó una firma múltiple en [§6.1: Envío de una Transacción a una Multifirma](06_1_Sending_a_Transaction_to_a_Multisig.md). Aunque multifirma es uno de los tipos de transacciones estándar, `bitcoin-cli` simplifica el uso de sus multifirmas al incrustarlas en transacciones P2SH, como se describe con más detalle en [§8.4: Scripting a Multisig](08_4_Scripting_a_Multisig.md).

Entonces, veamos una vez más el `scriptPubKey` de ese P2SH multifirma:
```
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
```
El script de bloqueo es bastante simple: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL`. Como de costumbre, hay una gran cantidad de datos en el medio. Este es un hash de otro script de bloqueo oculto (`redeemScript`) que solo se revelará cuando se canjeen los fondos. En otras palabras, el script de bloqueo estándar para una dirección P2SH es: `OP_HASH160 <redeemScriptHash> OP_EQUAL`.

> :book: ***¿Qué es un redeemScript?*** Cada transacción P2SH lleva la huella digital de un script de bloqueo oculto dentro de ella como un hash de 20 bytes. Cuando se canjea una transacción P2SH, el `redeemScript` completo (sin hash) se incluye como parte del `scriptSig`. Bitcoin se asegurará de que el `redeemScript` coincida con el hash; luego ejecuta el `redeemScript` para ver si los fondos se pueden gastar (o no).

 ¡Uno de los elementos interesantes de las transacciones P2SH es que ni el remitente ni la cadena de bloques saben realmente qué es el `redeemScript`! Un remitente simplemente envía a una dirección P2SH estandarizada marcada con un prefijo "2" y no se preocupa por cómo el destinatario va a recuperar los fondos al final.

> :link: **TESTNET vs MAINNET:** en testnet, el prefijo para las direcciones P2SH es `2`, mientras que en mainnet, es `3`.

## Entender Cómo Crear un Script P2SH

Dado que el script de bloqueo visible para una transacción P2SH es tan simple, crear una transacción de este tipo también es bastante simple. En teoria. Todo lo que necesita hacer es crear una transacción cuyo script de bloqueo incluya un hash de 20 bytes del `redeemScript`. Ese hash se realiza con el estándar de Bitcoin `OP_HASH160`.

> :book: ***Qué es OP_HASH160?*** La operación hash estándar para Bitcoin realiza un hash SHA-256, luego un HASH RIPEMD-160.

En general, se requieren cuatro pasos:

1. Cree un script de bloqueo arbitrario con Bitcoin Script.
2. Cree una versión serializada de ese script de bloqueo.
3. Realice un hash SHA-256 en esos bytes serializados.
4. Realice un hash RIPEMD-160 en los resultados de ese hash SHA-256.

Cada uno de esos pasos, por supuesto, requiere algo de trabajo por sí solo, y algunos de ellos pueden ser bastante intrincados. La buena noticia es que realmente no tiene que preocuparse por por ellos, porque son lo sucifientemente complejos como para que normalmente tenga una API que se encargue de todo por usted. 

Entonces, por ahora, solo le proporcionaremos una descripción general, para que comprenda la metodología general. En [§10.2: Construyendo la Estructura de P2SH](10_2_Building_the_Structure_of_P2SH.md) proporcionaremos una mirada más profunda a la creación de scripts, en caso de que alguna vez quiera comprender las entrañas de este proceso.

## Comprender Cómo Enviar una Transacción de Script P2SH

Entonces, ¿cómo envía realmente su transacción P2SH? Nuevamente, la teoría es muy simple:

1. Incruste su hash en un script `OP_HASH160 <redeemScriptHash> OP_EQUAL`.
2. Traducir eso a código hexadecimal.
3. Usa ese hexademical como su `scriptPubKey`. 
4. Crea el resto de la transacción.

Desafortunadamente, este es otro lugar donde necesitará recurrir a las API, en gran parte porque `bitcoin-cli` no proporciona ningún soporte para la creación de transacciones P2SH. (Puede redimirlos muy bien.)

## Entender Cómo Desbloquear una Transacción de Script P2SH

El truco para canjear una transacción P2SH es que el destinatario debe haber guardado el script de bloqueo serializado secreto que fue codificado para crear la dirección P2SH. Esto se llama `redeemScript` porque es lo que el destinatario necesita para canjear sus fondos. 

Un `scriptSig` de desbloqueo para una transacción P2SH se forma como: `... data ... <redeemScript>`. Los `datos` deben ser _solo_ datos que se insertan en la pila, no operadores. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) los llama firmas, pero eso no es un requisito real.)

> :warning: **ADVERTENCIA:** Aunque las firmas no son un requisito, un script P2SH en realidad no es muy seguro si no requiere al menos una firma en sus entradas. Las razones de esto se describen en [§13.1: Escritura de Scripts de Rompecabezas](13_1_Writing_Puzzle_Scripts.md).

Cuando se canjea un UTXO, se ejecuta en dos rondas de verificación:

1. Primero, el `redeemScript` en el `scriptSig` es codificado y se compara con el script codificado en el `scriptPubKey`. 
2. Si coinciden, comienza una segunda ronda de verificación.
3. En segundo lugar, el `redeemScript` se ejecuta utilizando los datos anteriores que se insertaron en la pila. 
4. Si esa segunda ronda de verificación _también_ tiene éxito, el UTXO se desbloquea.

Si bien no puede crear fácilmente una transacción P2SH sin una API, debería poder canjear fácilmente una transacción P2SH con `bitcoin-cli`. De hecho, ya lo hizo en [§6.2: Envío de una Transacción a un Multifirma](06_2_Spending_a_Transaction_to_a_Multisig.md). El proceso exacto se describe en [§10.6: Gasto de una Transacción P2SH](10_6_Spending_a_P2SH_Transaction.md), una vez que hayamos terminado con todas las complejidades de la creación de transacciones P2SH.

> :warning: **ADVERTENCIA:** Puede crear una transacción perfectamente válida con un código redeemScript correctamente codificado, pero si el código redeemScript no se ejecuta o no se ejecuta correctamente, sus fondos se perderán para siempre. Por eso es tan importante probar sus secuencias de comandos, como se explíca en [§9.3: Prueba de una Bitcoin Script](09_3_Testing_a_Bitcoin_Script.md).

## Resumen: Comprensión de la Base de P2SH

Los scripts arbitrarios de Bitcoin no son estándar en Bitcoin. Sin embargo, puede incorporarlos en transacciones estándar utilizando el tipo de dirección P2SH. Simplemente aplica un hash a su script como parte del script de bloqueo, luego lo revela y lo ejecuta como parte del script de desbloqueo. Siempre que pueda satisfacer el `redeemScript`, el UTXO se puede gastar. 

> :fire: ***¿Cuál es el Poder de P2SH?*** Ya conoce el poder de Bitcoin Script, el cual le permite crear contratos inteligentes más complejos de todo tipo. P2SH es lo que realmente libera ese poder al permitirle incluir un script de Bitcoin arbitrario en las transacciones estándar de Bitcoin.

## Que Sigue?

Continúe "Incrustando Bitcoin Scripts" con [§10.2: Construyendo la Estructura de P2SH](10_2_Building_the_Structure_of_P2SH.md).
