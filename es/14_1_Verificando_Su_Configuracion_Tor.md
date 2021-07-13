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

