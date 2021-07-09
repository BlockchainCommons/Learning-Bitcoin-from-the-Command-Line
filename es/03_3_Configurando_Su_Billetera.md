# 3.3: Configuración de su billetera

Ahora está listo para comenzar a trabajar con Bitcoin. Para empezar, deberá crear una dirección para recibir fondos.

## Crear una dirección.

Lo primero que debe hacer es crear una dirección para recibir pagos. Esto se hace con el comando `bitcoin-cli getnewaddress`. 
Recuerde que si quiere más información sobre este comando, debe escribir `bitcoin-cli help getnewaddress`. 
Actualmente, hay tres tipos de direcciones: legacy y los dos tipos de direcciones SegWit `p2sh-segwit` y `bech32`. 
Si no especifica lo contrario, obtendrá el valor predeterminado, que es actualmente bech32.

Sin embargo, en las próximas secciones usaremos direcciones `legacy`, tanto porque `bitcoin-cli` tuvó algunos problemas iniciales con sus primeras versiones 
de direcciones SegWit como porque otras personas podrían no ser capaces de enviar a direcciones `bech32`. Es poco probable que todo esto sea un problema para 
usted ahora, pero por el momento queremos comenzar con ejemplos de transacciones que están (en su mayoría) garantizados para funcionar.

Puede solicitar la dirección `legacy` con el segundo argumento a `getnewaddress` o con el argumento nombrado `addresstype`.
```
$ bitcoin-cli getnewaddress -addresstype legacy
moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B
```
Tenga en cuenta que esta dirección comienza con una "m" (o, a veces, una "n") para indicar una dirección heredada de testnet. 
Sería un "2" para una dirección P2SH o un "tb1" para una dirección Bech32.

> :link: **TESTNET vs MAINNET:** La dirección de mainnet equivalente comenzaría con un "1" (para Legacy), "3" (para P2SH) o "bc1" (para Bech32).

Toma nota de la dirección. Deberá entregárselo a quien le envíe los fondos.

> :book: ***Qué es una dirección Bitcoin?*** Una dirección de Bitcoin es, literalmente, donde recibe dinero. Es como una dirección de correo electrónico, pero para fondos. Técnicamente, es una clave pública, aunque diferentes esquemas de direcciones lo ajustan de diferentes maneras. Sin embargo, a diferencia de una dirección de correo electrónico, una dirección de Bitcoin debe considerarse de un solo uso: úsela para recibir fondos solo una vez . Cuando desee recibir fondos de otra persona o en otro momento, genere una nueva dirección. Esto se sugiere en gran parte para mejorar su privacidad. Toda la cadena de bloques es inmutable, lo que significa que los exploradores pueden observar largas cadenas de transacciones a lo largo del tiempo, lo que hace posible determinar estadísticamente quiénes son usted y sus contactos, sin importar cuán cuidadoso sea. Sin embargo, si sigue reutilizando la misma dirección, esto se vuelve aún más fácil.

> :book: ***Qué es una billetera Bitcoin?*** Al crear su primera dirección de Bitcoin, también ha comenzado a completar su billetera de Bitcoin. Más precisamente, ha comenzado a llenar el archivo `wallet.dat` en su directorio `~/.bitcoin/testnet3/wallets`. El archivo `wallet.dat` contiene datos sobre preferencias y transacciones, pero lo más importante es que contiene todos los pares de claves que crea: tanto la clave pública (que es la fuente de la dirección donde recibe los fondos) como la clave privada (que es la forma en que gasta esos fondos). En su mayor parte, no tendrá que preocuparse por esa clave privada: `bitcoind` la usará cuando sea necesaria. Sin embargo, esto hace que el archivo `wallet.dat` sea extremadamente importante: si lo pierde, pierde sus claves privadas, y si pierde sus claves privadas, ¡pierde sus fondos!

Con una sola dirección en la mano, puede pasar directamente a la siguiente sección y comenzar a recibir fondos. Sin embargo, antes de llegar allí, discutiremos brevemente los otros tipos de direcciones que encontrará en el futuro y hablaremos sobre algunos otros comandos de billetera que quizás desee usar en el futuro.

### Conociendo sus direcciones de Bitcoin

Hay tres tipos de direcciones de Bitcoin que puede crear con el comando RPC `getnewaddress`. Utilizará una dirección `legacy` (P2PKH) aquí, mientras que se moverá a una dirección SegWit (P2SH-SegWit) o a una dirección Bech32 en [§4.6: Creating a Segwit Transaction](04_6_Creating_a_Segwit_Transaction.md)

Como se señaló anteriormente, la base de una dirección de Bitcoin es una clave pública: alguien envía fondos a su clave pública y luego usa su clave privada para canjearla. 
¿Fácil? Excepto que poner su clave pública allí no es del todo seguro. Por el momento, si alguien tiene su clave pública, entonces no puede recuperar su clave privada (y por lo tanto sus fondos); esa es la base de la criptografía, que utiliza una función de trampilla para garantizar que solo pueda pasar de la clave privada a la pública, y no al revés. 
Pero el problema es que no sabemos lo que deparará el futuro. Excepto que sabemos que los sistemas de criptografía eventualmente se rompen por el avance incesante de la tecnología, por lo que es mejor no poner claves públicas en crudo en la red, para asegurar sus transacciones en el futuro.

Las transacciones clásicas de Bitcoin crearon direcciones P2PKH que agregan un paso criptográfico adicional para proteger las claves públicas.

> :book: ***Qué es una dirección heredada (P2PKH)?*** Esta es una dirección heredada del tipo utilizado por la naciente red de Bitcoin. Lo usaremos en ejemplos para las próximas secciones. Se llama dirección Pay to PubKey Hash (o P2PKH) porque la dirección es un hash de 160 bits de una clave pública. El uso de un hash de su clave pública como su dirección crea un proceso de dos pasos en el que gastar los fondos que necesita para revelar tanto la clave privada como la clave pública, y aumenta la seguridad futura en consecuencia. Este tipo de dirección sigue siendo importante para recibir fondos de personas con software de billetera desactualizado.

Como se describe con más detalle en [§4.6: Creating a Segwit Transaction](04_6_Creating_a_Segwit_Transaction.md), las Guerras de tamaño de bloque de finales del año 2010 dieron como resultado un nuevo tipo de dirección: SegWit. Este es el tipo de dirección preferido actualmente y debería estar completamente integrado en Bitcoin-Core en este punto, pero no obstante lo estamos guardando para §4.6.

SegWit simplemente significa "testigo segregado" y es una forma de separar las firmas de transacciones del resto de la transacción para reducir el tamaño de la transacción. Algunas direcciones de SegWit se filtrarán en algunos de nuestros ejemplos antes de §4.6 como direcciones de cambio, que verá como direcciones que comienzan con "tb". Esto está bien porque `bitcoin-cli` es totalmente compatible con su uso. Pero no los usaremos de otra manera.

Hay dos direcciones de este tipo:

> :book: ***Qué es una dirección P2SH-SegWit*** (también conocida como SegWit anidado)? Esta es la primera generación de SegWit. Envuelve la dirección de SegWit en un hash de script para garantizar la compatibilidad con versiones anteriores. El resultado crea transacciones que son aproximadamente un 25% más pequeñas (con las correspondientes reducciones en las tarifas de transacción).

> :book: ***Qué es una dirección Bech32*** (también conocida como Native SegWit, también conocida como P2WPKH)? Esta es la segunda generación de SegWit. Está completamente descrito en [BIP 173](https://en.bitcoin.it/wiki/BIP_0173). Crea transacciones que son incluso más pequeñas, pero más notablemente también tiene algunas ventajas en la creación de direcciones que son menos propensas a errores humanos y tienen alguna corrección de errores implícita más allá de eso. No es compatible con versiones anteriores como lo era P2SH-SegWit, y es posible que algunas personas pueden no ser capaces de enviar a la misma.

Hay otros tipos de direcciones de Bitcoin, como P2PK (que paga a una clave pública y está en desuso debido a su futura inseguridad) y P2SH (que paga a un Script Hash, y que es utilizada por el SegWit anidado de primera generación direcciones; lo veremos con más detalle en unos pocos capítulos).

## Opcional: Firme un mensaje

A veces, deberá demostrar que controla una dirección de Bitcoin (o más bien, que controla su clave privada). Esto es importante porque le permite a las personas saber que están enviando fondos a la persona adecuada. Esto se puede hacer creando una firma con el comando `bitcoin-cli signmessage` en la forma de `bitcoin-cli signmessage [address] [message]`. Por ejemplo:
```
$ bitcoin-cli signmessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "Hello, World"
HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=
```
Recibirás la firma como devolución.

> :book: ***Qué es una firma?*** Una firma digital es una combinación de un mensaje y una clave privada que luego se puede desbloquear con una clave pública. Dado que existe una correspondencia uno a uno entre los elementos de un par de claves, el desbloqueo con una clave pública demuestra que el firmante controlaba la clave privada correspondiente.

Luego, otra persona puede usar el comando `bitcoin-cli verifymessage` para verificar la firma. Ingresa la dirección en cuestión, la firma y el mensaje:
```
$ bitcoin-cli verifymessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
true
```
Si todos coinciden, entonces la otra persona sabe que puede transferir fondos de manera segura a la persona que firmó el mensaje enviándolo a la dirección.

Si algún "sombrero negro" estuviera inventando firmas, esto produciría un resultado negativo:
```
$ bitcoin-cli verifymessage "FAKEV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
error code: -3
error message:
Invalid address
```

## Opcional: volcar su billetera

Puede parecer peligroso tener todas sus claves privadas irremplazables en un solo archivo. Para eso ejecute `bitcoin-cli dumpwallet`. Le permite hacer una copia de su wallet.dat:
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
```
El archivo `mywallet.txt` en su directorio personal tendrá una larga lista de claves privadas, direcciones y otra información. Eso sí, ¡nunca querrá poner estos datos en un archivo de texto sin formato en una configuración de Bitcoin con fondos reales!

Luego puede recuperarlo con `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet ~/mywallet.txt
```
¡Pero tenga en cuenta que esto requiere un nodo sin podar!
```
$ bitcoin-cli importwallet ~/mywallet.txt
error code: -4
error message:
Importing wallets is disabled when blocks are pruned
```

## Opcional: vea sus claves privadas

A veces, es posible que desee ver las claves privadas asociadas con sus direcciones de Bitcoin. Quizás quiera poder firmar un mensaje o gastar bitcoins desde una máquina diferente. Quizás solo desee hacer una copia de seguridad de ciertas claves privadas importantes. También puede hacer esto con su archivo de volcado, ya que es legible por humanos.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/standup/mywallet.txt"
}
```
Lo más probable es que solo desee ver la clave privada asociada con una dirección específica. Esto se puede hacer con el comando `bitcoin-cli dumpprivkey`.
```
$ bitcoin-cli dumpprivkey "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B"
cTv75T4B3NsG92tdSxSfzhuaGrzrmc1rJjLKscoQZXqNRs5tpYhH
```
Luego, puede guardar esa clave en un lugar seguro, preferiblemente en un lugar que no esté conectado a Internet.

También puede importar cualquier clave privada, desde un volcado de billetera o un volcado de clave individual, de la siguiente manera:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Nuevamente, espere que esto requiera un nodo sin podar. Espere que esto tome un tiempo, ya que `bitcoind` debe volver a leer todas las transacciones pasadas, para ver si hay algunas nuevas a las que debería prestar atención.

> :information_source: **NOTA:** Muchas carteras modernas prefieren [códigos mnemotécnicos](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) para generar las semillas necesarias para crear las claves privadas. Esta metodología no se utiliza en `bitcoin-cli`, por lo que no podrá generar listas de palabras útiles para recordar sus claves privadas.

_Ha estado escribiendo esa dirección Bitcoin que generó muchas veces, mientras estaba firmando mensajes y ahora exportando llaves. Si cree que es una molestia, estamos de acuerdo. También es propenso a errores, un tema que abordaremos en la siguiente sección._

## Resumen: configuración de su billetera

Necesita crear una dirección para recibir fondos. Su dirección se almacena en una billetera, de la que puede hacer una copia de seguridad. También puede hacer mucho más con una dirección, como descargar su clave privada o usarla para firmar mensajes. Pero realmente, crear esa dirección es _todo_ lo que necesita hacer para recibir fondos de Bitcoin.

## Que sigue?

Aléjese de "Comprensión de la configuración de Bitcoin" con [Interludio: uso de variables de línea de comandos](03_3__Interlude_Using_Command-Line_Variables.md)
