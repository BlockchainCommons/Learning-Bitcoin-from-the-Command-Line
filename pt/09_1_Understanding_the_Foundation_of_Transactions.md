# 9.1: Compreendendo a Base das Transações

A base do Bitcoin é a capacidade de proteger transações, algo que é feito com uma linguagem de programação simples.

## Conhecendo as Partes do Quebra-Cabeça Criptográfico

Conforme descrito no [Capítulo 1](01_0_INTRODUCING_BITCOIN.MD), os saldos em cada transação Bitcoin estão travados com um quebra-cabeça criptográfico. Para sermos mais precisos, dizemos que o Bitcoin é composto de "uma sequência de transações atômicas". Observamos que: "Cada transação é autenticada por um remetente com a solução para um quebra-cabeça criptográfico anterior que foi armazenado como um script. A nova transação é bloqueada para o destinatário com um novo quebra-cabeça criptográfico que também é armazenado como um script". Esses scripts, que bloqueiam e desbloqueiam as transações, são escritos usando o Bitcoin Script.

> :book: ***O que é o Bitcoin Script?*** O Bitcoin Script é uma linguagem baseada em pilhas (ou _stacks_), similar à linguagem de programação Forth que propositadamente evita loops, o que significa que ela não é Turing-completa. É composta de opcodes individuais. Cada transação no Bitcoin é bloqueada com um script do Bitcoin. Quando a transação de bloqueio para um UTXO é executada com as entradas corretas, esse UTXO pode então ser gasto.

O fato de que as transações estão bloqueadas com scripts significa que podem ser bloqueadas de várias maneiras diferentes, exigindo uma variedade de chaves diferentes. Na verdade, encontramos vários mecanismos de travamento diferentes até o momento, cada um dos quais usa diferentes opcodes:

   * OP_CHECKSIG, que verifica uma chave pública contra uma assinatura, é a base do endereço P2PKH clássico, como será totalmente detalhado na seção [§9.4: Programando um P2PKH](09_4_Scripting_a_P2PKH.md);
   * OP_CHECKMULTISIG, faz a mesma coisa com os multisigs, como será totalmente detalhado na seção [§10.4: Programando um Multisig](10_4_Scripting_a_Multisig.md);
   * OP_CHECKLOCKTIMEVERIFY e OP_SEQUENCEVERIFY formam a base dos timelocks mais complexos, como será totalmente detalhado nas seções [§11.2: Usando CLTV em Scripts](11_2_Using_CLTV_in_Scripts) e [§11.3: Usando CSV em Scripts](11_3_Using_CSV_in_Scripts.md).
   * OP_RETURN é a marca de uma transação não gasta, e é por isso que é usado para transportar dados, como falamos na seção [§8.2: Enviando uma Transação com Dados](08_2_sending_a_transaction_with_data.md).

## Accessando Scripts em Nossas Transações

Podemos não perceber, mas já vimos esses scripts de bloqueio e desbloqueio como parte das transações brutas que fizemos. A melhor maneira de analisar esses scripts com mais profundidade é, portanto, criar uma transação bruta e examiná-la.

### Criando uma Transação de Teste

Para examinar scripts reais de desbloqueio e bloqueio, vamos criar uma transação bruta rapidamente, pegando um UTXO legado não gasto e reenviando-o a um endereço de troco legado, deixando uma parte para as taxas de transação:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ recipient=$(bitcoin-cli -named getrawchangeaddress address_type=legacy)
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.0009 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
```
Não precisamos enviá-la. O objetivo é simplesmente produzir uma transação para que possamos examiná-la.

>**NOTA:** Por que usar endereços legados? Porque seus scripts são mais significativos. No entanto, também ofereceremos um exemplo de um endereço nativo Segwit P2WPKH na seção [§9.5](09_5_Scripting_a_P2WPKH.md).

### Examinando Nossa Transação de Teste

Agora podemos examinar nossa transação profundamente usando ```decoderawtransaction``` no ```$signedtx```:
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
Os dois scripts são encontrados nas duas partes diferentes da transação.

O ```scriptSig``` está localizado no ```vin```. Este é o script de _desbloqueio_. É o que é executado para acessar o UTXO que está sendo usado para financiar esta transação. Haverá um ```scriptSig``` por UTXO em cada transação.

O ```scriptpubkey``` está localizado no ```vout```. Este é o script de _bloqueio_. É o que bloqueia a nova saída da transação. Haverá um ```scriptPubKey``` por cada saída em cada transação.

> :book: ***Como o scriptSig e o scriptPubKey interagem?*** O ```scriptSig``` de uma transação desbloqueia o UTXO anterior. A saída desta nova transação será bloqueada com um ```scriptPubKey```, que pode, por sua vez, ser desbloqueado pelo ```scriptSig``` da transação que reutiliza aquele UTXO.

### Lendo os Scripts na Transação

Se olharmos para os dois scripts veremos que cada um possui duas representações diferentes: o ```hex``` é o que realmente é armazenado, mas a linguagem assembly (```asm```) mais legível pode meio que mostrar o que está acontecendo.

Vamos dar uma olhada na ```asm``` do script de desbloqueio para podermos ver em primeira mão como é programar no Bitcoin:
```
04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
```
Como acontece, essa bagunça de números é uma assinatura de chave privada, seguida pela chave pública associada. Ou pelo menos isso é o esperado, porque é isso que é necessário para desbloquear o UTXO P2PKH que esta transação está usando.

Lendo o script de bloqueio vemos que é algo muito mais óbvio:
```
OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG
```
Esse é o método padrão em Bitcoin Script para bloquear uma transação P2PKH.

A seção [§9.4](09_4_Scripting_a_P2PKH.md) explicará como esses dois scripts andam juntos, mas primeiro precisamos saber como os scripts são avaliados no Bitcoin.

## Examinando um Tipo Diferente de Transação

Antes de deixarmos esta base para trás, vamos olhar para um tipo diferente de script de bloqueio. Aqui está o ```scriptPubKey``` da transação multisig que criamos na seção [§6.1: Enviando uma Transação com Multisig](06_1_sending_a_transaction_to_a_multisig.md).
```
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
```

Vamos comparar isso com o ```ScriptPubkey``` da nossa nova transação P2PKH:
```
    "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
```

Essas duas transações são _definitivamente_ trancadas de maneiras diferentes. O Bitcoin reconhece a primeira como sendo ```scripthash``` (P2SH) e a segunda como sendo ```pubkeyhash``` (P2PKH), mas também devemos ser capazes de ver a diferença nos diferentes códigos ```asm```: ```OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL``` versus ```OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 Op_equalverify op_checksig```. Este é o poder da programação: conseguimos, de maneira bem simples, produzir alguns dos tipos drasticamente diferentes de transações que aprendemos nos capítulos anteriores.

## Resumo: Compreendendo a Base das Transações

Cada transação do Bitcoin inclui pelo menos um script de desbloqueio (```scriptSig```), que resolve um quebra-cabeça criptográfico anterior, e pelo menos um script de bloqueio (```scriptPubKey```), que cria um novo quebra-cabeça criptográfico. Há um ```scriptSig``` para cada entrada e um ```scriptPubKey``` para cada saída. Cada um desses scripts é escrito usando o Bitcoin Script, uma linguagem semelhante à linguagem de programação Forth que fortalece ainda mais o Bitcoin.

> :fire: ***Qual é o poder dos scripts?*** Os scripts desbloqueiam todo o poder dos contratos inteligentes. Com os opcodes apropriados, podemos tomar decisões muito precisas sobre quem pode resgatar os fundos, quando podem ser resgatados e como eles podem resgatá-los. Regras mais complexas para saldos corporativos, endereços de parceria, gastos com proxy e outras metodologias também podem ser codificados dentro de um script. Os scripts também conseguem capacitar serviços mais complexos no Bitcoin, como a Lightning e sidechains.

## O Que Vem Depois?

Vamos continuar "Apresentando os Scripts no Bitcoin" na seção [§9.2: Executando um Script no Bitcoin](09_2_Running_a_Bitcoin_Script.md).
