# 9.2: Ejecución de un script de Bitcoin

Los scripts de Bitcoin pueden no parecer inicialmente tan intuitivos, sin embargo su ejecución es bastante simple, utilizando la notación polaca inversa y una pila.

## Comprender el lenguaje de secuencias de comandos

Un script de Bitcoin tiene tres partes: tiene una línea de entrada; tiene una pila para almacenamiento; y tiene comandos específicos para su ejecución.

### Entender el orden

Los scripts de Bitcoin se ejecutan de izquierda a derecha. Eso suena bastante fácil, porque es la misma forma en que lee. Sin embargo, en realidad podría ser el elemento menos intuitivo de Bitcoin Script, porque significa que las funciones no se ven como usted esperaría. En cambio, _los operandos van antes que el operador_.

Por ejemplo, si estuviera sumando "1" y "2", su script de Bitcoin para eso sería `1 2 OP_ADD`, no "1 + 2". Como sabemos que el operador OP_ADD toma dos entradas, sabemos que las dos entradas anteriores son sus operandos.

>:warning: **ADVERTENCIA:** Técnicamente, todo en Bitcoin Script es un código de operación, por lo que sería más apropiado registrar el ejemplo anterior como OP_1 OP_2 OP_ADD. En nuestros ejemplos, no nos preocupamos por cómo se evaluarán las constantes, ya que ese es un tema de traducción, como se explica en [§8.2: Building the Structure of P2SH](08_2_Building_the_Structure_of_P2SH.md). Algunos escritores prefieren dejar el prefijo "OP" fuera de todos los operadores, pero nosotros hemos optado por no hacerlo.

### Entender la pila

En realidad, no es del todo correcto decir que un operador se aplica a las entradas anteriores. Realmente, un operador aplica a las entradas principales en la pila de Bitcoin.

>:book: ***¿Qué es una pila?*** Una pila es una estructura de datos LIFO (último en entrar, primero en salir). Tiene dos funciones de acceso: Empujar y quitar. Empujar (push) coloca un nuevo objeto en la parte superior de la pila, empujando hacia abajo todo lo que está debajo. Y la fuunció de quitar (Pop) elimina el objeto superior de la pila.

Siempre que Bitcoin Script encuentra una constante, la empuja a la pila. Entonces, el ejemplo anterior de `1 2 OP_ADD` en realidad se vería así cuando se procesó:

```
Script: 1 2 OP_ADD
Stack: [ ]

Script: 2 OP_ADD
Stack: [ 1 ]

Script: OP_ADD
Stack: [ 1 2 ]
```
_Tenga en cuenta que en este y en los siguientes ejemplos, la parte superior de la pila está a la derecha y la parte inferior a la izquierda._
 
### Comprender los códigos de operación

Cuando un script de Bitcoin encuentra un operador, lo evalúa. Cada operador saca cero o más elementos de la pila como entradas, generalmente uno o dos. Luego los procesa de una manera específica antes de devolver cero o más elementos a la pila, generalmente uno o dos.

>:book: ***¿Qué es un código de operación?*** Opcode significa "código de operación". Por lo general, está asociado con el código en lenguaje de máquina y es una función simple (u "operador").

OP_ADD saca dos elementos de la pila (aquí: 2 y luego 1), luego suma y vuelve a colocar el resultado en la pila (aquí: 3).

```
Script:
Running: 1 2 OP_ADD
Stack: [ 3 ]
```

## Desarrollar la complejidad

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

## Comprender el uso del script de Bitcoin

Eso es prácticamente Bitcoin Scripting ... aparte de algunas complejidades de cómo este lenguaje de scripting interactúa con el propio Bitcoin.

### Comprender scriptSig y scriptPubKey

Como hemos visto, cada entrada para una transacción de Bitcoin contiene un `scriptSig` que se usa para desbloquear el `scriptPubKey` del UXTO  asociado. Están _efectivamente_ concatenados juntos, lo que significa que `scriptSig` y `scriptPubKey` se ejecutan juntos, en ese orden.

Por lo tanto, suponga que un UTXO estaba bloqueado con una scriptPubKey que incluía `OP_ADD 99 OP_EQUAL`, lo que requiere como entrada dos números que sumen noventa y nueve, y suponga que el `scriptSig` de `1 98` fue ejecutado para desbloquearlo. Los dos scripts se ejecutarían efectivamente en orden `1 98 OP_ADD 99 OP_EQUAL`.

Evalúe el resultado:
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
Esta abstracción no es del todo precisa: por razones de seguridad, el scriptSig se ejecuta, luego el contenido de la pila se transfiere  al `scriptPubKey` para ejecutarlo, pero es lo suficientemente preciso para comprender cómo la llave del `scriptSig` encaja en la cerradura del `scriptPubKey`.

>:warning **ADVERTENCIA** Lo anterior es un tipo de transacción no estándar. En realidad, no sería aceptado por los nodos que ejecutan Bitcoin Core con la configuración estándar. [§10.1: Building a Bitcoin Script with P2SH](10_1_Understanding_the_Foundation_of_P2SH.md) analiza cómo se podría ejecutar un script de Bitcoin como este, utilizando el poder de P2SH.

### Obtenga los resultados

Bitcoin verificará una transacción y permitirá que el UTXO sea gastado si se cumplen dos criterios durante la ejecución de `scriptSig` y `scriptPubKey`:

   1. La ejecución no se marcó como inválida en ningún momento, por ejemplo, con un OP_VERIFY fallido o el uso de un código de operación deshabilitado.
   2. El elemento superior de la pila al final de la ejecución es verdadero (distinto de cero).
   
En el ejemplo anterior, la transacción se realizaría correctamente porque la pila tiene un `True` en la parte superior. Pero sería igualmente permisible terminar con una pila completa y el número `42` en la parte superior.

## Resumen: ejecución de un script de Bitcoin

Para procesar un script de Bitcoin, un `scriptSig` se ejecuta despúes del `scriptPubKey` que está desbloqueando. Estos comandos se ejecutan en orden, de izquierda a derecha, con constantes que se insertan en una pila y los operadores extraen elementos de esa pila y luego devuelven los resultados. Si el script no se detiene en el medio y si el elemento en la parte superior de la pila al final no es cero, entonces el UTXO está desbloqueado.

## Que sigue?

Continue con la "Introducción a los scripts de Bitcoin" en [§9.3: Testing a Bitcoin Script](09_3_Testing_a_Bitcoin_Script.md).
