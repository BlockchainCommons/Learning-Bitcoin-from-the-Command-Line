# 6.3: Enviando e Gastando uma Transação Multisig de Maneira Automatizada

A técnica padrão para criar endereços com várias assinaturas e gastar os fundos é complexa, mas é um exercício interessante para entender um pouco mais como funcionam e como podemos manipulá-los em um nível relativamente baixo. No entanto, o Bitcoin Core tornou os multisigs mais fáceis de serem manipulados nas novas versões.

> :warning: **AVISO DE VERSÃO:** O comando ```addmultisigaddress``` está disponível no Bitcoin Core v0.10 ou superior.

## Criando um endereço Multisig em nossa carteira

Para tornar os fundos enviados para endereços multisig mais fáceis de serem gastos, só precisamos fazer algumas pré-configurações usando o comando ```addmultisigaddress```. Não é o que gostaríamos de fazer se estivéssemos escrevendo programas de carteiras que utilizam multisig, mas se estivesse apenas tentando receber alguns fundos, isso poderia evitar alguns problemas.

### Coletando as chaves

Vamos começar criando os endereços P2PKH e recuperando as chaves públicas, como de costume, para cada usuário que fará parte do multisig:
```
machine1$ address3=$(bitcoin-cli getnewaddress)
machine1$ echo $address3
tb1q4ep2vmakpkkj6mflu94x5f94q662m0u5ad0t4w
machine1$ bitcoin-cli -named getaddressinfo address=$address3 | jq -r '. | .pubkey'
0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc

machine2$ address4=$(bitcoin-cli getnewaddress)
machine2$ echo $address4
tb1qa9v5h6zkhq8wh0etnv3ae9cdurkh085xufl3de
machine2$ bitcoin-cli -named getaddressinfo address=$address4 | jq -r '. | .pubkey'
02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f
```

### Criando o endereço Multisig em qualquer lugar

Em seguida, vamos criar o multisig em _cada máquina que contribuiu com as assinaturas_ usando um novo comando, ```addmultisigaddress```, ao invés de ```createmultisig```. Este novo comando salva algumas das informações da nossa carteira, tornando muito mais fácil gastar o dinheiro no futuro.
```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$address3'","02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f"]'''
{
  "address": "tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex",
  "redeemScript": "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae",
  "descriptor": "wsh(multi(2,[d6043800/0'/0'/15']0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc,[e9594be8]02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f))#wxn4tdju"
}

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc","'$address4'"]'''
{
  "address": "tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex",
  "redeemScript": "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae",
  "descriptor": "wsh(multi(2,[ae42a66f]0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc,[fe6f2292/0'/0'/2']02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f))#cc96c5n6"
}
```
Conforme observado na seção anterior, atualmente não importa se usamos endereços ou chaves públicas, então mostramos o outro mecanismo aqui, misturando os dois. Iremos obter o mesmo endereço multisig de qualquer maneira. No entanto, _devemos usar a mesma ordem sempre_. Portanto, é melhor que os membros do multisig verifiquem entre si a ordem dos dados para ter certeza de que todos obtiveram o mesmo resultado.

### Esperando para receber os fundos

Depois disso, os membros do multisig ainda precisarão executar o comando ```importaddress``` para observar os fundos recebidos no endereço multisig:
```
machine1$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8 rescan="false"

machine2$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8 rescan="false"
```

## Gastando novamente com uma transação automatizada

Posteriormente, podemos receber os fundos no endereço com várias assinaturas sem nenhum problema. O uso do ```addmultisigaddress``` é simplesmente uma questão burocrática por parte dos destinatários: Um pouco de contabilidade para facilitar a vida deles quando desejarem gastar os saldos.

Mas, isso torna a vida muito mais fácil. Como as informações foram salvas na carteira, os assinantes poderão gastar os fundos enviados para o endereço com várias assinaturas exatamente como qualquer outro endereço, ao invés de assinar sempre em várias máquinas diferentes.

Podemos começar coletando as variáveis, mas não precisamos mais nos preocupar com o ```scriptPubKey``` ou com o ```redeemScript```.

Aqui está uma nova transação enviada para nosso novo endereço multisig:
```
machine1$ utxo_txid=b9f3c4756ef8159d6a66414a4317f865882ee04beb57a0f8349dafcc98f5acbc
machine1$ utxo_vout=0
machine1$ recipient=$(bitcoin-cli getrawchangeaddress)
```
Vamos criar uma transação bruta:
```
machine1$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
```
E depois, assiná-la:
```
machine1$ bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex
{
  "hex": "02000000000101bcacf598ccaf9d34f8a057eb4be02e8865f817434a41666a9d15f86e75c4f3b90000000000ffffffff0188130000000000001600144f93c831ec739166ea425984170f4dc6bac75829040047304402205f84d40ba16ff49e60a7fc9228ef5917473aae1ab667dad01e113ca0fef3008b02201a50da2c65f38798aea94bcbd5bbf065bc1e38de44bacee69d525dcddcc11bba01004752210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae00000000",
  "complete": false,
  "errors": [
    {
      "txid": "b9f3c4756ef8159d6a66414a4317f865882ee04beb57a0f8349dafcc98f5acbc",
      "vout": 0,
      "witness": [
        "",
        "304402205f84d40ba16ff49e60a7fc9228ef5917473aae1ab667dad01e113ca0fef3008b02201a50da2c65f38798aea94bcbd5bbf065bc1e38de44bacee69d525dcddcc11bba01",
        "",
        "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae"
      ],
      "scriptSig": "",
      "sequence": 4294967295,
      "error": "CHECK(MULTI)SIG failing with non-zero signature (possibly need more signatures)"
    }
  ]
}

```
Observe que não precisamos mais da ajuda extra do comando ```signrawtransactionwithkey```, porque todas essas informações já estão em nossa carteira. Mais importante, não tornamos nossas chaves privadas vulneráveis ​​ao manipulá-las diretamente. Ao invés disso, o processo é _exatamente_ o mesmo que gastar um UTXO normal, exceto que a transação não foi totalmente assinada no final.

### Assinando em outras máquinas

A etapa final é exportar o ```hex``` parcialmente assinado para a outra máquina e assinar a transação novamente:
```
machine2$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000 | jq -r '.hex')
```
Quando todos os os assinantes confirmaram a assinatura, estaremos prontos para enviar a transação para a rede:
```
machine2$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d
```
Tal como acontece com o atalho discutido na seção [§4.5: Enviando Moedas com Transações Brutas Automatizadas](04_5_Sending_Coins_with_Automated_Raw_Transactions.md), o resultado é muito mais fácil, mas perdemos certo controle no processo.

## Resumo: Enviando e Gastando uma Transação Multisig de Maneira Automatizada

Existe uma maneira mais fácil de gastar fundos enviados para os nossos endereços multisig que simplesmente requerem o uso do comando ```addmultisigaddress``` quando criamos nosso endereço. Ele não demonstra os meandros do gasto do P2SH e não nos dará um controle abrangente, mas se queremos apenas receber nossas moedas, este pode ser o melhor caminho.

## O Que Vem Depois?

Saiba mais sobre "Expandindo as transações de Bitcoin com multisigs" no [Capítulo 7: Expandindo as transações do Bitcoin com PSBTs](07_0_Expanding_Bitcoin_Transactions_PSBTs.md).