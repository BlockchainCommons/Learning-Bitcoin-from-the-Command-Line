# 9.1: Entendiendo la base de las transacciones

La base de Bitcoin es la capacidad de proteger las transacciones, algo que se hace con un lenguaje de programación simple.

## Conozca las partes del rompecabezas criptográfico

Como se describe en el [Capítulo 1](01_0_Introduccion.md), los fondos en cada transacción de Bitcoin están bloqueados con un rompecabezas criptográfico. Para ser precisos, dijimos que Bitcoin se compone de "una secuencia de transacciones atómicas". Observamos que: "Cada transacción es autenticada por un remitente con la solución a un rompecabezas criptográfico anterior que se almacenó como un script. La nueva transacción está bloqueada para el destinatario con un nuevo rompecabezas criptográfico que también se almacena como un script". Esos scripts, que bloquean y desbloquean transacciones, están escritos en Bitcoin Script.

>:book: ***¿Qué es Bitcoin Script?*** Bitcoin Script es un lenguaje similar a Forth basado en pilas que evita los bucles a propósito y, por lo tanto, no es Turing completo. Se compone de códigos de operación individuales. Cada transacción en Bitcoin está bloqueada con un script de Bitcoin; cuando la transacción de bloqueo para un UTXO se ejecuta con las entradas correctas, ese UTXO se puede gastar.

El hecho de que las transacciones estén bloqueadas con scripts significa que pueden bloquearse de varias formas diferentes, lo que requiere una variedad de claves diferentes. De hecho, hemos conocido varios mecanismos de bloqueo diferentes hasta la fecha, cada uno de los cuales usaba diferentes códigos de operación:

* OP_CHECKSIG, que compara una clave pública con una firma, es la base de la dirección P2PKH clásica, como se detallará completamente en [§9.4: Codificando un script P2PKH](09_4_Codificando_una_P2PKH.md).

* OP_CHECKMULTISIG verifica de manera similar las multifirma, como se detallará completamente en [§10.4: Codificando una Multifirma](10_4_Codificando_una_Multifirma.md).

* OP_CHECKLOCKTIMEVERIFY y OP_SEQUENCEVERIFY forman la base de bloqueos de tiempo más complejos, como se detallará completamente en [§11.2: Usando CLTV en Scripts](11_2_Usando_CLTV_en_Scripts.md) y [§11.3: Usando CSV en Scripts](11_3_Usando_CSV_en_Scripts.md).

* OP_RETURN es la marca de una transacción no prescindible, por lo que se utiliza para transportar datos, como se mencionó en [§8.2: Envío de una transacción con datos](08_2_Enviando_una_Transaccion_con_Datos.md).

## Acceda a los scripts en sus transacciones

Es posible que no se dé cuenta, pero ya ha visto estos scripts de bloqueo y desbloqueo como parte de las transacciones sin procesar con las que ha estado trabajando. La mejor manera de examinar estos scripts con más profundidad es crear una transacción sin procesar y luego examinarla.

### Crear una transacción de prueba

Para examinar scripts de desbloqueo y bloqueo reales, cree una transacción sin procesar rápida tomando un UTXO Legacy no gastado y reenvíela a una dirección de cambio Legacy, menos la tarifa de transacción:

```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ recipient=$(bitcoin-cli -named getrawchangeaddress address_type=legacy)
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.0009 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
```

En realidad, no es necesario que la envíe: el objetivo es simplemente producir una transacción completa que pueda examinar.

>** NOTA:** ¿Por qué las direcciones heredadas (legacy)? Porque sus scripts son más significativos. Sin embargo, también ofreceremos un ejemplo de un SegWit P2WPKH nativo en [§9.5](09_5_Codificando_una_P2WPKH.md).

### Examine su transacción de prueba

Ahora puede examinar su transacción a profundidad mediante el uso de `decoderawtransaction` sobre el `$signedtx`:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "hash": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "version": 2,
  "size": 191,
  "vsize": 191,
  "weight": 764,
  "locktime": 0,
  "vin": [
    {
      "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
      "vout": 0,
      "scriptSig": {
        "asm": "304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b",
        "hex": "47304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c01210315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
    }
  ]
}

``` 
Los dos scripts se encuentran en diferentes partes de la transación.

El `scriptSig` se encuentra en el` vin`. Este es el script _de desbloqueo_. Es lo que se ejecuta para acceder al UTXO que se utiliza para financiar esta transacción. Habrá un `scriptSig` por UTXO en una transacción.

El `scriptPubKey` se encuentra en el` vout`. Este es el script _de bloqueo_. Es lo que bloquea la nueva salida de la transacción. Habrá una `scriptPubKey` por salida en una transacción.

>:book:***¿Cómo interactúan scriptSig y scriptPubKey?*** El `scriptSig` de una transacción desbloquea el UTXO anterior; La salida de esta nueva transacción se bloqueará con un `scriptPubKey`, que a su vez puede ser desbloqueado por el` scriptSig` de la transacción que reutiliza ese UTXO.

### Lea los scripts en su transacción

Mire los dos scripts y verá que cada uno incluye dos representaciones diferentes: el `hex` es lo que realmente se almacena, aunque el lenguaje ensamblador más legible (`asm`) puede mostrarle lo que está sucediendo.

Eche un vistazo al `asm` del script de desbloqueo y verá por primera vez cómo se ve Bitcoin Scripting:

```
04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
```

Da la casualidad de que ese lío de números es una firma de clave privada seguida de la clave pública asociada. O al menos eso es lo que es, con suerte, porque eso es lo que se requiere para desbloquear el UTXO P2PKH que está utilizando esta transacción.

Lea el script de bloqueo y verá que es mucho más obvio:

```
OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG
```

Ese es el método estándar en Bitcoin Script para bloquear una transacción P2PKH.

[§9.4](09_4_Codificando_una_P2PKH.md) explicará cómo estos dos scripts van juntos, pero primero necesitará saber cómo se evalúan los scripts de Bitcoin.

## Examine un tipo diferente de transacción

Antes de dejar atrás esta base, veremos un tipo diferente de secuencia de comandos de bloqueo. Aquí está el `scriptPubKey` de la transacción multisig que creó en [§6.1: Enviando una Transacción a una Dirección Multifirma](06_1_Enviando_una_Transaccion_a_una_Direccion_Multifirma.md).

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

Compare eso con el `scriptPubKey` de su nueva transacción P2PKH:

```
    "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
```
Estas dos transacciones están _definitivamente_ bloqueadas de diferentes formas. Bitcoin reconoce el primero como `scripthash` (P2SH) y el segundo como` pubkeyhash` (P2PKH), pero también debería poder ver la diferencia en los distintos códigos `asm`:`OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL` frente a `OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG`.  Este es el poder de las secuencias de comandos: puede producir de manera muy simple algunos de los tipos de transacciones dramáticamente diferentes de los que aprendió en los capítulos anteriores.

## Resumen: Entendiendo la base de las transacciones

Cada transacción de Bitcoin incluye al menos un script de desbloqueo (`scriptSig`), que resuelve un rompecabezas criptográfico anterior, y al menos un script de bloqueo (`scriptPubKey`), que crea un nuevo rompecabezas criptográfico. Hay un "scriptSig" por entrada y un "scriptPubKey" por salida. Cada uno de estos scripts está escrito en Bitcoin Script, un lenguaje similar a Forth que potencia aún más a Bitcoin.


>:fire:***¿Cuál es el poder de los scripts?*** Los scripts desbloquean todo el poder de los contratos inteligentes. Con los códigos de operación adecuados, puede tomar decisiones muy precisas sobre quién puede canjear fondos, cuándo pueden canjear fondos y cómo pueden canjear fondos. También se pueden codificar en un script reglas más complejas para el gasto corporativo, el gasto en asociaciones, el gasto por poder y otras metodologías. Incluso habilita servicios de Bitcoin más complejos, como Lightning y sidechains.

## ¿Qué sigue?

Continúe "Introduciendo los scripts de Bitcoin" con [§9.2: Ejecutando un Script Bitcoin](09_2_Ejecutando_un_Script_Bitcoin.md).
