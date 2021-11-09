# 18.1: Acessando o Bitcoind com Go

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta seção explica como interagir com o `bitcoind` usando a linguagem de programação Go e o [btcd rpcclient](https://github.com/btcsuite/btcd/tree/master/rpcclient). É importante observar que ele tem algumas peculiaridades e limitações.

## Configurando o Go

Para nos preparamos para o uso do Go em nossa máquina UNIX, primeiro precisamos instalar o curl, caso ainda não o tenhamos feito:

```
$ sudo apt install curl
```

Então, vamos ler a [página de downloads do Go](https://golang.org/dl/) para obtermos o link para o download mais recente e posteriormente fazer o download usando `curl`. Para uma configuração Debian, vamos querer usar a versão `linux-amd64`:

```
$ curl -O https://dl.google.com/go/go1.15.1.linux-amd64.tar.gz
```
Assim que terminarmos o download, comparamos o hash com o hash na [página de downloads do Go](https://golang.org/dl/):

```
$ sha256sum go1.15.1.linux-amd64.tar.gz 
70ac0dbf60a8ee9236f337ed0daa7a4c3b98f6186d4497826f68e97c0c0413f6  go1.15.1.linux-amd64.tar.gz
```

Os hashes devem corresponder. Em caso afirmativo, extraímos o tarball e instalamos o Go em nosso sistema:
```
$ tar xfv go1.15.1.linux-amd64.tar.gz 
$ sudo chown -R root:root ./go
$ sudo mv go /usr/local
```

Agora precisamos criar um caminho no Go para especificar nosso ambiente. Abra o arquivo `~ / .profile` com o editor de texto de sua escolha e adicione o seguinte ao final dele:

```
export GOPATH=$HOME/work
export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
```

Em seguida, atualizamos nosso perfil:

```
$ source ~/.profile
```

Por fim, vamos criar o diretório para o nosso espaço de trabalho Go:

```
$ mkdir $HOME/work
```

### Configurando o `btcd` e o `rpcclient`

Usaremos o `rpcclient` que advém do `btcd`, uma implementação Bitcoin escrita em Go. Embora o `rpcclient` tenha sido originalmente projetado para funcionar com o full node `btcd` do Bitcoin, ele também funciona com o Bitcoin Core. Ele tem algumas peculiaridades que veremos mais adiante.

Podemos usar o ```go get``` para baixá-lo:

```
$ go get github.com/btcsuite/btcd/rpcclient
```

Para testar seu funcionamento, vmaos navegar até o diretório com os exemplos do Bitcoin Core:

```
$ cd $GOPATH/src/github.com/btcsuite/btcd/rpcclient/examples/bitcoincorehttp
```

Vamos modificar o arquivo `main.go` e inserir os detalhes associados à configuração do Bitcoin Core, que pode ser encontrado no caminho `~ /.bitcoin/bitcoin.conf`:

```
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "6305f1b2dbb3bc5a16cd0f4aac7e1eba",
```
> **MAINNET VS TESTNET:** A porta seria 8332 para uma configuração na Mainnet.

Agora podemos executar um teste:
```
$ go run main.go
```

Devemos ver a contagem de blocos impressa:

```
2020/09/01 11:41:24 Block count: 1830861
```

### Criando um Projeto `rpcclient`

Normalmente, iríamos criar projetos no diretório `~/work/src/myproject/bitcoin`:
```
$ mkdir -p ~/work/src/myproject/bitcoin
$ cd ~/work/src/myproject/bitcoin
```
Cada projeto deve ter os seguintes `imports`:
```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
)
```
Esta declaração `import` permite que importemos bibliotecas relevantes. Para cada exemplo aqui, precisaremos importar `"log", "fmt"` e `"github.com/btcsuite/btcd/rpcclient"`. Podemos precisar importar bibliotecas adicionais em alguns exemplos.

   * O `log` é usado para mostrar mensagens de erro na tela. Após cada vez que o node Bitcoin for chamado, uma instrução `if` irá verificar se há algum erro. Se houver erros, o `log` é usado para imprimi-los;
   * O `fmt` é usado para imprimir a saída;
   * O `rpcclient` é obviamente a biblioteca do `rpcclient`;

## Construindo Nossa Conexão

Cada função do `bitcoind` no Go começa com a criação da conexão RPC, usando a função `ConnConfig`:
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
Os parâmetros da `connCfg` permitem que escolhamos a porta Bitcoin RPC, nome de usuário, senha e se estamos usando a testnet ou mainnet.

> **NOTA:** Novamente, precisamos nos certificar de substituir o `User` e o `Pass` com aqueles encontrados no nosso `~/.bitcoin/bitcon.conf`.

A função `rpcclient.New(connCfg, nil)` configura o`client` para nos conectarmos ao nosso node Bitcoin.

A linha `defer client.Shutdown()` é para desconectar do nosso node Bitcoin, uma vez que a função `main()` termina de ser executada. Após a linha `defer client.Shutdown()` é onde as coisas interessantes acontecem –– e será muito fácil de utilizar. Isso porque o `rpcclient` ajuda a transformar os comandos `bitcoin-cli` em funções, usando PascalCase. Por exemplo, `bitcoin-cli getblockcount` se transformará em `client.GetBlockCount` no Go.

### Fazendo uma Chamada RPC

Agora, tudo o que precisamos fazer é uma chamada informativa como `GetBlockCount` ou` GetBlockHash` usando nosso `client`:
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

### Fazendo uma Chamada RPC com Argumentos

As funções `rpcclient` também podem receber entradas, por exemplo, `client.GetBlockHash(blockCount)` recebe a contagem de blocos como uma entrada. O `client.GetBlockHash (blockCount)` de cima seria parecido com um comando `bitcoin-cli`:
```
$ bitcoin-cli getblockhash 1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```
No entanto, uma peculiaridade com hashes no `rpcclient` é que normalmente eles irão ser mostrados em uma codificação diferente se imprimirmos normalmente com o ` blockHash`. Para imprimi-los como uma string, precisamos usar o `blockHash.String()`.

### Executando Nosso Código

Podemos baixar o código completo do [diretório src](../src/18_1_blockinfo.go).

Podemos então, executar:
```
$ go run blockinfo.go 
1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```

O último número do bloco, junto com nosso hash, devem ser impressos.

## Procurando por Fundos

Devido às limitações do `btcd` no `rpcclient`, não podemos fazer uso da função `getwalletinfo`. No entanto, podemos usar o RPC `getbalance`:

```
	wallet, err := client.GetBalance("*")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
```
O `client.GetBalance("*")` requer a entrada `"*"` devido a uma peculiaridade do `btcd`. O asterisco significa que desejamos obter o saldo de todas as nossas carteiras.

Se executarmos [o código src](../src/18_1_getbalance.go), deveremos obter uma saída semelhante a esta:
```
$ go run getbalance.go 
0.000689 BTC
```

## Criando um Endereço

Podemos gerar endereços em Go, mas não podemos especificar o tipo do endereço:

Isso requer o uso de uma função especial `chaincfg`, para especificar para qual rede os endereços estão sendo criados. Esta especificação é necessária apenas durante a geração do endereço, por isso é usada apenas neste exemplo. Também podemos incluir isso nos demais exemplos, mas não é necessário.

Certifique-se de importar o `"github.com/btcsuite/btcd/chaincfg"`:
```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg"
)
```
Em seguida, vamos chamar o `connCfG` com o parâmetro `chaincfg.TestNet3Params.Name`:
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
> **MAINNET VS TESTNET:** O `Params: chaincfg.TestNet3Params.Name` deve ser `Params: chaincfg.MainNetParams.Name,` na Mainnet.

Podemos então criar nosso endereço:
```
	address, err := client.GetNewAddress("")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(address)
```
Uma peculiaridade com o `client.GetNewAddress("")` é que uma string vazia precisa ser incluída para que tudo funcione perfeitamente.

Executando o [código fonte](18_1_getaddress.go) teremos os seguintes resultados:
```
$ go run getaddress.go 
tb1qutkcj34pw0aq7n9wgp3ktmz780szlycwddfmza
```

### Decodificando um Endereço

Criar um endereço exigia um trabalho extra, em especificar a blockchain correta. Usar um endereço também exigirá, porque teremos que decodificá-lo antes de usá-lo.

Isso significa que teremos que importar as bibliotecas `"github.com/btcsuite/btcutil"` e `"github.com/btcsuite/btcd/chaincfg"`.

   * O `btcutil` permite que um endereço Bitcoin seja decodificado de uma forma que o `rpcclient` possa entender. Isso é necessário ao trabalhar com endereços no `rpcclient`;
   * O `chaincfg` é (novamente) usado para configurar nossa chain como a chain Testnet. Isso é necessário para a decodificação de endereços, pois os endereços usados na Mainnet e na Testnet são diferentes.

```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)
```
A variável defaultNet agora é usada para especificar se nosso node Bitcoin está na Testnet ou na Mainnet. Essa informação (e o objeto `btcutil`) é então usada para decodificar o endereço.

> **MAINNET VS TESTNET:** `&chaincfg.TestNet3Params` deve ser `& chaincfg.MainNetParams` na Mainnet.

```
	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
```

> **NOTA:** Precisamos alterar o endereço (`mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha`) para um que seja da nossa carteira. Podemos usar o  `bitcoin-cli listunspent` para encontrar alguns endereços com fundos para teste. Se quisermos ser realmente sofisticados, podemos modificar o código Go para obter um argumento e, em seguida, escrever um script que executa o `listunspent`, para depois salvarmos a informação em uma variável e executar o código Go nela.

Só depois disso usamos o RPC `getreceivedbyaddress` no nosso endereço decodificado:
```
	wallet, err := client.GetReceivedByAddress(addr)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
```
Ao executar [o código](../src/18_1_getamountreceived.go), devemos obter uma saída semelhante a esta:
```
$ go run getamountreceived.go 
0.0085 BTC
```

## Enviando uma Transação

Agora temos todas as peças do quebra-cabeça no lugar para enviar uma transação. Vamos querer:

1. Importar as bibliotecas corretas, incluindo a `chaincfg` para especificar uma rede e o `btcutil` para decodificar um endereço;
2. Escolher um endereço para enviar;
3. Decodificar esse endereço;
4. Executar o `sendtoaddress` para enviar fundos da maneira fácil.
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
Quando executamos [o código](../src/18_1_sendtransaction.go), o txid da transação nos será retornado:

```
$ go run sendtransaction.go
9aa4cd6559e0d69059eae142c35bfe78b71a8084e1fcc2c74e2a9675e9e7489d
```

### Consultando uma Transação

Para consultar uma transação, como a que acabamos de enviar, precisaremos fazer mais uma vez algumas conversões, desta vez do txid. O `"github.com/btcsuite/btcd/chaincfg/chainhash"` é importado para permitir que os hashes sejam armazenados no código Go. O `chainhash.NewHashFromStr("hash")` converte um hash em uma string para um formato que funciona com o rpcclient.

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
> **NOTA:** Novamente, vamos querer trocar o txid por um que realmente será reconhecido pelo nosso sistema.

Ao executar [o código](../src/18_1_lookuptransaction.go), ele imprimirá os detalhes associados a uma transação, como seu valor e quantas vezes foi confirmada:

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

## Resumo: Acessando o Bitcoind com Go

Embora o `btcd` e o `rpcclient` tenham alguns limites, ainda podemos executar os principais comandos RPC no Go. A documentação para o `rpcclient` está disponível no [Godoc](https://godoc.org/github.com/btcsuite/btcd/rpcclient). Se a documentação não tiver o que procuramos, podemos consultar também o [repositório btcd](https://github.com/btcsuite/btcd). Geralmente é bem documentado e fácil de ler. Com base nesses exemplos, devemos ser capazes de incorporar o Bitcoin em um projeto Go e fazer coisas como enviar e receber moedas.

## O Que Vem Depois?

Vamos aprender mais sobre "Conversando com o Bitcoind com Outras Linguagens" na seção [§18.2: Acessando o Bitcoin com Java](18_2_Accessing_Bitcoind_with_Java.md).