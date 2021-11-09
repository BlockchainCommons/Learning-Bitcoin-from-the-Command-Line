# 16.2: Programando o Bitcoind usando C com bibliotecas RPC

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho que pode estar aguardando revisão. Portanto, leitor, tenha cuidado.

A seção [§16.1](16_1_Accessing_Bitcoind_with_C.md) apresentou a metodologia para a criação de programas C usando bibliotecas RPC e JSON. Agora vamos mostrar o potencial dessas bibliotecas C fazendo algumas coisas um pouco mais avançadas usando o programa real do Bitcoin.

## Planejando o Código

Esta seção irá criar uma versão simples do ``sendtoaddress``, permitindo ao usuário enviar as moedas para um endereço, desde que tenha um UTXO grande o suficiente para isso. Aqui está o que precisamos fazer:

  1. Solicitar um endereço e uma quantia;
  2. Definir uma taxa arbitrária;
  3. Preparar nosso RPC;
  4. Encontrar um UTXO que seja grande o suficiente para pagar o valor + a taxa;
  5. Criar uma mudança de endereço;
  6. Criar uma transação bruta que envie o UTXO para o endereço e altere o endereço;
  7. Assinar a transação;
  8. Enviar a transação.
  
### Planejando para o Futuro

Como este é o nosso primeiro programa C funcional, vamos mantê-lo simples (ou seja, vamos usar a filosofia, _Keep it Simple_ ou também conhecida como KISS). Se estivéssemos produzindo um programa para estar em produção, desejaríamos pelo menos os seguintes passos:

   1. Testar e/ou higienizar as entradas;
   2. Calcular uma taxa automaticamente;
   3. Pensar logicamente sobre qual UTXO seria válido utilizar;
   4. Combinar vários UTXOs, caso seja necessário;
   5. Ficar atento a mais erros nos comandos ``libbitcoinrpc`` ou no ``jansson``;
   6. Observar se há erros nas respostas RPC.
   
Se deseja continuar a expandir este exemplo, seria ótimo começar a lidar com as inadequações do programa.

## Escrevendo o Sistema de Transação

Agora estamos prontos para realizar o passo a passo do nosso plano

### Etapa 1: Solicitando um Endereço e uma Quantia

Inserir as informações é bem simples se usarmos os argumentos na linha de comando:
``` c
if(argc != 3) {

  printf("ERROR: Only %i arguments! Correct usage is '%s [recipient] [amount]'\n",argc-1,argv[0]);
  exit(-1);

}

char *tx_recipient = argv[1];
float tx_amount = atof(argv[2]);

printf("Sending %4.8f BTC to %s\n",tx_amount,tx_recipient);
```

> :aviso: **ATENÇÃO:** Um programa real precisaria de uma higienização muito melhor dessas variáveis.

### Etapa 2: Definindo uma Taxa Arbitrária

Este exemplo colocamos uma taxa arbitrária de 0.0005 BTC para garantir que as transações do teste sejam processadas rapidamente:

``` c
float tx_fee = 0.0005;
float tx_total = tx_amount + tx_fee;
```

> :warning: **ATENÇÃO:** Um programa real calcularia uma taxa que minimizasse o custo, garantindo que a velocidade fosse aquela que o remetente estivesse disposto a utilizar.

### Etapa 3: Preparando Nosso RPC

Obviamente, precisaremos preparar todas as nossas variáveis novamente, conforme discutido na seção [§16.1: Acessando o Bitcoind com C](16_1_Accessing_Bitcoind_with_C.md). Também precisaremos inicializar a nossa biblioteca, conectar o cliente RPC e preparar nosso objeto de resposta:
``` c
bitcoinrpc_global_init();
rpc_client = bitcoinrpc_cl_init_params("bitcoinrpc", "YOUR-RPC-PASSWD", "127.0.0.1", 18332);
btcresponse = bitcoinrpc_resp_init();
```

### Etapa 4: Encontrando um UTXO

Para encontrar um UTXO, precisaremos chamar a função RPC ``listunspent``:
``` c
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_LISTUNSPENT);
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
```

No entanto, o verdadeiro trabalho consiste em decodificar a resposta. Na seção anterior vimos que a biblioteca ``jansson`` era "um tanto quanto desajeitada" e esta é a razão: Precisamos criar (e limpar) um conjunto muito grande de objetos ``json_t`` para descobrir o que queremos.

Primeiro, precisamos nos recuperar o campo ``result`` do JSON:
``` c
json_t *lu_response = NULL;
json_t *lu_result = NULL;

lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");
```

> :warning: **ATENÇÃO:** Só obteremos um resultado se não houver nenhum erro. Aqui temos um momento para melhorar nossa verificação de erros no código que iremos colocar em produção.

Em seguida, vamos fazer um laço, examinando cada transação que não foi gasta, que aparece como um elemento em sua matriz do resultado JSON:
``` c
int i;

const char *tx_id = 0;
int tx_vout = 0;
double tx_value = 0.0;

for(i = 0 ; i < json_array_size(lu_result) ; i++) {

  json_t *lu_data = NULL;
  lu_data = json_array_get(lu_result, i);
  
  json_t *lu_value = NULL;
  lu_value = json_object_get(lu_data,"amount");
  tx_value = json_real_value(lu_value);
```

O UTXO é grande o suficiente para pagar sua transação? Se sim, pegue-o!

> :warning: **ATENÇÃO:** Um programa em produção pensaria com mais cuidado sobre qual UTXO utilizar, com base no tamanho e em outros fatores. Provavelmente não pegaria apenas o primeiro mais simples e pronto.

``` c
  if(tx_value > tx_total) {

    json_t *lu_txid = NULL;
    lu_txid = json_object_get(lu_data,"txid");
    tx_id = strdup(json_string_value(lu_txid));

    json_t *lu_vout = NULL;
    lu_vout = json_object_get(lu_data,"vout");
    tx_vout = json_integer_value(lu_vout);

    json_decref(lu_value);
    json_decref(lu_txid);
    json_decref(lu_vout);
    json_decref(lu_data);
    break;

  } 
```
Você também deve limpar os principais elementos do JSON:
``` c
}

json_decref(lu_result);
json_decref(lu_response);
```      

> :warning: **ATENÇÃO:** Um programa em produção também se certificaria de que os UTXOs são passíveis de serem `gastos`.

Se não encontramos nenhum UTXOs grande o suficiente, teremos que relatar este infortúnio ao usuário... E talvez, sugerir que ele deva usar um programa melhor, que irá mesclar os UTXOs de maneira correta.
``` c
if(!tx_id) {

  printf("Very Sad: You don't have any UTXOs larger than %f\n",tx_total);
  exit(-1);
}
```

> **ATENÇÃO** Um programa em produção usaria sub-rotinas para este tipo de pesquisa, de forma que pudéssemos chamar vários RPCs de uma biblioteca de funções C. Vamos apenas colocar tudo em um `main` como parte da nossa filosofia KISS.

### Etapa 5: Criando um Endereço de Troco

Repita a metodologia padrão de pesquisa RPC para obter um endereço de troco:
``` c
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETRAWCHANGEADDRESS);

if(!rpc_method) {

  printf("ERROR: Unable to initialize listunspent method!\n");
  exit(-1);

}

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

if(btcerror.code != BITCOINRPCE_OK) {

printf("Error: listunspent error code %d [%s]\n", btcerror.code,btcerror.msg);

  exit(-1);

}

lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");
char *changeaddress = strdup(json_string_value(lu_result));
```
A única diferença é quais informações específicas são extraídas do objeto JSON.

> :warning: **ATENÇÃO:** Aqui temos uma sub-rotina que seria bem legal: Abstrair toda a inicialização e chamada do método RPC.

### Etapa 6: Criando uma Transação Bruta

Criar a transação bruta real é outra parte complicada da programação da substituição do ``sendtoaddress``. Isso porque requer a criação de um objeto JSON complexo como parâmetro.

Para criarmos esses parâmetros corretamente, precisaremos revisar o que o RPC ``createrawtransaction`` espera que passemos como argumento. Felizmente, isso é fácil de determinar usando a funcionalidade ``bitcoin-cli help``:
```
$ bitcoin-cli help createrawtransaction
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime )
```

Para relembrar, as entradas serão uma matriz JSON contendo um objeto JSON para cada UTXO. Então, as saídas estarão todas em um objeto JSON. É mais fácil criar esses elementos JSON de dentro para fora, usando os comandos ``jansson``.

#### Etapa 6.1: Criando os Parâmetros de Entrada

Para criar o objeto de entrada para nosso UTXO, vamos usar o ``json_object`` e preencher com os valores-chave usando ``json_object_set_new`` (para referências recém-criadas) ou ``json_object_set`` (para referências já existentes):
``` c
json_t *inputtxid = NULL;
inputtxid = json_object();

json_object_set_new(inputtxid,"txid",json_string(tx_id));
json_object_set_new(inputtxid,"vout",json_integer(tx_vout));
```

Pode-se notar que teremos que traduzir novamente cada tipo de variável C em um tipo de variável JSON usando a função apropriada, como ``json_string`` ou ``json_integer``.

Para criar o array de entrada para todos os UTXOs, vamos usar o ``json_array`` e, em seguida, preenchê-lo com os objetos usando o ``json_array_append``:
``` c
json_t *inputparams = NULL;
inputparams = json_array();
json_array_append(inputparams,inputtxid);
```

#### Etapa 6.2: Criando os Parâmetros de Saída

Para criar a matriz de saída para a transação, vamos seguir o mesmo processo, criando um objeto JSON com ``json_object`` e, em seguida, vamos preenchê-lo com o ``json_object_set``:
``` c
json_t *outputparams = NULL;
outputparams = json_object();

char tx_amount_string[32];
sprintf(tx_amount_string,"%.8f",tx_amount);
char tx_change_string[32];
sprintf(tx_change_string,"%.8f",tx_value - tx_total);

json_object_set(outputparams, tx_recipient, json_string(tx_amount_string));
json_object_set(outputparams, changeaddress, json_string(tx_change_string));
```

> :warning: **ATENÇÃO:** É possível pensar que teremos que inserir os valores do Bitcoin como sendo números, usando a função ``json_real``. Infelizmente, isso expõe um dos maiores problemas com a integração da biblioteca ``jansson`` e o Bitcoin. O Bitcoin só é válido até oito dígitos depois da casa decimal. Devemos nos lembrar que 0,00000001 BTC é um satoshi, e essa é a menor divisão possível de um Bitcoin. O tipo ``double`` no C oferecem mais dígitos do que precisamos, embora sejam frequentemente imprecisos depois das oito casas decimais. Se tentarmos convertê-los diretamente do nosso valor ``double`` no C (ou de um tipo ``float``, neste caso) para um valor Bitcoin, a imprecisão frequentemente criará um valor Bitcoin com mais de oito dígitos. Antes da versão Bitcoin Core 0.12, isso não era problema, e podíamos usar a função ``json_real``. Mas à partir dessa versão, se tentarmos usar a função ``createrawtransaction`` com mais do que oito dígitos, obteremos um erro e a transação não será criada. Como resultado, se o valor do Bitcoin _sempre_ se tornar um ``double`` ou ``float``, devemos deixar com apenas oito casas decimais antes de transformá-lo em uma ``string``. Obviamente, isso é um erro, portanto, certifique-se disso para que o código continue funcionando nas versões mais novas do Bitcoin Core.

#### Etapa 6.3: Criando a Matriz de Parâmetros

Para terminarmos de criar os parâmetros, só precisaremos agrupá-los em uma matriz JSON:
``` c
json_t *params = NULL;
params = json_array();
json_array_append(params,inputparams);
json_array_append(params,outputparams);
```

#### Etapa 6.4: Fazendo a Chamada ao RPC

Vamos usar o método normal para criar uma chamada ao RPC:
``` c
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_CREATERAWTRANSACTION);
```
Agora, porém, devemos adicionar os nossos parâmetros. Isso pode ser feito facilmente com a função ``bitcoinrpc_method_set_params``:
``` c
if(bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf(stderr, "Error: Could not set params for createrawtransaction");

}
```
Depois, é só executar o RPC e obter os resultados:
``` c
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_rawhex = strdup(json_string_value(lu_result));
```
### Etapa 7. Assinando a Transação

É muito mais fácil atribuir um parâmetro simples a uma função. Basta criar uma matriz JSON e, em seguida, atribuir o parâmetro à matriz:
``` c
params = json_array();
json_array_append_new(params,json_string(tx_rawhex));
```
Por fim, vamos assinar a transação seguindo o rigamarole típico para criar uma chamada RPC:
``` c
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SIGNRAWTRANSACTION);
if(bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf(stderr, "Error: Could not set params for signrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
```
Novamente, usar a função ``jansson`` para acessar a saída pode ser complicado. Devemos lembrar que ``hex`` é parte de um objeto JSON, não um resultado independente, como era quando criamos a transação bruta. Claro,  sempre podemos acessar essas informações a partir da ajuda da linha de comando: ``bitcoin-cli help signrawtransaction``
``` c
lu_result = json_object_get(lu_response,"result");
json_t *lu_signature = json_object_get(lu_result,"hex");
char *tx_signrawhex = strdup(json_string_value(lu_signature));
json_decref(lu_signature);
```
> :warning: ***ATENÇÃO:*** Um programa em produção obviamente iria testar cuidadosamente a resposta de cada comando RPC para se certificar de que não teria erros. Isso é ainda mais verdadeiro para  a função ``signrawtransaction``, porque podemos acabar com uma transação parcialmente assinada. Ou ainda pior, se não verificarmos os erros no objeto JSON, veremos apenas o ``hex`` e não iremos saber que ele não está assinado ou se está parcialmente assinado.

### Etapa 8. Enviando a Transação

Agora podemos enviar a transação, usando todas as técnicas aprendidas anteriormente:
``` c
params = json_array();
json_array_append_new(params,json_string(tx_signrawhex));

rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SENDRAWTRANSACTION);

if(bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf(stderr, "Error: Could not set params for sendrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_newid = strdup(json_string_value(lu_result));

printf("Txid: %s\n",tx_newid);
```

O código inteiro, com um _pouco_ mais verificação de erros, está disponível no Apêndice.

## Testando o Código

O código completo pode ser encontrado no [diretório src/](../src/16_2_sendtoaddress.c).

Compile-o como de costume:
```
$ cc sendtoaddress.c -lbitcoinrpc -ljansson -o sendtoaddress
```
Agora, é possível utilizá-lo para enviar fundos para um endereço:

```
./sendtoaddress tb1qynx7f8ulv4sxj3zw5gqpe56wxleh5dp9kts7ns .001
Txid: b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2
```
Você pode ver as informações sobre esta transação que enviamos clicando [aqui](https://mempool.space/pt/testnet/tx/b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2/).

## Resumo: Programando o Bitcoind em C com Bibliotecas RPC

Com acesso a uma biblioteca C, podemos criar programas com muito mais recursos quando comparados aos  scripts no shell. Mas isso pode dar muito trabalho! Mesmo com 316 linhas de código, o ``sendtoaddress.c`` não cobre todos os detalhes necessários para transacionar bitcoins de forma segura e inteligente.

## O Que Vem Depois?

Aprenda mais sobre "Programando o Bitcoind com C" na próxima seção [16.3: Recebendo notificações usando C com a biblioteca ZMQ](16_3_Receiving_Bitcoind_Notifications_with_C.md).