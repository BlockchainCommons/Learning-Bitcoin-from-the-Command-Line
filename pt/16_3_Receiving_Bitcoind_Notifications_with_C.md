# 16.3 Recebendo Notificações em C com Bibliotecas ZMQ

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho que pode estar aguardando revisão. Portanto, leitor, tenha cuidado.

As sessões [§16.1](16_1_Accessing_Bitcoind_with_C.md) e [§16.2](16_2_Programming_Bitcoind_with_C.md) introduziram as bibliotecas RPC e JSON no C e, também mostrou uma das vantagens de acessar os comandos RPC do Bitcoin por meio de uma linguagem de programação: A capacidade de criar programas razoavelmente  complexos. Este capítulo apresenta uma terceira biblioteca, a [ZMQ](http://zeromq.org/) e, ao fazer isso, revela outra vantagem: a capacidade de monitorar as notificações. Iremos usá-la para codificar para ouvirmos a blockchain.

> :book: ***O que é ZMQ?*** O ZeroMQ (ZMQ) é uma biblioteca de mensagens assíncronas de alto desempenho que fornece uma fila de mensagens. A biblioteca oferece suporte a padrões de mensagens comuns (pub/sub, request/reply, client/server e outros) em uma variedade de transportes (TCP, in-process, inter-process, multicast, WebSocket e mais), tornando mensagens entre processos tão simples quanto mensagens entre threads. Podemos encontrar mais detalhes sobre as notificações do ZMQ e outros tipos de mensagens [neste repositório](https://github.com/Actinium-project/ChainTools/blob/master/docs/chainlistener.md).

## Configurando o ZMQ

Antes de podemos ouvir a blockchain, precisaremos configurar o ``bitcoind`` para permitir as notificações ZMQ. Por isso, precisamos instalar a biblioteca ZMQ para tirar proveito dessas notificações.

### Configurando o ``bitcoind`` para usar o ZMQ

O Bitcoin Core está pronto para ZMQ, mas devemos especificar os endpoints do ZMQ. O ZeroMQ publish-sockets prefixa cada item de dados com um prefixo de tópico arbitrário que permite aos clientes assinantes solicitarem apenas os itens com um prefixo correspondente. Existem atualmente quatro tópicos suportados pelo ``bitcoind``:
```
$ bitcoind --help | grep zmq | grep address
  -zmqpubhashblock=<address>
  -zmqpubhashtx=<address>
  -zmqpubrawblock=<address>
  -zmqpubrawtx=<address>
```

Podemos executar o ``bitcoind`` com argumentos de linha de comando para endpoints ZMQ, como mostrado acima, mas também podemos criar um endpoint acessível adicionando linhas apropriadas ao nosso arquivo ``~/.bitcoin/bitcoin.conf`` e reiniciando o sistema.

```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```
Podemos então testar se os nossos endpoints estão funcionando usando o RPC ``getzmqnotifications``:

```
$ bitcoin-cli getzmqnotifications
[
  {
    "type": "pubrawblock",
    "address": "tcp://127.0.0.1:28332",
    "hwm": 1000
  },
  {
    "type": "pubrawtx",
    "address": "tcp://127.0.0.1:28333",
    "hwm": 1000
  }
]
```
Nosso ``bitcoind`` agora irá emitir as notificações ZMQ

### Instalando o ZMQ

Para aproveitar essas notificações, precisamos de uma biblioteca ZMQ para usar com o C, portanto, estaremos usando uma nova biblioteca ZMQ ao invés da biblioteca ``libbitcoinrpc`` nesta seção, mas no futuro, podemos combinar ambas.

Felizmente, as bibliotecas ZMQ estão disponíveis por meio de pacotes Debian padrão:
```
$ sudo apt-get install libzmq3-dev
$ sudo apt-get install libczmq-dev
```
Agora estamos pronto para escrever o código!

## Escrevendo o Programa de Notificação

O programa C à seguir é um cliente simples que se inscreve em um ponto da conexão ZMQ servido pelo ``bitcoind`` e lê as mensagens recebidas.

O programa requer dois parâmetros: O primeiro é o "servidor", que é o ponto de conexão TCP exposto pelo ``bitcoind``. O segundo é o "tópico", que atualmente é o ``zmqpubhashblock``,``zmqpubhashtx``, ``zmqpubrawblock`` ou ``zmqpubrawtx``. O tópico precisa ter suporte do ``bitcoin.conf`` e o endereço IP e a porta do servidor devem corresponder ao que está definido no arquivo de configuração.

``` c
#include <czmq.h>
int main(int argc, char ** argv) {

  char *zmqserver;
  char *topic;

  if(argc < 3) {
    printf("\nUSAGE:\nchainlistener <tcp://localhost:port> <topic>\n\n");
    return 0;
  } else {
    zmqserver = argv[1];
    topic = argv[2];
  }
```
Abriremos um soquete ZMQ para o servidor e para o tópico definido:
``` c
  zsock_t *socket = zsock_new_sub(zmqserver, topic);
  assert(socket);
```
Depois, vamos esperar:
``` c
  while(1) {
    zmsg_t *msg;
    int rc = zsock_recv(socket, "m", &msg);
    assert(rc == 0);

    char *header = zmsg_popstr(msg);
    zframe_t *zdata = zmsg_pop(msg);
    unsigned int *no = (unsigned int*)zmsg_popstr(msg);

    char *data = zframe_strhex(zdata);
    int len = zframe_size(zdata);
    printf("Size: %d\n", len);
    printf("Data: %s", data);
    printf("\nNo: %d\n", *no);

    free(header);
    free(data);
    free(no);
    free(zdata);
    zmsg_destroy(&msg);
    sleep(1);
 }
```
Enquanto esperamos, observamos as mensagens no soquete ZMQ. Sempre que recebermos uma mensagem, iremos retirá-la da pilha, relatando o número, comprimento e, o os dados que foram importante para nós.

É isso!

Claro, quando terminar o processo, precisamos limpar tudo:
``` c
  zsock_destroy(&socket);
  return 0;
}
```

### Testando o Código de Notificação

O código-fonte completo está no [diretório src/](../src/16_3_chainlistener.c) como de costume. Precisamos compilá-lo:
```
$ cc -o chainlistener chainlistener.c -I/usr/local/include -L/usr/local/lib -lzmq -lczmq
```
Depois, podemos executá-lo com os tópicos e endereços que definimos no nosso ``bitcoin.conf``:
```
$ ./chainlistener tcp://127.0.0.1:28333 rawtx
Size: 250
Data: 02000000000101F5BD2032E5A9E6650D4E411AD272E391F26AFC3C9102B7C0C7444F8F74AE86010000000017160014AE9D51ADEEE8F46ED2017F41CD631D210F2ED9C5FEFFFFFF0203A732000000000017A9147231060F1CDF34B522E9DB650F44EDC6C0714E4C8710270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC02483045022100AE316D5F21657E3525271DE39EB285D8A0E89A20AB6413824E88CE47DCD0EFE702202F61E10C2A8F4A7125D5EB63AEF883D8E3584A0ECED0D349283AABB6CA5E066D0121035A77FE575A9005E3D3FF0682E189E753E82FA8BFF0A20F8C45F06DC6EBE3421079111B00
No: 67
Size: 249
Data: 0200000000010165C986992F7DAD22BBCE3FCF0BF546EDBC3C599618B04CFA22D9E64EF0CE4C030000000017160014B58E0A5CD68B249F1C407E9AAE9CD0332AAA3067FEFFFFFF02637932000000000017A914CCC47261489036CB6B9AA610857793FF5752E5378710270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC0247304402206CCC3F3B4BE01D4E532A01C2DC6BC3B53E4FFB6B494C8B87DD603EFC648A159902201653841E8B16A814DC375129189BB7CF01CFF7D269E91178645B6A97F5C7F4F10121030E20F3D2F172281B8DC747F007DF24B352248AC09E48CA64016942A8F01D317079111B00
No: 68
Size: 250
Data: 02000000000101E889CFC1FFE127BA49F6C1011388606A194109AE1EDAAB9BEE215E123C14A7920000000017160014577B0B3C2BF91B33B5BD70AE9E8BD8144F4B87E7FEFFFFFF02C34B32000000000017A914A9F1440402B46235822639C4FD2F78A31E8D269E8710270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC02483045022100B46318F53E1DCE63E7109DB4FA54AF40AADFC2FEB0E08263756BC3B7A6A744CB02200851982AF87DBABDC3DFC3362016ECE96AECFF50E24D9DCF264AE8966A5646FE0121039C90FCB46AEA1530E5667F8FF15CB36169D2AD81247472F236E3A3022F39917079111B00
No: 69
Size: 250
Data: 0200000000010137527957C9AD6CFF0C9A74597E6EFCD7E1EBD53E942AB2FA34A831046CA11488000000001716001429BFF05B3CD79E9CCEFDB5AE82139F72EB3E9DB0FEFFFFFF0210270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC231E32000000000017A9146C8D5FE29BFDDABCED0D6F4D8E82DCBFD9D34A8B8702483045022100F259846BAE29EB2C7A4AD711A3BC6109DE69AE91E35B14CA2742157894DD9760022021464E09C00ABA486AEAA0C49FEE12D2850DC03F57F04A1A9E2CC4D0F4F1459C012102899F24A9D60132F4DD1A5BA6DCD1E4E4B6C728927BA482C2C4E511679F60CA5779111B00
No: 70
.......
```

### Resumo: Recebendo Notificações em C com Bibliotecas ZMQ

Ao usar a estrutura ZMQ, podemos receber notificações facilmente, inscrevendo-as em um ponto de conexão exposto pelo ``bitcoind`` através do nosso arquivo de configuração.

> :fire: ***Qual é o poder das notificações?*** Com as notificações, não dependemos mais dos usuários para emitir os comandos. Ou seja, podemos criar programas que monitoram o blockchain do Bitcoin e tomar as ações apropriadas quando certas coisas ocorrem. Isso, por sua vez, pode ser utilizado juntamente com os comandos RPC que programamos nas seções anteriores. Este também é um grande passo além do que poderíamos fazer com os scripts shell: Certamente, podemos criar scripts shell que fica ouvindo infinitamente, mas existem outras linguagens de programação que possuem ferramentas melhores para isso.

## O Que Vem Depois?

Saiba mais sobre "Programando com RPC" no [Capítulo 17: Programando o Bitcoin com Libwally](17_0_Programming_with_Libwally.md).