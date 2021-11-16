# 16.3 Recibir notificaciones en C con bibliotecas ZMQ

> :information_source: **NOTA:** Esta sección se agregó recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

[§16.1](16_1_Accediendo_a_Bitcoind_en_C_con_las_Bibliotecas_RPC.md) y [§16.2](16_2_Programando_Bitcoind_en_C_con_las_Bibliotecas_RPC.md) introdujeron las bibliotecas RPC y JSON para C, y al hacerlo mostró una de las ventajas de acceder a los comandos RPC de Bitcoin a través de un lenguaje de programación: la capacidad de crear programas mucho más complejos. Este capítulo presenta una tercera biblioteca, para [ZMQ](http://zeromq.org/), y al hacerlo revela otra ventaja: la capacidad de monitorear las notificaciones. Lo usará para codificar un oyente de blockchain.

> :book: ***¿Qué es ZMQ?*** ZeroMQ (ZMQ) es una biblioteca de mensajería asincrónica de alto rendimiento que proporciona una cola de mensajes. ZeroMQ admite patrones de mensajería comunes (publicación / sub, solicitud / respuesta, cliente / servidor y otros) a través de una variedad de transportes (TCP, en proceso, entre procesos, multidifusión, WebSocket y más), lo que hace que la mensajería entre procesos tan simple como la mensajería entre subprocesos. Puede encontrar más detalles sobre las notificaciones de ZMQ y otros tipos de mensajes en [este repositorio](https://github.com/Actinium-project/ChainTools/blob/master/docs/chainlistener.md).

## Configurar ZMQ

Antes de que pueda crear un oyente de blockchain, deberá configurar `bitcoind` para permitir notificaciones ZMQ, y luego deberá instalar una biblioteca ZMQ para aprovechar esas notificaciones.

### Configurar `bitcoind` para ZMQ

Bitcoin Core está listo para ZMQ, pero debe especificar puntos finales ZMQ. Los conectores de publicación de ZeroMQ anteponen cada elemento de datos con un tema arbitrario
prefijo que permite a los clientes suscriptores solicitar solo aquellos elementos con un prefijo coincidente. Actualmente hay cuatro temas soportados por `bitcoind`:

```
$ bitcoind --help | grep zmq | grep address
  -zmqpubhashblock=<address>
  -zmqpubhashtx=<address>
  -zmqpubrawblock=<address>
  -zmqpubrawtx=<address>
```

Puede ejecutar `bitcoind` con argumentos de línea de comandos para puntos finales ZMQ, como se muestra arriba, pero también puede hacer que un punto final sea accesible agregando líneas apropiadas a su archivo `~/.bitcoin/bitcoin.conf` y reiniciando su demonio.

```
zmqpubrawblock = tcp: //127.0.0.1: 28332
zmqpubrawtx = tcp: //127.0.0.1: 28333
```

Luego puede probar que sus puntos finales están funcionando usando el RPC `getzmqnotifications`:

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

Su `bitcoind` ahora emitirá notificaciones ZMQ

### Instalar ZMQ

Para aprovechar esas notificaciones, necesita una biblioteca ZMQ que vaya con C; Por lo tanto, usaremos una nueva biblioteca ZMQ en lugar de la biblioteca `libbitcoinrpc` en esta sección, pero cuando experimente en el futuro, por supuesto, podrá combinarlas.

Afortunadamente, las bibliotecas ZMQ están disponibles a través de paquetes estándar de Debian:

```
$ sudo apt-get install libzmq3-dev
$ sudo apt-get install libczmq-dev
```

Ahora esta listo para codificar!

## Escriba su programa de notificaciones

El siguiente programa en C es un cliente simple que se suscribe a un punto de conexión ZMQ servido por `bitcoind` y lee los mensajes entrantes.

El programa requiere dos parámetros: el primer parámetro es el "servidor", que es el punto de conexión TCP expuesto por `bitcoind`; y el segundo es el "tema", que actualmente es `zmqpubhashblock`,` zmqpubhashtx`, `zmqpubrawblock` o `zmqpubrawtx`. El tema debe estar respaldado a través de `bitcoin.conf` y la dirección IP y el puerto del servidor deben coincidir con lo que se define allí.

``` c
#include <czmq.h>
int main(int argc, char ** argv) {

  char *zmqserver;
  char *topic;

  if (argc < 3) {
    printf("\nUSAGE:\nchainlistener <tcp://localhost:port> <topic>\n\n");
    return 0;
  } else {
    zmqserver = argv[1];
    topic = argv[2];
  }
```

Abrirá un socket ZMQ en el servidor definido para el tema definido:

``` c
  zsock_t *socket = zsock_new_sub(zmqserver, topic);
  assert(socket);
```

Después de eso, espera:

```
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

Mientras espera, observa los mensajes en el zócalo ZMQ. Siempre que reciba un mensaje, lo "sacará" de la pila e informará su número, su longitud y, lo más importante, los datos.

¡Eso es!

Por supuesto, cuando termine, debe limpiar:

```
  zsock_destroy(&socket);
  return 0;
}
```

### Pruebe el código de notificación

El código fuente está en el [directorio src](../src/16_3_chainlistener.c) como de costumbre. Debería compilarlo:

```
$ cc -o chainlistener chainlistener.c -I/usr/local/include -L/usr/local/lib -lzmq -lczmq
```

Luego, puede ejecutarlo con los temas y direcciones que definió en su `bitcoin.conf`:

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

### Resumen de recepción de notificaciones de Bitcoind con C.md

Al usar el marco ZMQ, puede recibir notificaciones fácilmente suscribiéndose a un punto de conexión expuesto por `bitcoind` a través de su archivo de configuración.

> :fire:***¿Cuál es el poder de las notificaciones?*** Con las notificaciones, ya no depende completamente de que los usuarios emitan comandos. En su lugar, puede crear programas que monitoreen la cadena de bloques de Bitcoin y tomar las acciones apropiadas cuando ocurran ciertas cosas. Esto, a su vez, podría combinarse con los comandos RPC que programó en las secciones anteriores. Este también es un gran paso más allá de lo que podría hacer con los scripts de shell: ciertamente, puede crear scripts de shell de escucha de bucle infinito, pero los lenguajes de programación tienden a ser una mejor herramienta para esa tarea.


## ¿Que sigue?

Obtenga más información sobre "Programación con RPC" en [Capítulo 17: Programación de Bitcoin con Libwally](17_0_Programando_Bitcoin_con_Libwally.md).
