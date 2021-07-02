# Interludio: uso de variables de la línea de comandos

La sección anterior demostró una serie de comandos de línea que se utilizan sin ofuscación ni interferencia. Sin embargo, a menudo esa no es la mejor manera de ejecutar Bitcoin desde la línea de comandos. Debido a que está tratando con variables largas, complejas e ilegibles, es fácil cometer un error si está copiando esas variables (si las está escribiendo a mano). Debido a que esas variables pueden significar la diferencia entre recibir y perder dinero real, no querrá cometer errores. Por estas razones, recomendamos encarecidamente utilizar variables de línea de comandos para guardar direcciones, firmas u otras cadenas largas de información siempre que sea razonable hacerlo.

Si está utilizando `bash`, puede guardar información en una variable como esta:
```
$ VARIABLE=$(command)
```
Esa es una simple sustitución de comando, el equivalente a ``VARIABLE=`command` ``. El comando entre paréntesis se ejecuta y luego se asigna a la VARIABLE

Para crear una nueva dirección se vería así:
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress "" legacy)
```
Estos comandos borran la variable NEW_ADDRESS_1, solo para estar seguros, luego la llenan con los resultados del comando `bitcoin-cli getnewaddress`.

Luego puede usar el comando `echo` comando de su shell para ver su (nueva) dirección:
```
$ echo $NEW_ADDRESS_1
mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE
```
Debido a que tiene su dirección en una variable, ahora puede firmar fácilmente un mensaje para esa dirección, sin preocuparse por escribir mal la dirección. ¡Por supuesto, también guardará esa firma en una variable!
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
$ echo $NEW_SIG_1
IPYIzgj+Rg4bxDwCyoPiFiNNcxWHYxgVcklhmN8aB2XRRJqV731Xu9XkfZ6oxj+QGCRmTe80X81EpXtmGUpXOM4=
```
El resto de este tutorial utilizará este estilo de guardar información en variables cuando sea práctico.

> :book: ***Cuándo no es práctico utilizar variables de línea de comandos?*** Las variables de la línea de comandos no son prácticas si necesita usar la información en otro lugar que no sea la línea de comandos. Por ejemplo, es posible que guardar su firma no sea realmente útil si solo va a tener que enviársela a otra persona en un correo electrónico. Además, algunos comandos futuros generarán objetos JSON en lugar de información simple, y las variables no se pueden usar para capturar esa información ... al menos no sin un _poco_ más de trabajo.

## Resumen: uso de variables de la línea de comandos

Las variables de shell se pueden usar para contener cadenas largas de Bitcoin, lo que minimiza las posibilidades de errores.

## Que sigue?

Continúe con la "Comprensión de la configuración de Bitcoin" en [§3.4: Recibiendo una Transacción](03_4_Recibiendo_una_Transaccion.md).
