# 16.1: Acceso a Bitcoind en C con bibliotecas RPC

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con advertencias.

Ya ha visto una forma alternativa de acceder a los puertos RPC de Bitcoind: `curl` que se cubrió en el [Interludio del Capítulo 4](04_4_Interludio_Usando_Curl.md). Interactuar con `bitcoind` a través de una biblioteca RPC en C no es diferente a eso, solo necesita algunas buenas bibliotecas para ayudarlo. Esta sección presenta un paquete llamado `libbitcoinrpc`, que le permite acceder al puerto JSON-RPC `bitcoind`. Utiliza una biblioteca `curl` para acceder a los datos y usa la biblioteca `jansson` para codificar y decodificar el JSON.

## Configurar libbitcoinrpc

Para usar `libbitcoinrpc`, es necesario instalar una configuración básica de C y los paquetes dependientes `libcurl`, `libjansson` y `libuuid`. Lo siguiente lo hará en su servidor Bitcoin Standup (o cualquier otro servidor Ubuntu).
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
Puede descargar [libbitcoinrpc de Github](https://github.com/gitmarek/libbitcoinrpc/blob/master/README.md). Clónelo o tome un archivo zip, como prefiera.
```
$ sudo apt-get install git
$ git clone https://github.com/gitmarek/libbitcoinrpc
```

> :warning: **ADVERTENCIA** Un cambio en el RPC de "signrawtransaction" provocó que la firma con `libbitcoinrpc` provocara un segfault para Bitcoin 0.17 o superior. [Se ha enviado un PR](https://github.com/gitmarek/libbitcoinrpc/pull/1/commits) para resolver el problema, pero si aún no se ha fusionado, puede hacer un simple cambio en el código fuente `src/bitcoinrpc_method.c` antes de compilar.


### Compilar libbitcoinrpc

Antes de que pueda compilar e instalar el paquete, probablemente necesitará ajustar su `$PATH`, para que pueda acceder a `/sbin/ldconfig`:
```
$ PATH="/sbin:$PATH"
```
Para un sistema Ubuntu, también querrá ajustar el `INSTALL_LIBPATH` en el fichero `Makefile` de `libbitcoinrpc` para instalar en `/usr/lib` lugar de `/usr/local/lib`:
```
$ emacs ~/libbitcoinrpc/Makefile 
...
INSTALL_LIBPATH    := $(INSTALL_PREFIX)/usr/lib
```
(Si prefiere no mancillar su `/usr/lib`, la alternativa es cambiar su `etc/ld.so.conf` o sus archivos dependientes de manera apropiada  ... pero para una configuración de prueba en una máquina de prueba, esto probablemente esté bien)

Del mismo modo, también querrá ajustar el `INSTALL_HEADERPATH` en el `libbitcoinrpc` `Makefile` para instalar en `/usr/include` en lugar de `/usr/local/include`:
```
...
INSTALL_HEADERPATH    := $(INSTALL_PREFIX)/usr/include
```

Entonces puede compilar:
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
Si eso funciona, puede instalar el paquete:
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

## Prepare su código

`libbitcoinrpc` tiene métodos simples y bien estructurados para conectarse a su `bitcoind`,  ejecutar llamadas RPC y decodificar la respuesta.

Para usar  `libbitcoinrpc`, asegúrese de que sus archivos de código incluyan los encabezados adecuados:
```
#include <jansson.h>
#include <bitcoinrpc.h>
```
También deberá vincular las bibliotecas correspondientes cada vez que compile:
```
$ cc yourcode.c -lbitcoinrpc -ljansson -o yourcode
```

## Construya su conexión

Establecer la conexión con su `bitcoind` requiere unos sencillos pasos.

Primero, inicialice la biblioteca:
```
bitcoinrpc_global_init();
```
Luego conéctese a su `bitcoind` con `bitcoinrpc_cl_init_params`. Los cuatro argumentos son  `bitcoinrpc_cl_init_params` son nombre de usuario, contraseña, dirección IP y puerto. Ya debería conocer toda esta información de su trabajo con [Curl](04_4_Interludio_Usando_Curl.md). Como recordará, la dirección IP 127.0.0.1 y el puerto 18332 deben ser correctos para la configuración estándar de testnet descrita en estos documentos, mientras que puede extraer el usuario y la contraseña de `~/.bitcoin/bitcoin.conf`.
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
Que luego coloca en el `bitcoinrpc_cl_init_params`:
```
bitcoinrpc_cl_t *rpc_client;
rpc_client = bitcoinrpc_cl_init_params("StandUp", "6305f1b2dbb3bc5a16cd0f4aac7e1eba", "127.0.0.1", 18332);
```

> **MAINNET VS TESTNET:**  El puerto sería 8332 para una configuración de mainnet.

Si `rpc_client` se inicializa correctamente, podrá enviar comandos RPC.

Más tarde, cuando haya terminado con su conexión a `bitcoind`, debe cerrarla:
```
bitcoinrpc_global_cleanup();
```

### Pruebe el código de prueba

El código de prueba se puede encontrar en el directorio src [16_1_testbitcoin.c](../src/16_1_testbitcoin.c). Descárguelo en su máquina de testnet, luego inserte la contraseña de RPC correcta (y cambie el usuario de RPC si no creó su servidor con StandUp).

Puede compilar y ejecutar esto de la siguiente manera:
```
$ cc testbitcoin.c -lbitcoinrpc -ljansson -o testbitcoin
$ ./testbitcoin 
Successfully connected to server!
```

> :warning: **ADVERTENCIA:** Si olvida ingresar su contraseña RPC en este o cualquier otro código de muestra que dependa de RPC, recibirá un mensaje misterioso `ERROR CODE 5`.

## Realizar una llamada RPC

Para utilizar un método RPC con `libbitcoinrpc`, debe inicializar una variable de tipo `bitcoinrpc_method_t`. Lo hace con el valor apropiado para el método que desea utilizar, todos los cuales se enumeran en la [referencia bitcoinrpc](https://github.com/gitmarek/libbitcoinrpc/blob/master/doc/reference.md).
```
bitcoinrpc_method_t *getmininginfo  = NULL;
getmininginfo = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETMININGINFO);
```
Por lo general, establecería los parámetros a continuación, pero `getmininginfo` no requiere parámetros, por lo que puede omitir eso por ahora.

También debe crear otros dos objetos, un "objeto de respuesta" y un "objeto de error". Se pueden inicializar de la siguiente manera:
```
bitcoinrpc_resp_t *btcresponse  = NULL;
btcresponse = bitcoinrpc_resp_init();

bitcoinrpc_err_t btcerror;
```
Use la variable `rpc_client` sobre la que ya aprendió en la prueba anterior y agregue su método `getmininginfo` y los otros dos objetos:
```
bitcoinrpc_call(rpc_client, getmininginfo, btcresponse, &btcerror);
```
### Envíe su respuesta

Querrá saber qué devolvió la llamada RPC. Para hacerlo, recupere el resultado de su llamada como un objeto JSON `bitcoinrpc_resp_get` y guárdelo en un objeto `jansson`, del tipo `json_t`:
```
json_t *jsonresponse = NULL;
jsonresponse = bitcoinrpc_resp_get(btcresponse);
```
Si desea generar los resultados JSON completos de la llamada RPC, puede hacerlo con una simple invocación de `json_dumps`, también desde la librería `jansson`:
```
printf ("%s\n", json_dumps(j, JSON_INDENT(2)));
```
Sin embargo, dado que ahora está escribiendo programas completos, probablemente desee hacer un trabajo más sutil, como extraer valores JSON individuales para un uso específico. La [referencia jansson](https://jansson.readthedocs.io/en/2.10/apiref.html) detalla cómo hacerlo.

Al igual que cuando usaba [Curl](04_4_Interludio_Usando_Curl.md), encontrará que RPC devuelve un objeto JSON que contiene un `id`, un `error`, y, lo más importante, un objeto JSON de `result`. 

La función `json_object_get` le permitirá recuperar un valor (como el `result`) de un objeto JSON por clave:
```
json_t *jsonresult = NULL;
jsonresult = json_object_get(jsonresponse,"result");
printf ("%s\n", json_dumps (jsonresult, JSON_INDENT(2)));
```
Sin embargo, probablemente desee profundizar más para obtener una variable específica. Una vez que haya recuperado el valor apropiado, deberá convertirlo en un objeto C estándar usando la función `json_*_value`. Por ejemplo, para acceder a un número entero usa `json_integer_value`:
```
json_t *jsonblocks = NULL;
jsonblocks = json_object_get(jsonresult,"blocks");

int blocks;
blocks = json_integer_value(jsonblocks);
printf("Block Count: %d\n",blocks);
```

> :warning: **ADVERTENCIA:** Es extremadamente fácil segmentar su código C cuando se trabaja con objetos `jansson` si se confunde con el tipo de objeto que está recuperando. Haga un uso cuidadoso de `bitcoin-cli help` para saber lo que debe esperar, y si experimenta una falla de segmentación, primero observe sus funciones de recuperación de JSON.

### Prueba el código de información

Recupere el código de prueba del [directorio src](../src/16_1_getmininginfo.c).
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
## Realizar una llamada RPC con argumentos

Pero lo que si su llamada RPC no tiene argumentos?

### Crear un arreglo JSON

Para enviar parámetros a su llamada RPC usando `libbitcoinrpc` debe envolverlos en un arreglo JSON. Dado que una matriz es solo una simple lista de valores, todo lo que tiene que hacer es codificar los parámetros como elementos ordenados en la matriz.

Cree el arreglo JSON usando la función  `json_array` de `jansson`:
```
json_t *params = NULL;
params = json_array();
```
Luego, invertirá el procedimiento que siguió para acceder a los valores JSON: convertirá objetos de tipo C en objetos de tipo JSON utilizando las funciones `json_*`. Luego, los agregará al arreglo:
```
json_array_append_new(params,json_string(tx_rawhex));
```
Tenga en cuenta que hay dos variantes del comando append: `json_array_append_new`, que agrega una variable recién creada y `json_array_append`, que agrega una variable existente.

Esta sencilla metodología `json_array_append_new` servirá para la mayoría de los comandos RPC con parámetros, pero algunos comandos RPC requieren entradas más complejas. En estos casos, es posible que deba crear objetos JSON subsidiarios o matrices JSON, que luego agregará a la matriz de parámetros como de costumbre. La siguiente sección contiene un ejemplo de cómo hacerlo usando  `createrawtransaction`, que contiene una matriz JSON de objetos JSON para las entradas, un objeto JSON para las salidas y el parámetro `locktime`. 

### Asignar los parámetros

Cuando haya creado su matriz JSON de parámetros, simplemente asígnela después de haber inicializado su método RPC, de la siguiente manera:
```
bitcoinrpc_method_set_params(rpc_method, params)
```
Esta sección no incluye un ejemplo completo de esta metodología más compleja, pero la veremos en acción varias veces en nuestro primer programa C completo basado en RPC, en la siguiente sección.

## Resumen: Accediendo a Bitcoind con C

Al vincular las bibliotecas RPC `bitcoinrpc` y JSON `jansson`, puede acceder fácilmente a `bitcoind` través de llamadas RPC desde una biblioteca C. Para hacerlo, cree una conexión RPC y luego realice llamadas RPC individuales, algunas de ellas con parámetros. `jansson` luego le permite decodificar las respuestas JSON. La siguiente sección demostrará cómo se puede utilizar esto para un programa pragmático del mundo real.

* :fire: ***Cuál es el poder de C?*** C le permite dar el siguiente paso más allá de las secuencias de comandos de shell, lo que permite la creación de programas más completos y sólidos.

## Que sigue?

Obtenga más información sobre "Hablar con Bitcoind con C" en [16.2: Programando Bitcoind en C con las Bibliotecas RPC](16_2_Programando_Bitcoind_en_C_con_las_Bibliotecas_RPC.md).
