# 18.2: Accesso a Bitcoind con Java

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Questa sezione spiega come interagire con `bitcoind` utilizzando il linguaggio di programmazione Java e il [JavaBitcoindRpcClient](https://github.com/Polve/JavaBitcoindRpcClient).

## Configurare Java

Puoi installare Java sul tuo server, utilizzando il comando `apt-get`. Installerai anche [Apache Maven](http://maven.apache.org/) per gestire le dipendenze.

```
$ sudo apt-get install openjdk-11-jre-headless maven
```

Puoi verificare la tua installazione di Java:

```
$ java -version
openjdk version "11.0.8" 2020-07-14
OpenJDK Runtime Environment (build 11.0.8+10-post-Debian-1deb10u1)
OpenJDK 64-Bit Server VM (build 11.0.8+10-post-Debian-1deb10u1, mixed mode, sharing)
```

### Creare un Progetto Maven

Per programmare con Bitcoin utilizzando Java, creerai un progetto Maven:

```
$ mvn archetype:generate -DgroupId=com.blockchaincommons.lbtc -DartifactId=java-project -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false
```

Questo scaricherà alcune dipendenze

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

Creerà anche un file di configurazione `pom.xml`:

```
$ cd java-project
$ ls -lagh
total 16K
drwxr-xr-x  3 sudo 4.0K Sep  1 13:58 .
drwxr-xr-x 15 sudo 4.0K Sep  1 13:58 ..
-rw-r--r--  1 sudo  663 Sep  1 13:58 pom.xml
drwxr-xr-x  4 sudo 4.0K Sep  1 13:58 src
```

Per includere `JavaBitcoindRpcClient`, devi aggiungere la sua dipendenza a `<dependendencies>` nel file `pom.xml`

```
      <dependency>
        <groupId>wf.bitcoin</groupId>
        <artifactId>bitcoin-rpc-client</artifactId>
        <version>1.2.1</version>
      </dependency>
```

Devi anche aggiungere le proprietà del compilatore per indicare quale versione di JDK compilerà il codice sorgente.

```
$ cd cancellami sono una riga extra
   <properties>
    <!-- https://maven.apache.org/general.html#encoding-warning -->
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>11</maven.compiler.source>
    <maven.compiler.target>11</maven.compiler.target>
  </properties>
```

Ogni volta che aggiungi codice sorgente alle tue classi, potrai testarlo con:

```
$ mvn compile
```

Puoi anche eseguire con `exec:java`

```
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
```

### Creare Progetti Alternativi

Se utilizzi [Gradle]((https://gradle.org/releases/), puoi invece eseguire:


```
compile 'wf.bitcoin:JavaBitcoindRpcClient:1.2.1'
```

Se vuoi un progetto di esempio e alcune istruzioni su come eseguirlo sul server che hai appena creato, puoi fare riferimento al [Progetto di Esempio Bitcoind Java](https://github.com/brunocvcunha/bitcoind-java-client-sample/). Puoi anche sfogliare tutto il codice sorgente per bitcoin-rpc-client (https://github.com/Polve/bitcoin-rpc-client).

## Costruire la Tua Connessione

Per utilizzare `JavaBitcoindRpcClient`, devi creare un'istanza di `BitcoindRpcClient`. Questo si fa creando un URL con argomenti di username, password, indirizzo IP e porta. Come ricorderai, l'indirizzo IP `127.0.0.1` e la porta `18332` dovrebbero essere corretti per la configurazione standard di testnet descritta in questo corso, mentre puoi estrarre l'utente e la password da `~/.bitcoin/bitcoin.conf`.

```
       BitcoindRpcClient rpcClient = new BitcoinJSONRPCClient("http://StandUp:6305f1b2dbb3bc5a16cd0f4aac7e1eba@localhost:18332");
```

Nota che dovrai anche importare le informazioni appropriate:

```
import wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient;
```

> **MAINNET VS TESTNET:** La porta sarebbe 8332 per una configurazione mainnet.

Se `rpcClient` viene inizializzato con successo, potrai inviare comandi RPC.

In seguito, quando avrai finito con la tua connessione `bitcoind`, dovresti chiuderla:

```
rpcClient.stop();
```

### Effettuare una Chiamata RPC

Scoprirai che `BitcoindRpcClient` fornisce la maggior parte delle funzionalità accessibili tramite `bitcoin-cli` o altri metodi RPC, utilizzando gli stessi nomi dei metodi, ma in camelCase.

Ad esempio, per eseguire il comando `getmininginfo` per ottenere le informazioni sul blocco e la difficoltà della rete, dovresti utilizzare il metodo `getMiningInfo()`:

```
MiningInfo info = rpcClient.getMiningInfo();
System.out.println("Mining Information");
System.out.println("------------------");
System.out.println("Chain......: " + info.chain());
System.out.println("Blocks.....: " + info.blocks());
System.out.println("Difficulty.: " + info.difficulty());
System.out.println("Hash Power.: " + info.networkHashps());
```

L'output per questa linea dovrebbe essere simile a questo:

```
Mining Information
------------------
Chain......: test
Blocks.....: 1830905
Difficulty.: 4194304
Hash Power.: 40367401348837.41
```

### Make an RPC Call with Arguments

You can look up addresses on your wallet by passing the address as an argument to `getAddressInfo`:

```
	String addr1 = "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo";
	AddressInfo addr1Info = rpcClient.getAddressInfo(addr1);
	System.out.println("Address: " + addr1Info.address());
	System.out.println("MasterFingerPrint: " + addr1Info.hdMasterFingerprint());
	System.out.println("HdKeyPath: " + addr1Info.hdKeyPath());
	System.out.println("PubKey: " + addr1Info.pubKey());
```

L'output sarà qualcosa del genere:

```
Address: mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo
MasterFingerPrint: ce0c7e14
HdKeyPath: m/0'/0'/5'
PubKey: 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65
```

### Eseguire il Codice

Il codice per questi esempi può essere trovato nella [directory src](src/18_2_App-getinfo.java) e dovrebbe essere installato nella struttura di directory standard creata qui come 
`~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`. Può quindi essere compilato ed eseguito.


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

(Vedrai anche molte altre informazioni sulla compilazione, naturalmente.)

## Cercare Fondi

Recuperare il saldo per un intero account è altrettanto semplice:


```
        System.out.println("Balance: " + rpcClient.getBalance());
```

## Creare un Indirizzo

Puoi creare un nuovo indirizzo sul tuo wallet, assegnargli una specifica etichetta e persino esportare la sua chiave privata.

```
  String address = rpcClient.getNewAddress("Learning-Bitcoin-from-the-Command-Line");
  System.out.println("New Address: " + address);

  String privKey = rpcClient.dumpPrivKey(address);
  System.out.println("Priv Key: " + privKey);
```

Output:

```
New Address: mpsFtZ8qTJPRGZy1gaaUw37fHeUSPLkzzs
Priv Key: cTy2AnmAALsHokYzJzTdsUBSqBtypmWfmSNYgG6qQH43euUZgqic
```

## Inviare una Transazione

La libreria JavaBitcoindRpcClient ha alcuni buoni strumenti che rendono facile creare una transazione da zero.

### Creare una Transazione

Puoi creare una transazione grezza utilizzando il metodo `createRawTransaction`, passando come argomenti due oggetti ArrayList contenenti gli input e gli output da utilizzare.

Prima imposti i tuoi nuovi indirizzi, qui un indirizzo esistente sul tuo sistema e un nuovo indirizzo sul tuo sistema.

```
        String addr1 = "tb1qdqkc3430rexxlgnma6p7clly33s6jjgay5q8np";
        System.out.println("Used address addr1: " + addr1);

        String addr2 = rpcClient.getNewAddress();
        System.out.println("Created address addr2: " + addr2);
```

Poi puoi utilizzare l'RPC `listUnspent` per trovare i UTXO per l'indirizzo esistente.


```
        List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
        System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```

Ecco un output di tutte le informazioni:

```
System.out.println("Created address addr1: " + addr1);
String addr2 = rpcClient.getNewAddress();
System.out.println("Created address addr2: " + addr2);
List<String> generatedBlocksHashes = rpcClient.generateToAddress(110, addr1);
System.out.println("Generated " + generatedBlocksHashes.size() + " blocks for addr1");
List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```
Le transazioni sono costruite con `BitcoinRawTxBuilder`:

```
        BitcoinRawTxBuilder txb = new BitcoinRawTxBuilder(rpcClient);
```

Per prima cosa riempi gli input con i UTXO che stai spendendo:

```
        TxInput in = utxos.get(0);
        txb.in(in);
```

> :warning: **ATTENZIONE:** Ovviamente in un vero programma selezioneresti intelligentemente un UTXO; qui, prendiamo solo il primo, una tattica che utilizzeremo in tutto questo capitolo.

In secondo luogo, riempi gli output ciascuno con un importo e un indirizzo:

```
	BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
      	BigDecimal txToAddr2Amount = utxos.get(0).amount().subtract(estimatedFee);
	txb.out(addr2, txToAddr2Amount);
	System.out.println("unsignedRawTx in amount: " + utxos.get(0).amount());
        System.out.println("unsignedRawTx out amount: " + txToAddr2Amount);
```

Ora sei pronto a creare effettivamente la transazione:

```
	String unsignedRawTxHex = txb.create();
	System.out.println("Created unsignedRawTx from addr1 to addr2: " + unsignedRawTxHex);
```

### Firmare una Transazione

Ora puoi firmare la transazione con il metodo `signRawTransactionWithKey`. Questo metodo riceve come parametri una stringa di transazione grezza non firmata, la chiave privata dell'indirizzo mittente e l'oggetto TxInput.


```
	SignedRawTransaction srTx = rpcClient.signRawTransactionWithKey(
					unsignedRawTxHex,
					Arrays.asList(rpcClient.dumpPrivKey(addr1)), //
					Arrays.asList(in),
					null);
	System.out.println("signedRawTx hex: " + srTx.hex());
	System.out.println("signedRawTx complete: " + srTx.complete());
```

### Inviare una Transazione

Infine, inviare richiede il comando `sendRawTransaction`:

```
String sentRawTransactionID = rpcClient.sendRawTransaction(srTx.hex());
System.out.println("Sent signedRawTx (txID): " + sentRawTransactionID);
```

### Eseguire il Codice

Ora puoi eseguire [il codice della transazione](src/18_2_App-sendtx.java) come `~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`.


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

## Ascoltare Transazioni o Blocchi

Come con [C e le sue librerie ZMQ](16_3_Ricevere_Notifiche_di_Bitcoind_in_C_tramite_Librerie_ZMQ.md), ci sono modi semplici per utilizzare Java per ascoltare la blockchain e per eseguire codice specifico quando accade qualcosa, come una transazione che coinvolge un indirizzo nel tuo wallet, o anche la generazione di un nuovo blocco nella rete.

Per fare ciò, utilizza la classe `BitcoinAcceptor` di `JavaBitcoindRpcClient`, che ti permette di collegare listener nella rete.


```
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


Consulta la [directory src](src/18_2_App-listen.java) per il codice completo. Ogni volta che viene inviata una transazione o generato un nuovo blocco, dovresti vedere un output sulla tua console:


```
Transaction: {account=Tests, address=mhopuJzgmTwhGfpNLCJ9CRknugY691oXp1, category=receive, amount=5.0E-4, label=Tests, vout=1, confirmations=0, trusted=false, txid=361e8fcff243b74ebf396e595a007636654f67c3c7b55fd2860a3d37772155eb, walletconflicts=[], time=1513132887, timereceived=1513132887, bip125-replaceable=unknown}
Block: 000000004564adfee3738314549f7ca35d96c4da0afc6b232183917086b6d971
```


### Sommario Accesso a Bitcoind con Java

Utilizzando la libreria javabitcoinrpc, puoi facilmente accedere a bitcoind tramite chiamate RPC da Java. Avrai anche accesso a funzionalità aggiuntive interessanti, come il servizio di ascolto `bitcoinAcceptor`.

## Cosa c'è Dopo?

Scopri di più su "Parlare con Bitcoin in Altri Linguaggi" nel [Capitolo 18.3: Accedere a Bitcoind con NodeJS](18_3_Accedere_a_Bitcoind_con_NodeJS.md).


