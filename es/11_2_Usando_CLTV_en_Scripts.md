# 11.2: Usando CLTV en Scripts

`OP_CHECKLOCKTIMEVERIFY` (o CLTV) es el complemento natural a `nLockTime`. Este traslada la idea de transacciones bloqueantes por tiempo absoluto o altura de bloques al reino de los opcodes, permitiendo el bloqueo de UTXOs individuales.

> :warning: **ADVERTENCIA DE VERSIÓN:** CLTV fue lanzado con Bitcoin Core 0.11.2, pero debería estar ampliamente implementado en este momento.

## Recordar nLockTime

Antes de profundizar en CLTV, primero debe recordar como funciona `nLockTime`.

Como fue detallado en [§8.1: Enviando una Transacción con Bloqueo de Tiempo](08_1_Enviando_una_Transaccion_con_Bloqueo_de_Tiempo.md), el bloqueo de tiempo es habilitado configurando dos variables, `nLockTime` y `nSequence`. `nSequence` debe ser configurada a menos de 0xffffffff (habitualmente: 0xffffffff-1), luego el `nLockTime` es interpretado como sigue:

* Si el `nLockTime` es menor que 500 millones, es interpretado como altura de bloque.
* Si el `nLockTime` es 500 millones o mas, es interpretado como una estampa de tiempo UNIX.

Una transacción con `nLockTime` configurado no puede ser gastada (o incluso puesta en la cadena de bloques) hasta que la altura de bloque o tiempo es alcanzado. Mientras tanto, la transacción puede ser cancelada gastando cualquiera de los UTXOs que forman la transacción.

## Comprender el opcode CLTV

`OP_CHECKLOCKTIMEVERIFY` funciona dentro del mismo paradigma de las alturas de bloques absolutas o los tiempos absolutos UNIX, pero este se ejecuta como parte de un Bitcoin Script. Este lee un argumento, el cual puede ser la altura de bloque o el tiempo absoluto UNIX. A través de una complicada metodología, compara esos argumentos con el tiempo actual. Si es anterior, el script falla; si la condición es cumplida, el script continua.

A raíz de que CLTV es solo parte de un script (y presumiblemente parte de una transacción P2SH), una transacción CLTV no es dejada fuera de la mempool como las transacciones `nLockTime`; tan pronto como es verificada, es incluida en la cadena de bloques y los fondos se consideran gastados. El truco es que todos los outputs que fueron bloqueados con CLTV no se encuentran disponibles para _gastar nuevamente_ hasta que CLTV lo permita.

### Comprender el Tiempo Absoluto de CLTV.

Así es como `OP_CHECKLOCKTIMEVERIFY` se usaría como chequeo comparando con el 24 de Mayo de 2017:
```
1495652013 OP_CHECKLOCKTIMEVERIFY
```
Pero, usualmente representaremos esta abstracción de la siguiente manera:
```
<24 de Mayo de 2017> OP_CHECKLOCKTIMEVERIFY
```
O la siguiente:
```
<Tiempo Absoluto> OP_CHECKLOCKTIMEVERIFY
```

### Comprender la Altura Absoluta de Bloque de CLTV

Así es como `OPCHECKLOCKTIMEVERIFY` se chequearía contra la altura de bloque que fue alcanzada el 24 de Mayo de 2017:
```
467951 OP_CHECKLOCKTIMEVERIFY
```
Pero, usualmente, lo abstraeremos de la siguiente manera:
```
<AlturaAbsolutaDeBloque> OP_CHECKLOCKTIMEVERIFY
```

### Comprender Como Realmente Funciona CLTV

La explicación de arriba es suficiente para usar y entender CLTV. Sin embargo, [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki), expone todos los detalles.

Un script bloqueante solo permitirá a una transacción volver a gastar un UTXO bloqueado con CLTV si `OP_CHECKLOCKTIMEVALUE` verifica todo lo siguiente:

* El campo `nSequence` debe ser configurado a menos de 0xffffffff, usualmente 0xfffffffff-1, para evitar conflictos con los bloqueos de tiempo relativos.
* CLTV debe sacar un operando fuera de la pila y este debe ser mayor o igual a 0.
* Tanto el operando de la pila como `nLockTime` deben estar sobre o debajo de los 500 millones, para representar el mismo tipo de bloqueo de tiempo absoluto.
* El valor `nLockTime` debe ser mayor o igual al del operando de la pila.

Entonces, la primera cosa a notar aquí es que `nLockTime` sigue siendo usado con CLTV. Para ser precisos, es requerido en las transacciones que intentan _volver a gastar_ un UTXO bloqueado temporalmente con CLTV. Esto significa, que no es parte de los requerimientos del script. Es solo un temporizador que es usado para liberar fondos, _como esta definido en el script_.

Esto es gestionado a través de un perspicaz entendimiento de como `nLockTime` funciona: un valor para `nLockTime` debe ser siempre elegido para que sea menor o igual al tiempo presente (o altura de bloque), para que así la transacción que vuelve a gastar los fondos pueda ser incluida en la cadena de bloques. Sin embargo, debido a los requerimientos de CLTV, un valor también debe ser elegido para ser mayor o igual al operando CLTV. La unión de estos dos conjuntos es `NULL` hasta que el tiempo actual sea igual al operando CLTV. Después de todo, cualquier valor puede ser elegido entre el operando CLTV y el tiempo actual. Usualmente, usted solo lo configurara al tiempo actual (o bloque).

## Escribir un Script CLTV.

`OP_CHECKLOCKTIMEVERIFY` incluye un `OP_VERIFY`, lo que significa que se detendrá inmediatamente el script si su verificación no tiene éxito. Tiene otra peculiaridad: a diferencia de la mayoría de los comandos de verificación, este deja lo que se esta comprobando en la pila (solo en caso de que quiera hacer mas comparaciones contra el tiempo). Esto significa que un `OP_CHECKLOCKTIMEVERIFY` es usualmente seguido por un `OP_DROP` para limpiar la pila.

El siguiente simple script de bloqueo puede ser usado para transformar el output de una transacción P2PKH a una transacción P2PKH con bloqueo de tiempo.
```
<AñoSiguiente> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <HashLlavePública> OP_EQUALVERIFY OP_CHECKSIG
```

### Codificar un Script CLTV

Por supuesto, como con cualquier Bitcoin Script complejo, este script CLTV sera codificado de hecho en un script P2SH, como se explica en [§10.1: Comprendiendo la base de P2SH](10_1_Comprendiendo_la_Base_de_P2SH.md) y [§10.2: Construyendo la estructura de P2SH](10_2_Construyendo_la_Estructura_de_P2SH.md). 

Asumiendo que `<SiguienteAño>` fuera el entero "1546288031" (hexadecimal en "little-endian": 0x9f7b2a5c) y `<HashLlavePública>` fuera "371c20fb2e9899338ce5e99908e64fd30b789313", este `redeemScript` seria construido como:
```
OP_PUSHDATA (4 bytes) 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 OP_PUSHDATA (20 bytes) 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
```
El cual se traduce a hexadecimal como:
```
04 9f7b2a5c b1 75 76 a9 14 371c20fb2e9899338ce5e99908e64fd30b789313 88 ac
```
O si lo prefiere:
```
$ btcc 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
049f7b2a5cb17576a914371c20fb2e9899338ce5e99908e64fd30b78931388ac
```
El RPC `decodescript` puede verificar que lo hicimos bien:
```
{
  "asm": "1546288031 OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG",
  "type": "nonstandard",
  "p2sh": "2MxANZMPo1b2jGaeKTv9rwcBEiXcXYCc3x9",
  "segwit": {
    "asm": "0 07e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "hex": "002007e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qqlj4hu02ah6ra3f274ah0ttnxpgxcgy6wrghlghps5esf25wfedse4yw4w"
    ],
    "p2sh-segwit": "2N4HTwMjVdm38bdaQ5h3X3VktLY74D2qBoK"
  }
}
```

No vamos a estar mostrando continuamente como todos los Bitcoin Scripts se codifican en transacciones P2SH, pero en su lugar ofrecemos estas abreviaturas: cuando describimos un script, sera un `redeemScript`, el cual sera normalmente serializado y codificado mediante una función hash en el script con bloqueo de tiempo y serializado en el script de desbloqueo; cuando mostremos un procedimiento de desbloqueo, sera la segunda ronda de validaciones, siguiendo la confirmación del hash del script con bloqueo de tiempo.

## Gastar un UTXO CLTV

Para poder gastar un UTXO que esta bloqueado con CLTV, debe configurar `nLockTime` en su nueva transacción, Usualmente, solo querrá configurarlo al tiempo actual o bloque actual, como corresponde. Mientras el tiempo CLTV o la altura de bloque se encuentre en el pasado, y mientras suministre cualquier otro dato requerido por el script de desbloqueo, sera capaz de procesar el UTXO.

En el caso del ejemplo de arriba, el siguiente script de desbloqueo sera suficiente, dado que `nLockTime` fue configurado a algún momento antes de la fecha `<SiguienteAño>`, y que fuera, en efecto, al menos `<SiguienteAño>`:

### Correr un Script CLTV

Para correr el Script, deberá primero concatenar los scripts de desbloqueo y de bloqueo:
```
Script: <firma> <LlavePública> <SiguienteAño> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <HashLlavePública> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]
```
Las tres constantes serán empujadas en la pila:
```
Script: OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <firma> <LlavePública> <SiguienteAño> ]
```
Luego, corre `OP_CHECKLOCKTIMEVERIFY`. Este encuentra algo en la pila y verifica que `nSequence` no sea 0xffffffff. Finalmente, compara `<SiguienteAño>` con `nLockTime`. Si ambos tienen el mismo tipo de representación y si `nLockTime >= <SiguienteAño>`, entonces procesa exitosamente (caso contrario, finaliza el script):
```
Script: OP_DROP OP_DUP OP_HASH160 <HashLlavePública> OP_EQUALVERIFY OP_CHECKSIG
Running: <SiguienteAño> OP_CHECKLOCKTIMEVERIFY
Stack: [ <firma> <LlavePública> <SiguienteAño> ]
```
Luego, `OP_DROP` se deshace de `<SiguienteAño>`, que quedaba rondando:
```
Script: OP_DUP OP_HASH160 <HashLlavePública> OP_EQUALVERIFY OP_CHECKSIG
Running: <SiguienteAño> OP_DROP
Stack: [ <firma> <LlavePública> ]
```
Finalmente, el resto del script se ejecuta, el cual es un chequeo normal de una firma y una llave pública.

## Resumen: Usando CLTV en Scripts

`OP-CHECKLOCKTIMEVERIFY` es un simple opcode que solo recibe un solo argumento, lo interpreta como altura de bloque o una estampa de tiempo UNIX, y solo permite a sus UTXO ser desbloqueadas si la altura de bloque o la estampa de tiempo UNIX esta en el pasado. Configurar `nLockTime` en la transacción de gasto es lo que permite a Bitcoin hacer este cálculo.

> :fire: ***¿Cual es el Poder de CLTV?** Usted ya a visto que los simples bloqueos de tiempo son las bases de los Contratos Inteligentes. CLTV toma el siguiente paso. Ahora puede garantizar que un UTXO no pueda ser gastado antes de cierto tiempo _y_ garantizar que no sera gastado tampoco. En su forma mas simple, esto podría ser usado para crear un fondo al que alguien solo pueda acceder cuando haya alcanzado los 18 años o un fondo de retiro al que solo pueda acceder cuando llegue a los 50. Sin embargo, su verdadero poder surge cuando es combinado con condicionales, donde el CLTV solo se activa ante ciertas situaciones.

## ¿Que sigue?

Continúe "Potenciando el Bloqueo de Tiempo" con [§11.3: Usando CSV en Scripts](11_3_Usando_CSV_en_Scripts.md). 
