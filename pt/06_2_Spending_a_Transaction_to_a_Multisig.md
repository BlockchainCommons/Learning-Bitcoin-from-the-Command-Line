# 6.2: Gastando uma Transação Multsig

A maneira clássica e complexa de gastar fundos enviados para um endereço com várias assinaturas usando o ```bitcoin-cli``` requer que suemos bastante a camisa.

## Encontrando os fundos

Para começar, precisamos encontrar nossos fundos. Nosso computador não sabe procurá-los, porque não estão associados a nenhum endereço da nossa carteira. Podemos alertar o ```bitcoind``` para fazer isso usando o comando ```importaddress```:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz
```
Se temos um node prunado (e provavelmente temos), ao invés desse jeito, precisaremos dizer não para verificar novamente:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz rescan="false"
```
Se preferirmos, podemos importar o endereço usando nosso descritor (e esta é geralmente a melhor resposta, porque é mais padronizada):
```
$ bitcoin-cli importmulti '[{"desc": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y", "timestamp": "now", "watchonly": true}]'
[
  {
    "success": true
  }
]
```
Depois, os fundos devem aparecer quando usarmos o comando ```listunspent```, porém, ainda não podemos gastar facilmente. Na verdade, nossa carteira pode alegar que não podemos gastá-los de forma alguma!

Se por algum motivo não puder incorporar o endereço da nossa carteira, podemos usar o comando ```gettransaction``` para obtermos mais informações (ou olharmos em um block explorer).
```
$ bitcoin-cli -named gettransaction txid=b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521 verbose=true
{
  "amount": -0.00006500,
  "fee": -0.00001000,
  "confirmations": 3,
  "blockhash": "0000000000000165b5f602920088a7e36b11214161d6aaebf5229e3ed4f10adc",
  "blockheight": 1773282,
  "blockindex": 9,
  "blocktime": 1592959320,
  "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "walletconflicts": [
  ],
  "time": 1592958753,
  "timereceived": 1592958753,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
      "category": "send",
      "amount": -0.00006500,
      "vout": 0,
      "fee": -0.00001000,
      "abandoned": false
    }
  ],
  "hex": "020000000001011b95a6055174ec64b82ef05b6aefc38f34d0e57197e40281ecd8287b4260dec60000000000ffffffff01641900000000000017a914a5d106eb8ee51b23cf60d8bd98bc285695f233f38702473044022070275f81ac4129e1d167ef7e700739f2899ea4c7f1adef3a4da29436f14fb97e02207310d4ec449eba49f0fa404ae45b9c82431d883490c7a0ed882ad0b5d7a623d0012102883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb00000000",
  "decoded": {
    "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
    "hash": "bdf4e3bc5d354a5dfa5528f172480976321d989d7e5806ac14f1fe9b0b1c093a",
    "version": 2,
    "size": 192,
    "vsize": 111,
    "weight": 441,
    "locktime": 0,
    "vin": [
      {
        "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "txinwitness": [
          "3044022070275f81ac4129e1d167ef7e700739f2899ea4c7f1adef3a4da29436f14fb97e02207310d4ec449eba49f0fa404ae45b9c82431d883490c7a0ed882ad0b5d7a623d001",
          "02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb"
        ],
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
}
```

## Configurando as nossas variáveis

Quando estivermos prontos para gastar os fundos recebidos por um endereço multisig, precisaremos coletar _muitos_ dados: Muito mais do que precisamos quando fazemos uma transação usando um UTXO de um P2PKH normal ou SegWit. Isso ocorre em parte porque as informações sobre o endereço multisig não estão em nossa posse e em parte porque estamos gastando dinheiro que foi enviado para um endereço P2SH (Pay-To-Script-Hash) e isso é muito mais exigente.

No total, precisaremos coletar três coisas: Informações estendidas sobre o UTXO; O redemScript e; Todas as chaves privadas envolvidas. Obviamente, também iremos precisar de um novo endereço de destinatário. As chaves privadas precisam aguardar a etapa de assinatura, mas tudo pode ser feito agora.

### Acessando as informações do UTXO

Para começar, vamos pegar o ```txid``` e o ```vout``` para a transação que desejamos gastar, como de costume. Nesse caso, os dados foram recuperados das informações ```gettransaction``` acima:
```
$ utxo_txid=b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521
$ utxo_vout=0
```
No entanto, precisamos também acessar um terceiro bit de informação sobre o UTXO, nosso ```scriptPubKey```/```hex```, que é o script que travamos a transação. Novamente, podemos fazer isso observando os detalhes da transação:
```
$ utxo_spk=a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387
```

### Gravando o script de resgate

Felizmente, salvamos nosso ```redeemScript```. Agora devemos registrá-lo em uma variável.

O valor foi extraído da criação de endereço na seção anterior.
```
redeem_script="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
```
### Decidindo o destinatário

Vamos apenas enviar o dinheiro de volta para nós mesmos. Isso é útil porque libera os fundos do multisig, convertendo-os em uma transação P2PKH normal que pode ser posteriormente confirmada por uma única chave privada:
```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
## Criando nossa transação

Agora podemos criar nossa transação. Essa parte é parecida com as transações normais.
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```

## Assinando a nossa transação

Agora estamos prontos para assinar a transação. Este é um processo de várias etapas porque precisaremos fazer em várias máquinas, cada uma das quais contribuirá com suas próprias chaves privadas.

### Carregando a primeira chave privada

Como essa transação não está fazendo uso total da nossa carteira, precisaremos acessar diretamente as chaves privadas. Começando com a ```máquina1```, onde devemos recuperar qualquer uma das chaves privadas do usuário que estavam envolvidas no multisig:
```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR
```
> :warning: **Atenção:** Acessar diretamente as chaves privadas do shell é um comportamento muito perigoso e deve ser feito com extremo cuidado se estivermos em um ambiente produtivo. No mínimo, é importante não salvar as informações em uma variável que possa ser acessada pela nossa máquina. Remover o histórico do  shell é outro grande passo. No máximo, podemos evitar de fazer isso.

### Fazendo a nossa primeira assinatura

Agora podemos fazer nossa primeira assinatura com o comando ```signrawtransactionwithkey```. É aqui que as coisas ficam diferentes das normais: Precisaremos treinar o comando sobre como assinar. Podemos fazer isso adicionando as seguintes informações novas:

* Incluir um argumento ```prevtxs``` que tenha o ```txid```, o ```vout```, o ```scriptPubKey``` e o ```redeemScript``` que gravamos, cada um deles um par individual do valor-chave no objeto JSON.
* Incluir um argumento ```privkeys``` que lista as chaves privadas que pegamos nesta máquina.

```
machine1$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": false,
  "errors": [
    {
      "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
      "vout": 0,
      "witness": [
      ],
      "scriptSig": "0047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
      "sequence": 4294967295,
      "error": "CHECK(MULTI)SIG failing with non-zero signature (possibly need more signatures)"
    }
  ]
}

```
Isso produz erros assustadores e mostra um status de ```failing```. Tudo bem. Podemos ver que a assinatura foi parcialmente bem-sucedida porque o ```hex``` ficou mais longo. Embora a transação tenha sido parcialmente assinada, ela não foi concluída porque precisa de mais assinaturas.

### Repetindo para os outros assinantes

Agora podemos passar a transação adiante, para ser assinada novamente por nós, que temos a outra parte da multisig. Eles fazem isso executando o mesmo comando de assinatura que fizemos, porém: (1) com o ```hex``` maior que produzimos anteriormente (```bitcoin-cli -named signrawtransactionwithkey hexstring = $ rawtxhex prevtxs = '' '[{"txid": " '$ utxo_txid'", "vout": '$ utxo_vout', "scriptPubKey": " '$ utxo_spk'", "redeemScript": " '$ redeem_script'"}] '' 'privkeys =' [ "cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"] '| jq -r'. | .hex'```) e; (2) com nossa própria chave privada.

> :information_source: **NOTA - M de N vs N de N:** Obviamente, se tivermos uma assinatura N de N (como a multisig 2 de 2 do exemplo), todas as partes precisarão assinar, mas se tiviermos uma multisignatura M de N onde "M <N", a assinatura estará completa quando apenas alguns ("M") tiverem assinado.

Para fazer isso, primeiro acessamos as chaves privadas:
```
machine2$ bitcoin-cli -named dumpprivkey address=$address2
cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz
```
Depois, assinamos o novo ```hex``` usando os mesmos valores ```prevtxs```:
```
machine1$  bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000d90047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e301473044022000a402ec4549a65799688dd531d7b18b03c6379416cc8c29b92011987084e9f402205470e24781509c70e2410aaa6d827aa133d6df2c578e96a496b885584fb039200147522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": true
}
```
Por fim, podemos precisar enviar uma ```hexstring``` ainda mais longa que produzem para assinaturas adicionais.

Mas, neste caso, podemos ver que a assinatura está `complete`!

## Enviando nossa transação

Quando terminarmos, devemos recorrer à metodologia ```JQ``` padrão para salvar nossa ```hexstring``` e, em seguida, enviá-la:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]' | jq -r .hex)
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```

## Compreendendo a importância desta metodologia de assinatura expandida

Isso deu um belo trabalho e, como logo iremos aprender, existe uma tolice ao utilizar as chaves privadas, o redeem script e com a scriptpubkey pois eles não são realmente necessários para resgatar os endereços de multisg usando versões mais recentes do Bitcoin Core. Então qual era a questão?

Esta metodologia de resgate mostra uma maneira padrão de assinar e reutilizar transações P2SH. Em suma, para resgatar fundos P2SH, uma ```signrawtransactionwithkey``` precisa:

1. Incluir o ```scriptPubKey```, que explica o quebra-cabeça criptográfico P2SH;
2. Incluir o ```redeemScript```, que resolve o quebra-cabeça criptográfico P2SH e apresenta um novo quebra-cabeça;
3. Ser executado em cada máquina que contém as chaves privadas necessárias;
4. Incluir as assinaturas relevantes, que resolvem o quebra-cabeça do redemScript.

Aqui, vimos essa metodologia usada para resgatar os fundos multisig. No futuro, também podemos usá-la para resgatar os fundos que foram trancados com outros scripts P2SH mais complexos, conforme explicado no Capítulo 9.

## Resumo: Gastando uma Transação Multsig

Acontece que gastar dinheiro enviado para um endereço multisig pode dar um pouco de trabalho. Mas, contanto que tenhamos os endereços originais e nosso redemScript, podemos fazer isso assinando uma transação bruta com cada endereço diferente e fornecendo mais algumas informações ao longo do caminho.

## O Que Vem Depois?

Vamos continuar "Expandindo as transações de Bitcoin com multisigs" na seção [§6.3: Enviando e Gastando uma Transação Multisig de Maneira Automatizada](06_3_Sending_an_Automated_Multisig.md).