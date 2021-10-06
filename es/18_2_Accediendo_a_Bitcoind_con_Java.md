# 18.2: Acceso a Bitcoind con Java

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias

Esta sección explica cómo interactuar con `bitcoind` usando el lenguaje de programación Java y el cliente [JavaBitcoindRpcClient](https://githubom/Polve/JavaBitcoindRpcClient). 

## Configuración de Java

Puede instalar Java en su servidor, usando el comando `apt-get` . También instalará [Apache Maven](http://maven.apache.org/) para administrar las dependencias. 
```
$ sudo apt-get install openjdk-11-jre-headless maven
```
Puede verificar su instalación de Java:
```
$ java -version
openjdk version "11.0.8" 2020-07-14
OpenJDK Runtime Environment (build 11.0.8+10-post-Debian-1deb10u1)
OpenJDK 64-Bit Server VM (build 11.0.8+10-post-Debian-1deb10u1, mixed mode, sharing)
```
### Crear un proyecto Maven

Para programar con Bitcoin usando java, creará un proyecto Maven:
```
$ mvn archetype:generate -DgroupId=com.blockchaincommons.lbtc -DartifactId=java-project -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false
```
Esto descargará algunas dependencias
```
Downloading: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-clean-plugin/2.5/maven-clean-plugin-2.5.pom
Downloaded: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-clean-plugin/2.5/maven-clean-plugin-2.5.pom (4 KB at 4.2 KB/sec)
Downloading: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-plugins/22/maven-plugins-22.pom
Downloaded: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-plugins/22/maven-plugins-22.pom (13 KB at 385.9 KB/sec)
Downloading: https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/21/maven-parent-21.pom
Downloaded: https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/21/maven-parent-21.pom (26 KB at 559.6 KB/sec)
Downloading: https://repo.maven.apache.org/maven2/org/apache/apache/10/apache-10.pom
..............
```
También creará un archivo de configuración `pom.xml`:

```
$ cd java-project
$ ls -lagh
total 16K
drwxr-xr-x  3 sudo 4.0K Sep  1 13:58 .
drwxr-xr-x 15 sudo 4.0K Sep  1 13:58 ..
-rw-r--r--  1 sudo  663 Sep  1 13:58 pom.xml
drwxr-xr-x  4 sudo 4.0K Sep  1 13:58 src
```
Para incluir `JavaBitcoindRpcClient`, debe añadir su dependencia a `dependendencies` en el archivo `pom.xml`

```xml
      <dependency>
        <groupId>wf.bitcoin</groupId>
        <artifactId>bitcoin-rpc-client</artifactId>
        <version>1.2.1</version>
      </dependency>
```
También necesita agregar propiedades de compilador para indicar qué versión de JDK compilará el código fuente.
```xml
  <properties>                                                                  
    <!-- https://maven.apache.org/general.html#encoding-warning -->
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>11</maven.compiler.source>
    <maven.compiler.target>11</maven.compiler.target>
  </properties>                                                                 
```
Siempre que agregue código fuente a sus clases, podrá probarlo con:
```
$ mvn compile
```
También puede ejecutar con `exec:java`
```
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
```
### Crear proyectos alternativos

Si usa [Gradle](https://gradle.org/releases/), puede ejecutar:

```groovy
compile 'wf.bitcoin:JavaBitcoindRpcClient:1.2.1'
```

Si desea un proyecto de ejemplo y algunas instrucciones sobre cómo ejecutarlo en el servidor que acaba de crear, puede consultar el [Proyecto de Ejemplo](https://github.com/brunocvcunha/bitcoind-java-client-sample/). También puede buscar todo el código fuente para [bitcoin-rpc-client](https://github.com/Polve/bitcoin-rpc-client).

## Construya su conexión

Para usar `JavaBitcoindRpcClient`, necesita crear una instancia `BitcoindRpcClient`. Esto se hace creando una URL con argumentos de nombre de usuario, contraseña, dirección IP y puerto. Como recordará, la dirección IP `127.0.0.1` y el puerto `18332` deberían ser correctos para la configuración de testnet estándar descrita en este curso, mientras que puede extraer el usuario y la contraseña de `~/. bitcoin/bitcoin.conf`.

```java
BitcoindRpcClient rpcClient = new BitcoinJSONRPCClient("http://StandUp:6305f1b2dbb3bc5a16cd0f4aac7e1eba@localhost:18332");
```

Tenga en cuenta que también necesitará importar la información adecuada:

```java
import wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient;
```
> **MAINNET VS TESTNET:** El puerto sería 8332 para una configuración de mainnet.

Si `rpcClient` se inicializa correctamente, podrá enviar comandos RPC.

Más tarde, cuando usted haya terminado con su conexión `bitcoind`, debería cerrarla:

```java
rpcClient.stop();
```

### Hacer una llamada RPC

Encontrará que `BitcoindRpcClient` proporciona la mayor parte de la funcionalidad a la que se puede acceder a través de `bitcoin-cli` u otros métodos RPC, utilizando los mismos nombres de método, pero en camelCase. 

Por ejemplo, para ejecutar el comando `getmininginfo` para obtener la información de bloque y la dificultad en la red, debe utilizar el método `getMiningInfo()`:

```java
MiningInfo info = rpcClient.getMiningInfo();
System.out.println("Mining Information");
System.out.println("------------------");
System.out.println("Chain......: " + info.chain());
System.out.println("Blocks.....: " + info.blocks());
System.out.println("Difficulty.: " + info.difficulty());
System.out.println("Hash Power.: " + info.networkHashps());
```
La salida para esta línea debe ser similar a esta:
```
Mining Information
------------------
Chain......: test
Blocks.....: 1830905
Difficulty.: 4194304
Hash Power.: 40367401348837.41
```
### Hacer una llamada RPC con argumentos

Puede buscar direcciones en su cartera pasando la dirección como argumento a `getAddressInfo`:

```java
	String addr1 = "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo";
	
	AddressInfo addr1Info = rpcClient.getAddressInfo(addr1);
	System.out.println("Address: " + addr1Info.address());
	System.out.println("MasterFingerPrint: " + addr1Info.hdMasterFingerprint());
	System.out.println("HdKeyPath: " + addr1Info.hdKeyPath());
	System.out.println("PubKey: " + addr1Info.pubKey());
```
La salida se verá algo así:
```
Address: mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo
MasterFingerPrint: ce0c7e14
HdKeyPath: m/0'/0'/5'
PubKey: 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65
```
### Ejecute su código

El código para estos ejemplos se puede encontrar en el [directorio src](../src/18_2_App-getinfo.java) y debe ser instalado en la estructura de directorios estándar creada aquí como `~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`. Luego se puede compilar y ejecutar.
```
$ mvn compile
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
Chain......: test
Blocks.....: 1831079
Difficulty.: 4194304
Hash Power.: 38112849943221.16
Address: mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo
MasterFingerPrint: ce0c7e14
HdKeyPath: m/0'/0'/5'
PubKey: 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65
```
(También verá mucha más información sobre la compilación, por supuesto.)

## Buscar fondos

Recuperar el saldo de una cuenta entera es igualmente fácil:
```java
        System.out.println("Balance: " + rpcClient.getBalance());
```
## Crear una dirección

Puede crear una nueva dirección en su cartera, adjuntar una etiqueta específica a ella, e incluso volcar su clave privada.
```java
  String address = rpcClient.getNewAddress("Learning-Bitcoin-from-the-Command-Line");
  System.out.println("New Address: " + address);

  String privKey = rpcClient.dumpPrivKey(address);
  System.out.println("Priv Key: " + privKey);
```
salida:
```
New Address: mpsFtZ8qTJPRGZy1gaaUw37fHeUSPLkzzs
Priv Key: cTy2AnmAALsHokYzJzTdsUBSqBtypmWfmSNYgG6qQH43euUZgqic
```
## Enviar una transacción

La biblioteca JavaBitcoindRpcClient tiene algunas buenas herramientas que facilitan la creación de una transacción desde cero.

### Crear una transacción

Puede crear una transacción sin procesar usando el método `createRawTransaction`, pasando como argumentos dos objetos ArrayList que contienen las entradas y salidas a utilizar.  

Primero usted establece sus nuevas direcciones, aquí una dirección existente en su sistema y una nueva dirección en su sistema.
```java
        String addr1 = "tb1qdqkc3430rexxlgnma6p7clly33s6jjgay5q8np";
        System.out.println("Used address addr1: " + addr1);

        String addr2 = rpcClient.getNewAddress();
        System.out.println("Created address addr2: " + addr2);
```
Luego, puede usar la lista `RPC para encontrar UTXOs para la dirección existente.
```java
        List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
        System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```
Aquí está una salida de toda la información:

```java
    System.out.println("Created address addr1: " + addr1);
    String addr2 = rpcClient.getNewAddress();
    System.out.println("Created address addr2: " + addr2);
    List<String> generatedBlocksHashes = rpcClient.generateToAddress(110, addr1);
    System.out.println("Generated " + generatedBlocksHashes.size() + " blocks for addr1");
    List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
    System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```
Las transacciones se construyen con `BitcoinRawTxBuilder`:
```java
        BitcoinRawTxBuilder txb = new BitcoinRawTxBuilder(rpcClient);
```
Primero llene las entradas con los UTXOs que está gastando:
```java
        TxInput in = utxos.get(0);
        txb.in(in);
```
> :warning: **ADVERTENCIA:** Obviamente, en un programa real, seleccionaría inteligentemente un UTXO; aquí, simplemente tomamos el 0º, una táctica que usaremos a lo largo de este capítulo.

En segundo lugar, se llena el ouputs cada uno con una cantidad y una dirección:
```java
	  BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
    BigDecimal txToAddr2Amount = utxos.get(0).amount().subtract(estimatedFee);
	  txb.out(addr2, txToAddr2Amount);
		System.out.println("unsignedRawTx in amount: " + utxos.get(0).amount());
    System.out.println("unsignedRawTx out amount: " + txToAddr2Amount);
```
Ahora está listo para crear la transacción:
```java
	String unsignedRawTxHex = txb.create();
	System.out.println("Created unsignedRawTx from addr1 to addr2: " + unsignedRawTxHex);
```

### Firmar una transacción

Ahora puede firmar la transacción con el método `signRawTransactionWithKey`. Este método recibe como parámetros una transacción de cadena bruta sin firmar, la clave privada de la dirección de envío y el objeto TxInput.
```java
	SignedRawTransaction srTx = rpcClient.signRawTransactionWithKey(
					unsignedRawTxHex,
					Arrays.asList(rpcClient.dumpPrivKey(addr1)), // 
					Arrays.asList(in),
					null);
	System.out.println("signedRawTx hex: " + srTx.hex());
	System.out.println("signedRawTx complete: " + srTx.complete());
```
### Enviar una transacción

Finalmente enviar requiere el comando `sendRawTransaction` :
```java
    String sentRawTransactionID = rpcClient.sendRawTransaction(srTx.hex());
    System.out.println("Sent signedRawTx (txID): " + sentRawTransactionID);```
```
### Ejecute su código

Ahora puede ejecutar [el código de transacción](../src/18_2_App-sendtx.java) como `~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`.
```
$ mvn compile
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
Used address addr1: tb1qdqkc3430rexxlgnma6p7clly33s6jjgay5q8np
Created address addr2: tb1q04q2wzlhfqlrnz95ynfj7gp4t3yynrj0542smv
Found 1 UTXOs (unspent transaction outputs) belonging to addr1
unsignedRawTx in amount: 0.00850000
unsignedRawTx out amount: 0.00849800
Created unsignedRawTx from addr1 to addr2: 0200000001d2a90fc3b43e8eb4ae9452af43c9448112d359cac701f7f537aa8b6f39193bb90100000000ffffffff0188f70c00000000001600147d40a70bf7483e3988b424d32f20355c48498e4f00000000
signedRawTx hex: 02000000000101d2a90fc3b43e8eb4ae9452af43c9448112d359cac701f7f537aa8b6f39193bb90100000000ffffffff0188f70c00000000001600147d40a70bf7483e3988b424d32f20355c48498e4f024730440220495fb64d8cf9dee9daa8535b8867709ac8d3763d693fd8c9111ce610645c76c90220286f39a626a940c3d9f8614524d67dd6594d9ee93818927df4698c1c8b8f622d01210333877967ac52c0d0ec96aca446ceb3f51863de906e702584cc4da2780d360aae00000000
signedRawTx complete: true
Sent signedRawTx (txID): 82032c07e0ed91780c3369a1943ea8abf49c9e11855ffedd935374ecbc789c45
```
# Escuchar transacciones o bloques

Al igual que con [C and its ZMQ libraries](16_3_Recibiendo_Notificaciones_de_Bitcoind_en_C_con_las_Bibliotecas_ZMQ.md), hay maneras fáciles de usar Java para escuchar la cadena de bloques y para ejecutar código específico cuando algo sucede, como una transacción que implica una dirección en su cartera, o incluso la generación de un nuevo bloque en la red.

Para ello, usa la clase `JavaBitcoindRpcClient` `BitcoinAcceptor`, que le permite conectar oyentes en la red.
```java
        String blockHash = rpcClient.getBestBlockHash();
	BitcoinAcceptor acceptor = new BitcoinAcceptor(rpcClient, blockHash, 6, new BitcoinPaymentListener() {

	      @Override
	      public void transaction(Transaction tx) {
	      	System.out.println("Transaction: " + tx);
	      }

	      @Override
	      public void block(String block) {
	      	System.out.println("Block: " + block);
	      }
	});
  acceptor.run();
```
Vea en el [directorio src](../src/18_2_App-listen.java) para el código completo. Cada vez que se envíe una transacción o se genere un nuevo bloque, debería ver la salida en su consola:
```
Transaction: {account=Tests, address=mhopuJzgmTwhGfpNLCJ9CRknugY691oXp1, category=receive, amount=5.0E-4, label=Tests, vout=1, confirmations=0, trusted=false, txid=361e8fcff243b74ebf396e595a007636654f67c3c7b55fd2860a3d37772155eb, walletconflicts=[], time=1513132887, timereceived=1513132887, bip125-replaceable=unknown}

Block: 000000004564adfee3738314549f7ca35d96c4da0afc6b232183917086b6d971
```
### Resumen Acceso a Bitcoind con Java

Al utilizar la biblioteca javabitcoinrpc, puede acceder fácilmente a bitcoind a través de llamadas RPC desde Java. También tendrá acceso a buenas funciones adicionales, como el servicio de escucha `bitcoinAcceptor` .

## ¿Qué es lo siguiente?

Obtenga más información sobre "Cómo hablar con Bitcoin en otros lenguajes" en [18.3: Cómo acceder a Bitcoin con NodeJS](18_3_Accediendo_a_Bitcoind_con_NodeJS.md).
