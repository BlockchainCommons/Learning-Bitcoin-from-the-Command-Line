# 3.4: Recibir una transacción

Ahora está listo para recibir algo de dinero en la nueva dirección que configuró.

## Conseguir algo de dinero

Para hacer algo más, necesita obtener algo de dinero. En testnet, esto se hace a través de faucets. Dado que el dinero es fingido, simplemente vaya a un grifo, solicite algo de dinero y se lo enviará. Sugerimos usar el faucet en https://testnet-faucet.mempool.co/, https://bitcoinfaucet.uo1.net/ o https://testnet.coinfaucet.eu/en/. Si no están disponibles por alguna razón, busque "bitcoin testnet faucet" y debería encontrar otros.

Para usar un faucet, generalmente deberá ir a una URL y copiar y pegar su dirección. Tenga en cuenta que este es uno de esos casos en los que, lamentablemente, no podrá utilizar variables de la línea de comandos. Posteriormente, se creará una transacción que le enviará dinero desde el grifo.

> :book: ***¿Qué es una transacción?*** Una transacción es un intercambio de bitcoins. El propietario de algunos bitcoins usa su clave privada para acceder a esas monedas, luego bloquea la transacción usando la clave pública del destinatario.

> :link: ** TESTNET vs MAINNET: ** Lamentablemente, no hay grifos en la vida real. Si estuvieras jugando en la red principal, deberías ir y comprar bitcoins en un intercambio de bitcoins o en un cajero automático, o necesitarías que alguien te los envíe. La vida de Testnet es mucho más fácil.

## Verifique su dinero

Una vez que haya solicitado su dinero, debería poder verificarlo con el comando 'bitcoin-cli getbalance':

```
$ bitcoin-cli getbalance
0.00000000
```

Pero espere, ¿¡todavía no hay saldo!?

Bienvenido al mundo de la latencia de Bitcoin. ¡El problema es que su transacción aún no se ha registrado en un bloque!

> :book: ***¿Qué es un bloque?*** Las transacciones se transmiten a través de la red y los mineros las reúnen en bloques. Estos bloques están asegurados con una prueba matemática de trabajo, que demuestra que la potencia de cálculo se ha gastado como parte de la creación del bloque. Es esa prueba de trabajo (multiplicada en muchos bloques, cada uno construido sobre el último) lo que, en última instancia, mantiene a Bitcoin seguro.

> :book: ***¿Qué es un minero?*** Un minero es un participante de la red Bitcoin que trabaja para crear bloques. Es un trabajo remunerado: cuando un minero crea con éxito un bloque, se le paga una recompensa única más las tarifas por las transacciones en su bloque. La minería es un gran negocio. Los mineros tienden a funcionar con hardware especial, acelerado de manera que es más probable que puedan crear bloques. También tienden a ser parte de grupos de minería, donde todos los mineros acuerdan compartir las recompensas cuando uno de ellos crea un bloque con éxito.

Afortunadamente, `bitcoin-cli getunconfirmedbalance` aún debería mostrar su saldo actualizado siempre que se haya creado la transacción inicial:

```
$ bitcoin-cli getunconfirmedbalance
0.01010000
```

Si todavía muestra un cero, probablemente se esté moviendo a través de este tutorial demasiado rápido. Espere un segundo. Las monedas deben aparecer sin confirmar, luego pasar rápidamente a confirmadas. Tenga en cuenta que una moneda puede pasar de un saldo no confirmado a un saldo confirmado casi de inmediato, así que asegúrese de verificar ambos. Sin embargo, si su `getbalance` y su `getunconfirmedbalance` siguen mostrando cero en diez minutos, entonces probablemente haya algo mal con el grifo y tendrá que elegir otro.

### Gane confianza en su dinero

Puede usar `bitcoin-cli getbalance "*"[n]`, donde reemplaza `[n]` con un número entero, para ver si un saldo confirmado es 'n' bloques de profundidad

> :book: ***¿Qué es la profundidad del bloque?*** Después de que se construye y se confirma un bloque, se construye otro bloque encima y otro ... Debido a que se trata de un proceso estocástico, existe alguna posibilidad de reversión cuando un bloque aún es nuevo. Por lo tanto, un bloque debe estar enterrado a varios bloques de profundidad en una cadena antes de que pueda sentirse totalmente seguro de sus fondos. Cada uno de esos bloques tiende a construirse en un promedio de 10 minutos ... por lo que generalmente toma alrededor de una hora para que una transacción confirmada reciba seis bloques de profundidad, que es la medida de plena confianza en Bitcoin.

A continuación se muestra que nuestras transacciones se han confirmado una vez, pero no dos veces:

```
$  bitcoin-cli getbalance "*" 1
0.01010000
$  bitcoin-cli getbalance "*" 2
0.00000000
```

Obviamente, cada diez minutos más o menos esta profundidad aumentará.

Por supuesto, en la red de prueba, nadie está tan preocupado por la confiabilidad de sus fondos. Podrás gastar tu dinero tan pronto como se confirme.

## Verificar su billetera

El comando `bitcoin-cli getwalletinfo` le brinda más información sobre el saldo de su billetera:

```
$ bitcoin-cli getwalletinfo
{
  "walletname": "",
  "walletversion": 169900,
  "balance": 0.01010000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 2,
  "keypoololdest": 1592335137,
  "keypoolsize": 999,
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "keypoolsize_hd_internal": 1000,
  "paytxfee": 0.00000000,
  "private_keys_enabled": true,
  "avoid_reuse": false,
  "scanning": false
}
```

## Descubra su ID de transacción

Su dinero llegó a su billetera a través de una transacción. Puede descubrir ese transactionid (txid) con el comando `bitcoin-cli listtransactions` :

```
$ bitcoin-cli listtransactions
[
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.01000000,
    "label": "",
    "vout": 1,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 73,
    "blocktime": 1592600085,
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "walletconflicts": [
    ],
    "time": 1592599884,
    "timereceived": 1592599884,
    "bip125-replaceable": "no"
  },
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.00010000,
    "label": "",
    "vout": 0,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 72,
    "blocktime": 1592600085,
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "walletconflicts": [
    ],
    "time": 1592599938,
    "timereceived": 1592599938,
    "bip125-replaceable": "no"
  }
]

```

Aquí se muestran dos transacciones (`8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9`) y (`ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36`) para una cantidad específica ( `0.01000000` y` 0.00010000`), que tanto se recibieron ( `receive`) por la misma dirección en nuestra cartera (`mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE`) . Eso es una mala higiene de claves, por cierto: debe usar una nueva dirección para cada Bitcoin que reciba. En este caso, nos impacientamos porque el primer grifo no parecía funcionar.

Puede acceder a información similar con el comando `bitcoin-cli listunspent`, pero solo muestra las transacciones por el dinero que no ha gastado. Estos se denominan UTXO y serán de vital importancia cuando envíe dinero al mundo de Bitcoin:

```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "vout": 1,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  }
]
```

Tenga en cuenta que los bitcoins no son solo un lío homogéneo de dinero en efectivo atascado en su bolsillo. Cada transacción individual que recibe o envía se coloca en el libro mayor inmutable de blockchain, en un bloque. Puede ver estas transacciones individuales cuando mira su dinero no gastado. Esto significa que el gasto en bitcoins no es tan anónimo como parece. Aunque las direcciones son bastante privadas, las transacciones se pueden examinar a medida que entran y salen de las direcciones. Esto hace que la privacidad sea vulnerable al análisis estadístico. También introduce cierta no fungibilidad potencial para bitcoins, ya que puede rastrear una serie de transacciones, incluso si no puede rastrear un "bitcoin" específico.

> :book: ***¿Por qué todas estas cantidades de bitcoins están en fracciones?*** Los bitcoins se producen lentamente, por lo que hay relativamente pocos en circulación. Como resultado, cada bitcoin en la red principal vale bastante (~ $ 9,000 en el momento de escribir este artículo). Esto significa que la gente suele trabajar en fracciones. De hecho, el .0101 en monedas de Testnet valdría alrededor de $ 100 si estuvieran en la red principal. Por esta razón, han aparecido nombres para cantidades más pequeñas de bitcoins, incluidos milibitcoins o mBTC (una milésima parte de un bitcoin), microbitcoins o bits o μBTC (una millonésima parte de un bitcoin) y satoshis (una centésima millonésima de un bitcoin). 

## Examine su transacción

Puede obtener más información sobre una transacción con el comando `bitcoin-cli gettransaction`:

```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9"
{
  "amount": 0.01000000,
  "confirmations": 1,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00"
}
```

El comando `gettransaction` detallará las transacciones que están en su billetera, como esta, que nos fue enviada.

Tenga en cuenta que "gettransaction" tiene dos argumentos opcionales:

```
$ bitcoin-cli help gettransaction
gettransaction "txid" ( include_watchonly verbose )

Get detailed information about in-wallet transaction <txid>

Arguments:
1. txid                 (string, required) The transaction id
2. include_watchonly    (boolean, optional, default=true for watch-only wallets, otherwise false) Whether to include watch-only addresses in balance calculation and details[]
3. verbose              (boolean, optional, default=false) Whether to include a `decoded` field containing the decoded transaction (equivalent to RPC decoderawtransaction)
```

Al establecer estos dos como verdaderos o falsos, podemos elegir incluir direcciones de solo observación en la salida (lo que no nos importa) o mirar una salida más detallada (lo que hacemos).

En su lugar, esto es lo que estos datos miran cuando establecemos `include_watchonly` en` false` y `verbose` en` true`.

```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9" false true
{
  "amount": 0.01000000,
  "confirmations": 3,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00",
  "decoded": {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "hash": "d4ae2b009c43bfe9eba96dcd16e136ceba2842df3d76a67d689fae5975ce49cb",
    "version": 2,
    "size": 249,
    "vsize": 168,
    "weight": 669,
    "locktime": 1772327,
    "vin": [
      {
        "txid": "80f57b9779a769eb8772faf319265a46b94499d7d51df5db7a13b0d17749d014",
        "vout": 1,
        "scriptSig": {
          "asm": "0014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f",
          "hex": "160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f"
        },
        "txinwitness": [
          "304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e13301",
          "022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c"
        ],
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01010143,
        "n": 0,
        "scriptPubKey": {
          "asm": "OP_HASH160 5c3bfb36b03f279967977ca9d1e35185e3991778 OP_EQUAL",
          "hex": "a9145c3bfb36b03f279967977ca9d1e35185e399177887",
          "reqSigs": 1,
          "type": "scripthash",
          "addresses": [
            "2N1ev1WKevSsdmAvRqZf7JjvDg223tPrVCm"
          ]
        }
      },
      {
        "value": 0.01000000,
        "n": 1,
        "scriptPubKey": {
          "asm": "OP_DUP OP_HASH160 1b72503639a13f190bf79acf6d76255d772360b7 OP_EQUALVERIFY OP_CHECKSIG",
          "hex": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
          "reqSigs": 1,
          "type": "pubkeyhash",
          "addresses": [
            "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE"
          ]
        }
      }
    ]
  }
}
```


Ahora puede ver la información completa sobre la transacción, incluidas todas las entradas ("vin") y todas las salidas ("vout). Una de las cosas interesantes a tener en cuenta es que, aunque recibimos .01 BTC en la transacción, otra .01010143 se envió a otra dirección. Probablemente se trataba de un cambio de dirección, un concepto que se explora en la siguiente sección. Es bastante típico que una transacción tenga múltiples entradas y / o múltiples salidas.

Hay otro comando, `getrawtransaction`, que le permite ver las transacciones que no están en su billetera. Sin embargo, requiere que tenga un nodo sin podar y `txindex = 1` en su archivo` bitcoin.conf`. A menos que tenga una necesidad seria de información que no esté en su billetera, probablemente sea mejor usar un explorador de Bitcoin para este tipo de cosas ...

## Opcional: Usar un explorador de bloques

Incluso mirar la información detallada de una transacción puede resultar un poco intimidante. El objetivo principal de este tutorial es enseñar cómo lidiar con transacciones sin procesar desde la línea de comandos, pero nos complace hablar sobre otras herramientas cuando sean aplicables. Una de esas herramientas es un explorador de bloques, que puede utilizar para ver las transacciones desde un navegador web en un formato mucho más amigable.

Actualmente, nuestro explorador de bloques preferido es [https://live.blockcypher.com/](https://live.blockcypher.com/).

Puede usarlo para buscar transacciones para una dirección:

[https://live.blockcypher.com/btc-testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE/](https://live.blockcypher.com/btc-testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE/)

También puede usarlo para ver transacciones individuales:

[https://live.blockcypher.com/btc-testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9/](https://live.blockcypher.com/btc-testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9/)


Un explorador de bloques generalmente no proporciona más información que una mirada de línea de comandos a una transacción sin procesar; simplemente hace un buen trabajo al resaltar la información importante y armar las piezas del rompecabezas, incluidas las tarifas de transacción detrás de una transacción, otro concepto que cubriremos en secciones futuras.

## Resumen: Recibir una transacción

Los Faucets le darán dinero en la red de prueba. Vienen como transacciones sin procesar, que se pueden examinar con "gettransaction" o un explorador de bloques. Una vez que haya recibido una transacción, podrá verla en su saldo y en su billetera.

## ¿Que sigue?

Para profundizar en cómo se describen las direcciones, de modo que se puedan transferir o convertir en partes de una firma múltiple, consulte [§3.5: Comprensión del descriptor](03_5_Understanding_the_Descriptor.md).

Pero si eso es demasiado profundo, continúe con el [Capítulo cuatro: Envío de transacciones de Bitcoin](04_0_Sending_Bitcoin_Transactions.md).
