# 17.2: Uso de BIP39 en Libwally

> :information_source: **NOTE:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Uno de los mayores poderes de Libwally es que puede poner al descubierto el trabajo subyacente de generar semillas, claves privadas y, en última instancia, direcciones. Para empezar, es compatible con [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), que es el BIP que define los códigos mnemónicos para Bitcoin - algo que es totalmente no compatible, hasta la fecha, por Bitcoin Core.
> :book: ***Qué es un código mnemónico?*** Las direcciones de Bitcoin (y sus correspondientes claves privadas y semillas subyacentes) son listas largas e ininteligibles de caracteres y números, que no solo son imposibles de recordar, sino también fáciles de escribir mal. Los códigos mnemotécnicos son una solución para esto que permite a los usuarios grabar 12 (o 24) palabras en su idioma, algo que es mucho menos propenso a errores. Estos códigos se pueden utilizar para restaurar completamente una semilla BIP32 que es la base de una cartera HD.

> :book: ***Que es una semilla?*** Mencionamos brevemente las semillas en [§3.5: Entendiendo el Descriptor](03_5_Entendiendo_El_Descriptor.md). Es el número aleatorio que se utiliza para generar una secuencia completa de claves privadas (y por lo tanto direcciones) en una cartera de jerárquica determinista (HD). Volveremos a las semillas en la siguiente sección, que es todo sobre carteras HD y Libwally. Por ahora, basta saber que un código mnemónico BIP39 corresponde a la semilla de una cartera determinística jerárquica BIP32.

## Crear códigos mnemotécnicos

Todas las llaves Bitcoin comienzan con entropía. Este primer uso de Libwally, y su mnemotecnia BIP39, muestra cómo generar entropía y obtener un código mnemotécnico a partir de eso.

> :book: ***Qué es la entropía?*** Entropía es una forma elegante de decir aleatoriedad, pero es una aleatoriedad cuidadosamente medida que se utiliza como la base de un verdadero número aleatorio generado (TRG). Se mide en "bits", con más bits de entropía resultando en más aleatoriedad (y por lo tanto más protección para lo que se está generando). Para Bitcoin, la entropía es la base de su semilla que en una cartera HD genera todas sus direcciones.
Siempre empezará a trabajar con Libwally inicializando la biblioteca y probando los resultados, como se demostró por primera vez en [§17.1](17_1_Configurando_Libwally.md):
```
  int lw_response;

  lw_response = wally_init(0);

  if (lw_response) {

    printf("Error: Wally_init failed: %d\n",lw_response);
    exit(-1);
    
  }
```
Ahora usted esta listo para entropizar.

### Crear entropía

Usando `libsodium`, puede crear entropía con el comando `randombytes_buf` :
```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);
```
Este ejemplo, que será la única forma de usar la biblioteca `libsodium`, crea 16 bytes de entropía. En general, para crear un código mnemónico seguro, debe utilizar entre 128 y 256 bits de entropía, que es de 16 a 32 bytes.

>:warning: **ADVERTENCIA:** De nuevo, tenga la certeza de que usted esta muy cómodo con tu método de generación de entropía antes de usarlo en un programa del mundo real.

### Traducir a un mnemónico

16 bytes de entropía es suficiente para crear un código mnemónico de 12 caracteres, que se hace con la función `bip39_mnemonic_from_bytes` de Libwally:
```
  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
```
Tenga en cuenta que tiene que pasar a lo largo del tamaño del byte, por lo que si fuera a aumentar el tamaño de su entropía, para generar una frase mnemónica más larga, también tendría que aumentar el valor en esta función.

> **NOTA:** ¡Hay listas de palabras mnemónicas para diferentes idiomas! El valor predeterminado es utilizar la lista en inglés, que es la variable `NULL` en estos comandos mnemotécnicos de Libwally, ¡pero también puede solicitar un idioma diferente!

¡Eso es! ¡Ha creado una frase mnemónica!

>:book: ***Cómo se crea la frase mnemónica?*** Usted puede aprender sobre eso en [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), pero si lo prefiere, Greg Walker tiene un [excelente ejemplo](https://learnmeabitcoin.com/technical/mnemonic): básicamente, se añade una suma de comprobación, luego se esconde cada conjunto de 11 bits en una palabra de la lista de palabras. Puede hacer esto con los comandos `bip39_get_wordlist` y `bip39_get_word` si no confía en el comando `bip39_mnemonic_from_bytes`.

### Traducir en una semilla

Hay algunas funciones, tales como `bip32_key_from_seed` (que veremos en la siguiente sección) que requieren que usted tenga la semilla en lugar de la mnemónica. Las dos cosas son funcionalmente idénticas: si tiene la semilla, puede generar la mnemónica, y viceversa.

Si necesita generar la semilla a partir de su mnemónico, sólo tiene que utilizar el comando `bip39_mnemonic_to_seed`:

```
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;
  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
Tenga en cuenta que todas las semillas BIP39 son actuales 512 bytes; sin embargo, tiene que establecer el tamaño de su variable apropiadamente, y pasar a lo largo de ese tamaño a `bip39_mnemonic_to_seed`.

### Imprima su semilla

Si quiere comprobar cómo se ve su semilla en hexadecimal, puede usar la función `wally_hex_from_bytes` para convertir su semilla en un código hexadecimal legible (pero no para la gente):

```
  char *seed_hex;
  wally_hex_from_bytes(seed,sizeof(seed),&seed_hex);
  printf("Seed: %s\n",seed_hex);
```
Si ha hecho todo bien, debería recuperar una semilla de 64 bytes. (Esa es la variable `BIP39_SEED_LEN_512` que usted ha estado lanzando, que define una longitud de semilla predeterminada como 512 bits o 64 bytes.)

> :warning: **WARNING:** Definitivamente debe probar que su longitud de semilla es de 64 bytes de alguna manera, porque es fácil de estropear, por ejemplo usando el tipo de variable equivocado cuando ejecuta `bip39_mnemonic_to_seed`.

## Prueba de código mnemónico

El código completo para generar entropía, generar un mnemónico BIP39, validar el mnemónico y generar una semilla se puede encontrar en el directorio [src](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/17_2_genmnemonic.c). Descargar y compilar:
```
$ cc genmnemonic.c -lwallycore -lsodium -o genmnemonic
```
Entonces usted puede ejecutar la prueba:
```
Mnemonic: parent wasp flight sweet miracle inject lemon matter label column canyon trend
Mnemonic validated!
Seed: 47b04cfb5d8fd43d371497f8555a27a25ca0a04aafeb6859dd4cbf37f6664b0600c4685c1efac29c082b1df29081f7a46f94a26f618fc6fd38d8bc7b6cd344c7
```
## Resumen: Usando BIP39 en Libwally

BIP39 le permite generar un conjunto de 12-24 palabras mnemónicas a partir de una semilla (¡y la biblioteca Libwally también le permite validarla!).

> :fire: ***Cuál es el poder de BIP39?*** Las semillas de bitcoin y las llaves privadas son propensas a todo tipo de pérdidas. Usted escribe mal un solo dígito, y su dinero se ha ido para siempre. Las palabras mnemónicas son una forma mucho más fácil de representar los mismos datos, pero debido a que son palabras en el idioma de elección del usuario, son menos propensas a errores. Asi que el poder de BIP39 es mejorar la accesibilidad, usabilidad y seguridad de Bitcoin.

> :fire: ***Cuál es el poder del BIP39 en Libwally?*** Bitcoind actualmente no admite palabras mnemónicas, por lo que el uso de Libwally puede permitirle generar palabras mnemotécnicas junto con las direcciones contenidas en `bitcoind` (aunque, como veremos en §17.7, en la actualidad requiere alguna solución alterna para importar sus claves a Bitcoin Core).

## ¿Qué es lo siguiente?

Más información sobre "Programar Bitcoin con Libwally" en [17.3: Usar BIP32 en Libwally](17_3_Usando_BIP32_en_Libwally.md).
