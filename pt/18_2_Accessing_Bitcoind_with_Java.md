# 18.2: Acessando o Bitcoind com Java

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta seção explica como interagir com `bitcoind` usando a linguagem de programação Java e o [JavaBitcoindRpcClient](https://github.com/Polve/JavaBitcoindRpcClient).

## Configurando o Java

Podemos instalar o Java em nosso servidor, usando o comando `apt-get`. Também precisaremos instalar o [Apache Maven](http://maven.apache.org/) para gerenciar as dependências.
```
$ sudo apt-get install openjdk-11-jre-headless maven
```

Podemos verificar a instalação do Java usando o comando abaixo:
```
$ java -version
openjdk version "11.0.8" 2020-07-14
OpenJDK Runtime Environment (build 11.0.8+10-post-Debian-1deb10u1)
OpenJDK 64-Bit Server VM (build 11.0.8+10-post-Debian-1deb10u1, mixed mode, sharing)
```

### Criando um Projeto Maven

Para programar o Bitcoin usando o java, criaremos um projeto Maven:
```
$ mvn archetype:generate -DgroupId=com.blockchaincommons.lbtc -DartifactId=java-project -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false
```
Isso irá baixar algumas dependências:
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
Ele também criará um arquivo de configuração `pom.xml`:

```
$ cd java-project
$ ls -lagh
total 16K
drwxr-xr-x  3 sudo 4.0K Sep  1 13:58 .
drwxr-xr-x 15 sudo 4.0K Sep  1 13:58 ..
-rw-r--r--  1 sudo  663 Sep  1 13:58 pom.xml
drwxr-xr-x  4 sudo 4.0K Sep  1 13:58 src
```

Para incluir o `JavaBitcoindRpcClient`, devemos adicionar nossa dependência a `<dependendencies>` no arquivo `pom.xml`.

```xml
      <dependency>
        <groupId>wf.bitcoin</groupId>
        <artifactId>bitcoin-rpc-client</artifactId>
        <version>1.2.1</version>
      </dependency>
```
Também precisamos adicionar propriedades do compilador para indicar qual versão do JDK compilará o código-fonte.  

```
  <properties>                                                                  
    <!-- https://maven.apache.org/general.html#encoding-warning -->
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>11</maven.compiler.source>
    <maven.compiler.target>11</maven.compiler.target>
  </properties>                                                                 
```

Sempre que adicionarmos um código-fonte às nossas classes, poderemos testá-lo com:
```
$ mvn compile
```
Também podemos executá-lo com `exec: java`
```
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
```

### Criando Projetos Alternativos

Se usarmos o [Gradle](https://gradle.org/releases/), poderemos  executar o seguinte comando:
```groovy
compile 'wf.bitcoin:JavaBitcoindRpcClient:1.2.1'
```

Se quisermos um projeto de amostra e algumas instruções sobre como executá-lo no servidor que acabamos de criar, podemos consultar o [Projeto de amostra do Bitcoind Java](https://github.com/bvolpato/bitcoind-java-client-sample). Também podemos navegar por todo o código de origem do [bitcoin-rpc-client](https://github.com/Polve/bitcoin-rpc-client).

## Construindo Nossa Conexão

Para usar o `JavaBitcoindRpcClient`, precisamos criar uma instância `BitcoindRpcClient`. Fazemos isso criando um URL com argumentos de nome de usuário, senha, endereço IP e porta. Como devemos nos lembrar, o endereço IP `127.0.0.1` e a porta `18332` devem estar corretos para a configuração testnet padrão descrita neste curso, enquanto podemos extrair o usuário e a senha abrindo o arquivo `~/.bitcoin/bitcoin.conf`.

```java
       BitcoindRpcClient rpcClient = new BitcoinJSONRPCClient("http://StandUp:6305f1b2dbb3bc5a16cd0f4aac7e1eba@localhost:18332");
```
Podemos observar que também precisaremos importar as informações apropriadas:
```
import wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient;
```

> **MAINNET VS TESTNET:** A porta seria 8332 para uma configuração da mainnet.

Se o `rpcClient` for inicializado com sucesso, poderemos enviar comandos RPC.

Mais tarde, quando terminarmos nossa conexão `bitcoind`, precisaremos fechá-la:
```
rpcClient.stop();
```

### Fazendo uma Chamada RPC

Você verá que o `BitcoindRpcClient` fornece a maioria das funcionalidades que podem ser acessadas através do `bitcoin-cli` ou outros métodos RPC, usando o mesmo método que o nome, mas em camelCase.

Por exemplo, para executar o comando `getmininginfo` para obter as informações do bloco e a dificuldade na rede, devemos usar o método `getMiningInfo()`:
```java
MiningInfo info = rpcClient.getMiningInfo();
System.out.println("Mining Information");
System.out.println("------------------");
System.out.println("Chain......: " + info.chain());
System.out.println("Blocks.....: " + info.blocks());
System.out.println("Difficulty.: " + info.difficulty());
System.out.println("Hash Power.: " + info.networkHashps());
```
O retorno para esta linha deve algo próximo a isso:
```
Mining Information
------------------
Chain......: test
Blocks.....: 1830905
Difficulty.: 4194304
Hash Power.: 40367401348837.41
```
### Fazendo uma Chamada RPC com Argumentos

Podemos procurar os endereços em nossa carteira passando o endereço como um argumento para o comando `getAddressInfo`:

```java
    String addr1 = "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo";
    
    AddressInfo addr1Info = rpcClient.getAddressInfo(addr1);
    System.out.println("Address: " + addr1Info.address());
    System.out.println("MasterFingerPrint: " + addr1Info.hdMasterFingerprint());
    System.out.println("HdKeyPath: " + addr1Info.hdKeyPath());
    System.out.println("PubKey: " + addr1Info.pubKey());
```
O resultado será mais ou menos assim:
```
Address: mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo
MasterFingerPrint: ce0c7e14
HdKeyPath: m/0'/0'/5'
PubKey: 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65
```

### Executando Nosso Código

O código para esses exemplos pode ser encontrado no [diretório src/](../src/18_2_App-getinfo.java) e deve ser instalado na estrutura de diretório padrão criada neste caso como `~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`. Ele pode então ser compilado e executado.

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
Você também verá muito mais informações sobre a compilação, é claro.

## Consultando Fundos

Recuperar o saldo de uma conta inteira é tão fácil quanto:
```
        System.out.println("Balance: " + rpcClient.getBalance());
```

## Criando um Endereço

Podemos criar um novo endereço em nossa carteira, anexando um rótulo específico a ela e até mesmo descartando nossa chave privada.

```java
  String address = rpcClient.getNewAddress("Learning-Bitcoin-from-the-Command-Line");
  System.out.println("New Address: " + address);

  String privKey = rpcClient.dumpPrivKey(address);
  System.out.println("Priv Key: " + privKey);
```
Retorno:
```
New Address: mpsFtZ8qTJPRGZy1gaaUw37fHeUSPLkzzs
Priv Key: cTy2AnmAALsHokYzJzTdsUBSqBtypmWfmSNYgG6qQH43euUZgqic
```

## Enviando uma Transação

A biblioteca `JavaBitcoindRpcClient` possui algumas ferramentas interessantes que facilitam a criação de uma transação desde o início.

### Criando uma Transação

Podemos criar uma transação bruta usando o método `createRawTransaction`, passando como argumentos dois objetos ArrayList contendo entradas e saídas a serem utilizadas.

Primeiro configuramos nossos novos endereços, aqui um endereço existente em nosso sistema e um novo endereço.
```
        String addr1 = "tb1qdqkc3430rexxlgnma6p7clly33s6jjgay5q8np";
        System.out.println("Used address addr1: " + addr1);

        String addr2 = rpcClient.getNewAddress();
        System.out.println("Created address addr2: " + addr2);
```
Então, podemos usar o RPC `listUnspent` para encontrar UTXOs para o endereço existente.
```
        List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
        System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```
Aqui está um resultado de todas as informações:
```java
System.out.println("Created address addr1: " + addr1);
String addr2 = rpcClient.getNewAddress();
System.out.println("Created address addr2: " + addr2);
List<String> generatedBlocksHashes = rpcClient.generateToAddress(110, addr1);
System.out.println("Generated " + generatedBlocksHashes.size() + " blocks for addr1");
List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```
As transações são criadas com o comando `BitcoinRawTxBuilder`:
```
        BitcoinRawTxBuilder txb = new BitcoinRawTxBuilder(rpcClient);
```
Primeiro preenchemos as entradas com os UTXOs que estamos gastando:
```
        TxInput in = utxos.get(0);
        txb.in(in);
```

> :warning: **AVISO:** Obviamente, em um programa real, selecionaríamos um UTXO de forma inteligente, porém, neste caso, pegamos apenas o enésimo, uma tática que usaremos ao longo deste capítulo.

Em segundo lugar, preenchemos as saídas, cada uma com um valor e um endereço:
```
    BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
        BigDecimal txToAddr2Amount = utxos.get(0).amount().subtract(estimatedFee);
    txb.out(addr2, txToAddr2Amount);
    
    System.out.println("unsignedRawTx in amount: " + utxos.get(0).amount());
        System.out.println("unsignedRawTx out amount: " + txToAddr2Amount);
```

Agora estamos prontos para realmente criar a transação:
```
    String unsignedRawTxHex = txb.create();
    System.out.println("Created unsignedRawTx from addr1 to addr2: " + unsignedRawTxHex);
```

### Assinando uma Transação

Agora podemos assinar a transação com o método `signRawTransactionWithKey`. Este método recebe como parâmetros uma transação de string bruta não assinada, a chave privada do endereço de envio e o objeto TxInput.

```java
    SignedRawTransaction srTx = rpcClient.signRawTransactionWithKey(
                    unsignedRawTxHex,
                    Arrays.asList(rpcClient.dumpPrivKey(addr1)), // 
                    Arrays.asList(in),
                    null);
    System.out.println("signedRawTx hex: " + srTx.hex());
    System.out.println("signedRawTx complete: " + srTx.complete());
```

### Enviando uma Transação

Finalmente, o envio requer o comando `sendRawTransaction`:
```java
String sentRawTransactionID = rpcClient.sendRawTransaction(srTx.hex());
System.out.println("Sent signedRawTx (txID): " + sentRawTransactionID);
```

### Executando Nosso Código

Agora podemos executar [o código da transação](../src/18_2_App-sendtx.java) como `~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`.

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

## Escutando Transações ou Blocos

Tal como acontece com [C e suas bibliotecas ZMQ](16_3_Receiving_Bitcoind_Notifications_with_C.md), existem maneiras fáceis de usar o Java para escutar a blockchain, além de executar o código específico quando algo acontece, como uma transação que envolve um endereço em nossa carteira, ou até a geração de um novo bloco na rede.

Para fazer isso, podemos usar a classe `BitcoinAcceptor` do `JavaBitcoindRpcClient`, que nos permite anexar "ouvintes" à rede.

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

Veja [o diretório src/](../src/18_2_App-listen.java) para o código completo. Cada vez que uma transação é enviada ou um novo bloco é gerado, devemos ver a saída em nosso console:
```
Transaction: {account=Tests, address=mhopuJzgmTwhGfpNLCJ9CRknugY691oXp1, category=receive, amount=5.0E-4, label=Tests, vout=1, confirmations=0, trusted=false, txid=361e8fcff243b74ebf396e595a007636654f67c3c7b55fd2860a3d37772155eb, walletconflicts=[], time=1513132887, timereceived=1513132887, bip125-replaceable=unknown}

Block: 000000004564adfee3738314549f7ca35d96c4da0afc6b232183917086b6d971
```

### Resumo: Acessando o Bitcoind com Java

Usando a biblioteca `javabitcoinrpc`, podemos acessar facilmente o `bitcoind` por meio de chamadas RPC com Java. Também teremos acesso a recursos adicionais interessantes, como o serviço de escuta usando o comando `bitcoinAcceptor`.

## O Que Vem Depois?

Vamos aprender mais sobre "Conversando com o Bitcoind com Outras Linguagens" na seção [§18.3: Acessando o Bitcoind com NodeJS](18_3_Accessing_Bitcoind_with_NodeJS.md).