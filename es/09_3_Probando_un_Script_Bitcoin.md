# 9.3: Probando un script de Bitcoin

Bitcoin Scripting permite un control considerable adicional sobre las transacciones de Bitcoin, pero también es algo peligroso. Como describiremos en [§10.1](10_1_Understanding_the_Foundation_of_P2SH.md), los scripts reales están algo aislados de la red Bitcoin, lo que significa que es posible escribir un script y hacer que la red lo acepte incluso si es imposible canjearlo desde ese script. Por lo tanto, debe probar a fondo sus scripts antes de invertir su dinero en ellos.

Por lo tanto, este capítulo describe un método principal para probar los scripts de Bitcoin, que también usaremos para ejemplos ocasionales en el resto de esta sección.

## Instalar btcdeb

Bitcoin Script Debugger (`btcdeb`) de @kallewoof es uno de los métodos más confiables que hemos encontrado para depurar Bitcoin Scripts. Sin embargo, requiere configurar C ++ y algunos otros accesorios en su máquina, por lo que también ofreceremos algunas otras opciones hacia el final de este capítulo.

Primero, debe clonar el repositorio `btcdeb` de GitHub, que también requerirá la instalación `git` si aún no lo tiene.

```
$ sudo apt-get install git
$ git clone https://github.com/bitcoin-core/btcdeb.git
```
Tenga en cuenta que cuando ejecute `git clone` se copiará `btcdeb` en su directorio actual. Hemos elegido hacerlo en nuestro directorio `~standup`.
```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  btcdeb  laanwj-releases.asc  SHA256SUMS.asc
```
Posteriormente debe instalar C++ y otros paquetes requeridos.
```
$ sudo apt-get install autoconf libtool g++ pkg-config make
```
También debe instalar readline, ya que esto hace que el depurador sea mucho más fácil de usar al admitir el historial usando las flechas arriba / abajo, movimiento de izquierda a derecha, autocompletado usando la pestaña y otras buenas interfaces de usuario.
```
$ sudo apt-get install libreadline-dev
```
Ahora está listo para compilar e instalar `btcdeb`:
```
$ cd btcdeb
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```
Después de todo eso, debería tener una copia de `btcdeb`:
```
$ which btcdeb
/usr/local/bin/btcdeb
```

## Utilice btcdeb

`btcdeb` funciona como un depurador estándar. Toma una secuencia de comandos (así como cualquier número de entradas de la pila) como argumento de inicio. A continuación, puede usar `step` para recorrer el script.

Si, en cambio, lo inicia sin argumentos, simplemente obtiene un intérprete donde puede emitir `exec [opcode]` comandos para realizar acciones directamente.

### Utilice btcdeb para un ejemplo de adición

El siguiente ejemplo muestra el uso de `btcdeb` para el ejemplo de adición de la sección anterior, `1 2 OP_ADD`
```
$ btcdeb '[1 2 OP_ADD]' 
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 1 is interpreted as a numeric value; use OP_1 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
3 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
1       | 
2       | 
OP_ADD  | 
#0000 1
```
Muestra nuestro script inicial, que se ejecuta de arriba a abajo, y también muestra lo que se ejecutará a continuación en el script.

Escribimos `step` y avanza un paso tomando el primer elemento del script y empujándolo a la pila:
```
btcdeb> step
		<> PUSH stack 01
script  |  stack 
--------+--------
2       | 01
OP_ADD  | 
#0001 2
```
Y otra vez:
```
btcdeb> step
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
        | 01
#0002 OP_ADD
```
Ahora ejecutamos el `OP_ADD` y hay una gran emoción porque ese código de operación saca los dos primeros elementos de la pila, los suma y luego empuja su suma a la pila.
```
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script  |  stack 
--------+--------
        | 03
```
Y ahí es donde termina nuestra secuencia de comandos, sin nada más que ejecutar y un `03` posicionado en la parte superior de nuestra pila como resultado de la secuencia de comandos.

> **NOTE:** `btcdeb` permite repetir el comando anterior presionando enter. Haremos esto en los siguientes ejemplos, así que no se sorprenda de las indicaciones `btcdeb` sin nada como entrada. Está simplemente repitiendo el comando previo (a menudo `step`).

### Utilice btcdeb para un ejemplo de resta

La sección anterior también incluyó un ejemplo de secuencias de comandos con resta un poco más compleja : `3 2 OP_ADD 4 OP_SUB`.  Así es como se ve:
```
$ btcdeb '[3 2 OP_ADD 4 OP_SUB]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 3 is interpreted as a numeric value; use OP_3 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 4 is interpreted as a numeric value; use OP_4 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
5 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
3       | 
2       | 
OP_ADD  | 
4       | 
OP_SUB  | 
#0000 3
btcdeb> step
		<> PUSH stack 03
script  |  stack 
--------+--------
2       | 03
OP_ADD  | 
4       | 
OP_SUB  | 
#0001 2
btcdeb> 
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
4       | 03
OP_SUB  | 
#0002 OP_ADD
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script  |  stack 
--------+--------
4       | 05
OP_SUB  | 
#0003 4
btcdeb> 
		<> PUSH stack 04
script  |  stack 
--------+--------
OP_SUB  |      04
        | 05
#0004 OP_SUB
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script  |  stack 
--------+--------
        | 01
```
Volveremos a `btcdeb` de vez en cuando, y seguirá siendo una excelente herramienta para probar sus propios scripts.

### Usar el poder de btcdeb

`btcdeb` también tiene algunas funciones más poderosas, como `print` and `stack`, que le muestran el script y la pila en cualquier momento.

Por ejemplo, en la secuencia de comandos anterior, una vez que haya avanzado al comando `OP_ADD`, puede ver lo siguiente:
```
btcdeb> print
    #0000 3
    #0001 2
 -> #0002 OP_ADD
    #0003 4
    #0004 OP_SUB
btcdeb> stack
<01>	02	(top)
<02>	03
```
El uso de estos comandos puede hacer que sea más fácil ver lo que está sucediendo y dónde se encuentra.

## Probar un script en línea

También hay algunos simuladores web que puede utilizar para probar scripts en línea. Pueden ser superiores a una herramienta de línea de comandos al ofrecer una salida más gráfica, pero también encontramos que tienden a tener deficiencias.

En el pasado, hemos intentado brindar pautas detalladas sobre el uso de sitios como [Script Playground](http://www.crmarsh.com/script-playground/) o [Bitcoin Online Script Debugger](https://bitcoin-script-debugger.visvirial.com/), pero se desactualizan y / o desaparecen demasiado rápido para mantenerse al día con ellos.

Suponga que estos depuradores tienen la gran ventaja de mostrar las cosas visual y explícitamente que le dicen si un script tiene éxito (se desbloquea) o falla (permanece bloqueado). Asumimos que tienen desventajas con las firmas, donde muchos de ellos siempre regresan `true` para las pruebas de firmas o tienen mecanismos muy engorrosos para incorporarlas.


## Pruebe un script con Bitcoin

Incluso con una gran herramienta como `btcdeb` o recursos transitorios como los diversos probadores de scripts en línea, no está trabajando con un ambiente real. No puede garantizar que sigan las reglas de consenso de Bitcoin, lo que significa que no puede garantizar sus resultados. Por ejemplo, Script Playground dice explícitamente que ignora un error implícito en las firmas múltiples de Bitcoin. Esto significa que cualquier código multifirma que pruebe con éxito en Script Playground se romperá en el mundo real.

Entonces, la única forma de probar _realmente_ los scripts de Bitcoin es probándolos en Testnet.

Y como se hace eso? Da la casualidad que es el tema del [capítulo 10](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md), que busca introducir estos scripts abstractos en el mundo real de Bitcoin incrustándolos en transacciones P2SH. (Pero incluso entonces, probablemente necesitará una API para impulsar su transacción P2SH a la red Bitcoin, por lo que las pruebas completas seguirán siendo una forma en el futuro).

_Independientemente_ de los otros métodos de prueba que haya utilizado, probar un script en Testnet debería ser su prueba final antes de poner su script en Mainnet. No confíe en que su código sea correcto; no se limite a mirarlo. Ni siquiera confíe en los simuladores o depuradores que ha estado usando. Hacerlo es otra excelente manera de perder fondos en Bitcoin.

## Resumen: prueba de un script de Bitcoin

Debería instalar `btcdeb` como una herramienta de línea de comandos para probar sus scripts de Bitcoin. En el momento de escribir este artículo, produce resultados precisos que pueden recorrer todo el proceso de creación de scripts. También puede buscar en algunos sitios en línea para obtener una representación más visual. Cuando haya terminado, deberá ir a testnet para asegurarse de que todo funciona correctamente, antes de implementarlo de manera más general.

## Que sigue?

Continue con "Introducción a los Scripts de Bitcoin" con nuestro primer ejemplo de la vida real: [§9.4: Scripting a P2PKH](09_4_Scripting_a_P2PKH.md).
