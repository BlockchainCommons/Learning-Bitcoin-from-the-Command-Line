# Interludio: Presentación de Bitcoin

Antes de que pueda comenzar a programar sobre Bitcoin (y Lightning), debe tener una comprensión básica de qué son y cómo funcionan. Esta sección proporciona una descripción general. Aparecerán muchas más definiciones dentro del mismo documento con la única intención de sentar las bases.

## Acerca de Bitcoin

Bitcoin es un sistema programático que permite la transferencia de la moneda bitcoin. Está habilitado por un sistema de nodos descentralizado de igual a igual, que incluye nodos completos, billeteras y mineros. De forma conjunta se aseguran de que las transacciones de bitcoin sean rápidas y no repudiadas. Gracias a la naturaleza descentralizada del sistema, estas transacciones también son resistentes a la censura y pueden proporcionar otras ventajas como el uso de seudónimos y la no correlación siempre y cuando se utilicen bien.

Obviamente, Bitcoin es el corazón de este libro, pero también es el creador de muchos otros sistemas, incluidas otras blockchains y Lightning, que se detallan en este tutorial al igual que muchas otras criptomonedas como Ethereum y Litecoin que no son mencionadas.

**_Cómo se transfieren las monedas?_** La moneda Bitcoin no son monedas físicas. En cambio, son una serie interminable de reasignaciones de propiedad. Cuando una persona envía monedas a otra, esa transferencia se almacena como una transacción. Es la transacción la que realmente registra la propiedad del dinero y no un token local en la billetera del propietario o en su máquina.

**_A quién puedes enviar monedas?_** La gran mayoría de las transacciones de bitcoin implican el envío de monedas a personas individuales (o al menos a direcciones de bitcoin individuales). Sin embargo, se pueden utilizar metodologías más complejas para enviar monedas a grupos de personas o scripts. Estas diversas metodologías tienen nombres como P2PKH, multisig y P2SH.

**_Cómo se almacenan las transacciones?_** Las transacciones se combinan en bloques de datos más grandes, que luego se escriben en el libro mayor de la cadena de bloques. Un bloque se construye de tal manera que no se puede reemplazar o reescribir una vez que se han construido varios bloques sobre él (después). Esto es lo que hace que la moneda bitcoin sean irreprochable: el libro de contabilidad global descentralizado donde todo se registra es efectivamente una base de datos permanente e inmutable.

Sin embargo, el proceso de construcción de estos bloques es estocástico: es algo aleatorio, por lo que nunca puede estar seguro de que una transacción se colocará en un bloque específico. También puede haber cambios en los bloques si son muy recientes, pero solo si son muy recientes. Por lo tanto las cosas se vuelven realmente irreprochables (así como permanentes e inmutables) después de un poco de tiempo.

**_Cómo se protegen las transacciones?_** Los fondos contenidos en una transacción de Bitcoin están bloqueados con un rompecabezas criptográfico. Estos acertijos están diseñados para que los pueda resolver fácilmente la persona a la que se enviaron los fondos. Esto se hace utilizando el poder de la criptografía de clave pública. Técnicamente, una transacción está protegida por una firma que demuestra que usted es el propietario de la clave pública a la que se envió una transacción: esta prueba de propiedad es el rompecabezas que se está resolviendo.

Los fondos están aún más protegidos mediante el uso de hashes. Las claves públicas no se almacenan realmente en la cadena de bloques hasta que se gastan los fondos: son los hashes de clave pública los que se guardan. Esto significa que incluso si apareciera la computadora cuántica, las transacciones de Bitcoin permanecerían protegidas por este segundo nivel de criptografía.

**_Cómo se crean las transacciones?_** El corazón de cada transacción de Bitcoin es un lenguaje de secuencias de comandos similar a FORTH que se utiliza para bloquear la transacción. Para volver a gastar el dinero, el destinatario debe proporcionar información específica al guión que demuestra que es el destinatario previsto.

Sin embargo, estos scripts de Bitcoin son el nivel más bajo de funcionalidad de Bitcoin. Gran parte del trabajo de Bitcoin se realiza a través del demonio de Bitcoin llamado `bitcoind`, que es controlado mediante el uso de comandos RPC. Mucha gente envía esos comandos RPC a través del programa `bitcoin-cli`, que proporciona una interfaz aún más simple. Los no programadores ni siquiera se preocupan por estas minucias, sino que utilizan carteras programadas con interfaces más simples.

## Bitcoin - En resumen

Una forma de pensar en Bitcoin es como una secuencia de transacciones atómicas. Cada transacción es autenticada por un remitente con la solución a un rompecabezas criptográfico anterior que se almacenó como un script. La nueva transacción está bloqueada para el destinatario con un nuevo rompecabezas criptográfico que también se almacena como un script. Cada transacción se registra en un libro mayor global inmutable.

## Acerca de la criptografía de clave pública

La criptografía de clave pública es un sistema matemático para proteger los datos y demostrar la propiedad a través de un par asimétrico de claves vinculadas: la clave pública y la clave privada.

Es importante para Bitcoin (y para la mayoría de los sistemas blockchain) porque es la base de gran parte de la criptografía que protege los fondos de criptomonedas. Una transacción de Bitcoin generalmente se envía a una dirección que es hash de una clave pública. Luego, el destinatario puede recuperar el dinero revelando tanto la clave pública como la clave privada.

**_Qué es una clave pública?_** Una clave pública es la clave que se entrega a otras personas. En un sistema típico de clave pública, un usuario genera una clave pública y una clave privada, luego le da la clave pública a todos y cada uno. Esos destinatarios podrán cifrar información con la clave pública, pero no podrán descifrar con la misma clave pública debido a la asimetría del par de claves.

**_Qué es una clave privada?_** Una clave privada está vinculada a una clave pública en un par de claves. En un sistema típico de clave pública, un usuario mantiene segura su clave privada y la usa para descifrar los mensajes que fueron encriptados con su clave pública antes de enviárselos.

**_Qué es una firma?_** Un mensaje (o más comúnmente, un hash de un mensaje) se puede firmar con una clave privada, creando una firma. Cualquiera que tenga la clave pública correspondiente puede validar la firma, lo que verifica que el firmante sea el propietario de la clave privada asociada con la clave pública en cuestión. SegWit es un formato específico para almacenar una firma en la red Bitcoin que encontraremos más adelante.

**_Qué es una función hash?_** Una función hash es un algoritmo que se utiliza con frecuencia en conjunto con la criptografía. Es una forma de asignar una gran cantidad arbitraria de datos a una pequeña cantidad fija de datos. Las funciones hash que se utilizan en criptografía son unidireccionales y resistentes a colisiones, lo que significa que un hash se puede vincular de manera confiable a los datos originales, pero los datos originales no se pueden regenerar a partir del hash. Por tanto, los hashes permiten la transmisión de pequeñas cantidades de datos para representar grandes cantidades de datos, que pueden ser importantes para la eficiencia y los requisitos de almacenamiento.

Bitcoin aprovecha la capacidad de un hash para disfrazar los datos originales, lo que permite ocultar la clave pública real de un usuario, lo que hace que las transacciones sean resistentes a la computación cuántica.

## Criptografía de clave pública: en resumen

Una forma de entender la criptografía de clave pública es: una forma para que cualquier persona proteja los datos de manera que solo una persona autorizada pueda acceder a ellos y de manera que la persona autorizada pueda demostrar que tendrá ese acceso.

## Sobre ECC

ECC significa Criptografía de Curva Elíptica. Es una rama específica de la criptografía de clave pública que depende de cálculos matemáticos realizados utilizando curvas elípticas definidas sobre campos finitos. Es más complejo y más difícil de explicar que la criptografía de clave pública clásica (que utiliza números primos), pero tiene algunas ventajas interesantes.

ECC no recibe mucha atención en este tutorial. Esto se debe a que este tutorial trata sobre la integración con los servidores Bitcoin Core y Lightning, que ya se han ocupado de la criptografía por el lector. De hecho, la intención de este tutorial es que no tengas que preocuparte en absoluto por la criptografía, porque eso es algo de lo que realmente quieres que se ocupen los expertos.

**_Qué es una curva elíptica?_** Una curva elíptica es una curva geométrica que toma la forma `y`<sup>`2`</sup> = `x`<sup>`3`</sup>` + ax + b`. Se elige una curva elíptica específica seleccionando valores específicos de `a` y `b`. Luego, la curva debe examinarse cuidadosamente para determinar si funciona bien para la criptografía. Por ejemplo, la curva secp256k1 utilizada por Bitcoin se define como `a=0` y `b=7`.

Cualquier línea que cruce una curva elíptica lo hará en 1 o 3 puntos ... y esa es la base de la criptografía de curva elíptica.

**_Qué son los campos finitos?_** Un campo finito es un conjunto finito de números, donde toda suma, resta, multiplicación y división se define de modo que da como resultado otros números también en el mismo campo finito. Una forma sencilla de crear un campo finito es mediante el uso de una función de módulo.

**_Cómo se define una curva elíptica sobre un campo finito?_** Una curva elíptica definida sobre un campo finito tiene todos los puntos de su curva extraídos de un campo finito específico. Esto toma la forma: `y`<sup>`2`</sup> `% field-size = (x`<sup>`3`</sup>` + ax + b) % field-size` El campo finito usado para secp256k1 es `2`<sup>`256`</sup>` - 2`<sup>`32`</sup>` - 2`<sup>`9`</sup>` - 2`<sup>`8`</sup>` - 2`<sup>`7`</sup>` - 2`<sup>`6`</sup>` - 2`<sup>`4`</sup>` - 1`.

**_Cómo se utilizan las curvas elípticas en criptografía?_** En la criptografía de curva elíptica, un usuario selecciona un número muy grande (256 bits) como clave privada. Luego agrega un punto base establecido en la curva a sí misma tantas veces. (En secp256k1, el punto base es `G = 04 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798 483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8`, que antepone las dos partes de la tupla con un `04` para decir que el punto de datos está en forma descomprimida. Si prefiere una definición geométrica recta, es el punto "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199") El número resultante es la clave pública. Luego, se pueden usar varias fórmulas matemáticas para demostrar la propiedad de la clave pública, dada la clave privada. Al igual que con cualquier función criptográfica, esta es una trampilla: es fácil pasar de la clave privada a la clave pública y general es imposible pasar de la clave pública a la clave privada.

Esta metodología en particular también explica por qué se utilizan campos finitos en curvas elípticas: asegura que la clave privada no crecerá demasiado. Tenga en cuenta que el campo finito para secp256k1 es un poco más pequeño que 256 bits, lo que significa que todas las claves públicas tendrán 256 bits de longitud, al igual que las claves privadas.

**_Cuáles son las ventajas de ECC?_** La principal ventaja de ECC es que permite la misma seguridad que la criptografía clásica de clave pública con una clave mucho más pequeña. Una clave pública de curva elíptica de 256 bits corresponde a una clave pública tradicional (RSA) de 3072 bits.

### ECC - En resumen

Una forma de pensar en ECC es: una forma de habilitar la criptografía de clave pública que utiliza claves muy pequeñas y matemáticas muy poco conocidas.

### Sobre Cadenas de bloques (Blockchains)

Blockchain es la generalización de la metodología utilizada por Bitcoin para crear un libro mayor distribuido de forma global. Bitcoin es una cadena de bloques al igual que cualquier cantidad de monedas alternativas, cada una de las cuales vive en su propia red y escribe en su propia cadena. Las cadenas laterales como Liquid también son cadenas de bloques. Las cadenas de bloques ni siquiera necesitan tener nada que ver con las finanzas. Por ejemplo, ha habido muchas discusiones sobre el uso de blockchains para proteger identidades soberanas.

Aunque debe comprender los conceptos básicos de cómo funciona una cadena de bloques para comprender cómo funcionan las transacciones en Bitcoin, no necesitará ir más allá. Debido a que las cadenas de bloques se han convertido en una amplia categoría de tecnología, es probable que esos conceptos básicos sean aplicables a muchos otros proyectos en este creciente sector tecnológico. Sin embargo, los comandos de programación específicos aprendidos en este libro no lo serán, ya que son bastante específicos de Bitcoin (y Lightning).

**_Por qué se llama cadena?_** Cada bloque de la cadena de bloques almacena un hash del bloque anterior. Esto une el último bloque actual hasta el "bloque génesis" original a través de una cadena ininterrumpida. Es una forma de crear un orden absoluto entre datos posiblemente conflictivos. Esto también proporciona la seguridad de la cadena de bloques, porque cada bloque se apila sobre uno antiguo, lo que dificulta la recreación del bloque antiguo debido a los algoritmos de prueba de trabajo utilizados en la creación de bloques. Una vez que se han construido varios bloques sobre un bloque de la cadena, es esencialmente irreversible.

**_Qué es una bifurcación (fork)?_** De forma ocasional se crean dos bloques aproximadamente al mismo tiempo. Esto crea temporalmente una bifurcación de un bloque, donde cualquiera de los bloques actuales podría ser el "real". De vez en cuando, una bifurcación puede expandirse para convertirse en dos bloques, tres bloques o incluso cuatro bloques de largo, pero rápidamente se determina que un lado de la bifurcación es el real y el otro queda "huérfano". Esto es parte del proceso estocástico de creación de bloques y demuestra por qué se deben construir varios bloques encima de un bloque antes de que pueda considerarse verdaderamente confiable y no repudiable.

### Blockchain - En resumen

Una forma de pensar en blockchain es: una serie enlazada de bloques de datos inmutables que se remontan al pasado. Otra forma es: una serie enlazada de bloques para ordenar datos de forma absoluta que podrían estar en conflicto.

## Blockchain es adecuado para mí?

Si desea realizar transacciones con bitcoins, obviamente Bitcoin es adecuado para usted. Sin embargo, de manera más generalizada, blockchain se ha convertido en una palabra de moda popular a pesar de que no es una fórmula mágica para todos los problemas técnicos. Dicho esto, hay muchas situaciones específicas en las que blockchain es una tecnología superior.

Las cadenas de bloques probablemente serán útiles si:

* Los usuarios no confían entre sí.
  * Los usuarios existen a través de varias fronteras.
* Los usuarios no confían en las autoridades centrales.
  * Los usuarios quieren controlar sus propios destinos.
* Los usuarios quieren tecnología transparente.
* Los usuarios quieren compartir algo.
  * Y: los usuarios quieren que lo que se comparte se registre de forma permanente.
* Los usuarios quieren una finalidad de transacción rápida.
  * Pero: los usuarios no necesitan la finalidad instantánea de las transacciones.

Las cadenas de bloques probablemente no serán útiles si:

* Los usuarios son de confianza:
  * por ejemplo: las transacciones ocurren dentro de una empresa u organización.
  * por ejemplo: las transacciones son supervisadas por una autoridad central.
* Se requiere secreto:
  * por ejemplo: la información debe ser secreta.
  * por ejemplo: las transacciones deben ser secretas.
  * por ejemplo: los Transactores deben ser secretos.
  * A menos que: Se considere, analice y pruebe cuidadosamente una metodología para el secreto criptográfico.
 * Los usuarios necesitan la finalidad instantánea de la transacción.
  * por ejemplo: en menos de 10 minutos en una red similar a Bitcoin, en menos de 2.5 minutos en una red similar a Litecoin, en menos de 15 segundos en una red similar a Ethereum
  
Tenga en cuenta que todavía puede haber soluciones para algunas de estas situaciones dentro del ecosistema de Bitcoin. Por ejemplo, los canales de pago están abordando rápidamente cuestiones de liquidez y finalidad del pago.

## Sobre Lightning

Lightning es un protocolo de capa 2 que interactúa con Bitcoin para permitir a los usuarios intercambiar sus bitcoins "fuera de la cadena". Tiene ventajas y desventajas sobre el uso de Bitcoin por sí solo.

Lightning Network también es el enfoque secundario de este tutorial. Aunque se trata principalmente de interactuar directamente con Bitcoin (y el `bitcoind`), presta algo de atención a Lightning porque es una tecnología de próxima aparición que probablemente se convierta en una alternativa popular a Bitcoin en un futuro próximo. Este libro adopta el mismo enfoque para Lightning que para Bitcoin: enseña cómo interactuar directamente con un demonio Lightning confiable desde la línea de comandos.

A diferencia de Bitcoin, en realidad existen varias variantes de Lightning. Este tutorial utiliza la implementación de [c-lightning](https://github.com/ElementsProject/lightning) compatible con el estándar como su servidor Lightning de confianza.

**_Qué es un protocolo de capa 2?_** Un protocolo de Bitcoin de capa 2 funciona sobre Bitcoin. En este caso, Lightning funciona sobre Bitcoin, interactuando con este a través de contratos inteligentes.

**_Qué es un canal Lightning?_** Un canal Lightning es una conexión entre dos usuarios de Lightning. Cada uno de los usuarios bloquea una cierta cantidad de bitcoins en la cadena de bloques de Bitcoin utilizando una transacción multi-sig firmada por ambos. Los dos usuarios pueden intercambiar bitcoins a través de su canal Lightning sin tener que escribir en la cadena de bloques de Bitcoin. Solo cuando quieren cerrar su canal liquidan sus bitcoins, según la división final de monedas.

**_Qué es la red Lightning?_** Al juntar varios canales se crea la red Lightning. Esto permite que dos usuarios que no han creado un canal entre ellos intercambien bitcoins usando Lightning: el protocolo forma una cadena de Canales entre los dos usuarios, luego intercambia las monedas a través de la cadena mediante transacciones de tiempo bloqueado.

**_Cuáles son las ventajas de Lightning?_** Lightning permite transacciones más rápidas con tarifas más bajas. Esto crea la posibilidad real de micropagos financiados con bitcoins. También ofrece una mejor privacidad, ya que está fuera de la cadena y solo se escribe el primer y último estado de la transacción en el libro de contabilidad inmutable de Bitcoin.

**_Cuáles son las desventajas de la red Lightning?_** Lightning sigue siendo una tecnología muy nueva y no se ha probado tan a fondo como Bitcoin. No se trata solo de la implementación tecnológica, sino también el diseño en sí mismo se puede actuar de alguna manera inesperada.

### Lightning - en resumen

Una forma de pensar en Lightning es: una forma de realizar transacciones con bitcoins utilizando canales fuera de la cadena entre pares de personas, de modo que solo se tenga que escribir un primer y último estado en la cadena de bloques.

## Resumen - Introduciendo Bitcoin

Bitcoin es un sistema peer-to-peer que permite la transferencia de fondos a través de transacciones bloqueadas con acertijos. Estos acertijos dependen de la criptografía de curva elíptica de clave pública. Cuando generalizas las ideas detrás de Bitcoin, surge la cadena de bloques, una tecnología que actualmente está creciendo e innovando. Cuando expande las ideas detrás de Bitcoin, obtiene protocolos de capa 2 como Lightning, que expanden el potencial de la moneda.

## Que Sigue?

Continue a través de la "Preparación para Bitcoin" con el [Capítulo dos: Configuración de un VPS Bitcoin-Core](02_0_Configurando_un_VPS_Bitcoin-Core.md)



