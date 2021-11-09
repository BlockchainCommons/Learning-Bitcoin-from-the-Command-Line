# 16.1: Acessando o Bitcoind em C com Bibliotecas RPC

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um esboço que ainda pode estar aguardando revisão. Portanto, leitor, tenha cuidado.

Você já viu uma maneira alternativa de acessar as portas RPC do Bitcoind: Usando o ``curl``, que cobrimos no [Capítulo 4 Prefácio](04_4__interlude_using_curl.md). Interagir com o ``Bitcoind`` através de uma biblioteca de RPC usando C não é diferente do que já vimos, só precisamos de boas bibliotecas para nos auxiliar. Esta seção introduz um pacote chamado ``libbitcoinrpc``, que permite acessar a porta JSON-RPC do ``bitcoind``. Ele usa uma biblioteca ``curl`` para acessar os dados e usa a biblioteca ``jansson`` para codificar e decodificar o JSON.

## Configurando o libbitcoinrpc

Para usar o ``libbitcoinrpc``,  precisaremos instalar uma configuração básica C e os pacotes dependentes, que são ``libcurl``, ``libjansson``, e ``libuuid``. Depois faremos isso no seu servidor Standup Bitcoin (ou em qualquer outro servidor Ubuntu).

```
$ sudo apt-get install make gcc libcurl4-openssl-dev libjansson-dev uuid-dev
Suggested packages:
  libcurl4-doc libidn11-dev libkrb5-dev libldap2-dev librtmp-dev libssh2-1-dev
The following NEW packages will be installed:
  libcurl4-openssl-dev libjansson-dev uuid-dev
0 upgraded, 3 newly installed, 0 to remove and 4 not upgraded.
Need to get 358 kB of archives.
After this operation, 1.696 kB of additional disk space will be used.
Do you want to continue? [Y/n] y
```
Agora, podemos baixar o [libbitcoinrpc no github](https://github.com/gitmarek/libbitcoinrpc/blob/master/readme.md). Vamos clonar ou pegar um arquivo zip, do jeito que preferir.

```
$ sudo apt-get install git
$ git clone https://github.com/gitmarek/libbitcoinrpc
```

> :warning: **ATENÇÃO** Uma alteração no RPC "signrawtransaction" causou uma assinatura com ``libbitcoinrpc`` para o segfault no Bitcoin 0.17 ou superior. O [Pull Request foi submetido](https://github.com/gitmarek/libbitcoinrpc/pull/1/commits) para resolver o problema, mas se ainda não tiver sido feito o merge, podemos simplesmente fazer uma simples mudança no código-fonte para ``src/bitcoinrpc_method.c`` antes de compilarmos.

### Compilando o libbitcoinrpc

Antes de compilarmos e instalarmos o pacote, provavelmente precisaremos ajustar nosso  ``$PATH``, para que  possamos acessar o ``/sbin/ldconfig``:
```
$ PATH="/sbin:$PATH"
```

Para o Ubuntu, também precisaremos ajustar o ``install_libpath`` no ``makefile`` do ``libbitcoinrpc`` para instalar no ``/usr/lib`` ao invés do ``/usr/local/lib``:

```
$ emacs ~/libbitcoinrpc/Makefile 
...
INSTALL_LIBPATH    := $(INSTALL_PREFIX)/usr/lib
```

(Se preferir não usar o ``/usr/lib``, precisará alterar o ``etc/ld.so.conf`` ou os arquivos dependentes de maneira apropriada... Porém, para uma configuração de teste em uma máquina de teste, acredito que isso não seja um problema).

Da mesma forma, vamos precisar ajustar o ``install_headerpath`` no ``Makefile`` do ``libbitcoinrpc`` para instalar no caminho ``/usr/include`` ao invés do ``/usr/local/inclusve``:

```
...
INSTALL_HEADERPATH    := $(INSTALL_PREFIX)/usr/include
```

Agora, podemos compilar:
```
$ cd libbitcoinrpc
~/libbitcoinrpc$ make

gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_err.o -c src/bitcoinrpc_err.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_global.o -c src/bitcoinrpc_global.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc.o -c src/bitcoinrpc.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_resp.o -c src/bitcoinrpc_resp.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_cl.o -c src/bitcoinrpc_cl.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_method.o -c src/bitcoinrpc_method.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -shared -Wl,-soname,libbitcoinrpc.so.0 \
src/bitcoinrpc_err.o src/bitcoinrpc_global.o src/bitcoinrpc.o src/bitcoinrpc_resp.o src/bitcoinrpc_cl.o src/bitcoinrpc_method.o \
-o .lib/libbitcoinrpc.so.0.2 \
-Wl,--copy-dt-needed-entries -luuid -ljansson -lcurl
ldconfig -v -n .lib
.lib:
  libbitcoinrpc.so.0 -> libbitcoinrpc.so.0.2 (changed)
ln -fs libbitcoinrpc.so.0 .lib/libbitcoinrpc.so
```
Se tudo correr bem, podemos instalar o pacote:
```
$ sudo make install
Installing to 
install .lib/libbitcoinrpc.so.0.2 /usr/local/lib
ldconfig  -n /usr/local/lib
ln -fs libbitcoinrpc.so.0 /usr/local/lib/libbitcoinrpc.so
install -m 644 src/bitcoinrpc.h /usr/local/include
Installing docs to /usr/share/doc/bitcoinrpc
mkdir -p /usr/share/doc/bitcoinrpc
install -m 644 doc/*.md /usr/share/doc/bitcoinrpc
install -m 644 CREDITS /usr/share/doc/bitcoinrpc
install -m 644 LICENSE /usr/share/doc/bitcoinrpc
install -m 644 Changelog.md /usr/share/doc/bitcoinrpc
Installing man pages
install -m 644 doc/man3/bitcoinrpc*.gz /usr/local/man/man3
```

## Preparando o Código

``libbitcoinrpc`` tem métodos simples e bem estruturados para conectar-se ao nosso `bitcoind`, executando chamadas RPC e decodificando a resposta.

Para usar o ``libbitcoinrpc``, é importante certificar de que nossos arquivos do código incluam os cabeçalhos apropriados:
``` c
#include <jansson.h>
#include <bitcoinrpc.h>
```

Precisaremos também vincular as bibliotecas apropriadas sempre que possamos compilar:

```
$ cc yourcode.c -lbitcoinrpc -ljansson -o yourcode
```

## Construindo a Conexão

Para construir a conexão com o servidor ``bitcoind`` é necessário alguns simples passos.

Primeiro, inicialize a biblioteca:
```
bitcoinrpc_global_init();
```
Em seguida, vamos conectar ao  ``Bitcoind`` com ``bitcoinrpc_cl_init_params``. Os quatro argumentos necessários para o ``bitcoinrpc_cl_init_params`` são o nome de usuário, a senha, o endereço IP e a porta. A esta altura, você deve saber todas essas informações, já que foram necessárias para realizar o trabalho com o [curl](04_4__interlude_using_curl.md). Apenas para recordar, o endereço de IP é 127.0.0.1 e a porta 18332 devem estar corretos para a configuração padrão da testenet descrita neste documento, enquanto podemos encontrar o usuário e a senha no arquivo ``~/.bitcoin/bitcoin.conf``.
```
$ cat bitcoin.conf 
server=1
dbcache=1536
par=1
maxuploadtarget=137
maxconnections=16
rpcuser=StandUp
rpcpassword=6305f1b2dbb3bc5a16cd0f4aac7e1eba
rpcallowip=127.0.0.1
debug=tor
prune=550
testnet=1
[test]
rpcbind=127.0.0.1
rpcport=18332
[main]
rpcbind=127.0.0.1
rpcport=8332
[regtest]
rpcbind=127.0.0.1
rpcport=18443
```
Com essas informações, vamos colocá-las no ``bitcoinrpc_cl_init_params``:
``` c
bitcoinrpc_cl_t *rpc_client;
rpc_client = bitcoinrpc_cl_init_params("StandUp", "6305f1b2dbb3bc5a16cd0f4aac7e1eba", "127.0.0.1", 18332);
```

> **MAINNET VS TESTNET:** A porta seria a 8332 caso estivéssemos usando a configuração da rede principal.

Se o ``rpc_client`` for inicializado com sucesso, poderemos enviar os comandos do RPC.

Mais tarde, quando tivermos feito com a conexão de ``bitcoind``, poderemos fechar da seguinte maneira:
``` c
bitcoinrpc_global_cleanup();
```

### Testando o Código de Teste

O código de teste pode ser encontrado [no diretório src com o nome 16_1_testbitcoin.c](../src/16_1_testbitcoin.c). Vamos fazer o download para a nossa máquina TestNet e depois inserir a senha correta do RPC (e alterar o usuário RPC se não tivermos criado o servidor com StandUp).

Podemos compilar e executar o código da seguinte maneira:
```
$ cc testbitcoin.c -lbitcoinrpc -ljansson -o testbitcoin
$ ./testbitcoin 
Successfully connected to server!
```

> :warning: **ATENÇÃO:** Se esquecermos de inserir a senha RPC nesta ou em qualquer outro código que possuem dependências do RPC, receberemos um misterioso ``ERROR CODE 5``.

## Fazendo uma Chamada ao RPC

Para usarmos um método RPC usando ``libbitcoinrpc``, devemos inicializar uma variável do tipo ``bitcoinrpc_method_t``. Podemos fazer com o valor apropriado para o método que desejamos utilizar, que estão todos listados na [Referências do BitcoinRPC](https://github.com/gitmarek/libbitcoinrpc/blob/master/doc/reference.md).
``` c
bitcoinrpc_method_t *getmininginfo  = NULL;
getmininginfo = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETMININGINFO);
```
Normalmente definiríamos os parâmetros em seguida, mas o ``GetMiningInfo`` não requer parâmetros, por isso podemos pular essa parte.

Também devemos criar outros dois objetos, um "objeto de resposta" e um "objeto de erro". Eles podem ser inicializados da seguinte forma:
``` c
bitcoinrpc_resp_t *btcresponse  = NULL;
btcresponse = bitcoinrpc_resp_init();

bitcoinrpc_err_t btcerror;
```
Vamos usar a variável ``rpc_client`` que aprendemos no teste anterior e vamos adicionar nosso método ``getmininginfo`` e os outros dois objetos:
``` c
bitcoinrpc_call(rpc_client, getmininginfo, btcresponse, &btcerror);
```

### Mostrando o Retorno da Chamada

Com certeza iremos querer saber o que a RPC retornou. Para fazermos isso, vamos recuperar a saída da nossa chamada como sendo um objeto JSON com ``bitcoinrpc_resp_get`` e vamos salvá-la em um objeto padrão ``jansson``, do tipo ``json_t``:
``` c
json_t *jsonresponse = NULL;
jsonresponse = bitcoinrpc_resp_get(btcresponse);
```
Se quisermos gerar os resultados completos da chamada RPC no JSON, podemos fazer com uma simples invocação do ``json_dumps``, da biblioteca ``jansson``:
``` c
printf("%s\n", json_dumps(j, JSON_INDENT(2)));
```
No entanto, como agora estamos escrevendo programas completos, provavelmente iremos querer fazer um trabalho mais sutil, como retirar valores individuais do JSON para algum uso específico. A [Referência do Jansson](https6//jansson.readthedocs.Io/en/2.10/apiref.html) traz detalhes de como fazer.

Assim como estávamos usando o [curl](04_4__interlude_using_curl.md), descobrimos que o RPC retorna um objeto JSON contendo um ``ID``, um ``error`` e, mais importante, um objeto JSON do tipo ``result``.

A função ``json_object_get`` permite recuperar um valor (como o ``result``) de um objeto JSON usando chaves:
``` c
json_t *jsonresult = NULL;
jsonresult = json_object_get(jsonresponse,"result");
printf("%s\n", json_dumps(jsonresult, JSON_INDENT(2)));
```

No entanto, provavelmente iremos querer analisar informações ainda mais profundas, para obter uma variável específica. Depois de recuperar o valor apropriado, precisaremos convertê-lo em um objeto C padrão usando a função ``JSON_*_value``. Por exemplo, para acessar um integer usamos o ``json_integer_value``:
``` c
json_t *jsonblocks = NULL;
jsonblocks = json_object_get(jsonresult,"blocks");

int blocks;
blocks = json_integer_value(jsonblocks);
printf("Block Count: %d\n",blocks);
```

> :warning: **ATENÇÃO:** É extremamente fácil ocasionar erros de segmentação no código C quando estivermos trabalhando com os objetos ``jansson`` caso fiquemos confusos com que tipo de objeto estamos recuperando.  Por isso, precisamos fazer isso com cuidado usando o ``bitcoin-cli help`` para saber o que devemos esperar, e se tivermos uma falha de segmentação, primeiro precisamos analisar se nossas funções de recuperação JSON estão corretas.

### Testando o Código de Informação

Vamos recuperar o código de teste que está no [diretório src](../src/16_1_getmininginfo.c).
```
$ cc getmininginfo.c -lbitcoinrpc -ljansson -o getmininginfo
$ ./getmininginfo 
Full Response: {
  "result": {
    "blocks": 1804406,
    "difficulty": 4194304,
    "networkhashps": 54842097951591.781,
    "pooledtx": 127,
    "chain": "test",
    "warnings": "Warning: unknown new rules activated (versionbit 28)"
  },
  "error": null,
  "id": "474ccddd-ef8c-4e3f-93f7-fde72fc08154"
}

Just the Result: {
  "blocks": 1804406,
  "difficulty": 4194304,
  "networkhashps": 54842097951591.781,
  "pooledtx": 127,
  "chain": "test",
  "warnings": "Warning: unknown new rules activated (versionbit 28)"
}

Block Count: 1804406
```

## Fazendo uma Chamada RPC Usando Argumentos

Mas e se a sua chamada RPC tiver argumentos?

### Criando uma Matriz JSON

Para enviar parâmetros para a nossa chamada RPC usando ``libbitcoinrpc`` teremos que envolvê-los em uma matriz json. Como uma matriz é apenas uma simples listagem de valores, tudo o que precisamos fazer é codificar os parâmetros como elementos ordenados na matriz.

Vamos criar a matriz JSON usando a função ``json_array do``  do ``jansson``:
``` c
json_t *params = NULL;
params = json_array();
```
Vamos fazer o processo inverso que fizemos para acessar valores do JSON: Vamos converter objetos no C para objetos no JSON usando as funções ``JSON_*``. Depois, vamos anexar tudo à matriz:
``` c
json_array_append_new(params,json_string(tx_rawhex));
```

Observe que existem duas variantes para o comando de anexação: ``json_array_apend_new``, que acrescenta uma variável recém-criada, e ``json_array_apend``, que anexa uma variável existente.

Esta metodologia simples ``json_array_apend_new`` servirá para a maioria dos comandos RPC com parâmetros, mas alguns dos comandos RPC exigem entradas mais complexas. Nesses casos, precisaremos criar objetos JSON ou arrays em JSON, que anexaremos ao parâmetros de array como de costume. A próxima seção contém um exemplo de como fazer isso usando o ``CrayAwTransaction``, que contém uma matriz JSON de objetos JSON para as entradas, um objeto JSON para as saídas e o parâmetro ``locktime``.

### Atribuindo os Parâmetros

Quando criamos o parâmetro array no JSON, simplesmente o atribuímos depois de inicializar o método RPC, da seguinte maneira:
``` c
bitcoinrpc_method_set_params(rpc_method, params)
```
Esta seção não inclui uma amostra abrangente dessa metodologia mais complexa, mas vamos vê-la em ação várias vezes no nosso primeiro programa C mais abrangente usando o RPC, na próxima seção.

## Resumo: Acessando o Bitcoind em C com Bibliotecas RPC

Ao vincular às bibliotecas ``BitcoinRPC`` do RPC e as bibliotecas ``jansson`` do JSON, podemos acessar facilmente o ``bitcoind`` usando chamadas RPC de uma biblioteca C. Para fazer isso, criamos uma conexão RPC, que faz as chamadas individuais de RPC, algumas delas passando alguns parâmetros. O ``jansson`` permite decodificar as respostas no formato JSON. A próxima seção demonstrará como isso pode ser usado para um programa de uso do mundo real.

* :fire: ***Qual é o poder do C?*** O C permite que façamos o próximo passo muito além do script shell, permitindo a criação de programas mais complexos e robustos.

## O Que Vem Depois?

Vamos falar mais um pouco no "Conversando com o Bitcoind com C" no capítulo [16.2: Programando o Bitcoind em C com Bibliotecas RPC](16_2_Programming_bitcoind_with_c.md).
