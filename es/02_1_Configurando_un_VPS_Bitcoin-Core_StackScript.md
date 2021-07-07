# 2.1: Configuración de un VPS Bitcoin-Core con Bitcoin Standup

Este documento explica cómo configurar un VPS (Virtual Private Sever) para ejecutar un nodo Bitcoin en Linode.com, instalado usando un StackScript automatizado del [proyecto Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts) . Solo necesita ingresar algunos comandos e iniciar su VPS. Casi inmediatamente después de arrancar, encontrará su nuevo nodo Bitcoin descargando bloques felizmente.

> :warning: **ADVERTENCIA:** : No use un VPS para una billetera bitcoin con fondos reales significativos; ver [http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident](http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident). Es genial poder experimentar con transacciones reales de bitcoins en un nodo en vivo sin tener que conectar un servidor autohospedado en una red local. También es útil poder usar un iPhone o iPad para comunicarse a través de SSH con su VPS para realizar algunas tareas simples de bitcoin. Pero se requiere un mayor nivel de seguridad para fondos importantes.

* Si desea comprender lo que hace esta configuración, lea el [Appendix I: Understanding Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md) mientras realiza la instalación.
* Si, en cambio, desea configurar en una máquina que no sea un VPS Linode, como una máquina AWS o una Mac, puede ir a [2.2: Configuración de una máquina Bitcoin-Core a través de otros medios](02_2_Configurando_Bitcoin_Core_Otros.md)
* Si ya tiene un nodo de Bitcoin en ejecución, vaya al [Capítulo tres: Entendiendo la configuración de Bitcoin](03_0_Entendiendo_su_configuracion_Bitcoin.md).

## Introducción a Linode

Linode es un servicio de alojamiento en la nube que ofrece servidores Linux rápidos y económicos con almacenamiento SSD. Los usamos para este tutorial principalmente porque sus StackScripts basados en BASH ofrecen una manera fácil de configurar automáticamente un nodo de Bitcoin sin problemas y sin complicaciones.

### Configurar una cuenta de Linode

Puede crear una cuenta de Linode yendo aquí:

```
https://www.linode.com
```

Si lo prefiere, el siguiente código de referencia le dará aproximadamente un mes de uso gratuito, ideal para aprender Bitcoin:

```
https://www.linode.com/?r=23211828bc517e2cb36e0ca81b91cc8c0e1b2d96
```

Deberá proporcionar una dirección de correo electrónico y luego precargar dinero de una tarjeta de crédito o PayPal para costos futuros.

Cuando haya terminado, debe aterrizar en [https://cloud.linode.com/dashboard](https://cloud.linode.com/dashboard).

### Considere la autenticación de dos factores

La seguridad de su servidor no estará completa si las personas pueden ingresar a su cuenta de Linode, así que considere configurar la autenticación de dos factores para ello. Puede encontrar esta configuración en su página [Mi perfil: contraseña y autenticación](https://manager.linode.com/profile/auth). Si no hace esto ahora, cree un elemento TODO para volver y hacerlo más tarde.

## Creando la imagen de Linode usando un StackScript

### Cargar el StackScript

Descargue el [Script Linode Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) del repositorio de [Scripts de Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). Este script básicamente automatiza todas las instrucciones de configuración para un servidor VPS de Bitcoin. Si prefiere ser particularmente prudente, léelo con atención. Si está satisfecho, puede copiar ese StackScript en su propia cuenta dirigíendose a la [página de Stackscripts](https://cloud.linode.com/stackscripts?type=account) en su cuenta de Linode y seleccionando [Crear nuevo Stackscript](https://cloud.linode.com/stackscripts/create). Dale un buen nombre (usamos `Bitcoin Standup`), luego copia y pega el script. Elija Debian 10 para su imagen de destino y "Guárdelo".

### Realizar la configuración inicial

Ahora está listo para crear un nodo basado en Stackscript.

1. En la página [Stackscripts page](https://cloud.linode.com/stackscripts?type=account), haga clic en "..." a la derecha de su nuevo script y seleccione "Implementar nuevo Linode".
2. Complete un nombre de host breve y completo.
   * **Nombre de host corto.** Elija un nombre para su VPS. Por ejemplo, "mybtctest".
   * **Nombre de host completamente calificado.** Si va a incluir este VPS como parte de una red con registros DNS completos, escriba el nombre de host con su dominio. Por ejemplo, "mybtctest.mydomain.com". De lo contrario, repita el nombre de host corto y agregue ".local", por ejemplo "mybtctest.local".
3. Ingrese la contraseña para el usuario "standup".
4. Elija un tipo de instalación en las opciones avanzadas. 
   * **Tipo de instalación.** Probablemente sea "Mainnet" o "Pruned Mainnet" si está configurando un nodo para su uso y "Testnet" o "Pruned Testnet" si solo está probando. La mayor parte de este tutorial asumirá que eligió "Testnet podado", pero aún debería poder seguir con otros tipos. Consulte la [Synopsis](#synopsis-bitcoin-installation-types) para obtener más información sobre estas opciones. (Tenga en cuenta que si planea probar los capítulos Lightning, probablemente querrá usar un nodo sin podar, ya que trabajar con nodos podados en Lightning es dudoso. Consulte [§18.1](18_1_Verifying_Your_Lightning_Setup.md#compiling-the-source-code) para conocer los detalles).
5. Complete cualquier otra opción avanzada apropiada.
   * **X25519 Clave pública.** Esta es una clave pública para agregar a la lista de clientes autorizados de Tor. Si no lo usa, cualquiera que obtenga el código QR de su nodo puede acceder a él. Obtendrá esta clave pública de cualquier cliente que esté utilizando para conectarse a su nodo. Por ejemplo, si usa [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2), puede ir a su configuración y "Exportar clave pública de autenticación Tor V3" para usar aquí.
  * **Clave SSH.** Copie aquí la clave SSH de su computadora local; esto le permite iniciar sesión automáticamente a través de SSH en la cuenta standup. Si aún no ha configurado una clave SSH en su computadora local, hay buenas instrucciones para ello en [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/). Es posible que también desee agregar su clave SSH en su Linode LISH (Linode Interactive Shell) yendo a su "Página de inicio de Linode / Mis preferencias / Configuración de LISH / Teclas de LISH". El uso de una clave SSH le brindará una forma más sencilla y segura de iniciar sesión en su servidor.
  * **IP's permitidas por SSH** Esta es una lista de direcciones IP separadas por comas que se permitirán SSH en el VPS. Por ejemplo, "192.168.1.15,192.168.1.16". Si no ingresa ninguna IP, _su VPS no será muy seguro_. Será bombardeado constantemente por atacantes que intentan encontrar un puerta de entrada y es muy posible que lo logren.
4. Seleccione una imagen
   * **Imagen de destino.** Si siguió las instrucciones, esto solo le permitirá seleccionar "Debian 10" (aunque "Debian 9" también funcionó con versiones anteriores de este Stackscript y aún podría hacerlo).
5. Elija una región donde se ubicará el Linode.

*Las preguntas restantes tienen que ver con la mecánica de la implementación del VPS y deben dejarse como están con una excepción: aumente el disco de intercambio de 256 MB a 512 MB, para asegurarse de que tiene suficiente memoria para descargar la cadena de bloques.

Finalmente, deberá completar una contraseña de root, que será la contraseña utilizada para la cuenta de root.

### Elija otras opciones de standup

Blockchain Commons se encuentra actualmente en el proceso de expandir sus scripts de Bitcoin Standup con opciones para instalar Lightning y otras aplicaciones de Bitcoin importantes. Eche un vistazo a las opciones adicionales y vea si son cosas con las que le gustaría jugar. En particular, si Lightning es una opción, sugerimos instalarlo, porque hará que el [Capítulo 18](18_0_Understanding_Your_Lightning_Setup.md) y el [Capítulo 19](19_0_Using_Lightning.md) sean mucho más fáciles.

### Elija un plan Linode

A continuación, elegirás un plan Linode.

Un Linode 4GB será suficiente para la mayoría de las configuraciones, incluidas: Mainnet podada, Testnet podada e incluso Testnet no podada. Todos usan menos de 50G de almacenamiento y 4GB es una cantidad cómoda de memoria. Esta es la configuración que sugerimos. Tiene un costo de $ 20 por mes.

Si, en cambio, desea tener una Mainnet no podada en un VPS, deberá instalar un Linode con un disco de más de 280G (!), Que actualmente es el Linode de 16GB, que tiene 320G de almacenamiento y 16G de memoria. y cuesta aproximadamente $ 80 por mes. Nosotros no recomendamos esto.

El siguiente cuadro muestra los requisitos mínimos

| Configuración | Memoria | Almacenamiento | Linnode |
|-------|--------|---------|---------|
| Mainnet | 2G | 280G | Linode 16GB |
| Mainnet podada | 2G | ~5G | Linode 4GB |
| Testnet | 2G | ~15G | Linode 4GB |
| Testnet podada | 2G | ~5G | Linode 4GB |
| Regtest | 2G | ~ | Linode 4GB |

Tenga en cuenta que puede haber formas de reducir ambos costos.

* Para las máquinas que sugerimos como **Linode 4GB**, es posible que pueda reducirlo a un Linode 2GB. Algunas versiones de Bitcoin Core han funcionado bien con ese tamaño, algunas ocasionalmente se han quedado sin memoria y luego se han recuperado, y algunas se han quedado sin memoria continuamente. Recuerde aumentar ese espacio de intercambio para maximizar las probabilidades de que esto funcione. Úselo bajo su propio riesgo.
* Para la red Mainnet sin podar, que sugerimos como **Linode 16GB**, probablemente pueda arreglárselas con un Linode 4GB, pero agregue [Block Storage](https://cloud.linode.com/volumes) suficiente para almacenar la cadena de bloques. Esta es sin duda una mejor solución a largo plazo, ya que los requisitos de almacenamiento de la cadena de bloques de Bitcoin aumentan continuamente si no se poda, mientras que los requisitos de la CPU no lo hacen (o no en el mismo grado). Un almacenamiento de 320 GibiByte costaría $ 32 al mes, que combinado con un Linode 4GB cuesta $ 52 al mes, en lugar de $ 80, y lo que es más importante, puede seguir creciendo. No documentamos completamente esta configuración por dos razones (1) no sugerimos la configuración de la red principal sin podar, por lo que sospechamos que es una configuración mucho menos común y (2) no hemos probado cómo se comparan los volúmenes de Linodes con sus SSD intrínsecos en cuanto a rendimiento y uso. Pero hay documentación completa en la página de almacenamiento de bloques. Debería configurar el nodo Linode, ejecutar su stackscript, pero luego interrumpirlo para mover el almacenamiento de la cadena de bloques en exceso a un volumen recién añadido antes de continuar.

### Hacer la configuración final

Lo último que debe hacer es ingresar una contraseña de root. (¡Si te perdiste algo, te lo dirán ahora!)

Haga clic en "Implementar" para inicializar sus discos y preparar su VPS. Toda la cola debería ejecutarse en menos de un minuto. Cuando haya terminado, debería ver en la "Cola de trabajos del host", y unos botones verdes de "Éxito" que indican "Crear disco desde StackScript - Configuración de contraseña para root" y "Crear sistema de archivos - Imagen de intercambio de 256 MB" que se ha terminado.

Es posible que ahora desee cambiar el nombre de su VPS Linode del predeterminado `linodexxxxxxxx`. Vaya a la pestaña Configuración y cambie la etiqueta para que sea más útil, como el nombre de host corto de su VPS. Por ejemplo, puede llamarlo `bitcoin-testnet-pruned` para diferenciarlo de otros VPS en su cuenta.

### Inicie sesión en su VPS

Si observa su panel de control Linode, debería ver que la nueva computadora gira. Cuando el trabajo haya alcanzado el 100%, podrá iniciar sesión.

Primero, necesitará la dirección IP. Haga clic en la pestaña "Linodes" y debería ver una lista de sus VPS, el hecho de que se está ejecutando, su "plan", su dirección IP y alguna otra información.

Vaya a su consola local e inicie sesión en la cuenta `standup` usando esa dirección:

```
ssh standup@[IP-ADDRESS]
```

Por ejemplo:

```
ssh standup@192.168.33.11
```

Si configuró su VPS para usar una clave SSH, el inicio de sesión debe ser automático (posiblemente requiera su contraseña SSH para desbloquear su clave). Si no configuró una clave SSH, deberá ingresar la contraseña de user.


### Espere unos minutos

Aquí hay un pequeño truco: _su StackScript se está ejecutando en este momento_. El script BASH se ejecuta la primera vez que se inicia el VPS. Eso significa que su VPS aún no está listo.

El tiempo total de ejecución es de unos 10 minutos. Por lo tanto, tome un descanso, tome un espresso o relájese durante unos minutos. Hay dos partes del script que toman un tiempo: la actualización de todos los paquetes Debian; y la descarga del código Bitcoin. No deberían tomar más de 5 minutos cada uno, lo que significa que si regresa en 10 minutos, probablemente estará listo para comenzar.

Si está impaciente, puede saltar y `sudo tail -f ~ root / standup.log`, que mostrará el progreso actual de la instalación, como se describe en la siguiente sección.

## Verifique su instalación

Sabrá que stackscrpit está listo cuando la salida del comando `tail` del` standup.log` muestre algo como lo siguiente:

`/root/StackScript - Bitcoin is setup as a service and will automatically start if your VPS reboots and so is Tor
/root/StackScript - You can manually stop Bitcoin with: sudo systemctl stop bitcoind.service
/root/StackScript - You can manually start Bitcoin with: sudo systemctl start bitcoind.service`

En ese momento, su directorio de inicio debería verse así:

```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  laanwj-releases.asc  SHA256SUMS.asc
```

Estos son los diversos archivos que se utilizaron para instalar Bitcoin en su VPS. Ninguno es necesario. Los dejamos en caso de que desee realizar una verificación adicional. De lo contrario, puede eliminarlos:

```
$ rm *
```

### Verificar la configuración de Bitcoin

Para garantizar que la versión de Bitcoin descargada sea válida, StackScript verifica tanto la firma como la suma de comprobación SHA. Debe verificar que ambas pruebas hayan salido bien:

```
$ sudo grep VERIFICATION ~root/standup.log
```

Si ve algo como lo siguiente, todo debería estar bien:

```
/root/StackScript - VERIFICATION SUCCESS / SIG: gpg: Good signature from "Wladimir J. van der Laan (Bitcoin Core binary release signing key) <laanwj@gmail.com>" [unknown]
/root/StackScript - VERIFICATION SUCCESS / SHA: 35ec10f87b6bc1e44fd9cd1157e5dfa4```
```

Sin embargo, si en alguna de esas dos comprobaciones se lee "ERROR DE VERIFICACIÓN", entonces hay un problema. Dado que todo esto está programado, es posible que solo haya habido un cambio menor que haya causado que las comprobaciones del script no funcionen correctamente. (Esto ha sucedido varias veces debido a la existencia del script que se convirtió en Standup). Pero también es posible que alguien esté tratando de persuadirlo a ejecutar una copia falsa del demonio de Bitcoin. Entonces, _¡asegúrese de saber lo que sucedió antes de usar Bitcoin!_

### Leer los registros

También es posible que desee leer todos los archivos de registro de instalación para asegurarse de que no haya ocurrido nada inesperado durante la instalación.

Es mejor revisar el archivo de registro estándar de StackScript, que tiene todos los resultados, incluidos los errores:

`$ sudo more ~root/standup.log`

Tenga en cuenta que es totalmente normal ver _algunos_ errores, particularmente cuando se ejecuta el ruidoso software pgp y cuando varias cosas intentan acceder al dispositivo inexistente `/dev/tty`.

Si, en cambio desea ver un conjunto de información más pequeño, todos los errores deben estar en:

`$ sudo more ~root/standup.err`

Todavía tiene una buena cantidad de información que no son errores, pero es una lectura más rápida.

Si todo se ve bien, felicitaciones, ¡tiene un nodo Bitcoin en funcionamiento usando Linode!

## Lo que hemos hecho

Aunque la imagen predeterminada de Debian 10 que estamos usando para su VPS ha sido modificada por Linode para que sea relativamente segura, su nodo Bitcoin instalado a través de Linode StackScript está configurado con un nivel de seguridad aún mayor. Es posible que encuentre esto limitante o que no pueda hacer las cosas que espera. Aquí hay algunas notas al respecto:

### Servicios protegidos

Su instalación de Bitcoin VPS es mínima y casi no permite ninguna comunicación. Esto se hace a través del sencillo firewall (`ufw`), que bloquea todo excepto las conexiones SSH. También es posible una seguridad adicional para sus puertos RPC, gracias a los servicios ocultos instalados por Tor.

** Ajustando UFW. ** ¡Probablemente debería dejar UFW en su etapa superprotegida! No debería utilizar una máquina Bitcoin para otros servicios, ¡porque todos aumentan su vulnerabilidad! Si decide lo contrario, hay varias [guías de UFW] (https://www.digitalocean.com/community/tutorials/ufw-essentials-common-firewall-rules-and-commands) que le permitirán agregar servicios. Como se anuncia, no es complicado. Por ejemplo, agregar servicios de correo solo requeriría abrir el puerto de correo: `sudo ufw allow 25`. Pero en general no debería eso.

** Ajuste de Tor. ** Es posible que desee proteger mejor servicios tales como SSH. Consulte el [Capítulo 12: Uso de Tor] (https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/12_0_Using_Tor.md) para obtener más información sobre Tor.

### Consolas de comando protegidas

Si definió "IP permitidas por SSH", el acceso SSH (y SCP) al servidor está severamente restringido. `/etc/hosts.deny` no permite que nadie inicie sesión. _No sugerimos cambiar esto_. `/etc/hosts.allow` entonces permite direcciones IP específicas. Simplemente agregue más direcciones IP en una lista separada por comas si necesita ofrecer más acceso.

Por ejemplo:

```
sshd: 127.0.0.1, 192.128.23.1
```

### Actualizaciones automatizadas

Debian también está configurado para actualizarse automáticamente, para asegurarse de estar al día con los parches de seguridad más recientes.


Si por alguna razón quisiera cambiar esto (_no lo sugerimos_), puede hacer esto:

```
echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean false" | debconf-set-selections
```

_Si desea saber más sobre lo que hace el stackscript de Bitcoin Standup, consulte el [Appendix I: Comprensión de Bitcoin Standup] (A1_0_Understanding_Bitcoin_Standup.md) ._

## Jugando con Bitcoin

¡Así que ahora probablemente quiera jugar con Bitcoin!

Pero espere, su demonio de Bitcoin probablemente todavía esté descargando bloques. El comando `bitcoin-cli getblockcount` le dirá cómo se encuentra actualmente:

```
$ bitcoin-cli getblockcount
1771352
```

Si es diferente cada vez que escribe el comando, debe esperar antes de trabajar con Bitcoin. Esto toma de 1 a 6 horas actualmente para una configuración podada, dependiendo de su máquina.

Pero, una vez que se establezca en un número, ¡estará listo para continuar!

Aún así, podría ser hora de algunos expresos más. Pero muy pronto, su sistema estará listo para funcionar y estará leyendo para empezar a experimentar.

## Resumen: Configuración de un VPS de Bitcoin-Core a mano

La creación de un VPS de Bitcoin-Core con los scripts Standup hizo que todo el proceso fuera rápido, simple y (con suerte) sin contratiempos.


## ¿Que sigue?

Tiene algunas opciones para lo que sigue:

 *Lea [StackScript] (https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) para comprender su configuración.

* Lea lo que hace StackScript en el [Appendix I] (A1_0_Understanding_Bitcoin_Standup.md).

* Elija una metodología completamente alternativa en [§2.2: Configuración de una máquina Bitcoin-Core a través de otros medios](02_2_Configurando_Bitcoin_Core_Otros.md).

* Pase a "bitcoin-cli" con el [Capítulo tres: Entendiendo la configuración de Bitcoin](03_0_Entendiendo_su_configuracion_Bitcoin.md).


## Sinopsis: Tipos de instalación de Bitcoin

** Mainnet. ** Esto descargará la totalidad de la cadena de bloques de Bitcoin. Esto es alrededor de 380G de datos (y cada día se aumenta más).

** Mainnet podado. ** Esto reducirá la cadena de bloques que está almacenando a solo los últimos 550 bloques. Si no está minando o ejecutando algún otro servicio de Bitcoin, esto debería ser suficiente para la validación.

** Testnet. ** Esto le da acceso a una cadena de bloques de Bitcoin alternativa donde los Bitcoins en realidad no tienen valor. Está destinado a la experimentación y las pruebas.

** Testnet podado. ** Estos son solo los últimos 550 bloques de Testnet ... porque la cadena de bloques de Testnet también es bastante grande ahora.

** Private Regtest. ** Este es el modo de prueba de regresión, que le permite ejecutar un servidor Bitcoin totalmente local. Permite pruebas aún más profundas. No es necesario podar aquí, porque comenzará desde cero. Esta es una configuración muy diferente, por lo que se trata en el [Appendix 3] (A3_0_Using_Bitcoin_Regtest.md).

