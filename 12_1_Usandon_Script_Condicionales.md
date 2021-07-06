# 12.1: Uso scripts condicionales

Hay un aspecto final de Bitcoin Scripting que es crucial para desbloquear su verdadero poder: los condicionales le permiten crear varias rutas de ejecución.

## Entender Verificar

Ya ha visto un condicional en los scripts: `OP_VERIFY` (0x69). Aparece el elemento superior de la pila y comprueba si es cierto; si no, finaliza la ejecución del script.

Verify generalmente se incorpora a otros códigos de operación. Ya ha visto `OP_EQUALVERIFY` (0xad),` OP_CHECKLOCKTIMEVERIFY` (0xb1) y `OP_CHECKSEQUENCEVERIFY` (0xb2). Cada uno de estos códigos de operación realiza su acción principal (igual, verificar tiempo de bloqueo o secuencia de verificación) y luego realiza una verificación. Los otros códigos de operación de verificación que no ha visto son: `OP_NUMEQUALVERIFY` (0x9d),` OP_CHECKSIGVERIFY` (0xad) y `OP_CHECKMULTISIGVERIFY` (0xaf).

Entonces, ¿cómo es `OP_VERIFY` un condicional? Es el tipo de condicional más poderoso. Usando `OP_VERIFY`, _si_ una condición es verdadera, 
el Script continúa ejecutándose, _de lo contrario_ el Script sale. Así es como verifica las condiciones que son absolutamente necesarias para que 
un script tenga éxito. Por ejemplo, el script P2PKH (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) tiene dos condiciones requeridas: (1) que la clave pública proporcionada coincide con el hash de clave pública; y (2) que la firma proporcionada coincida con esa clave pública. 
Un `OP_EQUALVERIFY` se usa para la comparación de la clave pública luego del hash y el hash de la clave pública porque es una condición absolutamente necesaria. 
No _desea_ que el script continúe si eso falla.

Puede notar que no hay `OP_VERIFY` al final de este script (o de la mayoría de estos), a pesar de que también se requiere la condición final. 
Eso es porque Bitcoin efectivamente hace un `OP_VERIFY` al final de cada Script, para asegurar que el resultado final de la pila sea verdadero. 
No _desea_ hacer un `OP_VERIFY` antes del final del script, ¡porque necesita dejar algo en la pila para ser probado!

## Comprender if / then

El otro condicional importante en Bitcoin Script es el clásico `OP_IF` (0x63) /` OP_ELSE` (0x67) / `OP_ENDIF` (0x68). 
Este es un control de flujo típico: si `OP_IF` detecta una declaración verdadera, ejecuta el bloque debajo de ella; de lo contrario, si hay un `OP_ELSE`, 
lo ejecuta; y `OP_ENDIF` marca el final del bloque final.

> :warning: **ADVERTENCIA:** Estos condicionales también son técnicamente códigos de operación, pero al igual que con los números pequeños, 
> dejaremos el prefijo `OP_` desactivado para mayor brevedad y claridad. Por lo tanto, escribiremos `IF`,`ELSE` y `ENDIF` en lugar de` OP_IF`, `OP_ELSE` y `OP_ENDIF`.

### Comprender if / then realizar pedidos

Hay dos grandes inconvenientes para los condicionales. Hacen que sea más difícil leer y evaluar los guiones si no se tiene cuidado.

Primero, el condicional `IF` verifica la verdad de lo que está _ antes de él_ (es decir, lo que está en la pila), no lo que está después.

En segundo lugar, el condicional `IF` tiende a estar en el script de bloqueo y lo que está comprobando tiende a estar en el script de desbloqueo.

Por supuesto, podría decirse, así es como funciona Bitcoin Script. Los condicionales usan la notación polaca inversa y adoptan el paradigma estándar de desbloqueo / bloqueo, al igual que _todo lo demás_ en Bitcoin Scripting. 
Todo eso es cierto, pero también va en contra de la forma estándar en que leemos los condicionales IF / ELSE en otros lenguajes de programación; por lo tanto, es fácil leer inconscientemente los condicionales de Bitcoin incorrectamente.

Considere el siguiente código: `IF OP_DUP OP_HASH160 <pubKeyHashA> ELSE OP_DUP OP_HASH160 <pubKeyHashB> ENDIF OP_EQUALVERIFY OP_CHECKSIG`.

Mirar condicionales en notación de prefijo puede llevarlo a leer esto como:

```
IF (OP_DUP) THEN

    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 

ELSE 

    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB> 

ENDIF 
 
 OP_EQUALVERIFY 
 OP_CHECKSIG
```

Entonces, podría pensar, si el `OP_DUP` es exitoso, entonces podemos hacer el primer bloque, de lo contrario, el segundo. 
¡Pero eso no tiene ningún sentido! ¡¿Por qué el `OP_DUP` no tendría éxito ?!

Y, de hecho, no tiene ningún sentido, porque accidentalmente leemos la declaración usando la notación incorrecta. La lectura correcta de esto es:

```
IF 
  
    OP_DUP
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 

ELSE 

    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB> 

ENDIF 
 
 OP_EQUALVERIFY 
 OP_CHECKSIG
```

La declaración que se evaluará como "Verdadero" o "Falso" se coloca en la pila _ antes de ejecutar el "IF", luego se ejecuta el bloque de código correcto en función de ese resultado.

Este código de ejemplo en particular está destinado a ser una firma múltiple 1 de 2 de un pobre hombre. 
El propietario de `<privKeyA>` pondría `<signatureA> <pubKeyA> True` en su script de desbloqueo, mientras que el propietario de` <privKeyB> `pondría` <signatureB> <pubKeyB> False` en su script de desbloqueo. 
Ese final de "Verdadero" o "Falso" es lo que marca la declaración "IF" / "ELSE". Le dice al script con qué hash de clave pública se debe verificar, luego el `OP_EQUALVERIFY` y el` OP_CHECKSIG` al final hacen el trabajo real.

### Ejecutar un If / Then Multisig

Con un conocimiento básico de los condicionales de Bitcoin en la mano, ahora estamos listos para ejecutar un script. 
Lo haremos creando una ligera variante de la firma múltiple 1 de 2 de nuestro pobre hombre, donde nuestros usuarios no tienen que recordar si son "Verdaderos" o "Falsos". 
En cambio, si es necesario, el script verifica ambos hash de clave pública, requiriendo que solo uno tenga éxito:

```
OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL
IF

    OP_CHECKSIG

ELSE

    OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
    
ENDIF

```

¡Recuerde su notación polaca inversa! ¡Esa declaración `IF` si se refiere al` OP_EQUAL` antes, no al `OP_CHECKSIG` después!

#### Ejecuta la rama verdadera

Así es como se ejecuta realmente si se desbloquea con `<signatureA> <pubKeyA>`:

```
Script: <signatureA> <pubKeyA> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```

Primero, colocamos constantes en la pila:

```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> ]
```

Luego ejecutamos los primeros comandos obvios, `OP_DUP` y` OP_HASH160` y presionamos otra constante:

```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_DUP
Stack: [ <signatureA> <pubKeyA> <pubKeyA> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_HASH160
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> <pubKeyHashA> ]
```

A continuación, ejecutamos el `OP_EQUAL`, que es lo que va a alimentar el `IF`:

```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashA> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureA> <pubKeyA> True ]
```

Ahora se ejecuta el `IF`, y como hay un` Verdadero`, solo se ejecuta el primer bloque, eliminando el resto:

```
Script: OP_CHECKSIG
Running: True IF
Stack: [ <signatureA> <pubKeyA> ]
```

Y el `OP_CHECKSIG` terminará siendo`True` también:

```
Script: 
Running: <signatureA> <pubKeyA> OP_CHECKSIG
Stack: [ True ]
```

#### Ejecutar la rama falsa

Así es como se ejecuta realmente si se desbloquea con `<signatureB> <pubKeyB>`:

```
Script: <signatureB> <pubKeyB> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```

Primero, colocamos constantes en la pila:

```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> ]
```

Luego ejecutamos los primeros comandos obvios, `OP_DUP` y` OP_HASH160` y presionamos otra constante:

```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashA> ]
```

A continuación, ejecutamos el `OP_EQUAL`, que es lo que va a alimentar el `IF`:

```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashB> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureB> <pubKeyB> False ]
```

¡Alarido! El resultado fue `False` porque` <pubKeyHashB> `no es igual a` <pubKeyHashA> `. Ahora, cuando se ejecuta `IF`, baja a solo la declaración `ELSE`:

```
Script: OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: False IF
Stack: [ <signatureB> <pubKeyB> ]
```

Luego, repasamos todo la relación disparatada nuevamente, comenzando con otro `OP_DUP`, pero eventualmente probando contra el otro` pubKeyHash`:

```
Script: OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashB> ]

Script:OP_CHECKSIG
Running: <pubKeyHashB> <pubKeyHashB> OP_EQUALVERIFY 
Stack: [ <signatureB> <pubKeyB> ]

Script: 
Running: <signatureB> <pubKeyB> OP_CHECKSIG
Stack: [ True ]
```

Esto probablemente no sea tan eficiente como un verdadero multisig de Bitcoin, pero es un buen ejemplo de cómo los resultados introducidos en la pila por pruebas anteriores pueden usarse para alimentar condicionales futuros. 
En este caso, es el fallo de la primera firma lo que le dice al condicional que debe ir a comprobar la segunda.

## Entender otras condiciones 

Hay algunos otros condicionales importantes. El grande es `OP_NOTIF` (0x64), que es lo opuesto a`OP_IF`: ejecuta el siguiente bloque si el elemento superior es `False`. 
Se puede colocar un `ELSE` con él, que como de costumbre se ejecuta si no se ejecuta el primer bloque. Aún termina con `OP_ENDIF`.

También hay un `OP_IFDUP` (0x73), que duplica el elemento de la pila superior solo si no es 0.

Estas opciones se utilizan con mucha menos frecuencia que la construcción principal `IF` /` ELSE` / `ENDIF`.

## Resumen: Uso de condicionales de script

Los condicionales en Bitcoin Script le permiten detener el script (usando `OP_VERIFY`) o elegir diferentes ramas de ejecución (usando`OP_IF`). 
Sin embargo, leer `OP_IF` puede ser un poco complicado. Recuerde que es el elemento empujado a la pila _antes_ de que se ejecute `OP_IF` el que controla su ejecución; ese elemento generalmente será parte del script de desbloqueo (o bien, un resultado directo de los elementos del script de desbloqueo).

> :fire:***¿Cuál es el poder de los condicionales?*** Los condicionales de script son el último bloque de construcción principal en Bitcoin Script. Son lo que se requiere para convertir scripts de Bitcoin simples y estáticos en scripts de Bitcoin complejos y dinámicos que pueden evaluarse de manera diferente en función de diferentes momentos, diferentes circunstancias o diferentes entradas de usuario. 
> En otras palabras, son la base final de los contratos inteligentes.

## ¿Que sigue?

Continúe "Expandiendo los scripts de Bitcoin" con [§12.2: Uso de otros comandos de script](12_2_Using_Other_Script_Commands.md).
