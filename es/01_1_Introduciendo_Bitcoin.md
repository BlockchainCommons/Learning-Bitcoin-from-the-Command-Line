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
