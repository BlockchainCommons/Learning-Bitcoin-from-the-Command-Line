# 4.5: Enviando Moedas com Transações Brutas Automatizadas

Este capítulo apresenta três maneiras de enviar fundos por meio da interface cli do Bitcoin. A seção [§4.1](04_1_Sending_Coins_The_Easy_Way.md) descreveu como fazer isso com um comando simples e a seção [§4.4](04_4_Sending_Coins_with_a_Raw_Transaction.md) detalhou como usar uma transação bruta mais perigosa. Esta seção fica no meio termo de ambas, mostrando como tornar as transações brutas mais simples e seguras.

## Deixando o Bitcoin Fazer os Cálculos Para Nós

A metodologia para transações brutas automatizadas é simples: criamos uma transação bruta, mas usamos o comando ```fundrawtransaction``` para pedir ao bitcoind para executar os cálculos para nós.

Para usar este comando, precisaremos garantir que nosso arquivo ```~/.bitcoin/bitcoin.conf``` contenha as variáveis racionais para calcular as taxas de transação. Podemos consultar a seção [§4.1: Enviando Moedas da Maneira Fácil](04_1_Sending_Coins_The_Easy_Way.md) para obter mais informações sobre isso.

Vamos usar números conservadores, por isso sugerimos adicionar o seguinte ao `bitcoin.conf`:
```
mintxfee=0.0001
txconfirmtarget=6
```
Para manter o tutorial em movimento (e para movimentarmos nosso dinheiro rapidamente), sugerimos o seguinte:
```
mintxfee=0.001
txconfirmtarget=1
```

## Criando uma Transação Bruta

Para usar o ```fundrawtransaction``` primeiro precisamos criar uma transação bruta básica que liste _nenhuma_ entrada e _nenhuma_ mudança de endereço. Apenas listaremos o nosso destinatário e quanto desejamos enviar, neste caso ```$recipient``` e ```0,0002``` BTC.
```
$ recipient=n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
$ unfinishedtx=$(bitcoin-cli -named createrawtransaction inputs='''[]''' outputs='''{ "'$recipient'": 0.0002 }''')
```

## Financiando Nossa Transação Bruta

Dizemos ao ```bitcoin-cli``` para financiar essa transação básica:
```
$ bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx
{
  "hex": "02000000012db87641c6a21e5a68b20c226428544978e6ac44964d5d8060d7388000c584eb0100000000feffffff02204e0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac781e0000000000001600140cc9cdcf45d4ea17f5227a7ead52367aad10a88400000000",
  "fee": 0.00022200,
  "changepos": 1
}
```
Isso fornece muitas informações úteis, mas uma vez que tenhamos certeza de como funciona, vamos querer usar o JQ para salvar nosso hex em uma variável, como de costume:
```
$ rawtxhex3=$(bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx | jq -r '.hex')
```
## Verificando Nossa Transação Financiada

Parece mágica, então nas primeiras vezes que usarmos o ```fundrawtransaction```, provavelmente vamos querer verificá-la.

Vamos executar o comando ```decoderawtransaction``` para mostrar que a transação bruta agora está disposta corretamente, usando um ou mais dos nossos UTXOs e enviando fundos excedentes de volta para um endereço de troco:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex3
{
  "txid": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "hash": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "version": 2,
  "size": 116,
  "vsize": 116,
  "weight": 464,
  "locktime": 0,
  "vin": [
    {
      "txid": "eb84c5008038d760805d4d9644ace67849542864220cb2685a1ea2c64176b82d",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00020000,
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
    },
    {
      "value": 0.00007800,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "hex": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r"
        ]
      }
    }
  ]
}
```
Uma coisa de interesse aqui é o endereço de troco, que é o segundo ```vout```. Observe que é um endereço ```tb1```, o que significa que é do tipo Bech32. Quando demos ao Bitcoin Core a capacidade total de gerenciar as alterações, ele o fez usando o tipo de endereço padrão, Bech32, e funcionou bem. É por isso que nossa mudança para endereços SegWit na seção [§4.6](04_6_Creating_a_Segwit_Transaction.md) realmente não é um grande negócio, mas existem algumas dicas para uso mais amplo, sobre as quais falaremos lá.

Embora tenhamos visto a taxa na saída de ```fundrawtransaction```, ela não pode ser visível aqui. No entanto, podemos verificar isso com o script JQ ```txfee-calc.sh``` criado na seção [Prefácio: Usando JQ](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/04_2__Interlude_Using_JQ.md):
```
$ ~/txfee-calc.sh $rawtxhex3
.000222
```
Finalmente, podemos usar o ```getaddressinfo``` para ver se o endereço de troco gerado realmente nos pertence:
```
$ bitcoin-cli -named getaddressinfo address=tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r
{
  "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
  "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "pubkey": "038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5",
  "ischange": true,
  "timestamp": 1592335137,
  "hdkeypath": "m/0'/1'/10'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
  ]
}
```
Observe o conteúdo do `ismine`.

## Enviando a Transação Financiada

Neste ponto, podemos assinar e enviar a transação normalmente.
```
$ signedtx3=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex3 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx3
8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa
```
Em alguns minutos, teremos o nosso troco de volta:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa",
    "vout": 1,
    "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
    "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
    "amount": 0.00007800,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
    "safe": true
  }
]
```

## Resumo: Enviando Moedas com Transações Brutas Automatizadas

Se formos enviar fundos usando transações brutas, então o ```fundrawtransaction``` oferece uma boa alternativa onde taxas, entradas e saídas são calculadas para nós, para que não percamos acidentalmente muito dinheiro.

> :fire: ***Qual é o poder de enviar moedas com transações brutas automatizadas?***
> _As vantagens._ Proporciona um bom meio termo. Se estamos enviando fundos manualmente e o ```sendtoaddress``` não oferece controle suficiente por qualquer motivo, podemos obter algumas das vantagens das transações brutas sem os perigos dela. Essa metodologia deve ser usada sempre que possível se estivermos enviando transações brutas manualmente.
> _As desvantagens._ É uma mistura. Embora existam algumas opções adicionais no ```fundrawtransaction``` que não foram mencionadas aqui, nosso controle ainda é limitado. Provavelmente, nunca desejaríamos usar esse método se formos escrever um programa cujo objetivo é saber exatamente o que está acontecendo.

## O Que Vem Depois?

Vamos concluir o capítulo "Enviando Transações no Bitcoin" com a seção [§4.6: Criando uma Transação SegWit](04_6_Creating_a_Segwit_Transaction.md).