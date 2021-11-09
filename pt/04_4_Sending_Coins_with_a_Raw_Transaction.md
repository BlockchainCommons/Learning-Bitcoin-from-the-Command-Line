# 4.4: Enviando Moedas com Transações Brutas

Conforme observado no início deste capítulo, a interface ```bitcoin-cli``` oferece três maneiras principais de enviar moedas. A seção [§4.1](04_1_Sending_Coins_The_Easy_Way.md) falou sobre como enviá-la pela primeira vez, usando o comando ```sendtoaddress```. Desde então, estamos construindo coisas mais detalhadas sobre como enviar moedas de uma segunda maneira, com transações brutas. A seção [§4.2](04_2_Creating_a_Raw_Transaction.md) nos ensinou como criar uma transação bruta, um [Prefácio](04_2__Interlude_Using_JQ.md) explicou JQ e a seção [§4.3](04_3_Creating_a_Raw_Transaction_with_Named_Arguments.md) demonstrou os argumentos nomeados.

Agora podemos colocá-los juntos e realmente enviar fundos usando uma transação bruta.

## Criando um Endereço de Troco

Nosso exemplo de transação bruta na seção §4.2 era simples demais: enviamos um UTXO inteiro para um novo endereço. Com mais frequência, iremos desejar enviar a alguém uma quantia em dinheiro que não corresponda a um UTXO. Mas, devemos nos lembrar que o excesso de dinheiro de um UTXO que não é enviado ao destinatário se torna apenas a taxa de transação. Então, como enviar para alguém apenas uma parte de um UTXO, enquanto guardamos o resto para a gente?

A solução é _enviar_ o restante dos fundos para um segundo endereço, um endereço de troco que criamos em nossa carteira especificamente para recebê-lo:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress legacy)
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
```
Observe que isso usa uma nova função: ```getrawchangeaddress```. É basicamente a mesma coisa que o ```getnewaddress```, mas é otimizado para uso como um endereço de troco em uma transação bruta, portanto, não faz coisas como criar entradas em nossa lista de endereços. Selecionamos novamente o endereço ```legacy```, ao invés de usar o padrão ```bech32```, simplesmente para consistência. Esta é uma situação em que teria sido seguro gerar um endereço Bech32 padrão, apenas usando ```bitcoin-cli getrawchangeaddress```, porque ele seria enviado e recebido por nós em nosso node Bitcoin Core, que tem suporte integral a isso. Mas, estamos adiantando as coisas. Mas vamos mudar para Bech32 na seção [§4.6](04_6_Creating_a_Segwit_Transaction.md).

Agora temos um endereço adicional em nossa carteira, para que possamos receber o troco de um UTXO! Para usá-lo, precisaremos criar uma transação bruta com duas saídas.

## Escolhendo UTXOs Suficientes

Nosso exemplo de transação bruta era simples também de outra maneira: assumia que havia dinheiro suficiente em um único UTXO para cobrir toda a transação. Frequentemente, esse será o caso, mas às vezes desejaremos criar transações que gastem mais dinheiro do que temos em um único UTXO. Para fazer isso, devemos criar uma transação bruta com duas (ou mais) entradas.

## Escrevendo uma Transação Bruta Real

Para resumir: a criação de uma transação bruta real para enviar moedas, às vezes, requer múltiplas entradas e, quase sempre, múltiplas saídas, uma das quais é um endereço de troco. Estaremos criando esse tipo de transação mais realista, em um exemplo que mostra um caso de uso da vida real, enviando fundos por meio da segunda metodologia do Bitcoin, as transações brutas.

Vamos usar nossos UTXOs de índice 0 e 2:
```
$ bitcoin-cli listunspent
[
[
  {
    "txid": "0619fecf6b2668fab1308fbd7b291ac210932602a6ac6b8cc11c7ae22c43701e",
    "vout": 1,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00899999,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },  
  {
    "txid": "0df23a9dba49e822bbc558f15516f33021a64a5c2e48962cec541e0bcc79854d",
    "vout": 0,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00100000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
   }
]

```
Em nosso exemplo, enviaremos 0,009 BTC, que é (um pouco) maior do que qualquer um de nossos UTXOs. Para isso, será necessário que os combinemos e, em seguida, vamos usar nosso endereço de troco para recuperar os fundos não gastos.

### Configurando as Variáveis

Já temos as variáveis ​​```$changeaddress``` e ```$recipient``` dos exemplos anteriores:
```
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
Também precisamos registrar o txid e vout para cada um de nossos dois UTXOs. Tendo identificado os UTXOs que queremos gastar, podemos usar as técnicas do JQ para garantir que o acesso a eles esteja livre de erros:
```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
```

### Escrevendo a Transação

Escrever a transação bruta em si é surpreendentemente simples. Tudo o que precisamos fazer é incluir um objeto JSON adicional separado por vírgula no array JSON de entradas e um par de valores-chave adicional separado por vírgula em um objeto JSON de saída.

Aqui temos um exemplo. Observe as múltiplas entradas após o argumento ```inputs``` e as múltiplas saídas após o argumento ```outputs```.
```
$ rawtxhex2=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.009, "'$changeaddress'": 0.0009 }''')
```
Fomos _muito_ cuidadosos em calcular a quantidade. Esses dois UTXOs contêm 0,00999999 BTC. Depois de enviar 0,009 BTC, teremos 0,00099999 BTC restantes. Escolhemos 0,00009999 BTC como a nossa taxa de transação. Para acomodar essa taxa, definimos o troco como sendo 0,0009 BTC. Se não tivéssemos prestado atenção e definido nosso troco como 0,00009 BTC, essa quantidade de BTC adicional seria enviada para os mineradores! Se tivéssemos esquecido de fazer o troco, todo o excesso teria desaparecido. Portanto, novamente, _ tenha cuidado_.

Felizmente, podemos verificar três vezes com o alias ```btctxfee``` do Prefácio do JQ:
```
$ ./txfee-calc.sh $rawtxhex2
.00009999
```

### Concluindo

Agora podemos assinar, selar e entregar nossa transação, e ela será sua (e do faucet):
```
$ signedtx2=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex2 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx2
e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d
```

### Esperando

Como de costume, nossas moedas estarão no fluxo por um tempo: o troco ficará indisponível até que a transação seja realmente confirmada e um novo UTXO nos seja dado.

Mas, em 10 minutos ou menos (provavelmente), teremos o dinheiro restante de volta e totalmente utilizável novamente. Por enquanto, ainda precisamos esperar:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```
E o troco eventualmente voltará para nós:
```
[
  {
    "txid": "e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d",
    "vout": 1,
    "address": "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
    "scriptPubKey": "76a91432db726320e4ad170c9c1ee83cd4d8a243c3435988ac",
    "amount": 0.00090000,
    "confirmations": 1,
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
    "amount": 0.00022000,
    "confirmations": 16,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```
Este também pode ser um bom momento para revisitar um explorador de blockchain, para que possamos ver mais intuitivamente como as entradas, saídas e taxas estão dispostas na transação: [e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d](https://mempool.space/pt/testnet/tx/e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d).

## Resumo: Enviando Moedas com Transações Brutas

Para enviar moedas usando transações brutas, precisamos criar uma transação bruta com uma ou mais entradas (para ter fundos suficientes) e uma ou mais saídas (para recuperar usando o troco). Então, podemos seguir nosso procedimento normal de usar o ```createrawtransaction``` com argumentos nomeados junto com o JQ, conforme descrito nas seções anteriores.

> :fire: ***Qual é o poder de enviar moedas com transações brutas?***
> _As vantagens._ Oferece maior controle. Se o nosso objetivo é escrever um programa ou script Bitcoin mais complexo, provavelmente usaremos as transações brutas para saber exatamente o que está acontecendo. Essa também é a situação mais _segura_ para usar transações brutas, porque podemos garantir que não cometeremos nenhum erro na parte da programação.
> _As desvantagens._ É muito fácil perder dinheiro. Não há avisos, bloqueios e barreiras na programação, a não ser que as criemos. Também é tudo muito misterioso. A formatação é desagradável, mesmo usando a interface ```bitcoin-cli``` que é fácil de usar, e temos que fazer muitas pesquisas e cálculos manualmente.

## O Que Vem Depois?

Veja uma forma alternativa de inserir comandos no [Prefácio: Usando Curl](04_4__Interlude_Using_Curl.md).

Ou, se preferir pular o que é francamente uma digressão, podemos aprender mais uma forma de enviar transações no Bitcoin na seção [§4.5 Enviando Moedas com Transações Brutas Automatizadas](04_5_Sending_Coins_with_Automated_Raw_Transactions.md).