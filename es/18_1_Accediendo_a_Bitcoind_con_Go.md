# 18.1: Acceso a Bitcoind con Go

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Esta sección explica cómo interactuar con `bitcoind` usando el lenguaje de programación Go y el [btcd rpcclient](https://github.com/btcsuite/btcd/tree/master/rpcclient). Tenga en cuenta que tiene algunas peculiaridades y algunas limitaciones.

## Configurar Go

Para prepararse para el uso de Go en su máquina UNIX,  primero instale curl si aún no lo ha hecho:
```
$ sudo apt install curl
```
Luego, mire la página [de descargas Go](https://golang.org/dl/), obtenga el enlace para la última descarga y descarguelo usando `curl`. Para una configuración de Debian, querrá usar la versión `linux-amd64`:

```
$ curl -O https://dl.google.com/go/go1.15.1.linux-amd64.tar.gz
```

Una vez finalizada la descarga, compare el hash de la descarga con el hash en la página [de descargas Go](https://golang.org/dl/):
```
$ sha256sum go1.15.1.linux-amd64.tar.gz 
70ac0dbf60a8ee9236f337ed0daa7a4c3b98f6186d4497826f68e97c0c0413f6  go1.15.1.linux-amd64.tar.gz
```
Los hashes deben coincidir. Si es así, extraiga el archivo e instale Go en su sistema:
```
$ tar xfv go1.15.1.linux-amd64.tar.gz 
$ sudo chown -R root:root ./go
$ sudo mv go /usr/local
```
Ahora necesita crear una ruta Go para especificar su entorno. Abra el archivo `~/.profile`  con un editor de su elección y añada lo siguiente al final de la misma:

```
export GOPATH=$HOME/work
export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
```

Por último, cree el directorio de su espacio de trabajo Go:

```
$ mkdir $HOME/work
```
### Configurar `btcd` `rpcclient`

Usara el `rpcclient` que viene con `btcd,` una implementación de Bitcoin escrita en Go. Aunque `rpcclient` fue originalmente diseñado para trabajar con el nodo completo de Bitcoin `btcd` , también funciona con Bitcoin Core. Tiene algunas peculiaridades que veremos. 

Puede utilizar ```go get``` para descargarlo:
```
$ go get github.com/btcsuite/btcd/rpcclient
```
Para probar que funciona, vaya al directorio con los ejemplos de Bitcoin Core:
```
$ cd $GOPATH/src/github.com/btcsuite/btcd/rpcclient/examples/bitcoincorehttp
```
Modificar el archivo `main.go` e introduzca los detalles asociados con la configuración del núcleo de Bitcoin, que se pueden encontrar en `~/.bitcoin/bitcoin.conf`:

```
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "6305f1b2dbb3bc5a16cd0f4aac7e1eba",
```
> **MAINNET VS TESTNET:** El puerto 8332 sería para una configuración de mainnet.

Ahora puede realizar una prueba:
```
$ go run main.go
```
Deberia ver impreso el conteo de bloques:
```
2020/09/01 11:41:24 Block count: 1830861
```
### Crear un proyecto `rpcclient` 

Normalmente estará creando proyectos en su directorio `~/work/src/myproject/bitcoin` :

```
$ mkdir -p ~/work/src/myproject/bitcoin
$ cd ~/work/src/myproject/bitcoin
```
Cada proyecto deberá tener los siguientes `imports`:

```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
)
```

Esta declaración `import`  le permite importar bibliotecas relevantes. Para cada ejemplo aquí, necesitará importar `"log", "fmt"` y `"github.com/btcsuite/btcd/rpcclient"`. Es posible que necesite importar bibliotecas adicionales para algunos ejemplos. 
   * `log` se utiliza para imprimir mensajes de error. Después de cada llamada al nodo Bitcoin, una declaración `if` comprobará si hay algún error. Si hay errores, `log` se utiliza para imprimirlos. 
   * `fmt` se utiliza para imprimir la salida. 
   * `rpcclient`; es obviamente la biblioteca `rpcclient`
   * 
## Construya su conexión

Cada función `bitcoind` en Go comienza con la creación de la conexión RPC, usando la función `ConnConfig` :
 
 ```
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()
```
Los parámetros `connCfg` le permiten elegir el puerto RPC de Bitcoin, nombre de usuario, contraseña y si está en testnet o mainnet. 

> **NOTA:** De nuevo, asegúrese de sustituir el `User` y `Pass` con el que se encuentra en su `~/.bitcoin/bitcon.conf`. 

Por tanto la función `rpcclient. New(connCfg, nil)` configura su `client` para conectarse a su nodo Bitcoin.
La línea `defer client.Shutdown()` es para desconectar de su nodo Bitcoin, una vez que la función `main()` termina de ejecutarse. 
Después de la línea `defer client.Shutdown()` es donde va el material emocionante y será bastante fácil de usar. 
Eso es porque `rpcclient` ayuda a convertir los comandos `bitcoin-cli` en funciones usando PascalCase. 
Por ejemplo, `bitcoin-cli getblockcount` será `client.GetBlockCount` en Go.

### Hacer una llamada RPC

Todo lo que se requiere ahora es hacer una llamada informativa como `GetBlockCount` o `GetBlockHash` usando su `client`:

```
	blockCount, err := client.GetBlockCount()
	if err != nil {
		log.Fatal(err)
	}
	blockHash, err := client.GetBlockHash(blockCount)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("%d\n", blockCount)
	fmt.Printf("%s\n", blockHash.String())
```
### Hacer una llamada RPC con argumentos

Las funciones `rpcclient` también pueden tomar entradas; por ejemplo `cliente.GetBlockHash(blockCount)` toma el conteo de bloques como una entrada. 
El cliente `client.GetBlockHash(blockCount)` visto anteriormente se vería como un comando `bitcoin-cli`:

```
$ bitcoin-cli getblockhash 1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```
Sin embargo, una peculiaridad con hashes en `rpcclient` es que típicamente se imprimirán en una codificación diferente si se va a imprimir normalmente usando `blockHash`. 
Para imprimirlos como una cadena, es necesario utilizar `blockHash.String()`.

### Ejecute su código

Puede descargar el código completo desde el [src directory](../src/18_1_blockinfo.go).

A continuación, puede ejecutar:
```
$ go run blockinfo.go 
1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```
El último número de bloque junto con su hash debe imprimirse.

## Buscar fondos

Debido a las limitaciones de la función `btcd` `rpcclient`, no se puede hacer un uso de la función `getwalletinfo`. 
Sin embargo, usted puede hacer uso de la llamada RPC `getbalance`:
```
	wallet, err := client.GetBalance("*")
	if err != nil {
		log.Fatal(err)
	}

```

`client.GetBalance("*")` requiere la entrada `"*"`, debido a una peculiaridad con `btcd`. El asterisco significa que usted quiere conseguir el saldo de todas sus carteras.
Si ejecuta [el código src](../src/18_1_getbalance.go), debería obtener una salida similar a esta:
```
$ go run getbalance.go 
0.000689 BTC
```

## Crear una dirección

Puede generar direcciones en Go, pero no puede especificar el tipo de dirección:

Esto requiere el uso de una función especial `chaincfg`, para especificar para qué red se están creando las direcciones. 
Esta especificación solo se requiere durante la generación de direcciones, por lo que solo se utiliza en este ejemplo. 
También puede incluir esto en otros ejemplos, pero no es necesario.

Asegúrese de importar `"github.com/btcsuite/btcd/chaincfg"`:

```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg"
)
```

Luego llame a `connCfG` con el parámetro `chaincfg.TestNet3Params.Name` :

```
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
		Params: chaincfg.TestNet3Params.Name,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()
```
**MAINNET VS TESTNET:** `Params: chaincfg.TestNet3Params.Name,` debe ser `Parameters: chaincfg.MainNetParams.Name,` en mainnet.

A continuación, puede crear su dirección:
```
	address, err := client.GetNewAddress("")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(address)
```
Una peculiaridad con `client.GetNewAddress(")` es que se debe incluir una cadena vacía para que funcione.

Ejecutando [la fuente](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_1_getaddress.go) se obtienen los siguientes resultados:

```
$ go run getaddress.go 
tb1qutkcj34pw0aq7n9wgp3ktmz780szlycwddfmza
```
### Decodificar una dirección

La creación de una dirección tomó un trabajo adicional, en especificar la cadena apropiada. 
Usar una dirección también lo hará porque tendrá que decodificarla antes de usarla. 

Los medios que usted tendrá que importar tanto las librerías `"github.com/btcsuite/btcutil"` como `"github.com/btcsuite/btcd/chaincfg"`.
   * `btcutil` permite que una dirección de Bitcoin sea decodificada de una manera que el cliente `rpcclient` pueda entender. Esto es necesario cuando se trabaja con direcciones en `rpcclient`. 
   * `chaincfg` se utiliza (de nuevo) para configurar su cadena como la cadena Testnet. Esto es necesario para la decodificación de direcciones ya que las direcciones utilizadas en Mainnet y Testnet son diferentes.
```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)
```
La variable defaultNet ahora se utiliza para especificar si su nodo Bitcoin está en testnet o en mainnet. Esa información (y el objeto `btcutil` ) se utiliza para decodificar la dirección.

> **MAINNET VS TESTNET:** `&chaincfg.TestNet3Params` debe ser `&chaincfg.MainNetParams` en mainnet.
```
	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
```
> **NOTA:** Cambie la dirección (`mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha`) para una de su billetera actual; puede utilizar `bitcoin-cli listunspent` para encontrar algunas direcciones con fondos para esta prueba. Si quiere ser realmente elegante, modifique el código Go para tomar un argumento, luego escriba un script que ejecute `listunspent`, guarde la información en una variable, y ejecute el código Go con eso.

Solo después de esto, use la dirección `getreceivedbyaddress` RPC, en su dirección decodificada:
```
	wallet, err := client.GetReceivedByAddress(addr)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
```
Cuando ejecute [el código](../src/18_1_getamountreceived.go), debería obtener una salida similar a:
```
$ go run getamountreceived.go 
0.0085 BTC
```
## Enviar una transacción

Ahora tiene todas las piezas del rompecabezas en su lugar para enviar una transacción. Va a querer:

1. Importar las bibliotecas correctas, incluyendo `chaincfg` para especificar una red y `btcutil` para decodificar una dirección.
2. Elija una dirección para enviar.
3. Descifre esa dirección.
4. Ejecute `sendtoaddress` para enviar los fondos de la manera fácil.

```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
	sent, err := client.SendToAddress(addr, btcutil.Amount(1e4))
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(sent)
}
```
Cuando se ejecuta [el código](../src/18_1_sendtransaction.go), el txid de la transacción se emite:
```
$ go run sendtransaction.go
9aa4cd6559e0d69059eae142c35bfe78b71a8084e1fcc2c74e2a9675e9e7489d
```
### Buscar una transacción

Para buscar una transacción, como la que acaba de enviar, tendrá que volver a hacer algunas conversiones, esta vez de txid. `"github.com/btcsuite/btcd/chaincfg/chainhash"` se importa para permitir que los hashes se almacenen en el código Go. `chainhash.NewHashFromStr("hash")` convierte un hash en una cadena a un formato que funciona con rpcclient.
```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg/chainhash"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	chash, err := chainhash.NewHashFromStr("1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df")
	if err != nil {
		log.Fatal(err)
	}
	transactions, err := client.GetTransaction(chash)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(transactions)
}
```
> **NOTA:** De nuevo, querrá cambiar el txid por uno realmente reconocido por su sistema.

Cuando ejecute [el código](../src/18_1_lookuptransaction.go) imprimirá los detalles asociados con una transacción, como su cantidad y cuántas veces se ha confirmado:
```
$ go run lookuptransaction.go
{
  "amount": 0.00100000,
  "confirmations": 4817,
  "blockhash": "000000006628870b0a8a66abea9cf0d4e815c491f079e3fa9e658a87b5dc863a",
  "blockindex": 117,
  "blocktime": 1591857418,
  "txid": "1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df",
  "walletconflicts": [
  ],
  "time": 1591857343,
  "timereceived": 1591857343,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha",
      "category": "receive",
      "amount": 0.00100000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "02000000000101e9e8c3bd057d54e73baadc60c166860163b0e7aa60cab33a03e89fb44321f8d5010000001716001435c2aa3fc09ea53c3e23925c5b2e93b9119b2568feffffff02a0860100000000001976a914600c8c6a4abb0a502ea4de01681fe4fa1ca7800688ac65ec1c000000000017a91425b920efb2fde1a0277d3df11d0fd7249e17cf8587024730440220403a863d312946aae3f3ef0a57206197bc67f71536fb5f4b9ca71a7e226b6dc50220329646cf786cfef79d60de3ef54f702ab1073694022f0618731902d926918c3e012103e6feac9d7a8ad1ac6b36fb4c91c1c9f7fff1e7f63f0340e5253a0e4478b7b13f41fd1a00"
}
```
## Resumen: Acceso a Bitcoind con Go

Aunque `btcd` y `rpcclient` tienen algunos límites, aún es posible realizar los comandos principales de RPC en Go. La documentación para `rpcclient` está disponible en [Godoc](https://godoc.org/github.com/btcsuite/btcd/rpcclient). Si la documentación no tiene lo que está buscando, consulte también el [repositorio btcd](https://github.com/btcsuite/btcd). Generalmente está bien documentado y es fácil de leer. Sobre la base de estos ejemplos debería ser capaz de incorporar Bitcoin en un proyecto Go y hacer cosas como enviar y recibir monedas.

## ¿Que sigue?

Obtenga más información sobre "Hablando con Bitcoin en otros lenguages" en [18.2: Accediendo a Bitcoin con Java](18_2_Accediendo_a_Bitcoind_con_Java.md).
