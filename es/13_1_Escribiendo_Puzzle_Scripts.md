# 13.1: Escribir guiones de rompecabezas

Los scripts de Bitcoin realmente _no_ tienen que depender del conocimiento de una clave secreta. En cambio, pueden ser rompecabezas de cualquier tipo.

## Escribir guiones de álgebra simples

Nuestro primer script real, de [§9.2: Ejecución de un script de Bitcoin](09_2_Ejecutando_un_Script_Bitcoin.md) fue un acertijo alegbraico. Ese script de Bitcoin, `OP_ADD 99 OP_EQUAL`, podría haberse descrito alternativamente como` x + y = 99`.

Este tipo de script no tiene mucha aplicabilidad en el mundo real, ya que es demasiado fácil reclamar los fondos. Pero, un concurso de resolución de acertijos que distribuya polvo de Bitcoin podría ofrecerlo como un entretenimiento divertido.

Más notablemente, la creación de acertijos alegebraicos le brinda una buena comprensión de cómo funcionan las funciones aritméticas en Bitcoin Script.

### Escribe un script multiplicador

Bitcoin Script tiene varios códigos de operación que se desactivaron para mantener la seguridad del sistema. Uno de ellos es `OP_MUL`, que habría permitido la multiplicación ... pero está deshabilitado.

Entonces, ¿cómo escribirías una función algebraica como `3x + 7 = 13`?

La respuesta más obvia es `OP_DUP`, el número ingresado desde el script de bloqueo dos veces. Luego puede presionar el `7` y seguir sumando hasta obtener el total. El script de bloqueo completo se vería así: `OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL`.

Así es como se ejecutaría si se ejecutara con el script de desbloqueo correcto de `2`:

```
Script: 2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ ]

Script: OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 ]

Script: OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 ]

Script: 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 2 ]

Script: OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 2 2 7 ]

Script: OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 7 OP_ADD
Stack: [ 2 2 9 ]

Script: OP_ADD 13 OP_EQUAL
Running: 2 9 OP_ADD
Stack: [ 2 11 ]

Script: 13 OP_EQUAL
Running: 2 11 OP_ADD
Stack: [ 13 ]

Script: OP_EQUAL
Stack: [ 13 13 ]

Script: 
Running: 13 13 OP_EQUAL
Stack: [ True ]
```
O si prefiere `btcdeb`:

```
$ btcdeb '[2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
9 op script loaded. type `help` for usage information
script   |  stack 
---------+--------
2        | 
OP_DUP   | 
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0000 2
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   | 02
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0001 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   |      02
7        | 02
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0002 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
7        |      02
OP_ADD   |      02
OP_ADD   | 02
OP_ADD   | 
13       | 
OP_EQUAL | 

#0003 7
btcdeb> step
		<> PUSH stack 07
script   |  stack 
---------+--------
OP_ADD   |      07
OP_ADD   |      02
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 09
script   |  stack 
---------+--------
OP_ADD   |      09
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0005 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0b
script   |  stack 
---------+--------
OP_ADD   |      0b
13       | 02
OP_EQUAL | 

#0006 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0d
script   |  stack 
---------+--------
13       | 0d
OP_EQUAL | 
#0007 13
btcdeb> step
		<> PUSH stack 0d
script   |  stack 
---------+--------
OP_EQUAL |      0d
         | 0d

#0008 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script   |  stack 
---------+--------
         | 01
```

### Escribe un sistema de ecuaciones

¿Qué pasaría si quisiera escribir un sistema de ecuaciones, como `x + y = 3`,` y + z = 5` y `x + z = 4`? Un poco de álgebra te dice que las respuestas son `x = 1`,` y = 2` y `z = 3`. Pero, ¿cómo lo escribe?

Lo más obvio es que después de que el redentor ingrese los tres números, necesitará dos copias de cada número, ya que cada número entra en dos ecuaciones diferentes. `OP_3DUP` se encarga de eso y da como resultado que`x y z x y z` esté en la pila. Sacar dos elementos a la vez le dará `y z`,` z x` y `x y`. ¡Voila! Estas son las tres ecuaciones, ¡así que solo necesita sumarlas y probarlas en el orden correcto! Aquí está el script completo: `OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL`.

Así es como se ejecuta con el script de desbloqueo correcto de `1 2 3`:

```
Script: 1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ ]

Script: OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 ]

Script: OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 1 2 3 OP_3DUP
Stack: [ 1 2 3 1 2 3 ]

Script: 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 2 3 OP_ADD
Stack: [ 1 2 3 1 5 ]

Script: OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 1 5 5 ]

Script: OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 5 5 OP_EQUALVERIFY
Stack: [ 1 2 3 1 ] — Does Not Exit

Script: 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 3 1 OP_ADD
Stack: [ 1 2 4 ]

Script: OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 4 4 ]

Script: OP_ADD 3 OP_EQUAL
Running: 4 4 OP_EQUALVERIFY
Stack: [ 1 2 ] — Does Not Exit

Script: 3 OP_EQUAL
Running: 1 2 OP_ADD
Stack: [ 3 ]

Script: OP_EQUAL
Stack: [ 3 3 ]

Script: 
Running: 3 3 OP_EQUAL
Stack: [ True ]
```

Aquí está en `btcdeb`:

```
$ btcdeb '[1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
13 op script loaded. type `help` for usage information
script         |  stack 
---------------+--------
1              | 
2              | 
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0000 1
btcdeb> step
		<> PUSH stack 01
script         |  stack 
---------------+--------
2              | 01
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0001 2
btcdeb> step
		<> PUSH stack 02
script         |  stack 
---------------+--------
3              |      02
OP_3DUP        | 01
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0002 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_3DUP        |      03
OP_ADD         |      02
5              | 01
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0003 OP_3DUP
btcdeb> step
		<> PUSH stack 01
		<> PUSH stack 02
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_ADD         |      03
5              |      02
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script         |  stack 
---------------+--------
5              |      05
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0005 5
btcdeb> step
		<> PUSH stack 05
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      05
OP_ADD         |      05
4              |      01
OP_EQUALVERIFY |      03
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0006 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      01
4              |      03
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0007 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 04
script         |  stack 
---------------+--------
4              |      04
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0008 4
btcdeb> step
		<> PUSH stack 04
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      04
OP_ADD         |      04
3              |      02
OP_EQUAL       | 01

#0009 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0010 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script         |  stack 
---------------+--------
3              | 03
OP_EQUAL       | 

#0011 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_EQUAL       |      03
               | 03

#0012 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script         |  stack 
---------------+--------
               | 01
```
> :WARNING: **ADVERTENCIA** `btcdeb` no solo es útil para proporcionar visualización de estos scripts, sino también para verificar los resultados. Efectivamente, nos equivocamos la primera vez, probando las ecuaciones en el orden incorrecto. Así de fácil es cometer un error financieramente fatal en un script de Bitcoin, y es por eso que cada script debe ser probado.

## Escriba scripts computacionales simples

Aunque los scripts de acertijos son triviales, en realidad pueden tener una utilidad en el mundo real si desea realizar un cálculo colectivo. Simplemente crea un script que requiere la respuesta al cálculo y envía fondos a la dirección P2SH como recompensa. Se quedará ahí hasta que alguien dé la respuesta.

Por ejemplo, Peter Todd [ofreció recompensas](https://bitcointalk.org/index.php?topic=293382.0) por resolver ecuaciones que demuestran colisiones para algoritmos criptográficos estándar. Aquí estaba su script para confirmar una colisión SHA1: `OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL`. Requiere dos entradas, que serán los dos números que colisionen.

Así es como funciona con respuestas correctas.

Primero, completamos nuestra pila:

```
Script: <numA> <numB> OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ ]

Script: OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ <numA> <numB> ]

Script: OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_2DUP
Stack: [ <numA> <numB> <numA> <numB> ]
```

Luego, nos aseguramos de que los dos números no sean iguales, saliendo si lo son:

```
Script: OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_EQUAL
Stack: [ <numA> <numB> False ]

Script: OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: False OP_NOT
Stack: [ <numA> <numB> True ]

Script: OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: True OP_VERIFY
Stack: [ <numA> <numB> ] — Does Not Exit
```

Ahora creamos dos SHA:

```
Script: OP_SWAP OP_SHA1 OP_EQUAL
Running: <numB> OP_SHA1
Stack: [ <numA> <hashB> ]

Script: OP_SHA1 OP_EQUAL
Running: <numA> <hashB> OP_SWAP
Stack: [ <hashB> <numA> ]

Script: OP_EQUAL
Running: <numA> OP_SHA1
Stack: [ <hashB> <hashA> ]
```

Finalmente, veremos si coinciden.

```
Script: 
Running: <hashB> <hashA> OP_EQUAL
Stack: [ True ]
```

Este es un buen script porque muestra un uso cuidadoso de la lógica (con `OP_NOT` y` OP_VERIFY`) y un buen uso de las funciones de pila (con `OP_SWAP`). Es un gran ejemplo de una función del mundo real. Y es del mundo muy real. Cuando [SHA-1 se rompió](https://shattered.io/), 2.48 BTC se liberaron rápidamente de la dirección, con un valor total de aproximadamente $ 3,000 en ese momento.

`btcdeb` se puede ejecutar para probar la colisión (y el script):

```
btcdeb $ btcdeb '[255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017f46dc93a6b67e013b029aaa1db2560b45ca67d688c7f84b8c4c791fe02b3df614f86db1690901c56b45c1530afedfb76038e972722fe7ad728f0e4904e046c230570fe9d41398abe12ef5bc942be33542a4802d98b5d70f2a332ec37fac3514e74ddc0f2cc1a874cd0c78305a21566461309789606bd0bf3f98cda8044629a1 255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017346dc9166b67e118f029ab621b2560ff9ca67cca8c7f85ba84c79030c2b3de218f86db3a90901d5df45c14f26fedfb3dc38e96ac22fe7bd728f0e45bce046d23c570feb141398bb552ef5a0a82be331fea48037b8b5d71f0e332edf93ac3500eb4ddc0decc1a864790c782c76215660dd309791d06bd0af3f98cda4bc4629b1 OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL']
```

Las otras [recompensas] de Peter Todd (https://bitcointalk.org/index.php?topic=293382.0) permanecen sin reclamar en el momento de escribir este artículo. Todos están escritos de la misma manera que en el ejemplo SHA-1 anterior.

## Comprenda las limitaciones de los scripts de rompecabezas

Los scripts de rompecabezas son excelentes para examinar más a fondo los scripts de Bitcoin, pero solo los verá en el mundo real si tienen pequeñas cantidades de fondos o si están destinados a ser canjeados por usuarios muy hábiles. Hay una razón para esto: no son seguras.

Aquí es donde cae la seguridad:

Primero, cualquiera puede redimirlos sin saber mucho de un secreto. Tienen que tener el `redeemScript`, que ofrece cierta protección, pero una vez que lo hacen, ese es probablemente el único secreto que es necesario, a menos que su rompecabezas sea _realmente_ difícil, como un rompecabezas computacional.

En segundo lugar, la redención real no es segura. Normalmente, una transacción de Bitcoin está protegida por la firma. Debido a que la firma cubre la transacción, nadie en la red puede reescribir esa transacción para enviarla a su dirección sin invalidar la firma (y por lo tanto la transacción). Eso no es cierto con transacciones cuyas entradas son solo números. Cualquiera podría tomar la transacción y reescribirla para permitirles robar los fondos. Si pueden hacer su transacción en un bloque antes que la suya, ganan y usted no obtiene el dinero del rompecabezas. Hay soluciones para esto, pero implican extraer el bloque usted mismo o hacer que un grupo de confianza lo extraiga, y ninguna de esas opciones es racional para un usuario promedio de Bitcoin.

Sin embargo, las recompensas criptográficas de Peter Todd demuestran que los scripts de rompecabezas tienen alguna aplicación en el mundo real.

## Resumen: escritura de guiones de rompecabezas

Los scripts de rompecabezas son una gran introducción a los scripts de Bitcoin más realistas y complejos. Demuestran el poder de las funciones matemáticas y de pila en Bitcoin Script y cómo se pueden combinar cuidadosamente para crear preguntas que requieren respuestas muy específicas. Sin embargo, su uso en el mundo real también está limitado por los problemas de seguridad inherentes a las transacciones de Bitcoin no firmadas.

> :fire: ***¿Cuál es el poder del guión de rompecabezas?*** A pesar de sus limitaciones, los guiones de rompecabezas se han utilizado en el mundo real como premios por recompensas computacionales. Cualquiera que pueda resolver un rompecabezas complejo, cuya solución presumiblemente tenga algún impacto en el mundo real, puede ganar la recompensa. Si pueden conservarlo es otra cuestión.

## ¿Que sigue?

Continúe "Diseñando scripts de Bitcoin reales" con [§13.2: Escritura de scripts complejos de varias firmas](13_2_Escribiendo_Scripts_Multifirma_Complejos.md)
