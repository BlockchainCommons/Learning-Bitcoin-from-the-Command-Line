# Prefácio: Usando Curl

O ```bitcoin-cli``` é, em última análise, apenas um invólucro. É uma forma de interagir com ```bitcoind``` a partir da linha de comando, fornecendo acesso simplificado aos seus diversos comandos RPC. Mas o RPC pode, é claro, ser acessado diretamente. É disso que iremos falar nessa seção: como nos conectarmos diretamente ao RPC com o comando ```curl```.

Não será muito usado nos próximos capítulos, mas essas informações serão importantes caso queiramos uma  alternativa para acessar o ```bitcoind```.

## Conhecendo o Curl

O ```curl```, abreviação de "ver URL" ("see URL" em inglês), é uma ferramenta que nos permite acessar URLs diretamente pela linha de comando. É uma maneira fácil de interagir com servidores como o ```bitcoind```, que ficam ouvindo as portas da internet e conversam utilizando uma variedade de protocolos. O Curl também está disponível como uma biblioteca para muitas linguagens de programação, como C, Java, PHP e Python. Então, depois de saber como trabalhar com o Curl, teremos uma base sólida para usar várias APIs diferentes.

Para usar o ```curl``` com o ```bitcoind```, devemos saber três coisas: o formato padrão, o nome de usuário e senha e a porta correta.

### Conhecendo o Formato

Os comandos ```bitcoin-cli``` estão todos vinculados aos comandos RPC no ```bitcoind```. Isso torna a transição do uso de ```bitcoin-cli``` para o uso do ```curl``` muito simples. Na verdade, se olharmos qualquer uma das páginas de ajuda do ```bitcoin-cli```, veremos que eles listam não apenas os comandos ```bitcoin-cli```, mas também os comandos ```curl``` paralelos. Por exemplo, aqui temos o resultado do comando ```bitcoin-cli help getmininginfo```:
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
E ali está o comando ```curl```, no final da tela de ajuda! Este comando um tanto longo tem quatro partes principais: (1) uma lista do nome de usuário; (2) um sinalizador ```--data-binary```; (3) um objeto JSON que diz ao ```bitcoind``` o que fazer, incluindo um array JSON com os parâmetros e; (4) um cabeçalho HTTP que inclui a URL do ```bitcoind```.

Quando estamos trabalhando com o ```curl```, muitos desses argumentos do ```curl``` permanecerão os mesmos de um comando para outro, apenas as entradas ```method``` e ```params``` no array JSON normalmente mudam. No entanto, precisaremos saber como preencher o nome do usuário e endereço da URL para que funcione, antes de mais nada!

_Sempre que não tivermos certeza sobre como usar o curl no RPC, basta usarmos a ajuda do bitcoin-cli e continuar._

### Descobrindo o Nome de Usuário

Para falar com a porta do ```bitcoind```, precisamos de um nome de usuário e senha. Eles foram criados como parte da configuração inicial do Bitcoin e podem ser encontrados no arquivo `~/.bitcoin/bitcoin.conf`.

Por exemplo, aqui está nossa configuração atual:
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
Nosso nome de usuário é ```StandUp``` e nossa senha é ```8eaf562eaf45c33c3328bc66008f2dd1```.

> **ATENÇÃO:** Obviamente, não é muito seguro ter essas informações em um arquivo de texto simples. A partir do Bitcoin Core 0.12, podemos omitir o ```rpcpassword``` do arquivo ```bitcoin.conf``` e fazer com que o ```bitcoind``` gere um novo cookie sempre que iniciarmos o serviço. A desvantagem disso é que torna o uso de comandos RPC por outros aplicativos, como os detalhados neste capítulo, mais difícil. Então, vamos ficar com as informações simples do ```rpcuser``` e ```rpcpassword``` por enquanto, mas para softwares em produção, é importante considerarmos essa alteração para cookies.

A maneira segura de usar o RPC com ```bitcoind``` é a seguinte:
```
$ curl --user StandUp --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
Enter host password for user 'bitcoinrpc':
```
Conforme observado, nossa senha será solicitada.

> :link: **TESTNET vs MAINNET:** A Testnet usa uma URL com a porta 18332 e a mainnet usa uma URL com a porta 8332. Se tivermos alguma dúvida, basta olharmos nosso ```bitcoin.conf```. As configurações estão todas lá.

A maneira insegura e errada de fazer isso é a seguinte:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
```
> **ATENÇÃO:** Digitar a senha na linha de comando pode colocá-la na tabela de processos e/ou salvá-la em um histórico qualquer. Isso é ainda menos recomendado do que colocá-la em um arquivo, exceto para testes utilizando a testnet. Se quisermos fazer em qualquer outro lugar, precisamos nos certificar de saber o que estamos fazendo!

### Conhecendo os Comandos e os Parâmetros

Com tudo isso em mãos, estamos prontos para enviar os comandos RPC padrão com o ```curl```, mas ainda precisamos saber como incorporar os dois elementos que tendem a mudar no comando ```curl```.

O primeiro é o ```method```, que é o método RPC que está sendo utilizado. Isso geralmente deve corresponder aos nomes de comando que alimentamos no ```bitcoin-cli``` por muito tempo.

O segundo é o ```params```, que é uma array JSON de parâmetros. Eles são iguais aos argumentos (ou argumentos nomeados) que estamos usando. Eles também são a parte mais confusa do ```curl```, em grande parte porque são um array estruturado ao invés de uma simples lista.

Esta é a aparência de alguns arrays de parâmetros:

  * `[]` — Um array vazio;
  * `["000b4430a7a2ba60891b01b718747eaf9665cb93fbc0c619c99419b5b5cf3ad2"]` — Um array com dados;
  * `["'$signedhex'"]` — Um array com uma variável;
  * `[6, 9999999]` — Uma array com dois parâmetros;
  * `{}` - Um objeto vazio;
  * `[''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.298, "'$changeaddress'": 1.0}'']` — Um array com um array contendo um objeto e um objeto vazio.

## Obtendo Informação

Agora podemos enviar nosso primeiro comando ```curl``` acessando o RPC ```getmininginfo```:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
{"result":{"blocks":1772428,"difficulty":10178811.40698772,"networkhashps":91963587385939.06,"pooledtx":61,"chain":"test","warnings":"Warning: unknown new rules activated (versionbit 28)"},"error":null,"id":"curltest"}```
Note that we provided the method, `getmininginfo`, and the parameter, `[]`, but that everything else was the standard `curl` command line.
```
> **ATENÇÃO:** Se obtivermos como resultado o seguinte erro: "Failed to connect to 127.0.0.1 port 8332: Connection refused", precisamos nos certificar de que uma linha como ```rpcallowip = 127.0.0.1``` esteja configurada no ```~/.bitcoin/bitcoin.conf```. Se ainda não funcionar, precisaremos permitir o acesso à porta 18332 (ou 8332) do nosso host local. Nossa configuração padrão do [Capítulo 2: Configurando um Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md) deve fazer tudo isso.

O resultado é outro array JSON, que infelizmente é ruim de ler se estivermos usando o ```curl``` manualmente. Felizmente, podemos limpá-lo simplesmente usando o ```jq```:
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
Você verá um pouco de relatório de conectividade à medida que os dados são baixados, então, quando os dados chegarem ao ```jq```, tudo será corretamente identado. Estaremos omitindo as informações do download nos próximos exemplos.

## Manipulando Nossa Carteira

Embora já estejamos acessando o ```bitcoind``` diretamente, ainda teremos acesso à funcionalidade de carteira, porque ela está amplamente armazenada no próprio ```bitcoind```.

### Pesquisando Endereços

Usando o RPC ```getaddressesbylabel``` para listar todos os nossos endereços atuais:
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
Este é o nosso primeiro exemplo de um parâmetro real, ```" "```. Este é o parâmetro ```label``` obrigatório para o ```getaddressesbylabel```, mas todos os nossos endereços estão sob o rótulo padrão, então nada de especial foi necessário neste momento.

O resultado é uma lista de todos os endereços que foram usados na nossa carteira. Alguns dos quais presumivelmente possuem saldo.

### Pesquisando Saldos

Use o RPC ```listunspent``` para listar os saldos que temos disponíveis:
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
Esta é quase a mesma saída que recebemos quando digitamos ```bitcoin-cli listunspent```, mostrando como as duas interfaces estão intimamente ligadas. Se nenhuma limpeza ou ajuda extra for necessária, então o ```bitcoin-cli``` apenas produzirá o RPC. Simples assim!

### Criando um Endereço

Depois de saber onde os saldos estão, a próxima etapa na elaboração de uma transação é obter um endereço de troco. Agora provavelmente já pegamos o jeito e sabemos que para os comandos RPC simples, tudo que precisamos fazer é ajustar o ```method``` no comando ```curl```:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "mrSqN37TPs89GcidSZTvXmMzjxoJZ6RKoz",
  "error": null,
  "id": "curltest"
}

```
Neste ponto, podemos até mesmo voltar à nossa prática padrão de salvar os resultados em variáveis com a ajuda adicional do `jq`:
```
$ changeaddress=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')
$ echo $changeaddress
mqdfnjgWr2r3sCCeuTDfe8fJ1CnycF2e6R
```
Não precisamos nos preocupar com as informações do download. Ele irá para o ```STDERR``` e será exibido em nossa tela, enquanto os resultados irão para o ```STDOUT``` e serão salvos em nossa variável.

## Criando uma Transação

Agora estamos prontos para criar uma transação com o ```curl```.

### Preparando as Variáveis

Assim como no ```bitcoin-cli```, para criar uma transação usando o Curl com o RPC, devemos primeiro salvar nossas variáveis. A única mudança aqui é que o ```curl``` cria um objeto JSON que inclui um valor-chave ```result```, então sempre precisaremos usar o pipe (```|```) através da tag ```.result``` antes de fazer outra coisa qualquer.

Este exemplo configura nossas variáveis para usar o BTC de 1.2985 em fundos listados na primeira transação não-gasta acima:
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

### Criando a Transação

A transação criada com o ```curl``` é muito semelhante à transação criada com o ```bitcoin-cli```, mas com algumas diferenças sutis:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.0003, "'$changeaddress'": 0.0005}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "02000000010d6acd0356db222ca3a7ee7fa1e3044316223ceec1f64b58aeb2e0de921007e70100000000ffffffff0230750000000000001976a914ac19d3fd17710e6b9a331022fe92c693fdf6659588ac50c30000000000001976a9147021efec134057043386decfaa6a6aa4ee5f19eb88ac00000000",
  "error": null,
  "id": "curltest"
}
```
O coração da transação é, obviamente, o array JSON ```params```, que estamos colocando em uso total pela primeira vez.

Podemos observar que todos os ```params``` estão alojados nos ```[]``` para marcar o array de parâmetros.

Nós também variamos as citações de como as coisas funcionavam no ```bitcoin-cli```, para iniciar e terminar cada array e objeto dentro do array ```params``` com ```''``` ao invés do tradicional ```'''```. Isso porque todo o conjunto de argumentos JSON já tem um ```'``` em torno dele. Como de costume, basta dar uma olhada na bizarra citação do shell e se acostumar com isso.

No entanto, há uma última coisa a ser observada neste exemplo, e pode ser _enlouquecedor_ se não tivermos percebido. Quando executamos um comando ```createrawtransaction``` com ```bitcoin-cli```, o array JSON de entradas e o objeto JSON de saídas eram parâmetros distintos, portanto, foram separados por um espaço. Agora, porque eles são parte do array JSON ```params```, eles são separados por uma vírgula (```,```). Se não tivéssemos percebido isso obteríamos um ```parse error``` sem muitas informações.

> **ATENÇÃO:** Todo mundo já teve problemas para depurar o ```curl```, não é mesmo? Para resolver isso basta adicionar o argumento ```--trace-ascii/tmp/foo```. Informações completas sobre o que está sendo enviado ao servidor serão salvas em ```/tmp/foo``` (ou qualquer nome de arquivo que quisermos informar).

Depois de verificarmos se as coisas estão funcionando, provavelmente desejaremos salvar o código hexadecimal em uma variável:
```
$ hexcode=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.0003, "'$changeaddress'": 0.0005}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')
```

### Assinando e Enviando

Assinar e enviar a nossa transação usando ```curl``` é bem simples, basta usar os seguintes comandos do RPC ```signrawtransactionwithwallet``` e ```sendrawtransaction```:

```
$ signedhex=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "signrawtransactionwithwallet", "params": ["'$hexcode'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .hex')

$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "sendrawtransaction", "params": ["'$signedhex'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "eb84c5008038d760805d4d9644ace67849542864220cb2685a1ea2c64176b82d",
  "error": null,
  "id": "curltest"
}
```
## Resumo: Acessando o Bitcoind com Curl

Terminando esta seção, podemos sentir que acessar o ```bitcoind``` através de ```curl``` é muito parecido com acessá-lo através de ```bitcoin-cli```, porém, é mais complicado. E estamos certos. O ```bitcoin-cli``` tem funcionalidade RPC bem completa, então qualquer coisa que fizermos através do ```curl```  provavelmente poderemos fazer através do ```bitcoin-cli```. E é por isso que vamos continuar nos concentrando no ```bitcoin-cli``` após esta digressão.

Mas ainda há razões para usar ```curl``` ao invés do ```bitcoin-cli```:

_Qual é o poder do curl?_ Obviamente, o ```curl``` elimina um nível intermediário. Ao invés de trabalhar com o ```bitcoin-cli```, que envia comandos RPC para o ```bitcoind```, estamos enviando esses comandos RPC diretamente para ele. Isso permite uma programação mais robusta, porque não precisamos nos preocupar com as coisas inesperadas que o ```bitcoin-cli``` pode fazer ou como isso pode mudar com o tempo. No entanto, também estamos dando os primeiros passos para usar uma linguagem de programação mais abrangente do que as opções pobres oferecidas por um script de shell. Como veremos nos últimos capítulos deste livro, podemos realmente ver que as bibliotecas curl são outras funções que acessam os comandos RPC em uma variedade de linguagens de programação: mas isso ainda está muito longe.

## O Que Vem Depois?

Aprenda mais uma maneira de enviar transações no Bitcoin com [§4.5 Enviando Moedas com Transações Brutas Automatizadas](04_5_Sending_Coins_with_Automated_Raw_Transactions.md).