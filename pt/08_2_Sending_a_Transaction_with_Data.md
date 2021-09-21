# 8.2: Enviando uma Transação com Dados

O último jeito de variar a forma como enviamos uma transação simples é usar a transação para enviar dados ao invés de saldos (ou, na prática, enviar algo além dos saldos). Isso nos dá a capacidade de inserir informações na blockchain. Isso é feito através de um comando especial, o ```OP_RETURN```.

A pegadinha? Só podemos armazenar 80 bytes por vez!

## Criando Nossos Dados

A primeira coisa que precisamos fazer é criar os 80 bytes (ou menos) de dados que iremos gravar no nosso ```OP_RETURN```. Isso pode ser tão simples quanto preparar uma mensagem ou podemos usar o hash dos dados existentes. Por exemplo, o ```sha256sum``` produz 256 bits de dados, que são apenas 32 bytes, bem abaixo do nosso limite:
```
$ sha256sum contract.jpg
b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75  contract.jpg
$ op_return_data="b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75"
```
> :book: _O que é um OP_RETURN?_ Todas as transações Bitcoin são construídas com scripts que iremos ver mais a fundo no próximo capítulo. O ```OP_RETURN``` é um script simples que define o OUTPUT como sendo inválido. A convenção resultou no uso dele para incorporar dados na blockchain.

## Separando Algumas Moedas

Nosso objetivo ao criar uma transação de dados não é enviar dinheiro para alguém, mas colocar dados dentro da blockchain. No entanto, _devemos_ gastar algumas moedas para fazer isso. Só precisamos usar um endereço de troco como sendo nosso _ único_ destinatário. Em seguida, podemos identificar um UTXO e enviá-lo para nosso endereço de troco, sem esquecer da taxa de transação, ao mesmo tempo que usamos a mesma transação para criar um OP_RETURN.

Esta é a configuração padrão:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "vout": 0,
    "address": "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh",
    "scriptPubKey": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
    "amount": 0.01463400,
    "confirmations": 1392,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/1'/12']02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb)#cjr03mru",
    "safe": true
  }
]

$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
```

## Escrevendo uma Transação Bruta

Agora podemos escrever uma nova transação bruta com duas saídas: uma é o nosso endereço de alteração para recuperar (a maior parte) do nosso saldo, a outra é um endereço de dados, que é uma variável ```bitcoin-cli``` para um OP_RETURN.
```
rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "data": "'$op_return_data'", "'$changeaddress'": 0.0146 }''')
```
Esta é a cara da transação:
```
{
  "txid": "a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8",
  "hash": "a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8",
  "version": 2,
  "size": 125,
  "vsize": 125,
  "weight": 500,
  "locktime": 0,
  "vin": [
    {
      "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_RETURN b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "hex": "6a20b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "type": "nulldata"
      }
    },
    {
      "value": 0.01460000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 998a9b0ed076bbdec1d88da4f475b9dde75e3620",
        "hex": "0014998a9b0ed076bbdec1d88da4f475b9dde75e3620",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qnx9fkrksw6aaaswc3kj0gademhn4ud3q7cz4fm"
        ]
      }
    }
  ]
}

```
Como podemos observar, ela envia a maior parte do dinheiro de volta para o endereço de troco (`tb1qnx9fkrksw6aaaswc3kj0gademhn4ud3q7cz4fm`) menos a taxa de transação. Mais importante, a primeira saída mostra um OP_RETURN com os dados (`b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75`) logo depois.

## Enviando uma Transação Bruta

Assine a transação bruta e envie-a, e logo esse OP_RETURN será incorporado a blockchain!

## Verificando Nosso OP_RETURN

Novamente, podemos olhar para essa transação usando um explorador da blockchain:
[https://mempool.space/pt/testnet/tx/a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8/]

Podemos observar um aviso sobre os dados estarem em um "protocolo desconhecido". Se estivermos projetando algum uso regular dos dados ```OP_RETURN```, provavelmente iremos marcar com um prefixo especial, para marcar esse protocolo. Então, os dados OP_RETURN reais podem ser algo como "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". Este exemplo não usa um prefixo para evitar qualquer tipo de confusão com os espaços de dados.

## Resumo: Enviando uma Transação com Dados

Podemos usar o ```OP_RETURN``` para armazenar até 80 bytes de dados na blockchain. Fazemos isso com a palavra-código ```data``` em um ```vout```. Ainda precisaremos enviar alguns satoshinhos, mas basta enviá-lo de volta para um endereço de troco, retirando a taxa de transação.

> :fire: _Qual é o poder do OP_RETURN?_ O OP_RETURN abre novas possibilidades para o blockchain, porque podemos inserir dados que provam que certas coisas aconteceram em determinados momentos. Várias organizações estão pensando em usar os OP_RETURNs como prova de existência, para direitos autorais, para moedas coloridas e [para outros fins](https://en.bitcoin.it/wiki/OP_RETURN). Embora 80 bytes possam não parecer muito, pode ser bastante eficaz se os OP_RETURNs forem usados ​​para armazenar hashes dos dados reais. Então, podemos provar a existência dos nossos dados digitais demonstrando que o hash deles corresponde ao hash na blockchain.

Observe que há alguma controvérsia sobre o uso da blockchain do Bitcoin usando-a para este fim.

## O Que Vem Depois?

Vamos conhecer mais sobre "Programando no Bitcoin" no [Capítulo Nove: Apresentando os Scripts do Bitcoin](09_0_Introducing_Bitcoin_Scripts.md).