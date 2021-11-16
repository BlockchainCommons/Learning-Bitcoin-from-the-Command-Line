# 4.2 Creación de una transacción en Crudo

Creación de una transacción sin procesar
Ahora está listo para crear transacciones en crudo de Bitcoin. Esto le permite enviar dinero, pero elaborar las transacciones con la precisión que desee. Esta primera sección se centra en una simple transacción de una entrada y una salida. Este tipo de transacción _no_ es en realidad tan útil, porque rara vez va a querer enviar todo su dinero a una persona (a menos que lo esté reenviando, como por ejemplo si está moviendo cosas de un monedero a otro). Por lo tanto, no etiquetamos esta sección como una forma de enviar dinero. Es sólo un paso fundamental para _realmente_ enviar dinero con una transacción en bruto.

## Entender una transacción en Bitcoin

Antes de sumergirse en la creación de transacciones en crudo, debería asegurarse de que entiende cómo funciona una transacción en Bitcoin. Se trata de los UTXOs.

> :book: ***¿Qué es un UTXO?*** Cuando recibe dinero en efectivo en su monedero Bitcoin, aparece como una transacción individual. Cada una de estas transacciones se denomina Unspent Transaction Output (UTXO), o salida de transacción no gastada. No importa si se hicieron varios pagos a la misma dirección o a múltiples direcciones: cada transacción entrante permanece distinta en su monedero como un UTXO.

Cuando crea una nueva transacción saliente, reúne uno o más UTXOs, cada uno de los cuales representa una porción de dinero que ha recibido. Los utiliza como entradas para una nueva transacción. Su importe conjunto debe ser igual a lo que quieres gastar _o más_. Luego, genera una o más salidas, que dan el dinero representado por las entradas a una o más personas. Esto crea nuevos UTXOs para los receptores, que luego pueden usar _esos_ para financiar futuras transacciones.

Este es el truco: ¡Todos de los UTXOs que reúna se gastan en su totalidad! Esto significa que, si quiere enviar sólo una parte del dinero de un UTXO a otra persona, ¡También tiene que generar una salida adicional que te devuelva el resto! Por ahora, no nos preocuparemos por eso, pero el uso de una dirección de cambio será vital cuando pasemos de la teoría de este capítulo a transacciones más prácticas.

## Listar transacciones no gastadas

Para crear una nueva transacción en crudo, debe saber qué UTXOs tiene a mano para gastar. Puede determinar esta información con el comando `bitcoin-cli listunspent`:
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
    "confirmations": 20,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
    "vout": 1,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00050000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

```
Este listado muestra tres UTXOs diferentes, con un valor de .0001, .0005 y .00022 BTC. Nótese que cada uno tiene su propio txid distinto y permanece distinto en el monedero, incluso los dos últimos, que fueron enviados a la misma dirección.

Cuando quiera gastar un UTXO, no es suficiente con saber el id de la transacción. ¡Eso es porque cada transacción puede tener múltiples salidas! ¿Recuerda el primer trozo de dinero que nos envió el grifo? En la transacción, una parte del dinero fue para nosotros y otra para otra persona. El `txid` se refiere a la transacción global, mientras que el `vout` dice cuál de las múltiples salidas ha recibido. En esta lista, cada una de estas transacciones es la 0ª `salida` de una transacción anterior, pero _no tiene por qué ser así_.

Así, txid+vout=UTXO. Esta será la base de cualquier transacción en crudo.

## Escribir una transacción en crudo con una salida

Ahora está listo para escribir una simple transacción cruda de ejemplo que muestra cómo enviar la totalidad de un UTXO a otra parte. Como se ha señalado, este no es necesariamente un caso muy frecuente en el mundo real.

> :warning: **ADVERTENCIA:** Es muy fácil perder dinero con una transacción en crudo. Considera que todas las instrucciones sobre el envío de bitcoins a través de transacciones en bruto son _muy_, _muy_ peligrosas. Siempre que vaya a enviar dinero real a otras personas, deberá utilizar uno de los otros métodos explicados en este capítulo. Crear transacciones en crudo es extremadamente útil si estás escribiendo programas de bitcoin, pero _sólo_ cuando estás escribiendo programas de bitcoin. (Por ejemplo: al escribir este ejemplo para una versión de este tutorial, accidentalmente gastamos la transacción equivocada, aunque tenía cerca de 10 veces más valor. Casi todo eso se perdió en los mineros).

### Preparar la transacción en crudo

Para las mejores prácticas, comenzaremos cada transacción registrando cuidadosamente los txids y vouts que vamos a gastar.

En este caso, vamos a gastar el que vale .00050000 BTC porque es el único con un valor decente.
```
$ utxo_txid="61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a"
$ utxo_vout="1"
```

De la misma manera, deberá anotar la dirección del destinatario, para asegurarse de tenerla bien. Volvemos a enviar algo de dinero al grifo de TP:
```
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
```
Como siempre, comprueba sus variables con cuidado, para asegurarse de que son las que espera.
```
$ echo $utxo_txid
61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a
$ echo $utxo_vout
1
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
Este destinatario es especialmente importante, porque si se equivoca, ¡Su dinero está _perdido_! (Y, como ya hemos visto, elegir la transacción equivocada puede suponer la pérdida de dinero). Así que compruébelo todo tres veces.

### Comprender la comisión por transacción

Cada transacción tiene una tasa asociada. Es _implícito_ cuando envía una transacción en crudo: la cantidad que pagará como tarifa es siempre igual a la cantidad de su entrada menos la cantidad de su salida. Por lo tanto, tiene que disminuir su salida un poco de su entrada para asegurarse de que su transacción salga.

> :warning: **AVISO:** ¡Esta es la parte más peligrosa de las transacciones en crudo! Debido a que automáticamente gasta toda la cantidad en los UTXOs que utiliza, es de vital importancia que se asegure de saber (1) exactamente qué UTXOs está usando; (2) exactamente cuánto dinero contienen; (3) exactamente cuánto dinero está enviando; y (4) cuál es la diferencia. Si se equivoca y utiliza el UTXO equivocado (con más dinero del que pensabas) o si envías muy poco dinero, el exceso se pierde. Para siempre. ¡No cometa ese error! Conozca sus entradas y salidas _precisamente_. O mejor, no utilice las transacciones en crudo salvo como parte de un programa cuidadosamente estudiado y triplemente comprobado. 

> :book: ***¿Cuánto deberá gastar en tasas de transacción?*** [Bitcoin Fees](https://bitcoinfees.21.co/) tiene una buena evaluación en vivo. Dice que "la tarifa de transacción más rápida y barata es actualmente de 42 satoshis/byte" y que "para el tamaño medio de transacción de 224 bytes, esto resulta en una tarifa de 9.408 satoshis".

Actualmente Bitcoin Fees sugiere una tarifa de transacción de unos 10.000 satoshis, que es lo mismo que 0,0001 BC. Sí, eso es para la red principal, no para la red de pruebas, pero queremos probar las cosas de forma realista, así que eso es lo que vamos a utilizar.

En este caso, eso significa tomar los 0,0005 BTC en el UTXO que hemos seleccionado, reduciéndolos en 0,0001 BTC para la tasa de transacción, y enviando los 0,0004 BTC restantes. (Y este es un ejemplo de por qué los micropagos no funcionan en la red Bitcoin, porque una tarifa de transacción de 1$ más o menos es bastante cara cuando se está enviando 4$, y no digamos si estuviera intentando hacer un micropago de 0,50$. Pero siempre es por eso que tenemos Lightning).

> :warning: **ADVERTENCIA:** Cuanto más baja sea la tarifa de la transacción, más tiempo pasará antes de que su transacción se incorpore a un bloque. El sitio de tarifas de Bitcoin enumera los tiempos esperados, desde un esperado 0 bloques, hasta 22. Dado que los bloques se construyen de media cada 10 minutos, ¡esa es la diferencia entre unos minutos y unas horas! Por lo tanto, elija una tarifa de transacción que sea apropiada para lo que está enviando. Tenga en cuenta que nunca debe bajar de la tarifa mínima de retransmisión, que es de 0,0001 BTC.

### Escribir la transacción en crudo

Ahora está listo para crear la transacción en crudo. Esto utiliza el comando `createrawtransaction`, que puede parecer un poco intimidante. Eso es porque el comando `createrawtransaction` no le protege completamente del RPC JSON que utiliza bitcoin-cli. En su lugar, va a introducir un arreglo JSON para listar los UTXOs que está gastando y un objeto JSON para listar las salidas.

Este es el formato estándar:
```
$ bitcoin-cli createrawtransaction
'''[
     {
       "txid": "'$your_txid'",
       "vout": '$your_vout'
      }
]'''
'''{
   "'$your_recipient'": bitcoin_amount
 }'''
 ```
Sí, hay todo tipo de comillas locas ahí, pero confíe en que harán lo correcto. Use `'''` para marcar el inicio y el final del array JSON y del objeto JSON. Protege las palabras normales como `"this"`, pero no necesita proteger los números normales: `0`. Si son variables, inserta comillas simples, como `"'$this_word'"` y `'$this_num'`. (Ya se acostumbrá).

 Aquí hay un comando que crea una transacción en crudo para enviar su $utxo a su $recipient
 ```
$ rawtxhex=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' '''{ "'$recipient'": 0.0004 }''')
$ echo $rawtxhex
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f3610100000000ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```

### Verifique su transacción en crudo

A continuación debe verificar su rawtransaction con `decoderawtransaction` para asegurarse de que hará lo correcto.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "hash": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00040000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }
  ]
}
```

Compruebe el `vin`. ¿Está gastando la transacción correcta? ¿Contiene la cantidad de dinero esperada? (Compruébelo con `bitcoin-cli gettransaction` y asegúrese de mirar el `vout` correcto). Compruebe su `vout`. ¿Está enviando la cantidad correcta? ¿Va a la dirección correcta? Por último, haga los cálculos para asegurarse de que el dinero cuadra. ¿El valor del UTXO menos la cantidad que se está gastando es igual a la tasa de transacción prevista?

> :information_source: **NOTA - SECUENCIA:** Puede notar que cada entrada tiene un número de secuencia, establecido aquí en 4294967295, que es 0xFFFFFFFF. Esta es la última frontera de las transacciones de Bitcoin, porque es un campo estándar en las transacciones que originalmente estaba destinado a un propósito específico, pero nunca fue implementado completamente. Así que ahora hay este número entero en las transacciones que podría ser reutilizado para otros usos. Y, de hecho, lo ha sido. En el momento de escribir este artículo hay tres usos diferentes para la variable que se llama `nSequence` en el código de Bitcoin Core: permite RBF, `nLockTime`, y timelocks relativos. Si no hay nada raro, `nSequence` se establecerá en 4294967295. Establecer un valor más bajo indica que están ocurriendo cosas especiales.

### Firmar la transacción cruda

Hasta la fecha, su transacción cruda es sólo algo teórico: usted _podría_ enviarla, pero no se ha prometido nada. Tiene que hacer algunas cosas para que salga a la red.

En primer lugar, tiene que firmar su transacción en crudo:
```

$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
$ signedtx="02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
Obsérvese que hemos capturado el hexágono firmado a mano, en lugar de intentar analizarlo a partir del objeto JSON. Un paquete de software llamado "JQ" podría hacerlo mejor, como explicaremos en un próximo interludio.

### Enviar la transacción cruda

Ahora tiene una transacción en crudo lista para funcionar, pero no cuenta hasta que la ponga en la red, lo que se hace con el comando `sendrawtransaction`. Recibirá de vuelta un txid:
```
$ bitcoin-cli sendrawtransaction $signedtx
a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a
```
Inmediatamente verá que el UTXO y su dinero han sido eliminados de tu cartera:
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
    "confirmations": 23,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 6,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

$ bitcoin-cli getbalance
0.00032000
```
Pronto `listtransactions` deberá mostrar una transacción confirmada de la categoría "enviar".
```
 {
    "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
    "category": "send",
    "amount": -0.00040000,
    "vout": 0,
    "fee": -0.00010000,
    "confirmations": 1,
    "trusted": true,
    "txid": "a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a",
    "walletconflicts": [
    ],
    "time": 1592608574,
    "timereceived": 1592608574,
    "bip125-replaceable": "no",
    "abandoned": false
  }
```
Puede ver que coincide con el `txid` y la dirección del `receptor`. No sólo muestra el "importe" enviado, sino también la `comisión` de la transacción. Y, ya ha recibido una confirmación, porque ofrecimos una tarifa que haría que se incluyera en un bloque rápidamente.

¡Felicidades! ¡Ahora es unos cuantos satoshis más pobre!


## Resumen: Creación de una transacción en crudo

Cuando el dinero entra en su monedero Bitcoin, permanece como cantidades distintas, llamadas UTXOs. Cuando usted crea una transacción cruda para enviar ese dinero de vuelta, utiliza uno o más UTXOs para financiarlo. Entonces puede crear una transacción en crudo, firmarla y enviarla en la red Bitcoin. Sin embargo, esto es sólo una base: ¡normalmente necesitará crear una transacción cruda con múltiples salidas para enviar realmente algo en la red bitcoin!

## ¿Qué sigue?

Retroceda con "Enviando Transacciones en Bitcoin" con [Interludio: Usando JQ](04_2_Interludio_Usando_JQ.md).
