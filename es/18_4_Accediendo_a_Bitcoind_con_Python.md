# 18.4: Acceso a Bitcoind con Python

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias

Esta sección explica cómo interactuar con `bitcoind` usando el lenguaje de programación Python y el paquete [Python-BitcoinRPC](https://github.com/jgarzik/python-bitcoinrpc).

## Configuración de Python

Si usted ya tiene Bitcoin Core instalado, debería tener Python 3 disponible también.
Puede comprobar esto corriendo:

`$ python3 --version`

Si devuelve un número de versión (por ejemplo, `3.7.3` o `3.8.3`) entonces tiene python3 instalado.

Sin embargo, si de alguna manera no tiene Python instalado, necesitará construirlo desde el código fuente como sigue. Consulte la variante ["Compilando python desde la fuente"](18_4_Accediendo_a_Bitcoind_con_Python.md#variant-build-python-from-source) antes de continuar.

### Configurar BitcoinRPC

Ya sea que haya usado un Python existente o lo haya compilado desde el código fuente, ahora usted esta listo para instalar la biblioteca `python-bitcoinrpc` :
```
$ pip3 install python-bitcoinrpc
```
Si no tiene `pip` instalado, necesitará ejecutar lo siguiente:
```
$ sudo apt install python3-pip
```
(A continuación, repita las instrucciones `pip3 install python-bitcoinrpc` .)

### Crear un proyecto BitcoinRPC

Generalmente necesitara incluir declaraciones apropiadas de `bitcoinrpc` en sus proyectos de Bitcoin en Python. Lo siguiente le dará acceso a los comandos basados en RPC:

```py
from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
```
También puede encontrar útil lo siguiente:

```py
from pprint import pprint
import logging
```
`pprint` imprimirá amigablemente la respuesta `json` de `bitcoind`.

`logging` imprimirá la llamada que haga a `bitcoind` y la respuesta de `bitcoind`, lo cual es útil cuando hace un montón de llamadas juntas. Si no quiere que la salida excesiva se muestre en el terminal sólo debe comentar el bloque `logging` .

## Construya su conexión

Ahora está listo para empezar a interactuar con `bitcoind` estableciendo una conexión. Cree un archivo llamado `btcrpc.py` y escriba lo siguiente:

```py
logging.basicConfig()
logging.getLogger("BitcoinRPC").setLevel(logging.DEBUG)
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = "StandUp"
rpc_pass = "6305f1b2dbb3bc5a16cd0f4aac7e1eba"
rpc_host = "127.0.0.1"
rpc_client = AuthServiceProxy(f"http://{rpc_user}:{rpc_pass}@{rpc_host}:18332", timeout=120)
```

Los argumentos en la URL son `rpc_username>:rpc_password>@host_IP_address>:port>`. Como de costumbre, el usuario `user` y `pass` se encuentran en su `~/. bitcoin/bitcoin.conf`, mientras que el `host` es su maquina local, y el puerto es `18332` para testnet. El argumento `timeout` se especifica desde _sockets timeout_ bajo carga pesada en la red principal. Si obtiene `socket.timeout: timed out` response, sea paciente y aumente el `timeout`.

> :link: **MAINNET VS TESTNET:** El puerto sería 8332 para una configuración de mainnet.

### Hacer una llamada RPC

Si `rpc_client` se inicializa correctamente, podrá enviar comandos RPC a su nodo bitcoin.

Para usar un método RPC desde `python-bitcoinrpc`, usará el objeto `rpc_client` que creó, que proporciona la mayor parte de la funcionalidad a la que se puede acceder a través de `bitcoin-cli`, usando los mismos nombres de método.

Por ejemplo, lo siguiente recuperará el conteo de bloques de su nodo:

```py
block_count = rpc_client.getblockcount()
print("---------------------------------------------------------------")
print("Block Count:", block_count)
print("---------------------------------------------------------------\n")
```
Debería ver la siguiente salida con `logging` activado:
```sh
DEBUG:BitcoinRPC:-3-> getblockcount []
DEBUG:BitcoinRPC:<-3- 1773020
---------------------------------------------------------------
Block Count: 1773020
---------------------------------------------------------------
```
### Hacer una llamada RPC con argumentos

Puede usar ese conteo de bloques como argumento para recuperar el blockhash de un bloque y también para recuperar detalles de ese bloque.

Esto se hace enviando los comandos objeto `rpc_client` con un argumento:
```py
blockhash = rpc_client.getblockhash(block_count)
block = rpc_client.getblock(blockhash)
```
El `getblockhash` devolverá un solo valor, mientras que el `getblock` devolverá un array asociativo de información sobre el bloque, que incluye un array bajo `block['tx']` proporcionando detalles sobre cada transacción dentro del bloque:
```py
nTx = block['nTx']
if nTx > 10:
    it_txs = 10
    list_tx_heading = "First 10 transactions: "
else:
    it_txs = nTx
    list_tx_heading = f"All the {it_txs} transactions: "
print("---------------------------------------------------------------")
print("BLOCK: ", block_count)
print("-------------")
print("Block Hash...: ", blockhash)
print("Merkle Root..: ", block['merkleroot'])
print("Block Size...: ", block['size'])
print("Block Weight.: ", block['weight'])
print("Nonce........: ", block['nonce'])
print("Difficulty...: ", block['difficulty'])
print("Number of Tx.: ", nTx)
print(list_tx_heading)
print("---------------------")
i = 0
while i < it_txs:
    print(i, ":", block['tx'][i])
    i += 1
print("---------------------------------------------------------------\n")
```
### Ejecute su código

Puede recuperar [el código fuente en](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_4_getinfo.py) y ejecutarlo con `python3`:
```
$ python3 getinfo.py
---------------------------------------------------------------
Block Count: 1831106
---------------------------------------------------------------
---------------------------------------------------------------
BLOCK:  1831106
-------------
Block Hash...:  00000000000003b2ea7c2cdfffd86156ad1f5606ab58e128940a2534d1348b04
Merkle Root..:  056a547fe59208167eef86fa694263728fb684119254b340c1f86bdd423a8082
Block Size...:  52079
Block Weight.:  128594
Nonce........:  1775583700
Difficulty...:  4194304
Number of Tx.:  155
First 10 transactions: 
---------------------
0 : d228d55112e3aa26265b0118cfdc98345c229d20fe074b9afb87107c03ce11b5
1 : 92822e8e34fafb472b87c99ea3f3e16440452b3f361ed86c6fa62175173fb750
2 : fa7c67600c14d4aa350a9674688f1429577954f4a6c5e4639d06c8964824f647
3 : 3a91d1527e308e5603dafde7ab17824f441a73a779d2571d073466dc9e8451b2
4 : 30fd0e5527b1522e7b26a4818b9edac80fe47c0c39fc34705478a49e684708d0
5 : 24c5372b38c78cbaf5b0b305925502a491bc0c1b5758f50c0bd335abb6ae85f5
6 : be70e125a5793efc5e32051fecba0668df971bdf371138c8261201c2a46b2d38
7 : 41ebf52c847a59ba0aeb4425c74e89a01e91defa86a82785ff53ed4668054561
8 : dc8211b4ce122f87692e7c203672e3eb1ffc44c0a307eafcc560323fcc5fae78
9 : 59e2d8e11cad287eacf3207e64a373f65059286b803ef0981510193ae29cbc8c
---------------------------------------------------------------
```
## Buscar fondos

Del mismo modo, puede recuperar la información de su cartera con el `getwalletinfo` RPC:
```py
wallet_info = rpc_client.getwalletinfo()
print("---------------------------------------------------------------")
print("Wallet Info:")
print("-----------")
pprint(wallet_info)
print("---------------------------------------------------------------\n")
```
Debería tener una salida similar a la siguiente con `logging` disabled:
```sh
---------------------------------------------------------------
Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.07160443'),
 'hdseedid': '6dko666b1cc0d69b7eb0539l89eba7b6390kdj02',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1542245729,
 'keypoolsize': 999,
 'keypoolsize_hd_internal': 1000,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 9,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------
```
Otros comandos informativos como `getblockchaininfo`, `getnetworkinfo`, `getpeerinfo`, y `getblockchaininfo` funcionarán de manera similar.

Otros comandos pueden darle información específica sobre determinados elementos dentro de su cartera.

### Recuperar una matriz

La llamada RPC `listtransactions` le permite ver las 10 transacciones más recientes en su sistema (o algún conjunto arbitrario de transacciones usando los argumentos `count` y `skip` ). Muestra cómo un comando RPC puede devolver una matriz fácil de manipular:
```py
tx_list = rpc_client.listtransactions()
pprint(tx_list)
```
### Explorar un UTXO

Del mismo modo, puede utilizar `listunspent` para obtener una matriz de UTXOs en su sistema:
```py
print("Exploring UTXOs")
## List UTXOs
utxos = rpc_client.listunspent()
print("Utxos: ")
print("-----")
pprint(utxos)
print("------------------------------------------\n")
```
Con el fin de manipular un array como el devuelto desde `listtransactions`o `listunpsent`, sólo tiene que tomar el elemento apropiado del elemento apropiado del array:
```
## Select a UTXO - first one selected here
utxo_txid = utxos[0]['txid']
```
Para `listunspent`, se obtiene un `txid`. Puede recuperar información sobre él con `gettransaction`, luego decodificar eso con `decoderawtransaction`:
```
utxo_hex = rpc_client.gettransaction(utxo_txid)['hex']

utxo_tx_details = rpc_client.decoderawtransaction(utxo_hex)

print("Details of Utxo with txid:", utxo_txid)
print("---------------------------------------------------------------")
print("UTXO Details:")
print("------------")
pprint(utxo_tx_details)
print("---------------------------------------------------------------\n")
```
Este código está disponible en [walletinfo.py](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_4_walletinfo.py).
```
$ python3 walletinfo.py 
---------------------------------------------------------------
Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.01031734'),
 'hdseedid': 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1596567843,
 'keypoolsize': 1000,
 'keypoolsize_hd_internal': 999,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 6,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------

Utxos: 
-----
[{'address': 'mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg',
  'amount': Decimal('0.00001000'),
  'confirmations': 1180,
  'desc': "pkh([ce0c7e14/0'/0'/25']02d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472)#n06pq9q5",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 0},
 {'address': 'tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4',
  'amount': Decimal('0.01029734'),
  'confirmations': 1180,
  'desc': "wpkh([ce0c7e14/0'/1'/26']02c581259ba7e6aef6d7ea23adb08f7c7f10c4c678f2e097a4074639e7685d4805)#j3pctfhf",
  'safe': True,
  'scriptPubKey': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 1},
 {'address': 'mzDxbtYY3LBBBJ6HhaBAtnHv6c51BRBTLE',
  'amount': Decimal('0.00001000'),
  'confirmations': 1181,
  'desc': "pkh([ce0c7e14/0'/0'/23']0377bdd176f985b4af2f6bdbb22c2925b6007b6c07ba171f75e65990c002615e98)#3y6ef6vu",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914cd339342b06042bb986a45e73d56db46acc1e01488ac',
  'solvable': True,
  'spendable': True,
  'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
  'vout': 1}]
------------------------------------------

Details of Utxo with txid: 84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
---------------------------------------------------------------
UTXO Details:
------------
{'hash': '0c6c27f58f122329bbc53a91f290b35ce23bd2708706b21a04cdc387dc8e2fd9',
 'locktime': 1831103,
 'size': 225,
 'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
 'version': 2,
 'vin': [{'scriptSig': {'asm': '', 'hex': ''},
          'sequence': 4294967294,
          'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
          'txinwitness': ['3044022014b3e2359fb46d8cbc4cd30fa991b455edfa4b419a4c64a53fcdfc79e3ca89db022010cefc3268bc252d55f1982c426328b709b47d02332def9e2efb3b12de2cf0d301',
                          '0351b470e87b44e8e9607acf09b8d4543c51c93c17dc741176319e60202091f2be'],
          'vout': 0}],
 'vout': [{'n': 0,
           'scriptPubKey': {'addresses': ['mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg'],
                            'asm': 'OP_DUP OP_HASH160 '
                                   'a080d1a10f5e7a02d0a291f118982ed19e8cfcd7 '
                                   'OP_EQUALVERIFY OP_CHECKSIG',
                            'hex': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
                            'reqSigs': 1,
                            'type': 'pubkeyhash'},
           'value': Decimal('0.00001000')},
          {'n': 1,
           'scriptPubKey': {'addresses': ['tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4'],
                            'asm': '0 1e127c28a5d696c01aee1adaa8662396a5902fff',
                            'hex': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
                            'reqSigs': 1,
                            'type': 'witness_v0_keyhash'},
           'value': Decimal('0.01029734')}],
 'vsize': 144,
 'weight': 573}
---------------------------------------------------------------
```
## Crear una dirección

Crear una nueva dirección con Python 3 solo requiere el uso de un RPC como `getnewaddress` o `getrawchangeaddress`.
```
new_address = rpc_client.getnewaddress("Learning-Bitcoin-from-the-Command-Line")
new_change_address = rpc_client.getrawchangeaddress()
```
En este ejemplo, se da el comando `getnewaddress` un argumento: la etiqueta `Learning-Bitcoin-from-the-Command-Line` .

## Enviar una transacción

Crear una transacción en Python 3 requiere combinar algunos de los ejemplos anteriores (de crear direcciones y recuperar UTXOs) con algunos nuevos comandos RPC para crear, firmar y enviar una transacción mucho de lo que ha hecho anteriormente desde la línea de comandos.
Hay cinco pasos a seguir:

0. Crear dos direcciones, una que actuará como receptor y la otra para el cambio.
1. Seleccione un UTXO y establezca los detalles de la transacción.
2. Crear una transacción en bruto.
3. Firmar la transacción en bruto con la clave privada de la UTXO.
4. Difundir la transacción en el bitcoin testnet.

### 1. Seleccione UTXO y definir los detalles de la transacción

En el siguiente fragmento de código, primero seleccione el UTXO que queremos gastar. Entonces usted consigue su dirección, la identificación de la transacción, y el índice vectorial de la salida.
```py
utxos = rpc_client.listunspent()
selected_utxo = utxos[0]  # again, selecting the first utxo here
utxo_address = selected_utxo['address']
utxo_txid = selected_utxo['txid']
utxo_vout = selected_utxo['vout']
utxo_amt = float(selected_utxo['amount'])
```
A continuación, también recupera la dirección del destinatario a la que quiere enviar los bitcoins, calcula la cantidad de bitcoins que quiere enviar, y calcula la tarifa minera y la cantidad de cambio. Aquí, la cantidad se divide arbitrariamente en dos y una cuota de minero se establece arbitrariamente.
```py
recipient_address = new_address
recipient_amt = utxo_amt / 2  # sending half coins to recipient
miner_fee = 0.00000300        # choose appropriate fee based on your tx size
change_address = new_change_address
change_amt = float('%.8f'%((utxo_amt - recipient_amt) - miner_fee))
```
> :warning: **WARNING:** Obviamente un programa real tomaría decisiones más sofisticadas sobre qué UTXO usar, qué hacer con los fondos, y qué cuota de minero pagar.

### 2. Crear transacciones sin procesar

Ahora usted tiene toda la información para enviar una transacción, pero antes de que pueda enviar una, tiene que crear una transacción.
```py
txids_vouts = [{"txid": utxo_txid, "vout": utxo_vout}]
addresses_amts = {f"{recipient_address}": recipient_amt, f"{change_address}": change_amt}
unsigned_tx_hex = rpc_client.createrawtransaction(txids_vouts, addresses_amts)
```
Recuerde que el formato del comando `createrawtransaction` es:

`$ bitcoin-cli createrawtransaction '[{"txid": <utxo_txid>, "vout": <vector_id>}]' '{"<address>": <amount>}'`

El `txids_vouts` es así una lista y el `addresses_amts` es un diccionario de python, para que coincida con el formato de `createrawtransaction`.

Si quiere ver más sobre los detalles de la transacción que ha creado, puede usar `decoderawtransaction`, ya sea en Python 3 o con `bitcoin-cli`.

### 3. Firma la transacción cruda

Firmar una transacción es a menudo la parte más complicada de enviar una transacción programáticamente. Aquí usted recupera una clave privada de una dirección con `dumpprivkey` y la coloca en una matriz:
```py
address_priv_key = []  # list of priv keys of each utxo
address_priv_key.append(rpc_client.dumpprivkey(utxo_address))
```
A continuación, puede utilizar esa matriz (que debe contener las claves privadas de cada UTXO que se está gastando) para firmar su `unsigned_tx_hex`:
```py
signed_tx = rpc_client.signrawtransactionwithkey(unsigned_tx_hex, address_priv_key)
```
Esto devuelve un objeto JSON con el hexadecimal de la transacción firmada, y si fue firmado completamente o no:

### 4. Transacción de transmisión

Por último, está listo para transmitir la transacción firmada en la red bitcoin:
```py
send_tx = rpc_client.sendrawtransaction(signed_tx['hex'])
```
### Ejecute su código

El [código de ejemplo](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_4_sendtx.py) está lleno de sentencias `print` para demostrar todos los datos disponibles en cada punto:
```
$ python3 sendtx.py 
Creating a Transaction
---------------------------------------------------------------
Transaction Details:
-------------------
UTXO Address.......:  mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg
UTXO Txid..........:  84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
Vector ID of Output:  0
UTXO Amount........:  1e-05
Tx Amount..........:  5e-06
Recipient Address..:  tb1qca0elxxqzw5xc0s3yq5qhapzzj90ka0zartu6y
Change Address.....:  tb1qrveukqrvqm9h6fua99xvcxgnvdx507dg8e8hrt
Miner Fee..........:  3e-06
Change Amount......:  2e-06
---------------------------------------------------------------

---------------------------------------------------------------
Unsigned Transaction Hex:  02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f20840000000000ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000
---------------------------------------------------------------

---------------------------------------------------------------
Signed Transaction: 
----------------------
{'complete': True,
 'hex': '02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f2084000000006a47304402205da9b2234ea057c9ef3b7794958db6c650c72dedff1a90d2915147a5f6413f2802203756552aba0dd8ebd71b0f28341becc01b28d8b28af063d7c8ce89f9c69167f8012102d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000'}
---------------------------------------------------------------

---------------------------------------------------------------
TXID of sent transaction:  187f8baa222f9f37841d966b6bad59b8131cfacca861cbe9bfc8656bd16a44cc
```
## Resumen: Acceso a Bitcoind con Python

Acceder a Bitcoind con Python es muy fácil mientras usa la biblioteca `python-bitcoinrpc`. Lo primero que hay que hacer siempre es establecer una conexión con su `bitcoind` instancia, a continuación, puede llamar a todas las llamadas a la API de bitcoin como se describe en la documentación de núcleo de bitcoin. Esto hace que sea fácil crear programas pequeños o grandes para administrar su propio nodo, comprobar balances, o crear aplicaciones interesantes en la parte superior dado que tiene acceso a la potencia total de `bitcoin-cli`.

## ¿Qué es lo siguiente?

Obtén más información sobre "Cómo hablar con Bitcoin en otros lenguajes" en [18.5: Cómo acceder a Bitcoin con Rust](18_5_Accediendo_a_Bitcoind_con_Rust.md).

## Variante: Construir Python desde el Origen

Si necesita instalar Python 3 desde el código fuente, siga estas instrucciones y luego continúe con ["Creando un proyecto BitcoinRPC"](18_4_Accediendo_a_Bitcoind_con_Python.md#creating-a-bitcoinrpc-project).

### 1. Instalar dependencias
```sh
$ sudo apt-get install build-essential checkinstall
$ sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev
```
### 2. Descargar y extraer Python
```sh
$ wget https://www.python.org/ftp/python/3.8.3/Python-3.8.3.tgz
$ tar -xzf Python-3.8.3.tgz
```
### 3. Compilar Python Origen & Comprobar instalación:
```sh
$ cd Python-3.8.3
$ sudo ./configure --enable-optimizations
$ sudo make -j 8  # enter the number of cores of your system you want to use to speed up the build process.
$ sudo make altinstall
$ python3.8 --version
```
Después de obtener la salida de la versión, elimine el archivo de origen:
```sh
$ rm Python-3.8.3.tgz
```
