# 7.2: Usare una transazione Bitcoin parzialmente firmata

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Letto avvertito.

Ora che hai imparato il flusso di lavoro di base per generare un PSBT, probabilmente vorrai farci qualcosa. Cosa possono fare i PSBT che i multi-sig (e le normali transazioni grezze) non possonofare? Per cominciare, hai la facilità d'uso di un formato standardizzato, il che significa che puoi utilizzare le tue transazioni "bitcoin-cli" e combinarle con transazioni generate da persone (o programmi) su altre piattaforme. Oltre a ciò, puoi fare alcune cose che semplicemente non sarebbero facili usando altre meccaniche.

Di seguito sono riportati tre esempi di utilizzo dei PSBT per: multi-sigs, pooling di denaro e coinjoin.

> :avviso: **AVVISO di VERSIONE:** Questa è un'innovazione di Bitcoin Core v 0.17.0. Le versioni precedenti di Bitcoin Core non saranno in grado di funzionare con il PSBT mentre è in corso (sebbene saranno comunque in grado di riconoscere la transazione finale).

## Usa una PSBT per spendere fondi MultiSig

Supponiamo di aver creato un indirizzo multisig, proprio come hai fatto nel [Capitolo 6.3](06_3_Inviare_e_Ricevere_una_Multifirma_Automatizzata.md)..
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
E hai dei soldi dentro:
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
Potresti _potresti_ spendere le monere utilizzando i meccanismi visti nel [Capitolo 6](06_0_Ampliare_le_Transazioni_Bitcoin_con_Multifirme.md), dove hai firmato in serie una transazione, ma invece mostreremo il vantaggio dei PSBT per multi-sigs: puoi generare una singola PSBT, consentire tutti di firmare in parallelo e poi a combinare i risultati! Non è più necessario passare faticosamente un hex in continua espansione da persona a persona, il che accelera le cose e riduce le possibilità di errori.

Per dimostrare questa metodologia, estrarremo quegli 0,02 BTC dal multi-sig e li divideremo tra i due firmatari, ciascuno dei quali ha generato un nuovo indirizzo a tale scopo:
```
machine1$ bitcoin-cli getnewaddress
tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma
machine2$ bitcoin-cli getnewaddress
tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp
```
La prima cosa che facciamo è creare un PSBT sulla macchina di nostra scelta. (Non importa quale.) Dobbiamo utilizzare `createpsbt` come visto nel[Capitolo 7.1](07_1_Creare_una_Transazione_Bitcoin_Parzialmente_Firmata.md) per questo, non il più semplice `walletcreatefundedpsbt`, perché abbiamo bisogno del controllo extra della selezione del denaro protetto dal multi -sig. (Questo sarà il caso di tutti e tre gli esempi in questa sezione, il che dimostra perché di solito è necessario utilizzare `createpsbt` per cose complesse.)
```
machine1$ utxo_txid=53ec62c5c2fe8b16ee2164e9699d16c7b8ac30ec53a696e55f09b79704b539b5
machine1$ utxo_vout=0
machine1$ split1=tb1qem5l3q5g5h6fsqv352xh4cy07kzq2rd8gphqma
machine1$ split2=tb1q3krplahg4ncu523m8h2eephjazs2hf6ur8r6zp
machine1$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$split1'": 0.009998,"'$split2'": 0.009998 }''')
```
Dovrai quindi inviare $psbt a tutti per la firma:
```
machine1$ echo $psbt
cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAAAAAA=
```
Ma devi solo inviarlo una volta! E lo fai contemporaneamente.
Ecco il risultato sulla prima macchina, dove generiamo il PSBT:
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
Questo dimostra che le informazioni UTXO sono state importate, e che c'è una _firma parziale_, ma che la firma del singolo input non è ancora completa.

Ecco la stessa cosa sull'altra macchina:
```
machine2$ psbt=cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAAAAAA=
machine2$ psbt_p2=$(bitcoin-cli walletprocesspsbt $psbt | jq -r '.psbt')
machine3$ echo $psbt_p2
cHNidP8BAHECAAAAAbU5tQSXtwlf5ZamU+wwrLjHFp1p6WQh7haL/sLFYuxTAAAAAAD/////AnhBDwAAAAAAFgAUzun4goil9JgBkaKNeuCP9YQFDad4QQ8AAAAAABYAFI2GH/borPHKKjs91ZyG8uigq6dcAAAAAAABASu4gx4AAAAAACIAICJMtQOn94NXmbnCLuDDx9k9CQNW4w5wAVw+u/pRWjB0IgIDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZHMEQCIDJ71isvR2We6ym1QByLV5SQ+XEJD0SAP76fe1JU5PZ/AiB3V7ejl2H+9LLS6ubqYr/bSKfRfEqrp2FCMISjrWGZ6QEBBUdSIQONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDiEDeJ9UNCNnDhaWZ/9+Hy2iqX3xsJEicuFC1YJFGs69BjZSriIGA3ifVDQjZw4Wlmf/fh8toql98bCRInLhQtWCRRrOvQY2ENPtiCUAAACAAAAAgAYAAIAiBgONc63yx+oz+dw0t3titZr0M8HenHYzMtp56D4VX5YDDgRZu4lPAAAiAgNJzEMyT3rZS7QHqb8SvFCv2ee0MKRyVy8bY8tVUDT1KhDT7YglAAAAgAAAAIADAACAAA==
```
Tieni presente che abbiamo gestito la firma di questo multi-sig generando un PSBT non firmato ma contenendo l'UTXO corretto, consentendo quindi a ciascuno degli utenti di elaborare quel PSBT per conto proprio, aggiungendo input e firme. Di conseguenza, abbiamo due PSBT, ciascuno dei quali contiene una firma e non l'altra. Ciò non funzionerebbe nel classico scenario multi-sig, perché tutte le firme devono essere serializzate. Qui, invece, possiamo firmare in parallelo e poi utilizzare il ruolo "Combinatore" per mescolarli insieme.

Andiamo di nuovo su una delle due macchine e ci assicuriamo di avere entrambi i PSBT nelle variabili, quindi li combiniamo:
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
Ha funzionato! Basta finalizzare e inviare e il gioco è fatto:
```
machine2$ psbt_c_hex=$(bitcoin-cli finalizepsbt $psbt_c | jq -r '.hex')
standup@btctest2:~$ bitcoin-cli -named sendrawtransaction hexstring=$psbt_c_hex
ee82d3e0d225e0fb919130d68c5052b6e3c362c866acc54d89af975330bb4d16
```
Ovviamente, non c'è stato un grande miglioramento nell'usare questo metodo rispetto alla firma seriale di una transazione per un multisig 2 su 2 quando tutti usavano `bitcoin-cli`: avremmo potuto passare una transazione grezza con firme parziali da un utente a l'altro con la stessa facilità con cui invii quel PSBT. Ma questo era il caso più semplice. Man mano che approfondiamo multisig più complessi, questa metodologia diventa sempre migliore.

Prima di tutto, è indipendente dalla piattaforma. Finché tutti utilizzano un servizio che supporta Bitcoin Core 0.17, saranno tutti in grado di firmare questa transazione, il che non è vero quando i classici multi-sig vengono trasmessi tra piattaforme diverse.

In secondo luogo, è molto più scalabile. Considera un multisig 3 su 5. Secondo la vecchia metodologia la strringa 'hex' dovrebbe passare da persona a persona, aumentando notevolmente i problemi in caso di rottura di un singolo anello della catena. Qui, gli altri utenti devono semplicemente rispedire i PSBT al Creatore e, non appena ne avrà abbastanza, potrà generare la transazione finale.

## Usa un PSBT per raggruppare denaro

I multisig come quello utilizzato nell'esempio precedente vengono spesso utilizzati per ricevere pagamenti per il lavoro collaborativo, che si tratti di royalty per un libro o di pagamenti effettuati a un'azienda. In quella situazione, l’esempio sopra funziona alla grande: i due partecipanti ricevono i loro soldi che poi si dividono. Ma che dire del caso contrario, in cui due (o più) partecipanti vogliono costituire una joint venture e hanno bisogno di finanziarla?

La risposta tradizionale è creare un multisig, quindi chiedere ai partecipanti di inviarvi i propri fondi individualmente. Il problema è che il primo pagatore deve dipendere dalla buona fede del secondo, e questo non si basa sulla forza di Bitcoin, che non necessita della _fiducia_ altrui. Fortunatamente, con l’avvento dei PSBT, ora possiamo effettuare pagamenti trustless che mettono in comune i fondi.

> :book: ***Cosa significa trustless?*** "Senza fiducia" significa che nessun partecipante ha bisogno di fidarsi degli altri partecipanti. Si aspettano invece che i protocolli software garantiscano che tutto venga attuato in modo equo e nel modo previsto. Bitcoin è un protocollo trustless perché non è necessario che nessun altro agisca in buona fede; il sistema lo gestisce. Allo stesso modo, i PSBT consentono la creazione trustless di transazioni che mettono in comune o dividono i fondi.

L'esempio seguente mostra due utenti che hanno ciascuno 0,010 BTC che desiderano raggruppare nell'indirizzo multisig `tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0`, creato sopra.
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
Impostano variabili per utilizzare tali transazioni:
```
machine1$ utxo_txid_1=2536855bc8588e87206600cc980c30e8a65cf7f81002a34d6c37535e38a5b9db
machine1$ utxo_vout_1=0
machine1$ utxo_txid_2=d7c7c24aeb1a61cdab81be6aefbf6c6a27562079629ffd9da4729bb82d93e4fe
machine1$ utxo_vout_2=0
machine1$ multisig=tb1qyfxt2qa877p40xdecghwps78my7sjq6kuv88qq2u86al5526xp6qfqjud0

```
E creano la  PSBT:
```
machine1$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$multisig'": 0.019998 }''')
```
Ecco come si vede:
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
Non importa che le transazioni siano di proprietà di due persone diverse o che le loro informazioni complete appaiano su due macchine diverse. Questo PSBT di finanziamento funzionerà esattamente come il PSBT multisig: una volta che tutte le parti controllanti avranno firmato, la transazione potrà essere finalizzata.

Ecco il processo, questa volta passando il PSBT parzialmente firmato da un utente all'altro invece di dover combinare le cose alla fine.
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
Abbiamo utilizzato una PSBT per raccogliere denaro in un multisig senza dover fidarci!

## Usa una PSBT per CoinJoin

CoinJoin è un'altra applicazione Bitcoin che richiede fiducia. Qui hai una varietà di parti che non necessariamente si conoscono che uniscono i soldi e li recuperano.

La metodologia per gestirlo con le PSBT è esattamente la stessa che hai visto negli esempi precedenti, come dimostra il seguente pseudo-codice:
```
$ psbt=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' }, { "txid": "'$utxo_txid_3'", "vout": '$utxo_vout_3' } ]''' outputs='''{ "'$split1'": 1.7,"'$split2'": 0.93,"'$split3'": 1.4 }''')
```
Ogni utente inserisce il proprio UTXO e ognuno riceve un output corrispondente.

Il modo migliore per gestire un CoinJoin è inviare il PSBT di base a tutte le parti (che potrebbero essere numerose), quindi chiedere a ciascuno di firmare il PSBT e rispedirlo a una singola parte che combinerà, finalizzerà e invierà.

> :book: ***Che cos'è CoinJoin?*** CoinJoin è una metodologia in base alla quale un gruppo di persone può mescolare insieme le proprie monete, contribuendo ad aumentare la fungibilità. Ogni persona inserisce e preleva la stessa quantità di monete (meno le commissioni di transazione) in una transazione tra più persone condotta simultaneamente da un numero elevato di persone. È progettato per essere "trustless" in modo che le parti non abbiano bisogno di conoscersi o fidarsi l'una dell'altra. Un CoinJoin alla fine aumenta l’anonimato rendendo difficile il tracciamento delle monete. Ci sono alcune wallet come sparrow e samourai che supportano il coinjooin e una serie di servizi "whirpool" su larga scala per migliorare l'anonimato.

## Riepilogo: utilizzo di una transazione Bitcoin parzialmente firmata

Ora hai visto il processo PSBT che hai imparato in [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md) in uso in tre esempi di vita reale: creazione di un multi-sig, pooling di fondi e CoinJoining. Tutto ciò era teoricamente possibile nel Bitcoin classico facendo firmare più persone a transazioni attentamente costruite, ma i PSBT lo rendono standardizzato e semplice.

> :fire: ***Qual è il potere di un PSBT?*** Un PSBT consente la creazione di transazioni trustless tra più parti e più macchine. Se più di una parte avesse bisogno di finanziare una transazione, se più di una parte avesse bisogno di firmare una transazione, o se una transazione dovesse essere creata su una macchina e firmata su un’altra, allora un PSBT rende tutto semplice senza dipendere dal meccanismi di firma parziale non standardizzati che esistevano prima del PSBT.

Quest'ultimo punto, sulla creazione di una transazione su una macchina e sulla firma su un'altra, è un elemento dei PSBT a cui non siamo ancora arrivati. È il cuore dei portafogli hardware, dove spesso desideri creare una transazione su un nodo completo, quindi trasferirla a un portafoglio hardware quando è richiesta una firma. Questo è l'argomento dell'ultima sezione (e del nostro quarto esempio di vita reale) in questo capitolo sui PSBT.

## Cosa segue??

Continua con l' "Espansione delle transazioni Bitcoin con PSBT" nel [Capitolo 7.3: Integrazione con hardware wallet](07_3_Integrazione_con_Hardware_Wallets.md).
