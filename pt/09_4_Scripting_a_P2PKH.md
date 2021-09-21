# 9.4: Programando um P2PKH

Os endereços P2PKH estão desaparecendo rapidamente devido ao advento do Segwit, porém, são importantes para entender o Bitcoin, e especialmente para entender os scripts do Bitcoin. Vamos dar uma olhada rápida como os scripts nativos do Segwit (P2WPKH) funcionam na próxima seção.

## Entendendo o Script de Desbloqueio

Dissemos que quando os fundos são enviados para um endereço de bitcoin, eles estão bloqueados juntamente com a chave privada associada a esse endereço. Isso é gerenciado através do ```scriptPubKey``` de uma transação P2PKH, que é projetado de forma que requer que o destinatário tenha a chave privada associada ao endereço bitcoin do tipo P2PKH. Para ser preciso, o destinatário deve fornecer tanto a chave pública ligada à chave privada quanto uma assinatura gerada pela chave privada.

Vamos dar uma olhada novamente na transação que criamos na seção [§9.1](09_1_Understanding_the_Foundation_of_Transactions.md):
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "hash": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "version": 2,
  "size": 191,
  "vsize": 191,
  "weight": 764,
  "locktime": 0,
  "vin": [
    {
      "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
      "vout": 0,
      "scriptSig": {
        "asm": "304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b",
        "hex": "47304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c01210315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
    }
  ]
}
```
Podemos ver que o script de desbloqueio do ```scriptSig``` tem dois valores, uma ```<signature>``` (e um ```[ALL]```) e uma ```<pubkey>```:
```
304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
```
Isso é tudo que um script de desbloqueio é! (No caso de um endereço P2PKH).

## Entendendo o Script de Bloqueio

Lembre-se de que cada script de desbloqueio desbloqueia um UTXO anterior. No exemplo acima, o ```vin``` revela que é realmente desbloqueado o vout ```0``` da txid ```bb4362dec15e67d366088f5493c789f22fbb4a6049f22fb4a604e767dae1f6a631687E2784aa```.

Podemos examinar esse UTXO usando o comando ```gettransaction```.
```
$ bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa"
{
  "amount": 0.00095000,
  "confirmations": 12,
  "blockhash": "0000000075a4c1519da5e671b15064734c42784eab723530a6ace83ca1e66d3f",
  "blockheight": 1780789,
  "blockindex": 132,
  "blocktime": 1594841768,
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "walletconflicts": [
  ],
  "time": 1594841108,
  "timereceived": 1594841108,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9",
      "category": "receive",
      "amount": 0.00095000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "020000000001011efcc3bf9950ac2ea08c53b43a0f8cc21e4b5564e205f996f7cadb7d13bb79470000000017160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71feffffff0218730100000000001976a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac713b10000000000017a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd8702473044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b571530121033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873342c1b00"
}
```
Mas como podemos observar, não recebemos o ```scriptPubKey``` com o ```gettransaction```. Precisamos dar um passo adicional para recuperar essa informação, examinando as informações da transação bruta (que é o ```hex```) com o ```Decoderawtransaction```:
```bash
$ hex=$(bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa" | jq -r '.hex')
$ bitcoin-cli decoderawtransaction $hex
{
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "hash": "6866490b16a92d68179e1cf04380fd08f16ec80bf66469af8d5e78ae624ff202",
  "version": 2,
  "size": 249,
  "vsize": 168,
  "weight": 669,
  "locktime": 1780788,
  "vin": [
    {
      "txid": "4779bb137ddbcaf796f905e264554b1ec28c0f3ab4538ca02eac5099bfc3fc1e",
      "vout": 0,
      "scriptSig": {
        "asm": "0014c4ea10874ae77d957e170bd43f2ee828a8e3bc71",
        "hex": "160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71"
      },
      "txinwitness": [
        "3044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b5715301",
        "033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9"
        ]
      }
    },
    {
      "value": 0.01063793,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 b780fc2e945bea71b9ee2d8d2901f00914a25fbd OP_EQUAL",
        "hex": "a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9yWARt5E3TQsX2RjsauxSZaEZVhinAS4h"
        ]
      }
    }
  ]
}
```
Podemos olhar agora para o ```vout``` ```0``` e ver que ele foi bloqueado com o ```scriptPubKey``` no ```OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG```. Essa é a metodologia de bloqueio padrão usada para um endereço P2PKH mais antigo com o `<pubKeyHash>` preso no meio.

Executando-o demonstrará seu funcionamento.

## Executando um Script P2PKH 

Quando desbloqueamos um UTXO do tipo P2PKH, efetivamente concatenamos os scripts de desbloqueio e de bloqueio. Para um endereço P2PKH, como o exemplo que usamos neste capítulo, isso produz o seguinte:
```
Script: <signature> <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```
Com essas informações juntas, podemos examinar como um UTXO P2PKH é desbloqueado.

Primeiro, colocamos as constantes iniciais na pilha, depois fazemos uma cópia do pubKey com o `OP_DUP`:
```
Script: <signature> <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]

Script: <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> ]

Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> ]

Script: OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKey> OP_DUP
Stack: [ <signature> <pubKey> <pubKey> ]
```
Por que duplicar? Porque é necessário verificar os dois elementos de desbloqueio, a chave pública e a assinatura.

Em seguida, o ```OP_HASH160``` retira o ```<pubKey>``` fora da pilha, cria um hash e coloca o resultado de volta nela.
```
Script: <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKey> OP_HASH160
Stack: [ <signature> <pubKey> <pubKeyHash> ]
```
Então, colocamos o ```<pubKeyHash>``` que estava no script de bloqueio na pilha:
```
Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <pubKeyHash> <pubKeyHash> ]
```
O ```OP_EQUALVERIFY``` é efetivamente dois opcodes: ```OP_EQUAL```, que retira dois itens da pilha e coloca `True` ou `False` de volta baseado na comparação de ambos elementos; e o ```OP_VERIFY``` que retira o resultado e imediatamente marca a transação como inválida se for ```False```. (No capítulo 12 falaremos mais sobre o uso de ```OP_VERIFY``` como condicional.)

Assumindo que os dois ```<pubKeyHash>``` são iguais, teremos o seguinte resultado:
```
Script: OP_CHECKSIG
Running: <pubKeyHash> <pubKeyHash> OP_EQUALVERIFY
Stack: [ <signature> <pubKey> ]
```
Neste momento, provamos que a `<pubkey>` fornecida no `scriptSig` leva ao endereço Bitcoin em questão, então sabemos que o redentor conhecia a chave púbica. Mas, ele também precisa provar o conhecimento da chave privada, o que é feito com os ```OP_CHECKSIG```, que confirmam que a assinatura do script de desbloqueio corresponde a essa chave pública.

```
Script:
Running: <signature> <pubKey> OP_CHECKSIG
Stack: [ True ]
```
O script agora termina e, se foi bem sucedido, a transação é permitida para gastar o UTXO em questão novamente.

### Usando o btcdeb em um Exemplo P2PKH

Testando transações de bitcoin reais com o ```btcdeb``` é um pouco mais complicado, porque precisamos conhecer a chave pública e uma assinatura para fazer todo o trabalho, e gerar o último é um pouco difícil. No entanto, uma maneira de testar as coisas é deixar o Bitcoin fazer o trabalho para nós, gerando uma transação que _possa_ desbloquear um UTXO. Isso é o que fizemos acima, geramos a transação para gastar o UTXO criado pelo ```bitcoin-cli``` para calcular a ```<signature>``` e a ```<pubKey>```. Podemos analisar as informações da transação bruta do UTXO para conhecer o script de bloqueio, incluindo o ```<pubKeyHash>```. 

Podemos montar o script de bloqueio, a assinatura e a pubkey usando o `btcdeb`, mostrando como pode ser simples um script P2PKH .
```
$ btcdeb '[304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
unknown key ID 41d83eaffbf80f82dee4c152de59a38ffd0b6021: returning fake key
valid script
7 op script loaded. type `help` for usage information
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b... | 
0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 
OP_DUP                                                             |
OP_HASH160                                                         |
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           | 
OP_EQUALVERIFY                                                     | 
OP_CHECKSIG                                                        | 
                                                                   | 
                                                                   | 
#0000 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c
```
Colocamos a ```<signature>``` e a ```<pubKey>``` dentro da pilha:
```
btcdeb> step
    <> PUSH stack 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
OP_DUP                                                             | 
OP_HASH160                                                         | 
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           |  
OP_EQUALVERIFY                                                     |  
OP_CHECKSIG                                                        |     
                                                                   |   
                                                                   | 
                                                                   | 
                                                                   | 
                                                                   | 
#0001 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
btcdeb> step
    <> PUSH stack 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_DUP                                                             | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
OP_HASH160                                                         | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           | 
OP_EQUALVERIFY                                                     |
OP_CHECKSIG                                                        |  
                                                                   |   
                                                                   |    
                                                                   | 
                                                                   |
                                                                   | 
                                                                   | 
                                                               | 
```
Usamos o ```OP_DUP``` e o ```OP_HASH``` na ```<pubKey>```:

```
#0002 OP_DUP
btcdeb> step
    <> PUSH stack 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_HASH160                                                         | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
OP_EQUALVERIFY                                                     | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
OP_CHECKSIG                                                        | 
                                                                   | 
                                                                   |   
                                                                   |  
                                                                   |   
                                                                   |   
                                                                   | 
                                                                   | 
                                                                   |
                                                                   | 
#0003 OP_HASH160
btcdeb> step
    <> POP  stack
    <> PUSH stack 41d83eaffbf80f82dee4c152de59a38ffd0b6021
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           |                           41d83eaffbf80f82dee4c152de59a38ffd0b6021
OP_EQUALVERIFY                                                     | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
OP_CHECKSIG                                                        | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
                                                                   | 
                                                                   | 
                                                                   |   
                                                                   |    
                                                                   |    
                                                                   |     
                                                                   | 
                                                                   | 
                                                                   | 
                                                                   | 
```
Depois colocamos o ```<pubKeyHash>``` do script de bloqueio na pilha e o verificamos:
```
#0004 41d83eaffbf80f82dee4c152de59a38ffd0b6021
btcdeb> step
    <> PUSH stack 41d83eaffbf80f82dee4c152de59a38ffd0b6021
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_EQUALVERIFY                                                     |                           41d83eaffbf80f82dee4c152de59a38ffd0b6021
OP_CHECKSIG                                                        |                           41d83eaffbf80f82dee4c152de59a38ffd0b6021
                                                                   | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
                                                                   | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
                                                                   | 
                                                                   | 
                                                                   |  
                                                                   |  
                                                                   |  
                                                                   |       
                                                                   |   
                                                                   | 
                                                                   |
                                                                   | 
#0005 OP_EQUALVERIFY
btcdeb> step
    <> POP  stack
    <> POP  stack
    <> PUSH stack 01
    <> POP  stack
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_CHECKSIG                                                        | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
                                                                   | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
                                                                   | 
                                                                   | and_v(
                                                                   |     sig(304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c...
                                                                   |     and_v(
                                                                   |         pk(0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3...
                                                                   |         c:pk_h(030500000000000000000000000000000000000000000000...
                                                                   |     )
                                                                   | 
                                                                   | )
                                                                   | 
      
```
E nesse ponto, tudo o que é necessário é o ```OP_CHECKSIG```:
```
#0006 OP_CHECKSIG
btcdeb> step
error: Signature is found in scriptCode
```
Infelizmente, esta verificação pode ou não estar funcionando no momento do teste devido a caprichos do Bitcoin Core e do `btcdeb`.

Como mostramos, um P2PKH  é bastante simples. Nossa proteção vem através da força de sua criptografia.

### Como Procurar uma PubKey e uma Assinatura Manualmente

E se quiséssemos gerar a ```<signature>``` e a ```<PubKey>```, informações necessárias para desbloquear um UTXO, sem usar o ```bitcoin-cli``` para criar uma transação?

Acontece que é muito fácil obtermos uma ````<pubKey>````. Nós precisamos usar o ```getaddressinfo``` para examinar o endereço onde o UTXO está:
```
$ bitcoin-cli getaddressinfo mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9
{
  "address": "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9",
  "scriptPubKey": "76a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac",
  "ismine": true,
  "solvable": true,
  "desc": "pkh([f004311c/0'/0'/2']0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b)#t3g5mjk9",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": false,
  "pubkey": "0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b",
  "iscompressed": true,
  "ischange": false,
  "timestamp": 1594835792,
  "hdkeypath": "m/0'/0'/2'",
  "hdseedid": "f058372260f71fea37f7ecab9e4c5dc25dc11eac",
  "hdmasterfingerprint": "f004311c",
  "labels": [
    ""
  ]
}
```
Descobrir essa assinatura, no entanto, requer que entendamos os meandros de como as transações do Bitcoin são criadas. Portanto, vamos deixar isso como sendo um estudo avançado para você leitor: criando uma transação de ```bitcoin-cli``` para "resolver" o UTXO é a melhor solução para isso neste momento.

## Resumo: Programando um P2PKH

Enviar para um endereço P2PKH foi relativamente fácil quando estávamos usando o ````bitcoin-cli````. Examinando o script do Bitcoin subjacente nos revela as funções criptográficas que estavam implícitas no financiamento da transação: como o UTXO foi desbloqueado com uma assinatura e uma chave pública.

## O Que Vem Depois?

Vamos continuar "Apresentando Scripts no Bitcoin" na seção [§9.5: Programando um P2WPKH](09_5_Scripting_a_P2WPKH.md).