# Interludio: Usando Curl

`bitcoin-cli` es, en última instancia, sólo una envoltura. Es una forma de interactuar con `bitcoind` desde la línea de comandos, proporcionando un acceso simplificado a sus muchos comandos RPC. Pero el RPC puede, por supuesto, ser accedido directamente. De eso trata este interludio: de conectar directamente con RPC con el comando `curl`.

No se usará mucho en los próximos capítulos, pero es un bloque importante que puede ver como un acceso alternativo a `bitcoind` si así lo prefiere.

## Conoce Curl

`curl`, abreviatura de "see URL", es una herramienta de línea de comandos que permite acceder directamente a las URLs de forma programática. Es una forma fácil de interactuar con servidores como `bitcoind` que escuchan puertos en Internet y que hablan una variedad de protocolos. Curl también está disponible como biblioteca para muchos lenguajes de programación, como C, Java, PHP y Python. Así que, una vez que sepa cómo trabajar con Curl, tendrá una base sólida para utilizar un montón de API diferentes.

Para usar `curl` con `bitcoind`, debe saber tres cosas: el formato estándar, el nombre de usuario y la contraseña, y el puerto correcto.

### Conocer el formato

Los comandos de `bitcoin-cli` están todos vinculados a comandos RPC en `bitcoind`. Esto hace que la transición de usar `bitcoin-cli` a usar `curl` sea muy sencilla. De hecho, si mira cualquiera de las páginas de ayuda de `bitcoin-cli`, verá que no sólo enumeran los comandos de `bitcoin-cli`, sino también los comandos paralelos de `curl`. Por ejemplo, aquí está `bitcoin-cli help getmininginfo`:
```
$ bitcoin-cli help getmininginfo
getmininginfo

Returns a json object containing mining-related information.
Result:
{                              (json object)
  "blocks" : n,                (numeric) The current block
  "currentblockweight" : n,    (numeric, optional) The block weight of the last assembled block (only present if a block was ever assembled)
  "currentblocktx" : n,        (numeric, optional) The number of block transactions of the last assembled block (only present if a block was ever assembled)
  "difficulty" : n,            (numeric) The current difficulty
  "networkhashps" : n,         (numeric) The network hashes per second
  "pooledtx" : n,              (numeric) The size of the mempool
  "chain" : "str",             (string) current network name (main, test, regtest)
  "warnings" : "str"           (string) any network and blockchain warnings
}

Examples:
> bitcoin-cli getmininginfo 
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id": "curltest", "method": "getmininginfo", "params": []}' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
Y ahí está el comando `curl`, al final de la pantalla de ayuda. Este comando algo largo tiene cuatro partes principales: (1) un listado con tu nombre de usuario; (2) una bandera `--data-binary`; (3) un objeto JSON que le dice a `bitcoind` lo que debe hacer, incluyendo una arreglo JSON de parámetros; y (4) una cabecera HTTP que incluye la URL de `bitcoind`.

Cuando trabaje con `curl`, la mayoría de estos argumentos para `curl` serán los mismos de un comando a otro; sólo las entradas `method` y `params` del array JSON cambiarán normalmente. Sin embargo, ¡Necesita saber cómo rellenar su nombre de usuario y su dirección URL para que funcione en primer lugar!

_Siempre que no esté seguro de cómo ejecutar un comando RPC, sólo tiene que mirar la ayuda de bitcoin-cli y partir de ahí._

### Conozca su Nombre de Usuario

Para hablar con el puerto `bitcoind`, necesita un nombre de usuario y una contraseña. Estos fueron creados como parte de su configuración inicial de Bitcoin, y se pueden encontrar en `~/.bitcoin/bitcoin.conf`.

Por ejemplo, esta es nuestra configuración actual:
```
$ cat ~/.bitcoin/bitcoin.conf
server=1
dbcache=1536
par=1
maxuploadtarget=137
maxconnections=16
rpcuser=StandUp
rpcpassword=8eaf562eaf45c33c3328bc66008f2dd1
rpcallowip=127.0.0.1
debug=tor
prune=550
testnet=1
mintxfee=0.001
txconfirmtarget=1
[test]
rpcbind=127.0.0.1
rpcport=18332
[main]
rpcbind=127.0.0.1
rpcport=8332
[regtest]
rpcbind=127.0.0.1
rpcport=18443
```
Nuestro nombre de usuario es `StandUp` y nuestra contraseña es `8eaf562eaf45c33c3328bc66008f2dd1`.

> **AVISO:** Claramente, no es muy seguro tener esta información en un archivo de texto plano. A partir de Bitcoin Core 0.12, puede omitir la `rpcpassword` de su archivo `bitcoin.conf`, y hacer que `bitcoind` genere una nueva cookie cada vez que se inicie. La desventaja de esto es que dificulta el uso de comandos RPC por parte de otras aplicaciones, como las que se detallan en este capítulo. Por lo tanto, vamos a quedarnos con la información simple de `rpcuser` y `rpcpassword` por ahora, pero para el software de producción, considera pasar a las cookies.

La forma segura de hacer RPC con `bitcoind` es la siguiente:
```
$ curl --user StandUp --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
Enter host password for user 'bitcoinrpc':
```
Como se ha señalado, se le pedirá la contraseña.

> 🔗 **TESTNET vs MAINNET:** Testnet utiliza una URL con el puerto 18332 y mainnet utiliza una URL con el puerto 8332. Echa un vistazo a su `bitcoin.conf`, está todo dispuesto allí.

La forma insegura de hacerlo es la siguiente:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
```
> **AVISO:** Introducir su contraseña en la línea de comandos puede poner su contraseña en la tabla de procesos y/o guardarla en un historial. Esto es aún menos recomendable que ponerla en un archivo, excepto para pruebas en testnet. Si quiere hacerlo en cualquier otro lugar, ¡Asegúrese de saber lo que está haciendo!

### Conociendo los Comandos y Parámetros

Con todo esto en la mano, está listo para enviar comandos RPC estándar con `curl` ... pero todavía necesita saber cómo incorporar los dos elementos que tienden a cambiar en el comando `curl`.

El primero es `method`, que es el método RPC que se utiliza. Por lo general, debería coincidir con los nombres de los comandos que has estado introduciendo en `bitcoin-cli` durante años.

El segundo es `params`, que es una arreglo JSON de parámetros. Estos son los mismos que los argumentos (o argumentos con nombre) que ha estado utilizando. También son la parte más confusa de `curl`, en gran parte porque son una arreglo estructurada en lugar de una simple lista.

Así es como se ven algunos arreglos de parámetros:

  * `[]` — Un arreglo vacío
  * `["000b4430a7a2ba60891b01b718747eaf9665cb93fbc0c619c99419b5b5cf3ad2"]` — Un arreglo con datos
  * `["'$signedhex'"]` — Un arreglo con una varible
  * `[6, 9999999]` — Un arreglo con parametros
  * `{}` - Un objeto vacío
  * `[''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.298, "'$changeaddress'": 1.0}'']` — Un arreglo con una arreglo que contiene un objeto y un objeto vacío

## Obtener información

Ahora puede enviar su primer comando `curl` accediendo al RPC `getmininginfo`:

```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
{"result":{"blocks":1772428,"difficulty":10178811.40698772,"networkhashps":91963587385939.06,"pooledtx":61,"chain":"test","warnings":"Warning: unknown new rules activated (versionbit 28)"},"error":null,"id":"curltest"}
```
Tenga en cuenta que proporcionamos el método, `getmininginfo`, y el parámetro,`[]`, pero que todo lo demás era la línea de comando estándar` curl`.

> **AVISO:** Si obtiene un resultado como "Failed to connect to 127.0.0.1 port 8332: Connection refused", asegúrese de que una línea como `rpcallowip=127.0.0.1` está en su ~/.bitcoin/bitcoin.conf. Si las cosas siguen sin funcionar, asegúrese de que está permitiendo el acceso al puerto 18332 (o 8332) desde localhost. Nuestra configuración estándar del [2.0: Configuración de un Servidor Privado Virtual de Bitcoin-Core](02_0_Configurando_un_Bitcoin-Core_VPS.md) debería hacer todo esto.

El resultado es otro arreglo JSON, que desafortunadamente es feo de leer si está usando `curl` a mano. Afortunadamente, puede limpiarlo simplemente pasándolo por `jq`:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   295  100   218  100    77  72666  25666 --:--:-- --:--:-- --:--:-- 98333
{
  "result": {
    "blocks": 1772429,
    "difficulty": 10178811.40698772,
    "networkhashps": 90580030969896.44,
    "pooledtx": 4,
    "chain": "test",
    "warnings": "Warning: unknown new rules activated (versionbit 28)"
  },
  "error": null,
  "id": "curltest"
}
```
Verá un poco de información de conectividad a medida que se descargan los datos, luego cuando esos datos lleguen a `jq`, todo saldrá en una forma correctamente indentada. (Omitiremos la información de descarga en futuros ejemplos).

## Manipular su monedero

Aunque esté accediendo directamente a `bitcoind`, seguirá teniendo acceso a la funcionalidad del monedero, ya que ésta se almacena en gran parte en el propio `bitcoind`.

### Buscar direcciones

Usa el RPC `getaddressesbylabel` para listar todas sus direcciones actuales:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getaddressesbylabel", "params": [""] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": {
    "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE": {
      "purpose": "receive"
    },
    "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff": {
      "purpose": "receive"
    },
    "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B": {
      "purpose": "receive"
    },
    "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d": {
      "purpose": "receive"
    },
    "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6": {
      "purpose": "receive"
    },
    "tb1qmtucvjtga68kgrvkl7q05x4t9lylxhku7kqdpr": {
      "purpose": "receive"
    }
  },
  "error": null,
  "id": "curltest"
}
```
Este es nuestro primer ejemplo de un parámetro real, `""`. Este es el parámetro requerido `label` para `getaddressesbylabel`, pero todas nuestras direcciones están bajo la etiqueta por defecto, así que no se requiere nada especial aquí.

El resultado es una lista de todas las direcciones que han sido utilizadas por este monedero... algunas de las cuales presumiblemente contienen fondos.

### Buscar fondos

Utilice el RPC `listunspent` para listar los fondos que tiene disponibles:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": [
    {
      "txid": "e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d",
      "vout": 1,
      "address": "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
      "scriptPubKey": "76a91432db726320e4ad170c9c1ee83cd4d8a243c3435988ac",
      "amount": 0.0009,
      "confirmations": 4,
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
      "amount": 0.00022,
      "confirmations": 19,
      "spendable": true,
      "solvable": true,
      "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
      "safe": true
    }
  ],
  "error": null,
  "id": "curltest"
}
```
Esta es casi la misma salida que recibe cuando escribe `bitcoin-cli listunspent`, lo que demuestra lo estrechamente ligadas que están las dos interfaces. Si no se necesita ninguna limpieza o ayuda extra, entonces `bitcoin-cli` sólo muestra la RPC. ¡Fácil!

### Crear una dirección

Después de saber dónde están sus fondos, el siguiente paso en la elaboración de una transacción es conseguir una dirección de cambio. A estas alturas, probablemente ya se haya hecho a la idea, y sabe que para los comandos RPC simples, todo lo que necesita hacer es ajustar el `method` es el comando `curl`:

```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["", "legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "mrSqN37TPs89GcidSZTvXmMzjxoJZ6RKoz",
  "error": null,
  "id": "curltest"
}
```

En este punto, podemos incluso volver a nuestra práctica estándar de guardar los resultados en variables con la ayuda adicional de `jq`:
```
$ changeaddress=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["", "legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')
$ echo $changeaddress
mqdfnjgWr2r3sCCeuTDfe8fJ1CnycF2e6R
```
No hay que preocuparse por la información de la descarga. Irá a `STDERR` y se mostrará en su pantalla, mientras que los resultados van a `STDOUT` y se guardan en su variable.

## Crear una Transacción

Ahora está listo para crear una transacción con `curl`.

### Prepare sus variables

Al igual que con `bitcoin-cli`, para crear una transacción curlando comandos RPC, primero debe guardar sus variables. El único cambio aquí es que `curl` crea un objeto JSON que incluye un valor-clave `resultado`, por lo que siempre hay que pasar la etiqueta `.resultado` antes de hacer cualquier otra cosa.

Este ejemplo configura nuestras variables para utilzar los 1.2985 BTC en fondos que aparecen en la primera transacción no gastada anterior:
```
$ utxo_txid=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .[0] | .txid')
$ utxo_vout=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .[0] | .vout')
$ recipient=mwCwTceJvYV27KXBc3NJZys6CjsgsoeHmf
$ changeaddress=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')

$ echo $utxo_txid
e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d
$ echo $utxo_vout
1
$ echo $recipient
mwCwTceJvYV27KXBc3NJZys6CjsgsoeHmf
$ echo $changeaddress
n2jf3MzeFpFGa7wq8rXKVnVuv5FoNSJZ1N
```

### Crear la transacción

La transacción creada con `curl` es muy similar a la transacción creada con `bitcoin-cli`, pero con algunas sutiles diferencias:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.0003, "'$changeaddress'": 0.0005}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "02000000010d6acd0356db222ca3a7ee7fa1e3044316223ceec1f64b58aeb2e0de921007e70100000000ffffffff0230750000000000001976a914ac19d3fd17710e6b9a331022fe92c693fdf6659588ac50c30000000000001976a9147021efec134057043386decfaa6a6aa4ee5f19eb88ac00000000",
  "error": null,
  "id": "curltest"
}
```
El corazón de la transacción es, por supuesto, el arreglo JSON `params`, que estamos poniendo en uso por primera vez.

Observa que todo el `params` está alojado en `[]`s para marcar el arreglo de parámetros.

También hemos variado las citas de cómo funcionaban las cosas en `bitcoin-cli`, para empezar y terminar cada arreglo y objeto dentro del arreglo `params` con `''` en lugar de nuestro tradicional `'''`. Esto se debe a que todo el conjunto de argumentos JSON ya tiene un `'` alrededor. Como de costumbre, sólo hay que echar un vistazo a las extrañas citas del shell y acostumbrarse a ellas.

Sin embargo, hay una última cosa a tener en cuenta en este ejemplo, y puede ser _enfadante_ si se lo pierde. Cuando ejecutaba un comando `createrawtransaction` con `bitcoin-cli` el arreglo JSON de entradas y el objeto JSON de salidas eran cada uno parámetros distintos, por lo que estaban separados por un espacio. Ahora, como son parte de ese arreglo JSON `params`, están separados por una coma (`,`). Si no lo hace, obtendrá un "error de análisis" sin mucha información adicional.

> **AVISO:** ¿Alguna vez ha tenido problemas para depurar su `curl`? Añade el argumento `--trace-ascii /tmp/foo`. La información completa de lo que se envía al servidor se guardará en `/tmp/foo` (o cualquier nombre de archivo que proporcione).

Habiendo verificado que las cosas funcionan, probablemente quiera guardar el código hexadecimal en una variable:
```
$ hexcode=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.0003, "'$changeaddress'": 0.0005}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')
```

### Firmar y enviar

Firmar y enviar su transacción usando `curl` es un uso sencillo del RPC `signrawtransactionwithwallet` y `sendrawtransaction`:

```
$ signedhex=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "signrawtransactionwithwallet", "params": ["'$hexcode'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .hex')

$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "sendrawtransaction", "params": ["'$signedhex'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "eb84c5008038d760805d4d9644ace67849542864220cb2685a1ea2c64176b82d",
  "error": null,
  "id": "curltest"
}
```
## Resumen: Accediendo a Bitcoind con Curl

Una vez terminada esta sección, puede que sienta que acceder a `bitcoind` a través de `curl` es muy parecido a acceder a través de `bitcoin-cli` ... pero más engorroso. Y, tendría razón. `bitcoin-cli` tiene una funcionalidad RPC bastante completa, así que cualquier cosa que haga a través de `curl` probablemente pueda hacerla a través de `bitcoin-cli`. Por eso vamos a seguir concentrándonos en `bitcoin-cli` después de este paréntesis.

Pero todavía hay razones para usar `curl` en lugar de `bitcoin-cli`:

Lo más obvio es que `curl` elimina un nivel de indirección. En lugar de trabajar con `bitcoin-cli` que envía comandos RPC a `bitcoind`, está enviando esos comandos RPC directamente. Esto permite una programación más robusta, porque no tiene que preocuparse de las cosas inesperadas que pueda hacer `bitcoin-cli` o de cómo pueda cambiar con el tiempo. Sin embargo, también usted está dando sus primeros pasos hacia el uso de un lenguaje de programación más completo que las pobres opciones que ofrece un script de shell. Como verá en los últimos capítulos de esto, podría ver que las librerías curl son otras funciones para acceder a los comandos RPC en una variedad de lenguajes de programación: pero eso está todavía muy lejos.

## ¿Qué sigue?

Conozca una forma más de "Enviar Transacciones Bitcoin" con [§4.5: Enviando Monedas con Transacciones Crudas Automatizadas](04_5_Enviando_Monedas_con_Transacciones_Crudas_Automatizadas.md).
