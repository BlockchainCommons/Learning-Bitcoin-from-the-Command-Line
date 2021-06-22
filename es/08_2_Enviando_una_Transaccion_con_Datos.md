# 8.2: Enviando una transacción con datos 

En esta última forma para variar una transacción básica es usar la transacción para enviar datos en lugar de fondos (o en realidad, además de fondos). Esto le brinda la capacidad de incrustar información en la cadena de bloques. Se realiza mediante un comando especial `OP_RETURN`.

El truco? Solo puede almacenar 80 bytes a la vez!

## Crear sus datos.

Lo primero que debe hacer es crear los 80 bytes (o menos) de datos que grabará en su `OP_RETURN`. Esto puede ser tan simple como preparar un mensaje o puede estar usando datos existentes. Por ejemplo, `sha256sum` produce 256 bits de datos, que son 32 bytes, muy por debajo de los límites:

```
$ sha256sum contract.jpg
b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75  contract.jpg
$ op_return_data="b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75"
```
>:book: _¿Qué es un OP_RETURN?_ Todas las transacciones de Bitcoin se basan en scripts de código opcode que veremos en el próximo capítulo. El `OP_RETURN` es un código de operación simple que define una SALIDA como inválida. La convención ha dado como resultado que se utilice para incrustar datos en la cadena de bloques.


## Prepare algún dinero

Su propósito al crear una transacción de datos no es enviar dinero a nadie, es poner datos en la cadena de bloques. Sin embargo, _debe_ enviar dinero para hacerlo. Solo necesita usar una dirección de cambio como su _único_ destinatario. Luego, puede identificar un UTXO y enviarlo a su dirección de cambio, menos una tarifa de transacción, mientras usa la misma transacción para crear un OP_RETURN.

Aquí está la configuración estándar:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "vout": 0,
    "address": "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh",
    "scriptPubKey": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
    "amount": 0.01463400,
    "confirmations": 1392,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/1'/12']02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb)#cjr03mru",
    "safe": true
  }
]

$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
```

## Escribir la transacción sin procesar (raw)

Ahora puede escribir una nueva transacción sin procesar con dos salidas: una es su dirección de cambio para recuperar (la mayor parte) su dinero, la otra es una dirección de datos, que es el término `bitcoin-cli` para un OP_RETURN.

```
rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "data": "'$op_return_data'", "'$changeaddress'": 0.0146 }''')
```

Así es como se ve realmente esa transacción:
```
{
  "txid": "a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8",
  "hash": "a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8",
  "version": 2,
  "size": 125,
  "vsize": 125,
  "weight": 500,
  "locktime": 0,
  "vin": [
    {
      "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_RETURN b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "hex": "6a20b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "type": "nulldata"
      }
    },
    {
      "value": 0.01460000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 998a9b0ed076bbdec1d88da4f475b9dde75e3620",
        "hex": "0014998a9b0ed076bbdec1d88da4f475b9dde75e3620",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qnx9fkrksw6aaaswc3kj0gademhn4ud3q7cz4fm"
        ]
      }
    }
  ]
}

```
Como puede ver, esto envía la mayor parte del dinero directamente a la dirección de cambio (`tb1qnx9fkrksw6aaaswc3kj0gademhn4ud3q7cz4fm`) menos una pequeña tarifa de transacción. Más importante aún, la primera salida muestra un OP_RETURN con los datos (`b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75`) inmediatamente después.

## Enviar la transacción sin procesar

Firme su transacción sin procesar y envíela, así muy pronto dicho OP_RETURN se integrará en la cadena de bloques!

## Verifique su OP_RETURN

Nuevamente, recuerde que puede ver esta transacción usando un explorador de blockchain:
[https://live.blockcypher.com/btc-testnet/tx/a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8/](https://live.blockcypher.com/btc-testnet/tx/a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8/)

Es posible que observe una advertencia sobre los datos que se encuentran en un "protocolo desconocido". Si estuviera diseñando un uso regular de datos OP_RETURN, probablemente lo marcaría con un prefijo especial para marcar ese protocolo. Entonces, los datos reales de OP_RETURN podrían ser algo así como "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". Este ejemplo no usó un prefijo para evitar enturbiar el espacio de datos.

## Resumen: Envío de una transacción con datos

You can use an `OP_RETURN` opcode to store up to 80 bytes of data on the blockchain. You do this with the `data` codeword for a `vout`. You still have to send money along too, but you just send it back to a change address, minus a transaction fee.

Puede usar un código de operación (opcode) OP_RETURN para almacenar hasta 80 bytes de datos en la cadena de bloques. Haz esto con la palabra `data` en clave para una salida `vout`. También debe enviar dinero, pero simplemente devuélvalo a una dirección de cambio, menos una tarifa de transacción.

>:fire: ¿Cuál es el poder de OP_RETURN? El OP_RETURN abre posibilidades completamente nuevas para la cadena de bloques, porque puede incrustar datos que prueben que ciertas cosas sucedieron en ciertos momentos. Varias organizaciones han utilizado OP_RETURN como prueba de existencia, derechos de autor, monedas de colores y [otros fines](https://en.bitcoin.it/wiki/OP_RETURN). Aunque 80 bytes pueden parecer no muchos datos, puede ser bastante efectivos si se utiliza OP_RETURN para almacenar hashes de los datos reales. Luego, puede probar la existencia de sus datos digitales demostrando que el hash coincide con el hash en la cadena de bloques.

Tenga en cuenta que existe cierta controversia sobre el uso de la cadena de bloques de Bitcoin de esta manera.

## Que sigue?

Pase a "Scripting con Bitcoin" en [Chapter Nine: Introducing Bitcoin Scripts](09_0_Introducing_Bitcoin_Scripts.md).

