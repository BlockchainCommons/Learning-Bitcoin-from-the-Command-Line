# 10.2: Construyendo la Estructura de P2SH

En la sección anterior, analizamos la teoría de cómo crear transacciones P2SH para contener Scripts de Bitcoin. La práctica real de hacerlo es _mucho más difícil_, pero en aras de la exhaustividad, lo veremos aquí. Probablemente esto no sea algo que haría sin una API, por lo que si se vuelve demasiado intimidante, tenga en cuenta que voleremos a los scripts prístinos y de alto nivel en un momento.

## Crear una Script de Bloqueo

Cualquier transacción P2SH comienza con un script de bloqueo. Este es el tema de los capítulos 9 y 11-12. Puede utilizar cualquiera de los métodos de Bitcoin Script que se describen en el mismo para crear cualquier tipo de script de bloqueo, siempre que el `redeemScript` serializado resultante sea de 520 bytes o menos. 

> :book: ***¿Por qué los scripts P2SH están limitados a 520 bytes?*** Como ocurre con muchas cosas en Bitcoin, la respuesta es la compatibilidad con versiones anteriores: la nueva funcionalidad debe construirse constantemente dentro de las viejas limitaciones del sistema. En este caso, 520 bytes es el máximo que se puede insertar en la pila a la vez. Dado que todo el código de canje se incluye en la pila como parte del proceso de canje alcanza ese límite.

## Serializar un Script de Bloqueo de la Manera Difícil

Después de crear un script de bloqueo, debe serializarlo antes de que pueda ingresarse en Bitcoin. Este es un proceso de dos partes. Primero, debe convertirlo en código hexadecimal, luego debe transformar ese hexadecimal en binario.

### Crea el Código Hex

Crear el código hexadecimal necesario para serializar un script es una traducción simple y algo lo suficientemente complejo como para ir más allá de cualquier script de shell que probablemente escriba. Este paso es una de las principales razones por las que necesita una API para crear transacciones P2SH.

Puede crear un código hexadecimal siguiendo el script de bloqueo y convirtiendo cada elemento en un comando hexadecimal de un byte, posiblemente seguido de datos adicionales, según la guía de la [página de Bitcoin Wiki Script](https://en.bitcoin.it/wiki/Script):

* Los operadores se traducen al byte correspondiente para ese código de operación
* Las constantes 1-16 se traducen a códigos de operación 0x51 a 0x61 (OP_1 a OP_16)
* La constante -1 se traduce a código de operación 0x4f (OP_1NEGATE)
* Otras constantes están precedidas por los códigos de operación 0x01 a 0x4e (OP_PUSHDATA, con el número que especifica cuántos bytes insertar)
   * Los enteros se traducen a hexadecimal usando notación little-endian signed-magnitude.

### Traducir Enteros

Los números enteros son la parte más problemática de la traducción de un script de bloqueo.

Primero, debe verificar que su número esté entre -2147483647 y 2147483647, el rango de enteros de cuatro bytes cuando se usa el byte más significativo para firmar.

En segundo lugar, debe traducir el valor decimal a haxadecimal y rellenarlo con un número par de dígitos. Esto se puede hacer con el comando `printf`:
```
$ integer=1546288031
$ hex=$(printf '%08x\n' $integer | sed 's/^\(00\)*//')
$ echo $hex
5c2a7b9f
```
En tercer lugar, debe agregar un byte inicial adicional de `00` si el dígito superior es "8" o más, para que el número no se interprete como negativo.
```
$ hexfirst=$(echo $hex | cut -c1)
$ [[ 0x$hexfirst -gt 0x7 ]] && hex="00"$hex
```
En cuatro lugar, debe traducir el hex de big-endian (el byte menos significativo al final) a little-endian (el byte menos significativo primero). Puede hacer esto con el comando `tac`:
```
$ lehex=$(echo $hex | tac -rs .. | echo "$(tr -d '\n')")
$ echo $lehex
9f7b2a5c
```
Además, siempre necesita saber el tamaño de cualquier dato que coloque en la pila, de modo que pueda precederlo con el código de operación adecuado. Puede recordar que cada dos caracteres hexadecimales es un byte. O puede user `echo -n` canalizado a `wc -c`, y dividirlo por la mitad:
```
$ echo -n $lehex | wc -c | awk '{print $1/2}'
4
```
Con todo ese procidimiento complejo, sabría que podría traducir el entero 1546288031 en un opcode `04` (para insertar cuatro bytes en la pila) seguido de `9f7b2a5c` (la representación hex little-endian de 1546288031).

Si en cambio tuviera un número negativo, necesitaría (1) hacer sus cáclulos en el valor absoluto del número, luego (2) bit a bit o 0x80 para su resultado final de little-endian result. Por ejemplo, `9f7b2a5c`, que es 1546288031, se convertiría en `9f7b2adc`, que es -1546288031:
```
$ neglehex=$(printf '%x\n' $((0x$lehex | 0x80)))
$ echo $neglehex
9f7b2adc
```
### Transformar el Hexadecimal en Binario

Para completar su serialización, traduzca el código hexadecimal a binario. En la línea de comando, esto solo requiere una simple invocación de `xxd -r -p`. Sin embargo, es probable que desee hacerlo como parte de una única tubería que también aplicará un hash al script ...

## Ejecute el Script de Conversión de Enteros

Un script complejo para cambiar un entero entre -2147483647 y 2147483647 a una representación de little-endian signed-magnitude se puede encontrar en el [directorio de código src](src/10_2_integer2lehex.sh). Puede descargarlo como `integeer2lehex.sh`.

> :warning: **ADVERTENCIA:** Esta script no se ha comprobado de forma exhaustiva. Se va a usarlo para crear scripts de bloqueo reales, debe asegurarse de verificar y probar sus resultados.

Asegúrese de que los permisos del script sean correctos:
```
$ chmod 755 integer2lehex.sh
```
Luego puede ejecutar el script de la siguiente manera:
```
$ ./integer2lehex.sh 1546288031
Integer: 1546288031
LE Hex: 9f7b2a5c
Length: 4 bytes
Hexcode: 049f7b2a5c

$ ./integer2lehex.sh -1546288031
Integer: -1546288031
LE Hex: 9f7b2adc
Length: 4 bytes
Hexcode: 049f7b2adc
```

## Analizar un P2SH Multifirma

Para comprender mejor este proceso, aplicaremos ingeniería inversa al P2SH multifirma que creamos en [§6.1: Envío de una Transacción a un Multifirma](06_1_Sending_a_Transaction_to_a_Multisig.md). Eche un vistazo al `redeemScript` que usó, que ahora sabe que es la versión serializada en hex del script de bloqueo:
```
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
Puede traducir esto de nuevo a Script a mano usando la [página de Bitcoin Wiki Script](https://en.bitcoin.it/wiki/Script) como referencia. Solo mire un byte (dos caracteres hex) de datos a la vez, a menos que un comando OP_PUSHDATA le indique que mire más (un opcode en el rango de 0x01 a 0x4e).

Todo el guión se dividirá de la siguiente manera:
```
52 / 21 / 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 / 21 / 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 / 52 / ae
```
Esto es lo que significan las partes individuales:

* 0x52 = OP_2
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 = los siguientes 33 bytes (public-key hash)
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 = los siguientes 33 bytes (public-key hash)
* 0x52 = OP_2
* 0xae = OP_CHECKMULTISIG

En otras palabras, ese `redeemScript` era una traducción de `2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG`. Regresaremos a este script en [§10.4: Creación de una Script Multifirma](10_4_Scripting_a_Multisig.md) cuando detallemos exactamente cómo funcionan las multifirmas dentro del paradigma P2SH.

Si desea una mano mecánica con este tipo de traducción en el futuro, puede usar `bitcoin-cli decodescript`:
```
$ bitcoin-cli -named decodescript hexstring=522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
{
  "asm": "2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG",
  "reqSigs": 2,
  "type": "multisig",
  "addresses": [
    "mmC2x2FoYwBnVHMPRUAzPYg6WDA31F1ot2",
    "mhwZFJUnWqTqy4Y7pXVum88qFtUnVG1keM"
  ],
  "p2sh": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "segwit": {
    "asm": "0 6fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "hex": "00206fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qdl5lg5wvakuwgzgtsgku4ktn6qug5da5ez0artk5s5gs4hk2k25szvjky9"
    ],
    "p2sh-segwit": "2NByn92W1vH5oQC1daY69F5sU7PEStKKQBR"
  }
}
```
Es especialmente útil para verificar su trabajo cuando está serializando.

## Serializar un Script de Bloqueo de Forma Sencilla

Cuando instaló `btcdeb` en [§9.3](09_3_Testing_a_Bitcoin_Script.md) también instaló `btcc` que se puede usar para seralizar scripts de Bitcoin:
```
$ btcc 2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
Eso es mucho más fácil que averiguarlo a mano!

También considere el Python [Compilador de Script de Transacciones](https://github.com/Kefkius/txsc), que traduce de ida y vuelta.

## Hash un Script Serializado

Después de haber creado un script de bloqueo y de serializarlo, el tercer paso para crear una transacción P2SH es aplicar un hash al script de bloqueo. Como se señalo anteriormente, un hash OP_HASH160 de 20 bytes se crea mediante una combinación de un hash SHA-256 y un hash RIPEMD-160. El hash de un script serializado requiere dos comandos: `openssl dgst -sha256 -binary` hace el hash SHA-256 y genera un binario que se enviará a través de la tuberia, luego `openssl dgst -rmd160` toma ese flujo binario, hace un hash RIPEMD-160, y finalmente genera un código hexadecimal legible por humanos.

Aquí está la tubería completa, incluida la transformación anterior del script serializado hexadecimal en binario:
```
$ redeemScript="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= a5d106eb8ee51b23cf60d8bd98bc285695f233f3
```
## Crear un Transacción P2SH

La creación de su hash de 20 bytes solo le brinda el hash en el centro de un script de bloqueo P2SH. Aún debe juntarlo con los otros códigos de operación que crean una transacción P2SH estándar: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL`.

Dependiendo de su API, es posible que pueda ingresar esto como un `scriptPubKey` de estilo `asm` para su transacción, o puede que tenga que traducirlo también al código `hex`. Si tiene que traducir, use los mismos métodos descritos anteriormente para "Crear el Código Hex" (o use `btcc`), lo que da resultado `a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387`.

 Tenga en cuenta que la `hex scriptPubKey` para la transacción P2SH Script _siempre_ comenzará con un `a914`, que es el `OP_HASH160` seguido de un `OP_PUSHDATA` de 20 bytes (hex: `0x14`); y _siempre_ terminará con un `87`, que es un `OP_EQUAL`. Entonces, todo lo que tiene que hacer es colocar su script de canje con hash entre esos números.

## Resumen: Comprensión de la Base de P2SH

En realidad, la creación del script de bloqueo P2SH se sumerge en las entrañas de Bitcoin más que nunca. Aunque es útil saber cómo funciona todo esto a un nivel muy bajo, lo más probable es que tenga una API que se encargue de todo el trabajo pesado por usted. Su tarea será simplemente crear el script de Bitcoin para hacer el bloqueo ... que es el tema principal de los capítulos 9 y 11-12.

## ¿Que Sigue?

Continúe "Incrustando Bitcoin Scripts" con [§10.3: Ejecución de un Bitcoin Script con P2SH](10_3_Running_a_Bitcoin_Script_with_P2SH.md).
