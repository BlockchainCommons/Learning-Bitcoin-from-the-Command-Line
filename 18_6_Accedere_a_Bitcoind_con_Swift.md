# 18.6: Accessing Bitcoind with Swift

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe ancora essere in attesa di revisione. Attenzione lettore.

Questa sezione spiega come interagire con `bitcoind` utilizzando il linguaggio di programmazione Swift e il proprio client RPC.

## Configurare Swift sul tuo Mac

Fino ad oggi, hai creato tutti i tuoi ambienti di sviluppo in linguaggi alternativi sul tuo nodo virtuale Debian. Tuttavia, quella non è la piattaforma migliore per Swift. Anche se esiste una versione di Swift disponibile per le piattaforme Ubuntu, non è completamente funzionale e funziona in modo leggermente diverso rispetto alla versione nativa di Swift per Mac. Una "variante" alla fine di questa sezione spiega come configurarla, ma attenzione, sarai in territorio inesplorato.

Invece, suggeriamo di creare un ambiente Swift ottimale su un Mac. Ci sono quattro passaggi principali per farlo.

### 1. Installa Xcode

Avrai bisogno di `Xcode`, l'ambiente di sviluppo integrato per Swift e Objective-C. Questo può essere facilmente installato andando al Mac App Store e selezionando `Get` per Xcode.

#### Alternativa: Installare Manualmente

Alcuni consigliano di evitare l'installazione dall'App Store perché è un po' tutto o niente; inoltre, non funzionerà se stai ancora utilizzando Mojave perché vuoi evitare le incompatibilità di Catalina. In tal caso, puoi scaricare direttamente dall'[Area Sviluppatori](https://developer.apple.com/download/more/) di Apple.

Se stai utilizzando Mojave, avrai bisogno del file `xip` per Xcode 10.3.1. Altrimenti, prendi l'ultima versione disponibile.

Una volta scaricato, puoi fare clic sul file `xip` per estrarlo, quindi spostare l'app Xcode nella tua cartella Applicazioni.

(In entrambi i casi, alla fine di questo passaggio dovresti avere Xcode installato nella tua cartella Applicazioni.)

### 2. Installa il Gordian Server

Avrai anche bisogno di un nodo Bitcoin sul tuo Mac, così da poter comunicare con esso. Tecnicamente, potresti usare un nodo remoto e accedervi con il login e la password RPC su internet. Tuttavia, suggeriamo invece di installare un nodo completo direttamente sul tuo Mac, perché è l'installazione più sicura e pulita, garantendo che nessuna delle tue comunicazioni lasci il tuo dispositivo.

Per installare facilmente un nodo completo sul tuo Mac, usa il [GordianServer per MacOS](https://github.com/BlockchainCommons/GordianServer-macOS) di Blockchain Commons. Vedi le [istruzioni di installazione](https://github.com/BlockchainCommons/GordianServer-macOS#installation-instructions) nel README, ma generalmente tutto quello che devi fare è scaricare il file `dmg` corrente, aprirlo e installare l'app nella tua directory Applicazioni.

Dopo, esegui l'App GordianServer e digli di `Start` Testnet.

> :link: **TESTNET vs. MAINNET:** O `Start` Mainnet.

#### 3. Rendi Accessibile il Tuo bitcoin-cli di Gordian

Quando desideri accedere al `bitcoin-cli` creato da GordianServer sul tuo Mac locale, puoi trovarlo in `~/.standup/BitcoinCore/bitcoin-VERSION/bin/bitcoin-cli`, ad esempio `~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli`.

Potresti voler creare un alias per questo:

```
alias bitcoin-cli="~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli -testnet"
```

> :link: **TESTNET vs. MAINNET:** Ovviamente, il parametro `-testnet` è richiesto solo se stai eseguendo su testnet.

### 4. Trova le Informazioni di GordianServer

Infine, avrai bisogno delle informazioni su `rpcuser` e `rpcpassword`. Questo si trova di default in `~/Library/Application Support/Bitcoin/bitcoin.conf` sotto Gordian.

```
$ grep rpc ~/Library/Application\ Support/Bitcoin/bitcoin.conf
rpcuser=oIjA53JC2u
rpcpassword=ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU
...
```

## Costruisci la Tua Connessione Manualmente

Al momento della scrittura, non esiste una libreria Bitcoin RPC aggiornata e facile da usare specifica per Swift, qualcosa che puoi semplicemente integrare e iniziare a usare immediatamente. Quindi, farai qualcosa che non hai mai fatto prima: costruire una connessione RPC manualmente.

### Scrivi il Trasmettitore RPC

Questo richiede solo la scrittura di una funzione che passa i comandi RPC a `bitcoind` nel formato corretto:

```
func makeCommand(method: String, param: Any, completionHandler: @escaping (Any?) -> Void) -> Void {
```

Le connessioni RPC a `bitcoind` usano il protocollo HTML, il che significa che devi fare tre cose: creare un URL; fare una URLRequest; e iniziare una URLSession.

#### 1. Crea un URL

All'interno della funzione, devi creare un URL dal tuo IP, porta, `rpcuser`, `rpcpassword` e wallet:

```
    let testnetRpcPort = "18332"
    let nodeIp = "127.0.0.1:\(testnetRpcPort)"
    let rpcusername = "oIjA53JC2u"
    let rpcpassword = "ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU"
    let walletName = ""
```

La connessione RPC effettiva a Bitcoin Core è costruita usando un URL del formato "http://rpcusername:rpcpassword@nodeIp/walletName":

```
    let walletUrl = "http://\(rpcusername):\(rpcpassword)@\(nodeIp)/\(walletName)"

    let url = URL(string: walletUrl)
```

Questo significa che le tue variabili di esempio risultano nel seguente URL:

```
http://oIjA53JC2u:ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU@127.0.0.1:18332/
```

Che dovrebbe assomigliare molto all'URL usato in alcune delle sezioni precedenti per le connessioni RPC.

#### 2. Crea una URLRequest

Con quell'URL in mano, puoi ora creare una URLRequest, con il metodo `POST` e il tipo di contenuto `text/plain`. Il corpo HTTP è poi il consueto oggetto JSON che hai inviato ogni volta che ti connetti direttamente alle porte RPC di Bitcoin Core, come dimostrato per la prima volta utilizzando Curl nel [Capitolo 4.4](04_4_Intermezzo_Usare_Curl.md).

```
    var request = URLRequest(url: url!)
    request.httpMethod = "POST"
    request.setValue("text/plain", forHTTPHeaderField: "Content-Type")
    request.httpBody = "{\"jsonrpc\":\"1.0\",\"id\":\"curltest\",\"method\":\"\(method)\",\"params\":[\(param)]}".data(using: .utf8)
```

#### 3. Crea una URLSession

Infine, sei pronto per costruire una URLSession attorno alla tua URLRequest.

```
    let session = URLSession(configuration: .default)
    let task = session.dataTask(with: request as URLRequest) { data, response, error in
```

Il completion handler per `dataTask` deve controllare gli errori:


```
        do {

            if error != nil {

                    //Handle the error
                    
            } else {
```


E poi analizzare i dati che stai ricevendo. Qui, stai estraendo i risultati JSON in un `NSDictionary`:


```
                if let urlContent = data {
                        
                    do {
                            
                        let json = try JSONSerialization.jsonObject(with: urlContent, options: JSONSerialization.ReadingOptions.mutableLeaves) as! NSDictionary
```


Dopo di che, ci sono ulteriori gestioni degli errori e poi puoi eventualmente restituire il `result` dictionary utilizzando il `completionHandler` che hai definito per la nuova funzione `makeCommand`:


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


Ovviamente, alla fine devi dire al `task` di iniziare:


```
    task.resume()
```

E questo è "tutto" quello che c'è da fare per realizzare quell'interazione RPC manualmente usando un linguaggio di programmazione come Swift.

> :pray: **GRAZIE:** Grazie a @Fonta1n3 che ha fornito il [codice principale](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues/137) per il nostro Trasmettitore RPC.

### Esegui una Chiamata RPC

Avendo scritto la funzione RPC `makeCommand`, puoi inviare una chiamata RPC eseguendola. Ecco `getblockchaininfo`:


```
let method = "getblockchaininfo"
let param = ""

makeCommand(method: method,param: param) { result in
    
    print(result!)

}
```

### Esegui una Chiamata RPC con Argomenti

Allo stesso modo potresti ottenere il numero del blocco corrente da quelle informazioni e usarlo per (ridondantemente) ottenere l'hash del blocco corrente, utilizzando il parametro `param`:

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

### Esegui il Codice

Il codice completo è disponibile nella [directory src](src/18_6_getinfo.playground). Caricalo nel tuo playground di Xcode e poi seleziona "Editor -> Run Playground" e dovresti ottenere risultati come:

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

## Controlla i Fondi

Con il tuo nuovo `makeCommand` per le funzioni RPC, puoi eseguire comandi simili a `getwalletinfo` o `getbalance`:

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

Che restituisce:

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

## Crea un Indirizzo

Creare un indirizzo è abbastanza semplice, ma che dire di creare un indirizzo legacy con una specifica etichetta? Questo richiede due parametri nella tua chiamata RPC.

Dato che la funzione `makeCommand` in questa sezione passa semplicemente i suoi `param` come il contenuto di un oggetto JSON, tutto ciò che devi fare è formattare correttamente quel contenuto. Ecco un modo per farlo:

```
method = "getnewaddress"
param = "\"learning-bitcoin\", \"legacy\""

makeCommand(method: method,param: param) { result in

    let address = result as! NSString
    print(address)
}
```

Eseguendo questo nel playground di Xcode ottieni un risultato:

```
mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF
```

Quel risultato è ovviamente un indirizzo Legacy; la sua etichetta può essere quindi controllata dalla riga di comando:

```
$ bitcoin-cli getaddressesbylabel "learning-bitcoin"
{
  "mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF": {
    "purpose": "receive"
  }
}
```

Successo!

> :information_source: **NOTA:** Come diciamo spesso in questi esempi di codifica, un programma del mondo reale sarebbe molto più sofisticato. In particolare, vorresti essere in grado di inviare un vero oggetto JSON come parametro, e poi avere il tuo programma `makeCommand` che lo analizza e lo immette nella URLSession in modo appropriato. Quello che abbiamo qui massimizza la leggibilità e la semplicità senza concentrarsi sulla facilità d'uso.

## Invia una Transazione

Come al solito, inviare una transazione (nel modo difficile) è un processo a più fasi:

0. Generare o ricevere un indirizzo di ricezione
1. Trovare un UTXO non speso
2. Creare una transazione raw
3. Firmare la transazione raw
4. Inviare la transazione raw

Userai l'indirizzo che hai generato nel passaggio precedente come destinatario.

### 1. Trova un UTXO Non Speso

Il comando `listunspent` RPC ti permette di trovare il tuo UTXO:

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

Come in altri esempi, prenderai arbitrariamente il primo UTXO, e estrarrai da esso il `txid`, `vout` e l'importo `amount`.

> :information_source **NOTA:** Ancora una volta, un programma reale sarebbe molto più sofisticato.

### 2. Crea una Transazione Raw

Creare una transazione raw è la cosa più difficile perché devi mettere a punto tutti i tuoi oggetti JSON, array e virgolette. Ecco come farlo in Swift, utilizzando la formattazione molto basilare dei parametri del trasmettitore:

```
        method = "createrawtransaction"
        param="[ { \"txid\": \"\(txid)\", \"vout\": \(vout) } ], { \"\(address)\": \(new_amount)}"
        makeCommand(method: method,param: param) { result in

            let hex = result as! NSString
```

### 3. Firma la Transazione Raw

Firmare la tua transazione richiede solo di eseguire il comando `signrawtransactionwithwallet` RPC, utilizzando il tuo nuovo `hex`:

```
            method = "signrawtransactionwithwallet"
            param = "\"\(hex)\""
           
            makeCommand(method: method,param: param) { result in

                let signedhexinfo = result as! NSDictionary
                let signedhex = signedhexinfo["hex"] as! NSString
```

### 4. Invia la Transazione Raw

Inviare la tua transazione è altrettanto semplice:

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

Il codice per questo invio di transazione può essere trovato nella [directory src](src/18_6_sendtx.playground).

## Utilizza Swift in Altri Modi

Questo copre le nostre solite discussioni sulla programmazione di Bitcoin RPC in un linguaggio, ma Swift è un linguaggio particolarmente importante poiché può essere utilizzato su dispositivi mobili, uno dei luoghi principali per i wallet. Pertanto, potresti considerare alcune altre librerie:

* Il framework [ios-Bitcoin](https://github.com/BlockchainCommons/iOS-Bitcoin) di Blockchain Commons converte la libreria Libbitcoin da C++ a Swift
* [Libwally Swift](https://github.com/blockchain/libwally-swift) è un wrapper Swift per Libwally

## Sommario: Accesso a Bitcoind con Swift

Swift è un linguaggio di programmazione moderno e robusto che purtroppo non ha ancora librerie RPC facili da usare ... il che ci ha appena dato l'opportunità di scrivere una funzione di accesso RPC da soli. Con questa a disposizione, puoi interagire con `bitcoind` su un Mac o costruire applicazioni companion su un iPhone, che è una combinazione perfetta per lavori Bitcoin airgapped.

## E ora?

Scopri Lightning in [Capitolo 19: Comprendere la Configurazione Lightning](19_0_Comprendere_la_Configurazione_Lightning.md).

## Variante: Distribuire Swift su Ubuntu

Se preferisci distribuire Swift su Ubuntu, puoi farlo, anche se la funzionalità non è la stessa. Alcuni dei codici in questo capitolo probabilmente genereranno errori che dovrai risolvere, e dovrai anche fare più lavoro per collegare le librerie C.

Per iniziare, installa alcune librerie Debian richieste:



```
$ sudo apt-get install clang
$ sudo apt-get install libcurl4 libpython2.7 libpython2.7-dev
```

Se stai usando Debian 10 o superiore (e dovresti farlo), dovrai anche aggiornare alcune librerie per ottenere versioni più vecchie:

```
$ sudo apt-get install libtinfo5 libncurses5
```

Dopo puoi scaricare e installare Swift:

```
$ wget https://swift.org/builds/swift-5.1.3-release/ubuntu1804/swift-5.1.3-RELEASE/swift-5.1.3-RELEASE-ubuntu18.04.tar.gz
$ tar xzfv swift-5.1.3-RELEASE-ubuntu18.04.tar.gz 
$ sudo mv swift-5.1.3-RELEASE-ubuntu18.04 /usr/share/swift
```

Per poter usare la tua nuova installazione di Swift, devi aggiornare il tuo `PATH` nel tuo `.bashrc`:

```
$ echo "export PATH=/usr/share/swift/usr/bin:$PATH" >> ~/.bashrc
$ source ~/.bashrc
```

Ora puoi testare Swift con l'argomento `--version`:

```
$ swift --version
Swift version 5.1.3 (swift-5.1.3-RELEASE)
Target: x86_64-unknown-linux-gnu
```

### Crea un Progetto
Una volta installato Swift sulla tua macchina Ubuntu, puoi creare progetti con il comando `package init`:

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

Modificherai quindi `Sources/.../main.swift` e quando sei pronto per compilare, puoi usare il comando `build`:

```
$ swift build
[4/4] Linking swift-project
```

Infine, sarai in grado di eseguire il programma dalla directory `.build/debug`:

```
$ .build/debug/swift-project
Hello, world!
```

Good luck!
