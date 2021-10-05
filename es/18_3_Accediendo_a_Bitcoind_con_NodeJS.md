# 18.3: Acceso a Bitcoind con NodeJS

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias

Esta sección explica cómo interactuar con `bitcoind` usando el lenguaje de programación NodeJS y el [paquete BCRPC](https://github.com/dgarage/bcrpc).

## Configurar node.js

BCRPC está construido en node.js. Por lo tanto, primero necesitará instalar los paquetes `node.js` y `npm` (el administrador de paquetes de nodos) para su sistema. 

Si está usando una máquina de Ubuntu, puede ejecutar los siguientes comandos para obtener una nueva versión de `node.js` (a diferencia de la horriblemente desactualizada versión en el sistema de paquetes de Ubuntu).

```
$ curl -sL https://deb.nodesource.com/setup_14.x | sudo bash -
$ sudo apt-get install -y nodejs
$ sudo npm install mocha -g

```
### Configuración de BCRPC

Ahora puede clonar el paquete BCRPC desde GitHub e instalar sus dependencias.
```
$ git clone https://github.com/dgarage/bcrpc.git
$ cd bcrpc
$ npm install
```

Para probar el paquete BCRPC, primero debe establecer variables ambientales para rpcuser y rpcpassword. Como de costumbre, estas provienen de `~/. bitcoin/bitcoin.conf`. También debe establecer el puerto RPC en 18332 que debería ser correcto para la configuración estándar de testnet descrita en estos documentos.

```
$ export BITCOIND_USER=StandUp
$ export BITCOIND_PASS=d8340efbcd34e312044c8431c59c792c
$ export BITCOIND_PORT=18332
```
> :warning: **ADVERTENCIA:** Obviamente, nunca debería poner su contraseña en una variable ambiental en un entorno de producción.

> :link: **MAINNET VS TESTNET:** El puerto sería 8332 para una configuración de mainnet.

Ahora puede verificar que todo funciona correctamente:
```
$ npm test

> bcrpc@0.2.2 test /home/user1/bcrpc
> mocha tests.js

  BitcoinD
    ✓ is running

  bcrpc
    ✓ can get info

  2 passing (36ms)
```
Felicitaciones, ahora tiene un envoltorio RPC listo para Bitcoin para Node.js que está trabajando con su configuración de Bitcoin.

### Crear un proyecto BCRPC

Ahora puede crear un nuevo proyecto Node.js e instalar BCRPC a través de npm.
```
$ cd ~
$ mkdir myproject
$ cd myproject
$ npm init
  [continue with default options]
$ npm install bcrpc
```
# Construya su conexión

En su directorio `myproject` cree un archivo `.js` donde se ejecutará el código JavaScript. 

Puede iniciar una conexión RPC creando un `RpcAgent`:
```
const RpcAgent = require('bcrpc');
agent = new RpcAgent({port: 18332, user: 'StandUp', pass: 'd8340efbcd34e312044c8431c59c792c'});
```
Obviamente, su usuario `user` y `pass` deberían coincidir de nuevo con lo que está en su `~/. bitcoin/bitcoin.conf`, y usar `port 18332` si está en testnet.

### Hacer una llamada RPC

Usando BCRPC, puede usar los mismos comandos RPC que usaría normalmente a través de `bitcoin-cli` con su `RpcAgent`, excepto que necesitan estar en camelCase. Por ejemplo, `getblockhash` sería `getBlockHash`.

Para imprimir el nuevo número de bloque, sólo tiene que llamar a `getBlockCount` a través de su `RpcAgent`:
```
  agent.getBlockHash(blockCount.result, function (err, hash) {
    if (err)
      throw Error(JSON.stringify(err));
    console.log(hash.result);
  })
```
El resultado de las funciones BCRPC es un objeto JSON que contiene información sobre cualquier error y el id de la solicitud. Al acceder a su resultado, se añade `.result` al final de la misma para especificar que usted está interesado en el resultado real, no información sobre errores. 

### Ejecute su código

Puede encontrar el código `getinfo` en el [directorio src](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_3_getinfo.js).
```
$ node getinfo.js
1831094
00000000000002bf8b522a830180ad3a93b8eed33121f54b3842d8838580a53c
```
Esto es lo que la salida del ejemplo anterior se vería si se reemplaza `console.log(blockCount.result);` `console.log(hash.result);` `console.log(blockCount);` y `console.log(hash);`, respectivamente:
```
{ result: 1774686, error: null, id: null }
{
  result: '00000000000000d980c495a2b7addf09bb0a9c78b5b199c8e965ee54753fa5da',
  error: null,
  id: null
}
```
## Buscar fondos

Es útil al aceptar Bitcoin poder comprobar el bitcoin recibido en una dirección específica en su cartera. Por ejemplo, si estuviera dirigiendo una tienda online aceptando Bitcoin, por cada pago de un cliente, generaría una nueva dirección, mostraría esa dirección al cliente, y luego comprobaría el saldo de la dirección después de algún tiempo, para asegurarse de que se ha recibido la cantidad correcta:
```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```
> :information_source: **NOTA:** Obviamente, necesitará introducir una dirección reconocida por su máquina.

Por defecto, esta función comprueba las transacciones que se han confirmado una vez, sin embargo, puede aumentar este a un número más alto, como 6:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', 6, function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

### Buscar información de la cartera

También puede buscar información adicional sobre su cartera y ver su saldo, conteo de transacciones, etc:
```
agent.getWalletInfo(function (err, walletInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(walletInfo.result);
});
```
La fuente está disponible como [walletinfo.js](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_3_walletinfo.js).
```
$ node walletinfo.js
0.008498
{
  walletname: '',
  walletversion: 169900,
  balance: 0.010438,
  unconfirmed_balance: 0,
  immature_balance: 0,
  txcount: 4,
  keypoololdest: 1596567843,
  keypoolsize: 999,
  hdseedid: 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
  keypoolsize_hd_internal: 1000,
  paytxfee: 0,
  private_keys_enabled: true,
  avoid_reuse: false,
  scanning: false
}
```
En lugar de imprimir todos los detalles asociados con su cartera, puede imprimir información específica, como su saldo. Como se está accediendo a un objeto JSON, esto se puede hacer cambiando la línea `console.log(walletInfo.result);`` a `console.log(walletInfo.result.balance);`:

## Crear una dirección

También puede pasar argumentos adicionales a los comandos RPC. Por ejemplo, lo siguiente genera una nueva dirección heredada, con la marca `-addresstype`.

```
agent.getNewAddress('-addresstype', 'legacy', function (err, newAddress) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(newAddress.result);
});
```
Esto es lo mismo que ejecutar lo siguiente desde la línea de comandos:
```
$ bitcoin-cli getnewaddress -addresstype legacy
mtGPcBvRPZFEHo2YX8un9qqPBydhG82uuZ
```
En BCRPC, generalmente puede utilizar los mismos parámetros que en `bitcoin-cli` en BCRPC. Aunque usa camelCase (`getNewAddress`) para los métodos, las banderas, que normalmente están separadas por espacios en la línea de comandos, se colocan en cadenas y separadas por comas.

## Enviar una transacción

Puede enviar monedas a una dirección más fácilmente usando la función `sendToAddress` :
```
agent.sendToAddress(newAddress.result, 0.00001, function(err, txid) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(txid.result);
});
```
Esto debe imprimir el txid de la transacción:
```
1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9
```

### Buscar una transacción

Es posible que ahora desee ver una transacción, como la que acaba de enviar.
```
agent.getTransaction(txid.result, function (err, transaction) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(transaction.result);
});
```
Debería obtener una salida similar a esta:
```
{
  amount: 0.001,
  confirmations: 4776,
  blockhash: '000000006628870b0a8a66abea9cf0d4e815c491f079e3fa9e658a87b5dc863a',
  blockindex: 117,
  blocktime: 1591857418,
  txid: '1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df',
  walletconflicts: [],
  time: 1591857343,
  timereceived: 1591857343,
  'bip125-replaceable': 'no',
  details: [
    {
      address: 'mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha',
      category: 'receive',
      amount: 0.001,
      label: '',
      vout: 0
    }
  ],
  hex: '02000000000101e9e8c3bd057d54e73baadc60c166860163b0e7aa60cab33a03e89fb44321f8d5010000001716001435c2aa3fc09ea53c3e23925c5b2e93b9119b2568feffffff02a0860100000000001976a914600c8c6a4abb0a502ea4de01681fe4fa1ca7800688ac65ec1c000000000017a91425b920efb2fde1a0277d3df11d0fd7249e17cf8587024730440220403a863d312946aae3f3ef0a57206197bc67f71536fb5f4b9ca71a7e226b6dc50220329646cf786cfef79d60de3ef54f702ab1073694022f0618731902d926918c3e012103e6feac9d7a8ad1ac6b36fb4c91c1c9f7fff1e7f63f0340e5253a0e4478b7b13f41fd1a00'
}
```
El código completo está disponible como [sendtx.js](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_3_sendtx.js).

## Resumen: Acceso a Bitcoind con node.js

Con BCRPC puede acceder a todos los comandos RPC disponibles a través de `bitcoin-cli`, en JavaScript. El archivo [README de BCRPC](https://github.com/dgarage/bcrpc) tiene algunos ejemplos que usan promesas (los ejemplos en este documento usan callbacks). El [JavaScript detrás de él](https://github.com/dgarage/bcrpc/blob/master/index.js) es corto y legible. 

En base a estos ejemplos debería ser capaz de incorporar Bitcoin en un proyecto Node.js y hacer cosas como enviar y recibir monedas.

## ¿Qué es lo siguiente?

Obtenga más información sobre "Cómo hablar con Bitcoin en otros lenguajes" en [18.4: Cómo acceder a Bitcoin con Python](18_4_Accediendo_a_Bitcoind_con_Python.md).
