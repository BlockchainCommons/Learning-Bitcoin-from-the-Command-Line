# 9.2: Ejecución de un Script de Bitcoin

Los scripts de Bitcoin pueden no parecer inicialmente tan intuitivos, pero su ejecución es bastante simple, utilizando notación polaca inversa y una pila.

## Comprender el Lenguaje de Secuencias de Comandos

Un script de Bitcoin tiene tres partes: tiene una línea de entrada; tiene una pila para almacenamiento; y tiene comandos específicos para su ejecución.

### Entender el Order

Los scripts de Bitcoin se ejecutan de izquierda a derecha. Eso suena bastante fácil, porque es la misma forma en que lee. Sin embargo, en realidad podría ser el elemento menos intuitivo de Bitcoin Script, porque significa que las funciones no se ven como esperaba. En cambio, _los operandos van antes que la operadora._

Por ejemplo, si estuviera sumando "1" y "2", su Bitcoin Script para eso sería `1 2 OP_ADD`, _no_ "1 + 2". Como sabemos que el operador OP_ADD toma dos entradas, sabemos que las dos entradas anteriores son sus operandos.

> :warning: **ADVERTENCIA:** Técnicamente, todo en Bitcoin Script es un código de operación, por lo que sería más apropiado registrar el ejemplo anterior como `OP_1 OP_2 OP_ADD`. En nuestros ejemplos, no nos preocupamos por cómo se evaluarán las constantes, ya que ese es un tema de traducción, como se explica en [§8.2: Building the Structure of P2SH](08_2_Building_the_Structure_of_P2SH.md). Algunos escritores prefieren dejar el prefijo "OP" también fuera de todos los operadores, nosotros hemos optado por no hacerlo.

### Entender la Pila

En realidad, no es del todo correcto decir que un operador aplica a las entradas enfrente que lo. En realidad, no es del todo correcto decir que un operador se aplica a las entradas anteriores.

> :book: ***Que es un pila?*** Una pila es una estructura de datos LIFO (last-in-first-out o último en entrar, primero en salir). Tiene dos funciones de accesso: push y pop. Push coloca un nuevo objeto en la parte superior de la pila, empujando hacia abajo todo lo que está debajo. Pop elimina el objeto superior de la pila.

Siempre que Bitcoin Script encuentra una constante, la empuja a la pila. Por lo tanto, el ejemplo anterior de `1 2 OP_ADD` se vería así cuando se procesó:
```
Script: 1 2 OP_ADD
Stack: [ ]

Script: 2 OP_ADD
Stack: [ 1 ]

Script: OP_ADD
Stack: [ 1 2 ]
```
_Tenga en cuenta que en este y en los siguientes ejemplos, la parte superior de la pila está a la derecha y la parte inferior a la izquierda._
 
### Comprender los Opcodes

Cuando un script de Bitcoin encuentra un operador, lo evalúa. Cada operador pops (saca) cero o más elementos de la pila como entradas, generalmente uno o dos. Luego los procesa de una manera específica antes de devolver cero o más elementos a la pila, generalmente uno o dos.

> :book: ***Que es un Opcode?*** Opcode significa "operation code" o "código de operación". Por lo general, se asocia con el código en lenguaje de máquina y es una función simple (u "operador").

OP_ADD pops dos elementos de la pila (aquí: 2 y luego 1), luego suma y vuelve a colocar el resultado en la pila (aquí: 3).
```
Script:
Running: 1 2 OP_ADD
Stack: [ 3 ]
```

## Para Construir Complejidad

Se crean scripts más complejos ejecutando más comandos en orden. Deben evaluarse cuidadosamente de izquierda a derecha, para que pueda comprender el estado de la pila a medida que se ejecuta cada nuevo comando. Cambiará constantemente, como resultado de operadores anteriores:
```
Script: 3 2 OP_ADD 4 OP_SUB
Stack: [ ]

Script: 2 OP_ADD 4 OP_SUB
Stack: [ 3 ]

Script: OP_ADD 4 OP_SUB
Stack: [ 3 2 ]

Script: 4 OP_SUB
Running: 3 2 OP_ADD
Stack: [ 5 ]

Script: OP_SUB
Stack: [ 5 4 ]

Script: 
Running: 5 4 OP_SUB
Stack: [ 1 ]
```

## Comprender el Uso de Bitcoin Script

Eso es prácticamente Bitcoin Scripting ... además de algunas complejidades sobre cómo este lenguaje de secuencias de comandos interactúa con el propio Bitcoin.

### Comprender scriptSig y scriptPubKey

Como hemos visto, cada entrada para una transacción de Bitcoin contiene un `scriptSig` que se usa para desbloquear el `scriptPubKey` para el UTXO asociado. Están _efectivamente_ concatenados juntos `scriptSig` y `scriptPubKey` se ejecutan juntos, en ese orden.

Entonces, suponga que un UTXO fue bloqueado con una `scriptPubKey` que lee `OP_ADD 99 OP_EQUAL`, requiriendo como entrada dos números que suman noventa y nueve, y suponga que el `scriptSig` de `1 98` se ejecutó para desbloquear. Los dos scripts se ejecutarían efectivamente en orden como `1 98 OP_ADD 99 OP_EQUAL`.

Evaluar el resultado:
```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: []

Script: 98 OP_ADD 99 OP_EQUAL
Stack: [ 1 ]

Script: OP_ADD 99 OP_EQUAL
Stack: [ 1 98 ]

Script: 99 OP_EQUAL
Running: 1 98 OP_ADD
Stack: [ 99 ]

Script: OP_EQUAL
Stack: [ 99 99 ]

Script: 
Running: 99 99 OP_EQUAL
Stack: [ True ]
```
Esta abstracción no es del todo precisa: por razones de seguridad, se ejecuta el `scriptSig`, luego el contenido de la pila se transfiere para que se ejecute el `scriptPubKey` pero es lo suficientemente preciso para comprender cómo la clave de `scriptSig` encaja en la cerradura de `scriptPubKey`.

> :warning: **ADVERTENCIA** El anterior es un tipo de transacción no estándar. En realidad, los nodos que ejecutan Bitcoin Core con la configuración estándar no lo aceptarían. [§10.1: Building a Bitcoin Script with P2SH](10_1_Understanding_the_Foundation_of_P2SH.md) analiza cómo en realidad _podría_ ejecutar un Bitcoin Script como este, utilizando el poder de P2SH.

### Obtenga los Resultados

Bitcoin verificará una transacción y permitirá que el UTXO sea respetado si se cumplen dos criterios al ejecutar `scriptSig` y `scriptPubKey`:

   1. La ejecución no se marcó como inválida en ningún momento, por ejemplo, con un OP_VERIFY fallido o el uso de un opcode deshabilitado.
   2. El elemento superior de la pila al final de la ejecución es verdadero (distinto de cero).
   
En el ejemplo anterior, la transacción se realizaría correctamente porque la pila tiene un `True` en la parte superior. Pero, sería igualmente permisible terminar con una pila completa y el número `42` en la parte superior.

## Resumen: Ejecución de un Bitcoin Script

Para procesar un script de Bitcoin, se ejecuta un `scriptSig` seguido de la `scriptPubKey` que está desbloqueando. Estos comandos se ejecutan en orden, de izquierda a derecha, con constantes que se insertan en una pila y los operadores extraen elementos de esa pila y luego devuelven los resultados a ella. Si el script no se detiene en el medio y si el elemento en la parte superior de la pila al final no es cero, entonces el UTXO está desbloqueado.

## Que Sigue?

Continúe "Introducción a los Bitcoin Scripts" con [§9.3: Testing a Bitcoin Script](09_3_Testing_a_Bitcoin_Script.md).
