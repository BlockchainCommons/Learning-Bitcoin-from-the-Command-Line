# 9.5: Codificando una P2WPKH

> :information_source: **NOTE:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector con precaución.

Las UXTO's P2PKH están bien para explicar la forma fundamental en que funcionan los scripts de Bitcoin, pero ¿qué pasa con los scripts nativos de SegWit P2WPKH, que se están convirtiendo cada vez más en la mayoría de las transacciones de Bitcoin? Resulta que las direcciones P2WPKH no usan scripts de Bitcoin como lo hacen las direcciones tradicionales de Bitcoin, por lo que esta sección es realmente una digresión hacia el scripting de este capítulo, aunque es importante porque describe la otra forma principal en la cual bitcoin puede ser enviado.

## Ver un script P2WPKH

Es bastante fácil ver cómo se ve un script P2WPKH. La siguiente transacción sin procesar se creó gastando un UXTO P2WPKH y luego enviando el dinero a una dirección de cambio P2WPKH, tal como hicimos con una dirección heredada en [§9.1](09_1_Entendiendo_la_base_de_las_Transacciones.md).
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "bdf8f12768a9870d41ac280f8bb4f8ecd9d2fa66fffc75606811f5751c17cb3a",
  "hash": "ec09c84cae48694bec7fd3461b3c5b38a76829c56e9d876037bf2484d443174b",
  "version": 2,
  "size": 191,
  "vsize": 110,
  "weight": 437,
  "locktime": 0,
  "vin": [
    {
      "txid": "3f5417bc7a3a4144d715f3f006d35ea2b405f06091cbb9ce492e04ccefe02b18",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001",
        "03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 92a0db923b3a13eb576a40c4b35515aa30206cba",
        "hex": "001492a0db923b3a13eb576a40c4b35515aa30206cba",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qj2sdhy3m8gf7k4m2grztx4g44gczqm96y6sszv"
        ]
      }
    }
  ]
}
```

Probablemente hay dos cosas sorprendentes aquí: (1) No hay `scriptSig` para desbloquear la transacción anterior; y (2) el `scriptPubKey` para bloquear la nueva transacción es justo 0 92a0db923b3a13eb576a40c4b35515aa30206cb.

Eso es, simplemente, ¡porque P2WPKH funciona de manera diferente!

## Comprender una transacción P2WPKH

Una transacción P2WPKH contiene la misma información que una transacción P2PKH clásica, aunque la coloca en lugares extraños y no dentro de un script Bitcoin tradicional, y ese es el punto exacto de las transacciones SegWit, el hecho de extraer la información del "testigo", que sirve para indicar las claves públicas y las firmas, fuera de la transacción para admitir un cambio en el tamaño del bloque.

Pero, si observa con atención, verá que el vacío `scriptSig` ha sido reemplazado por dos entradas en una nueva sección `txinwitness`. Si examina sus tamaños y formato, deberían parecerle familiares: son una firma y una clave pública. Del mismo modo, si mira en el `scriptPubKey`, verá que está compuesto por un 0 (en realidad: `OP_0` es el número de versión de SegWit) y otro número largo, que es el hash de clave pública.


Aquí hay una comparación de nuestros dos ejemplos:
| Type | PubKeyHash | PubKey | Signature |
|----------------|----------|-------------|---------|
| SegWit | 92a0db923b3a13eb576a40c4b35515aa30206cba | 03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903 | 3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001 |
| SegWit | 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c |

Entonces, ¿cómo funciona esto? Depende de que el código antiguo interprete esto como una transacción válida y el nuevo código sepa verificar la nueva información del "testigo".

### Leer un script de SegWit en una máquina antigua

Si un nodo no se ha actualizado para admitir SegWit, entonces ejecuta el truco habitual de concatenar el `scriptSig` y el `scriptPubKey`. Esto produce: `0 92a0db923b3a13eb576a40c4b35515aa30206cba` (porque solo hay un `scriptPubKey`). Ejecutar eso producirá una pila con todo lo que contiene en orden inverso:

```
$ btcdeb '[0 92a0db923b3a13eb576a40c4b35515aa30206cba]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
miniscript failed to parse script; miniscript support disabled
valid script
2 op script loaded. type `help` for usage information
script                                   |  stack 
-----------------------------------------+--------
0                                        | 
92a0db923b3a13eb576a40c4b35515aa30206cba | 
#0000 0
btcdeb> step
		<> PUSH stack 
script                                   |  stack 
-----------------------------------------+--------
92a0db923b3a13eb576a40c4b35515aa30206cba | 0x
#0001 92a0db923b3a13eb576a40c4b35515aa30206cba
btcdeb> step
		<> PUSH stack 92a0db923b3a13eb576a40c4b35515aa30206cba
script                                   |                                   stack 
-----------------------------------------+-----------------------------------------
                                         | 92a0db923b3a13eb576a40c4b35515aa30206cba
                                         | 0x
```

Los scripts de Bitcoin se consideran exitosos si hay algo en la pila y no es cero, por lo que los scripts de SegWit tienen éxito automáticamente en los nodos antiguos siempre que el `scriptPubKey` se cree correctamente con un hash de clave pública distinto de cero. Esto se llama una transacción "cualquiera puede gastar", porque los nodos antiguos verificaron que eran correctos sin necesidad de firmas.

> :book: ***Qué los nodos antiguos no pueden robar UTXOs SegWit ?*** SegWit se habilitó en la red Bitcoin cuando el 95% de los mineros señalaron que estaban listos para comenzar a usarlo. Eso significa que solo el 5% de los nodos en ese momento podrían haber registrado transacciones SegWit que _cualquiera pueda gastar_ como válidas sin pasar por el trabajo adecuado de verificar el `txinwitness`. Si incorporaron incorrectamente un UTXO inválido que _cualquiera puede gastar_ en un bloque, el otro 95% de los nodos se negaría a validar ese bloque, por lo que rápidamente quedaría huérfano en lugar de agregarse a la cadena de bloques "principal". (Ciertamente, el 51% de los nodos podría optar por dejar de interpretar correctamente las transacciones de SegWit, pero el 51% de los nodos puede hacer cualquier cosa en una red de consenso como una cadena de bloques)

Debido a que los nodos antiguos siempre ven los scripts de SegWit como correctos, siempre los verificarán, incluso sin comprender su contenido.

### Leer un script de SegWit en una máquina nueva

A machine that understands how SegWit work does the exact same things that it would with an old P2PKH script, but it doesn't use a script per se: it just knows that it needs to hash the public key in the `txinwitness`, check that against the hashed key after the version number in the `scriptPubKey` and then run `OP_CHECKSIG` on the signature and public key in the `txinwitness`.

Una máquina que entiende cómo funciona SegWit hace exactamente las mismas cosas que haría con un antiguo script P2PKH, pero no usa un script per se: simplemente sabe que necesita hacer un hash de la clave pública en el `txinwitness`, compruébelo contra la clave hasheada después del número de versión en el `scriptPubKey` y luego ejecute `OP_CHECKSIG` en la firma y la clave pública en el `txinwitness`.

Entonces, es otra forma de hacer lo mismo, pero sin tener los scripts integrados en las transacciones. (En su lugar, el proceso está integrado en el software del nodo).

## Resumen: Programación de un hash de clave pública de Pay to Witness

En gran medida usted no codifica un script P2WPKH. En cambio, Bitcoin Core crea la transacción de una manera diferente, colocando la información del testigo en un lugar diferente en lugar del tradicional `scriptSig`. Eso significa que los P2WPKH son una digresión de los scripts de Bitcoin de esta parte del libro, porque son una expansión de Bitcoin que se aleja del scripting tradicional.

Sin embargo, SegWit también fue un uso inteligente de los scripts de Bitcoin. Sabiendo que habría nodos que no se actualizarían y necesitarían ser compatibles con versiones anteriores, los desarrolladores crearon el formato P2WPKH para que generara un script que siempre se validara en los nodos antiguos (mientras que ese script proporcionaba información a los nuevos nodos en la forma de un número de versión y una clave pública hasheada).

Cuando está programando desde la línea de comandos, fundamentalmente no tiene que preocuparse por esto, aparte de saber que no encontrará scripts tradicionales en transacciones SegWit sin procesar (que, nuevamente, era el punto).

## Que sigue?

Continúe "Creación de secuencias de comandos de Bitcoin" con el [Chapter 10: Embedding Bitcoin Scripts in P2SH Transactions](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md).

