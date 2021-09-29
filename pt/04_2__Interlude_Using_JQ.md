# Prefácio: Usando JQ

A criação de uma transação bruta revelou como resultados mais complexos do ```bitcoin-cli``` não podem ser salvos facilmente em variáveis de linha de comando. A resposta para isso é usar o JQ, que permite filtrar elementos individuais de dados JSON mais complexos.

## Instalando JQ

O JQ está disponível em um [repositório no Github](https://stedolan.github.io/jq/). Basta fazermos o download para Linux, OS X ou Windows, de acordo com o seu sistema operacional.

Depois de baixar o binário, podemos instalá-lo em nosso sistema. Se estivermos trabalhando em um VPS Debian, fazendo o passo a passo desse curso, nossa instalação será parecida com esta:
```
$ mv jq-linux64 jq
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin jq
```
> :book: ***O que é JQ?*** O repositório explica melhor, dizendo "O jq é como o sed para dados JSON - você pode usá-lo para dividir, filtrar, mapear e transformar dados estruturados com a mesma facilidade que o sed, awk, e grep permitem que você brinque com o texto".

## Usando JQ Para Acessar o Valor de um Objeto JSON Pelo Índice

**Caso de Uso:** _Capturando o hex de uma transação bruta assinada._

Na seção anterior, o uso de ```signrawtransaction``` não pareceu ser um bom método devido ao fato de não ser capaz de capturar os dados facilmente em variáveis devido a seu retorno no formato JSON:
```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
```
Felizmente, o JQ pode facilmente capturar os dados desse tipo!

Para usar o JQ, vamos executar ```jq``` no backend de um pipe e sempre usar a invocação padrão que é ```jq -r '.'```. O ```-r``` diz ao JQ para produzir uma saída bruta, que funcionará para variáveis de linha de comando, enquanto o ```.``` diz ao JQ para mostrar na tela. Protegemos esse argumento com ```' '``` porque precisaremos dessa proteção mais tarde conforme nossas invocações ```jq``` se tornarem mais complexas.

Para capturar um valor específico de um objeto JSON, basta listar o índice após o ```.```:
```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex'
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
Com essa ferramenta em mãos, podemos capturar as informações de objetos JSON para variáveis na linha de comando:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex')
$ echo $signedtx
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
Podemos usar então essas variáveis facilmente e sem erros:
```
$ bitcoin-cli sendrawtransaction $signedtx
3f9ccb6e16663e66dc119de1866610cc4f7a83079bfec2abf0598ed3adf10a78
```
## Usando JQ Para Acessar Valores Únicos de um Objeto JSON em um Array Pelo Índice

**Caso de Uso:** _Capturando o txid e o vout para um UTXO selecionado._

Extrair dados de um objeto JSON é fácil, mas e se esse objeto JSON estiver em um array JSON? O comando ```listunspent``` oferece um ótimo exemplo, porque geralmente contém várias transações diferentes. E se quisermos capturar as informações específicas de _um_ deles?

Ao trabalhar com um array JSON, a primeira coisa que precisamos fazer é informar ao JQ qual índice acessar. Por exemplo, podemos estar olhando nosswas transações no ```listunspent``` e decidimos que queremos trabalhar com a segunda. Podemos usar o ```'. [1]'``` para acessar o primeiro elemento. O ```[]``` diz que estamos referenciando um array JSON e o ```0``` diz que queremos o índice 0.
```
$ bitcoin-cli listunspent | jq -r '.[1]'
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
  "label": "",
  "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
  "amount": 0.00022,
  "confirmations": 9,
  "spendable": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
  "safe": true
}
```
Podemos então capturar um valor individual desse array selecionado (1) usando um pipe _dentro_ dos argumentos JQ; e então (2) solicitando o valor específico posteriormente, como no exemplo anterior. Isso iria capturar o ```txid``` do primeiro objeto JSON no array JSON produzido pelo comando ```listunspent```:
```
$ bitcoin-cli listunspent | jq -r '.[1] | .txid'
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
```
Observe cuidadosamente como os ```' '``` circundam toda a expressão JQ _incluindo_ o pipe.

Este método pode ser usado para preencher variáveis para um UTXO que desejamos utilizar:
```
$ newtxid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid')
$ newvout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ echo $newtxid
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
$ echo $newvout
0
```
Pronto! Agora podemos criar uma nova transação bruta usando nosso primeiro UTXO como entrada, sem ter que digitar qualquer uma das informações do UTXO manualmente!

## Usando JQ Para Acessar Valores de Objetos JSON Correspondentes em um Array Por Índices

**Caso de Uso:** _Listar o valor de todos os UTXOs._

Ao invés de acessar um único valor específico em um objeto JSON específico, podemos acessar todos os valores específicos em todos os objetos JSON. Isso é feito com ```.[]```, Onde nenhum índice é especificado. Por exemplo, podemos listar todos os saldos não gastos:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.0001
0.00022
```

## Usando JQ Para Cálculos Simples Por Índices

**Caso de Uso:** _Adicionando o valor de todos os UTXOs não gastos._

Neste ponto, podemos começar a usar o retorno do JQ para fazermos uma matemática simples. Por exemplo, somar os valores dessas transações não gastas com um script ```awk``` simples nos daria o equivalente ao ```getbalance```:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
0.00032
$ bitcoin-cli getbalance
0.00032000
```

## Usando JQ Para Exibir Vários Valores de um Objeto JSON em um Array Por Vários Índices

**Caso de Uso:** _Listar as informações de uso para todos os UTXOs._

O JQ pode capturar facilmente elementos individuais de objetos JSON e arrays e colocar os elementos em variáveis. Esse será o principal uso que iremos fazer nas seções futuras. No entanto, ele também pode ser usado para reduzir grandes quantidades de informações geradas pelo ```bitcoin-cli``` em quantidades razoáveis de informações.

Por exemplo, podemos querer ver uma lista de todos os nossos UTXOs (```.[]```) E obter uma lista de todas as informações mais importantes (```.txid, .vout, .amount```):
```
$ bitcoin-cli listunspent | jq -r '.[] | .txid, .vout, .amount'
ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36
0
0.0001
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
0
0.00022
```
Isso torna mais fácil decidir quais UTXOs gastar em uma transação bruta, mas não é muito bonito.

Felizmente, o JQ também permite que sejamos mais sofisticados. Podemos usar as ```{}``` para criar novos objetos JSON (para análise adicional ou para um retorno mais bonito). Também podemos definir o nome do novo índice para cada um dos valores. A saída resultante deve ser muito mais intuitiva e menos sujeita a erros (embora, menos útil para jogar as informações diretamente nas variáveis).

O exemplo a seguir mostra exatamente a mesma análise do ```listunspent```, mas com cada objeto JSON antigo reconstruído como um novo objeto JSON abreviado, com todos os novos valores nomeados com os índices antigos:
```
$ bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'
{
  "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "vout": 0,
  "amount": 0.0001
}
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "amount": 0.00022
}
```
Podemos, é claro, renomear nossos novos índices conforme acharmos necessário. Não há nada de mágico nos nomes originais:
```
$ bitcoin-cli listunspent | jq -r '.[] | { tx: .txid, output: .vout, bitcoins: .amount }'
{
  "tx": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "output": 0,
  "bitcoins": 0.0001
}
{
  "tx": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "output": 0,
  "bitcoins": 0.00022
}
```
## Usando JQ Para Acessar Objetos JSON Por Valor Pesquisado

**Caso de Uso:** _Pesquisando automaticamente os UTXOs que estão sendo usados em uma transação._

As pesquisas JQ até agora têm sido bastante simples: Usamos um índice para pesquisar um ou mais valores em um objeto JSON ou no array. Mas e se quisermos pesquisar um valor em um objeto JSON... usando outro valor? Esse tipo de pesquisa indireta tem aplicabilidade real quando estamos trabalhando com transações criadas usando os UTXOs existentes. Por exemplo, podemos calcular o valor da soma dos UTXOs sendo usados em uma transação, algo de vital importância.

Este exemplo usa a seguinte transação bruta. Podemos observar que esta é uma transação bruta mais complexa com duas entradas e duas saídas. Aprenderemos como fazer isso nas próximas seções, mas por enquanto, é necessário sermos capazes de oferecer exemplos robustos. Observe que, ao contrário dos nossos exemplos anteriores, neste, temos dois objetos em nosso array ```vin``` e dois em nosso array ```vout```.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "hash": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "size": 160,
  "vsize": 160,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    },
    {
      "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 1.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 cfc39be7ea3337c450a0c77a839ad0e160739058 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914cfc39be7ea3337c450a0c77a839ad0e16073905888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mzTWVv2QSgBNqXx7RC56zEhaQPve8C8VS9"
        ]
      }
    },
    {
      "value": 0.04500000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 166692bda9f25ced145267bb44286e8ee3963d26 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914166692bda9f25ced145267bb44286e8ee3963d2688ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP"
        ]
      }
    }
  ]
}
```

### Recuperando Valor(es)

Suponha que saibamos exatamente como essa transação é construída: sabemos que ela usa dois UTXOs como entrada. Para recuperar o txid para os dois UTXOs, poderíamos usar ```jq``` para consultar o valor .vin da transação e, em seguida, fazer referência ao primeiro índice do .vin e, então, ao valor .txid deste array. Posteriormente, poderíamos fazer o mesmo com o primeiro array e, em seguida, o mesmo com os dois valores .vout de .vin. Fácil:
```
$ usedtxid1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ echo $usedtxid1
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ usedtxid2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .txid')
$ echo $usedtxid2
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .vout')
$ echo $usedvout1
1
$ usedvout2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .vout')
$ echo $usedvout2
1
```
No entanto, seria melhor ter um caso geral que salvasse _automaticamente_ todos os txids de nossos UTXOs.

Já sabemos que podemos acessar todos os ```.txid``` usando um valor do array ```.[] ```. Podemos usar isso para criar uma pesquisa geral no .txid:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ echo ${usedtxid[0]}
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ echo ${usedtxid[1]}
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ echo ${usedvout[0]}
1
$ echo ${usedvout[1]}
1
```
O único truque real aqui é como salvamos as informações usando o shell bash. Ao invés de salvar em uma variável com ```$(command)```, nós salvamos em um array com ```($(command))```. Fomos então capazes de acessar os elementos individuais do array bash com uma construção ```${variable [n]}```. Ao invés disso, poderíamos acessar todo o array usando a ```${variable [@]}```. (E antes que diga algo, ninguém nunca disse que o bash ficaria bonito).

> :warning: **ATENÇÃO:** Lembre-se sempre de que um UTXO é uma transação _mais_ um vout. Perdemos o vout na primeira vez que escrevemos este exemplo JQ, e ele parou de funcionar quando acabamos com uma situação em que dois ```vouts``` foram enviados da mesma transação.

### Recuperando o(s) Objeto(s) Relacionado(s)

Agora podemos usar as informações salvas no ```txid``` e no ```vout``` para referenciar os UTXOs no ```listunspent```. Para encontrar as informações sobre os UTXOs usados pela transação bruta, precisamos examinar todo o array JSON (```[]```) com as transações não-gastas. Podemos então escolher (```select```) objetos JSON individuais que incluam (```contains```) os txids. _Então_ selecionamos (```select```) as transações entre aquelas que _também_ contêm (```contain```) o valor correto.

O uso de outro nível do pipe é a metodologia padrão do JQ: Pegamos um conjunto de dados, depois a reduzimos para todas as transações relevantes e, em seguida, reduzimos para os valores que foram realmente usados nessas transações. No entanto, os argumentos ```select``` e ```contains``` são algo novo. Eles mostram um pouco da complexidade do JSON que vai além do escopo deste tutorial. Por enquanto, saiba apenas que esta invocação em particular funcionará para capturar objetos correspondentes.

Para começar de forma simples, vamos selecionar os dois UTXOs, um de cada vez:
```
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[0]}'")) | select(.vout | contains('${usedvout[0]}'))'
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[1]}'")) | select(.vout | contains('${usedvout[1]}'))'
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 5,
  "spendable": true,
  "solvable": true
}
```
Ao invés disso, um simples bash usando um loop ```for``` poderia nos dar _todos_ os nossos UTXOs:
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout'))'; done;
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 7,
  "spendable": true,
  "solvable": true
}
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}

```
Observe que estamos deixando um pouquinho mais feio nosso array ```${#usedtxid[*]}``` para determinar o tamanho dele e, em seguida, acessamos cada valor no array ```usedtxid``` e cada valor no array ```usedvout``` em paralelo, colocando-os em variáveis mais simples para termos um acesso menos feio.

## Usando JSON Para Cálculos Simples Por Valor

**Caso de Uso:** _Calcular automaticamente o valor dos UTXOs usados em uma transação._

Agora podemos ir um passo adiante e solicitar o .amount (ou qualquer outro par índice-valor do JSON) dos UTXOs que estamos recuperando.

Este exemplo repete o uso dos arrays ```$usedtxid``` e ```$usedvout``` definidos da seguinte forma:

```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
```

O mesmo script ```for``` pode ser usado para percorrer os arrays, mas com um pipe adicionado no JQ que produz o valor ```amount``` para cada um dos UTXOs selecionados.

```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done;
0.9
0.4
```

Neste ponto, podemos somar os .amounts com um script ```awk```, para realmente ver quantas moedas estão nos UTXOs gastos na transação:

```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}'
1.3
```

Ufa!

## Usando JQ Para Cálculos Complexos

**Caso de Uso:** _Calcular a taxa de uma transação._

Descobrir a taxa completa de transação neste ponto requer apenas mais um pouco de matemática: basta determinar quanto dinheiro está passando pelo .vout. Este é um uso simples de JQ onde apenas usamos o ```awk``` para somar o ```value``` de todas as informações do ```vout```:

```
$ bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}'
1.045
```

Para completar o cálculo da taxa de transação, vamos subtrair o .vout .amount (1.045) do .vin .amount (1.3).
Para fazer isso, precisaremos instalar ```bc```:

```
$ sudo apt-get intall bc
```

Se juntarmos tudo, iremos criar uma calculadora completa com apenas um script de cinco linhas:

```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ btcin=$(for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}')
$ btcout=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
$ echo "$btcin-$btcout"| /usr/bin/bc
.255
```

E esse também é um bom exemplo de por que precisamos verificar nossas taxas: pretendíamos enviar uma taxa de transação com 5.000 satoshis, mas invés disso enviamos pagando 255.000 satoshis de taxa. Ops!

> :warning: **ATENÇÃO:** A primeira vez que escrevemos esta lição, realmente calculamos mal a nossa taxa e não a vimos até que executamos nossa calculadora de taxas. É *tão* fácil, que nosso dinheiro acabou. (O exemplo acima é, na verdade, de nossa segunda iteração da calculadora, e dessa vez cometemos o erro de propósito).

Para mais magia usando o JQ (e se alguma coisa não estiver clara), leia o [Manual JQ](https://stedolan.github.io/jq/manual/) e o [Livro de Receitas do JQ](https://github.com/stedolan/jq/wiki/Cookbook). Estaremos usando o JQ regularmente nos exemplos futuros.

## Fazendo Alguns Aliases Novos

Código em JQ pode ser um pouco pesado, então devemos considerar adicionar algumas invocações mais longas e interessantes ao nosso ```~/.bash_profile```.

Sempre que estivermos procurando por uma grande massa de informações em uma saída de objeto JSON por um comando ```bitcoin-cli```, precisamos considerar escrever um alias para reduzi-lo exatamente ao que desejamos observar.

```
alias btcunspent="bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'"
```

## Executando o Script de Taxa de Transação

O [Script de Cálculo de Taxa](../src/04_2_i_txfee-calc.sh) está disponível no diretório `src/`. Você pode baixá-lo e salvá-lo como ```txfee-calc.sh```.

> :warning: **ATENÇÃO:** Este script não foi verificado extensivamente. Se for usá-lo para verificar as taxas de transação reais, só deve fazê-lo depois de fazer uma verificação pessoal dos valores.

Certifique-se de que as permissões no script estejam corretas:

```
$ chmod 755 txfee-calc.sh
```

Podemos, então, executar o script da seguinte maneira:

```
$ ./txfee-calc.sh $rawtxhex
.255
```

Também podemos criar um alias:

```
alias btctxfee="~/txfee-calc.sh"
```

## Resumo: Usando JQ

O JQ facilita a extração de informações de arrays e objetos JSON. Ele também pode ser usado em scripts shell para cálculos bastante complexos que tornarão nossa vida mais fácil.

## O Que Vem Depois?

Continue "Enviando Transações no Bitcoin" na sessão [§4.3 Criando uma Transação Bruta com Argumentos Nomeados](04_3_Creating_a_Raw_Transaction_with_Named_Arguments.md).