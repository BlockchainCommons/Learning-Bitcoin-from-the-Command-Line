# 4.3 Criando uma Transação Bruta com Argumentos Nomeados

Às vezes, pode ser assustador descobrir a ordem correta dos argumentos para um comando ```bitcoin-cli```. Felizmente, podemos usar _argumentos nomeados_ como alternativa.

> :warning: **AVISO DE VERSÃO:** Esta é uma inovação do Bitcoin Core v0.14.0. Se usarmos os scripts de configuração do tutorial, o que é importante fazer, precisamos verificar novamente a versão se tiver algum problema. Há também um bug no uso do comando ```createrawtransaction``` usando argumentos nomeados que presumivelmente serão corrigidos na versão 0.14.1.

## Criando um Alias de Argumento Nomeado

Para usar um argumento nomeado, devemos executar o ```bitcoin-cli``` com o argumento ```-named```. Se planejamos fazer isso regularmente, provavelmente precisaremos criar um alias:
```
alias bitcoin-cli="bitcoin-cli -named"
```
Como de costume, isso é apenas para facilitar o uso, mas continuaremos usando todos os comandos para manter a clareza.

## Testando um Argumento Nomeado

Para saber quais são os nomes dos argumentos de um comando, precisamos consultar o ```bitcoin-cli help```. Ele listará os argumentos com a ordem adequada, mas agora também fornecerá nomes para cada um deles.

Por exemplo, `bitcoin-cli help getbalance` lista estes argumentos:

   1. dummy [costumava ser account]
   2. minconf
   3. include_watchonly
   4. avoid_reuse
   
O exemplo seguinte mostra um uso tradicional e não intuitivo do ```getbalance``` usando o argumento de confirmação mínimo:
```
$ bitcoin-cli getbalance "*" 1
```
Com argumentos nomeados, podemos esclarecer o que estamos fazendo, o que também minimiza os erros:
```
$ bitcoin-cli -named getbalance minconf=1
```

## Testando uma Transação Bruta

Veja como seriam os comandos para enviar uma transação bruta com argumentos nomeados:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00001 }''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "hash": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
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
      "value": 0.00001000,
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
    }
  ]
}

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e70dd2aa13422d12c222481c17ca21a57071f92ff86bdcffd7eaca71772ba172
```
Pronto! Enviamos outra transação bruta, mas desta vez usando argumentos nomeados para maior clareza e redução de erros.

> :warning: **AVISO DE VERSÃO:** É aqui que o bug no Bitcoin Core 0.14 aparece: O argumento ```inputs``` no ```createrawtransaction``` tem o nome ```transactions``` incorreto. Portanto, se estiver no Bitcoin Core 0.14.0, substitua o argumento nomeado ```inputs``` por ```transactions``` para este e também para os exemplos futuros. No entanto, a partir do Bitcoin Core 0.14.1, este código deve funcionar normalmente.

## Resumo: Criando uma Transação Bruta com Argumentos Nomeados

Executando o ```bitcoin-cli``` com o argumento ```-named``` podemos usar argumentos nomeados ao invés de depender de argumentos ordenados. O comando ```bitcoin-cli help``` sempre mostrará o nome correto para cada argumento. Isso pode resultar em um código mais robusto, mais fácil de ler e menos sujeito a erros.

_À partir de agora, usaremos argumentos nomeados para todos os exemplos futuros, para maior clareza e para estabelecer as melhores práticas. No entanto, também mostraremos todos os argumentos na ordem correta. Portanto, se preferir não usar os argumentos nomeados, apenas retire o argumento ```-named``` e todos os ```name=``` que os exemplos devem continuar funcionando corretamente._

## O Que Vem Depois?

Continue "Enviando Transações no Bitcoin" na seção [§4.4: Enviando Moedas com Transações Brutas](04_4_Sending_Coins_with_a_Raw_Transaction.md).