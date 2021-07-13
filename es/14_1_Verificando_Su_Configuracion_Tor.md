# 14.1: Verificación de la configuración de Tor

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con advertencias.

Si realizó una instalación estándar con [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup), entonces debería tener Tor configurado como parte de su nodo Bitcoin: Tor está instalado y ha creado servicios ocultos para los puertos RPC de Bitcoin; mientras que también se ha creado una dirección de cebolla para `bitcoind`. Esta sección habla sobre qué es todo eso y qué hacer con él.

> :book: ***Qué es Tor?*** Tor es una red de anonimato y superposición de baja latencia basada en enrutamiento de cebolla y diseño de construcción de rutas para permitir la comunicación anónima. Es un software gratuito y de código abierto con el nombre derivado del acrónimo del nombre del proyecto de software original: "The Onion Router".

> :book: ***Por qué usar Tor para Bitcoin?*** La red Bitcoin es una red de igual a igual que escucha las transacciones y las propaga utilizando una dirección IP pública. Al conectarse a la red sin usar Tor, compartiría su dirección IP, lo que podría exponer su ubicación, su tiempo de actividad y otros detalles a terceros, lo cual es una práctica de privacidad indeseable. Para protegerse en línea, debe usar herramientas como Tor para ocultar los detalles de su conexión. Tor permite mejorar su privacidad en línea ya que sus datos están codificados criptográficamente y pasan por diferentes nodos, cada uno decodificando una sola capa (de ahí la metáfora de la cebolla.

## Entender Tor

Entonces, ¿cómo funciona Tor?

Cuando un usuario quiere conectarse a un servidor de Internet, Tor intenta construir una ruta formada por al menos tres nodos relés de Tor, llamados Guard (entrada), Middle (medios) y Exit (salida). Mientras se construye esta ruta, se negocian claves de cifrado simétricas; cuando un mensaje se mueve a lo largo de la ruta, cada relé elimina su capa de cifrado. De esta manera, el mensaje llega al destino final en su forma original, y cada parte solo conoce el salto anterior y el siguiente y no puede determinar el origen ni el destino.

Así es como se ve una conexión sin Tor:
```
20:58:03.804787 IP bitcoin.36300 > lb-140-82-114-25-iad.github.com.443: Flags [P.], seq 1:30, ack 25, win 501, options [nop,nop,TS val 3087919981 ecr 802303366], length 29
```
Por el contrario, con Tor se transmite mucha menos información sobre las máquinas reales:
```
21:06:52.744602 IP bitcoin.58776 > 195-xxx-xxx-x.rev.pxxxxxm.eu.9999: Flags [P.], seq 264139:265189, ack 3519373, win 3410, options [nop,nop,TS val 209009853 ecr 3018177498], length 1050
21:06:52.776968 IP 195-xxx-xxx-x.rev.pxxxxxm.eu.9999 > bitcoin.58776: Flags [.], ack 265189, win 501, options [nop,nop,TS val 3018177533 ecr 209009853], length 0
```
En pocas palabras: Tor encripta sus datos de tal manera que oculta su origen, su destino y los servicios que está utilizando, mientras que un protocolo de encriptación estándar como TLS solo protege lo que contienen sus datos.

### Comprender la arquitectura de la red Tor

La arquitectura básica de la red Tor se compone de los siguientes componentes:

* **Client Tor (OP or Onion Proxy).** Un cliente Tor instala software local que actúa como un proxy cebolla. Empaqueta los datos de la aplicación en celdas que son todas del mismo tamaño (512 bytes), que luego envía a la red Tor. Una celda es la unidad básica de transmisión Tor.
* **Nodo de cebolla (OR o enrutador de cebolla).** Un nodo de cebolla transmite células provenientes del cliente Tor y de servidores en línea. Hay tres tipos de nodos de cebolla: de entrada (Guardia), nodos intermedios (Medio) y nodos de salida (Salida).
*  **Servidor de directorio.** Un servidor de directorio almacena información sobre los enrutadores de cebolla y los servidores de cebolla (servicios ocultos), como sus claves públicas.
*  **Onion Server (servidor oculto).** Un servidor cebolla admite aplicaciones TCP como páginas web o IRC como servicios.

### Comprender las limitaciones de Tor

Tor no es una herramienta perfecta. Debido a que la información de la red Tor se descifra en los nodos de salida antes de enviarse a sus destinos finales, teóricamente un observador podría recopilar suficientes metadatos para comprometer el anonimato y potencialmente identificar a los usuarios.

También hay estudios que sugieren que posibles exploits de la protección anti-DoS de Bitcoin podrían permitir a un atacante obligar a otros usuarios que usan Tor a conectarse exclusivamente a través de sus nodos de salida o sus pares de Bitcoin, aislando al cliente del resto de la red de Bitcoin. y exponerlos a la censura, la correlación y otros ataques.

Del mismo modo, los usuarios de Tor con Bitcoin podrían ser atacados mediante huellas dactilares configurando una cookie de dirección en sus nodos. Esto también permitiría la correlación y por tanto la desanonimización.

Mientras tanto, incluso sobre Tor, Bitcoin es solo un servicio pseudoanónimo debido a los muchos peligros de correlación que se derivan del propio libro mayor permanente. Esto significa que el uso de Bitcoin en Tor tiene más probabilidades de ser desanonimizado que otros servicios (y podría llevar a la desanonimización de otras actividades).

Dicho esto, Tor generalmente se considera más seguro que la alternativa, que es la navegación no anónima.

## Verifica tu configuración de Tor

Entonces, ¿cómo verifica que ha habilitado Tor? Si lo instaló con Bitcoin Standup, lo siguiente verificará que Tor se esté ejecutando en su sistema
```
$ sudo -u debian-tor tor --verify-config
```

Si Tor está instalado correctamente, debería salir así:
```
Jun 26 21:52:09.230 [notice] Tor 0.4.3.5 running on Linux with Libevent 2.0.21-stable, OpenSSL 1.0.2n, Zlib 1.2.11, Liblzma 5.2.2, and Libzstd N/A.
Jun 26 21:52:09.230 [notice] Tor can't help you if you use it wrong! Learn how to be safe at https://www.torproject.org/download/download#warning
Jun 26 21:52:09.230 [notice] Read configuration file "/etc/tor/torrc".
Configuration was valid
```
> :warning: **ADVERTENCIA:** Esto solo significa que Tor se está ejecutando, no que se esté usando para todas (o algunas) conexiones.

### Verifique su configuración de Tor para RPC

### Verifique su configuración de Tor para RPC

El propósito más importante de Tor, instalado por Bitcoin Standup, es ofrecer servicios ocultos para los puertos RPC que se utilizan para enviar comandos de estilo de línea de comandos `bitcoind`.

> :book: ***Qué es un servicio oculto de Tor?*** Un servicio oculto (también conocido como "un servicio de cebolla") es un servicio al que se puede acceder a través de Tor. La conexión realizada a ese servicio a través de la red Onion será anónima.

El archivo de configuración Tor se encuentra en `/etc/tor/torrc`. Si lo revisa, debería ver los siguientes servicios para proteger sus puertos RPC:
```
HiddenServiceDir /var/lib/tor/standup/
HiddenServiceVersion 3
HiddenServicePort 1309 127.0.0.1:18332
HiddenServicePort 1309 127.0.0.1:18443
HiddenServicePort 1309 127.0.0.1:8332
```
> :link: **TESTNET vs MAINNET:** Mainnet RPC se ejecuta en el puerto 8332, testnet en el puerto 18332.

> :information_source: **NOTA:** El directorio `HiddenServiceDires` es donde todos los archivos se mantienen para este servicio en particular. Si necesita buscar su dirección de cebolla, claves de acceso o agregar clientes autorizados, ¡aquí es donde debe hacerlo!

La forma más sencilla de probar su servicio oculto de RPC es utilizar la API [QuickConnect](https://github.com/BlockchainCommons/Bitcoin-Standup/blob/master/Docs/Quick-Connect-API.md) integrada en Bitcoin Standup. Simplemente descargue el código QR que se encuentra en /qrcode.pngy escanéelo con una billetera o un nodo que admita QuickConnect, como [The Gordian Wallet](https://github.com/BlockchainCommons/FullyNoded-2). Cuando escanee el QR, debería ver que la billetera se sincroniza con su nodo; lo hace utilizando los servicios ocultos de RPC.

La forma más difícil de probar su servicio oculto RPC es enviar un comando a `bitcoin-cli` con torify, lo que le permite traducir un comando normal de UNIX a un comando protegido por Tor. Es difícil porque necesita obtener tres datos.

1. **Su puerto de servicio oculto.** Esto viene de `/etc/tor/torrc/`. De forma predeterminada, es el puerto 1309.
2. **Su dirección Tor.** Esto está en el archivo `hostname` en el `HiddenServiceDir` directorio definido en `/etc/tor/torrc`. Por defecto el archivo es  asi `/var/lib/tor/standup/hostname`. Está protegido, por lo que deberá usar `sudo` para acceder a este.:
```
$ sudo more /var/lib/tor/standup/hostname
mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion
```
3. **Su contraseña RPC.** Esto está en `~/.bitcoin/bitcoin.conf`

Cuando tenga toda esa información, puede emitir un comando `bitcoin-cli` usando `torifyy` especificando `-rpcconnect` como su dirección de cebolla, `-rpcport` como su puerto de servicio oculto y `-rpcpassword` como su contraseña:

```
$ torify bitcoin-cli -rpcconnect=mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion -rpcport=1309 -rpcuser=StandUp -rpcpassword=685316cc239c24ba71fd0969fa55634f getblockcount
```

### Verifique su configuración de Tor para Bitcoind

Bitcoin Standup también garantiza que `bitcoind` esté configurado para comunicarse opcionalmente en una dirección de cebolla.

Puede verificar la configuración inicial de Tor para `bitcoind` haciendo grepping para "tor" `debug.log` en su directorio de datos:
```
$ grep "tor:" ~/.bitcoin/testnet3/debug.log
2021-06-09T14:07:04Z tor: ADD_ONION successful
2021-06-09T14:07:04Z tor: Got service ID vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd, advertising service vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion:18333
2021-06-09T14:07:04Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_v3_private_key
```
> :information_source: **NOTA:** Bitcoin Core ya no admite direcciones v2. La compatibilidad con Tor v2 se eliminó en # [#22050](https://github.com/bitcoin/bitcoin/pull/22050)

> **TESTNET vs MAINNET:** Mainnet `bitcoind` responde en el puerto 8333, testnet en el puerto 18333.

Puede verificar que se ha creado un servicio oculto Tor para Bitcoin con la llamada RPC `getnetworkinfo`:

```
$ bitcoin-cli getnetworkinfo
...
 "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
...
```
Esto muestra tres direcciones para acceder a su servidor Bitcoin, una dirección IPv4 (`173.255.245.83`), una dirección IPv6 (`2600:3c01::f03c:92ff:fe86:f26`), y una dirección Tor (`vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion`).

> :warning: **ADVERTENCIA:** Obviamente: ¡nunca revele su dirección Tor de una manera que esté asociada con su nombre u otra PII!

Puede ver información similar con `getnetworkinfo`.
```
 bitcoin-cli getnetworkinfo
{
  "version": 200000,
  "subversion": "/Satoshi:0.20.0/",
  "protocolversion": 70015,
  "localservices": "0000000000000408",
  "localservicesnames": [
    "WITNESS",
    "NETWORK_LIMITED"
  ],
  "localrelay": true,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 10,
  "networks": [
    {
      "name": "ipv4",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "ipv6",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "onion",
      "limited": false,
      "reachable": true,
      "proxy": "127.0.0.1:9050",
      "proxy_randomize_credentials": true
    }
  ],
  "relayfee": 0.00001000,
  "incrementalfee": 0.00001000,
  "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
  "warnings": ""
}
```
Este servicio oculto permitirá conexiones anónimas a su `bitcoind` en la red Bitcoin.

> :warning: **ADVERTENCIA:** Ejecutar Tor y tener un servicio oculto de Tor no le obliga ni a usted ni a sus compañeros a usar Tor.

### Verifique su configuración de Tor para los pares

Usando el comando RPC `getpeerinfo`, puede ver qué nodos están conectados a su nodo y verificar si están conectados con Tor.

```
$ bitcoin-cli getpeerinfo
```
Algunos pueden estar conectados a través de Tor:
```
...
{
    "id": 9,
    "addr": "nkv.......xxx.onion:8333",
    "addrbind": "127.0.0.1:51716",
    "services": "000000000000040d",
    "servicesnames": [
      "NETWORK",
      "BLOOM",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1593981053,
    "lastrecv": 1593981057,
    "bytessent": 1748,
    "bytesrecv": 41376,
    "conntime": 1593980917,
    "timeoffset": -38,
    "pingwait": 81.649295,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 637875,
    "banscore": 0,
    "synced_headers": -1,
    "synced_blocks": -1,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00000000,
    "bytessent_per_msg": {
      "addr": 55,
      "feefilter": 32,
      "getaddr": 24,
      "getheaders": 1053,
      "inv": 280,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 30082,
      "feefilter": 32,
      "getdata": 280,
      "getheaders": 1053,
      "headers": 106,
      "inv": 9519,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    }
  }
...
```
Es posible que algunos no, como ésta conexión IPv6:
```
...
  {
    "id": 17,
    "addr": "[2001:638:a000:4140::ffff:191]:18333",
    "addrlocal": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "addrbind": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "services": "0000000000000409",
    "servicesnames": [
      "NETWORK",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1595447081,
    "lastrecv": 1595447067,
    "bytessent": 12250453,
    "bytesrecv": 2298711417,
    "conntime": 1594836414,
    "timeoffset": -1,
    "pingtime": 0.165518,
    "minping": 0.156638,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 1780784,
    "banscore": 0,
    "synced_headers": 1781391,
    "synced_blocks": 1781391,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00001000,
    "bytessent_per_msg": {
      "addr": 4760,
      "feefilter": 32,
      "getaddr": 24,
      "getdata": 8151183,
      "getheaders": 1085,
      "headers": 62858,
      "inv": 3559475,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 132,
      "sendheaders": 24,
      "tx": 145098,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 33877,
      "block": 2291124374,
      "feefilter": 32,
      "getdata": 9430,
      "getheaders": 1085,
      "headers": 60950,
      "inv": 2019175,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 66,
      "sendheaders": 24,
      "tx": 5136622,
      "verack": 24,
      "version": 126
    }
  }
...
```
Having a Tor address for your `bitcoind` is probably somewhat less useful than having a Tor address for your RPC connections. That's in part because it's not recommended to try and send all your Bitcoin connections via Tor, and in part because protecting your RPC commands is really what's important: you're much more likely to be doing that remotely, from a software wallet like The Gordian Wallet, while your server itself is more likely to be sitting in your office, basement, or bunker.

Nonetheless, there are ways to make `bitcoind` use Tor more, as discussed in the next section.

Tener una dirección Tor para su `bitcoind` probablemente sea algo menos útil que tener una dirección Tor para sus conexiones RPC. Eso es en parte porque no se recomienda intentar enviar todas sus conexiones de Bitcoin a través de Tor, y en parte porque proteger sus comandos RPC es realmente lo importante: es mucho más probable que lo haga de forma remota, desde una billetera de software como The Gordian. Wallet, mientras que es más probable que su servidor esté en su oficina, sótano o búnker.

No obstante, hay formas de forzar que `bitcoind` use Tor, como se explica en la siguiente sección.

## Resumen: verificación de la configuración de Tor

Tor es un paquete de software instalado como parte de Bitcoin Standup que le permite intercambiar comunicaciones de forma anónima. Protegerá tanto sus puertos RPC (8332 o 18332) como sus puertos `bitcoind` (8333 o 18333), ¡pero debe conectarse activamente a la dirección de cebolla para usarlos! Tor es una piedra de construcción de privacidad y seguridad para su configuración de Bitcoin, y puede verificar que está disponible y vinculado a Bitcoin con unos pocos comandos simples.

> :fire: ***Cuál es el poder de Tor?*** Muchos ataques a los usuarios de Bitcoin dependen de saber quién es la víctima y de que están realizando transacciones con Bitcoins. Tor puede protegerle de eso ocultando dónde está y qué está haciendo. Es particularmente importante si desea conectarse a su propio nodo de forma remota a través de una billetera de software, y puede ser crucial si lo hace en algún país donde es posible que no sienta que su uso de Bitcoin es apreciado o protegido. Si debe llevar sus servicios de Bitcoin a la carretera, asegúrese de que su billetera sea totalmente compatible con Tor e intercambie todos los comandos RPC con su servidor utilizando ese protocolo.

## Que sigue?

Continue "Entendiendo Tor" con [§14.2: Cambiando sus servicios ocultos Bitcoin](14_2_Cambiando_Sus_Servicios_Bitcoin_Ocultos.md).
