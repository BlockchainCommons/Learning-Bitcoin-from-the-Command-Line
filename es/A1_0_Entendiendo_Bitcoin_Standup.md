# Apéndice I: Entendiendo la Configuración Inicial de Bitcoin

[§2.1: Configurando un VPS Bitcoin-Core con Bitcoin-Standup](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md)
explica el proceso de crear un nodo Bitcoin usando
[Bitcoin-Standup-Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).
El siguiente apéndice explica que hacen las principales secciones del programa.
Usted puede querer seguir junto con 
[Linode Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh)
en otra ventana.

## Paso 1: Nombre del host

Su nombre de host es almacenado en `/etc/hostname` y configurado con el comando
`hostname`. También aparece en `/etc/hosts`.

## Paso 2: Zona horaria

La zona horaria de su host es almacenada en `/etc/timezone`, luego un archivo
apropiado es copiado desde `/usr/share/zoneinfo/` a `/etc/localtime`.

## Paso 3: Actualizando Debian

El gestor de paquetes `apt-get` es usado para actualizar su maquina e instalar
`gnupg`, el generador de números aleatorios `haveged`, y el cortafuegos simple
`ufw`.

Su maquina es configurada automáticamente para estar al día con `echo
"unattended-upgrades unattended-upgrades/enable_auto_updates boolean true" |
debconf-set-selections`.

## Paso 4: Configurando un usuario

Un usuario `standup` es creado, el cual sera usado por sus aplicaciones
Bitcoin. Este también tiene permisos `sudo`, permitiéndole tomar acciones
privilegiadas con esta cuenta.

Si usted suministro una llave SSH, esta le permitirá acceder a su cuenta (de
otra manera, deberá usar una contraseña creada en la configuración).

Si usted suministro una dirección IP, el acceso `ssh` estará limitado a esa
dirección, según `/etc/hosts.allow`.

## Paso 5: Configurando Tor

Tor es instalado para proveer servicios protegidos (ocultos) para acceder a los
comandos RPC de Bitcoin a través de su servidor. Vea 
[§14.1: Verificando Su Configuración Tor](14_1_Verificando_Su_Configuracion_Tor.md) 
para mas información acerca de su configuración Tor.

Si usted suministro un cliente autorizado para los servicios ocultos, el acceso
estará limitado para esa llave, según
`/var/lib/tor/standup/authorized_clients`. Si usted no lo hizo,
[§14.2](14_2_Cambiando_Sus_Servicios_Bitcoin_Ocultos.md) explica como hacerlo
luego.

## Paso 6: Instalando Bitcoin

Bitcoin es instalado en `~standup/.bitcoin`. Su configuración es almacenada en
`~standup/.bitcoin/bitcoin.conf`. 

Asegúrese que las sumas de comprobación verifiquen según
[§2.1](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md), de otra
manera, podría estar expuesto a ataques de la cadena de suministro.

## Paso 7: Instalando codificador QR

Para mantener todo compatible con
[GordianSystem](https://github.com/BlockchainCommons/GordianSystem), un código
QR es creado en `/qrcode.png`. Esto puede leerse desde un cliente QuickConnect,
tal como
[GordianWallet](https://github.com/BlockchainCommons/GordianWallet-iOS)

## Conclusión — Entendiendo la Configuración Inicial de Bitcoin

Bitcoin Standup usa programas para probar e imitar mucha de las funcionalidades
de un [GordianNode](https://github.com/BlockchainCommons/GordianNode-macOS).
Este debe proveerlo con un entorno Bitcoin seguro, construido en las bases de
Bitcoin Core y Tor para comunicaciones RPC.

## ¿Que sigue?

Si usted se encontraba en el proceso de crear un nodo Bitcoin para utilizar en
este curso, usted debería volver a
[§2.1](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md).

Si usted esta leyendo a través de los apéndices, continúe con 
[Apéndice II: Compilando Bitcoin desde la Fuente](A2_0_Compilando_Bitcoin_desde_la_Fuente.md).
