# 8.1: Envío de una transacción con un tiempo de bloqueo

Los capítulos anteriores mostraron dos formas diferentes de enviar fondos desde múltiples máquinas y a múltiples destinatarios. Pero hay otras dos formas de cambiar fundamentalmente las transacciones básicas. El primero de ellos es variar el tiempo eligiendo un tiempo de bloqueo. Esto le brinda la posibilidad de enviar transacciones sin procesar en algún momento en el futuro.

## Entender como funcionan los bloqueos de tiempo

Cuando crea una transacción de tiempo de bloqueo, la bloquea con un número que representa la altura de un bloque (si es un número pequeño) o una marca de tiempo UNIX (si es un número grande). Esto le dice a la red Bitcoin que la transacción no se puede poner en un bloque hasta que haya llegado la hora especificada o la cadena de bloques haya alcanzado la altura especificada.

>:book: _¿Cuál es la altura del bloque? _ Es el recuento total de bloques en la cadena, volviendo al bloque génesis de Bitcoin.

Cuando una transacción de tiempo de bloqueo está esperando para entrar en un bloque, se puede cancelar. Esto significa que está lejos, lejos de estar finalizado. De hecho, la capacidad de cancelar es el objetivo principal de una transacción de tiempo de bloqueo.

>:book: _¿Qué es nLockTime? _ Es lo mismo que locktime. Más específicamente, es lo que se denomina tiempo de bloqueo interno al código fuente de Bitcoin Core.

>:book: _¿Qué es Timelock?_ Locktime es solo una forma de bloquear las transacciones de Bitcoin hasta algún momento en el futuro; colectivamente, estos métodos se denominan bloqueos de tiempo. Locktime es el método de bloqueo de tiempo más básico. Bloquea una transacción completa con un tiempo absoluto, y está disponible a través de `bitcoin-cli` (por lo que es el único bloqueo de tiempo cubierto en esta sección). Un método paralelo, que bloquea una transacción con un tiempo relativo, se define en [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) y se cubre en [§11.3: Usando CSV en Scripts](11_3_Using_CSV_in_Scripts.md).

> El lenguaje de Script de Bitcoin potencia aún más ambos tipos de bloqueos de tiempo, lo que permite el bloqueo de salidas individuales en lugar de transacciones completas. Los bloqueos de tiempo absolutos (como Locktime) están vinculados al código de operación de script OP_CHECKLOCKTIMEVERIFY, que se define en [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) y se cubre en [§11.2: Uso de CLTV en scripts](11_2_Using_CLTV_in_Scripts.md), mientras que los temporizadores relativos (como Timelock) están vinculados al código de operación de script OP_CHECKSEQUENCEVERIFY, que se define en [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) y también está cubierto en [§11.3](11_3_Using_CSV_in_Scripts.md).

## Crear una transacción de Locktime

Para crear una transacción de tiempo de bloqueo, primero debe determinar en qué establecerá el tiempo de bloqueo.

### Determine su tiempo de bloqueo por marca de tiempo UNIX

Con mayor frecuencia, establecerá el tiempo de bloqueo en una marca de tiempo de UNIX que representa una fecha y hora específicas. Puede calcular una marca de tiempo de UNIX en un sitio web tal como [Sello de tiempo de UNIX](http://www.unixtimestamp.com/) o en [Epoch Convertor](https://www.epochconverter.com/). Sin embargo, sería mejor [escribir su propio script](https://www.epochconverter.com/#code) en su máquina local, para que sepa que la marca de tiempo UNIX que recibe es precisa. Si no lo hace, al menos verifique dos veces en dos sitios diferentes.

>:book: _¿Por qué debería usar una marca de tiempo de UNIX?_ El uso de una marca de tiempo de UNIX facilita la vinculación definitiva de una transacción 
>a un momento específico, sin preocuparse por si la velocidad de creación de bloques podría cambiar en algún momento. En particular, si está creando un 
>tiempo de bloqueo que está muy alejado en el futuro, es lo más seguro que puede hacer. Pero, más allá de eso, es más intuitivo, ya que crea una 
>correlación directa entre alguna fecha del calendario y el momento en que se puede extraer la transacción.

>:warning: **ADVERTENCIA:** El tiempo de bloqueo con marcas de tiempo de UNIX tiene un poco de margen de maniobra: la liberación de bloques no es regular y los tiempos de bloqueo pueden adelantarse dos horas al tiempo real, por lo que un tiempo de bloqueo en realidad significa "dentro de unas pocos horas de este tiempo, más o menos".

### Calcule su tiempo de bloqueo por altura de bloque

Alternativamente, puede establecer el tiempo de bloqueo en un número más pequeño que represente la altura de un bloque. Para calcular la altura futura de su bloque, primero debe saber cuál es la altura actual del bloque. `bitcoin-cli getblockcount` le dirá cuál piensa su máquina local que es la altura del bloque. Es posible que desee volver a verificar con un explorador de Bitcoin.

Una vez que haya descubierto la altura actual, puede decidir qué tan lejos en el futuro establecerá su tiempo de bloqueo. Recuerde que, en promedio, se creará un nuevo bloque cada 10 minutos. Entonces, por ejemplo, si quisiera establecer el tiempo de bloqueo en una semana en el futuro, elegiría una altura de bloque de 6 x 24 x 7 = 1,008 bloques antes que la actual.

>:book: _¿Por qué debería usar una altura de bloque?_ A diferencia de las marcas de tiempo, no hay confusión para las alturas de bloque. Si establece una altura de bloque de 120,000 para su tiempo de bloqueo, entonces no hay absolutamente ninguna forma de que entre en el bloque 119,999. Esto puede facilitar el control algorítmico de su transacción bloqueada. La desventaja es que no puede estar tan seguro de cuándo será exactamente el tiempo de bloqueo.

>:advertencia: ** ADVERTENCIA: ** Si desea establecer un tiempo de bloqueo de altura de bloque, debe establecer el tiempo de bloqueo en menos de 500 millones. Si lo establece en 500 millones o más, su número se interpretará como una marca de tiempo. Dado que la marca de tiempo de UNIX de 500 millones fue el 5 de noviembre de 1985, eso probablemente significa que su transacción será puesta en un bloque en la primera oportunidad de los mineros.

## Escriba su transacción

Una vez que haya descubierto su tiempo de bloqueo, todo lo que necesita hacer es escribir una transacción sin formato típica, con una tercera variable para `locktime`:

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.00095 }''' locktime=1774650)
```
Tenga en cuenta que este uso de "locktime" es inferior a 500 millones, lo que significa que define una altura de bloque. En este caso, son solo unos pocos bloques más allá de la altura del bloque actual en el momento de escribir este artículo, con la intención de ejemplificar cómo funciona el tiempo de bloqueo sin sentarse durante mucho tiempo para esperar y ver qué sucede.

Así es como se ve la transacción creada:

```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "hash": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "version": 2,
  "size": 113,
  "vsize": 113,
  "weight": 452,
  "locktime": 1774650,
  "vin": [
    {
      "txid": "0ad9fb6992dfe4ea90236b69852b3605c0175633b32996a486dcd0b2e739e385",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00100000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "hex": "0014f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q7ve42nxqsvxs82wp7fn43ch8urc425ul5um4un"
        ]
      }
    },
    {
      "value": 0.00095000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a37718a3510958112b6a766e0023ff251b6c2bfb",
        "hex": "0014a37718a3510958112b6a766e0023ff251b6c2bfb",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q5dm33g63p9vpz2m2wehqqglly5dkc2lmtmr98d"
        ]
      }
    }
  ]
}
```
Tenga en cuenta que el número de secuencia (`4294967294`) es menor que` 0xffffffff`. Esta es una señalización necesaria para mostrar que la transacción incluye un tiempo de bloqueo. También lo hace automáticamente `bitcoin-cli`. Si, en cambio, el número de secuencia se establece en `0xffffffff`, se ignorará el tiempo de bloqueo.

>:information_source: **NOTA - SECUENCIA:** Este es el segundo uso del valor `nSequence` en Bitcoin. Al igual que con RBF, `nSequence` se usa nuevamente como opción de suscripción, esta vez para el uso del tiempo de bloqueo. 0xffffffff-1 (4294967294) es el valor preferido para señalar el tiempo de bloqueo porque intencionalmente no permite el uso de RBF (que requiere `nSequence <0xffffffff-1`) y bloqueo de tiempo relativo (que requiere` nSequence <0xf0000000`), los otros dos usos del valor `nSequence`. Si configura `nSequence` por debajo de `0xf0000000`, entonces también bloqueará el tiempo relativo de su transacción, que probablemente no sea lo que desea.

>:warning: ** ADVERTENCIA: ** Si está creando una transacción sin procesar de tiempo de bloqueo por algún otro medio que no sea `bitcoin-cli`, tendrá que establecer la secuencia a menos de `0xffffffff` a mano.

A estas alturas probablemente ya esté familiarizado con terminar las cosas:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
error code: -26
error message:
non-final
```

¡Ups! ¿¡Qué error es ese!?

Desde 2013, generalmente no puede colocar la transacción bloqueada por tiempo en el mempool hasta que su bloqueo haya expirado. Sin embargo, aún puede retener la transacción, reenviéndola ocasionalmente a la red Bitcoin hasta que sea aceptada en el mempool. Alternativamente, puede enviar la transacción firmada (`$signedtx`) al destinatario, para que pueda colocarla en el mempool cuando el tiempo de bloqueo haya expirado.

Una vez transcurrido el tiempo de bloqueo, cualquiera puede enviar esa transacción firmada a la red, y el destinatario recibirá el dinero según lo previsto ... siempre que la transacción no se haya cancelado.

## Cancelar una transacción de Locktime

Cancelar una transacción de tiempo de bloqueo es _muy_ simple: envía una nueva transacción utilizando al menos una de las mismas UTXO.

## Resumen: Envío de una transacción con un tiempo de bloqueo

Locktime ofrece una forma de crear una transacción que _debería_ no ser retransmitida a la red y que _no será_ aceptada en un bloque hasta que llegue el momento apropiado. Mientras tanto, se puede cancelar simplemente reutilizando un UTXO.

>:fire: _¿Cuál es el poder del tiempo de bloqueo?_ El poder del tiempo de bloqueo puede no ser inmediatamente obvio debido a la capacidad de cancelarlo tan fácilmente. Sin embargo, es otra de las bases de los contratos inteligentes (Smart Contracts: tiene mucha utilidad en una variedad de aplicaciones de custodia o contractuales. Por ejemplo, considere una situación en la que un tercero tiene sus bitcoins. Para garantizar la devolución de sus bitcoins si el custodio alguna vez desapareciera, podrían producir una transacción de bloqueo de tiempo para devolverle las monedas y luego actualizarlas de vez en cuando con una nueva, en el futuro. Si alguna vez no se actualizaran, las monedas le regresarían cuando expirara el bloqueo de tiempo actual. El tiempo de bloqueo podría aplicarse de manera similar a una red de pago, donde la red contiene monedas mientras los participantes de la red las intercambian. Finalmente, un testamento ofrece un ejemplo de un contrato más complejo, en el que los pagos se envían a varias personas. Estos pagos se basarían en transacciones de tiempo de bloqueo y se actualizarían continuamente siempre que el propietario continúe mostrando signos de vida. (El factor unificador de todas estas aplicaciones es por supuesto, la _confianza_. Las transacciones simples de tiempo de bloqueo solo funcionan si se puede confiar en que el poseedor de las monedas las enviará en las condiciones adecuadas).

## ¿Que sigue?

Continúe "Expandiendo transacciones de Bitcoin" con [§8.2: Envío de una transacción con datos](08_2_Enviando_una_Transaccion_con_Datos.md).
