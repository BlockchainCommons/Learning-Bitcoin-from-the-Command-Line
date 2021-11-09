# 18.6: Acessando o Bitcoind com Swift

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta seção explica como interagir com o `bitcoind` usando a linguagem de programação Swift e seu próprio cliente RPC.

## Configurando o Swift no Mac

Até o momento, construímos todos os nossos ambientes de desenvolvimento de linguagem de programação alternativa em nosso node virtual no Debian. No entanto, essa não é a melhor plataforma para o Swift. Embora haja uma versão do Swift disponível para plataformas Ubuntu, ela não possui todos os recursos e funciona de maneira um pouco diferente do Swift nativo do Mac. Uma "variante" no final desta seção explica como configurá-lo, mas esteja avisado de que estaremos em um território desconhecido.

Ao invés disso, sugerimos a criação de um ambiente Swift ideal em um Mac. Existem quatro etapas principais para fazer isso.

### 1. Instalando o Xcode

Vamos precisar do `Xcode`, o ambiente de desenvolvimento integrado para o Swift e para o Objective-C. Isso pode ser facilmente instalado acessando a Mac App Store e baixando o Xcode.

#### Alternativa: Instalando Manualmente

Algumas pessoas desaconselham a instalação da App Store porque é tudo ou nada. Também não funcionará se estivermos usando o Mojave, pois desejaremos evitar as incompatibilidades do Catalina. Nesse caso, podemos fazer o download diretamente da [Área do desenvolvedor](https://developer.apple.com/download/more/) na Apple.

Se estivermos usando o Mojave, precisaremos do arquivo `xip` para o Xcode 10.3.1. Caso contrário, podemos utilizar o mais recente.

Depois de baixado, podemos clicar no `xip` para extraí-lo e mover o aplicativo Xcode para a pasta de Aplicativos.

De qualquer forma, devemos ter o Xcode instalado na pasta de Aplicativos no final desta etapa.

### 2. Instalando o Servidor Gordian

Também vamos precisar de um node Bitcoin em nosso Mac, para que possamos nos comunicar com ele. Tecnicamente, poderíamos usar um node remoto e acessá-lo com o login e senha RPC pela rede. No entanto, sugerimos a instalação do full node diretamente no Mac, porque essa é a configuração mais segura e limpa, garantindo que nenhuma das comunicações saia de nossa máquina.

Para instalar facilmente um full node em nosso Mac, podemos usar o [GordianServer for MacOS](https://github.com/BlockchainCommons/GordianServer-macOS) da Blockchain Commons. Podemos seguir as [instruções de instalação](https://github.com/BlockchainCommons/GordianServer-macOS#installation-instructions) no README, mas geralmente tudo que precisamos fazer é baixar o arquivo `dmg` atual, abri-lo e instalar a aplicação em nosso diretório de aplicações.

Depois, vamos executar o aplicativo GordianServer e dizer a ele para `Start` na Testnet.

> :link: **TESTNET vs. MAINNET:** Ou `Start` na Mainnet.

#### 3. Tornando Nosso bitcoin-cli Gordian Acessível

Quando desejarmos acessar o `bitcoin-cli` criado pelo GordianServer em nosso Mac local, podemos encontrá-lo em `~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli`.

Podemos criar um alias para isso:
```
alias bitcoin-cli="~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli -testnet"
```

> :link: **TESTNET vs. MAINNET:** Obviamente, o parâmetro `-testnet` só é necessário se estivermos rodando na testnet.

### 4. Encontrando Nossas Informações do GordianServer

Finalmente, precisaremos de nossas informações de `rpcuser` e `rpcpassword`. Essas informações estarão em `~/Library/Application Support/Bitcoin/bitcoin.conf` por padrão no Gordian.
```
$ grep rpc ~/Library/Application\ Support/Bitcoin/bitcoin.conf
rpcuser=oIjA53JC2u
rpcpassword=ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU
...
```
## Construindo Nossa Conexão Manualmente

No momento em que este artigo foi escrito, não havia uma biblioteca Bitcoin RPC atualizada e simples de ser usada, que fosse específica para Swift, algo que pudéssemos baixar e começar a usar imediatamente. Portanto, faremos algo que nunca fizemos antes: construir uma conexão RPC manualmente.

### Grave o Transmissor RPC

Isso requer apenas a escrita de uma função que passe os comandos RPC para o `bitcoind` no formato correto:
```
func makeCommand(method: String, param: Any, completionHandler: @escaping (Any?) -> Void) -> Void {
```

As conexões RPC para o `bitcoind` usam o protocolo HTML, o que significa que precisamos fazer três coisas: Criar uma URL, fazer um URLRequest e iniciar uma URLSession.

#### 1. Criando uma URL

Dentro da função, precisamos criar uma URL a partir do nosso IP, porta, `rpcuser`, `rpcpassword` e wallet:
```
    let testnetRpcPort = "18332"
    let nodeIp = "127.0.0.1:\(testnetRpcPort)"
    let rpcusername = "oIjA53JC2u"
    let rpcpassword = "ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU"
    let walletName = ""
```
A conexão RPC real com o Bitcoin Core é construída usando uma URL no formato "http://rpcusername:rpcpassword@nodeIp/walletName":
```
    let walletUrl = "http://\(rpcusername):\(rpcpassword)@\(nodeIp)/\(walletName)"

    let url = URL(string: walletUrl)
```
Isso significa que nossas variáveis ​​de amostra resultam na seguinte URL:
```
http://oIjA53JC2u:ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU@127.0.0.1:18332/
```
Que deve se parecer muito com a URL usada em algumas das seções anteriores para conexões RPC.

#### 2. Criando uma URLRequest

Com essa URL em mãos, agora podemos criar um URLRequest, com o método `POST` e o tipo de conteúdo `text/plain`. O corpo HTTP será então o objeto JSON familiar que enviamos sempre que nos conectamos diretamente às portas RPC do Bitcoin Core, conforme demonstrado pela primeira vez ao usar o Curl na seção [§4.4](04_4__Interlude_Using_Curl.md).
```
    var request = URLRequest(url: url!)
    request.httpMethod = "POST"
    request.setValue("text/plain", forHTTPHeaderField: "Content-Type")
    request.httpBody = "{\"jsonrpc\":\"1.0\",\"id\":\"curltest\",\"method\":\"\(method)\",\"params\":[\(param)]}".data(using: .utf8)
```

#### 3. Criando uma URLSession

Finalmente, estamos prontos para construir uma URLSession em torno da nossa URLRequest.
```
    let session = URLSession(configuration: .default)
    let task = session.dataTask(with: request as URLRequest) { data, response, error in
```
O manipulador de conclusão para `dataTask` precisa verificar se há erros:
```
        do {

            if error != nil {

                    //Handle the error
                    
            } else {
```
E então analisar os dados que estamos recebendo. Aqui, estamos pegando os resultados JSON e colocando-os em um `NSDictionary`:
```
                if let urlContent = data {
                        
                    do {
                            
                        let json = try JSONSerialization.jsonObject(with: urlContent, options: JSONSerialization.ReadingOptions.mutableLeaves) as! NSDictionary
```
Depois disso, há mais manipulação de erros e então podemos eventualmente retornar o `result` do dicionário usando o `CompletHandler` que definimos para a nova função `makeCommand`:
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
Claro que eventualmente teremos que dizer à `task` para que ela seja iniciada:
```
    task.resume()
```
E isso é "tudo" o que precisamos fazer nessa interação RPC manual, usando uma linguagem de programação como o Swift.

> :pray: **AGRADECIMENTO:** Obrigado @Fonta1n3, que forneceu o [código principal](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues/137) para nosso Transmissor RPC.

### Fazendo uma Chamada RPC

Tendo escrito a função RPC `makeCommand`, podemos enviar uma chamada RPC executando-a. Aqui está `getblockchaininfo`:
```
let method = "getblockchaininfo"
let param = ""

makeCommand(method: method,param: param) { result in
    
    print(result!)

}
```
### Fazendo uma Chamada RPC com Argumentos

Poderíamos da mesma forma obter a contagem de bloco atual a partir dessa informação e usá-la de modo redundante para obter o hash do bloco atual, usando o parâmetro `param`:
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

### Executando Nosso Código

O código completo está disponível no [diretório src/](../src/18_6_getinfo.playground). Carregue-o no playground Xcode e, em seguida, "Editor -> Executar Playground" e devemos obter resultados como:
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
## Procurando por Fundos

Com nosso novo `makeCommand` para funções RPC, podemos executar um comando como `getwalletinfo` ou `getbalance`:
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
Que irá retornar:
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
## Criando um Endereço

Criar um endereço é bastante simples, mas que tal criar um endereço legado com um rótulo específico? Isso requer dois parâmetros na nossa chamada RPC.

Uma vez que a simples função `makeCommand` desta seção apenas passa nossos `param`s como as entranhas de um objeto JSON, tudo o que precisamos fazer é formatar corretamente essas entranhas. Aqui está uma maneira para fazermos isso:
```
method = "getnewaddress"
param = "\"learning-bitcoin\", \"legacy\""

makeCommand(method: method,param: param) { result in

    let address = result as! NSString
    print(address)
}
```
Executá-lo no playground do Xcode produz o seguinte resultado:
```
mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF
```
Esse resultado é obviamente um endereço legado; seu rótulo pode então ser verificado na linha de comando:
```
$ bitcoin-cli getaddressesbylabel "learning-bitcoin"
{
  "mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF": {
    "purpose": "receive"
  }
}
```
Sucesso!

> :information_source: **NOTA:** Como costumamos dizer nesses exemplos de programação, um programa do mundo real seria muito mais sofisticado. Em particular, gostaríamos de poder enviar um objeto JSON real como um parâmetro e, em seguida, ter no nosso programa um `makeCommand` analisando-o e inserindo-o na URLSession de forma adequada. O que temos aqui maximiza a legibilidade e a simplicidade sem enfocar na facilidade de uso.

## Enviando uma Transação

Como de costume, o envio de uma transação (da maneira mais difícil) é um processo de várias etapas:

0. Gerar ou receber um endereço de recebimento;
1. Encontrar um UTXO não gasto;
2. Criar uma transação bruta;
3. Assinar a transação bruta;
4. Enviar a transação bruta.

Usando o `address` gerado na etapa anterior como nosso destinatário.

### 1. Encontrando um UTXO Não Gasto

O RPC `listunspent` permite que encontremos nosso UTXO:
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
Como em outros exemplos, vamos arbitrariamente pegar o enésimo UTXO e pegar o `txid`, `vout` e `amount` dele.

> :information_source **NOTA:** Mais uma vez, um programa real seria muito mais sofisticado.

### 2. Criando uma Transação Bruta

Criar uma transação bruta é a coisa mais complicada, porque precisamos acertar todos os nossos objetos JSON, arrays e aspas. Veja como fazer isso no Swift, usando a formatação `param` muito básica do transmissor:
```
        method = "createrawtransaction"
        param="[ { \"txid\": \"\(txid)\", \"vout\": \(vout) } ], { \"\(address)\": \(new_amount)}"
        makeCommand(method: method,param: param) { result in

            let hex = result as! NSString
```
### 3. Assinando a Transação Bruta

Assinar nossa transação requer apenas que executemos o RPC  `signrawtransactionwithwallet`, usando nosso novo `hex`:
```
            method = "signrawtransactionwithwallet"
            param = "\"\(hex)\""
            
            makeCommand(method: method,param: param) { result in

                let signedhexinfo = result as! NSDictionary
                let signedhex = signedhexinfo["hex"] as! NSString
```

### 4. Enviando a Transação Bruta

Enviar nossa transação é igualmente simples:
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

O código para este remetente de transação pode ser encontrado no [diretório src/](../src/18_6_sendtx.playground).

## Usando o Swift de Outras Maneiras

Isso cobre nossas discussões habituais sobre a programação do Bitcoin RPC usando uma linguagem, mas o Swift é uma linguagem particularmente importante, pois pode ser implantada em dispositivos móveis, um dos principais locais para nossas carteiras. Como tal, podemos querer considerar algumas outras bibliotecas:

* O [framework ios-Bitcoin](https://github.com/BlockchainCommons/iOS-Bitcoin) da Blockchain Commons converte a biblioteca Libbitcoin de C++ para Swift;
* [Libwally Swift](https://github.com/blockchain/libwally-swift) é um wrapper Swift para a Libwally.

## Resumo: Acessando o Bitcoind com Swift

O Swift é uma linguagem de programação robusta e moderna que infelizmente ainda não tem nenhuma biblioteca RPC fácil de ser utilizada... o que acabou de nos dar a oportunidade de escrever uma função de acesso ao RPC. Com isso em mãos, podemos interagir com o `bitcoind` em um Mac ou criar aplicativos complementares em um iPhone, o que é uma combinação perfeita para usar o Bitcoin com airgap.

## O Que Vem Depois?

Aprenda sobre o Lightning em [Capítulo 19: Compreendendo Nossa Configuração da Lightning](19_0_Understanding_Your_Lightning_Setup.md).

## Variante: Implantando o Swift no Ubuntu

Se preferirmos implantar o Swift no Ubuntu, podemos fazê-lo, embora a funcionalidade não seja a mesma. Parte do código neste capítulo provavelmente gerará erros que precisaremos resolver e também precisaremos trabalhar mais para vincular as bibliotecas C.

Para começar, vamos instalar algumas bibliotecas Debian necessárias:
```
$ sudo apt-get install clang
$ sudo apt-get install libcurl4 libpython2.7 libpython2.7-dev
```
Se estivermos usando o Debian 10 ou superior (e realmente deveríamos estar usando), também precisaremos retroagir algumas bibliotecas para obter as versões mais antigas:
```
$ sudo apt-get install libtinfo5 libncurses5
```
Posteriormente, podemos baixar e instalar o Swift:
```
$ wget https://swift.org/builds/swift-5.1.3-release/ubuntu1804/swift-5.1.3-RELEASE/swift-5.1.3-RELEASE-ubuntu18.04.tar.gz
$ tar xzfv swift-5.1.3-RELEASE-ubuntu18.04.tar.gz 
$ sudo mv swift-5.1.3-RELEASE-ubuntu18.04 /usr/share/swift
```
Para poder usar nossa nova configuração do Swift, precisaremos atualizar nosso `PATH` em nosso arquivo `.bashrc`:
```
$ echo "export PATH=/usr/share/swift/usr/bin:$PATH" >> ~/.bashrc
$ source ~/.bashrc
```
Agora podemos testar o Swift com o argumento `--version`:
```
$ swift --version
Swift version 5.1.3 (swift-5.1.3-RELEASE)
Target: x86_64-unknown-linux-gnu
```

### Criando um Projeto
Depois de instalar o Swift em nossa máquina Ubuntu, podemos criar projetos com o comando `package init`:
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
Também editaremos o `Sources/.../main.swift` e, quando estivermos pronto para compilar, podemos usar o comando `build`:
```
$ swift build
[4/4] Linking swift-project
```
Finalmente, seremos capazes de executar o programa a partir do diretório `.build/debug`:
```
$ .build/debug/swift-project
Hello, world!
```
Boa sorte!