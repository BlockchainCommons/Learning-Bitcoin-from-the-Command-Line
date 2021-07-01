# 4.6: Criando uma Transação SegWit

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um esboço que ainda pode estar aguardando revisão. Portanto, leitor, tenha cuidado.

Era uma vez, nos céus do Bitcoin, uma guerra entre os tamanhos de blocos. As taxas disparavam e os usuários estavam preocupados se o Bitcoin podia realmente escalar. Os desenvolvedores do Bitcoin Core relutaram em simplesmente aumentar o tamanho do bloco, mas chegaram a um acordo: fizeram o SegWit, que significa Segregated Witness. Segregated Witness é uma maneira elegante de dizer "Assinatura Separada". Ele cria novos tipos de transações que removem assinaturas no final da transação. Ao combinar isso com o aumento dos tamanhos de bloco que são visíveis apenas para nodes atualizados, o SegWit resolveu os problemas de dimensionamento do Bitcoin na época e também resolveu um bug de maleabilidade desagradável, tornando o dimensionamento ainda melhor para protocolos de segunda camada, como a Lightning Network.

A sacada? O SegWit usa endereços diferentes, alguns dos quais são compatíveis com nodes mais antigos, ao passo que outros não.

> :warning: **AVISO DE VERSÃO:** O SegWit foi introduzido no BitCoin 0.16.0, que foi descrito na época como tendo "suporte total". Dito isso, havia algumas falhas na integração com o ```bitcoin-cli``` na época, que impediam a assinatura de funcionar corretamente em novos endereços P2SH-SegWit. O endereço Bech32, não compatível com versões anteriores, também foi introduzido no Bitcoin 0.16.0 e se tornou o tipo de endereço padrão no Bitcoin 0.19.0. Toda essa funcionalidade deve estar totalmente funcional em relação às funções ```bitcoin-cli``` (e, portanto, devem funcionar completamente neste tutorial).

> O problema está em interagir com o mundo. Todos devem ser capazes de enviar para um endereço P2SH-SegWit porque foi construído propositadamente para suportar compatibilidade com as versões anteriores, envolvendo a funcionalidade SegWit em um Script Bitcoin. O mesmo não é verdade para endereços Bech32: Se alguém nos disser que não pode enviar para o nosso endereço Bech32 precisaremos gerar um endereço ```legacy``` ou P2SH-SegWit para fazer a transação. (Muitos sites, principalmente as corretoras, também não podem gerar ou receber em endereços SegWit, particularmente endereços Bech32, mas isso é um problema totalmente diferente e não afeta o uso delas).

## Compreendendo uma Transação SegWit

Em transações clássicas, as informações de assinatura (witness) eram armazenadas no meio da transação, enquanto nas transações SegWit, elas ficam na parte inferior. Isso anda de mãos dadas com os aumentos de tamanho do bloco que foram introduzidos na atualização do SegWit. O tamanho do bloco foi aumentado de 1 mega para um valor variável com base em quantas transações SegWit estão em um bloco, começando em 1 mega (sem transações SegWit) e podendo chegar a 4 megas (caso todas as transações sejam SegWit). Este tamanho variável foi criado para acomodar os nodes clássicos, de forma que tudo permaneça compatível com as versões anteriores. Se um node clássico vê uma transação SegWit, ele joga fora as informações da witness (resultando em um bloco de tamanho menor, abaixo do antigo limite de 1 mega), enquanto se um novo node vê uma transação SegWit, ele mantém as informações da witness (resultando em um maior tamanho de bloco, até o novo limite de 4 megas).

Portanto, acabamos de responder o quê são e como funcionam as transações SegWit. Não que precisemos saber disso para usá-las. A maioria das transações na rede Bitcoin são SegWit. Elas são aquilo que iremos utilizar nativamente para as transações e recebimentos de bitcoins. A este ponto, os detalhes não são mais relevantes do que os detalhes de como grande parte do Bitcoin funciona.

## Criando um Endereço SegWit

Criamos um endereço SegWit da mesma maneira que qualquer outro endereço, com os comandos ```getnewaddress``` e ```getrawchangeaddress```.

Se precisarmos criar um endereço para alguém que não pode enviar para os endereços Bech32 mais recentes, podemos usar um endereço ```p2sh-segwit```:
```
$ bitcoin-cli -named getnewaddress address_type=p2sh-segwit
2N5h2r4karVqN7uFtpcn8xnA3t5cbpszgyN
```
Se conseguirmos ver um endereço com o prefixo "2" significa que fizemos tudo certo.

> :link: **TESTNET vs MAINNET:** "3" para a Mainnet.

No entanto, se a pessoa com quem estamos interagindo tem um node com uma versão mais nova, ela poderá enviar para um endereço Bech32, que criamos usando os comandos da maneira padrão:
```
$ bitcoin-cli getnewaddress
tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6
```
Como já vimos, os endereços de troco gerados a partir do ```bitcoin-cli``` interagem bem com os endereços Bech32, então não há motivo nenhum para usar o sinalizador ```legacy``` lá também:
```
$ bitcoin-cli getrawchangeaddress
tb1q05wx5tyadm8qe83exdqdyqvqqzjt3m38vfu8ff
```

Aqui, podemos observar que o prefixo "tb1" exclusivo denota que o endereço é um Bech32.

> :link: **TESTNET vs MAINNET:** "bc1" para a mainnet.

O bitcoin-cli não se importa com o tipo de endereço que estamos utilizando. Podemos executar um comando como ```listaddressgroupings``` que ele irá misturar os endereços livremente, não importando os tipos:
```
$ bitcoin-cli listaddressgroupings
[
  [
    [
      "mfsiRhxbQxcD7HLS4PiAim99oeGyb9QY7m",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      0.00000000,
      ""
    ],
    [
      "tb1q6dak4e9fz77vsulk89t5z92l2e0zm37yvre4gt",
      0.00000000
    ]
  ],
  [
    [
      "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
      0.00022000,
      ""
    ]
  ],
  [
    [
      "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
      0.00000000
    ],
    [
      "mqjrdY5raxKzXQf5t2VvVvzhvFAgersu9B",
      0.00000000
    ],
    [
      "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
      0.00000000,
      ""
    ],
    [
      "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
      0.00007800
    ]
  ],
  [
    [
      "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
      0.01000000,
      ""
    ]
  ]
]
```

## Enviando uma Transação SegWit da Maneira Fácil

Então, como enviamos uma transação Segwit? Exatamente como qualquer outra transação. Não importa se o UTXO é SegWit, o endereço é SegWit ou alguma combinação dos dois. Podemos ter a certeza que o ```bitcoin-cli``` irá fazer a coisa certa. Embora possamos perceber algumas diferenças nos endereços, eles não importam para interagir com as coisas no nível do ```bitcoin-cli``` ou do RPC. (E esta é uma das vantagens de usar a linha de comando e a interface do RPC, conforme sugerido neste tutorial: os especialistas já fizeram o trabalho duro para nós, incluindo coisas como enviar para endereços legacy e Bech32. Acabamos usando essa funcionalidade para nosso próprio benefício).

Aqui está um exemplo de um envio para um endereço SegWit, da maneira fácil:
```
$ bitcoin-cli sendtoaddress tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx 0.005
854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42
```
Se olhar para a nossa transação, podemos ver o uso do endereço Bech32:
```
$ bitcoin-cli gettransaction txid="854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42" verbose=true
{
  "amount": -0.00500000,
  "fee": -0.00036600,
  "confirmations": 0,
  "trusted": true,
  "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
  "walletconflicts": [
  ],
  "time": 1592948795,
  "timereceived": 1592948795,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00500000,
      "vout": 1,
      "fee": -0.00036600,
      "abandoned": false
    }
  ],
  "hex": "0200000002114d5a4c3b847bc796b2dc166ca7120607b874aa6904d4a43dd5f9e0ea79d4ba010000006a47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042dfeffffffa71321e81ef039af490251379143f7247ad91613c26c8f3e3404184218361733000000006a47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7efeffffff026854160000000000160014d591091b8074a2375ed9985a9c4b18efecfd416520a1070000000000160014751e76e8199196d454941c45d1b3a323f1433bd6c60e1b00",
  "decoded": {
    "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "hash": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "version": 2,
    "size": 366,
    "vsize": 366,
    "weight": 1464,
    "locktime": 1773254,
    "vin": [
      {
        "txid": "bad479eae0f9d53da4d40469aa74b8070612a76c16dcb296c77b843b4c5a4d11",
        "vout": 1,
        "scriptSig": {
          "asm": "304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7[ALL] 03ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d",
          "hex": "47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d"
        },
        "sequence": 4294967294
      },
      {
        "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
        "vout": 0,
        "scriptSig": {
          "asm": "304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a316[ALL] 0339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e",
          "hex": "47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e"
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01463400,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
          ]
        }
      },
      {
        "value": 0.00500000,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 751e76e8199196d454941c45d1b3a323f1433bd6",
          "hex": "0014751e76e8199196d454941c45d1b3a323f1433bd6",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx"
          ]
        }
      }
    ]
  }
}
```
Na verdade, ambos ```vouts``` usam endereços Bech32: o nosso destinatário e o endereço de troco gerado automaticamente.

Mas quando retrocedemos nosso ```vin```, descobrimos que veio de um endereço legacy. Porque isso não importa:
```
$ bitcoin-cli -named gettransaction txid="33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7"
{
  "amount": 0.01000000,
  "confirmations": 43,
  "blockhash": "00000000000000e2365d2f814d1774b063d9a04356f482010cdfdd537b1a24bb",
  "blockheight": 1773212,
  "blockindex": 103,
  "blocktime": 1592937103,
  "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
  "walletconflicts": [
  ],
  "time": 1592936845,
  "timereceived": 1592936845,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "020000000001016a66efa334f06e2c54963e48d049a35d7a1bda44633b7464621cae302f35174a0100000017160014f17b16c6404e85165af6f123173e0705ba31ec25feffffff0240420f00000000001976a914626ab1ca41d98f597d18d1ff8151e31a40d4967288acd2125d000000000017a914d5e76abfe5362704ff6bbb000db9cdfa43cd2881870247304402203b3ba83f51c1895b5f639e9bfc40124715e2495ef2c79d4e49c0f8f70fbf2feb02203d50710abe3cf37df4d2a73680dadf3cecbe4f2b5d0b276dbe7711d0c2fa971a012102e64f83ee1c6548bcf44cb965ffdb803f30224459bd2e57a5df97cb41ba476b119b0e1b00"
}
```

## Enviando uma Transação SegWit da Maneira Difícil

Da mesma forma, podemos financiar uma transação com um endereço Bech32, sem nenhuma diferença em relação às técnicas que aprendemos até agora. Aqui está uma maneira exata de fazer isso com uma transação bruta completa:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
tb1q4xje3mx9xn7f8khv7p69ekfn0q72kfs8x3ay4j
$ bitcoin-cli listunspent
[
...
  {
    "txid": "003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452",
    "vout": 0,
    "address": "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
    "label": "",
    "scriptPubKey": "0014a226e1dfd08537b02de04f667a49bd46f9b9f578",
    "amount": 0.01000000,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/0'/5']0327dbe2d58d9ed2dbeca28cd26e18f48aa94c127fa6fb4b60e4188f6360317640)#hd66hknp",
    "safe": true
  }
]
$ recipient=tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
$ echo $utxo_txid $utxo_vout
003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452 0
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.002, "'$changeaddress'": 0.007 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e02568b706b21bcb56fcf9c4bb7ba63fdbdec1cf2866168c4f50bc0ad693f26c
```
Tudo funciona exatamente da mesma forma que outros tipos de transações!

### Reconhecendo o Novo Descritor

Se olharmos o campo ```desc```, notaremos que o endereço SegWit tem um descritor de estilo diferente daqueles encontrados na seção [§3.5: Compreendendo o Descritor](03_5_Understanding_the_Descriptor.md). Um descritor legacy descrito nessa seção se parecia com algo assim: `pkh ([d6043800 / 0 '/ 0' / 18 '] 03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388) # 4ahsl9pk`. Nosso novo descritor SegWit se parece mais com isso: `wpkh ([d6043800 / 0 '/ 0' / 5 '] 0327dbe2d58d9ed2dbeca28cd26e18f48aa94c127fa6fb4b60e4188f6360317640) # hd66hknp" `.

A grande diferença que precisamos notar é que a função mudou. Anteriormente, era ```pkh```, que é um endereço padrão de chave pública com hash P2PKH. Ao invés disso, o endereço SegWit é ```wpkh```, o que significa que é um endereço SegWit P2WPKH nativo. Isto destaca o :fire: ***poder dos descritores***. Eles descrevem como criar um endereço a partir de uma chave ou outra informação, com as funções definindo de forma inequívoca como fazer o endereço com base em seu tipo.

## Resumo: Criando uma Transação SegWit

Realmente não há complexidade para criar transações SegWit. Internamente, elas são estruturadas de forma diferente das transações legacy, mas na linha de comando não existe diferença: apenas usamos um endereço com um prefixo diferente. A única coisa a ser observada é que algumas pessoas podem não conseguir enviar para um endereço Bech32 se estiverem usando um software obsoleto.

> :fire: ***Qual é o poder de enviar transações com SegWit?***
> _As vantagens._ As transações SegWit são menores e, portanto, serão mais baratas de serem enviadas do que as transações legacy devido às taxas mais baixas. O Bech32 diminui essa vantagem e também cria endereços que são mais difíceis de errar durante a transcrição, e isso é muito importante, visto que o erro do usuário é uma das maneiras mais prováveis de perderem seus bitcoins.
> _As desvantagens._ Os endereços SegWit não tem suporte em nodes Bitcoin obsoletos. Em particular, as pessoas podem não conseguir enviar para o nosso endereço Bech32.

## O Que Vem Depois?

Vamos avançar mais um pouco no "bitcoin-cli" com o [Capítulo 5: Controlando Transações no Bitcoin](05_0_Controlling_Bitcoin_Transactions.md).