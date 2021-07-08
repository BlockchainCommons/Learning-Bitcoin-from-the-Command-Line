# 7.2: Usando uma transação Bitcoin parcialmente assinada

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Agora que aprendemos o fluxo de trabalho básico para gerar um PSBT, provavelmente desejamos fazer algo com ele. O que os PSBTs podem fazer que os multisigs (e as transações brutas normais) não podem? Para começar, temos a facilidade de usar um formato padronizado, o que significa que podemos usar nossas transações do ```bitcoin-cli``` e combiná-las com transações geradas por pessoas (ou programas) em outras plataformas. Além disso, podemos fazer algumas coisas que simplesmente não eram fáceis usando outros mecanismos.

A seguir estão três exemplos do uso dos PSBTs para: Multisigs, financiamentos e CoinJoins.

> :aviso: **AVISO DE VERSÃO:** Esta é uma inovação do Bitcoin Core v0.17.0. As versões anteriores do Bitcoin Core não funcionarão com o PSBT enquanto ele estiver em andamento (embora ainda consigam reconhecer a transação final).

## Usando um PSBT para gastar fundos MultiSig

Suponha que tenhamos criado um endereço multisig, assim como fizemos na seção [§6.3](06_3_Sending_an_Automated_Multisig.md).
```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]'''
{
  "address": "tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0",
  "redeemScript": "5221038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e2103789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd063652ae",
  "descriptor": "wsh(multi(2,[d6043800/0'/0'/26']038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e,[be686772]03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636))#07zyayfk"
}
machine1$ bitcoin-cli -named importaddress address="tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0" rescan=false

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]'''
{
  "address": "tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0",
  "redeemScript": "5221038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e2103789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd063652ae",
  "descriptor": "wsh(multi(2,[d6043800/0'/0'/26']038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e,[be686772]03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636))#07zyayfk"
}
machine2$ bitcoin-cli -named importaddress address="tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0" rescan=false
```
E que tenhamos algumas moedas:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5",
    "vout": 0,
    "address": "tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0",
    "label": "",
    "witnessScript": "5221038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e2103789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd063652ae",
    "scriptPubKey": "0020224cb503a7f7835799b9c22ee0c3c7d93d090356e30e70015c3ebbfa515a3074",
    "amount": 0.01999800,
    "confirmations": 2,
    "spendable": false,
    "solvable": true,
    "desc": "wsh(multi(2,[d6043800/0'/0'/26']038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e,[be686772]03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636))#07zyayfk",
    "safe": true
  }
]
```
Nós _poderíamos_ gastá-las usando os mecanismos no [Capítulo 6](06_0_Expanding_Bitcoin_Transactions_Multisigs.md), onde assinamos serialmente uma transação, mas ao invés disso, vamos mostrar a vantagem do PSBTs para o uso das multisigs. Podemos gerar um único PSBT, permitir que todos o assinem em paralelo e depois podemos combinar todos resultados! Não há mais como passar o arquivo hexadecimal em constante expansão de pessoa para pessoa, o que acelera as coisas e reduz as chances de erros.

Para demonstrar essa metodologia, vamos extrair aquele BTC 0,02 do multisig e dividi-lo entre os dois assinantes, cada um deles gerando um novo endereço com esse propósito:
```
machine1$ bitcoin-cli getnewaddress
tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma
machine2$ bitcoin-cli getnewaddress
tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp
```
A primeira coisa que fazemos é criar um PSBT em uma máquina. Não importa onde. Precisamos usar o ```createpsbt``` da seção [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md) para isso, não iremos usar o método mais simples que é o comando ```walletcreatefundedpsbt```, porque precisamos do controle extra de selecionar o dinheiro protegido pelo multisig. Esse será o caso para todos os três exemplos nesta seção, o que demonstra porque geralmente precisemos usar o ```createpsbt``` para coisas mais complexas.
```
machine1$ utxo_txid=53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5
machine1$ utxo_vout=0
machine1$ split1=tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma
machine1$ split2=tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp
machine1$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$split1'": 0.009998,"'$split2'": 0.009998 }''')
```
Em seguida, precisamos enviar esse $psbt a todas as partes do multisig para que assinem:
```
machine1$ echo $psbt
cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAAAAAA=
```
Mas você só precisa enviar uma vez! E você faz isso simultaneamente.

Aqui está o resultado da primeira máquina, onde geramos o PSBT:
```
machine1$ psbt_p1=$(bitcoin-cli walletprocesspsbt $psbt | jq -r '.psbt')
machine1$ bitcoin-cli decodepsbt $psbt_p1
{
  "tx": {
    "txid": "1687e89fcb9dd3067f75495b4884dc1d4d1cf05a6c272b783cfe29eb5d22e985",
    "hash": "1687e89fcb9dd3067f75495b4884dc1d4d1cf05a6c272b783cfe29eb5d22e985",
    "version": 2,
    "size": 113,
    "vsize": 113,
    "weight": 452,
    "locktime": 0,
    "vin": [
      {
        "txid": "25e8a26f60cf485768a1e6953b983675c867b7ab126b02e753c47b7db0c4be5e",
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
        "value": 0.00499900,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 cee9f88288a5f4980191a28d7ae08ff584050da7",
          "hex": "0014cee9f88288a5f4980191a28d7ae08ff584050da7",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma"
          ]
        }
      },
      {
        "value": 0.00049990,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 8d861ff6e8acf1ca2a3b3dd59c86f2e8a0aba75c",
          "hex": "00148d861ff6e8acf1ca2a3b3dd59c86f2e8a0aba75c",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp"
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
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2abb5d49ce7e753cbf5a9ffa8cdaf815bf1074f5c0bf495a93df8eb5112f65aa",
          "hex": "00202abb5d49ce7e753cbf5a9ffa8cdaf815bf1074f5c0bf495a93df8eb5112f65aa",
          "type": "witness_v0_scripthash",
          "address": "tb1q92a46jww0e6ne066nlagekhczkl3qa84czl5jk5nm78t2yf0vk4qte328m"
        }
      },
      "partial_signatures": {
        "03f52980d322acaf084bcef3216f3d84bfb672d1db26ce2861de3ec047bede140d": "304402203abb95d1965e4cea630a8b4890456d56698ff2dd5544cb79303cc28cb011cbb40220701faa927f8a19ca79b09d35c78d8d0a2187872117d9308805f7a896b07733f901"
      },
      "witness_script": {
        "asm": "2 033055ec2da9bbb34c2acb343692bfbecdef8fab8d114f0036eba01baec3888aa0 03f52980d322acaf084bcef3216f3d84bfb672d1db26ce2861de3ec047bede140d 2 OP_CHECKMULTISIG",
        "hex": "5221033055ec2da9bbb34c2acb343692bfbecdef8fab8d114f0036eba01baec3888aa02103f52980d322acaf084bcef3216f3d84bfb672d1db26ce2861de3ec047bede140d52ae",
        "type": "multisig"
      },
      "bip32_derivs": [
        {
          "pubkey": "033055ec2da9bbb34c2acb343692bfbecdef8fab8d114f0036eba01baec3888aa0",
          "master_fingerprint": "c1fdfe64",
          "path": "m"
{
  "tx": {
    "txid": "ee82d3e0d225e0fb919130d68c5052b6e3c362c866acc54d89af975330bb4d16",
    "hash": "ee82d3e0d225e0fb919130d68c5052b6e3c362c866acc54d89af975330bb4d16",
    "version": 2,
    "size": 113,
    "vsize": 113,
    "weight": 452,
    "locktime": 0,
    "vin": [
      {
        "txid": "53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5",
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
        "value": 0.00999800,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 cee9f88288a5f4980191a28d7ae08ff584050da7",
          "hex": "0014cee9f88288a5f4980191a28d7ae08ff584050da7",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma"
          ]
        }
      },
      {
        "value": 0.00999800,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 8d861ff6e8acf1ca2a3b3dd59c86f2e8a0aba75c",
          "hex": "00148d861ff6e8acf1ca2a3b3dd59c86f2e8a0aba75c",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp"
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
        "amount": 0.01999800,
        "scriptPubKey": {
          "asm": "0 224cb503a7f7835799b9c22ee0c3c7d93d090356e30e70015c3ebbfa515a3074",
          "hex": "0020224cb503a7f7835799b9c22ee0c3c7d93d090356e30e70015c3ebbfa515a3074",
          "type": "witness_v0_scripthash",
          "address": "tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0"
        }
      },
      "partial_signatures": {
        "038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e": "3044022040aae4f2ba37b1526524195f4a325d97d1317227b3c82aea55c5abd66810a7ec0220416e7c03e70a31232044addba454d6b37b6ace39ab163315d3293e343ae9513301"
      },
      "witness_script": {
        "asm": "2 038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e 03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636 2 OP_CHECKMULTISIG",
        "hex": "5221038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e2103789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd063652ae",
        "type": "multisig"
      },
      "bip32_derivs": [
        {
          "pubkey": "03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636",
          "master_fingerprint": "be686772",
          "path": "m"
        },
        {
          "pubkey": "038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e",
          "master_fingerprint": "d6043800",
          "path": "m/0'/0'/26'"
        }
      ]
    }
  ],
  "outputs": [
    {
      "bip32_derivs": [
        {
          "pubkey": "02fce26085452d07abc63bd389cb7dba9871e79bbecd08039291226be8232a9000",
          "master_fingerprint": "d6043800",
          "path": "m/0'/0'/24'"
        }
      ]
    },
    {
    }
  ],
  "fee": 0.00000200
}
machine1$ bitcoin-cli analyzepsbt $psbt_p1
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "be6867729bcc35ed065bb4c937557d371218a8e2"
        ]
      }
    }
  ],
  "estimated_vsize": 168,
  "estimated_feerate": 0.00001190,
  "fee": 0.00000200,
  "next": "signer"
}

```
Isso demonstra que as informações do UTXO foram importadas e que há uma _assinatura parcial_, mas que uma assinatura ainda não a deixa completa.

Aqui temos o mesmo resultado na outra máquina:
```
machine2$ psbt=cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAAAAAA=
machine2$ psbt_p2=$(bitcoin-cli walletprocesspsbt $psbt | jq -r '.psbt')
machine3$ echo $psbt_p2
cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAABASu4gx4AAAAAACIAICJMtQOn94NXmbnCLuDDx9k9CQNW4w5wAVw+u/pRWjB0IgIDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZHMEQCIDJ71isvR2We6ym1QByLV5SQ+XEJD0SAP76fe1JU5PZ/AiB3V7ejl2H+9LLS6ubqYr/bSKfRfEqrp2FCMISjrWGZ6QEBBUdSIQONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDiEDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZSriIGA3ifVDQjZw4Wlmf/fh8toql98bCRInLhQtWCRRrOvQY2ENPtiCUAAACAAAAAgAYAAIAiBgONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDgRZu4lPAAAiAgNJzEMyT3rZS7QHqb8SvFCv2ee0MKRyVy8bY8tVUDT1KhDT7YglAAAAgAAAAIADAACAAA==
```
Observe novamente que gerenciamos a assinatura deste multisig gerando um PSBT totalmente sem assinatura com o UTXO correto, permitindo que cada um dos usuários processe esse PSBT por conta própria, adicionando entradas e assinaturas. Como resultado, temos dois PSBTs, cada um contendo apenas uma assinatura. Isso não funcionaria no cenário multisig clássico, porque todas as assinaturas devem ser serializadas. Porém, podemos assinar em paralelo neste momento, e em seguida, usar a função Combinador para juntá-los.

Vamos novamente em qualquer máquina e vamos nos certificar de que temos os dois PSBTs nas variáveis e, em seguida, vamos combiná-los:
```
machine1$ psbt_p2="cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAABAIcCAAAAAtu5pTheUzdsTaMCEPj3XKboMAyYzABmIIeOWMhbhTYlAAAAAAD//////uSTLbibcqSd/Z9ieSBWJ2psv+9qvoGrzWEa60rCx9cAAAAAAP////8BuIMeAAAAAAAiACAiTLUDp/eDV5m5wi7gw8fZPQkDVuMOcAFcPrv6UVowdAAAAAAAACICA0nMQzJPetlLtAepvxK8UK/Z57QwpHJXLxtjy1VQNPUqENPtiCUAAACAAAAAgAMAAIAA"
machine2$ psbt_c=$(bitcoin-cli combinepsbt '''["'$psbt_p1'", "'$psbt_p2'"]''')
$ bitcoin-cli decodepsbt $psbt_c
{
  "tx": {
    "txid": "ee82d3e0d225e0fb919130d68c5052b6e3c362c866acc54d89af975330bb4d16",
    "hash": "ee82d3e0d225e0fb919130d68c5052b6e3c362c866acc54d89af975330bb4d16",
    "version": 2,
    "size": 113,
    "vsize": 113,
    "weight": 452,
    "locktime": 0,
    "vin": [
      {
        "txid": "53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5",
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
        "value": 0.00999800,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 cee9f88288a5f4980191a28d7ae08ff584050da7",
          "hex": "0014cee9f88288a5f4980191a28d7ae08ff584050da7",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma"
          ]
        }
      },
      {
        "value": 0.00999800,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 8d861ff6e8acf1ca2a3b3dd59c86f2e8a0aba75c",
          "hex": "00148d861ff6e8acf1ca2a3b3dd59c86f2e8a0aba75c",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp"
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
        "amount": 0.01999800,
        "scriptPubKey": {
          "asm": "0 224cb503a7f7835799b9c22ee0c3c7d93d090356e30e70015c3ebbfa515a3074",
          "hex": "0020224cb503a7f7835799b9c22ee0c3c7d93d090356e30e70015c3ebbfa515a3074",
          "type": "witness_v0_scripthash",
          "address": "tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0"
        }
      },
      "partial_signatures": {
        "038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e": "3044022040aae4f2ba37b1526524195f4a325d97d1317227b3c82aea55c5abd66810a7ec0220416e7c03e70a31232044addba454d6b37b6ace39ab163315d3293e343ae9513301",
        "03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636": "30440220327bd62b2f47659eeb29b5401c8b579490f971090f44803fbe9f7b5254e4f67f02207757b7a39761fef4b2d2eae6ea62bfdb48a7d17c4aaba761423084a3ad6199e901"
      },
      "witness_script": {
        "asm": "2 038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e 03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636 2 OP_CHECKMULTISIG",
        "hex": "5221038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e2103789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd063652ae",
        "type": "multisig"
      },
      "bip32_derivs": [
        {
          "pubkey": "03789f543423670e169667ff7e1f2da2a97df1b0912272e142d582451acebd0636",
          "master_fingerprint": "be686772",
          "path": "m"
        },
        {
          "pubkey": "038d73adf2c7ea33f9dc34b77b62b59af433c1de9c763332da79e83e155f96030e",
          "master_fingerprint": "d6043800",
          "path": "m/0'/0'/26'"
        }
      ]
    }
  ],
  "outputs": [
    {
      "bip32_derivs": [
        {
          "pubkey": "02fce26085452d07abc63bd389cb7dba9871e79bbecd08039291226be8232a9000",
          "master_fingerprint": "d6043800",
          "path": "m/0'/0'/24'"
        }
      ]
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "0349cc43324f7ad94bb407a9bf12bc50afd9e7b430a472572f1b63cb555034f52a",
          "master_fingerprint": "d3ed8825",
          "path": "m/0'/0'/3'"
        }
      ]
    }
  ],
  "fee": 0.00000200
}
$ bitcoin-cli analyzepsbt $psbt_c
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    }
  ],
  "estimated_vsize": 168,
  "estimated_feerate": 0.00001190,
  "fee": 0.00000200,
  "next": "finalizer"
}
```
Funcionou! Acabamos de finalizar, enviar e pronto:
```
machine2$ psbt_c_hex=$(bitcoin-cli finalizepsbt $psbt_c | jq -r '.hex')
standup@btctest2:~$ bitcoin-cli -named sendrawtransaction hexstring=$psbt_c_hex
ee82d3e0d225e0fb919130d68c5052b6e3c362c866acc54d89af975330bb4d16
```
Obviamente, não houve uma grande melhoria no uso deste método em relação à assinatura em série de uma transação para um multisig 2 de 2 quando todos estavam usando o ```bitcoin-cli```: Poderíamos ter passado uma transação bruta com assinaturas parciais de um usuário para o outro tão facilmente quanto enviar o PSBT. Mas, este foi o caso mais simples. À medida que nos aprofundamos em multisigs mais complexas, essa metodologia se torna cada vez melhor, mas já podemos destacar alguns pontos importantes:

Em primeiro lugar, é independente de plataforma. Enquanto todos estiverem usando um serviço compatível com o Bitcoin Core 0.17, todos poderão assinar essa transação, o que não é verdade quando os multisigs clássicos estão sendo repassados ​​entre diferentes plataformas.

Em segundo lugar, é muito mais escalável. Considere um multisig 3 de 5. De acordo com a metodologia antiga, isso teria que ser transmitido de pessoa para pessoa, aumentando enormemente os problemas se alguém o corrompesse. Aqui, outros usuários precisam apenas enviar os PSBTs de volta ao Criador e, assim que tiver a quantidade suficiente, poderá gerar a transação final.

## Usando um PSBT para financiamentos

Os Multisigs como o usado no exemplo anterior são frequentemente usados ​​para receber pagamentos por trabalho colaborativo, seja royalties de um livro ou pagamentos feitos a uma empresa. Nessa situação, o exemplo acima funciona muito bem: Os dois participantes recebem seu dinheiro, que depois é dividido. Mas e quanto ao caso inverso, em que dois (ou mais) participantes desejam criar uma joint venture e precisam dividir o dinheiro?

A resposta tradicional é criar um multisig e depois fazer com que os participantes enviem individualmente os fundos para lá. O problema é que o primeiro pagador depende da boa fé do segundo, e isso não é a base do Bitcoin, que diz que _não devemos confiar_. Felizmente, com o advento dos PSBTs, agora podemos fazer pagamentos sem confiança para podemos enviar fundos.

> :book: ***O que significa a ideia de não podermos confiar?*** Quando dizemos, não devemos confiar, significa que nenhum participante precisa confiar em nenhum outro participante. Ao invés disso, eles esperam que os protocolos de software garantam que tudo seja executado de maneira justa, de maneira esperada. O Bitcoin é um protocolo que não depende de confiança porque não precisamos de ninguém para agir de boa fé. O sistema faz o gerenciamento. Da mesma forma, os PSBTs permitem a criação de transações que agrupam ou dividem fundos sem a necessidade de nenhuma parte confiar na outra.

O exemplo a seguir mostra dois usuários que possuem 0,010 BTC e que desejam agrupar no endereço multisig `tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0`, criado acima.
```
machine1$ bitcoin-cli listunspent
[
  {
    "txid": "2536855bc8588e87206600cc980c30e8a65cf7f81002a34d6c37535e38a5b9db",
    "vout": 0,
    "address": "tb1qfg5y4fx979xkv4ezatc5eevufc8vh45553n4ut",
    "label": "",
    "scriptPubKey": "00144a284aa4c5f14d665722eaf14ce59c4e0ecbd694",
    "amount": 0.01000000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/0'/25']02bea222cf9ea1f49b392103058cc7c8741d76a553fe627c1c43fc3ef4404c9d54)#4hnkg9ml",
    "safe": true
  }
]
machine2$ bitcoin-cli listunspent
[
 {
    "txid": "d7c7c24aeb1a61cdab81be6aefbf6c6a27562079629ffd9da4729bb82d93e4fe",
    "vout": 0,
    "address": "tb1qfqyyw6xrghm5kcrpkus3kl2l6dz4tpwrvn5ujs",
    "label": "",
    "scriptPubKey": "001448084768c345f74b6061b7211b7d5fd3455585c3",
    "amount": 0.01000000,
    "confirmations": 5363,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d3ed8825/0'/0'/0']03ff6b94c119582a63dbae4fb530efab0ed5635f7c3b2cf171264ca0af3ecef33a)#gtmd2e2k",
    "safe": true
  }
]
```
Eles configuram variáveis para usarem essas transações:
```
machine1$ utxo_txid_1=2536855bc8588e87206600cc980c30e8a65cf7f81002a34d6c37535e38a5b9db
machine1$ utxo_vout_1=0
machine1$ utxo_txid_2=d7c7c24aeb1a61cdab81be6aefbf6c6a27562079629ffd9da4729bb82d93e4fe
machine1$ utxo_vout_2=0
machine1$ multisig=tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0

```
E criam um PSBT:
```
machine1$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$multisig'": 0.019998 }''')
```
O resultado fica mais ou menos assim:
```
machine1$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5",
    "hash": "53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5",
    "version": 2,
    "size": 135,
    "vsize": 135,
    "weight": 540,
    "locktime": 0,
    "vin": [
      {
        "txid": "2536855bc8588e87206600cc980c30e8a65cf7f81002a34d6c37535e38a5b9db",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967295
      },
      {
        "txid": "d7c7c24aeb1a61cdab81be6aefbf6c6a27562079629ffd9da4729bb82d93e4fe",
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
        "value": 0.01999800,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 224cb503a7f7835799b9c22ee0c3c7d93d090356e30e70015c3ebbfa515a3074",
          "hex": "0020224cb503a7f7835799b9c22ee0c3c7d93d090356e30e70015c3ebbfa515a3074",
          "reqSigs": 1,
          "type": "witness_v0_scripthash",
          "addresses": [
            "tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0"
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
Não importa se as transações sejam de propriedade de duas pessoas diferentes ou que as informações completas apareçam em duas máquinas diferentes. Este PSBT de financiamento funcionará exatamente da mesma forma que o PSBT multisig. Uma vez que todos os controladores tenham assinado, a transação pode ser finalizada.

Aqui está o processo, desta vez passando o PSBT parcialmente assinado de um usuário para outro, ao invés de ter que combinar as coisas no final.
```
machine1$ bitcoin-cli walletprocesspsbt $psbt
{
  "psbt": "cHNidP8BAIcCAAAAAtu5pTheUzdsTaMCEPj3XKboMAyYzABmIIeOWMhbhTYlAAAAAAD//////uSTLbibcqSd/Z9ieSBWJ2psv+9qvoGrzWEa60rCx9cAAAAAAP////8BuIMeAAAAAAAiACAiTLUDp/eDV5m5wi7gw8fZPQkDVuMOcAFcPrv6UVowdAAAAAAAAQEfQEIPAAAAAAAWABRKKEqkxfFNZlci6vFM5ZxODsvWlAEIawJHMEQCIGAiKIAWRXiw68o3pw61/cVNP7n2oH73S654XXgQ4kjHAiBtTBqmaF1iIzYGXrG4DadH8y6mTuCRVFDiPl+TLQDBJwEhAr6iIs+eofSbOSEDBYzHyHQddqVT/mJ8HEP8PvRATJ1UAAABAUdSIQONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDiEDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZSriICA3ifVDQjZw4Wlmf/fh8toql98bCRInLhQtWCRRrOvQY2BL5oZ3IiAgONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDhDWBDgAAAAAgAAAAIAaAACAAA==",
  "complete": false
}

machine2$  psbt_p="cHNidP8BAIcCAAAAAtu5pTheUzdsTaMCEPj3XKboMAyYzABmIIeOWMhbhTYlAAAAAAD//////uSTLbibcqSd/Z9ieSBWJ2psv+9qvoGrzWEa60rCx9cAAAAAAP////8BuIMeAAAAAAAiACAiTLUDp/eDV5m5wi7gw8fZPQkDVuMOcAFcPrv6UVowdAAAAAAAAQEfQEIPAAAAAAAWABRKKEqkxfFNZlci6vFM5ZxODsvWlAEIawJHMEQCIGAiKIAWRXiw68o3pw61/cVNP7n2oH73S654XXgQ4kjHAiBtTBqmaF1iIzYGXrG4DadH8y6mTuCRVFDiPl+TLQDBJwEhAr6iIs+eofSbOSEDBYzHyHQddqVT/mJ8HEP8PvRATJ1UAAABAUdSIQONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDiEDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZSriICA3ifVDQjZw4Wlmf/fh8toql98bCRInLhQtWCRRrOvQY2BL5oZ3IiAgONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDhDWBDgAAAAAgAAAAIAaAACAAA=="
machine2$ psbt_f=$(bitcoin-cli walletprocesspsbt $psbt_p | jq -r '.psbt')
machine2$ bitcoin-cli analyzepsbt $psbt_f
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
  "estimated_vsize": 189,
  "estimated_feerate": 0.00001058,
  "fee": 0.00000200,
  "next": "extractor"
}
machine2$ psbt_hex=$(bitcoin-cli finalizepsbt $psbt_f | jq -r '.hex')
machine2$ bitcoin-cli -named sendrawtransaction hexstring=$psbt_hex
53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5
```
Usamos um PSBT para reunir o dinheiro usando um multisig sem precisar de confiança!

## Useando um PSBT para CoinJoin

O CoinJoin é outro aplicativo Bitcoin que não requer confiança. Aqui, temos uma variedade de entes que não se conhecem juntando dinheiro e recebendo de volta.

A metodologia para gerenciá-lo com PSBTs é exatamente a mesma que vimos nos exemplos acima, como o seguinte pseudocódigo demonstra:
```
$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' }, { "txid": "'$utxo_txid_3'", "vout": '$utxo_vout_3' } ]''' outputs='''{ "'$split1'": 1.7,"'$split2'": 0.93,"'$split3'": 1.4 }''')
```
Cada usuário coloca o próprio UTXO e cada um recebe uma saída correspondente.

A melhor maneira de gerenciar um CoinJoin é enviar o PSBT básico para todas as partes (que podem ser inúmeras) e, em seguida, fazer com que cada uma assine o PSBT e envie de volta para uma única parte que irá combinar, finalizar e enviar.

## Resumo: Usando uma transação Bitcoin parcialmente assinada

Agora vimos o processo PSBT que aprendemos na seção [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md) em uso em três exemplos da vida real: Criando um multisig, um financiamento e fazendo o CoinJoin. Tudo isso era teoricamente possível no Bitcoin por ter várias pessoas assinando transações cuidadosamente construídas, mas os PSBTs tornam isso padronizado e simples.

> :fire: ***Qual é o poder de um PSBT?*** Um PSBT permite a criação de transações sem necessidade de confiança entre várias partes e várias máquinas. Se mais de uma parte precisar financiar uma transação, se mais de uma parte precisar assinar uma transação, ou se uma transação precisar ser criada em uma máquina e assinada em outra, um PSBT torna isso simples, sem depender do mecanismos de assinatura parcial não padronizados que existiam antes do PSBT.

Esse último ponto, ao criar uma transação em uma máquina e assinar em outra, é um elemento dos PSBTs que ainda não chegamos. Ele está no centro das carteiras de hardware, onde geralmente desejamos criar uma transação em um full node e, em seguida, passá-la para uma hardware wallet quando uma assinatura for necessária. Esse é o tópico da nossa última seção (e do nosso quarto exemplo da vida real) deste capítulo sobre PSBTs.

## O Que Vem Depois?

Continue "Expanding Bitcoin Transactions with PSBTs" with [§7.3: Inegrating with Hardware Wallets](07_3_Integrating_with_Hardware_Wallets.md).
Vamos continuar "Expandindo as Transações de Bitcoin com PSBTs" na seção [§7.3: Integração com hardware wallets](07_3_Integrating_with_Hardware_Wallets.md).