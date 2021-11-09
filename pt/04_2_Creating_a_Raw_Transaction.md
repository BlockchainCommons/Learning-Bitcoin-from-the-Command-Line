# 4.2 Criando uma Transação Bruta

Agora estamos prontos para criar transações brutas no Bitcoin. Isso permite que enviemos dinheiro, mas que criemos as transações com a precisão desejada. Nesta primeira seção, iremos nos concentrar em uma transação simples de uma entrada e uma saída. Este tipo de transação _não_ é realmente útil, porque raramente vamos querer enviar todo o nosso dinheiro para uma pessoa (a menos que estejamos apenas encaminhando, como se estivesse movendo coisas de uma carteira para outra). Portanto, esse _não é o melhor método_ para enviar dinheiro. É apenas um conteúdo fundamental para _realmente_ enviar dinheiro com uma transação bruta.

## Compreendendo a Transação no Bitcoin

Antes de mergulhar na criação de transações brutas, devemos nos certificar de que entendemos como uma transação no Bitcoin funciona. Tudo gira entorno dos UTXOs.

> :book: ***O que é um UTXO?*** Quando recebemos dinheiro em nossa carteira Bitcoin, ele aparece como uma transação individual. Cada uma dessas transações é chamada de "Saída de Transação Não-Gasta" (Unspent Transaction Output em inglês, mais conhecido como UTXO). Não importa se vários pagamentos foram feitos para o mesmo endereço ou para vários endereços: cada transação recebida permanece distinta na carteira como um UTXO.

Ao criarmos uma nova transação de saída, reunimos um ou mais UTXOs, cada um representando um pouquinho do dinheiro que recebemos. Nós os usamos como entradas para uma nova transação. Juntos, o valor deles deve ser igual ao que desejamos gastar _ou mais do que o total_. Em seguida, geramos uma ou mais saídas, que dão o dinheiro representado pelas entradas a uma ou mais pessoas. Isso cria novos UTXOs para os destinatários, que podem então usá-los para financiar transações futuras.

Aqui está o truque: _Todos os UTXOs que coletarmos são gastos na íntegra!_ Isso significa que se quisermos enviar apenas parte do dinheiro em um UTXO para outra pessoa, também precisamos gerar uma saída adicional que envia o resto para nós! Por enquanto, não vamos nos preocupar com isso, mas o uso de um endereço de mudança será vital ao passar da teoria deste capítulo para transações mais práticas.

## Listando as Transações Não-Gastas

Para criar uma nova transação bruta, devemos saber quais UTXOs estão disponíveis para gastar. Podemos determinar essas informações com o comando ```bitcoin-cli listunspent```:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 20,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
    "vout": 1,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00050000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

```
Esta lista mostra três UTXOs diferentes, no valor de 0,0001, 0,0005 e 0,00022 BTC. Observe que cada um tem o próprio _txid_ distinto e permanece seperado na carteira, até mesmo os dois últimos, que foram enviados para o mesmo endereço.

Quando quisermos gastar um UTXO, não é suficiente apenas saber o id da transação. Isso porque cada transação pode ter várias saídas! Lembra daquele primeiro valor que o faucet nos enviou? Na transação, parte do dinheiro foi para nós e parte para outra pessoa. O ```txid``` se refere à transação geral, enquanto um ```vout``` diz qual das múltiplas saídas recebemos. Nesta lista, cada uma dessas transações é a primeira ```vout``` de uma transação anterior, mas _não necessariamente é sempre o caso_.

Portanto, o txid+vout=UTXO. Essa será a base de qualquer transação bruta.

## Escrevendo uma Transação Bruta Com Uma Saída

Agora estamos prontos para escrever um exemplo simples de transação bruta que mostra como enviar um UTXO inteiro para outra pessoa. Conforme falamos anteriormente, este não é um caso muito realista.

> :warning: **ATENÇÃO:** É muito fácil perder dinheiro com uma transação bruta. Considere que todas as instruções sobre como enviar bitcoins por meio de transações brutas são _muito_, _muito_ perigosas. Sempre que estiver enviando moedas na _mainnet_ para outras pessoas, devemos usar um dos outros métodos explicados neste capítulo. Criar transações brutas é extremamente útil se estivermos escrevendo programas para o bitcoin, mas _só_ neste caso. (Por exemplo: ao escrever este exemplo para uma versão anterior deste tutorial, acidentalmente gastamos a transação errada, embora tivéssemos cerca de 10 vezes mais. Quase tudo isso foi enviado para os mineradores.)

### Preparando a Transação Bruta

Para as melhores práticas, iremos começar cada transação registrando cuidadosamente os txids e vouts que iremos gastar.

Nesse caso, vamos gastar um no valor de 0,00050000 BTC porque é o único com um valor decente.
```
$ utxo_txid="61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a"
$ utxo_vout="1"
```
Da mesma forma, devemos registrar o endereço do destinatário para ter certeza de que está correto. No nosso exemplo estamos enviando novamente o saldo de volta para o faucet:
```
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
```
Como de prache, vamos verificar as variáveis com cuidado, para ter certeza de que são aquilo que esperamos!
```
$ echo $utxo_txid
61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a
$ echo $utxo_vout
1
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
Esse destinatário é particularmente importante, porque se bagunçarmos tudo, nosso dinheiro terá dado _tchau tchau_! E como já vimos, escolher a transação errada pode resultar em perda de dinheiro! Portanto, vamos verificar tudo, pelo menos uma vez mais do que a quantidade de costume.

### Entendendo a Taxa de Transação

Cada transação tem uma taxa associada. Ela fica _implícita_ quando enviamos uma transação bruta: O valor que vamos pagar como taxa é sempre igual ao valor de entrada menos o valor de saída. Portanto, temos que diminuir um pouco o valor enviado para ter certeza de que nossa transação será realizada.

> :warning: **ATENÇÃO:** Esta é a parte muito perigosa das transações brutas!! Como gastamos automaticamente toda a quantidade de UTXOs que tivermos, é extremamente importante certificar-se de que sabemos: (1) precisamente quais UTXOs estamos utilizando; (2) exatamente quanto dinheiro ele possui; (3) exatamente quanto dinheiro estamos enviando; e (4) qual é a diferença que ficará para os mineradores. Se errarmos e, por exemplo, usarmos o UTXO errado (um que tenha mais dinheiro do que pensávamos) ou se enviarmos muito pouco dinheiro, o excesso será perdido. _Para sempre_! Não podemos cometer esse erro! Por isso, é importante sabermos as entradas e saídas com _precisão_. Ou melhor, não utilizarmos as transações brutas, exceto como parte de um programa cuidadosamente considerado e verificado três vezes.

> :book: ***Quanto devemos gastar com taxas de transação?*** [Bitcoin Fees](https://bitcoinfees.21.co/) tem uma ótima avaliação ao vivo. O site diz que "fastest and cheapest transaction fee is currently XXX satoshis/byte" onde o XXX será a quantidade de satoshis por byte que precisaremos pagar e também, "For the median transaction size of YYY bytes, this results in a fee of ZZ,ZZZ satoshis", onde YYY é o tamanho de uma transação média e ZZ,ZZZ é o resultado da multiplicação entre YYY e XXX. No caso, precisamos apenas observar o valor ZZ,ZZZ descrito no site.

No momento em que este tutorial está sendo escrito, o _Bitcoin Fees_ sugere uma taxa de transação de cerca de 10.000 satoshis, que é o mesmo que 0,0001 BTC. Obviamente, isso é para a mainnet, não para a testnet, mas queremos testar as coisas de forma realista, então iremos utilizar esta quantidade.

Nesse caso, isso significa que vamos pegar 0,0005 BTC no UTXO que selecionamos, reduzindo a quantidade de 0,0001 BTC para a taxa de transação e enviar os 0,0004 BTC restantes. (E este é um exemplo do porque os micropagamentos não funcionam na rede principal do Bitcoin, porque uma taxa de transação que consome 20% do valor enviado é muito cara, agora imagina se os valores forem menores do que a taxa de transação. Mas é por isso que temos a Lightning.)

> :warning: **ATENÇÃO:** Quanto menor for a taxa de transação, mais tempo irá demorar para que nossa transação entre na blockchain. O site _Bitcoin Fees_ lista os tempos que precisaremos esperar em relação a quantidade de satoshi por byte. Como os blocos são construídos em média a cada 10 minutos, a mudança de taxa pode significar uma mudança de espera de minutos para algumas horas ou dias! Portanto, escolha uma taxa de transação apropriada para o que estamos enviando. É importante observar que nunca devemos colocar taxas abaixo da quantidade mínima para transação, que é 0,0001 BTC.

### Escrevendo a Transação Bruta

Agora estamos prontos para criar a transação bruta. Usaremos o comando ```createrawtransaction```, que pode parecer um pouco intimidante. Isso porque o comando ```createrawtransaction``` não o protege inteiramente do JSON RPC que o ```bitcoin-cli``` utiliza. Ao invés disso, vamos inserir uma matriz JSON para listar os UTXOs que está gastando e um objeto JSON para listar as saídas.

Este é o formato padrão:
```
$ bitcoin-cli createrawtransaction
'''[
     {
       "txid": "'$your_txid'",
       "vout": '$your_vout'
      }
]'''
'''{
   "'$your_recipient'": bitcoin_amount
 }'''
```
 
 Não, não é um erro de digitação. Existem todos os tipos de citações malucas, mas confie que elas farão a coisa certa. Vamos usar `'''` para marcar o início e o fim do array e no objeto JSON. Protegendo as palavras normais como ```"this"```, mas não precisamos proteger os números: ```0```. Se forem variáveis, vamos inserir as aspas simples, como ```"'$this_word'"``` e ```'$this_num'```. (Ufa! Não se preocupe, você pega o jeito).

 Aqui está um comando que cria uma transação bruta para enviar $utxo para o $recipient.
```
$ rawtxhex=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' '''{ "'$recipient'": 0.0004 }''')
$ echo $rawtxhex
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f3610100000000ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```

### Verificando a Transação Bruta

Depois disso, devemos verificar a transação bruta com o comando ```decoderawtransaction``` para ter certeza de que faremos a coisa certa.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "hash": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
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
      "value": 0.00040000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }
  ]
}
```

É importante verificarmos o ```vin```. Estamos gastando a transação certa? Ela contém a quantia de dinheiro esperada? (Vamos verificar com o comando ```bitcoin-cli gettransaction``` e nos certificar de olhar o ```vout``` se está correto). Além disso, vamos verificar o nosso ```vout```. Estamos enviando a quantidade correta? Está indo para o endereço certo? Finalmente, vamos fazer as contas para ter certeza de que o dinheiro que irá para as taxas está correto. O valor do UTXO menos a quantia que está sendo gasta é igual à taxa da transação esperada?

> :information_source: **NOTA:** Podemos notar que cada entrada tem um número de sequência, definido aqui como 4294967295, que é 0xFFFFFFFF. Esta é a última fronteira das transações Bitcoin, porque é um campo padrão em transações que foram originalmente planejadas para um propósito específico, mas nunca foram totalmente implementadas. Portanto, agora existe esse inteiro parado em transações que podem ser reaproveitadas para outros usos. E, de fato, tem sido. No momento em que este livro foi escrito, havia três usos diferentes para a variável chamada ```nSequence``` no código Bitcoin Core: Ela habilita a possibilidade de RBF, ```nLockTime``` e timelocks relativos. Se não houver nada de estranho acontecendo, o ```nSequence``` será definido como 4294967295. Ajustar para um valor mais baixo sinaliza que coisas especiais estão acontecendo.

### Assinando a Transação Bruta

Até o momento, nossa transação bruta é apenas uma teoria: _Podemos_ enviá-la, mas nada irá acontecer. Precisamos fazer algumas coisas para colocá-la na rede.

Primeiro, precisamos assinar nossa transação bruta:
```

$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
$ signedtx="02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
Observe que capturamos o hexadecimal assinado manualmente, ao invés de tentar analisá-lo a partir do objeto JSON. Um pacote de software chamado "JQ" poderia ter um desempenho melhor, como explicaremos no próximo prefácio.

### Enviando a Transação Bruta

Agora temos uma transação bruta pronta para ser usada, mas nada acontece com ela se não a colocarmos na rede, o que iremos fazer com o comando ```sendrawtransaction```. O retorno dele será uma txid:
```
$ bitcoin-cli sendrawtransaction $signedtx
a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a
```
Iremos observar imediatamente que o UTXO e o as nossas moedas foram removidas da nossa carteira:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 23,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 6,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

$ bitcoin-cli getbalance
0.00032000
```
Logo o ```listtransactions``` deve mostrar uma transação confirmada da categoria 'send'.
```
 {
    "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
    "category": "send",
    "amount": -0.00040000,
    "vout": 0,
    "fee": -0.00010000,
    "confirmations": 1,
    "trusted": true,
    "txid": "a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a",
    "walletconflicts": [
    ],
    "time": 1592608574,
    "timereceived": 1592608574,
    "bip125-replaceable": "no",
    "abandoned": false
  }
```
Podemos observar que ele corresponde aos endereços ```txid``` e ```recipient``` (recebedor). Não só mostra o ```amount``` (montante) enviado, mas também mostra a ```fee``` (taxa) da transação. E, a transação já recebeu uma confirmação, porque oferecemos uma taxa que a faria ser colocada em um bloco rapidamente.

Parabéns! Estamos um pouco mais pobres agora!

## Resumo: Criando uma Transação Bruta

Quando satoshinhos entram na nossa carteira Bitcoin, eles permanecem em quantidades distintas, chamadas de UTXOs. Ao criar uma transação bruta para enviar as moedas, usamos um ou mais UTXOs para financiá-la. Podemos então, criar uma transação bruta, assiná-la e enviá-la pela rede Bitcoin. No entanto, esta é apenas uma base do que realmente acontece: geralmente precisaremos criar uma transação bruta com várias saídas para realmente enviar algo na rede Bitcoin!

## O Que Vem Depois?

Vamos fazer uma pausa em "Enviando Transações no Bitcoin" para lermos o [Prefácio: Usando JQ](04_2__Interlude_Using_JQ.md).