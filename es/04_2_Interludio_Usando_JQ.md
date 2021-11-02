# Interludio: Usando JQ

La creación de una transacción en crudo reveló cómo los resultados más complejos de bitcoin-cli no pueden guardarse fácilmente en variables de línea de comandos. La respuesta es JQ, que permite extraer elementos individuales de JSON más complejos.

## Instalación de JQ

JQ esta disponible en [Github repository](https://stedolan.github.io/jq/). Descargar para Linux, OS X, or Windows, según corresponda.

Una vez que haya descargado el binario, puede instalarlo en su sistema. Si está trabajando en un VPS Debian como sugerimos, tu instalación se verá así:
```
$ mv jq-linux64 jq
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin jq
```
> :book: ***¿Qué es JQ?*** jq es como sed para datos JSON - puede usarlo para cortar y filtrar y mapear y transformar datos estructurados con la misma facilidad que sed, awk, grep y amigos se permiten jugar con el texto".

## Utilizar JQ para acceder al valor de un objeto JSON por clave

**Ejemplo de uso:** _Capturar el hex de una transacción cruda firmada._

En la sección anterior, el uso de `signrawtransaction` ofreció un ejemplo de no poder capturar fácilmente los datos en las variables debido al uso de JSON como salida:
```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
```
Afortunadamente, JQ puede capturar fácilmente este tipo de datos.

Para utilizar JQ, ejecute `jq` en el extremo posterior de una tubería, y utilice siempre la invocación estándar de `jq -r '.'`. El `-r` le dice a JQ que produzca una salida cruda, que funcionará para las variables de la línea de comandos, mientras que el `.` le dice a jq que salga. Protegemos ese argumento en `' ` porque necesitaremos esa protección más adelante cuando nuestras invocaciones de `jq` se vuelvan más complejas.

Para capturar un valor específico de un objeto JSON, basta con enumerar la clave después del `.`:
```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex'
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
Con esa herramienta en la mano, puede capturar información de objetos JSON en variables de línea de comandos:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex')
$ echo $signedtx
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
Así podrá utilizar esas variables fácilmente y sin errores:
```
$ bitcoin-cli sendrawtransaction $signedtx
3f9ccb6e16663e66dc119de1866610cc4f7a83079bfec2abf0598ed3adf10a78
```
## Utilizar JQ para acceder a los valores de un objeto JSON en un arreglo por clave

**Ejemplo de uso:** _Capturar el txid y vout para un UTXO seleccionado._

Sacar datos de un objeto JSON es fácil, pero ¿Qué pasa si ese objeto JSON está en un arreglo JSON? El comando `listunspent` ofrece un gran ejemplo, porque normalmente contendrá un número de transacciones diferentes. ¿Y si quiere capturar información específica de _una_ de ellas?

Cuando se trabaja con un array JSON, lo primero que hay que hacer es decirle a JQ a qué índice debe acceder. Por ejemplo, puede que haya mirado sus transacciones en `listunspent` y que haya decidido que quiere trabajar con la segunda de ellas. Usas `'.[1]'` para acceder a ese segundo elemento. El `[]` dice que estamos haciendo referencia a un arreglo JSON y el `1` dice que queremos el índice 1.
```
$ bitcoin-cli listunspent | jq -r '.[1]'
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
  "label": "",
  "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
  "amount": 0.00022,
  "confirmations": 9,
  "spendable": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
  "safe": true
}
```
A continuación, puede capturar un valor individual de ese arreglo seleccionado (1) utilizando una tubería _dentro_ de los argumentos JQ; y luego (2) solicitando el valor específico después, como en el ejemplo anterior. Lo siguiente capturaría el `txid` del objeto JSON número 1 del arreglo JSON producido por `listunspent`:
```
$ bitcoin-cli listunspent | jq -r '.[1] | .txid'
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
```
Observe cuidadosamente cómo las `' 's` van alrededor de toda la expresión JQ _incluyendo_ la tubería.

Este método se puede utilizar para rellenar las variables de un UTXO que se quiera utilizar:
```
$ newtxid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid')
$ newvout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ echo $newtxid
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
$ echo $newvout
0
```
¡Voila! Ahora podemos crear una nueva transacción en crudo utilizando nuestro primer UTXO como entrada, ¡Sin tener que teclear ninguna información del UTXO a mano!

## Utilizar JQ para acceder a valores de objetos JSON coincidentes con un arreglo por clave

**Ejemplo de uso:** _Lista el valor de todos los UTXOs no gastados._

En lugar de acceder a un único valor específico en un objeto JSON específico, se puede acceder a todo un valor específico en todos los objetos JSON. Esto se hace con `.[]`, donde no se especifica ningún índice. Por ejemplo, esto listaría todos los fondos no gastados:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.0001
0.00022
```

## Utilizar JQ para cálculos sencillos por clave

**Ejemplo de uso:** _Sumar el valor de todos los UTXOs no gastados._

En este punto, puede empezar a usar la salida de JQ para hacer cálculos sencillos. Por ejemplo, sumando los valores de esas transacciones no gastadas con un simple script `awk` le dara el equivalente a `getbalance`:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
0.00032
$ bitcoin-cli getbalance
0.00032000
```

## Usar JQ para mostrar múltiples valores de objetos JSON en un arreglo de claves múltiples

**Ejemplo de uso:** _Lista de información de uso para todos los UTXOs._

JQ puede capturar fácilmente elementos individuales de objetos JSON y arreglos y colocar esos elementos en variables. Ese será su principal uso en futuras secciones. Sin embargo, también se puede utilizar para reducir las enormes cantidades de información emitidas por `bitcoin-cli` en cantidades razonables de información.

Por ejemplo, puede querer ver un listado de todos sus UTXOs (`.[]`) y obtener un listado de toda su información más importante (`.txid, .vout, .amount`):
```
$ bitcoin-cli listunspent | jq -r '.[] | .txid, .vout, .amount'
ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36
0
0.0001
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
0
0.00022
```
Esto hace que sea fácil decidir qué UTXOs gastar en una transacción en crudo, pero no es muy bonito.

Afortunadamente, JQ también le permite ser elegante. Puede usar `{}`s para crear nuevos objetos JSON (ya sea para un análisis adicional o para una salida bonita). También puede definir el nombre de la nueva clave para cada uno de sus valores. La salida resultante debería ser mucho más intuitiva y menos propensa a errores (aunque, obviamente, menos útil para volcar información directamente en las variables).

El siguiente ejemplo muestra exactamente el mismo análisis de `listunspent`, pero con cada objeto JSON antiguo reconstruido como un nuevo objeto JSON abreviado, con todos los nuevos valores nombrados con sus antiguas claves:
```
$ bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'
{
  "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "vout": 0,
  "amount": 0.0001
}
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "amount": 0.00022
}
```
Por supuesto, puede cambiar el nombre de sus nuevas llaves como mejor se parezca. No hay nada mágico en los nombres originales.
```
$ bitcoin-cli listunspent | jq -r '.[] | { tx: .txid, output: .vout, bitcoins: .amount }'
{
  "tx": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "output": 0,
  "bitcoins": 0.0001
}
{
  "tx": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "output": 0,
  "bitcoins": 0.00022
}
```
## Utilizar JQ para acceder a objetos JSON por valor de búsqueda

**Ejemplo de uso:** _Buscar automáticamente los UTXOs que se utilizan en una transacción._

Hasta ahora, las búsquedas en JQ han sido bastante sencillas: se utiliza una clave para buscar uno o más valores en un objeto o arreglo JSON. ¿Pero qué pasa si quiere buscar un valor en un objeto JSON... por otro valor? Este tipo de búsqueda indirecta tiene una aplicación real cuando se trabaja con transacciones construidas sobre UTXOs existentes. Por ejemplo, puede permitirse calcular el valor de la suma de los UTXOs que se utilizan en una transacción, algo que es de vital importancia.

Este ejemplo utiliza la siguiente transacción en crudo. Tenga en cuenta que esta es una transacción cruda más compleja con dos entradas y dos salidas. Aprenderemos a hacerlas en próximas secciones; por ahora, es necesario para poder ofrecer ejemplos robustos. Observa que, a diferencia de nuestros ejemplos anteriores, éste tiene dos objetos en su matriz `vin` y dos en su matriz `vout`.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "hash": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "size": 160,
  "vsize": 160,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    },
    {
      "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
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
      "value": 1.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 cfc39be7ea3337c450a0c77a839ad0e160739058 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914cfc39be7ea3337c450a0c77a839ad0e16073905888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mzTWVv2QSgBNqXx7RC56zEhaQPve8C8VS9"
        ]
      }
    },
    {
      "value": 0.04500000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 166692bda9f25ced145267bb44286e8ee3963d26 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914166692bda9f25ced145267bb44286e8ee3963d2688ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP"
        ]
      }
    }
  ]
}
```

### Recuperar el(los) valor(es)

Supongamos que sabemos exactamente cómo se construye esta transacción: sabemos que utiliza dos UTXOs como entrada. Para recuperar el txid de los dos UTXOs, podríamos usar `jq` para buscar el valor .vin de la transacción, luego referenciar el arreglo 0 del .vin, y luego el valor .txid de ese arreglo. Después, podríamos hacer lo mismo con el 1er arreglo, y luego lo mismo con los dos valores .vout del .vin. Fácil:
```
$ usedtxid1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ echo $usedtxid1
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ usedtxid2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .txid')
$ echo $usedtxid2
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .vout')
$ echo $usedvout1
1
$ usedvout2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .vout')
$ echo $usedvout2
1
```
Sin embargo, sería mejor tener un caso general que _automáticamente_ guardara todos los txids de nuestros UTXOs.

Ya sabemos que podemos acceder a todos los `.txid`s utilizando un valor de matriz `.[]`. Podemos usar eso para construir una búsqueda general de .txid:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ echo ${usedtxid[0]}
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ echo ${usedtxid[1]}
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ echo ${usedvout[0]}
1
$ echo ${usedvout[1]}
1
```
El único truco real aquí es cómo guardamos la información usando el shell bash. En lugar de guardar en una variable con `$(comando)`, guardamos en una matriz con `($(comando))`. Entonces pudimos acceder a los elementos individuales de la matriz bash con una construcción `${variable[n]}`. En cambio, podíamos acceder a todo el arreglo con `${variable[@]}`. (Sí, nadie dijo nunca que bash fuera bonito).

> :warning: **ADVERTENCIA:** Recuerde siempre que un UTXO es una transacción _más_ un vout. La primera vez que escribimos este ejemplo de JQ se nos escapó el vout, y dejó de funcionar cuando acabamos con una situación en la que nos habían enviado dos `vouts` de la misma transacción.

### Recuperar los objetos relacionados

Ahora puede utilizar la información guardada de `txid` y `vout` para referenciar UTXOs en `listunspent`. Para encontrar la información sobre los UTXOs que están siendo utilizados por la transacción en crudo, es necesario buscar en todo el arreglo JSON (`[]`) de las transacciones no gastadas. A continuación, puede seleccionar (`select`) objetos JSON individuales que incluyan (`contains`) los txids. A continuación, seleccione (`select`) las transacciones que también contengan (`contain`) la salida correcta vout.

El uso de otro nivel de tuberías es la metodología estándar de JQ: se toma un conjunto de datos, luego se reduce a todas las transacciones relevantes, luego se reduce a los vouts que realmente se usaron de esas transacciones. Sin embargo, los argumentos `select` y `contains` son algo nuevo. Muestran algo de la complejidad de JSON que va más allá del alcance de este tutorial; por ahora, solo hay que saber que esta invocación particular funcionará para agarrar objetos que coincidan.

Para empezar de forma sencilla, esto escoge los dos UTXO de uno en uno:
```
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[0]}'")) | select(.vout | contains('${usedvout[0]}'))'
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[1]}'")) | select(.vout | contains('${usedvout[1]}'))'
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 5,
  "spendable": true,
  "solvable": true
}
```
Un simple bucle for-loop de bash podría, en cambio, traer _todos_ sus UTXOs:
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout'))'; done;
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 7,
  "spendable": true,
  "solvable": true
}
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}

```
Obsérvese que hemos utilizado otra parte de la fealdad de la arreglo `${#usedtxid[*]}` para determinar el tamaño del arreglo, y luego hemos accedido a cada valor en el arreglo `usedtxid` y a cada valor en el arreglo paralelo `usedvout`, colocándolos en variables más simples para un acceso menos feo.

## Utilizar JSON para el cálculo simple por valor

**Ejemplo de uso:** _Calcular automáticamente el valor de los UTXOs utilizados en una transacción._

Ahora puede ir un paso más allá, y solicitar el .amount (o cualquier otro valor-clave JSON) de los UTXOs que está recuperando.

Este ejemplo repite el uso de las arreglos `$usedtxid` y `$usedvout` que fueron establecidas de la siguiente manera:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
```
El mismo script `for` se puede utilizar para recorrer esos arreglos, pero con una tubería añadida en el JQ da salida al valor `amount` para cada uno de los UTXOs seleccionados.
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done;
0.9
0.4
```
En este punto, también se pueden sumar las .cantidades con un script `awk`, para ver realmente cuánto dinero hay en los UTXOs que la transacción está gastando:
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}'
1.3
```
¡Uf!

## Utilizar JQ para cálculos complejos

**Ejemplo de uso:** _Calcular la tasa de una transacción._

Para calcular la tarifa completa de la transacción en este punto sólo hace falta un poco más de matemáticas: determinar cuánto dinero pasa por el .vout. Este es un uso sencillo de JQ en el que simplemente se utiliza `awk` para sumar el `valor` de toda la información de `vout`:
```
$ bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}'
1.045
```
Para completar el cálculo de la tasa de transacción, se resta el importe de .vout (1.045) del importe de .vin (1.3).

Para ello, tendrá que instalar `bc`:
```
$ sudo apt-get intall bc
```

Al juntar todo esto se crea una calculadora completa en sólo cinco líneas de script:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ btcin=$(for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}')
$ btcout=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
$ echo "$btcin-$btcout"| /usr/bin/bc
.255
```
Y este es también un buen ejemplo de por qué hay que comprobar dos veces las tarifas: teníamos la intención de enviar una tarifa de transacción de 5.000 satoshis, pero enviamos 255.000 satoshis en su lugar. ¡Ups!

> :warning: **ADVERTENCIA:** La primera vez que escribimos esta lección, realmente calculamos mal nuestra tarifa y no lo vimos hasta que ejecutamos nuestra calculadora de tarifas. Es *así de fácil*, luego su dinero se esfuma. (El ejemplo de arriba es en realidad de nuestra segunda iteración de la calculadora, y esa vez cometimos el error a propósito).

Para más magia de JSON (y si algo de esto no está claro), por favor lee el [Manual de JSON](https://stedolan.github.io/jq/manual/) y el [JSON Cookbook](https://github.com/stedolan/jq/wiki/Cookbook). Usaremos regularmente JQ en futuros ejemplos.

## Crear nuevos alias

El código JQ puede ser un poco difícil de manejar, así que debería considerar añadir algunas invocaciones más largas e interesantes a su ~/.bash_profile.

Cada vez que busque una gran cantidad de información en un objeto JSON emitido por un comando `bitcoin-cli`, considere escribir un alias para reducirlo a lo que quiere ver.
```
alias btcunspent="bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'"
```

## Ejecutar el script comisión por transacción

El [script de cálculo de tasas](../src/04_2_i_txfee-calc.sh) está disponible en el directorio src-code. Puede descargarlo y guardarlo como `txfee-calc.sh`.

> :warning: **ADVERTENCIA:** Este script no ha sido verificado de forma robusta. Si va a utilizarlo para verificar las comisiones de las transacciones reales, sólo deberá hacerlo como triple comprobación después de haber hecho todos los cálculos usted mismo.

Asegúrese de que los permisos del script son correctos:
```
$ chmod 755 txfee-calc.sh
```
A continuación, puede ejecutar el script de la siguiente manera:
```
$ ./txfee-calc.sh $rawtxhex
.255
```
También puede crear un alias:
```
alias btctxfee="~/txfee-calc.sh"
```

## Resumen: Usando JQ

JQ facilita la extracción de información de arreglos y objetos JSON. También se puede utilizar en scripts de shell para realizar cálculos bastante complejos que le harán la vida más fácil.

## ¿Qué sigue?

Continue "Enviando Transacciones en Bitcoin" con [§4.3 Creando Transacciones en Cruda con Argumentos Ingresados con Nombre](04_3_Creando_una_Transaccion_Cruda_con_Argumentos_Ingresados_con_Nombre.md).
