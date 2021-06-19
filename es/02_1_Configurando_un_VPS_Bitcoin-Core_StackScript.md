# 2.1: Configuración de un VPS Bitcoin-Core con Bitcoin Standup

Este documento explica cómo configurar un VPS (Virtual Private Sever) para ejecutar un nodo Bitcoin en Linode.com, instalado usando un StackScript automatizado del [proyecto Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts) . Solo necesita ingresar algunos comandos e iniciar su VPS. Casi inmediatamente después de arrancar, encontrará su nuevo nodo Bitcoin descargando bloques felizmente.

> :warning: **ADVERTENCIA:** : No use un VPS para una billetera bitcoin con fondos reales significativos; ver [http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident](http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident). Es genial poder experimentar con transacciones reales de bitcoins en un nodo en vivo sin tener que conectar un servidor autohospedado en una red local. También es útil poder usar un iPhone o iPad para comunicarse a través de SSH con su VPS para realizar algunas tareas simples de bitcoin. Pero se requiere un mayor nivel de seguridad para fondos importantes.

* Si desea comprender lo que hace esta configuración, lea el [Appendix I: Understanding Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md) mientras realiza la instalación.
* Si, en cambio, desea configurar en una máquina que no sea un VPS Linode, como una máquina AWS o una Mac, puede ir a [§2.2: Setting Up a Bitcoin-Core via Other Means](02_2_Setting_Up_Bitcoin_Core_Other.md)
* Si ya tiene un nodo de Bitcoin en ejecución, vaya al [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).

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
