# 11.3: Usando CSV en Scripts

`nLockTime` y `OP_CHECKLOCKTIMEVERIFY` (o CLTV) son solo una lado de la ecuación de los bloqueos de tiempo. En el otro lado están `nSequence` y `OP_CHECKSEQUENCEVERIFY`, las cuales pueden ser usadas para chequear contra tiempos relativos en lugar de tiempos absolutos.

> :warning: **ADVERTENCIA DE VERSIÓN:** CSV fue lanzado con Bitcoin Core 0.12.1, en la primavera de 2016.

## Entienda nSequence

Todo input dentro de una transacción tiene un valor `nSequence` (o si usted prefiere `sequence`). Este a sido una herramienta primaria en la expansión de Bitcoin, como fue discutido previamente en [5.2: Reenviando una Transacción con RBF](05_2_Reenviando_a_Transaccion_con_RBF.md) y [8.1: Enviando una Transacción con un Bloqueo de Tiempo](08_1_Enviando_una_Transaccion_con_un_Bloqueo_de_Tiempo.md), donde fue usado para señalar RBF y `nLockTime`, respectivamente. Sin embargo, hay un uso mas para `nSequence`, descrito por [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki): usted puede usarlo para crear un bloqueo de tiempo relativo en una transacción.

Un bloqueo de tiempo relativo es un bloqueo de tiempo sobre un input especifico de una transacción y es calculado en relación a la fecha de minado del UTXO siendo usado como input. Por ejemplo, si un UTXO fue minado en el bloque #468260 y una transacción fue creada donde al input para este UTXO se le dio un `nSequence` de 100, entonces la nueva transacción no va a poder ser minada hasta el bloque #468360.

Fácil!

> :information_source: **NOTA — SECUENCIA:** Este es el tercer uso del valor `nSequence` en Bitcoin. Cualquier valor `nSequence` sin el trigésimo segundo bit configurado  (1<<31),de 0x00000001 a 0x7ffffffff, sera interpretado como un bloqueo de tiempo relativo. Si `nVersion ≥ 2` (el cual es el valor por defecto desde Bitcoin Core 0.14.0). Debe ser cuidadoso para asegurarse que los bloqueos de tiempo relativo no entran en conflicto con los otros dos usos de `nSequence`, para señalar `nTimeLock` y RBF. `nTimeLock` usualmente se configura a un valor de 0xffffffff-1, donde un bloqueo de tiempo relativo no es permitido; y RBF usualmente configura su valor a "1", donde un bloqueo de tiempo relativo es irrelevante, porque este define un bloqueo de tiempo de 1 bloque.

> En general, recuerda: con un valor de `nVersion` de 2, un valor de `nSequence` de 0x00000001 a 0x7fffffff permite un bloqueo relativo de tiempo, RBF, y `nTimeLock`; un valor de `nSequence` de 0x7fffffff a 0xffffffff-2 permite RBF y `nTimeLock`; un valor `nSequence` de 0xffffffff-1 permite solo `nTimeLock`; un valor `nSequence` igual a 0xffffffff no habilita nada; y `nVersion` puede ser configurada al valor 1 para no permitir los bloqueos de tiempo relativo para cualquier valor de `nSequence`. Uf!

### Crea una transacción CSV con Tiempo de Bloque Relativo

El formato para usar `nSequence` para representar bloqueos de tiempo relativo esta definido en [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) y es levemente mas complejo que solo introducir un numero, como hizo para `nTimeLock`. En su lugar, las especificaciones BIP dividen los cuatro bytes en tres partes:

* Los primeros dos bytes son usados para especificar un bloqueo de tiempo relativo.
* El vigésimo tercer bit es usado para señalar positivamente si el bloqueo se refiere a un tiempo en lugar de a una altura de bloque.
* El trigésimo segundo bit es usado para señalar positivamente si los bloqueos de tiempo relativos están desactivados.

Con todo eso dicho, la construcción de un bloqueo relativo de tiempo basado en el bloque es todavía bastante fácil, porque los dos bits marcados son configurados a un valor `0`, por lo que solo tiene que configurar `nSequence` a un valor entre 1 y 0xffff (65535). La nueva transacción puede ser minada ese numero de bloques después de que el UTXO asociado fue minado.

### Crea una transacción CSV de Tiempo Relativo

Puede en cambio configurar `nSequence` como un tiempo relativo, donde el bloque dura por 512 segundos multiplicado por el valor de `nSequence`.

Con el fin de hacer eso:

1. Decida cuan lejos en el futuro configurar su bloqueo de tiempo relativo.
2. Convierta ese valor a segundos.
3. Divida por 512.
4. Redondee ese valor hacia arriba o abajo y configure `nSequence` a ese valor.
5. Configure el vigésimo tercer bit a verdadero.

Para configurar un tiempo de 6 meses en el futuro, debe primero calcularlo como sigue:
```
$ segundos=$((6*30*24*60*60))
$ nvalor=$(($segundos/512))
```
Luego, conviértalo a hexadecimal:
```
$ valorhexadecimal=$(printf '%x\n' $nvalor)
```
Finalmente, haga un or bit a bit del vigésimo tercer bit dentro del valor hexadecimal que creo:
```
$ valorrelativo=$(printf '%x\n' $((0x$valorhexadecimal | 0x400000)))
$ echo $valorrelativo
4224679
```
Si usted convierte el valor a binario vera que 4224679 = 10000000111011010100111. El vigésimo tercer dígito es configurado a "1"; mientras tanto, los primeros 2 bytes, 0111011010100111, convierta a 76A7 en hexadecimal o 30375 en decimal. Multiplique eso por 512 y obtendrá 15.55 millones de segundos, lo que es, de hecho, 180 días.

## Crea una transacción con Bloqueo de Tiempo Relativo

¿Entonces, usted quiere crear una transacción simple con bloqueo de tiempo relativo? Todo lo que tiene que hacer es emitir una transacción donde el valor `nSequence` en un input es configurado como se muestra arriba: con el valor `nSequence` para ese input configurado de forma tal que los primeros dos bytes definan el bloqueo de tiempo, el vigésimo tercer bit defina el tipo de bloque de tiempo y el trigésimo segundo bit tenga un valor de falso.

Emita la transacción y usted vera que no puede ser minada legalmente hasta que suficientes bloques o suficiente tiempo haya pasado mas allá del tiempo en el que esa UTXO fue minada.

Prácticamente nadie hace eso. En el [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) definiciones para `nSequence` fueron incorporadas dentro de Bitcoin Core al mismo tiempo que [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki), el cual describe el opcode CSV, el equivalente al `nSequence` para el opcode CLTV. Al igual que CLTV, CSV ofrece capacidades mejoradas. Entonces, casi todo el uso de bloqueos de tiempo relativo han sido con el opcode CSV, no con el valor crudo `nSequence` por si mismo.

|                         | Bloqueos de Tiempo Absolutos | Bloqueos de Tiempo Absolutos |
|:-----------------------:|------------------------------|------------------------------|
| **Bloquea transacción** | nTimeLock                    | nSequence                    |
| **Bloquea Output**      | OP_CHECKLOCKTIMEVERIFY       | OP_CHECKSEQUENCEVERIFY       |

## Entienda el Opcode CSV

`OP_SEQUENCEVERIFY` en Bitcoin Scripts funciona bastante parecido a `OP_LOCKTIMEVERIFY`. 

Puede que requiera conservar una UTXO por una centena de bloques mas allá de su minado:
```
100 OP_CHECKSEQUENCEVERIFY
```
O puede hacer un calculo mas complejo que requiera que conserve un UTXO por seis meses, en cuyo caso terminara con un numero mas complejo:
```
4224679 OP_CHECKSEQUENCEVERIFY
```
En este caso usaremos una abreviatura:
```
<+6Meses> OP_CHECKSEQUENCEVERIFY
```

> :warning: **ADVERTENCIA:** Recuerde que un bloqueo de tiempo relativo es un lapso de tiempo desde el minado de el UTXO usado como input. _No_ es un lapso de tiempo después de que la transacción es creada. Si usa un UTXO que ya ha sido confirmado una centena de veces, y usted configura un bloqueo de tiempo de 100 bloques sobre este, sera elegible para minarse inmediatamente. Los bloqueos de tiempo relativos tienen usos bien específicos, pero probablemente no apliquen si su único objetivo es determinar algún tiempo fijado en el futuro. 

### Entienda Como Funciona Realmente CSV

CSV tiene muchas de las sutilezas en su uso como CLTV:

* El campo `nVersion` le debe ser asignado el valor 2 o mayor.
* El campo `nSequence` debe ser menor que 0x80000000.
* Cuando CSV es ejecutado, debe haber un operando en la pila que este entre 0 y 0xf0000000-1.
* Ambos, el operando de la pila y `nSequence` deben tener el mismo valor en el vigésimo tercer bit.
* El valor `nSequence` debe ser mayor o igual al operando de la pila.

Tal como con CLTV, cuando esta gastando un UTXO con CSV, en sus condiciones de bloqueo, debe configurar el campo `nSequence` para habilitar la transacción. Usted usualmente lo configurara al valor exacto del script bloqueante.

## Escriba un Script CSV

Al igual que `OP_CHECKLOCKTIMEVERIFY`, `OP_CHECKSEQUENCEVERIFY` incluye un `OP_VERIFY` implícito y deja sus argumentos en la pila, requiriendo un `OP_DROP` cuando haya terminado.

Un script que bloqueara fondos hasta que hayan pasado seis meses después del minado del input, y que requerirá una firma P2PKH estándar se vera de la siguiente manera:
```
<+6Meses> OP_CHECKSEQUENCEVERIFY OP_DROP OP_DUP OP_HASH160 <HashLlavePublica> OP_EQUALVERIFY OP_CHECKSIG
```

### Codifique un Script CSV

Cuando codifica un script CSV, sea cuidadoso en como codifica el valor entero para el bloqueo de tiempo relativo. Este debe pasarse como un entero de 3 bytes, lo que significa que se ignora el byte mas grande, lo cual podría no activar el bloqueo de tiempo relativo. Dado que es un entero, asegúrese de convertirlo al formato 'little-endian'.

Esto puede realizarse con el shell script `integer2lehex.sh` del capitulo previo.

Para un tiempo relativo de 100 bloques:
```
$ ./integer2lehex.sh 100
Integer: 100
LE Hex: 64
Length: 1 bytes
Hexcode: 0164
```
Aunque eso debería ser rellenado a `000064`, requiriendo un código de `03000064`.

Para un tiempo relativo de 6 meses:
```
$ ./integer2lehex.sh 4224679
Integer: 4224679
LE Hex: a77640
Length: 3 bytes
Hexcode: 03a77640
```

## Gaste una UTXO CSV

Para gastar un UTXO bloqueado con un script CSV, debe configurar el valor `nSequence` de ese input a un valor mas grande que el requerido en el script, pero menor que el tiempo entre el UTXO y el bloque actual. Sí, esto significa que usted necesita saber exactamente el requerimiento en el script bloqueante... pero tiene una copia de `redeemScript`, entonces, si no conoce los requerimientos, deserializa la copia y luego configura el valor `nSequence` al numero que se muestra en esta.

## Resumen: Usando CSV en Scripts

`nSequence` y CSV ofrecen una alternativa a `nLockTime` y CLTV donde usted bloquea una transacción basada en un tiempo relativo desde que el input fue minado, en lugar de basar el bloqueo en un tiempo fijado en el futuro. Estos funcionan casi de la misma manera, mas allá del hecho que el valor de `nSequence` es codificado levemente distinto que el valor de `nLockTime`, con bits específicos significando cosas especificas.

> :fire: ***¿Cual es el poder de CSV?*** CSV no es solo una forma perezosa de bloquear, cuando usted no quiere calcular un tiempo en el futuro. En su lugar, es un paradigma totalmente diferente, un bloqueo que utilizaría si fuera importante crear una duración mínima especifica cuando una transacción es minada y cuando sus fondos pueden ser re-gastados. El uso mas obvio es (una vez mas) para deposito, cuando usted quiere un tiempo preciso entre el input de los fondos y su output. Sin embargo, tiene posibilidades mucho mas poderosas en transacciones fuera de la cadena, incluyendo canales de pago. Esas aplicaciones son por definición construidas sobre transacciones que no han sido puestas en la cadena de bloques, lo cual significa que si son introducidas en la cadena un lapso de tiempo forzado podría ser muy útil. [Contratos por Bloqueos de Tiempo Hasheados](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts) ha sido una de estas implementaciones, potenciando la red de pagos Lightning. Estos son debatidos en [§13.3: Potenciando Bitcoin con Scripts](13_3_Potenciando_Bitcoin_con_Scripts.md).

## ¿Que sigue?

Avanza a través de "Bitcoin Scripting" con [12.0: Expandiendo Scripts Bitcoin](12_0_Expandiendo_Scripts_Bitcoin.md) 
