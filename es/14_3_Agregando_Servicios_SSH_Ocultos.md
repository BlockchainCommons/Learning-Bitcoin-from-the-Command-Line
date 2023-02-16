# Capítulo 14.3: Agregar servicios ocultos SSH

> :information_source: **NOTE:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con advertencias.

Hasta la fecha, ha usado Tor con sus servicios de Bitcoin, pero también puede usarlo para proteger otros servicios en su máquina, mejorando su seguridad y privacidad. Esta sección demuestra cómo mediante la introducción de un servicio `ssh` oculto para iniciar sesión de forma remota usando Tor.

## Crear servicios ocultos SSH

Los nuevos servicios se crean agregándolos al archivo `/etc/tor/torrc`:
```
$ su 
# cat >> /etc/tor/torrc << EOF
HiddenServiceDir /var/lib/tor/hidden-service-ssh/
HiddenServicePort 22 127.0.0.1:22
EOF
# exit
```
Esto es lo que eso significa:

* HiddenServiceDir: Indica que tiene un directorio de servicio oculto con la configuración necesaria en esta ruta.
* HiddenServicePort: Indica el puerto tor que se utilizará; en el caso de SSH, suele ser 22.

Después de agregar las líneas apropiadas a su archiv `torrc`, deberá reiniciar Tor:

```
$ sudo /etc/init.d/tor restart
```
Después del reinicio, su directorio `HiddenServiceDir` debería tener nuevos archivos de la siguiente manera:
```
$ sudo ls -l /var/lib/tor/hidden-service-ssh
total 16
drwx--S--- 2 debian-tor debian-tor 4096 Jul 22 14:55 authorized_clients
-rw------- 1 debian-tor debian-tor   63 Jul 22 14:56 hostname
-rw------- 1 debian-tor debian-tor   64 Jul 22 14:55 hs_ed25519_public_key
-rw------- 1 debian-tor debian-tor   96 Jul 22 14:55 hs_ed25519_secret_key
```
El archivo `hostname` de este directorio contiene su nueva identificación de cebolla:
```
$ sudo cat /var/lib/tor/hidden-service-ssh/hostname
qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
```
Puede conectarse al `ssh` servicio oculto usando `torify` y esa dirección:
```
$ torify ssh standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
The authenticity of host 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion (127.42.42.0)' can't be established.
ECDSA key fingerprint is SHA256:LQiWMtM8qD4Nv7eYT1XwBPDq8fztQafEJ5nfpNdDtCU.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion' (ECDSA) to the list of known hosts.
standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion's password: 
```
## Resumen: Agregar servicios ocultos SSH

Ahora que tiene Tor instalado y sabe cómo usarlo, puede agregar otros servicios a Tor. Simplemente agregue líneas a su `torrc` (en su servidor), luego conéctese con `torify` (en su cliente).

> :fire: ***¿Cuál es el poder de otros servicios ocultos?*** Cada vez que accede a un servicio en su servidor de forma remota, deja huellas en la red. Incluso si los datos están encriptados por algo como SSH (o TLS), los merodeadores en la red pueden ver desde dónde se está conectando, a dónde se está conectando y qué servicio está utilizando. ¿Importa esto? Esta es la pregunta que debe hacer. Pero si la respuesta es "Sí", puede proteger la conexión con un servicio oculto.


## ¿Que sigue?

Para un tipo diferente de privacidad pase a "Usando i2p" en el [Capítulo quince](15_0_Usando_i2p.md).

O, si lo prefiere, hay dos caminos alternativos:

Si desea mantenerse enfocado en Bitcoin, continúe con "Programación con RPC" con [Capítulo 16: Hablar con Bitcoind con C](16_0_Hablando_a_Bitcoind_con_C.md).

O, si no es un programador, puede pasar al [Capítulo 19: Entendiendo la configuración de Lightning](19_0_Entendiendo_Su_Configuracion_Lightning.md) para continuar con su educación de línea de comandos con la red Lightning.
