# 17.4: Usando PSBTs na Libwally

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Nós aprendemos tudo sobre as transações parcialmente assinadas no Bitcoin (PSBTs) na seção [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md) e na [§7.2](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md), e como vimos na [§7.3: Integrando com Hardware Wallets](/github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/07_3_Integrating_with_Hardware_Wallets.md), uma das nossas principais vantagens é a capacidade de integração com nodes offline, como Hardware Wallets. O HWI permitiu que passássemos comandos para uma carteira de hardware, mas o que a própria carteira usa para gerenciar os PSBTs? Por acaso, podemos usar algo na Libwally, como esta seção irá demonstrar.

Basicamente, a Libwally tem todas as funcionalidades PSBT, então se há algo que podemos fazer com o `bitcoind`, também podemos fazer usando a Libwally, mesmo se nosso dispositivo estiver offline. O que segue é uma introdução básica a um tópico bem complexo.

## Convertendo um PSBT

Converter um PSBT na estrutura interna da Libwally é incrivelmente fácil, basta executar `wally_psbt_from_base64` com um PSBT base64, que são as saídas produzidas pelo `bitcoin-cli`, como:

`cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==`

No entanto, é um pouco mais difícil de lidar com o resultado, porque a Libwally o converte em uma estrutura `wally_psbt` muito complexa.

Veja como é definido no arquivo `/usr/include/wally_psbt.h`:
```
struct wally_psbt {
    unsigned char magic[5];
    struct wally_tx *tx;
    struct wally_psbt_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_psbt_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
    struct wally_unknowns_map *unknowns;
    uint32_t version;
};

struct wally_psbt_input {
    struct wally_tx *non_witness_utxo;
    struct wally_tx_output *witness_utxo;
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    unsigned char *final_script_sig;
    size_t final_script_sig_len;
    struct wally_tx_witness_stack *final_witness;
    struct wally_keypath_map *keypaths;
    struct wally_partial_sigs_map *partial_sigs;
    struct wally_unknowns_map *unknowns;
    uint32_t sighash_type;
};

struct wally_psbt_output {
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    struct wally_keypath_map *keypaths;
    struct wally_unknowns_map *unknowns;
};
```
Estes, por sua vez, usam algumas estruturas de transação definidas em `/usr/include/wally_transaction.h`:
```
struct wally_tx {
    uint32_t version;
    uint32_t locktime;
    struct wally_tx_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_tx_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
};

struct wally_tx_output {
    uint64_t satoshi;
    unsigned char *script;
    size_t script_len;
    uint8_t features;
};
```
Tem muita coisa aí! Embora muito disso deva ser familiar dos capítulos anteriores, é um pouco intimidador ver tudo disposto em estruturas C.

## Lendo um PSBT Convertido

Obviamente, podemos ler qualquer coisa de uma estrutura PSBT chamando os elementos individuais das várias subestruturas. A seguir, veja uma breve visão geral que mostra como captar alguns dos elementos.

Aqui está um exemplo de recuperação dos valores e os `scriptPubKeys` das entradas:
```
  int inputs = psbt->num_inputs;
  printf("TOTAL INPUTS: %i\n",inputs);

  for (int i = 0 ; i < inputs ; i++) {
    printf("\nINPUT #%i: %i satoshis\n",i, psbt->inputs[i].witness_utxo->satoshi);

    char *script_hex;
    wally_hex_from_bytes(psbt->inputs[i].witness_utxo->script,psbt->inputs[i].witness_utxo->script_len,&script_hex);
    printf("scriptPubKey: %s\n",script_hex);
    wally_free_string(script_hex);
    
  }
```
Este padrão de programação será usado em muitas partes do PSBT. Podemos olhar para o tamanho do array de entrada e depois o percorrer, recuperando o que desejamos observar (neste caso, satoshis e scripts).

Aqui está um exemplo semelhante para as saídas:
```
  int outputs = psbt->num_outputs;
  printf("\nTOTAL OUTPUTS: %i\n",outputs);
  for (int i = 0 ; i < outputs ; i++) {

    char *pubkey_hex;
    wally_hex_from_bytes(psbt->tx->outputs[i].script,psbt->tx->outputs[i].script_len,&pubkey_hex);
    printf("\nINPUT #%i\n",i);
    printf("scriptPubKey: %s\n",pubkey_hex);
    wally_free_string(pubkey_hex);    
  }
```
Obviamente, há muito mais coisas que poderemos observar nos PSBTs. Na verdade, olhar é o ponto principal de um PSBT: podemos verificar entradas e saídas de um computador offline.

> :warning: **AVISO:** Estas funções de leitura são _muito_ rudimentares e não funcionarão corretamente para situações extremamente normais como uma entrada ou saída que ainda está vazia ou que inclui um `non_witness_utxo`. Elas darão um segfault se não forem entregues um PSBT precisamente esperado. Um leitor de verdade precisaria ser consideravelmente mais robusto, para cobrir todas as situações possíveis, mas vamos deixar isso como um exercício para o leitor.

### Testando Nosso Leitor PSBT
 
Novamente, o código para este leitor PSBT (extremamente rudimentar e específico) está no [diretório src/](../src/17_4_examinepsbt.c).

Podemos compilá-lo normalmente:
```
$ cc examinepsbt.c -lwallycore -o examinepsbt
```
O seguinte PSBT da seção [§7.3](07_3_Integrating_with_Hardware_Wallets.md) pode ser usado para o teste, pois atende aos critérios muito restritos exigidos por esta implementação limitada:
```
psbt=cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==
```
Ao executar o comando `examinepsbt` com esse PSBT, deveremos ver os scripts nas entradas e saídas:
```
$ ./examinepsbt $psbt
TOTAL INPUTS: 2

INPUT #0: 1000000 satoshis
scriptPubKey: 001400193c8bf25ef056f8d4571a1c3f65123e5fe788

INPUT #1: 1000000 satoshis
scriptPubKey: 00142d4f139faa885304d15616a166d2d51574af4a5d

TOTAL OUTPUTS: 2

INPUT #0
scriptPubKey: 0014c772d6f95542e11ef11e8efc7c7a69830ad38a05

INPUT #1
scriptPubKey: 0014f4e8dde5db370898b57c84566e3f76098850817d
```
E, claro, você pode verificar tudo isso com o comando RPC `decodepsbt` para` bitcoin-cli`:
```
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "hash": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01500000,
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
      },
      {
        "value": 0.00499791,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 f4e8dde5db370898b57c84566e3f76098850817d",
          "hex": "0014f4e8dde5db370898b57c84566e3f76098850817d",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q7n5dmewmxuyf3dtus3txu0mkpxy9pqtacuprak"
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
          "asm": "0 00193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "hex": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "type": "witness_v0_keyhash",
          "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/569"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2d4f139faa885304d15616a166d2d51574af4a5d",
          "hex": "00142d4f139faa885304d15616a166d2d51574af4a5d",
          "type": "witness_v0_keyhash",
          "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/0"
        }
      ]
    }
  ],
  "outputs": [
    {
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "03d942b59eea527d70bcd67981eb95d9fa9625269fd6eb0364e452ede59092c7a9",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/1/1"
        }
      ]
    }
  ],
  "fee": 0.00000209
}
```
Podemos ver a entrada de satoshis e o `scriptPubKey` claramente listadOs nos `inputs` e a nova `scriptPubKey` no `vout` do `tx`.

Então, está tudo lá para utilizarmos!

## Criando um PSBT

Conforme observado no início desta seção, todas as funções necessárias para criar e processar os PSBTs estão disponíveis na Libwally. Na verdade, percorrer todo o processo para fazer isso é tão complexo que foge do escopo desta seção, mas aqui está um resumo rápido das funções necessárias. Observe que a [documentação](https://wally.readthedocs.io/en/latest/psbt/) está desatualizada para os PSBTs, portanto, precisaremos consultar o `/usr/include/wally_psbt.h` para obter as informações completas.

Conforme discutido na seção [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md), existem várias funções envolvidas na criação de PSBTs.

### Assumindo o Papel de Criador

A função de criador tem a tarefa de criar um PSBT com pelo menos uma entrada.

Um PSBT é criado com um simples uso do comando `wally_psbt_init_alloc`, informando quantas entradas e saídas eventualmente adicionaremos:
```
  struct wally_psbt *psbt;
  lw_response = wally_psbt_init_alloc(0,1,1,0,&psbt);
```
Mas o que temos ainda não é um PSBT legal, por falta de entradas. Podemos criá-las criando uma transação e definindo-a como a transação global no PSBT, que atualiza todas as entradas e saídas:
```
  struct wally_tx *gtx;
  lw_response = wally_tx_init_alloc(0,0,1,1,&gtx);
  lw_response = wally_psbt_set_global_tx(psbt,gtx);
```
### Testando nosso PSBT Criado

Neste ponto, devemos ter um PSBT vazio, mas funcionando, que pode ser visto compilando e executando [o programa](../src/17_4_createemptypsbt.c).
```
$ cc createemptypsbt.c -lwallycore -o createemptypsbt
$ ./createemptypsbt 
cHNidP8BAAoAAAAAAAAAAAAAAA==
```
Podemos até usar o `bitcoin-cli` para testar o resultado:
```
$ psbt=$(./createpsbt)
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "hash": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "version": 0,
    "size": 10,
    "vsize": 10,
    "weight": 40,
    "locktime": 0,
    "vin": [
    ],
    "vout": [
    ]
  },
  "unknown": {
  },
  "inputs": [
  ],
  "outputs": [
  ],
  "fee": 0.00000000
}
```
## Assumindo as Demais Funções

Assim como na leitura de PSBT, estamos introduzindo o conceito de criação do PSBT e, em seguida, deixando o resto como um exercício para o leitor.

A seguir está uma lista aproximada de funções para cada papel. Mais funções serão necessárias para criar alguns dos elementos que são adicionados aos PSBTs.

**Criador:**

* wally_psbt_init_alloc
* wally_psbt_set_global_tx

**Atualizador:**

* wally_psbt_input_set_non_witness_utxo
* wally_psbt_input_set_witness_utxo
* wally_psbt_input_set_redeem_script
* wally_psbt_input_set_witness_script
* wally_psbt_input_set_keypaths
* wally_psbt_input_set_unknowns
* wally_psbt_output_set_redeem_script
* wally_psbt_output_set_witness_script
* wally_psbt_output_set_keypaths
* wally_psbt_output_set_unknowns

**Assinante:**

* wally_psbt_input_set_partial_sigs
* wally_psbt_input_set_sighash_type
* wally_psbt_sign

**Combinador:**

* wally_psbt_combine

**Finalizador:**
* wally_psbt_finalize
* wally_psbt_input_set_final_script_sig
* wally_psbt_input_set_final_witness

**Extrator:**

* wally_psbt_extract

## Resumo: Usando PSBTs na Libwally

Esta seção poderia ser um capítulo inteiro, já que trabalhar com os PSBTs em um nível baixo é um trabalho muito intenso que requer uma manipulação muito mais intensiva de entradas e saídas como foi o caso do [Capítulo 7](07_0_Expanding_Bitcoin_Transactions_PSBTs.md). Ao invés disso, esta seção mostra o básico: como extrair informações de um PSBT e como começar a criar um.

> :fire: ***Qual é o poder dos PSBTs na Libwally?*** Obviamente, já podemos fazer tudo isso nos `bitcoin-cli`, e é mais simples porque o Bitcoin Core gerencia muito do trabalho penoso. A vantagem de usar a Libwally é que ela pode ser executada offline, então pode ser a Libwally que está do outro lado de um dispositivo de hardware com o qual nosso `bitcoin-cli` está se comunicando com o HWI. Este é, de fato, um dos principais pontos dos PSBTs: ser capaz de manipular transações parcialmente assinadas sem a necessidade de um node completo. A Libwally permite isso.

## O Que Vem Depois?

Vamos aprender mais sobre "Programando Bitcoin com Libwally" na seção [§17.5: Usando Scripts na Libwally](17_5_Using_Scripts_in_Libwally.md).