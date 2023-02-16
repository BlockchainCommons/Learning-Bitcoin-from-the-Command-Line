# 7.2: Usando una Transacción Bitcoin Parcialmente Firmada

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con advertencias.

Ahora que ha aprendido el flujo de trabajo básico para generar un PSBT, probablemente quiera hacer algo con él. ¿Qué pueden hacer las PSBT que las multifirmas (y las transacciones sin procesar normales) no pueden hacer? Para empezar, tiene la facilidad de uso de un formato estandarizado, lo que significa que puede usar sus transacciones `bitcoin-cli` y combinarlas con transacciones generadas por personas (o programas) en otras plataformas. Más allá de eso, puede hacer algunas cosas que no fueron fáciles usando otras mecánicas.

A continuación se muestran tres ejemplos del uso de PSBT para: multifirmas, agrupación de dinero y unión de monedas.

> :warning: **ADVERTENCIA DE VERSIÓN:** Esta es una innovación de Bitcoin Core v 0.17.0. Las versiones anteriores de Bitcoin Core no podrán funcionar con el PSBT mientras esté en progreso (aunque aún podrán reconocer la transacción final).
## Utilice un PSBT para gastar fondos Multifirma

Suponga que ha creado una dirección de firma múltiple, tal como lo hizo en [§6.3](06_3_Enviando_una_Multifirma_Automatizada.md). 
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
Y tiene algo de dinero en ello:
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
_Podría_ gastar esto usando los mecanismos del [Capítulo 6](06_0_Expandiendo_las_Transacciones_Bitcoin_con_Multifirmas.md), donde firmó una transacción en serie, pero en su lugar vamos a mostrar la ventaja de los PSBT para multi-sigs: puede generar un solo PSBT, permitir que todos firmen eso en paralelo y luego combinen los resultados. Ya no es más laborioso pasar un maleficio en constante expansión de persona a persona, lo que acelera las cosas y reduce las posibilidades de errores.

Para demostrar esta metodología, vamos a sacar ese 0.02 BTC de la multifirma y dividirlo entre los dos firmantes, cada uno de los cuales generó una nueva dirección para ese propósito:
```
machine1$ bitcoin-cli getnewaddress
tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma
machine2$ bitcoin-cli getnewaddress
tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp
```
Lo primero que hacemos es crear un PSBT en la máquina de nuestra elección. (No importa cuál). Necesitamos usar `createpsbt` de [§7.1](07_1_Creando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md) para esto, no el más simple `walletcreatefundedpsbt`, porque necesitamos el control adicional de seleccionar el dinero protegido por la multifirma. (Este será el caso de los tres ejemplos de esta sección, lo que demuestra por qué normalmente necesita usar `createpsbt` para las cosas complejas).
```
machine1$ utxo_txid=53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5
machine1$ utxo_vout=0
machine1$ split1=tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma
machine1$ split2=tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp
machine1$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$split1'": 0.009998,"'$split2'": 0.009998 }''')
```
Luego debe enviar ese $psbt a todos para que lo firmen:
```
machine1$ echo $psbt
cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAAAAAA=
```
¡Pero solo tiene que enviarlo una vez! Y lo haces de forma simultánea.

Aquí está el resultado en la primera máquina, donde generamos el PSBT:
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
Esto demuestra que la información UTXO ha sido importada y que hay una _firma parcial_, pero que la firma de la entrada única aún no está completa.

Aquí está lo mismo en la otra máquina:
```
machine2$ psbt=cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAAAAAA=
machine2$ psbt_p2=$(bitcoin-cli walletprocesspsbt $psbt | jq -r '.psbt')
machine3$ echo $psbt_p2
cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAABASu4gx4AAAAAACIAICJMtQOn94NXmbnCLuDDx9k9CQNW4w5wAVw+u/pRWjB0IgIDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZHMEQCIDJ71isvR2We6ym1QByLV5SQ+XEJD0SAP76fe1JU5PZ/AiB3V7ejl2H+9LLS6ubqYr/bSKfRfEqrp2FCMISjrWGZ6QEBBUdSIQONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDiEDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZSriIGA3ifVDQjZw4Wlmf/fh8toql98bCRInLhQtWCRRrOvQY2ENPtiCUAAACAAAAAgAYAAIAiBgONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDgRZu4lPAAAiAgNJzEMyT3rZS7QHqb8SvFCv2ee0MKRyVy8bY8tVUDT1KhDT7YglAAAAgAAAAIADAACAAA==
```
Tenga en cuenta nuevamente que gestionamos la firma de este multifirma generando un PSBT totalmente sin firmar con el UTXO correcto, luego permitiendo que cada uno de los usuarios procese ese PSBT por su cuenta, agregando entradas y firmas. Como resultado, tenemos dos PSBT, cada uno de los cuales contiene una firma y no la otra. Eso no funcionaría en el escenario clásico multifirma, porque todas las firmas tienen que ser serializadas. Aquí, en cambio, podemos iniciar sesión en paralelo y luego hacer uso del rol de Combinador para mezclarlas juntas.

Volvemos a ir a cualquiera de las dos máquinas y nos aseguramos de tener ambos PSBT en variables, luego los combinamos:
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
¡Funcionó! Simplemente finalizamos, enviamos y terminamos:
```
machine2$ psbt_c_hex=$(bitcoin-cli finalizepsbt $psbt_c | jq -r '.hex')
standup@btctest2:~$ bitcoin-cli -named sendrawtransaction hexstring=$psbt_c_hex
ee82d3e0d225e0fb919130d68c5052b6e3c362c866acc54d89af975330bb4d16
```
Obviamente, no hubo una gran mejora en el uso de este método en comparación con la firma en serie de una transacción para una multifirma 2 de 2 cuando todos usaban `bitcoin-cli`: podríamos haber pasado una transacción sin procesar con firmas parciales de un usuario a otro tan fácilmente como enviar ese PSBT. Pero este fue el caso más simple. A medida que nos adentramos en multifirmas más complejas, esta metodología se vuelve cada vez mejor.

En primer lugar, es independiente de la plataforma. Siempre que todos utilicen un servicio que admita Bitcoin Core 0.17, todos podrán firmar esta transacción, lo cual no es cierto cuando los multifirmas clásicos se transmiten entre diferentes plataformas.

En segundo lugar, es mucho más escalable. Considere un multifirma de 3 de 5. Bajo la vieja metodología, tendría que pasar de persona a persona, aumentando enormemente los problemas si se rompe algún eslabón de la cadena. Aquí, otros usuarios solo tienen que enviar los PSBTs al Creador, y tan pronto como tenga suficiente, puede generar la transacción final.

## Utilice un PSBT para juntar dinero

Multifirmas como la utilizada en el ejemplo anterior se suelen utilizar para recibir pagos por trabajo colaborativo, ya sean regalías por un libro o pagos realizados a una empresa. En esa situación, el ejemplo anterior funciona muy bien: los dos participantes reciben su dinero y luego lo dividen. Pero, ¿qué pasa con el caso inverso, donde dos (o más) participantes quieren establecer una empresa conjunta y necesitan financiarla con dinero?

La respuesta tradicional es crear una multifirma y luego hacer que los participantes le envíen sus fondos individualmente. El problema es que el primer pagador tiene que depender de la buena fe del segundo, y eso no se basa en la fortaleza de Bitcoin, que es su _confianza sin depender de las contrapartes_. Afortunadamente, con la llegada de los PSBT, ahora podemos realizar pagos sin confianza que agrupan fondos.

> :book: ***¿Qué significa sin confianza?*** Sin confianza significa que ningún participante tiene que confiar en ningún otro participante. En su lugar, esperan que los protocolos de software garanticen que todo se ejecute de manera justa de la manera esperada. Bitcoin es un protocolo sin confianza porque no necesita que nadie más actúe de buena fe; el sistema lo gestiona. De manera similar, los PSBT permiten la creación sin confianza de transacciones que agrupan o dividen fondos.
El siguiente ejemplo muestra dos usuarios que cada uno tiene 0.010 BTC que quieren agrupar en la dirección multifirma `tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0`, creada anteriormente.
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
Ellas configuran variables para usar esas transacciones:
```
machine1$ utxo_txid_1=2536855bc8588e87206600cc980c30e8a65cf7f81002a34d6c37535e38a5b9db
machine1$ utxo_vout_1=0
machine1$ utxo_txid_2=d7c7c24aeb1a61cdab81be6aefbf6c6a27562079629ffd9da4729bb82d93e4fe
machine1$ utxo_vout_2=0
machine1$ multisig=tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0
```
Y crea una PSBT:
```
machine1$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$multisig'": 0.019998 }''')
```
Así es como se ve:
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
No importa que las transacciones sean propiedad de dos personas diferentes o que su información completa aparezca en dos máquinas diferentes. Este PSBT de financiación funcionará exactamente igual que el PSBT multifirma: una vez que todas las partes controladoras hayan firmado, se podrá finalizar la transacción.

Aquí está el proceso, esta vez pasando el PSBT parcialmente firmado de un usuario a otro en lugar de tener que combinar las cosas al final.
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
¡Hemos utilizado un PSBT para reunir dinero sin confianza en una multifirma!

## Usa una PSBT para CoinJoin

CoinJoin es otra aplicación de Bitcoin que requiere desconfianza. Aquí, tiene una variedad de partes que no necesariamente se conocen entre sí juntando dinero y recuperándolo.

La metodología para administrarlo con PSBT es exactamente la misma que ha visto en los ejemplos anteriores, como lo demuestra el siguiente pseudocódigo:
```
$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' }, { "txid": "'$utxo_txid_3'", "vout": '$utxo_vout_3' } ]''' outputs='''{ "'$split1'": 1.7,"'$split2'": 0.93,"'$split3'": 1.4 }''')
```
Cada usuario pone su propio UTXO y cada uno recibe una salida correspondiente.

La mejor manera de administrar un CoinJoin es enviar el PSBT base a todas las partes (que podrían ser numerosas), y luego hacer que cada una de ellas firme el PSBT y lo envíe de vuelta a una sola parte que combinará, finalizará y enviará.

## Resumen: Usando una transacción Bitcoin parcialmente firmada

Ahora ha visto el proceso de PSBT que aprendió en [§7.1](07_1_Creando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md) en uso en tres ejemplos de la vida real: creación de una multifirma, agrupación de fondos y dentro de un CoinJoin. Todo esto era teóricamente posible en Bitcoin clásico al hacer que varias personas firmen transacciones cuidadosamente construidas, pero los PSBT lo hacen estandarizado y simple.

> :fire: ***¿Cuál es el poder de una PSBT?*** Un PSBT permite la creación de transacciones sin confianza entre múltiples partes y múltiples máquinas. Si más de una parte necesita financiar una transacción, si más de una parte necesita firmar una transacción o si una transacción debe crearse en una máquina y firmarse en otra, entonces un PSBT lo simplifica sin depender de la mecanismos de firma parcial no estandarizados que solían existir antes de PSBT.

Ese último punto, sobre la creación de una transacción en una máquina y la firma en otra, es un elemento de los PSBT al que aún no hemos llegado. Está en el corazón de las billeteras de hardware, donde a menudo desea crear una transacción en un nodo completo y luego pasarla a una billetera de hardware cuando se requiere una firma. Ese es el tema de la última sección (y nuestro cuarto ejemplo de la vida real) en este capítulo sobre PSBTs.

## ¿Qué sigue?

Continúe "Expandiendo transacciones de Bitcoin con PSBT" con [§7.3: Integrando con Hardware Wallets](07_3_Integrando_con_Hardware_Wallets.md).
