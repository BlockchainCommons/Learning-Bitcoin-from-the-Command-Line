# 17.7: Integrando Libwally e Bitcoin-CLI

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

A Libwally é limitada. Trata-se de manipular seeds, chaves, endereços e outros elementos de carteiras, com algumas funções adicionais relacionadas a transações e PSBTs que podem ser úteis para serviços que não estão conectados a nodes completos na Internet. No final das contas, no entanto, precisaremos de serviços de nodes completos para aproveitar as vantagens da Libwally.

Esta última seção oferecerá alguns exemplos de uso de programas Libwally para complementar um ambiente `bitcoin-cli`. Embora esses exemplos impliquem que esses serviços estão todos na mesma máquina, eles podem se tornar ainda mais poderosos se o serviço `bitcoin-cli` estiver diretamente conectado à internet e o serviça Libwally não.

## Compartilhando uma Transação

Na seção [§17.5: Usando Scripts na Libwally](17_5_Using_Scripts_in_Libwally.md) detalhamos como a Libwally poderia ser usada para reescrever uma transação existente, para fazer algo que o `bitcoin-cli` não pode, ou seja, produzir uma transação que contém um P2SH único. Obviamente, esta é uma parte essencial, mas não o todo; se você decidir se aprofundar na Libwally, poderá criar transações inteiras por conta própria. Mas esta metodologia abreviada também tem seu próprio uso, mostrar como as transações podem ser passadas de um lado para outro entre `bitcoin-cli` e Libwally, demonstrando um primeiro exemplo de como usá-los de forma complementar.

Para demonstrar totalmente essa metodologia, criaremos uma transação com o `bitcoin-cli`, usando este UTXO:
```
  {
    "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
    "vout": 1,
    "address": "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo",
    "label": "",
    "scriptPubKey": "76a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac",
    "amount": 0.00094000,
    "confirmations": 17375,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([ce0c7e14/0'/0'/5']0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65)#qldtsl65",
    "safe": true
  }
```
Agora, já sabemos como configurar uma transação com o `bitcoin-cli`:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient=tb1qycsmq3jas5wkhf8xrfn8k7438cm5pc8h9ae2k0
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.0009 }''')
```
Embora tenhamos colocado um destinatário e uma quantia na saída, isto é irrelevante, porque nós o reescreveremos. Um código mais elaborado poderia ler as informações `vout` existentes antes de reescrever, mas estamos mantendo as coisas muito próximas do nosso [código original](../src/17_5_replacewithscript.c).

Aqui está a única alteração necessária, para permitir que especifiquemos os satoshis no `vout`, sem ter que codificá-lo, como no original:
```
...
  int satoshis = atoi(argv[3]);
...
  lw_response = wally_tx_output_init_alloc(satoshis,p2sh,sizeof(p2sh),&tx_output);
...
```
Então executamos as coisas como antes:
```
$ newtxhex=$(./replacewithscript $rawtxhex $script 9000)
```
Esta é a aparência da transação original:
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "hash": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "version": 2,
  "size": 82,
  "vsize": 82,
  "weight": 328,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 2621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "hex": "00142621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qycsmq3jas5wkhf8xrfn8k7438cm5pc8h9ae2k0"
        ]
      }
    }
  ]
}
```
E aqui está a transação reescrita pela Libwally para usar em um P2SH:
```
standup@btctest:~/c$ bitcoin-cli decoderawtransaction $newtxhex
{
  "txid": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "hash": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "version": 2,
  "size": 83,
  "vsize": 83,
  "weight": 332,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL",
        "hex": "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2My2ApqGcoNXYceZC4d7fipBu4GodkbefHD"
        ]
      }
    }
  ]
}
```
Depois, podemos assiná-la normalmente com o `bitcoin-cli`:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $newtxhex | jq -r '.hex')
```
E, como podemos ver, o resultado é uma transação legítima pronta para ir para a rede Bitcoin:
```
$ bitcoin-cli decoderawtransaction $signedtx
{
  "txid": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "hash": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "version": 2,
  "size": 189,
  "vsize": 189,
  "weight": 756,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "3044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe0[ALL] 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65",
        "hex": "473044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe001210368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL",
        "hex": "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2My2ApqGcoNXYceZC4d7fipBu4GodkbefHD"
        ]
      }
    }
  ]
}
```
Pronto! Esse é o poder da Libwally com o `bitcoin-cli`.

Obviamente, também podemos passar um PSBT usando as funções descritas na seção [§17.4](17_4_Using_PSBTs_in_Libwally.md) e esta é uma metodologia mais atualizada para o uso moderno do Bitcoin; mas em qualquer um dos exemplos, o conceito de passar transações do `bitcoin-cli` para códiga Libwally e vice-versa deve ser semelhante.

## Importando e Exportando Seeds BIP39

Infelizmente, nem todas as interações entre a Libwally e o `bitcoin-cli` são fáceis. Por exemplo, seria bom se pudéssemos exportar uma seed HD do `bitcoin-cli` para gerar a frase mnemônica com a Libwally, ou gerar uma seed de uma frase mnemônica usando a Libwally e depois importá-la para o `bitcoin-cli `. Infelizmente, nada disso é possível neste momento. Uma frase mneômica é traduzida em uma seed usando HMAC-SHA512, o que significa que o resultado é 512 bits. No entanto, o `bitcoin-cli` exporta seeds HD (usando `dumpwallet`) e importa seeds HD (usando `sethdseed`) com um comprimento de 256 bits. Até que isso seja mudado, não há nada que possamos fazer.

> :book: ***Qual é a diferença entre entropia e uma seed?*** A Libwally diz que criamos nossas frases mnemônicas a partir de entropia. Isso é essencialmente a mesma coisa que uma seed, pois ambos são números grandes e aleatórios. Portanto, se o `bitcoin-cli` fosse compatível com as seeds de frase mnemônica de 512 bits, poderíamos usar uma para gerar as frases mnemônicas e obter os resultados esperados.

> :book: ***Qual é a diferença entre entropia e entropia bruta?*** Nem toda entropia é a mesma. Quando inserimos entropia em um comando que cria uma seed mnemônica, ela tem um comprimento específico e bem conhecido. Mudar de entropia bruta para entropia requer alterar a entropia bruta até que se tenha o comprimento e o formato corretos, e nesse ponto podemos reutilizar essa entropia (não bruta) para sempre recriar os mesmos mnemônicos (razão pela qual a entropia é efetivamente a mesma coisa que uma seed naquele ponto, mas a entropia bruta não).

## Importando Chaves Privadas

Felizmente, podemos fazer quase a mesma coisa importando uma chave privada gerada na Libwally. Dê uma olhada no [genhd-for-import.c](../src/17_7_genhd_for_import.c), uma versão simplificada do programa `genhd` da seção [§17.3](17_3_Using_BIP32_in_Libwally.md) que também usa a biblioteca `jansson` da seção [§16.1](16_1_Accessing_Bitcoind_with_C.md) para saída regularizada.

O código atualizado também contém uma alteração importante, pois ele solicita uma impressão digital da Libwally para que se possa criar um caminho de derivação de maneira adequada:
```
  char account_fingerprint[BIP32_KEY_FINGERPRINT_LEN];
  lw_response = bip32_key_get_fingerprint(key_account,account_fingerprint,BIP32_KEY_FINGERPRINT_LEN);

  char *fp_hex;
  lw_response = wally_hex_from_bytes(account_fingerprint,BIP32_KEY_FINGERPRINT_LEN,&fp_hex);
```

> :aviso: **AVISO:** Lembre-se de que a impressão digital nos caminhos de derivação é arbitrária. Como a Libwally fornece um, nós o estamos usando, mas se não tivéssemos um, poderíamos adicionar um código hexadecimal arbitrário de 4 bytes como uma impressão digital em nosso caminho de derivação.

Certifique-se de compilar o novo código com a biblioteca `jansson`, após instalá-lo (se necessário) de acordo com a seção [§16.1](16_1_Accessing_Bitcoind_with_C.md).
```
$ cc genhd-for-import.c -lwallycore -lsodium -ljansson -o genhd-for-import
```
Quando executarmos o novo programa, ele nos dará uma lista de saída com todas as informações:
```
$ ./genhd-for-import 
{
  "mnemonic": "physical renew say quit enjoy eager topic remind riot concert refuse chair",
  "account-xprv": "tprv8yxn8iFNgsLktEPkWKQpMqb7bcx5ViFQEbJMtqrGi8LEgvy8es6YeJvyJKrbYEPKMw8JbU3RFhNRQ4F2pataAtTNokS1JXBZVg62xfd5HCn",
  "address": "tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n",
  "derivation": "[d1280779/84h/1h/0h]"
}
```
Temos o `mnemonic` que podemos recuperar, um `account-xprv` que podemos importar, uma `derivation` para usar para a importação e um `address` de amostra, que podemos usar para testar a importação.

Agora podemos recorrer às lições aprendidas na seção [§3.5](03_5_Understanding_the_Descriptor.md) sobre como transformar esse xprv em um descritor e importá-lo.

Primeiro, precisamos descobrir o checksum:
```
$ xprv=tprv8yxn8iFNgsLktEPkWKQpMqb7bcx5ViFQEbJMtqrGi8LEgvy8es6YeJvyJKrbYEPKMw8JbU3RFhNRQ4F2pataAtTNokS1JXBZVg62xfd5HCn
$ dp=[d1280779/84h/1h/0h]
$ bitcoin-cli getdescriptorinfo "wpkh($dp$xprv/0/*)"
{
  "descriptor": "wpkh([d1280779/84'/1'/0']tpubDWepH8HcqF2RmhRYPy5QmFFEAeU1f3SJotu9BMta8Q8dXRDuHFv8poYqUUtEiWftBjtKn1aNhi9Qg2P4NdzF66dShYvB92z78WJbYeHTLTz/0/*)#f8rmqc0z",
  "checksum": "46c00dk5",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": true
}
```

Há três coisas a serem observadas aqui:

1. Usamos o `wpkh` como a função em nosso caminho de derivação. Isso porque queremos gerar endereços Segwit modernos, não endereços legados. Isso corresponde ao uso na Libwally da função `wally_bip32_key_to_addr_segwit`. A coisa mais importante, entretanto, é ter as mesmas expectativas com a Libwally e o `bitcoin-cli` (e nosso descritor) de que tipo de endereço estamos gerando, para que tudo dê certo;
2. Usamos o caminho `/0/*` porque queríamos os endereços externos para esta conta. Se, em vez disso, quiséssemos alterar os endereços, usaríamos `/1/*`;
3. Não vamos usar a linha `descriptor` que foi retornada, pois é para um endereço `xpub`. Ao invés disso, aplicaremos o `checksum` retornado ao `xprv` que já temos.
```
$ cs=$(bitcoin-cli getdescriptorinfo "wpkh($dp$xprv/0/*)" | jq -r ' .checksum')
```
Então conectamos isso em um `importmulti` para importar esta chave no `bitcoin-cli`:
```
$ bitcoin-cli importmulti '''[{ "desc": "wpkh('$dp''$xprv'/0/*)#'$cs'", "timestamp": "now", "range": 10, "watchonly": false, "label": "LibwallyImports", "keypool": false, "rescan": false }]'''
[
  {
    "success": true
  }
]

```
Aqui, importamos/geramos os primeiros dez endereços para a chave privada.

Vamos examinar o novo rótulo `LibwallyImports`:
```
$ bitcoin-cli getaddressesbylabel "LibwallyImports"
{
  "tb1qzeqrrt77xhvazq5g8sc9th0lzjwstknan8gzq7": {
    "purpose": "receive"
  },
  "tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n": {
    "purpose": "receive"
  },
  "tb1q8fsgxt0z9r9hfl5mst5ylxka2yljjxlxlvaf8j": {
    "purpose": "receive"
  },
  "tb1qg6dayhdk4qc6guutxvdweh6pctc9dpguu6awqc": {
    "purpose": "receive"
  },
  "tb1qdphaj0exvemxhgfpyh4p99wn84e2533u7p96l6": {
    "purpose": "receive"
  },
  "tb1qwv9mdqkpx6trtmvgw3l95npq8gk9pgllucvata": {
    "purpose": "receive"
  },
  "tb1qwh92pkrv6sps62udnmez65vfxe9n5ceuya56xz": {
    "purpose": "receive"
  },
  "tb1q4e98ln8xlym64qjzy3k8zyfyt5q60dgcn39d90": {
    "purpose": "receive"
  },
  "tb1qhzje887fyl65j4mulqv9ysmntwn95zpgmgvtqd": {
    "purpose": "receive"
  },
  "tb1q62xf9ec8zcfkh2qy5qnq4qcxrx8l0jm27dd8ru": {
    "purpose": "receive"
  },
  "tb1qlw85usfk446ssxejm9dmxsfn40kzsqce77aq20": {
    "purpose": "receive"
  }
}

```
O segundo em nossa lista realmente corresponde ao que temos de amostra (`tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n`). A importação desta chave privada e a derivação de dez endereços foram bem-sucedidas.

Se olharmos agora para trás na seção [§7.3](07_3_Integrating_with_Hardware_Wallets.md), veremos que esta era a mesma metodologia que usamos para importar endereços de uma hardware wallet (embora desta vez também importamos a chave privada como prova de conceito). A maior diferença é que anteriormente a informação era criada por uma caixa preta (literalmente: era um dispositivo Ledger), e desta vez nós mesmos criamos a informação usando a Libwally, mostrando como podemos fazer esse tipo de trabalho de maneira airgaped ou em outro dispositivo remoto e, em seguida, levá-lo ao `bitcoin-cli`.

## Importando Endereços

Obviamente, se podemos importar chaves privadas, também podemos importar endereços, o que geralmente significa importar endereços somente para observação _sem_ as chaves privadas.

Uma maneira de fazer isso é utilizar a metodologia `importmulti` acima, mas neste caso, usamos o endereço xpub fornecido (`wpkh([d1280779/84'/1'/0']tpubDWepH8HcqF2RmhRYPy5QmFFEAeU1f3SJotu9BMta8Q8dXRDuHFv8poYqUUtEiWftBjtKn1aNhi9Qg2P4NdzF66dShYvB92z78WJbYeHTLTz/0/*)#f8rmqc0z`) ao invés do xprv original. Essa é a melhor maneira de importar uma sequência inteira de endereços somente para observação.

Como alternativa, podemos importar endereços individuais. Por exemplo, considere o único endereço de amostra retornado pelo programa `genhd-for-import`:
```
$ ./genhd-for-import 
{
  "mnemonic": "finish lady crucial walk illegal ride hamster strategy desert file twin nature",
  "account-xprv": "tprv8xRujYeVN7CwBHxLoTHRdmzwdW7dKUzDfruSo56GqqfRW9QXtnxnaRG8ke7j65uNjxmCVfcagz5uuaMi2vVJ8jpiGZvLwahmNB8F3SHUSyv",
  "address": "tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d",
  "derivation": "[6214ecff/84h/1h/0h]"
}
```
Podemos importá-lo como um endereço de observação com `importaddress`:
```
$ bitcoin-cli -named importaddress address=tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d label=LibwallyWO rescan=false
$ bitcoin-cli getaddressesbylabel "LibwallyWO"
{
  "tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d": {
    "purpose": "receive"
  }
}
}
```

## Resumo: Integrando Libwally e Bitcoin-CLI

Com um conhecimento básico de Libwally, agora podemos complementar todo o trabalho das lições anteriores. A transferência de endereços, chaves, transações e PSBTs são apenas algumas das maneiras pelas quais podemos usar esses dois poderosos métodos de programação do Bitcoin juntos. Também há muito mais profundidade potencial se quisermos nos aprofundar na extensa biblioteca de funções da Libwally.

> :fire: ***Qual é o poder de integrar Libwally e Bitcoin-CLI?*** Uma das maiores vantagens da Libwally é que ela tem muitas funções que podem ser usadas offline. Em comparação, o Bitcoin Core é um programa em rede. Isso pode ajudá-lo a aumentar a segurança fazendo com que o `bitcoin-cli` passe chaves, endereços, transações ou PSBTs para uma fonte offline (que estaria executando programas Libwally). Além disso, a Libwally pode fazer coisas que o Bitcoin Core não pode, como gerar uma seed a partir de um mnemônico BIP39 (e mesmo se não pudermos importá-la para o Bitcoin Core, _podemos_ importar a chave mestra, conforme mostrado aqui).

## O Que Vem Depois?

Saiba mais sobre outras linguagens de programação no [Capítulo 18: Conversando com o Bitcoind com Outras Linguagens](18_0_Talking_to_Bitcoind_Other.md).