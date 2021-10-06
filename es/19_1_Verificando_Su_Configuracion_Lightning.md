# 19.1: Creación de una configuración de c-lightning

> :information_source: **NOTA :** Esta sección se agregó recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con advertencias.

En esta sección, instalará y verificará c-lightning, su utilidad para acceder a Lightning Network.

> :book: ***¿Qué es Lightning Network?*** Lightning Network es una red descentralizada que utiliza la funcionalidad de contrato inteligente de la cadena de bloques de Bitcoin para permitir pagos instantáneos a través de una red de participantes. Lightning está construido como un protocolo de capa 2 que interactúa con Bitcoin para permitir a los usuarios intercambiar sus bitcoins "fuera de la cadena".

> :book: ***¿Qué es un protocolo de capa 2?*** La capa 2 se refiere a un protocolo secundario construido sobre el sistema de cadena de bloques de Bitcoin. El objetivo principal de estos protocolos es resolver la velocidad de transacción y las dificultades de escalado que están presentes en Bitcoin: Bitcoin no es capaz de procesar miles de transacciones por segundo (TPS), por lo que se han creado protocolos de capa 2 para resolver el problema de escalabilidad de blockchain. Estas soluciones también se conocen como soluciones de escalado "fuera de la cadena".

## Instalar C-Lightning

Si utilizó [Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts), es posible que ya haya instalado Lightning al comienzo de este curso. Puede probar esto viendo si `lightningd` se está ejecutando:

```
$ ps auxww | grep -i lightning
standup  31213  0.0  0.2  24144 10424 pts/0    S    15:38   0:00 lightningd --testnet
standup  31214  0.0  0.1  22716  7444 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/autoclean
standup  31215  0.0  0.2  22992  8248 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/bcli
standup  31216  0.0  0.1  22756  7604 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/keysend
standup  31217  0.0  0.1  22776  7648 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/pay
standup  31218  0.0  0.1  22720  7652 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/txprepare
standup  31219  0.0  0.1  22744  7716 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/spenderp
standup  31227  0.0  0.1  22748  7384 pts/0    SL   15:38   0:00 /usr/local/libexec/c-lightning/lightning_hsmd
standup  31228  0.0  0.2  23044  8192 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_connectd
standup  31229  0.0  0.1  22860  7556 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_gossipd
standup  32072  0.0  0.0   6208   888 pts/0    S+   15:50   0:00 grep -i lightning
```

Si no es así, deberá instalarlo ahora. Desafortunadamente, si está utilizando Debian, deberá instalarlo a mano, compilando el código fuente, pero aún así debería ser bastante simple si sigue estas instrucciones. Si se encuentra en un sistema Ubuntu estándar, intente [Instalar desde Ubuntu ppa](#variant-install-from-ubuntu-ppa), y siempre puede intentar [Instalar binarios precompilados](#variant-install-binarios precompilados).

> :book: ***¿Qué es c-lightning?*** Hay tres implementaciones diferentes de Lightning en la actualidad: c-lightning, LND y Eclair. Todos deben ser funcionalmente compatibles, basados ​​en las mismas [RFC de BOLT](https://github.com/lightningnetwork/lightning-rfc/blob/master/00-introduction.md), pero sus detalles de implementación pueden ser diferentes. Hemos elegido c-lightning como base de nuestro curso porque también es parte del mismo [Elements Project](https://github.com/ElementsProject) que también contiene Libwally.

### Compilar el código fuente de c-lightning

La instalación de Lightning desde el código fuente debería ser bastante simple si sigue estas instrucciones.

_Probablemente_ desea hacer esto en un nodo sin podar, ya que trabajar con nodos podados en Lightning puede causar problemas con la instalación y el uso. Si configuró su nodo al principio de este curso para podarlo, es posible que desee reemplazarlo con un nodo sin podar ahora. (Si está usando testnet, debería poder usar el mismo tipo de máquina que usó para su nodo podado).

> :warning:**ADVERTENCIA:** En realidad, puede ejecutar c-lightning en un nodo podado. Sin embargo, como señala el [repositorio Lightning](https://github.com/ElementsProject/lightning#pruning), puede haber problemas. Para que funcione, debe asegurarse de que su nodo Lightning solo intente actualizar la información de los bloques que su nodo Bitcoin no ha eliminado. Para hacerlo, debe asegurarse de (1) que su nodo Bitcoin esté completamente actualizado antes de iniciar su nodo Lightning por primera vez; y (2) que su nodo Lightning nunca se quede muy atrás de su nodo Bitcoin (para una poda estándar de 550 bloques, nunca se puede apagar durante 4 o más días). Entonces, puede hacerlo, pero presenta cierto peligro, lo cual no es una buena idea si está ejecutando un servicio de producción.

Con eso, está listo para instalar Lightning:

Primero, instale las dependencias, incluidos los requisitos de desarrollo.

```
$ sudo apt-get install -y \
   autoconf automake build-essential git libtool libgmp-dev \
   libsqlite3-dev python3 python3-mako net-tools zlib1g-dev libsodium-dev \
   gettext
$ sudo apt-get install -y valgrind python3-pip libpq-dev
```

Estos pueden llevar un tiempo, porque hay varios y algunos son grandes.

En segundo lugar, clona el repositorio de Lightning:

```
$ cd ~
$ git clone https://github.com/ElementsProject/lightning.git
$ cd lightning
```

Ahora puede usar el `pip3` que instaló para instalar requisitos adicionales para la compilación y configurarlo todo:

```
$ pip3 install -r requirements.txt
$ ./configure
```

Ahora compila. Esto también puede llevar algún tiempo dependiendo de su máquina.

```
$ make
```

Luego, todo lo que necesita hacer es instalar:
```
$ sudo make install
```

## Compruebe su instalacion 
Puede confirmar que ha instalado lightningd correctamente usando el parámetro `help`:


## Ejecutar lightining

Comenzará su exploración de la red Lightning con el comando `lightning-cli`. Sin embargo, `lightningd` _ debe_ estar ejecutándose para usar` lightning-cli`, ya que `lightning-cli` envía comandos JSON-RPC al` lightningd` (todo igual que con `bitcoin-cli` y` bitcoind`).

Si instaló `c-lightning` a mano, ahora deberá iniciarlo:

```
$ nohup lightningd --testnet &
```

### Ejecutar lightningd como servicio

Si lo prefiere, puede instalar `lightningd` como un servicio que se ejecutará cada vez que reinicie su máquina. Lo siguiente lo hará y comenzará a ejecutarse de inmediato:

```
$ lightningd --help
lightningd: WARNING: default network changing in 2020: please set network=testnet in config!
Usage: lightningd 
A bitcoin lightning daemon (default values shown for network: testnet).
--conf=<file>                        Specify configuration file
--lightning-dir=<dir>                Set base directory: network-specific
                                     subdirectory is under here
                                      (default: "/home/javier/.lightning")
--network <arg>                      Select the network parameters (bitcoin,
                                     testnet, regtest, litecoin or
                                     litecoin-testnet) (default: testnet)
--testnet                            Alias for --network=testnet
--signet                             Alias for --network=signet
--mainnet                            Alias for --network=bitcoin

```

### Habilitar conexiones remotas

Si tiene algún tipo de firewall, deberá abrir el puerto 9735 para permitir que otros nodos Lightning se comuniquen con usted.

Si usa `ufw` de Bitcoin Standup, esto se hace de la siguiente manera:

```
$ sudo ufw allow 9735
```

## Verificar su nodo

Puede comprobar si su nodo Lightning está listo para funcionar comparando la salida de `bitcoin-cli getblockcount` con el resultado de` blockheight` de `lightning-cli getinfo`.


```
$ bitcoin-cli -testnet getblockcount
1838587
$ lightning-cli --testnet getinfo
{
   "id": "03d4592f1244cd6b5a8bb7fba6a55f8a91591d79d3ea29bf8e3c3a405d15db7bf9",
   "alias": "HOPPINGNET",
   "color": "03d459",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      },
      {
         "type": "ipv6",
         "address": "2600:3c01::f03c:92ff:fe48:9ddd",
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
   "blockheight": 1838587,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```

En este caso, el `blockheight` se muestra como` 1838587` por ambas salidas.

En su lugar, puede obtener un error, dependiendo de la situación precisa.

Si el nodo de Bitcoin aún se sincroniza con la red de bitcoin, debería ver un mensaje como este:

```
"warning_bitcoind_sync": "Bitcoind is not up-to-date with network."
```

Si su demonio lightning no está actualizado,  debería ver un mensaje como este:

```
"warning_lightningd_sync": "Still loading latest blocks from bitcoind."
```

Si intentó ejecutar en una cadena de bloques podada donde el nodo Bitcoin no estaba actualizado cuando inició el nodo Lightning, obtendrá mensajes de error en su registro como este:

```
bitcoin-cli -testnet getblock 0000000000000559febee77ab6e0be1b8d0bef0f971c7a4bee9785393ecef451 0 exited with status 1
```

## Crear alias

Sugerimos crear algunos alias para facilitar el uso de c-lightning.

Puede hacerlo colocándolos en su `.bash_profile`.

```
cat >> ~/.bash_profile <<EOF
alias lndir="cd ~/.lightning/" #linux default c-lightning path
alias lnc="lightning-cli"
alias lnd="lightningd"
alias lninfo='lightning-cli getinfo'
EOF
```

Después de ingresar estos alias, puede `source ~/.bash_profile` para ingresarlos o simplemente cerrar sesión y volver a ingresar.

Tenga en cuenta que estos alias incluyen atajos para ejecutar `lightning-cli`, para ejecutar` lightningd` y para ir al directorio c-lightning. Estos alias están destinados principalmente a facilitarle la vida. Le sugerimos que cree otros alias para facilitar el uso de comandos frecuentes (y argumentos) y minimizar los errores. Los alias de este tipo pueden ser aún más útiles si tiene una configuración compleja en la que ejecuta regularmente comandos asociados con Mainnet, con Testnet, _y_ con Regtest, como se explica más adelante.

Dicho esto, el uso de estos alias en _este_ documento podría oscurecer accidentalmente las lecciones centrales que se enseñan sobre c-lightning, por lo que continuaremos mostrando los comandos completos; ajuste para su propio uso según corresponda.

## Opcional: Modifique sus tipos de servidor

> :link: **TESTNET vs MAINNET:** Cuando configura su nodo, elige crearlo como un nodo Mainnet, Testnet o Regtest. Aunque este documento presupone una configuración de testnet, vale la pena comprender cómo puede acceder y utilizar los otros tipos de configuración, ¡incluso todos en la misma máquina! Pero, si es un usuario nuevo, omita esto, ya que no es necesario para una configuración básica.

Cuando se inicia lightningd, generalmente lee un archivo de configuración cuya ubicación depende de la red que esté utilizando (predeterminado: `~/.lightning/testnet/config`). Esto se puede cambiar con los indicadores `–conf` y` –lightning-dir`.

```
~/.lightning/testnet$ ls -la config
-rw-rw-r-- 1 user user 267 jul 12 17:08 config
```

También hay un archivo de configuración general (predeterminado: `~/.lightning/config`). Si desea ejecutar varios tipos diferentes de nodos simultáneamente, debe dejar el indicador testnet (o regtest) fuera de este archivo de configuración. A continuación, debe elegir si está utilizando la red principal, la red de prueba o su registro cada vez que ejecute `lightningd` o` lightning-cli`.

Es posible que su configuración no tenga ningún archivo de configuración: c-lightning se ejecutará con una buena configuración predeterminada sin ellos.

## Resumen: Verificación de su configuración Lightning

Antes de comenzar a jugar con Lightning, debe asegurarse de que sus alias estén configurados, su `lightningd` se esté ejecutando y su nodo esté sincronizado. Es posible que también desee configurar algún acceso a configuraciones alternativas de iluminación en otras redes.

## ¿Que sigue?

Continúe "Comprender su configuración Lightning" con [§19.2: Conocer su configuración Lightning](19_2_Conociendo_Su_Configuracion_Lightning.md).

## Variante: instalar desde Ubuntu ppa

Si está usando una versión de Ubuntu que no sea Debian, puede instalar c-lightning usando [Ubuntu ppa](https://launchpad.net/~lightningnetwork/+archive/ubuntu/ppa):

```
$ sudo apt-get install -y software-properties-common
$ sudo add-apt-repository -u ppa:lightningnetwork/ppa
$ sudo apt-get install lightningd
```

## Variante: instalar binarios precompilados

Otro método para instalar Lightning es utilizar los binarios precompilados en el [repositorio de Github](https://github.com/ElementsProject/lightning/releases). Elija el tarball más reciente, como `clightning-v0.9.1-Ubuntu-20.04.tar.xz`.

Después de descargarlo, debe moverse al directorio raíz y descomprimirlo:

```
$ cd /
$ sudo tar xf ~/clightning-v0.9.1-Ubuntu-20.04.tar.xz 
```

Advertencia: esto requerirá que tenga exactamente las mismas bibliotecas que se usaron para crear el binario. A menudo es más fácil simplemente volver a compilar.
