# Capítulo 14.2: Cambiar sus servicios ocultos de Bitcoin

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura de advertencias.

Tiene un servicio Tor en funcionamiento, pero con el tiempo es posible que desee restablecerlo o ajustarlo.

## Asegure sus servicios ocultos

Tor le permite limitar qué clientes hablan con sus servicios ocultos. Si aún no autorizó a su cliente durante la configuración de su servidor debe hacer lo siguiente:


1. Solicite su clave pública de autenticación Tor V3 a su cliente. (En [GordianWallet](https://github.com/BlockchainCommons/GordianWallet-iOS), está disponible en el menú de configuración)
2. Vaya al subdirectorio apropiado para su servicio oculto de Bitcoin, que si utilizó Bitcoin Standup es `/var/lib/tor/standup/`.
3. Vaya al subdirectorio `authorized_clients`.
4. Agregue un archivo llamado `[anything].auth`. El `[anything]` puede ser cualquier cosa realmente.
5. Coloque la clave pública (y nada más) en el archivo.

Una vez que haya agregado un archivo `.auth` al subdirectorio `authorized_client`, solo los clientes autorizados podrán comunicarse con ese servicio oculto. Puede agregar ~330 claves públicas diferentes para habilitar diferentes clientes.

## Restabelcer su dirección de cebolla para  `bitcoind`

Si alguna vez desea restablecer su dirección de cebolla para `bitcoind`, simplemente elimine la `onion_private_key` en su directorio de datos, como por ejemplo `~/.bitcoin/testnet`:
```
$ cd ~/.bitcoin/testnet
$ rm onion_private_key 
```
Cuando reinicie, se generará una nueva dirección de cebolla:
```
2020-07-22T23:52:27Z tor: Got service ID pyrtqyiqbwb3rhe7, advertising service pyrtqyiqbwb3rhe7.onion:18333
2020-07-22T23:52:27Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_private_key
```

## Restablezca su dirección de cebolla RPC

Si desea restablecer su dirección de cebolla para el acceso RPC, de manera similar borre el `HiddenServiceDirectory` apropiado y reinicie Tor:
```
$ sudo rm -rf /var/lib/tor/standup/
$ sudo /etc/init.d/tor restart
```

> :warning: **ADVERTENCIA:** Restablecer su dirección de cebolla RPC desconectará cualquier billetera móvil u otros servicios que haya conectado usando la API de Quicklink. Haga esto con extrema precaución.

## Forzar el uso de Tor para `bitcoind`

Finalmente, puede forzar el uso de Tor `bitcoind` agregando lo siguiente a su bitcoin.conf:
```
proxy=127.0.0.1:9050
listen=1
bind=127.0.0.1
onlynet=onion
```
Luego, deberá agregar nodos de semillas basados en cebolla u otros nodos a su configuración, una vez más editando `bitcoin.conf`:
```
seednode=address.onion
seednode=address.onion
seednode=address.onion
seednode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
```
Luego, reinicie `tor` y `bitcoind`.

Ahora debería comunicarse exclusivamente en Tor. Pero, a menos que se encuentre en un estado hostil, este nivel de anonimato probablemente no sea necesario. 
Tampoco es particularmente recomendable: puede disminuir en gran medida su número de pares potenciales, provocando problemas de censura o incluso de correlación. 
También puede ver un retraso. Y esta configuración puede darle una falsa sensación de anonimato que realmente no existe en la red Bitcoin.

> :warning: **ADVERTENCIA:**  Esta configuración no está probada! ¡Úselo bajo su propio riesgo!

## Resumen: cambio de los servicios ocultos de Bitcoin

Probablemente no necesitará engañar con sus servicios de Onion una vez que los haya verificado, pero en caso de que lo haga, aquí le mostramos cómo restablecer una dirección de Tor que se ha visto comprometida o pasar al uso exclusivo de Tor para su `bitcoind`.

## Que sigue??

Continue "Entendiendo Tor" con [14.3: Agregando Servicios SSH Ocultos](14_3_Agregando_Servicios_SSH_Ocultos.md).
