# 17.5: Usando Scripts na Libwally

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Na seção 3 deste capítulo, ao apresentar os scripts, dissemos que era provável que criássemos transações usando scripts com uma API e dissemos que este era um tópico para o futuro. Bem, agora o futuro chegou.

## Criando o Script

Criar o script é a coisa _mais fácil_ de se fazer na Libwally. Veja o exemplo a seguir, um simples [Script de Quebra-Cabeças](13_1_Writing_Puzzle_Scripts.md) ao qual retornamos de vez em quando:
```
OP_ADD 99 OP_EQUAL
```
Usando o `btcc`, podemos serializar isto.
```
$ btcc OP_ADD 99 OP_EQUAL
warning: ambiguous input 99 is interpreted as a numeric value; use 0x99 to force into hexadecimal interpretation
93016387
```
Anteriormente, construímos o script P2SH padrão manualmente, mas a Libwally pode fazer isso por nós.

Primeiro, a Libwally precisa converter o hex em bytes, uma vez que os bytes são o que realmente funciona:
```
  int script_length = strlen(script)/2;
  unsigned char bscript[script_length];
    
  lw_response = wally_hex_to_bytes(script,bscript,script_length,&written);
```
Então, executamos o comando `wally_scriptpubkey_p2sh_from_bytes` com os bytes, dizendo à Libwally para também fazer o `HASH160` para nós:
```
  unsigned char p2sh[WALLY_SCRIPTPUBKEY_P2SH_LEN];
          
  lw_response = wally_scriptpubkey_p2sh_from_bytes(bscript,sizeof(bscript),WALLY_SCRIPT_HASH160,p2sh,WALLY_SCRIPTPUBKEY_P2SH_LEN,&written);
```
Se olharmos os resultados do `p2sh`, veremos o seguinte:
```
a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87
```
Que [devemos nos lembrar](10_2_Building_the_Structure_of_P2SH.md) se divide em:
```
a9 / 14 / 3f58b4f7b14847a9083694b9b3b52a4cea2569ed / 87
```
Este é o nosso velho amigo `OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL`.

Basicamente, a Libwally pegou nosso script de resgate serializado, fez o hash para nós com SHA-256 e RIPEMD-160 e aplicou o enquadramento padrão para transformá-lo em um P2SH adequado. Fizemos um trabalho semelhante ao feito na seção [§10.2](10_2_Building_the_Structure_of_P2SH.md), mas com um excesso de comandos shell.

Na verdade, podemos verificar novamente o nosso trabalho usando os mesmos comandos do §10.2:
```
$ redeemScript="93016387"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= 3f58b4f7b14847a9083694b9b3b52a4cea2569ed
```

## Criando uma Transação

A fim de fazer uso do `pubScriptKey` que acabamos de criar, precisamos criar uma transação e incorporar o `pubScriptKey` dentro dela (e esta é a grande mudança do `bitcoin-cli`: podemos criar manualmente uma transação com um script P2SH).

O processo de criação de uma transação na Libwally é muito intenso, assim como o processo de criação de um PSBT, portanto, vamos apenas esboçá-lo, pegando um atalho e, em seguida, deixando um método sem atalhos para investigação futura.

Criar uma transação em si é fácil, só precisamos dizer ao comando `wally_tx_init_alloc` nosso número de versão, o tempo de bloqueio e o número de entradas e saídas:
```
  struct wally_tx *tx;
  lw_response = wally_tx_init_alloc(2,0,1,1,&tx);
```
Preencher essas entradas e saídas é onde as coisas ficam complicadas!

### Criando uma Saída de Transação

Para criar uma saída, dizemos a `wally_tx_output_init_alloc` quantos satoshis estamos gastando, além de entregar o script de bloqueio:
```
  struct wally_tx_output *tx_output;
  lw_response = wally_tx_output_init_alloc(95000,p2sh,sizeof(p2sh),&tx_output);
```
Essa parte não foi nem um pouco difícil e nos permitiu que finalmente incorporássemos um P2SH em um `vout`.

Mais um comando o adiciona à nossa transação:
```
  lw_response = wally_tx_add_output(tx,tx_output);
```

### Criando uma Entrada de Transação

Criar a entrada é muito mais difícil porque temos que empilhar informações nas rotinas de criação, nem todas as quais são intuitivamente acessíveis quando usamos a Libwally. Então, ao invés de mergulharmos fundo nessa biblioteca, é neste momento que pegamos nosso atalho. Escrevemos o código de forma que seja passado o código hexadecimal para uma transação que já foi criada e, em seguida, apenas reutilizamos a entrada.

A conversão do código hexadecimal é feita com `wally_tx_from_hex`:
```
  struct wally_tx *utxo;
  lw_response = wally_tx_from_hex(utxo_hex,0,&utxo);
```
Então podemos roubar as entradas de nosso código hexadecimal para criar uma entrada com a Libwally:
```
  struct wally_tx_input *tx_input;
  lw_response = wally_tx_input_init_alloc(utxo->inputs[0].txhash,sizeof(utxo->inputs[0].txhash),utxo->inputs[0].index,0,utxo->inputs[0].script,utxo->inputs[0].script_len,utxo->inputs[0].witness,&tx_input);
  assert(lw_response == WALLY_OK);            
```
Como seria de se esperar, adicionamos essa entrada à nossa transação:
```
  lw_response = wally_tx_add_input(tx,tx_input);
```

> **NOTA:** Obviamente, iremos querer criar nossas próprias entradas se estivermos usando Libwally para aplicações reais, mas isso é uma primeira etapa. E, na verdade, pode ser útil para integração com o `bitcoin-cli`, como veremos na seção [§17.7](17_7_Integrating_Libwally_and_Bitcoin-CLI.md).

### Vendo uma Transação

Teoricamente, poderíamos assinar e enviar esta transação para nosso programa C construído na Libwally, mas mantendo a ideia de que estamos apenas usando um programa C simples para substituir um P2SH, vamos imprimir o novo hex. Isto é feito com a ajuda de `wally_tx_to_hex`:
```
  char *tx_hex;
  lw_response = wally_tx_to_hex(tx,0, &tx_hex);

  printf("%s\n",tx_hex);
```
Mostraremos como fazer uso disso na seção §17.7.

## Testando Nosso Script de Substituição

Podemos pegar o código de teste do [diretório src/](../src/17_5_replacewithscript.c) e compilá-lo:
```
$  cc replacewithscript.c -lwallycore -o replacewithscript
```
Depois, preparamos uma transação hexadecimal e um script hexadecimal serializado:
```
hex=020000000001019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972dfeffffff026f8460000000000017a914ba421212a629a840492acb2324b497ab95da7d1e87306f0100000000001976a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac02463043021f757054fa61cfb75b64b17230b041b6d73f25ff9c018457cf95c9490d173fb4022075970f786f24502290e8a5ed0f0a85a9a6776d3730287935fb23aa817791c01701210293fef93f52e6ce8be581db62229baf116714fcb24419042ffccc762acc958294e6921b00

script=93016387
```
Podemos então executar o programa de substituição:
```
$ ./replacewithscript $hex $script
02000000019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d0000000001187301000000000017a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed8700000000
```
Podemos ver os resultados com o `bitcoin-cli`:
```
$ bitcoin-cli decoderawtransaction $newhex
{
  "txid": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "hash": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "version": 2,
  "size": 106,
  "vsize": 106,
  "weight": 424,
  "locktime": 0,
  "vin": [
    {
      "txid": "6e000b69fcd8e1d7451c6ccd58fcc6d78dc18fe5a11b1b96a7242507bbce2795",
      "vout": 0,
      "scriptSig": {
        "asm": "0014cc6e8522f0287b87b7d0a83629049c2f2b0e972d",
        "hex": "160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
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
O `vin` deve apenas corresponder à entrada que substituímos, mas é o `vout` que é empolgante. Criamos uma transação com um `scripthash`!

## Resumo: Usando Scripts na Libwally

A criação de transações na Libwally é outro tópico que pode ocupar um capítulo inteiro, mas o melhor é que uma vez que demos esse salto, podemos introduzir um P2SH `scriptPubKey`, e só essa parte é muito fácil. Embora a metodologia detalhada neste capítulo exija que já tenhamos um hex de transação em mãos (provavelmente criado com o `bitcoin-cli`), se nos aprofundarmos na Libwally, podemos fazer tudo sozinhos.

> :fire: ***Qual é o poder dos scripts na Libwally?*** De maneira bem simples, podemos fazer algo que não podíamos antes. Criar uma transação bloqueada com um P2SH arbitrário.

## O Que Vem Depois?

Vamos aprender mais sobre "Programando Bitcoin com Libwally" na seção [§17.6: Usando Outras Funções na Libwally](17_6_Using_Other_Functions_in_Libwally.md).