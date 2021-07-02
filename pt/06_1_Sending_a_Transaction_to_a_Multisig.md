# 6.1: Enviando uma Transação Multsig

A primeira maneira de variar a forma como enviamos uma transação básica é usando uma multisig. Isso nos dá a capacidade de exigir que várias pessoas (ou, ao menos, várias chaves privadas) autorizem o uso dos fundos.

## Entendendo como Funcionam as Multisigs

Em uma transação P2PKH ou SegWit padrão, os bitcoins são enviados para um endereço baseado na chave pública, o que significa que a chave privada relacionada é necessária para desbloquear a transação, resolvendo o quebra-cabeça criptográfico e permitindo que reutilizemos o saldo. Mas e se pudéssemos bloquear uma transação com _múltiplas_ chaves privadas? Isso efetivamente permitiria que os fundos fossem enviados a um grupo de pessoas, onde todas teriam que concordar em reutilizar o saldo.

> :book: ***O que é uma multisignature ou multisig?*** Uma _multisignature_ é uma metodologia que permite que mais de uma pessoa crie uma assinatura digital em conjunto. É uma técnica para o uso criptográfico de chaves que vai muito além do Bitcoin.

Tecnicamente, um quebra-cabeça criptográfico com várias assinaturas é criado pelo Bitcoin usando o comando OP_CHECKMULTISIG e, normalmente, é encapsulado em um endereço P2SH. A seção [§10.4: Fazendo um scripto multisig](10_4_Scripting_a_Multisig.md) irá detalhar como isso funciona com mais precisão. Por enquanto, tudo que precisamos saber é que podemos usar o comando ```bitcoin-cli``` para criar endereços multi assinados. Os fundos podem ser enviados para esses endereços como qualquer endereço P2PKH ou Segwit normal, mas várias chaves privadas serão necessárias para que o saldo seja enviado.

> :book: ***O que é uma transação multisig?*** Uma transação com várias assinaturas é uma transação Bitcoin enviada para um endereço com várias assinaturas, exigindo assim que as pessoas de um grupo com várias assinaturas precisem assinar a transação para poder ter acesso ao saldo.

As multisigs simples exigem que todos no grupo assinem o UTXO quando estiver gasto. No entanto, há mais complexidade possível. As assinaturas múltiplas são geralmente descritas como sendo "M de N". Isso significa que a transação está presa com um grupo de chaves "N", mas apenas "M" delas são necessárias para desbloquear a transação.

> :book: ***O que é uma multisg M-de-N? *** Em uma multisig, as assinaturas "M" de um grupo de "N" são necessárias para formar a assinatura, onde "M ≤ N".

## Criando um Endereço Multisig

Para bloquear um UTXO com várias chaves privadas, devemos primeiro criar um endereço com várias assinaturas. Os exemplos usados ​​aqui mostram a criação (e uso) de uma multisig 2 de 2 (normalmente é assim que falamos quando nos referimos a um endereço multisg, descrevemos a quantidade de chaves e a quantidade necessária para desbloquear o saldo).

### Criando os Endereços

Para criar um endereço multisig, devemos primeiro preparar os endereços que o multisig irá combinar. A prática recomendada sugere que sempre criemos novos endereços. Isso significa que cada participante irá executar o comando ```getnewaddress``` em sua própria máquina:
```
machine1$ address1=$(bitcoin-cli getnewaddress)
```
E:
```
machine2$ address2=$(bitcoin-cli getnewaddress)
```
Posteriormente, um dos destinatários (ou talvez algum terceiro) precisará combinar ambos endereços.

#### Coletando as Chaves Públicas

No entanto, não podemos criar um multisig com os endereços, pois esses são os hashes das chaves públicas: Ao invés disso, precisamos das próprias chaves públicas.

Esta informação está disponível facilmente com o comando ```getaddressinfo```.

Na máquina remota, que assumimos aqui é ```machine2```, podemos obter as informações em uma lista.
```
machine2$ bitcoin-cli -named getaddressinfo address=$address2
{
  "address": "tb1qr2tkjh8rs9xn5xaktf5phct0wxqufplawrfd9q",
  "scriptPubKey": "00141a97695ce3814d3a1bb65a681be16f7181c487fd",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([fe6f2292/0'/0'/1']02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3)#zc64l8dw",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "1a97695ce3814d3a1bb65a681be16f7181c487fd",
  "pubkey": "02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3",
  "ischange": false,
  "timestamp": 1592957904,
  "hdkeypath": "m/0'/0'/1'",
  "hdseedid": "1dc70547f2b80e9bb5fde5f34fb3d85f8d8d1dab",
  "hdmasterfingerprint": "fe6f2292",
  "labels": [
    ""
  ]
}
```
O endereço ```pubkey``` (` 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3`) é o que precisamos. Vamos copiá-lo para nossa máquina local por qualquer meio que acharmos mais eficiente e _que seja menos sujeito a erros_.

Este processo precisa ser realizado para _cada_ endereço de uma máquina diferente daquela onde o multisig está sendo construída. Obviamente, se algum terceiro estiver criando o endereço, precisaremos fazer isso para cada endereço.

> :warning: **Atenção:** O uso de hashes de chave pública pelo Bitcoin como endereços, ao invés de chaves públicas, na verdade representa uma camada adicional de segurança. Porém, o envio de uma chave pública aumenta ligeiramente a vulnerabilidade do endereço associado, para alguma possibilidade no futuro distante de um comprometimento devido a curva elíptica. Não devemos nos preocupar em termos que enviar ocasionalmente uma chave pública para um uso como esse, mas devemos estar cientes de que os hashes de chave pública representam segurança e, portanto, as chaves públicas reais não devem ser enviadas de qualquer jeito para qualquer pessoa.

Se um dos endereços foi criado em nossa máquina local, que assumimos aqui que seja ```machine1```, podemos simplesmente colocar o endereço ```pubkey``` em uma nova variável.
```
machine1$ pubkey1=$(bitcoin-cli -named getaddressinfo address=$address1 | jq -r '.pubkey')
```

### Criando o Endereço

Uma multisig agora pode ser criada com o comando ```createmultisig```:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3"]'''
{
  "address": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "redeemScript": "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
  "descriptor": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y"
}
```
> :warning: **AVISO DE VERSÃO:** Algumas versões do ```createmultisig``` permitem a entrada de chaves públicas ou endereços, algumas requerem apenas as chaves públicas. Atualmente, ambas parecem funcionar.

Ao criar o endereço multisig, listamos quantas assinaturas são necessárias com o argumento ```nrequired``` (que é o "M" quando falamos _uma multisig "M de N"_), então listamos o conjunto total de assinaturas possíveis com a argumento ```keys``` (que é o "N"). Observe que as entradas ```keys``` provavelmente vieram de lugares diferentes. Nesse caso, incluímos ```$pubkey1``` da máquina local e ```02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3``` de uma máquina remota.

> :information_source: **NOTA - M de N vs N de N:** Este exemplo mostra a criação de um multisig simples 2 de 2 . Se quisermos criar uma assinatura M de N onde "M<N", precisamos apenas ajustar o campo ```nrequired``` e/ou o número de assinaturas ```keys``` no objeto JSON. Para um multisig 1 de 2, seria definido como ```nrequired = 1``` e também listaria duas chaves, enquanto para um multisig 2 de 3, seria necessário um ```nrequired = 2```, mas adicionaria mais uma chave pública à lista de ```keys```.

Quando usado corretamente, o ```createmultisig``` retorna três resultados, todos criticamente importantes.

O _endereço_ é o que iremos distribuir para as pessoas que desejam enviar os fundos. Podemos notar que ele tem um novo prefixo ```2```, exatamente como os endereços P2SH-SegWit. Isso porque, como eles, o comando ```createmultisig``` está na verdade criando um tipo de endereço totalmente novo chamado endereço P2SH. Ele funciona exatamente como um endereço P2PKH padrão para envio de fundos, mas como este foi criado para exigir vários endereços, precisaremos trabalhar um pouco mais para utilizá-los.

> :link: **TESTNET vs MAINNET:** Na Testenet, o prefixo para endereços P2SH é ```2```, enquanto na Mainnet é ```3```.

O _redeemScript_ é o que precisaremos para resgatar os fundos (junto com as chaves privadas de "M" dos "N" endereços). Este script é outro recurso especial dos endereços P2SH e será explicado com mais detalhes na seção [§8.1: Construindo um Script Bitcoin com P2SH](08_1_Building_a_Bitcoin_Script_with_P2SH.md). Por enquanto, precisamos apenas saber que alguns dados são necessários para podemos gastar nosso dinheiro.

O _descritor_ é a descrição padronizada para um endereço que encontramos na seção [§3.5: Compreendendo o descriptor](03_5_Understanding_the_Descriptor.md). Ele fornece uma maneira de importar esse endereço de volta para a outra máquina, usando o RPC ```importmulti```.

> :book: ***O que é um endereço P2SH?*** O P2SH significa Pay-To-Script-Hash. É um tipo de destinatário diferente de um endereço P2PKH padrão ou mesmo de um Bech32, usado para fundos cujo resgate é baseado em scripts de Bitcoin mais complexos. O ```bitcoin-cli``` usa o encapsulamento P2SH para ajudar a padronizar e simplificar os multisigs como "multisigs P2SH ", assim como P2SH-SegWit estava usando o P2SH para padronizar os endereços SegWit e torná-los totalmente compatíveis com as versões legadas.

> :warning: **Atenção:** endereços P2SH multisig, como os criados pelo ```bitcoin-cli```, têm um limite para "M" e "N" nos multisigs com base no tamanho máximo do script de resgate, que atualmente é de 520 bytes. Praticamente, não vamos chegar a isso, a menos que estejamos nos excedendo em algo.

### Salvando Nosso Trabalho

Aqui está uma informação importante: Nada sobre nosso multisig é salvo em nossa carteira usando essas técnicas básicas. Para resgatar o saldo enviado para este endereço multisig no futuro, precisaremos reter duas informações cruciais:

   * Uma lista dos endereços Bitcoin usados ​​no multisig, e;
   * A saída ```redeemScript``` criada pelo comando ```createmultsig```.

Tecnicamente, o ```redeemScript``` pode ser recriado executando novamente o ```createmultisig``` com a lista completa de chaves públicas _na mesma ordem_ e com a contagem correta de M e de N. Mas, é melhor agarrar-se a ela e evitar qualquer tipo de estresse futuro.

### Observando a Ordem

Aqui está mais uma coisa que devemos tomar muito cuidado: A _ordem é muito importante_. A ordem das chaves usadas para criar um multisig cria um hash único, ou seja, se colocarmos as chaves em uma ordem diferente, elas irão produzir um endereço diferente, conforme mostrado abaixo:
```
$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]'''
{
  "address": "2NFBQvz57UzKWDr2Vx5D667epVZifjGixkm",
  "redeemScript": "52210342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da0321039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b352ae",
  "descriptor": "sh(multi(2,0342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da03,039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3))#8l6hvjsk"
}
standup@btctest20:~$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey2'","'$pubkey1'"]'''
{
  "address": "2N5bC4Yc5Pqept1y8nPRqvWmFSejkVeRb1k",
  "redeemScript": "5221039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3210342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da0352ae",
  "descriptor": "sh(multi(2,039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3,0342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da03))#audl88kg"
}
```
Mais notavelmente, cada ordem cria um _redeemScript_ diferente. Isso significa que se usamos essas técnicas básicas e não conseguimos salvar o redemScript conforme as instruções, teremos que percorrer um número cada vez maior de variações para encontrar aquela correta quando tentarmos gastar os fundos!

O [BIP67](https://github.com/bitcoin/bips/blob/master/bip-0067.mediawiki) sugere uma maneira de ordenar lexicograficamente as chaves, de modo que sempre gerem as mesmas multisigs. A ColdCard e a Electrum estão entre as carteiras que já possuem suporte a isso. Claro, isso pode causar problemas por si só se não soubermos se um endereço multisig foi criado com chaves classificadas ou não classificadas. Mais uma vez, os [descritores](03_5_Understanding_the_Descriptor.md) vem ao nosso resgate. Se uma multisig não for classificada, ele será construído com a função ```multi``` e se for classificada, será construída com a função ```sortedmulti```.

Se olharmos o ```desc``` da multisig que criamos acima, veremos que o Bitcoin Core atualmente não classifica os multisigs:
```
  "descriptor": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y"
```
Porém, se ele importa um endereço do tipo ```sortedmulti```, ele fará o processo correto, que é o ponto principal dos descritores!

> :warning: **AVISO DE VERSÃO:** O Bitcoin Core só entende a função do descritor ```sortedmulti``` após a  versão 0.20.0. Podemos tentar acessar o descritor em uma versão anterior do Bitcoin Core e obteremos um erro como ```A function is needed within P2WSH```.

## Enviando Fundos para um Endereço Multisig

Se tivermos uma multisig em um formato P2SH conveniente, como o gerado pelo ```bitcoin-cli```, podemos enviar como um endereço normal.
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.000065}''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "hash": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "version": 2,
  "size": 83,
  "vsize": 83,
  "weight": 332,
  "locktime": 0,
  "vin": [
    {
      "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
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
      "value": 0.00006500,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
    }
  ]
}

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521
```
Como podemos ver, não houve nada de incomum na criação da transação e ela parece normal, embora com um endereço com um prefixo diferente do normal (```2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr```). Sem surpresa, como também não vimos diferença quando enviamos para endereços Bech32 pela primeira vez na seção [§4.6](04_6_Creating_a_Segwit_Transaction.md).

## Resumo: Enviando uma Transação Multsig

Os endereços do Multisig trancam os fundos usando várias chaves privadas, possivelmente exigindo todas essas chaves privadas para resgate e, possivelmente, exigindo apenas algumas do conjunto. Eles são fáceis de serem criadas com o ```bitcoin-cli``` e são simples para serem enviadas. Essa facilidade se deve em grande parte ao uso invisível de endereços P2SH (Pay-To-Script-Hash), um tópico extenso que já falamos duas vezes, com endereços P2SH-SegWit e agora com as multisig, e um outro que receberá mais atenção no futuro.

> :fire: ***Qual é o poder das multisig?*** As multisig permitem a modelagem de uma variedade de arranjos financeiros, como corporações, parcerias, comitês e outros grupos. Uma multisig 1 de 2 pode ser a conta bancária conjunta de um casal, enquanto uma multisig 2 de 2 pode ser usado para grandes despesas por uma parceria de responsabilidade limitada. As multisigs também constituem uma das bases dos Smart Contracts. Por exemplo, um negócio imobiliário pode ser fechado com um multisig 2 de 3, onde as assinaturas são enviadas pelo comprador, pelo vendedor e por um agente de custódia. Depois que o agente de custódia concorda que todas as condições foram atendidas, ele libera os fundos para o vendedor, ou, alternativamente, o comprador e o vendedor podem liberar os fundos em conjunto.

## O Que Vem Depois?

Vamos continuar "Expandindo as transações de Bitcoin com multisigs" na seção [§6.2: Gastando uma transação Multsig](06_2_Spending_a_Transaction_to_a_Multisig.md).