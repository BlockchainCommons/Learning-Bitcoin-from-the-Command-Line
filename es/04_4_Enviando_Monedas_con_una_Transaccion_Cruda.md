# 4.4: Enviando Monedas con una Transacción Cruda

Como se indicó al principio de este capítulo, la interfaz `bitcoin-cli` ofrece tres formas principales de enviar monedas. En [§4.1](04_1_Enviando_Monedas_de_la_Forma_Facil.md) hablamos de enviarlas de la primera manera, usando el comando `sendtoaddress`. Desde entonces, hemos estado construyendo detalles sobre cómo enviar monedas de una segunda manera, con transacciones en crudo. El [§4.2](04_2_Creando_una_Transaccion_Cruda.md) enseñó cómo crear una transacción sin procesar, un [Interlude](04_2_Interludio_Usando_JQ.md) explicó JQ, y el [§4.3](04_3_Creando_una_Transaccion_Cruda_con_Argumentos_Ingresados_con_Nombre.md) demostró los argumentos con nombre.

Ahora podemos juntar todo esto y enviar fondos usando una transacción en crudo.

## Crear una dirección de cambio

Nuestro ejemplo de transacción cruda en la sección §4.2 era muy simplista: enviamos la totalidad de un UTXO a una nueva dirección. Más frecuentemente, querrá enviar a alguien una cantidad de dinero que no coincide con un UTXO. Pero, recordará que el exceso de dinero de un UTXO que no se envía a tu destinatario se convierte en una comisión de transacción. Entonces, ¿Cómo se puede enviar a alguien sólo una parte de un UTXO y quedarse con el resto?

La solución es _enviar_ el resto de los fondos a una segunda dirección, una dirección de cambio que has creado en su cartera específicamente para recibirlos:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress legacy)
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
```
Tenga en cuenta que esto utiliza una nueva función: `getrawchangeaddress`. Es en gran medida la misma que `getnewaddress` pero está optimizada para su uso como una dirección de cambio en una transacción cruda, por lo que no hace cosas como hacer entradas en su libreta de direcciones. De nuevo seleccionamos la dirección `legacy`, en lugar de ir con el valor por defecto de `bech32`, simplemente por consistencia. Esta es una situación en la que habría sido totalmente seguro generar una dirección Bech32 por defecto, simplemente usando `bitcoin-cli getrawchangeaddress`, porque sería enviada y recibida por usted en su nodo Bitcoin Core que lo soporta completamente. Pero, hobgoblins; cambiaremos esto a Bech32 también en [§4.6](04_6_Creando_una_Transaccion_Segwit.md).

Ahora tiene una dirección adicional dentro de tu monedero, ¡Para que pueda recibir cambio de un UTXO! Para usarlo, tendrá que crear una transacción en crudo con dos salidas.

## Escoger suficientes UTXOs

Nuestro ejemplo de transacción en crudo era simple en otro sentido: asumía que había suficiente dinero en un solo UTXO para cubrir la transacción. A menudo este será el caso, pero a veces querrá crear transacciones que gasten más dinero del que tiene en un solo UTXO. Para ello, debe crear una transacción cruda con dos (o más) entradas.

## Escribir una transacción en crudo real

Para resumir: crear una transacción real en crudo para enviar monedas a veces requerirá múltiples entradas y casi siempre requerirá múltiples salidas, una de las cuales es una dirección de cambio. Vamos a crear ese tipo de transacción más realista aquí, en un nuevo ejemplo que muestra un ejemplo real de envío de fondos a través de la segunda metodología de Bitcoin, las transacciones en bruto.

Vamos a utilizar nuestros UTXOs 0 y 2:
```
$ bitcoin-cli listunspent
[
[
  {
    "txid": "0619fecf6b2668fab1308fbd7b291ac210932602a6ac6b8cc11c7ae22c43701e",
    "vout": 1,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00899999,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },  
  {
    "txid": "0df23a9dba49e822bbc558f15516f33021a64a5c2e48962cec541e0bcc79854d",
    "vout": 0,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00100000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
   }
]

```
En nuestro ejemplo, vamos a enviar 0.009 BTC, que es (apenas) mayor que cualquiera de nuestros UTXOs. Esto requiere combinarlos, y luego usar nuestra dirección de cambio para recuperar los fondos no gastados.

### Configurar las variables

Ya tenemos las variables `$changeaddress` y `$recipient` de los ejemplos anteriores:
```
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
También necesitamos registrar el txid y el vout para cada uno de nuestros dos UTXOs. Una vez identificados los UTXOs que queremos gastar, podemos utilizar nuestras técnicas JQ para asegurarnos de que el acceso a ellos está libre de errores:
```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
```

### Escribir la transacción

Escribir la transacción en crudo es sorprendentemente sencillo. Todo lo que tiene que hacer es incluir un objeto JSON adicional, separado por comas, en la arreglo JSON de entradas y un par clave-valor adicional, separado por comas, en el objeto JSON de salidas.

Este es el ejemplo. Fíjese en las múltiples entradas después del argumento `inputs` y en las múltiples salidas después del argumento `outputs`.
```
$ rawtxhex2=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.009, "'$changeaddress'": 0.0009 }''')
```
Fuimos _muy_ cuidadosos al calcular nuestro dinero. Estos dos UTXOs contienen 0.00999999 BTC. Después de enviar 0.009 BTC, nos quedarán .00099999 BTC. Elegimos .00009999 BTC como comisión de la transacción. Para acomodar esa tarifa, fijamos nuestro cambio en .0009 BTC. Si nos hubiéramos equivocado en las matemáticas y hubiéramos puesto el cambio a 0.00009 BTC, ¡Ese BTC adicional se perdería para los mineros! Si nos hubiéramos olvidado de hacer el cambio, todo el exceso habría desaparecido. Así que, de nuevo, _tenga cuidado_. 

Afortunadamente, podemos hacer una triple comprobación con el alias `btctxfee` del Interludio JQ:
```
$ ./txfee-calc.sh $rawtxhex2
.00009999
```

### Finalizarlo

Ya puede firmar, sellar y entregar su transacción, y es suya (y del grifo):
```
$ signedtx2=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex2 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx2
e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d
```

### Espera

Como es habitual, su dinero estará en el aire durante un tiempo: el cambio no estará disponible hasta que se confirme la transacción y se entregue un nuevo UTXO.

Pero, en 10 minutos o menos (probablemente), tendrá su dinero restante de vuelta y totalmente gastable de nuevo. Por ahora, seguimos esperando:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```
Y el cambio llegará:
```
[
  {
    "txid": "e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d",
    "vout": 1,
    "address": "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
    "scriptPubKey": "76a91432db726320e4ad170c9c1ee83cd4d8a243c3435988ac",
    "amount": 0.00090000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/1'/2']02881697d252d8bf181d08c58de1f02aec088cd2d468fc5fd888c6e39909f7fabf)#p6k7dptk",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 16,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```

Este también podría ser un buen momento para volver a ver un explorador de blockchain, para que pueda ver de forma más intuitiva cómo están dispuestas las entradas, salidas y la tasa de transacción: [e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d](https://live.blockcypher.com/btc-testnet/tx/e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d/).

## Resumen: Envío de monedas con transacciones crudas

Para enviar monedas con transacciones en crudo, necesita crear una transacción en crudo con una o más entradas (para tener fondos suficientes) y una o más salidas (para recuperar el cambio). Entonces, puede seguir el procedimiento normal de usar `createrawtransaction` con argumentos nombrados y JQ, como se ha expuesto en las secciones anteriores.

> 🔥 ___¿Cuál es el poder de enviar monedas con transacciones en crudo?___

> _Las ventajas._ Le otorga el mejor control. Si su objetivo es escribir un script o programa de Bitcoin más complejo, probablemente utilizará transacciones en crudo para saber exactamente lo que está pasando. Esta es también la situación _más segura_ para usar transacciones en crudo, porque puede asegurarse programáticamente de no cometer errores.

> _Las desventajas._ Es fácil perder dinero. No hay advertencias, no hay salvaguardias, y no hay respaldos programáticos a menos que usted los escriba. También es arcaico. El formato es odioso, incluso usando la interfaz `bitcoin-cli`, que es muy fácil de usar, y tiene que hacer muchas búsquedas y cálculos a mano.

## ¿Qué sigue?

Vea otra forma alternativa de introducir comandos con [Interludio: Usando Curl](04_4_Interludio_Usando_Curl.md).

O, si prefiere saltarse lo que es francamente una paréntesis, aprende una forma más de "Enviar Transacciones Bitcoin" con [§4.5: Enviando Monedas con Transacciones Crudas Automatizadas](04_5_Enviando_Monedas_con_Transacciones_Crudas_Automatizadas.md).

