# 10.5: Programando um Script Segwit
> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

O Segwit introduziu uma série de novas opções para tipos de endereço e também de scripts. Na seção [§9.5: Programando um P2WPKH](09_5_Scripting_a_P2WPKH.md) explicamos como o novo tipo de endereço Bech32 variou os scripts padrão encontrados na maioria das transações tradicionais. Este capítulo examina os três outros tipos de scripts introduzidos pela atualização do Segwit: o P2SH-Segwit (que era o endereço "nested Segwit" de transição, quando o Segwit passou a ser usado), o P2WSH (que é o Segwit equivalente ao endereço P2SH , assim como P2WPKH é o Segwit equivalente ao endereço P2PKH) e o endereço nested P2WSH.

Esta é outra situação onde realmente não teremos que nos preocupar com os nuances ao trabalhar com o ```bitcoin-cli```, mas é útil saber como tudo funciona.

## Compreendendo um Script P2SH-Segwit

O endereço P2SH-Segwit é um tipo em extinção. Foi basicamente uma medida paliativa enquanto o Bitcoin fazia a transição para o Segwit que permitia a um usuário criar um endereço Segwit e, em seguida, alguém como uma exchange que ainda não habilitou o Segwit ou carteira antiga financiasse esse endereço.

Se precisarmos usar um, há uma opção para criar um endereço P2SH-Segwit usando ```getnewaddress```:
```
$ bitcoin-cli getnewaddress -addresstype p2sh-segwit
2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU
```
O endereço começa com um ```2``` (ou ```3```) revelando-o como um script.

> :book: ***Por que os nodes antigos não podem enviar para endereços Segwit nativos?*** Na seção [§10.1](10_1_Understanding_the_Foundation_of_P2SH.md) observamos que havia um número definido de transações padrão no Bitcoin. Na verdade, não podemos bloquear uma transação com um script que não seja um desses tipos padrão. O Segwit agora é reconhecido como um desses padrões, mas um node antigo não saberá disso e, portanto, se recusará a enviar tal transação para a proteção daquele que o está enviando. Encapsular um endereço Segwit dentro de um script padrão em hash resolve o problema.

Quando você olha para um UTXO enviado para esse endereço, pode ver que o ```desc``` é diferente, revelando um endereço WPKH envolto em um script:
```
$ bitcoin-cli listunspent
  {
    "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
    "vout": 1,
    "address": "2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU",
    "redeemScript": "001443ab2a09a1a5f2feb6c799b5ab345069a96e1a0a",
    "scriptPubKey": "a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c687",
    "amount": 0.00095000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([f004311c/0'/0'/3']03bb469e961e9a9cd4c23db8442d640d9b0b11702dc0126462ac9eb88b64a4dd48))#p29e839h",
    "safe": true
  }
```
Mais importante ainda, é a presença de um ```redeemScript```, que decodifica para ```OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a```. Isso deve parecer familiar, porque é um ```OP_0``` seguido por um hexcode de 20 bytes de um hash de chave pública. Em outras palavras, um P2SH-SegWit é apenas um SegWit ```scriptPubKey``` preso em um script. Isso é tudo que há para fazer. Corresponde precisamente ao modo como os multisigs modernos são um multsig preso em um P2SH, conforme discutido na seção [§10.4: Programando um Multisig](10_4_Scripting_a_Multisig.md).

A transação bruta revela um pouco mais de detalhes quando olhamos para o ```vout``` ```1```:
```
$ hex=$(bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa" | jq -r '.hex')
$ bitcoin-cli decoderawtransaction $hex
{
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "hash": "6866490b16a92d68179e1cf04380fd08f16ec80bf66469af8d5e78ae624ff202",
  "version": 2,
  "size": 249,
  "vsize": 168,
  "weight": 669,
  "locktime": 1780788,
  "vin": [
    {
      "txid": "4779bb137ddbcaf796f905e264554b1ec28c0f3ab4538ca02eac5099bfc3fc1e",
      "vout": 0,
      "scriptSig": {
        "asm": "0014c4ea10874ae77d957e170bd43f2ee828a8e3bc71",
        "hex": "160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71"
      },
      "txinwitness": [
        "3044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b5715301",
        "033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9"
        ]
      }
    },
    {
      "value": 0.01063793,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 b780fc2e945bea71b9ee2d8d2901f00914a25fbd OP_EQUAL",
        "hex": "a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9yWARt5E3TQsX2RjsauxSZaEZVhinAS4h"
        ]
      }
    }
  ]
}
```
Isso confirma que este é apenas um P2SH normal, bloqueado por ```"OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG"```. É quando o script de resgate é executado que a mágica ocorre. Assim como com um P2WPKH, um node antigo verá ```OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a``` e verificará automaticamente, enquanto um novo node verá isso, saberá que é um P2WPKH e, assim, irá verificar o ```witness```. Podemos consultar isso na seção [§9.5: Programando um P2WPKH](09_5_Scripting_a_P2WPKH.md).

> :book: ***Quais são as desvantagens das transações nested Segwit?*** Elas são maiores do que as transações Segwit nativas, então temos algumas das vantagens do Segwit, mas não todas.

## Compreendendo um Script P2WSH

Ao contrário, as transações P2WSH devem ser cada vez mais utilizadas, uma vez que são a substituição do Segwit nativo para o P2SH, oferecendo todas as mesmas vantagens de tamanho de bloco que foram criadas com transações Segwit P2WPKH nativas.

Este é um exemplo de endereço P2WSH:
[https://mempool.space/pt/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7](https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7).

Os detalhes mostram que um UTXO enviado para este endereço está bloqueado com um ```scriptPubKey``` como este:
```
OP_0 OP_PUSHDATA (32 bytes) 1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262
```

O funcionamento é exatamente como um endereço P2WPKH, a única diferença é que ao invés de um hash de chave pública de 20 bytes, o UTXO inclui um hash de script de 32 bytes. Assim como com um P2WPKH, os nodes antigos apenas verificam isso, enquanto os novos nodes reconhecem que se trata de um P2WSH e verificam internamente o script conforme descrito nas seções anteriores, mas usando os dados da ```witness```, que agora incluem o script de resgate.

Há também mais uma variante, um script P2WSH embutido em um script P2SH, que funciona muito como o P2SH-Segwit descrito acima, mas para scripts P2WSH aninhados. (Ufa!)

## Resumo: Programando um Script Segwit

Existem dois tipos de scripts P2SH relacionados ao Segwit.

O endereço P2SH-Segwit é um endereço nested Segwit que incorpora o Segwit ```scriptPubkey``` simples dentro de um Script, assim como multisigs são incorporados nos scripts hoje em dia: a chave do tipo Segwit é desenrolada e então analisada normalmente em uma máquina que entende o Segwit . O objetivo é a compatibilidade com versões anteriores dos nodes antigos que, de outra forma, não seriam capazes de enviar saldos para endereços Segwit nativos.

O endereço P2WSH é uma variante Segwit de P2SH, assim como P2WPKH é uma variante Segwit de P2WSH. Ele funciona com a mesma lógica e é identificado por ter um hash de 32 bytes em vez de um hash de 20 bytes. O objetivo é estender as vantagens do Segwit a outros tipos de scripts.

## O Que Vem Depois?

Vamos continuar "Incorporando Scripts em Transações P2SH no Bitcoin" na seção [§10.6: Gastando uma Transação P2SH](10_6_Spending_a_P2SH_Transaction.md).