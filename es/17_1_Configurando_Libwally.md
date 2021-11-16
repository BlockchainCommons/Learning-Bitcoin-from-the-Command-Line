# 17.1: Configuración de Libwally

> :information_source: **NOTE:** En esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Esta primera sección explicará cómo descargar la Biblioteca Libwally C y hacer que funcione.

> :book: ***Qué es Libwally?*** Libwally es una biblioteca de primitivos útil para la creación de billeteras que son multiplataforma y multiplataforma, de modo que las mismas funciones se pueden utilizar en todas partes. Hay [documentos en línea](https://wally.readthedocs.io/en/latest/). Libwally está disponible como parte del proyecto [Elements Project](https://github.com/ElementsProject) de Blockstream.

## Instalar Libwally

Como siempre, usted necesitará algunos paquetes en su sistema:
```
$ sudo apt-get install git
$ sudo apt-get install dh-autoreconf
```
A continuación, usted puede descargar Libwally desde su repositorio Git:
```
$ git clone https://github.com/ElementsProject/libwally-core
```
Posteriormente, usted puede comenzar el proceso de configuración:
```
$ ./tools/autogen.sh
```
Al igual que con `libbitcoinrpc`, es posible que desee instalar esto en `/usr/include` y `/usr/lib` para facilitar su uso. Simplemente modifique la línea apropiada en el programa `configure` :
```
 ac_default_prefix=/usr
--
> ac_default_prefix=/usr/local
```
Después puede terminar su preparación:
```
$ ./configure
$ make
```
Luego, puede verificar que las pruebas están funcionando:
```
$ make check
Making check in src
make[1]: Entering directory '/home/standup/libwally-core/src'
Making check in secp256k1
make[2]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make  check-TESTS
make[3]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make[4]: Entering directory '/home/standup/libwally-core/src/secp256k1'
============================================================================
Testsuite summary for libsecp256k1 0.1
============================================================================
# TOTAL: 0
# PASS:  0
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[3]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Entering directory '/home/standup/libwally-core/src'
make  check-TESTS check-local
make[3]: Entering directory '/home/standup/libwally-core/src'
make[4]: Entering directory '/home/standup/libwally-core/src'
PASS: test_bech32
PASS: test_psbt
PASS: test_psbt_limits
PASS: test_tx
============================================================================
Testsuite summary for libwallycore 0.7.8
============================================================================
# TOTAL: 4
# PASS:  4
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src'
make[3]: Nothing to be done for 'check-local'.
make[3]: Leaving directory '/home/standup/libwally-core/src'
make[2]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Entering directory '/home/standup/libwally-core'
make[1]: Nothing to be done for 'check-am'.
make[1]: Leaving directory '/home/standup/libwally-core'
```
Por último, puede instalar:
```
$ sudo make install
```

## Preparación para Libwally

Entonces, ¿cómo usar Libwally en un programa? Como de costumbre, usted necesitara incluir archivos apropiados y enlazar bibliotecas apropiadas para su código.

### Incluir los archivos

Hay un número considerable de archivos de inclusión posibles:
```
$ ls /usr/include/wally*
/usr/include/wally_address.h  /usr/include/wally_bip39.h   /usr/include/wally_elements.h  /usr/include/wally_script.h
/usr/include/wally_bip32.h    /usr/include/wally_core.h    /usr/include/wally.hpp	  /usr/include/wally_symmetric.h
/usr/include/wally_bip38.h    /usr/include/wally_crypto.h  /usr/include/wally_psbt.h	  /usr/include/wally_transaction.h
```
Afortunadamente, los nombres de archivo coinciden en gran medida con las secciones de [docs](https://wally.readthedocs.io/en/latest/), por lo que debería ser capaz de incluir los archivos correctos en función de lo que está haciendo, después de incluir el ubicuo `wally_core.h`.

### Enlazar las bibliotecas

También necesitará enlazar las bibliotecas apropiadas:
```
$ ls /usr/lib/libsecp* /usr/lib/libwally*
/usr/lib/libsecp256k1.a   /usr/lib/libwallycore.la  /usr/lib/libwallycore.so. 0
/usr/lib/libsecp256k1.la  /usr/lib/libwallycore.so  /usr/lib/libwallycore.so. 0.0.0
```
Mayormente, usted usara `libwallycore`.

## Configurar un programa Libwally

En comparación con algunas de las bibliotecas anteriores, Libwally es ridículamente fácil de inicializar:
```
lw_response = wally_init(0);
```
Y cuando termine, hay una práctica función para limpiar cualquier memoria asignada:
```
wally_cleanup(0);
```
En ambos casos, el argumento es para flags, pero actualmente tiene el valor `0`.

## Prueba un programa de prueba de Libwally

El directorio src contiene [testwally.c](../src/17_1_testwally.c), que sólo muestra cómo funcionan las funciones de inicialización y limpieza.

Puede compilarlo de la siguiente manera:
```
$ cc testwally.c -lwallycore -o testwally
```
Después se puede ejecutar:
```
$ ./testwally
Startup: 0
```
El valor "Startup" es el retorno de `wally_init`. El valor `0` puede parecer inicialmente desalentador, pero es lo que quiere ver:
```
include/wally_core.h:#define WALLY_OK      0 /** Success */
```
## Instalar Libsodium

También debe instalar Libsodium para obtener acceso a un generador de números aleatorios de alta calidad con fines de prueba.
> :warning: **WARNING:** La generación de números aleatorios puede ser uno de los mayores puntos de vulnerabilidad en cualquier software de Bitcoin. Si usted lo hace mal, expone a sus usuarios a ataques que podrían terminar como claves de Bitcoin inseguras, y esto no es un [problema teórico](https://github.com/BlockchainCommons/SmartCustodyBook/blob/master/manuscript/03-adversaries.md#adversary-systemic-key-compromise). Blockchain.Info alguna vez generó incorrectamente el 0.0002% de sus claves, lo que dio lugar a la pérdida temporal de 250 Bitcoins. Conclusión: asegúrese de estar totalmente cómodo con su generación de números aleatorios. Podría ser Libsodium, o podría ser un método TRNG aún más robusto.
Puede descargar [Libsodium](https://download.libsodium.org/libsodium/releases/) y seguir las instrucciones en [Instalación de Libsodium](https://doc.libsodium.org/installation) para instalar.

En primer lugar, descomprimir:
```
$ tar xzfv /tmp/libsodium-1.0.18-stable.tar.gz 
```
Luego, ajuste el archivo `configure` exactamente como tiene las otras bibliotecas hasta la fecha:
```
< ac_default_prefix=/usr
---
> ac_default_prefix=/usr/local
```
Finalmente, `make`, `check`, e `install`
```
$ make
$ make check
...
============================================================================
Testsuite summary for libsodium 1.0.18
============================================================================
# TOTAL: 77
# PASS:  77
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
...
$ sudo make install
```
Este curso sólo usará `libsodium` para un pequeño (¡pero crucial!) bit de generación de entropía, pero esté atento a ello en la siguiente sección.

## Resumen: Configuración de Libwally

Al instalar Libwally (y Libsodium) se incluyen dependencias, bibliotecas y obtiene acceso a una serie de funciones criptográficas y de cartera, que pueden complementar sus bibliotecas RPC y ZMQ (o su línea de comandos `bitcoin-cli`).

Entonces, ¿qué puede hacer ahora?   De eso es lo que se trata en el siguiente capítulo.

## ¿Qué es lo siguiente?

Más información sobre "Programar Bitcoin con Libwally" en [17.2: Usar BIP39 en Libwally](17_2_Usando_BIP39_en_Libwally.md).
