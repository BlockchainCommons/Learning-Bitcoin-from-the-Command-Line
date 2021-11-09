# 3.4: Recebendo uma Transação

Agora estamos prontos para receber dinheiro no endereço que configuramos.

## Pegando um Pouco de Dinheiro

Para fazermos qualquer coisa a mais, precisamos de dinheiro. Na Testnet, isso é feito por meio de _faucets_. Como o dinheiro é de mentira, basta irmos a um faucet e pedirmos um pouco de dinheiro que ele será enviado para nós. Sugerimos usar o faucet em https://testnet-faucet.mempool.co/, https://bitcoinfaucet.uo1.net/ ou https://testnet.coinfaucet.eu/en/. Se eles não estiverem disponíveis por algum motivo, podemos pesquisar por "bitcoin testnet faucet" na internet e encontrar outros. 

Para usarmos um faucet, geralmente precisamos acessar uma URL e copiar e colar nosso endereço. É importante observamos que este é um daqueles casos em que não seremos capazes de usar variáveis de linha de comando, infelizmente. Posteriormente, será criada uma transação que enviará dinheiro da faucet para nós. 

> :book: ***O que é uma transação?*** Uma transação é uma troca de bitcoins. O proprietário de alguns bitcoins usa sua chave privada para acessar essas moedas e, em seguida, bloqueia a transação usando a chave pública do destinatário.

> :link: **TESTNET vs MAINNET:** Infelizmente, não existem faucets na vida real. Se estivéssemos usando a Mainnet, precisaríamos realmente comprar bitcoins em uma exchange ou em um caixa eletrônico, ou precisaríamos que alguém os enviasse alguns satoshinhos. A vida na Testnet é muito mais fácil. 

## Verificando o Nosso Dinheiro

Depois de solicitarmos nosso dinheiro, devemos ser capazes de verificá-lo com o comando `bitcoin-cli getbalance`: 
```
$ bitcoin-cli getbalance
0.00000000
```
Mas espere, não há nenhum saldo ainda!?

Bem-vindo ao mundo da latência do Bitcoin. O problema é que nossa transação ainda não foi gravada no bloco!

> :book: ***O que é um bloco?*** As transações são transmitidas pela rede e reunidas em blocos pelos mineradores. Esses blocos são protegidos com uma prova de trabalho matemática (em inglês, _proof-of-work_, ou PoW), que prova que o poder de computação foi gasto como parte da criação do bloco. É essa prova de trabalho (multiplicada por muitos blocos, cada um construído sobre o último) que mantém o Bitcoin seguro.

> :book: ***O que é um minerador?*** Um minerador é um participante na rede Bitcoin que trabalha para criar blocos. É um trabalho remunerado: quando um minerador cria um bloco com sucesso, ele recebe uma recompensa única mais as taxas pelas transações em seu bloco. A mineração é um grande negócio. Os mineradores tendem a utilizar hardwares especiais, projeto de maneiras que tornam mais provável que sejam capazes de criar blocos. Eles também tendem a fazer parte de pools de mineração (_mining pools_), onde todos os mineradores concordam em dividir as recompensas quando um deles cria um bloco com sucesso. 

Felizmente, `bitcoin-cli getunconfirmedbalance` ainda deve mostrar nosso saldo atualizado, desde que a transação inicial tenha sido criada:
```
$ bitcoin-cli getunconfirmedbalance
0.01010000
```

Se ainda estiver mostrando um zero, provavelmente estamos avançando neste tutorial rápido demais. Espere alguns segundos. As moedas devem aparecer não confirmadas e, em seguida, serão rapidamente confirmadas. Observe que uma moeda pode passar de um saldo não confirmado para o confirmado quase imediatamente, portanto, devemos nos certificar de verificar ambos. No entanto, se o `getbalance` e o `getunconfirmedbalance` ainda mostrarem zero nos próximos dez minutos, então provavelmente há algo errado com o faucet e precisaremos escolher outra.

### Ganhando Confiança no Nosso Dinheiro

Podemos usar `bitcoin-cli getbalance" * "[n]`, onde substituímos `[n]` por um inteiro, para ver se um saldo confirmado tem 'n' blocos de profundidade.

> :book: ***O que é a profundidade do bloco?*** Depois que um bloco é construído e confirmado, outro bloco é construído em cima dele e então outro... Como este é um processo estocástico, há alguma chance de reversão quando um bloco ainda é novo. Portanto, um bloco deve ser "enterrado" vários blocos na blockchain antes que possamos nos sentir totalmente confiantes com nossos fundos. Cada um desses blocos tende a ser construído em uma média de 10 minutos, então normalmente leva cerca de uma hora para uma transação confirmada receber seis blocos de profundidade, que é a medida para completa confiança no Bitcoin.

O seguinte mostra que nossas transações foram confirmadas uma vez, mas não duas vezes: 
```
$  bitcoin-cli getbalance "*" 1
0.01010000
$  bitcoin-cli getbalance "*" 2
0.00000000
```

Obviamente, a cada dez minutos mais ou menos essa profundidade aumentará.

É claro que, na Testnet, ninguém está tão preocupado com a confiabilidade dos fundos. Nós poderemos gastar nosso dinheiro assim que for confirmado.

## Verificando Nossa Carteira

O comando `bitcoin-cli getwalletinfo` fornece mais informações sobre o saldo da nossa carteira: 
```
$ bitcoin-cli getwalletinfo
{
  "walletname": "",
  "walletversion": 169900,
  "balance": 0.01010000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 2,
  "keypoololdest": 1592335137,
  "keypoolsize": 999,
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "keypoolsize_hd_internal": 1000,
  "paytxfee": 0.00000000,
  "private_keys_enabled": true,
  "avoid_reuse": false,
  "scanning": false
}
```

## Descobrindo o ID da Nossa Transação

Nosso dinheiro entrou na nossa carteira por meio de uma transação. Podemos descobrir o ID da transação (txid) com o comando `bitcoin-cli listtransactions`:
```
$ bitcoin-cli listtransactions
[
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.01000000,
    "label": "",
    "vout": 1,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 73,
    "blocktime": 1592600085,
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "walletconflicts": [
    ],
    "time": 1592599884,
    "timereceived": 1592599884,
    "bip125-replaceable": "no"
  },
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.00010000,
    "label": "",
    "vout": 0,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 72,
    "blocktime": 1592600085,
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "walletconflicts": [
    ],
    "time": 1592599938,
    "timereceived": 1592599938,
    "bip125-replaceable": "no"
  }
]

```
Isto mostra duas transações (`8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9`) e (` ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36`) para uma quantidade específica (`0.01000000` e `0.00010000`), que foram recebidas (`receive`) pelo mesmo endereço em nossa carteira (` mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE`) . À propósito, isso é uma boa prática para as chaves: devemos usar um novo endereço para cada Bitcoin que recebermos. Nesse caso, ficamos impacientes porque a primeira faucet parecia não estar funcionando.

Podemos acessar informações semelhantes com o comando `bitcoin-cli listunspent`, mas ele só mostra as transações para o dinheiro que não gastamos. Eles são chamados de UTXOs e serão de vital importância quando estivermos enviando dinheiro de volta para o mundo Bitcoin: 
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
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "vout": 1,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  }
]
```
Observe que os bitcoins não são simplesmente uma bagunça homogênea de dinheiro enfiado no nosso bolso. Cada transação individual que recebemos ou enviamos é colocada no livro razão da blockchain imutável, em um bloco. Podemos ver essas transações individuais quando olhamos para o nosso dinheiro não gasto. Isso significa que os gastos com bitcoins não são tão anônimos quanto pensamos. Embora os endereços sejam bastante privados, as transações podem ser examinadas à medida que entram e saem dos endereços. Isso torna a privacidade vulnerável à análise estatística. Também introduz uma potencial de não fungibilidade para os bitcoins, já que se pode rastrear uma série de transações, mesmo se não puder rastrear um "bitcoin" específico.

>: book: ***Por que todas essas quantidades de bitcoin em frações?*** Os bitcoins são produzidos lentamente e, portanto, há relativamente poucos em circulação. Como resultado, cada bitcoin na rede principal valem relativamente bastante (pouco mais de 9 mil dólares no momento em que este livro foi escrito). Isso significa que as pessoas geralmente trabalham com frações. Na verdade, o 0,0101 em moedas Testnet valeria cerca de $100 se eles estivessem na rede principal. Por esse motivo, surgiram nomes para quantidades menores de bitcoins, incluindo milibitcoins ou mBTCs (um milésimo de um bitcoin), microbitcoins ou bits ou μBTCs (um milionésimo de um bitcoin) e satoshis (um centésimo de milionésimo de um bitcoin).

## Examinando Nossa Transação

Podemos adquirir maiores informações em uma transação com o comando `bitcoin-cli gettransaction`:
```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9"
{
  "amount": 0.01000000,
  "confirmations": 1,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00"
}
```
O comando `gettransaction` detalhará transações que estão na nossa carteira, como esta, que nos foi enviada.

Observando o `gettransaction` temos dois argumentos opcionais:
```
$ bitcoin-cli help gettransaction
gettransaction "txid" ( include_watchonly verbose )

Get detailed information about in-wallet transaction <txid>

Arguments:
1. txid                 (string, required) The transaction id
2. include_watchonly    (boolean, optional, default=true for watch-only wallets, otherwise false) Whether to include watch-only addresses in balance calculation and details[]
3. verbose              (boolean, optional, default=false) Whether to include a `decoded` field containing the decoded transaction (equivalent to RPC decoderawtransaction)
```
Configurando esses dois como `true` ou `false`, podemos escolher se queremos incluir endereços "watch-only" no resultado (com os quais não nos importamos) ou vermos um resultado mais extenso (o que queremos).

Aqui estão os dados quando definimos `include_watchonly` como `false` e `verbose` como `true`. 
```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9" false true
{
  "amount": 0.01000000,
  "confirmations": 3,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00",
  "decoded": {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "hash": "d4ae2b009c43bfe9eba96dcd16e136ceba2842df3d76a67d689fae5975ce49cb",
    "version": 2,
    "size": 249,
    "vsize": 168,
    "weight": 669,
    "locktime": 1772327,
    "vin": [
      {
        "txid": "80f57b9779a769eb8772faf319265a46b94499d7d51df5db7a13b0d17749d014",
        "vout": 1,
        "scriptSig": {
          "asm": "0014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f",
          "hex": "160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f"
        },
        "txinwitness": [
          "304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e13301",
          "022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c"
        ],
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01010143,
        "n": 0,
        "scriptPubKey": {
          "asm": "OP_HASH160 5c3bfb36b03f279967977ca9d1e35185e3991778 OP_EQUAL",
          "hex": "a9145c3bfb36b03f279967977ca9d1e35185e399177887",
          "reqSigs": 1,
          "type": "scripthash",
          "addresses": [
            "2N1ev1WKevSsdmAvRqZf7JjvDg223tPrVCm"
          ]
        }
      },
      {
        "value": 0.01000000,
        "n": 1,
        "scriptPubKey": {
          "asm": "OP_DUP OP_HASH160 1b72503639a13f190bf79acf6d76255d772360b7 OP_EQUALVERIFY OP_CHECKSIG",
          "hex": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
          "reqSigs": 1,
          "type": "pubkeyhash",
          "addresses": [
            "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE"
          ]
        }
      }
    ]
  }
}
```
Agora podemos ver todas as informações sobre a transação, incluindo todas as entradas ("vin") e todas as saídas ("vout). Uma das coisas interessantes a se notar é que, embora tenhamos recebido 0,01 BTC na transação, outro 0.01010143 foi enviado para outro endereço. Provavelmente era um endereço de troco, um conceito que será explorado na próxima seção. É bastante comum uma transação ter várias entradas e/ou várias saídas.

Existe outro comando, `getrawtransaction`, que nos permite ver as transações que não estão em nossa carteira. No entanto, requer que tenhamos um node não prunado e `txindex = 1` em nosso arquivo `bitcoin.conf`. A menos que tenhamos uma necessidade séria de informações que não estão em nossa carteira, provavelmente é melhor usarmos um explorador de Bitcoin para esse tipo de coisa. 

## Opcional: Usando um Explorador de Blocos

Até mesmo olhar para as informações detalhadas de uma transação pode ser um pouco intimidante. O principal objetivo deste tutorial é ensinar como lidar com transações brutas da linha de comando, mas ficaremos felizes em falar sobre outras ferramentas quando elas forem aplicáveis. Uma dessas ferramentas é um explorador de blocos, que podemos usar para ver as transações de um navegador web em um formato muito mais amigável.

Atualmente, nosso explorador de bloco preferido é [https://mempool.space/](https://mempool.space/).

Podemos usá-lo para procurar transações para um endereço:

[https://mempool.space/pt/testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE](https://mempool.space/pt/testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE)

Também podemos usá-lo para ver transações individuais:

[https://mempool.space/pt/testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9](https://mempool.space/pt/testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9)

Um explorador de bloco geralmente não fornece mais informações do que uma linha de comando em uma transação bruta; ele apenas destaca as informações importantes e junta as peças do quebra-cabeça, incluindo as taxas de transação por trás de uma transação - outro conceito que abordaremos nas próximas seções. 

## Resumo: Recebendo uma Transação

As faucets nos vão dar dinheiro na Testnet. O dinheiro vem como transações brutas, que podem ser examinadas com `gettransaction` ou um explorador de blocos. Assim que recebermos uma transação, poderemos vê-la em nosso saldo e em nossa carteira.

## O Que Vem Depois?

Para sabermos como os endereços são descritos, para que eles possam ser transferidos ou transformados em partes de uma assinatura múltipla, podemos consultar [§3.5: Compreendendo o Descritor](03_5_Understanding_the_Descriptor.md).

Mas se isso for muito aprofundado, podemos continuar no [Capítulo Quatro: Enviando Transações no Bitcoin](04_0_Sending_Bitcoin_Transactions.md).
