# Interludio: Acceso a un segundo nodo de Lightning

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Cuando jugaba con Bitcoin estaba accediendo a una red existente, y eso hizo que fuera relativamente fácil trabajar con ello: solo fue arrancar  `bitcoind` e inmediatamente interactuaba con la red. Así funciona ahora Lightning: es fundamentalmente una red de igual a igual, construida a partir de las conexiones entre dos nodos individuales. En otras palabras, para interactuar con la red Lightning, primero tendrá que encontrar un nodo al cual conectarse.

Hay cuatro maneras de hacerlo (las tres primeras son posibles para su primera conexión):

## Solicitar información sobre un nodo

Si alguien más ya tiene un nodo Lightning en la red de su elección, simplemente pídale su ID. 

Si están ejecutando c-lightning, solo necesitan usar el comando `getinfo`:

```
$ lightning-cli getinfo
lightning-cli: WARNING: default network changing in 2020: please set network=testnet in config!
   "id": "03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687",
   "alias": "VIOLETGLEE",
   "color": "03240a",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      }
   ],
   "binding": [
      {
         "type": "ipv6",
         "address": "::",
         "port": 9735
      },
      {
         "type": "ipv4",
         "address": "0.0.0.0",
         "port": 9735
      }
   ],
   "version": "v0.9.1-96-g6f870df",
   "blockheight": 1862854,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```
A continuación, pueden decirle su `id` (`03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687`). También tendrán que indicarle su dirección IP (`74.207.240.32`) y su puerto (`9735`).

## Crear un nuevo nodo c-lightning

Sin embargo, para fines de prueba, es probable que desee tener un segundo nodo bajo su propio control. La forma más fácil de hacerlo es crear un segundo nodo c-lightning en una máquina nueva, usando Bitcoin Standup, por [§2.1](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md) o compilándolo a mano, por [§19.1](19_1_Verificando_Su_Configuracion_Lightning.md).

Una vez que tenga su nodo en ejecución, puede ejecutar `getinfo` para recuperar su información, como se muestra arriba.

## Crear un nuevo nodo LND

Sin embargo, para nuestros ejemplos en el siguiente capítulo, vamos a crear un nodo LND. Esto nos permitirá demostrar un poco de la profundidad del ecosistema Lightning al mostrar cómo funcionan comandos similares en las dos plataformas diferentes.

Una forma de crear un nodo LND es ejecutar los Scripts de Bitcoin de nuevo en una máquina nueva, pero esta vez debe elegir LND, en [§2.1](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md).

Otra es compilar LND a partir del código fuente en una máquina donde ya está ejecutando un nodo Bitcoin, como sigue.

### Compilar el código fuente del LND 

Primero, necesita descargar e instalar Go:
```
$ wget --progress=bar:force https://dl.google.com/go/"go1.14.4"."linux"-"amd64".tar.gz -O ~standup/"go1.14.4"."linux"-"amd64".tar.gz
$ /bin/tar xzf ~standup/"go1.14.4"."linux"-"amd64".tar.gz -C ~standup
$ sudo mv ~standup/go /usr/local
```
Asegúrese de que la versión Go es la más actualizada (es `go1.14.4` en el momento actual), y la plataforma y la arquitectura son adecuadas para su máquina. (Lo anterior funcionará para Debian.)

Actualizar su ruta:
```
$ export GOPATH=~standup/gocode
$ export PATH="$PATH":/usr/local/go/bin:"$GOPATH"/bin
```
Entonces asegúrese de que `go` funciona:
```
$ go version
go version go1.14.4 linux/amd64
```
También necesitará `git` y `make`:
```
$ sudo apt-get install git
$ sudo apt-get install build-essential
```
Ahora está listo para recuperar LND. Asegúrese de obtener el verison actual (actualmente `v0.11.0-beta.rc4`).
```
$ go get -d github.com/lightningnetwork/lnd
```
Y ahora puede compilar:
```
$ cd "$GOPATH"/src/github.com/lightningnetwork/lnd
$ git checkout v0.11.0-beta.rc4
$ make
$ make install
```
Esto se instalará en `~/gocode/bin`, que es `$GOPATH/bin`.

Debería moverlo a directorios globales:
```
$ sudo cp $GOPATH/bin/lnd $GOPATH/bin/lncli /usr/bin
```
### Crear un archivo de configuración de LND

A diferencia de c-lightning, necesitará crear un archivo de configuración predeterminado para LND.

Sin embargo, usted primero necesita habilitar ZMQ en su Bitcoind, si no lo hizo ya en [§16.3](16_3_Recibiendo_Notificaciones_de_Bitcoind_en_C_con_las_Bibliotecas_ZMQ.md).

Esto requiere agregar lo siguiente a su archivo `~/.bitcoin/bitcoin.conf` si no está ya allí:
```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```
Si usted está usando un archivo de configuración de Bitcoin desde Standup o algún otro `conf` especializado, asegúrese de poner sus nuevos comandos en la sección correcta. Idealmente, deben ir cerca de la parte superior del archivo, de lo contrario en la sección `[test]`  (asumiendo, como de costumbre, que está probando en testnet).

Luego debe reiniciar bitcoin (o simplemente reiniciar su máquina). Puede probar que está funcionando de la siguiente manera:
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
Ahora está listo para crear un archivo de configuración.

Primero, necesita recuperar su rpcuser y rpcpassword. Esta es una manera automatizada de hacerlo:
```
$ BITCOINRPC_USER=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcuser | awk -F = '{print $2}')
$ BITCOINRPC_PASS=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcpassword | awk -F = '{print $2}')
```
> :warning: **ADVERTENCIA:** Obviamente, nunca almacene su contraseña RPC en una variable de shell en un entorno de producción.

Luego, puede escribir el archivo:
```
$ mkdir ~/.lnd
$ cat > ~/.lnd/lnd.conf << EOF
[Application Options]
maxlogfiles=3
maxlogfilesize=10
#externalip=1.1.1.1 # change to your public IP address if required.
alias=StandUp
listen=0.0.0.0:9735
debuglevel=debug
[Bitcoin]
bitcoin.active=1
bitcoin.node=bitcoind
bitcoin.testnet=true
[Bitcoind]
bitcoind.rpchost=localhost
bitcoind.rpcuser=$BITCOINRPC_USER
bitcoind.rpcpass=$BITCOINRPC_PASS
bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332
bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333
EOF
```

### Crear un servicio de LND

Por último, puede crear un servicio LND para ejecutar automáticamente `lnd`:
```
$ cat > ~/lnd.service << EOF
# It is not recommended to modify this file in-place, because it will
# be overwritten during package upgrades. If you want to add further
# options or overwrite existing ones then use
# $ systemctl edit lnd.service
# See "man systemd.service" for details.
# Note that almost all daemon options could be specified in
# /etc/lnd/lnd.conf, except for those explicitly specified as arguments
# in ExecStart=
[Unit]
Description=LND Lightning Network Daemon
Requires=bitcoind.service
After=bitcoind.service
[Service]
ExecStart=/usr/bin/lnd
ExecStop=/usr/bin/lncli --lnddir /var/lib/lnd stop
PIDFile=/run/lnd/lnd.pid
User=standup
Type=simple
KillMode=process
TimeoutStartSec=60
TimeoutStopSec=60
Restart=always
RestartSec=60
[Install]
WantedBy=multi-user.target
EOF
```
A continuación, tendrá que instalar eso y poner en marcha las cosas:
```
$ sudo cp ~/lnd.service /etc/systemd/system
$ sudo systemctl enable lnd
$ sudo systemctl start lnd
```
(Espere que esto tome un minuto la primera vez.)

### Habilitar conexiones remotas

Al igual que con c-lightning, va a tener que hacer LND accesible a otros nodos. He aquí cómo hacerlo si utiliza `ufw`, según las configuraciones de Bitcoin Standup:
```
$ sudo ufw allow 9735
```
### Crear una cartera

La primera vez que ejecute LND, debe crear una cartera:
```
$ lncli --network=testnet create
```
LND le pedirá una contraseña y luego le preguntará si desea ingresar un mnemónico existente (simplemente pulse `n` para el último).

Ahora debería tener un funcionamiento `lnd`, que puede verificar con `getinfo`:
```
$ lncli --network=testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 0,
    "num_inactive_channels": 0,
    "num_peers": 2,
    "block_height": 1862848,
    "block_hash": "000000000000000ecb6fd95e1f486283d48683aa3111b6c23144a2056f5a1532",
    "best_header_timestamp": "1602632294",
    "synced_to_chain": true,
    "synced_to_graph": false,
    "testnet": true,
    "chains": [
        {
            "chain": "bitcoin",
            "network": "testnet"
        }
    ],
    "uris": [
    ],
    "features": {
        "0": {
            "name": "data-loss-protect",
            "is_required": true,
            "is_known": true
        },
        "5": {
            "name": "upfront-shutdown-script",
            "is_required": false,
            "is_known": true
        },
        "7": {
            "name": "gossip-queries",
            "is_required": false,
            "is_known": true
        },
        "9": {
            "name": "tlv-onion",
            "is_required": false,
            "is_known": true
        },
        "13": {
            "name": "static-remote-key",
            "is_required": false,
            "is_known": true
        },
        "15": {
            "name": "payment-addr",
            "is_required": false,
            "is_known": true
        },
        "17": {
            "name": "multi-path-payments",
            "is_required": false,
            "is_known": true
        }
    }
}
```
El ID de este nodo es `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`. Aunque este comando no le muestra la dirección IP y el puerto, deben ser la dirección IP para su máquina y el puerto `9735`. 

## Escuchando el Gossip

Si ya estaba conectado a la red lightning, y estaba "chismeando" con nodos compañeros, también podría ser capaz de encontrar información sobre compañeros automáticamente, a través del comando `listpeers`:

```       
c$ lightning-cli --network=testnet listpeers
{
   "peers": [
      {
         "id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
         "connected": true,
         "netaddr": [
            "127.0.0.1:9736"
         ],
         "features": "02a2a1",
         "channels": []
      }
   ]
}
```       
Sin embargo, eso definitivamente no será el caso para su primera interacción con la red Lightning.

## Resumen: Acceso a un segundo nodo de rayos

Siempre necesitará dos nodos Lightning para formar un canal. Si no tiene a alguien más que esté probando las cosas con usted, va a necesitar crear un segundo nodo, ya sea usando c-lightning o (como haremos en nuestros ejemplos) LND.

## ¿Qué es lo siguiente?

Aunque posiblemente haya creado un LND, c-lightning seguirá siendo el corazón de nuestros ejemplos hasta que necesitemos empezar a usar ambos, en el [Capítulo 19](19_0_Entendiendo_Su_Configuracion_Lightning.md).

Continuar "Comprender la configuración de nodo Lightning" con [§19.3: Creando un Canal en Lightning](19_3_Creando_un_Canal_en_Lightning.md).
