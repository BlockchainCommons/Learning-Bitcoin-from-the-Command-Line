# 5.3: Financiando uma Transação com CPFP

Se nossa transação do Bitcoin travar e formos os _recebedores_, poderemos aumentar a velocidade usando o CPFP (Child-Pays-For-Parent). Esta é uma alternativa semelhante ao que a parte que _ envia_ o saldo pode fazer usando o RBF.

> :warning: **AVISO DE VERSÃO:** Esta é uma inovação do Bitcoin Core v0.13.0, o que novamente significa que a maioria das pessoas já deve estar utilizando-a.

## Entendendo o Funcionamento do CPFP

O RBF é possível caso você seja a parte que está enviando o saldo. O remetente errou e precisou aumentar a taxa, ou queria ser inteligente e combinar transações por diversos motivos. O RBF é um poderoso recurso voltado para o remetente. De certa forma, o CPFP é o oposto do RBF, pois dá poder ao destinatário que sabe que o dinheiro ainda não chegou e quer acelerar o processo. No entanto, também é um recurso muito mais simples, com aplicabilidade não tão ampla quanto a primeira.

Basicamente, a ideia do CPFP é que um destinatário tenha uma transação que não foi confirmada e deseja gastar o seu saldo. Portanto, ele inclui essa transação não confirmada em uma nova transação e paga uma taxa alta o suficiente para encorajar um minerador a incluir a transação original (pai) e a nova transação (filha) em um bloco. Como resultado, as duas transações são confirmadas e eliminadas da mempool simultaneamente.

Devemos observar que o CPFP não é um recurso novo no protocolo, assim como o RBF. É apenas um novo esquema de incentivo que pode ser usado para a seleção de transações pelos mineradores. Isso também significa que não é tão confiável quanto uma alteração feita pelo RBF: Pode haver motivos para que o filho não seja selecionado para ser colocado em um bloco e isso impedirá que o pai também seja colocado no bloco.

## Gastando UTXOs Não Confirmados

Financiar uma transação com o CPFP é um processo muito simples, usando os métodos com os quais já estamos familiarizados:

   1. Encontre o ```txid``` e ```vout``` da transação não confirmada. Esta será a parte mais complicada, já que o ```bitcoin-cli``` geralmente tenta nos proteger de transações não confirmadas. O remetente pode enviar essas informações para nós, mesmo com apenas o ```txid```, devemos ser capazes de descobrir o ```vout``` em um explorador da blockchain;

Temos uma outra opção: Podemos usar o ```bitcoin-cli getrawmempool```, que pode ser usado para listar o conteúdo de toda a nossa mempool, onde estarão as transações não confirmadas. Podemos vasculhar várias transações se nossa mempool estiver cheia. Podemos obter mais informações sobre uma transação específica usando o comando ```bitcoin-cli getrawtransaction``` com o sinalizador verbose definido como ```true```:
```
$ bitcoin-cli getrawmempool
[
  "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061"
]

$ bitcoin-cli getrawtransaction 95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061 true
{
  "txid": "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061",
  "hash": "9729e47b8aee776112a82cec46df7638d112ca51856c53e238a9b1f7af3be4ce",
  "version": 2,
  "size": 247,
  "vsize": 166,
  "weight": 661,
  "locktime": 1773277,
  "vin": [
    {
      "txid": "7a0178472300247d423ac4a04ff9165fa5b944104f6d6f9ebc557c6d207e7524",
      "vout": 0,
      "scriptSig": {
        "asm": "0014334f3a112df0f22e743ad97eec8195a00faa59a0",
        "hex": "160014334f3a112df0f22e743ad97eec8195a00faa59a0"
      },
      "txinwitness": [
        "304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae01",
        "03574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.01000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 f079f77f2ef0ef1187093379d128ec28d0b4bf76 OP_EQUAL",
        "hex": "a914f079f77f2ef0ef1187093379d128ec28d0b4bf7687",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5"
        ]
      }
    },
    {
      "value": 0.02598722,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 8799be12fb9eae6644659d95b9602ddfbb4b2aff OP_EQUAL",
        "hex": "a9148799be12fb9eae6644659d95b9602ddfbb4b2aff87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N5cDPPuCTtYq13oXw8RfpY9dHJW8sL64U2"
        ]
      }
    }
  ],
  "hex": "0200000000010124757e206d7c55bc9e6f6d4f1044b9a55f16f94fa0c43a427d2400234778017a0000000017160014334f3a112df0f22e743ad97eec8195a00faa59a0feffffff0240420f000000000017a914f079f77f2ef0ef1187093379d128ec28d0b4bf768742a727000000000017a9148799be12fb9eae6644659d95b9602ddfbb4b2aff870247304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae012103574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9dd0e1b00"
}
```
Vamos observar o vetor ```vout```. Assim, encontramos o objeto que corresponde ao nosso endereço. No nosso exemplo ele é o único. O valor ```n``` é o nosso ```vout```. Agora temos tudo que precisamos para criar uma nova transação CPFP.
```
$ utxo_txid=2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5
$ utxo_vout=0
$ recipient2=$(bitcoin-cli getrawchangeaddress)
```

   2. Criar uma transação bruta usando a transação não confirmada como entrada;
   3. Dobrar as taxas de transação (ou colocar mais do que isso);

Ao seguir essas etapas, tudo deve parecer igual ao que já fizemos, apesar de estarmos trabalhando com uma transação não confirmada. Para verificar se tudo está bem, podemos até olhar os resultados de nossa assinatura antes de salvarmos as informações em uma variável:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient2'": 0.03597 }''')

$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex'
02000000012b137ef780666ba214842ff6ea2c3a0b36711bcaba839c3710f763e3d9687fed000000006a473044022003ca1f6797d781ef121ba7c2d1d41d763a815e9dad52aa8bc5ea61e4d521f68e022036b992e8e6bf2c44748219ca6e0056a88e8250f6fd0794dc69f79a2e8993671601210317b163ab8c8862e09c71767112b828abd3852e315441893fa0f535de4fa39b8dffffffff01905abd07000000001976a91450b1d90a130c4f3f1e5fbfa7320fd36b7265db0488ac00000000

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e
```

   4. Cruzar os dedos não é necessário. Verificamos que nossos dados estão corretos. Deste ponto em diante, as coisas estão fora de nosso controle.

Nossas transações podem ser processadas rapidamente, ou não. Tudo depende se os mineradores que estão gerando aleatoriamente os blocos possuem o patch CPFP ou não. Apesar disso, fizemos tudo o que pudemos.

E isso realmente é tudo o que podemos fazer.

### Atenção aos Nuances

Embora o CPFP seja geralmente descrito como sendo um destinatário que usa uma nova transação para pagar por uma antiga que não foi confirmada, existem alguns nuances.

A parte que está _enviando_ poderia usar o CPFP para liberar uma transação se recebesse algum troco. Ele apenas usaria essa mudança como uma entrada, e o uso resultante do CPFP liberaria toda a transação. Lembre-se de que ele poderia fazer algo melhor se usasse o RBF, desde que estivesse habilitado, pois as taxas totais seriam menores.

A parte que está _recebendo_ pode usar o CPFP mesmo se não estiver planejando gastar o dinheiro imediatamente, por exemplo, se estiver preocupado que os fundos possam não ser reenviados se a transação expirar. Nesse caso, ele apenas cria uma transação secundária que envia todo o dinheiro (menos a taxa de transação) para um endereço de troco. Isso é o que fizemos no nosso exemplo.

## Resumo: Financiando uma Transação com CPFP

Podemos aproveitar os incentivos do CPFP para liberar fundos que nos foram enviados, mas que não foram confirmados. Basta usar a transação não confirmada como sendo um UTXO e pagar uma taxa de transação acima da média.

> :fire: ***Qual é o poder do CPFP?*** O seu uso é apenas para liberar fundos quando formos os recebedores dos fundos e o remetente não quer ajudar por qualquer que seja o motivo. Ele não tem as mesmas habilidades que o RBF, mas é uma maneira alternativa de exercer controle sobre uma transação depois que ela foi colocada na mempool, mas antes de ser confirmada em um bloco.

## O Que Vem Depois?

Vamos avançar para o [Capítulo 6: Expandindo Transações no Bitcoin com Multisigs](06_0_Expanding_Bitcoin_Transactions_Multisigs.md).