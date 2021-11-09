# 9.5: Programando um P2WPKH

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão. Cautela, leitor.

Os P2PKHs são bons para explicar a maneira fundamental de como os scripts de Bitcoin funcionam, mas por que os scripts SegWit P2WPKH nativos estão se tornando cada vez mais a maioria das transações no Bitcoin? Acontece que os endereços P2WPKH não usam Scripts de Bitcoin como os endereços de Bitcoin tradicionais fazem e, portanto, esta seção é realmente uma digressão do script deste capítulo, mas ela é importante, porque descreve a _outra_ maneira principal pela qual bitcoins podem ser transacionados.

## Observando um Script P2WPKH

É fácil ver a aparência de um script P2WPKH. A transação bruta abaixo foi criada gastando um UTXO P2WPKH e, em seguida, enviando o dinheiro para um endereço de troco P2WPKH, assim como fizemos com um endereço legado na seção [§9.1](09_1_Understanding_the_Foundation_of_Transactions.md).
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "bdf8f12768a9870d41ac280f8bb4f8ecd9d2fa66fffc75606811f5751c17cb3a",
  "hash": "ec09c84cae48694bec7fd3461b3c5b38a76829c56e9d876037bf2484d443174b",
  "version": 2,
  "size": 191,
  "vsize": 110,
  "weight": 437,
  "locktime": 0,
  "vin": [
    {
      "txid": "3f5417bc7a3a4144d715f3f006d35ea2b405f06091cbb9ce492e04ccefe02b18",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001",
        "03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 92a0db923b3a13eb576a40c4b35515aa30206cba",
        "hex": "001492a0db923b3a13eb576a40c4b35515aa30206cba",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qj2sdhy3m8gf7k4m2grztx4g44gczqm96y6sszv"
        ]
      }
    }
  ]
}
```
Provavelmente existem duas coisas surpreendentes aqui: (1) Não há ```scriptSig``` para desbloquear a transação anterior e; (2) o ```scriptPubKey``` para bloquear a nova transação é apenas ```0 92a0db923b3a13eb576a40c4b35515aa30206cb```.

Isso é assim porque o P2WPKH funciona de forma diferente!

## Compreendendo uma Transação P2WPKH

Uma transação P2WPKH contém todas as mesmas informações que uma transação P2PKH clássica, mas a coloca em lugares diferentes, não dentro de um script Bitcoin tradicional, e essa é a cereja do bolo das transações SegWit, pois retira as informações "witness" (testemunhas), que são as chaves públicas e as assinaturas, para fora da transação, dando suporte a uma mudança de tamanho do bloco.

Mas, se olharmos com cuidado, veremos que o ```scriptSig``` vazio foi substituído por duas entradas em uma nova seção chamada ```txinwitness```. Se examinarmos os tamanhos e formatações, eles devem parecer familiares, afinal, elas são uma assinatura e uma chave pública. Da mesma forma, se olharmos o `scriptPubKey`, veremos que é feito de um ```0``` (na verdade, de um ```OP_0```, que é o número da versão do SegWit) e outro número longo, que é o hash de chave pública.

Aqui está uma comparação dos nossos dois exemplos:
| Tipo | PubKeyHash | PubKey | Signature |
|----------------|----------|-------------|---------|
| SegWit | 92a0db923b3a13eb576a40c4b35515aa30206cba | 03839e6035b33e37597908c83a2f992ec835b093d65790f43218cb49ffe5538903 | 3044022064f633ccfc4e937ef9e3edcaa9835ea9a98d31fbea1622c1d8a38d4e7f8f6cb602204bffef45a094de1306f99da055bd5a603a15c277a59a48f40a615aa4f7e5038001 |
| SegWit | 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c |

Então, como isso tudo funciona? Depende da interpretação do código antigo como uma transação válida e do conhecimento do novo código para verificar as novas informações "testemunhas".

### Lendo um Script SegWit em uma Máquina Antiga

Se um node não foi atualizado para suportar SegWit, então ele faz o seu truque usual de concatenar o ```scriptSig``` e o ```scriptPubKey```. Isso produz o seguinte: ```0 92a0db923b3a13eb576a40c4b35515aa30206cba``` (porque há apenas um ```scriptPubKey```). Ao executar isso, iremos produzir uma pilha com tudo na ordem inversa:
```
$ btcdeb '[0 92a0db923b3a13eb576a40c4b35515aa30206cba]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
miniscript failed to parse script; miniscript support disabled
valid script
2 op script loaded. type `help` for usage information
script                                   |  stack 
-----------------------------------------+--------
0                                        | 
92a0db923b3a13eb576a40c4b35515aa30206cba | 
#0000 0
btcdeb> step
    <> PUSH stack 
script                                   |  stack 
-----------------------------------------+--------
92a0db923b3a13eb576a40c4b35515aa30206cba | 0x
#0001 92a0db923b3a13eb576a40c4b35515aa30206cba
btcdeb> step
    <> PUSH stack 92a0db923b3a13eb576a40c4b35515aa30206cba
script                                   |                                   stack 
-----------------------------------------+-----------------------------------------
                                         | 92a0db923b3a13eb576a40c4b35515aa30206cba
                                         | 0x
```
Os scripts do Bitcoin são considerados bem-sucedidos se houver algo na pilha e ele for diferente de zero, portanto, os scripts SegWit são automaticamente bem-sucedidos nos nodes antigos, desde que o ```scriptPubKey``` seja criado corretamente com um hash de uma pub-key diferente de zero. Isso é chamado de transação _"anyone-can-spend"_, ou no português, _"qualquer-um-pode-gastar"_, porque os nodes antigos os verificaram como corretos sem a necessidade de assinaturas.

> :book: ***Os nodes antigos não podem roubar os UTXOs do SegWit?*** O SegWit foi habilitado na rede Bitcoin quando 95% dos mineradores sinalizaram que estavam prontos para começar a utilizá-lo. Isso significa que apenas 5% dos nodes naquele momento podem ter registrado transações SegWit que _qualquer-um-pode-gastar_ como sendo válidas, sem passar pelo trabalho adequado de verificação dos ```txinwitness```. Se  incorporassem incorretamente um UTXO inválido que _qualquer-um-pode-gastar_ em um bloco, os outros 95% dos nodes se recusariam a validar esse bloco, e assim ele ficaria rapidamente órfão ao invés de ser adicionado à blockchain "principal". (Certamente, 51% dos nodes poderiam escolher parar de interpretar as transações SegWit corretamente, mas 51% dos nodes podem fazer qualquer coisa em uma rede de consenso, como uma blockchain).

Como os nodes antigos sempre acham que os scripts SegWit estão corretos, eles sempre os verificam, mesmo sem entender seu conteúdo.

### Lendo um Script SegWit em uma Máquina Nova

Uma máquina que entende como o SegWit funciona faz exatamente as mesmas coisas que faria com um script P2PKH antigo, mas não usa o script por si só. Ela apenas sabe que precisa fazer o hash da chave pública no ```txinwitness```, verificando-a usando a chave hash após o número da versão no ```scriptPubKey``` e, em seguida, executando o ```OP_CHECKSIG``` na assinatura e na chave pública do ```txinwitness```.

Portanto, é outra maneira de fazer a mesma coisa, mas sem ter os scripts embutidos nas transações. Ao invés disso, o processo é integrado ao software do node.

## Resumo: Programando um P2WPKH

Em grande parte, _não_ programamos um P2WPKH. Ao invés disso, o Bitcoin Core cria a transação de uma maneira diferente, colocando as informações da testemunha em um lugar diferente ao invés de um ```scriptSig``` tradicional. Isso significa que os P2WPKHs são uma digressão dos scripts de Bitcoin desta parte do livro, porque são uma expansão do Bitcoin que se distancia dos scripts tradicionais.

No entanto, o SegWit também usa os scripts do Bitcoin de maneira inteligente. Sabendo que haveria nodes que não seriam atualizados e que precisariam permanecer compatíveis com versões anteriores, os desenvolvedores criaram o formato P2WPKH de forma que gerasse um script que sempre seria validado nos nodes antigos (embora ainda tivesse aquele script fornecendo informações para os nodes novos em uma forma de um número de versão e uma chave pública com hash).

Quando estamos programando usando a linha de comando, fundamentalmente não precisamos nos preocupar com isso, a não ser com saber que não encontraremos os scripts tradicionais em transações brutas do tipo SegWit (o que, novamente, era o ponto desta seção).

## O Que Vem Depois?

Vamos continuar "Programando no Bitcoin" no [Capítulo 10: Incorporando Scripts em Transações P2SH no Bitcoin](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md).