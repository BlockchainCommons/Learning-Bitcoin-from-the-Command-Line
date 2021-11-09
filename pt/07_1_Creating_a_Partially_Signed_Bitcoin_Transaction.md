# 7.1: Criando uma Transação Parcialmente Assinada no Bitcoin (PSBT)

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

As Transações Bitcoin Parcialmente Assinadas (Partially Signed Bitcoin Transactions ou mais conhecidas como PSBTs) são a forma mais recente de variar a criação de transações básicas do Bitcoin. Eles fazem isso introduzindo a colaboração em todas as etapas do processo, permitindo que as pessoas (ou sistemas) não apenas autentiquem as transações em conjunto (como nas multisigs), mas também criem, financiem e transmitam facilmente de forma colaborativa.

> :aviso: **AVISO DE VERSÃO:** Esta é uma inovação do Bitcoin Core v 0.17.0. As versões anteriores do Bitcoin Core não funcionarão com o PSBT enquanto estiver em andamento (apesar de conseguir reconhecer a transação final). Algumas atualizações e upgrades para PSBTs continuaram até a versão 0.20.0.

## Entendendo Como PSBTs Funcionam

Os multisigs são ótimos para o caso específico de manter fundos em conjunto e definir regras para quem, entre os signatários conjuntos, poderia autenticar o uso do saldo. Existem muitos casos de uso, como: uma conta  conjunta entre cônjuges (uma assinatura 1-de-2); um requisito fiduciário para duplo controle (uma assinatura 2-de-2); e um depósito (uma assinatura 2-de-3).

> :book: ***O que é uma PSBT?*** Como o nome sugere, uma PSBT é uma transação que não foi totalmente assinada. Isso é importante porque, uma vez que uma transação é assinada, seu conteúdo está travado. O [BIP174](https://github.com/bitcoin/bips/blob/master/bip-0174.mediawiki) definiu uma metodologia abstrata para colocar as PSBTs junto que descreve e padroniza funções de criação colaborativa. Um *Criador* propõe uma transação; um ou mais *Atualizadores* a complementam; e um ou mais *Assinantes* a autenticam; antes de um *Finalizador* concluir o processo; e um *Extrator* o transformar em uma transação na rede Bitcoin. Também pode haver um *Combinador* que mescla as PSBTs paralelas de vários usuários.

As PSBTs podem, inicialmente, ter a mesma aparência que os multisigs porque têm um único pedaço de funcionalidade sobreposto: a capacidade de assinar uma transação em conjunto. No entanto, elas foram criadas para um caso de uso totalmente diferente. As PSBTs reconhecem a necessidade de vários programas criarem uma transação em conjunto por vários motivos diferentes e fornecem um formato regularizado para isso. Elas são especialmente úteis para casos de uso envolvendo hardware wallets (veja a seção [§7.3](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/07_3_Integrating_with_Hardware_Wallets.md )), que são protegidas contra acesso total à internet e tendem a ter um histórico mínimo, quando não zero, de transações.

Em geral, as PSBTs fornecem vários elementos funcionais que aprimoram esse caso de uso:

1. Elas fornecem um _padrão_ para criar transações de forma colaborativa, enquanto as metodologias anteriores (incluindo o multisig do capítulo anterior) dependiam de implementação;
2. Elas oferecem suporte a uma _variedade mais ampla de casos de uso_, incluindo financiamento simples em conjunto;
3. Elas suportam _hardware wallets_ e outros casos em que um node pode não ter um histórico completo das transações;
4. Elas opcionalmente permitem a combinação de _transações não serializadas_, não exigindo que um código hexadecimal cada vez maior seja passado de usuário para usuário.

As PSBTs fazem seu trabalho complementando as informações normais da transação com um conjunto de entradas e saídas, cada uma delas definindo tudo o que precisamos saber sobre os UTXOs, de forma que mesmo uma carteira com airgap possa tomar uma decisão informada sobre as assinaturas. Assim, uma entrada lista o saldo de um UTXO e o que precisa ser feito para gastá-lo, enquanto a saída faz o mesmo para os UTXOs que ela está criando.

Esta primeira seção descreverá o processo PSBT padrão com as seguintes funções: Criador, Atualizador, Assinante, Finalizador, Extrator. Faremos isso em uma máquina só, o que irá parecer uma forma complicada de criar uma transação bruta. Mas, tenha fé, há um motivo para isso! Nas seções [§7.2](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md) e [§7.3](07_3_Integrating_with_Hardware_Wallets.md) vamos ver alguns exemplos reais de como utilizar as PSBTs e vamos transformar este sistema simples em um processo colaborativo compartilhado entre várias máquinas que tem efeitos reais e cria oportunidades reais.

## Criando uma PSBT à Moda Antiga

#### Função na PSBT: Criador

A maneira mais fácil de criar uma PSBT é pegando uma transação existente e usando o comando ```converttopsbt``` para transformá-la em uma PSBT. Certamente, esta não é a _melhor_ maneira, uma vez que requer que façamos uma transação usando um formato (uma transação bruta) e depois convertendo-a para outro (PSBT), mas se tivermos um software antigo que só pode gerar uma transação bruta, pode ser que precisemos utilizá-lo.

Apenas criamos uma transação bruta normalmente:

```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[1] | .txid')
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ echo $utxo_txid_1 $utxo_vout_1 $utxo_txid_2 $utxo_vout_2
c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b 1 8748eff5f12ca886e3603d9e30227dcb3f0332e0706c4322fec96001f7c7f41c 0
$ recipient=tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.0000065 }''')
```
Então agora, a convertemos:
```
$ psbt=$(bitcoin-cli -named converttopsbt hexstring=$rawtxhex)
$ echo $psbt
cHNidP8BAHsCAAAAAhuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////8BigIAAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBQAAAAAAAAAA
```
É possível notar que a codificação PSBT é muito diferente da transação hexadecimal que estamos acostumados.

Mas, se pudermos, optaremos por criar o PSBT diretamente.

## Criando uma PSBT da Maneira Difícil

#### Função na PSBT: Criador

A primeira metodologia de criação de uma PSBT sem passar por outro formato é usando o comando para PSBT análogo ao ```createrawtransaction```. Usando o ```createpsbt``` teremos o controle máximo a custo do máximo trabalho e da oportunidade máxima de erros.

A CLI deve parecer bastante familiar, a diferença é que agora usamos um novo comando RPC:
```
$ psbt_1=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.0000065 }''')
```
A equipe do Bitcoin Core certificou-se de que ```createpsbt``` funcionasse de maneira muito parecida com a```createrawtransaction```, portanto não precisamos aprender uma nova sintaxe diferente.

Podemos verificar se a nova PSBT é a mesma criada pelo ```converttopsbt```:
```
$ echo $psbt_1
cHNidP8BAHsCAAAAAhuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////8BigIAAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBQAAAAAAAAAA
$ if [ "$psbt" == "$psbt_1" ]; then     echo "PSBTs are equal"; else     echo "PSBTs are not equal"; fi
PSBTs are equal
```

## Examinando uma PSBT

#### Função na PSBT: Qualquer Uma

Então, como a nossa PSBT realmente se parece? Podemos ver isso com o comando ```decodepsbt```:
```
$ bitcoin-cli -named decodepsbt psbt=$psbt
{
  "tx": {
    "txid": "ea73a631b456d2b041ed73bf5767946408c6ff067716929a68ecda2e3e4de6d3",
    "hash": "ea73a631b456d2b041ed73bf5767946408c6ff067716929a68ecda2e3e4de6d3",
    "version": 2,
    "size": 123,
    "vsize": 123,
    "weight": 492,
    "locktime": 0,
    "vin": [
      {
        "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967295
      },
      {
        "txid": "8748eff5f12ca886e3603d9e30227dcb3f0332e0706c4322fec96001f7c7f41c",
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
        "value": 0.00000650,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
    },
    {
    }
  ],
  "outputs": [
    {
    }
  ]
}
```
É importante notar que, embora tenhamos definido os fundamentos da transação: os ```vins``` de onde o dinheiro está vindo e os ```vouts``` para onde está indo, ainda não definimos os ```inputs``` e os ```outputs``` que são o coração da PSBT e que são necessários para os usuários offline avaliá-las. Isso já é esperado: o papel do Criador conforme definido no [BIP174](https://github.com/bitcoin/bips/blob/master/bip-0174.mediawiki) é delinear a transação, enquanto o papel do Atualizador é começar a preencher os dados específicos da PSBT. (Outros comandos combinam as funções de Criador e Atualizador, mas o ```createpsbt``` não, porque não tem acesso à nossa carteira).

Também podemos usar o comando ```analyzepsbt``` para verificar nosso estado atual:
```
standup@btctest20:~$ bitcoin-cli -named analyzepsbt psbt=$psbt
{
  "inputs": [
    {
      "has_utxo": false,
      "is_final": false,
      "next": "updater"
    },
    {
      "has_utxo": false,
      "is_final": false,
      "next": "updater"
    }
  ],
  "next": "updater"
}
```
Da mesma forma, o comando ```analysepsbt``` nos mostra uma PSBT que precisa ser trabalhada. Damos uma olhada em cada um dos dois `inputs` (correspondendo aos dois ```vins```), e nenhum deles possui as informações de que precisamos.

## Finalizando uma PSBT

#### Funções na PSBT: Atualizador, Assinante e Finalizador

Existe o comando ```utxoupdatepsbt``` que pode ser usado para atualizar UTXOs, importando as informações do descritor manualmente, mas não queremos usá-lo a menos que tenhamos um caso de uso em que não tenhamos todas as informações nas carteiras de todos que irão assinar a PSBT.

> :information_source: **NOTA:** Se escolhermos Atualizar a PSBT com o ```utxoupdatepsbt```, ainda precisaremos usar o ```walletprocesspsbt``` para assiná-lo: é o único comando que a função Assinante possui em PSBTs que está disponível no `bitcoin-cli`.

Ao invés disso, devemos usar o ```walletprocesspsbt```, que irá atualizar, assinar e finalizar:
```
$ bitcoin-cli walletprocesspsbt $psbt
{
  "psbt": "cHNidP8BAHsCAAAAAhuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////8BigIAAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBQAAAAAAAQEfAQAAAAAAAAAWABRsRdOvqHYghsS9dtinGsfJduGRlgEIawJHMEQCIAqJbxz6dBzNpfaDu4XZXb+DbDkM3UWnhezh9UdmeVghAiBRxMlW2o0wEtphtUZRWIiJOaGtXfsQbB4lovkvE4eRIgEhArrDpkX9egpTfGJ6039faVBYxY0ZzrADPpE/Gpl14A3uAAEBH0gDAAAAAAAAFgAU1ZEJG4B0ojde2ZhanEsY7+z9QWUBCGsCRzBEAiB+sNNCO4xiFQ+DoHVrqqk9yM0V4H9ZSyExx1PW7RbjsgIgUeWkQ3L7aAv1xIe7h+8PZb8ECsXg1UzbtPW8wd2qx0UBIQKIO7VGPjfVUlLYs9XCFBsAezfIp9tiEfdclVrMXqMl6wAA",
  "complete": true
}
```
Obviamente, precisaremos salvar as informações da ```psbt``` usando```JQ```:
```
$ psbt_f=$(bitcoin-cli walletprocesspsbt $psbt | jq -r '.psbt')
```
Podemos ver que os `inputs` foram preenchidos:
```
$ bitcoin-cli decodepsbt $psbt_f
{
  "tx": {
    "txid": "ea73a631b456d2b041ed73bf5767946408c6ff067716929a68ecda2e3e4de6d3",
    "hash": "ea73a631b456d2b041ed73bf5767946408c6ff067716929a68ecda2e3e4de6d3",
    "version": 2,
    "size": 123,
    "vsize": 123,
    "weight": 492,
    "locktime": 0,
    "vin": [
      {
        "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967295
      },
      {
        "txid": "8748eff5f12ca886e3603d9e30227dcb3f0332e0706c4322fec96001f7c7f41c",
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
        "value": 0.00000650,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.00000001,
        "scriptPubKey": {
          "asm": "0 6c45d3afa8762086c4bd76d8a71ac7c976e19196",
          "hex": "00146c45d3afa8762086c4bd76d8a71ac7c976e19196",
          "type": "witness_v0_keyhash",
          "address": "tb1qd3za8tagwcsgd39awmv2wxk8e9mwryvktqmkkg"
        }
      },
      "final_scriptwitness": [
        "304402200a896f1cfa741ccda5f683bb85d95dbf836c390cdd45a785ece1f54766795821022051c4c956da8d3012da61b5465158888939a1ad5dfb106c1e25a2f92f1387912201",
        "02bac3a645fd7a0a537c627ad37f5f695058c58d19ceb0033e913f1a9975e00dee"
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.00000840,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "type": "witness_v0_keyhash",
          "address": "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
        }
      },
      "final_scriptwitness": [
        "304402207eb0d3423b8c62150f83a0756baaa93dc8cd15e07f594b2131c753d6ed16e3b2022051e5a44372fb680bf5c487bb87ef0f65bf040ac5e0d54cdbb4f5bcc1ddaac74501",
        "02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb"
      ]
    }
  ],
  "outputs": [
    {
    }
  ],
  "fee": 0.00000191
}
```
Ou para ser mais preciso: (1) a PSBT foi atualizada com as informações de ```witness_utxo```; (2) a PSBT foi assinada; e (3) a PSBT foi finalizada.

## Criando uma PSBT da Maneira Fácil

#### Função na PSBT: Criador e Atualizador

Se esperamos que haja um comando equivalente ao ```fundrawtransaction```, ficaremos satisfeitos em saber que existe: ele é o ```walletcreatefundedpsbt```. Podemos usá-lo da mesma forma que ```createpsbt```:
```
$ bitcoin-cli -named walletcreatefundedpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.0000065 }'''
{
  "psbt": "cHNidP8BAOwCAAAABBuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP/////uFwerANKjyVK6WaR7gzlX+lOf+ORsfjP5LYCSNIbhaAAAAAAA/v///4XjOeey0NyGpJYpszNWF8AFNiuFaWsjkOrk35Jp+9kKAAAAAAD+////AtYjEAAAAAAAFgAUMPsier2ey1eH48oGqrbbYGzNHgKKAgAAAAAAABYAFMdy1vlVQuEe8R6O/Hx6aYMK04oFAAAAAAABAR8BAAAAAAAAABYAFGxF06+odiCGxL122Kcax8l24ZGWIgYCusOmRf16ClN8YnrTf19pUFjFjRnOsAM+kT8amXXgDe4Q1gQ4AAAAAIABAACADgAAgAABAR9IAwAAAAAAABYAFNWRCRuAdKI3XtmYWpxLGO/s/UFlIgYCiDu1Rj431VJS2LPVwhQbAHs3yKfbYhH3XJVazF6jJesQ1gQ4AAAAAIABAACADAAAgAABAIwCAAAAAdVmsvkSBmfeHqNAe/wDCQ5lEp9F/587ftzCD1UL60nMAQAAABcWABRzFxRJfFPl8FJ6SxjAJzy3mCAMXf7///8CQEIPAAAAAAAZdqkUf0NzebzGbEB0XtwYkeprODDhl12IrMEwLQAAAAAAF6kU/d+kMX6XijmD+jWdUrLZlJUnH2iHPhQbACIGA+/e40wACf0XXzsgteWlUX/V0WdG8uY1tEYXra/q68OIENYEOAAAAACAAAAAgBIAAIAAAQEfE4YBAAAAAAAWABTVkQkbgHSiN17ZmFqcSxjv7P1BZSIGAog7tUY+N9VSUtiz1cIUGwB7N8in22IR91yVWsxeoyXrENYEOAAAAACAAQAAgAwAAIAAIgICKMavAB+71Adqsbf+XtC1g/OlmLEuTp3U0axyeu/LAI0Q1gQ4AAAAAIABAACAGgAAgAAA",
  "fee": 0.00042300,
  "changepos": 0
}
```
No entanto, a grande vantagem é que podemos usá-lo para se autofinanciar, deixando de fora os ```inputs```, assim como o ```fundrawtransaction```.
```
$ psbt_new=$(bitcoin-cli -named walletcreatefundedpsbt inputs='''[]''' outputs='''{ "'$recipient'": 0.0000065 }''' | jq -r '.psbt')
$ bitcoin-cli decodepsbt $psbt_new
{
  "tx": {
    "txid": "9f2c6205ac797c1020f7f261e3ab71cd0699ff4b1a8934f68b273c71547e235f",
    "hash": "9f2c6205ac797c1020f7f261e3ab71cd0699ff4b1a8934f68b273c71547e235f",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "8748eff5f12ca886e3603d9e30227dcb3f0332e0706c4322fec96001f7c7f41c",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "68e1863492802df9337e6ce4f89f53fa5739837ba459ba52c9a3d200ab0717ee",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.00971390,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 09a74ef0bae4d68b0b2ec9a7c4557a2b5c85bd8b",
          "hex": "001409a74ef0bae4d68b0b2ec9a7c4557a2b5c85bd8b",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qpxn5au96untgkzewexnug4t69dwgt0vtfahcv6"
          ]
        }
      },
      {
        "value": 0.00000650,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.00000840,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "type": "witness_v0_keyhash",
          "address": "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb",
          "master_fingerprint": "d6043800",
          "path": "m/0'/1'/12'"
        }
      ]
    },
    {
      "non_witness_utxo": {
        "txid": "68e1863492802df9337e6ce4f89f53fa5739837ba459ba52c9a3d200ab0717ee",
        "hash": "68e1863492802df9337e6ce4f89f53fa5739837ba459ba52c9a3d200ab0717ee",
        "version": 2,
        "size": 140,
        "vsize": 140,
        "weight": 560,
        "locktime": 1774654,
        "vin": [
          {
            "txid": "cc49eb0b550fc2dc7e3b9fff459f12650e0903fc7b40a31ede670612f9b266d5",
            "vout": 1,
            "scriptSig": {
              "asm": "0014731714497c53e5f0527a4b18c0273cb798200c5d",
              "hex": "160014731714497c53e5f0527a4b18c0273cb798200c5d"
            },
            "sequence": 4294967294
          }
        ],
        "vout": [
          {
            "value": 0.01000000,
            "n": 0,
            "scriptPubKey": {
              "asm": "OP_DUP OP_HASH160 7f437379bcc66c40745edc1891ea6b3830e1975d OP_EQUALVERIFY OP_CHECKSIG",
              "hex": "76a9147f437379bcc66c40745edc1891ea6b3830e1975d88ac",
              "reqSigs": 1,
              "type": "pubkeyhash",
              "addresses": [
                "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ"
              ]
            }
          },
          {
            "value": 0.02961601,
            "n": 1,
            "scriptPubKey": {
              "asm": "OP_HASH160 fddfa4317e978a3983fa359d52b2d99495271f68 OP_EQUAL",
              "hex": "a914fddfa4317e978a3983fa359d52b2d99495271f6887",
              "reqSigs": 1,
              "type": "scripthash",
              "addresses": [
                "2NGParh82hE2Zif5PVK3AfLpYhfwF5FyRGr"
              ]
            }
          }
        ]
      },
      "bip32_derivs": [
        {
          "pubkey": "03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388",
          "master_fingerprint": "d6043800",
          "path": "m/0'/0'/18'"
        }
      ]
    }
  ],
  "outputs": [
    {
      "bip32_derivs": [
        {
          "pubkey": "029bb586a52657dd98852cecef78552a4e21d081a7a30e4008ce9b419840d4deac",
          "master_fingerprint": "d6043800",
          "path": "m/0'/1'/27'"
        }
      ]
    },
    {
    }
  ],
  "fee": 0.00028800
}
```
Como podemos ver, criamos a PSBT e depois a atualizamos com todas as informações que encontramos localmente.

A partir daí, precisamos usar o ```walletprocesspsbt``` para finalizar, como de costume:
```
$ psbt_new_f=$(bitcoin-cli walletprocesspsbt $psbt_new | jq -r '.psbt')
```
Posteriormente, uma análise mostrará que também está pronto:
```
$ bitcoin-cli analyzepsbt $psbt_new_f
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": true,
      "next": "extractor"
    },
    {
      "has_utxo": true,
      "is_final": true,
      "next": "extractor"
    }
  ],
  "estimated_vsize": 288,
  "estimated_feerate": 0.00100000,
  "fee": 0.00028800,
  "next": "extractor"
}
```
Agora, realmente gostaríamos de usar o ```walletcreatefundedpsbt``` se estivermos criando um programa com o ```bitcoin-cli```? Provavelmente não. Mas é a mesma análise que fazemos com o ```fundrawtransaction```. Deixamos o Bitcoin Core fazer a análise, o cálculo e as decisões, ou nós mesmos fazemos?

## Enviando uma PSBT

#### Função na PSBT: Extrator

Para finalizar a PSBT, usamos o ```finalizepsbt```, que irá transformar a PSBT novamente em hexadecimal. (Também assumirá a função de Finalizador, caso ainda não tenha acontecido).
```
$ bitcoin-cli finalizepsbt $psbt_f
{
  "hex": "020000000001021b95a6055174ec64b82ef05b6aefc38f34d0e57197e40281ecd8287b4260dec60100000000ffffffff1cf4c7f70160c9fe22436c70e032033fcb7d22309e3d60e386a82cf1f5ef48870000000000ffffffff018a02000000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a050247304402200a896f1cfa741ccda5f683bb85d95dbf836c390cdd45a785ece1f54766795821022051c4c956da8d3012da61b5465158888939a1ad5dfb106c1e25a2f92f13879122012102bac3a645fd7a0a537c627ad37f5f695058c58d19ceb0033e913f1a9975e00dee0247304402207eb0d3423b8c62150f83a0756baaa93dc8cd15e07f594b2131c753d6ed16e3b2022051e5a44372fb680bf5c487bb87ef0f65bf040ac5e0d54cdbb4f5bcc1ddaac745012102883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb00000000",
  "complete": true
}
```
Como de costume, vamos querer salvar e depois enviar.
```
$ psbt_hex=$(bitcoin-cli finalizepsbt $psbt_f | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$psbt_hex
ea73a631b456d2b041ed73bf5767946408c6ff067716929a68ecda2e3e4de6d3
```
## Revisando o Fluxo de Trabalho

Ao criar software ```bitcoin-cli```, é mais provável que iremos cumprir as cinco funções principais dos PSBTs usando os comandos ```createpsbt```, ```walletprocesspsbt``` e ```finalizepsbt```. Esta é a aparência desse fluxo:

![](images/psbt-roles-for-cli-1.png)

Se escolhermos usar o atalho de ```walletcreatefundedpsbt```, teremos um fluxo mais ou menos assim:

![](images/psbt-roles-for-cli-2.png)

Finalmente, se precisarmos de mais controle e optarmos por usar o comando ```utxoupdatepsbt``` (que não está documentado aqui), teremos um fluxo de trabalho assim:

![](images/psbt-roles-for-cli-3.png)

## Resumo: Criando uma Transação Parcialmente Assinada no Bitcoin (PSBT)

A criação de uma PSBT envolve um fluxo de trabalho um tanto complexo de criação, atualização, assinatura, finalização e extração da PSBT, após o qual ela se converte novamente em uma transação bruta. Por que teríamos tanto trabalho? Porque desejamos colaborar entre vários usuários ou vários programas. Agora que entendemos esse fluxo de trabalho, a próxima seção irá apresentar alguns exemplos reais de como fazer isso.

## O Que Vem Depois?

Vamos continuar "Expandindo Transações no Bitcoin com PSBTs" na seção [§7.2: Usando uma Transação Parcialmente Assinada no Bitcoin (PSBT)](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md).
