# 18.6: Acceso a Bitcoind con Swift

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Esta sección explica cómo interactuar con `bitcoind` usando el lenguaje de programación Swift y su propio cliente RPC.

## Configurar Swift en su Mac

Hasta la fecha, ha construido todos sus entornos de desarrollo de lenguaje de programación alternativo en su nodo virtual de Debian. Sin embargo, esa no es la mejor plataforma para Swift. Aunque hay una versión de Swift disponible para las plataformas de Ubuntu, no está completamente disponible, y funciona un poco diferente de la Swift nativa de Mac. Una "variante" en la parte inferior de esta sección explica cómo configurarlo, pero tenga en cuenta que estará en un territorio desconocido.

En su lugar, sugerimos crear un entorno Swift óptimo en una Mac. Hay cuatro pasos importantes para hacerlo.

### 1. Instalar Xcode

Vas a necesitar `Xcode`, el entorno de desarrollo integrado para Swift y Objective-C. Eso se puede instalar fácilmente yendo a Mac App Store y consiguiendo `Xcode`.

#### Alternativa: Instalar a mano

Algunas personas aconsejan no instalar desde el App Store porque es algo como todo o nada; tampoco funcionará si sigue usando Mojave porque quiere evitar las incompatibilidades de Catalina. En ese caso, puede descargar directamente desde el [area de Developer](https://developer.apple.com/download/more/) en Apple.

Si usted esta usando Mojave, necesitará el archivo `xip` para Xcode 10.3.1. De lo contrario, obtenga el más reciente.

Una vez descargado, puede hacer clic en el `xip` para extraerlo, y luego mover la aplicación Xcode a la carpeta Aplicaciones.

(De cualquier manera, debería tener Xcode instalado en su carpeta Aplicaciones al final de este paso.)

### 2. Instale el servidor Gordian

También va a necesitar un nodo Bitcoin en su Mac, para que pueda comunicarse con él. Técnicamente, se puede utilizar un nodo remoto y acceder a él con el inicio de sesión RPC y contraseña a través de la red. Sin embargo, le sugerimos que instale un nodo completo directamente en su Mac, porque es la configuración más segura y limpia, asegurando que ninguna de sus comunicaciones salga de su máquina.

Para instalar fácilmente un nodo completo en su Mac, utilice el servidor de Blockchain Commons [GordianServer for MacOS](https://github.com/BlockchainCommons/GordianServer-macOS). Vea las [instrucciones de instalación](https://github.com/BlockchainCommons/GordianServer-macOS#installation-instructions) en el README, pero generalmente todo lo que tiene que hacer es descargar el archivo actual `dmg`, abrirlo e instalar esa aplicación en su directorio de Aplicaciones también.

Después, ejecute la aplicación GordianServer y decirle que inicie `Start` Testnet. 

> :link: **TESTNET vs. MAINNET:** O `Start` Mainnet.

#### 3. Haga accesible su Gordian bitcoin-cli

Cuando quiera acceder al `bitcoin-cli` creado por GordianServer en su Mac local, puede encontrarlo en  `~/.standup/BitcoinCore/bitcoin-VERSION/bin/bitcoin-cli`, por ejemplo  `~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli`. 

Es posible que desee crear un alias para eso:
```
alias bitcoin-cli="~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli -testnet"
```

> :link: **TESTNET vs. MAINNET:** Obviamente, el parámetro `-testnet` solo es necesario si se está ejecutando en testnet.

### 4. Encuentre su información de GordianServer
Finalmente necesitará su información `rpcuser` y `rpcpassword`. Eso está en `~/Library/Application Support/Bitcoin/bitcoin.conf` por defecto en Gordian.

```
$ grep rpc ~/Library/Application\ Support/Bitcoin/bitcoin.conf
rpcuser=oIjA53JC2u
rpcpassword=ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU
...
```
## Construya su conexión a mano

En el momento de escribir esto, no hay una Biblioteca RPC de Bitcoin actualizada y fácil de usar que sea específica para Swift, algo que pueda introducir e inmediatamente empezar a usar. Por lo tanto, va a hacer algo que nunca ha hecho antes: construir una conexión RPC a mano.

### Escriba el transmisor RPC

Esto solo requiere escribir una función que pase comandos RPC a `bitcoind` en el formato correcto:

```
func makeCommand(method: String, param: Any, completionHandler: @escaping (Any?) -> Void) -> Void {
```
Las conexiones de PC a `bitcoind` usan el protocolo HTML, lo que significa que necesita hacer tres cosas: crear una URL; hacer una URLRequest; e iniciar una URLSession.

#### 1. Crear una URL

Dentro de la función, necesita crear una URL desde su IP, puerto, `rpcuser`, `rpcpassword`, y cartera:
```
    let testnetRpcPort = "18332"
    let nodeIp = "127.0.0.1:\(testnetRpcPort)"
    let rpcusername = "oIjA53JC2u"
    let rpcpassword = "ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU"
    let walletName = ""
```
La conexión RPC real a Bitcoin Core se construye utilizando una URL del formato "http://rpcusername:rpcpassword@nodeIp/walletName":
```
    let walletUrl = "http://\(rpcusername):\(rpcpassword)@\(nodeIp)/\(walletName)"

    let url = URL(string: walletUrl)
```
Esto significa que las variables de muestra resultan en la siguiente URL:
```
http://oIjA53JC2u:ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU@127.0.0.1:18332/
```
Que debería parecerse mucho a la URL utilizada en algunas de las secciones anteriores para conexiones RPC.

#### 2. Crear una URLRequest

Con esa URL en su mano, ahora puede crear una URLRequest, con el método `POST` y el tipo de contenido `text/plain`. El cuerpo HTTP es entonces el objeto JSON familiar que ha estado enviando cada vez que se conectas directamente a los puertos RPC de Bitcoin Core, como se demostró por primera vez al usar Curl en [§4.4](04_4__Interludio_Usando_Curl.md).

```
    var request = URLRequest(url: url!)
    request.httpMethod = "POST"
    request.setValue("text/plain", forHTTPHeaderField: "Content-Type")
    request.httpBody = "{\"jsonrpc\":\"1.0\",\"id\":\"curltest\",\"method\":\"\(method)\",\"params\":[\(param)]}".data(using: .utf8)
```
#### 3. Crear una URLSession

Finalmente, está listo para construir una URLSession alrededor de su URLRequest.
```
    let session = URLSession(configuration: .default)
    let task = session.dataTask(with: request as URLRequest) { data, response, error in
```
El manejador de completitud para `dataTask` necesita comprobar si hay errores:
```
        do {

            if error != nil {

                    //Handle the error
                    
            } else {
```
Y luego analiza los datos que está recibiendo. Aquí, está leyendo de los resultados de JSON en un `NSDictionary`:
```
 if let urlContent = data {
                        
                    do {
                            
                        let json = try JSONSerialization.jsonObject(with: urlContent, options: JSONSerialization.ReadingOptions.mutableLeaves) as! NSDictionary
```
Después de eso, hay más manejo de errores y más manejo de errores y entonces usted puede eventualmente devolver el diccionario `result` usando el `completionHandler` que definió para la nueva función `makeCommand` :
```
                        if let errorCheck = json["error"] as? NSDictionary {
                                                                
                            if let errorMessage = errorCheck["message"] as? String {

                                print("FAILED")
                                print(errorMessage)

                            }
                                
                        } else {
                                
                            let result = json["result"]
                            completionHandler(result)
                            
                        }
                            
                    } catch {
                            
                            //Handle error here
                            
                    }
```
Por supuesto que eventualmente tiene que decirle a la `task` para empezar:
```
    task.resume()
```
Y eso es "todo" lo que hay que hacer para dicha interacción RPC a mano usando un lenguaje de programación como Swift.

> :pray: **THANKS:** Gracias a @Fonta1n3 quien facilitó el [código principal](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues/137) para nuestro transmisor RPC.

### Hacer una llamada RPC

Después de haber escrito la función `makeCommand` RPC, puede enviar una llamada RPC ejecutándola. Aquí está `getblockchaininfo`:
```
let method = "getblockchaininfo"
let param = ""

makeCommand(method: method,param: param) { result in
    
    print(result!)

}
```
### Hacer una llamada RPC con argumentos

De manera similar, podría tomar el conteo de bloques actual de esa información y usarlo para (redundantemente) obtener el hash del bloque actual, usando el parámetro `param` :
```
let method = "getblockchaininfo"
let param = ""

makeCommand(method: method,param: param) { result in
    
    let blockinfo = result as! NSDictionary
    let block = blockinfo["blocks"] as! NSNumber
    
    let method = "getblockhash"
    makeCommand(method: method,param: block) { result in
        print("Blockhash for \(block) is \(result!)")
    }
    
}
```
### Ejecute su código

El código completo está disponible en el [directorio src](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_6_getinfo.playground). Cárgalo en su entorno Xcode y luego "Editor -> Run Playground" y obtendrá resultados como:
```
{	
    bestblockhash = 00000000000000069725608ebc5b59e520572a8088cbc57ffa5ba87b7f300ac7;
    blocks = 1836745;
    chain = test;
    chainwork = 0000000000000000000000000000000000000000000001cc3e9f8e0bc6b71196;
    difficulty = "16508683.81195478";
    headers = 1836745;
    initialblockdownload = 0;
    mediantime = 1601416765;
    pruned = 0;
    "size_on_disk" = 28205538354;
    softforks =     {
        bip34 =         {
            active = 1;
            height = 21111;
            type = buried;
        };
        bip65 =         {
            active = 1;
            height = 581885;
            type = buried;
        };
        bip66 =         {
            active = 1;
            height = 330776;
            type = buried;
        };
        csv =         {
            active = 1;
            height = 770112;
            type = buried;
        };
        segwit =         {
            active = 1;
            height = 834624;
            type = buried;
        };
    };
    verificationprogress = "0.999999907191804";
    warnings = "Warning: unknown new rules activated (versionbit 28)";
}
Blockhash for 1836745 is 00000000000000069725608ebc5b59e520572a8088cbc57ffa5ba87b7f300ac7
```
## Buscar fondos

Con su nuevo `makeCommand` para funciones RPC, también puede ejecutar un comando como `getwalletinfo` o `getbalance`:
```
var method = "getwalletinfo"
var param = ""

makeCommand(method: method,param: param) { result in
    
    print(result!)
    
}

method = "getbalance"
makeCommand(method: method,param: param) { result in

    let balance = result as! NSNumber
    print("Balance is \(balance)")

}
```
Lo que nos dice:
```
Balance is 0.01
{
    "avoid_reuse" = 0;
    balance = "0.01";
    hdseedid = bf493318f548df8e25c390d6a7f70758fd6b3668;
    "immature_balance" = 0;
    keypoololdest = 1599723938;
    keypoolsize = 999;
    "keypoolsize_hd_internal" = 1000;
    paytxfee = 0;
    "private_keys_enabled" = 1;
    scanning = 0;
    txcount = 1;
    "unconfirmed_balance" = 0;
    walletname = "";
    walletversion = 169900;
}
```
## Crear una dirección

Crear una dirección es bastante simple, pero ¿qué pasa con crear una dirección heredada con una etiqueta específica? Eso requiere dos parámetros en su llamada RPC.

Dado que la función simplista `makeCommand` en esta sección simplemente pasa su `param`s como las entrañas de un objeto JSON, todo lo que tiene que hacer es formatear correctamente esas entrañas. Esta es una manera de hacerlo:
```
method = "getnewaddress"
param = "\"learning-bitcoin\", \"legacy\""

makeCommand(method: method,param: param) { result in

    let address = result as! NSString
    print(address)
}
```
Ejecutar esto en el entorno de Xcode produce un resultado:
```
mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF
```
Ese resultado es obviamente una dirección Legacy; su etiqueta se puede comprobar desde la línea de comandos:
```
$ bitcoin-cli getaddressesbylabel "learning-bitcoin"
{
  "mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF": {
    "purpose": "receive"
  }
}
```
Éxito!

> :information_source: **NOTA:** Como decimos a menudo en estos ejemplos de codificación, un programa del mundo real sería mucho más sofisticado. En particular, querrá ser capaz de enviar un objeto JSON real como parámetro, y luego tener su programa `makeCommand` para analizarlo e introducirlo en la URLSession apropiadamente. Lo que tenemos aquí maximiza la legibilidad y la simplicidad sin centrarse en la facilidad de uso.

## Enviar una transacción

Como de costumbre, el envío de una transacción (la forma difícil) es un proceso de varios pasos:
0. Generar o recibir una dirección de recepción
1. Encontrar una UTXO no utilizada
2. Crear una transacción en bruto
3. Firmar la transacción en bruto
4. Envío de la transacción en bruto

Usará la `dirección` que generó en el paso anterior como su destinatario.

### 1. Encuentre un UTXO no gastado

El RPC `listunspent` le permite encontrar su UTXO:
```
    method = "listunspent"
    param = ""
    
    makeCommand(method: method,param: param) { result in

        let unspent = result as! NSArray
        let utxo = unspent[0] as! NSDictionary
        
        let txid = utxo["txid"] as! NSString
        let vout = utxo["vout"] as! NSInteger
        let amount = utxo["amount"] as! NSNumber
        let new_amount = amount.floatValue - 0.0001
```
Como en otros ejemplos, usted va a tomar arbitrariamente el 0º UTXO, y usará las `txid`, `vout`, y `amount` de ella.

> :information_source **NOTA:** Una vez más, un programa de la vida real sería mucho más sofisticado.

### 2. Crear una transacción sin procesar

Crear una transacción en bruto es la cosa más complicada porque necesita obtener todos sus objetos JSON, matrices y comillas correctas. He aquí cómo hacerlo en Swift, usando el formato muy básico `param` del transmisor:
```
        method = "createrawtransaction"
        param="[ { \"txid\": \"\(txid)\", \"vout\": \(vout) } ], { \"\(address)\": \(new_amount)}"
        makeCommand(method: method,param: param) { result in

            let hex = result as! NSString
```
### 3. Firme la transacción cruda

La firma de su transacción sólo requiere que ejecute la transacción `signrawctionwithwallet` RPC, usando su nuevo `hex`:
```
            method = "signrawtransactionwithwallet"
            param = "\"\(hex)\""
            
            makeCommand(method: method,param: param) { result in

                let signedhexinfo = result as! NSDictionary
                let signedhex = signedhexinfo["hex"] as! NSString
```
### 4. Enviar la transacción sin procesar

El envío de su transacción es igualmente simple:
```
                method = "sendrawtransaction"
                param = "\"\(signedhex)\""

                makeCommand(method: method,param: param) { result in

                    let new_txid = result as! NSString
                    print("TXID: \(new_txid)")
                    
                }
            }         
        }
    }
}
```
El código para este remitente de transacción se puede encontrar en el [src directory](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_6_sendtx.playground).

## Usar Swift de otras maneras

Eso cubre nuestras discusiones habituales sobre la programación de Bitcoin RPC en un lenguaje, pero Swift es un lenguaje particularmente importante ya que se puede implementar en dispositivos móviles, uno de los principales lugares para carteras. Como tal, puede considerar otras bibliotecas:

* El entorno de Blockchain Commons [ios-Bitcoin framework](https://github.com/BlockchainCommons/iOS-Bitcoin) convierte la biblioteca Libbitcoin de C++ a Swift
* [Libwally Swift](https://github.com/blockchain/libwally-swift) es una envoltura de Swift para Libwally

## Resumen: Acceso a Bitcoind con Swift

Swift es un robusto lenguaje de programación moderno que desafortunadamente aún no tiene bibliotecas RPC fáciles de usar ... lo que nos dio la oportunidad de escribir una función de acceso RPC propia. Con eso en la mano, puede interactuar con `bitcoind` en un Mac o construir aplicaciones complementarias en un iPhone, que es una combinación perfecta para el trabajo Bitcoin desconectado o `airgapped`.

## ¿Qué es lo siguiente?

Aprenda acerca de Lightning en [Capítulo 19: Entendiendo su configuración de Lightning](19_0_Entendiendo_Su_Configuracion_Lightning.md).

## Variante: Implementar Swift en Ubuntu

Si prefiere implementar Swift en Ubuntu, puede hacerlo, aunque la funcionalidad no es la misma. Parte del código en este capítulo probablemente generará errores que necesitará resolver, y también necesitará hacer más trabajo para enlazar en bibliotecas C.

Para empezar, instale algunas necesarias bibliotecas Debian :
```
$ sudo apt-get install clang
$ sudo apt-get install libcurl4 libpython2.7 libpython2.7-dev
```
Si está usando Debian 10 o superior (y realmente debería estarlo), también necesitará poner en una fecha anterior algunas bibliotecas para obtener versiones anteriores:
```
$ sudo apt-get install libtinfo5 libncurses5
```
Luego puede descargar e instalar Swift:
```
$ wget https://swift.org/builds/swift-5.1.3-release/ubuntu1804/swift-5.1.3-RELEASE/swift-5.1.3-RELEASE-ubuntu18.04.tar.gz
$ tar xzfv swift-5.1.3-RELEASE-ubuntu18.04.tar.gz 
$ sudo mv swift-5.1.3-RELEASE-ubuntu18.04 /usr/share/swift
```
Para poder usar su nueva configuración de Swift, y necesita actualizar su `PATH`en su `.bashrc`:
```
$ echo "export PATH=/usr/share/swift/usr/bin:$PATH" >> ~/.bashrc
$ source ~/.bashrc
```
Ahora puede probar Swift con el argumento `-versión` :
```
$ swift --version
Swift version 5.1.3 (swift-5.1.3-RELEASE)
Target: x86_64-unknown-linux-gnu
```
### Crear un proyecto
Una vez que haya instalado Swift en su máquina Ubuntu, puede crear proyectos con el comando `package init` :
```
$ mkdir swift-project
$ cd swift-project/
/swift-project$ swift package init --type executable
Creating executable package: swift-project
Creating Package.swift
Creating README.md
Creating .gitignore
Creating Sources/
Creating Sources/swift-project/main.swift
Creating Tests/
Creating Tests/LinuxMain.swift
Creating Tests/swift-projectTests/
Creating Tests/swift-projectTests/swift_projectTests.swift
Creating Tests/swift-projectTests/XCTestManifests.swift
```
Luego editará `Sources/.../main.swift` y cuando esté listo para compilar, puede usar el comando `build` :
```
$ swift build
[4/4] Linking swift-project
```
Finalmente, podrá ejecutar el programa desde el directorio `.build/debug` :
```
$ .build/debug/swift-project
Hello, world!
```
Buena suerte!
