# 3.1: Verificación de la configuración de Bitcoin

Antes de comenzar a jugar con Bitcoin, debe asegurarse de que todo esté configurado correctamente.

## Cree sus Alias

Sugerimos crear algunos alias para facilitar el uso de Bitcoin.

Puede hacerlo colocándolos en sus archivos `.bash_profile`, `.bashrc` o `.profile`.
```
cat >> ~/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
EOF
```
Después de ingresar estos alias, puede ejecutar `source .bash_profile` para cargarlos o iniciar sesión de nuevo.

Tenga en cuenta que estos alias incluyen atajos para ejecutar `bitcoin-cli`, para ejecutar `bitcoind` y para posicionarse en el directorio de Bitcoin. 
Estos alias están destinados principalmente a facilitarle la vida. Le sugerimos que cree otros alias para facilitar el uso de comandos frecuentes 
(y argumentos) y minimizar los errores. Los alias de este tipo pueden ser aún más útiles si tiene una configuración compleja en la que ejecuta 
regularmente comandos asociados con Mainnet, con Testnet _y_ con Regtest, como se explica más adelante.

Dicho esto, el uso de estos alias en este documento podría oscurecer accidentalmente las lecciones principales que se enseñan sobre Bitcoin, 
por lo que el único alias que se usa directamente aquí es `btcinfo` porque encapsula un comando mucho más largo y complejo. De lo contrario, mostramos 
los comandos completos; ajuste para su propio uso según corresponda.

## Ejecutar Bitcoind

Comenzará su exploración de la red Bitcoin con el comando `bitcoin-cli`. Sin embargo, bitcoind _debe_ estar ejecutándose para usar bitcoin-cli, 
ya que bitcoin-cli envía comandos JSON-RPC al bitcoind. Si usó nuestra configuración estándar, bitcoind ya debería estar en funcionamiento. 
Puede verificar dos veces mirando la tabla de procesos.

```
$ ps auxww | grep bitcoind
standup    455  1.3 34.4 3387536 1392904 ?     SLsl Jun16  59:30 /usr/local/bin/bitcoind -conf=/home/standup/.bitcoin/bitcoin.conf
```
Si no se está ejecutando, querrá ejecutarlo `/usr/local/bin/bitcoind -daemon` manualmente y también colocarlo en su crontab.

## Verificando sus bloques

Debería tener toda la cadena de bloques descargada antes de comenzar a jugar. Simplemente ejecute el alias `bitcoin-cli getblockcount`  para ver si está todo cargado.

```
$ bitcoin-cli getblockcount
1772384
```
That tells you what's loaded; you'll then need to check that against an online service that tells you the current block height.
Eso le indica lo que está descargado; luego deberá verificarlo con un servicio en línea que le indique la altura actual del bloque.

> :book: ***Qué es la altura del bloque?*** La altura del bloque es la distancia a la que se retira un bloque particular del bloque génesis. La altura del bloque actual es la altura del bloque más nuevo agregado a una cadena de bloques.

Puede hacer esto mirando un explorador de blocknet, como el explorador de [Blockcypher Testnet explorer](https://live.blockcypher.com/btc-testnet/). Su número más reciente coincide con la salida del `getblockcount`? Si es así, está actualizado.

Si desea un alias para ver todo a la vez, lo siguiente funciona actualmente para Testnet, pero puede desaparecer en algún momento en el futuro:

```
$ cat >> ~/.bash_profile << EOF
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - https://blockstream.info/testnet/api/blocks/tip/height 2> /dev/null | cut -d : -f2 | rev | cut -c 1- | rev\\\`"
EOF
$ source .bash_profile 
$ btcblock
1804372/1804372
```

> :link: **TESTNET vs MAINNET:** Recuerde que este tutorial generalmente asume que está usando testnet. Si en cambio está utilizando la red MainNet, se puede recuperar la altura del bloque actual con: wget -O - http://blockchain.info/q/getblockcount 2>/dev/null. Puede reemplazar la segunda mitad del alias `btblock` (después de `/`) con eso.

Si no está actualizado, pero la salida de su `getblockcount` está aumentando, no hay problema. El tiempo total de descarga puede llevar desde una hora hasta varias horas, según su configuración.

## Opcional: conozca los tipos de servidor

> **TESTNET vs MAINNET:** Cuando configura su nodo, usted puede elegir crearlo como un nodo Mainnet, Testnet o Regtest. Aunque este documento presupone una configuración de testnet, vale la pena comprender cómo puede acceder y utilizar los otros tipos de configuración, ¡incluso todos en la misma máquina! Pero, si es un usuario nuevo, omita esto, ya que no es necesario para una configuración básica.

El tipo de configuración se controla principalmente a través del archivo ~/.bitcoin/bitcoin.conf. Si está ejecutando testnet, probablemente contenga esta línea:
```
testnet=1
```
Si está ejecutando regtest, probablemente contenga esta línea:
```
regtest=1
```

Sin embargo, si desea ejecutar varios tipos diferentes de nodos simultáneamente, debe dejar la marca testnet (o regtest) fuera de su archivo de configuración. Luego puede elegir si está utilizando la red principal, la red de prueba o su registro cada vez que ejecuta bitcoind o bitcoin-cli.

Aquí hay un conjunto de alias que lo facilitarían al crear un alias específico para iniciar y detener el bitcoind, para ir al directorio de bitcoin y para ejecutar bitcoin-cli, para cada una de las redes principales (que no tiene indicadores adicionales), el testnet (que es -testnet), o su regtest (que es -regtest).

```
cat >> ~/.bash_profile <<EOF
alias bcstart="bitcoind -daemon"
alias btstart="bitcoind -testnet -daemon"
alias brstart="bitcoind -regtest -daemon"

alias bcstop="bitcoin-cli stop"
alias btstop="bitcoin-cli -testnet stop"
alias brstop="bitcoin-cli -regtest stop"

alias bcdir="cd ~/.bitcoin/" #linux default bitcoin path
alias btdir="cd ~/.bitcoin/testnet" #linux default bitcoin testnet path
alias brdir="cd ~/.bitcoin/regtest" #linux default bitcoin regtest path

alias bc="bitcoin-cli"
alias bt="bitcoin-cli -testnet"
alias br="bitcoin-cli -regtest"
EOF
```

Para una mayor complejidad, puede hacer que cada uno de sus alias de 'inicio' use el indicador -conf para cargar la configuración desde un archivo diferente. Esto va mucho más allá del alcance de este tutorial, pero lo ofrecemos como punto de partida para cuando sus exploraciones de Bitcoin alcancen el siguiente nivel.

## Resumen: verificación de la configuración de Bitcoin

Antes de comenzar a jugar con bitcoin, debe asegurarse de que sus alias estén configurados, su bitcoind se esté ejecutando y sus bloques estén descargados. Es posible que también desee configurar algún acceso a configuraciones alternativas de Bitcoin, si es un usuario avanzado.

## Que sigue?

Continue con "Entendiendo su Configuración Bitcoin" en [§3.2: Knowing Your Bitcoin Setup](03_2_Knowing_Your_Bitcoin_Setup.md).
