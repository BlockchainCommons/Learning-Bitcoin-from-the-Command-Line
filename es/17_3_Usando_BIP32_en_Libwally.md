# 17.3: Uso de BIP32 en Libwally

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

En [§17.2](17_2_Usando_BIP39_en_Libwally.md), usted fue capaz de usar entropía para generar una semilla y su mnemónico relacionado. Como usted puede recordar de [§3.5: Entendiendo el Descriptor](03_5_Entendiendo_El_Descriptor.md), una semilla es la base de un Monedero Jerárquico Determinista (HD), donde esa sola semilla puede ser usada para generar muchas direcciones. Entonces, ¿cómo se pasa de la semilla a las direcciones reales? Ahí es donde entra [BIP32](https://en.bitcoin.it/wiki/BIP_0032).

## Crear una raíz HD

Para crear una dirección HD se requiere comenzar con una semilla, y luego ir hacía abajo por la jerarquía hasta el punto en el que se crean las direcciones:
```c
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);

  char *mnem = NULL;# 
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
  
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
Como puede ver, necesitará decirle qué versión de la clave devolver, en este caso `BIP32_VER_TEST_PRIVATE`, una clave de testnet privada.

> :link: **TESTNET vs MAINNET:** En mainnet, en cambio, se pide `BIP32_VER_MAIN_PRIVATE`.

### Generar xpub & xprv

Siempre que tenga una llave en la mano, puede convertirla en claves xpub o xprv para distribución con el comando `bip32_key_to_base58`. Solo tiene que decirle si usted quiere una clave `PRIVATE` (xprv) o `PUBLIC` (xpub):

```c
  char *xprv;
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PRIVATE, &xprv);

  char *xpub;  
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PUBLIC, &xpub);
```
## Entender la jerarquía

Antes de ir más lejos, es necesario entender cómo funciona la jerarquía de una cartera HD. Como se discute en [§3.5](03_5_Entendiendo_El_Descriptor.md), una ruta de derivación describe el árbol que se sigue para llegar a una clave jerárquica, por lo que `[0/1/0]` es el 0º hijo del 1er hijo del 0º hijo de una clave raíz. A veces parte de esa derivación está marcada con `'`s o `h`s para mostrar derivaciones endurecidas, que aumentan la seguridad: `[0'/1'/0']`.

Sin embargo, para las carteras HD, cada uno de esos niveles de la jerarquía se utiliza de una manera muy específica. Esto se definió originalmente en [BIP44](https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki) y más tarde se actualizó para Segwit en [BIP84].

En conjunto, una ruta de derivación BIP32 se define para tener cinco niveles:
1. **Proósito.** Normalmente se establece en `44'` o `84'`, dependiendo del BIP que se esté siguiendo.
2. **Moneda.** Para bitcoins en MAINNET, esto es `0'`, para testnet es `1'`.
3. **Cuenta.** Una cartera puede contener varias cuentas discretas, empezando por `0'`.
4. **Cambio.** Las direcciones externas (para distribución) se establecen en `0`, mientras que las direcciones internas (para cambio) se establecen en `1`.
5. **Índice.** La enésima dirección de la jerarquía, comenzando con `0`.

Así que en testnet, la dirección cero para una dirección externa para la cuenta cero para monedas de testnet usando los estándares BIP84 es `[m/84'/1'/0'/0/0]`. Esa es la dirección que usted estara creando momentáneamente.

> :link: **TESTNET vs MAINNET:** Para mainnet, eso sería `[m/84'/0'/0'/0'/0]`

### Entender la jerarquía en el núcleo de Bitcoin

Usaremos la jerarquía anterior para todas las claves de HD en Libwally, pero tenga en cuenta que este estándar no es utilizado por Bitcoin Core `bitcoin-cli`, que en su lugar utiliza `[m/0'/0'/0']` para la dirección externa 0º y `[m/0'/1'/0']` para la dirección de cambio 0º.

## Generar una dirección

Para generar una dirección, tiene que excavar a través de toda la jerarquía.

### Generar una clave de cuenta

Una manera de hacer esto es usar la función `bip32_key_from_parent_path_alloc` para desplegar varios niveles de una jerarquía. Incrusta los niveles en una matriz:
```c
  uint32_t path_account[] = {BIP32_INITIAL_HARDENED_CHILD+84, BIP32_INITIAL_HARDENED_CHILD+1, BIP32_INITIAL_HARDENED_CHILD};
```
Aquí veremos al descendiente endurecido a cero (esa es la cuenta) o al primer descendiente endurecido (eso es monedas de tesnet) del descendiente 84º endurecido (ese es el estándar BIP84): `[m/84'/1'/0']`.

A continuación, puede utilizar esa ruta para generar una nueva clave a partir de su antigua clave:
```c
  struct ext_key *key_account;
  lw_response = bip32_key_from_parent_path_alloc(key_root,path_account,sizeof(path_account),BIP32_FLAG_KEY_PRIVATE,&key_account);
```
Cada vez que tenga una nueva clave,usted  puede usarla para generar nuevas claves xprv y xpub, si lo desea:
```c
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PRIVATE, &a_xprv);
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PUBLIC, &a_xpub);
```
### Generar una clave de dirección

Alternativamente, puede usar la función `bip32_key_from_parent_alloc`, que simplemente baja un nivel de la jerarquía a la vez. El siguiente ejemplo se reduce al descendiente 0º de la clave de cuenta (que es la dirección externa) y luego al descendiente 0º de la anterior. Esto sería útil porque entonces usted podría seguir generando la primera dirección, la segunda dirección, y así sucesivamente a partir de esa clave externa:
```c
  struct ext_key *key_external;  
  lw_response = bip32_key_from_parent_alloc(key_account,0,BIP32_FLAG_KEY_PRIVATE,&key_external);

  struct ext_key *key_address;  
  lw_response = bip32_key_from_parent_alloc(key_external,0,BIP32_FLAG_KEY_PRIVATE,&key_address);
```
> :warning: **ADVERTENCIA:** En algún momento de esta jerarquía, puede que decida generar `BIP32_FLAG_KEY_PUBLIC` en lugar de `BIP32_FLAG_KEY_PRIVATE`. Obviamente esta decisión se basará en su seguridad y sus necesidades, pero recuerde que sólo necesita una clave pública para generar la dirección real.

### Generar una dirección

Finalmente, usted esta listo para generar una dirección a partir de su clave final. Todo lo que hace es ejecutar `wally_bip32_to_addr_segwit` usando su clave final y una descripción del tipo de dirección que es.
```c
  char *segwit;
  lw_response = wally_bip32_key_to_addr_segwit(key_address,"tb",0,&segwit);

  printf("[m/84'/1'/0'/0/0]: %s\n",segwit);
```  
> :link: **TESTNET vs MAINNET:** El argumento `tb` define una dirección de testnet. Para mainnet use `bc`.

También hay una función `wally_bip32_key_to_address` que se puede utilizar para generar una dirección heredada o una dirección Segwit anidada.

## Prueba de código HD

El código para este ejemplo de HD se puede encontrar, como de costumbre, en el [src directory](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/src/17_3_genhd.c).

Puede compilarlo y probarlo:
```
$ cc genhd.c -lwallycore -lsodium -o genhd
$ ./genhd
Mnemonic: behind mirror pond finish borrow wood park foam guess mail regular reflect
Root xprv key: tprv8ZgxMBicQKsPdLFXmZ6VegTxcmeieNpRUq8J2ahXxSaK2aF7CGqAc14ZADLjdHJdCr8oR2Zng9YH1x1A7EBaajQLVGNtxc4YpFejdE3wyj8
Root xpub key: tpubD6NzVbkrYhZ4WoHKfCm64685BoAeoi1L48j5K6jqNiNhs4VspfeknVgRLLiQJ3RkXiA9VxguUjmEwobtmrXNbhXsPHfm9W5HJR9DKRGaGJ2
Account xprv key: tprv8yZN7h6SPvJXrhAk56z6cwHQE6qZBRreB9fqqZJ1Xd1nLci3Rw8HTmqNkpFNgf3eZx8hYzhFWafUhHSt3HgF13aHvCE6kveS7gZAyfQwMDi
Account xpub key: tpubDWFQG78gYHzCkACXxkeh2LwWo8MVLm3YkTGd85LJwtpBB6xp4KwseGTEvxjeZNhnCNPdfZqRcgcZZAka4tD3xGS2J53WKHPMRhG357VKsqT
[m/84'/1'/0'/0/0]: tb1q0knqq26ek59pfl7nukzqr28m2zl5wn2f0ldvwu
```
## Resumen: Usando BIP32 en Libwally

Una cartera HD le permite generar un gran número de claves a partir de una sola semilla. Ahora sabe cómo se organizan esas claves bajo BIP44, BIP84 y Bitcoin Core y cómo derivarlas, empezando con una semilla o palabras mnemónicas.

> :fire: ***Cuál es la potencia de BIP32?*** Las llaves son el elemento más difícil (y más peligroso) de la mayoría de las operaciones criptográficas. Si los pierde, pierde cualquier llave protegida. BIP32 se asegura de que sólo necesita conocer una clave, la semilla, en lugar de un gran número de claves diferentes para diferentes direcciones.

> :fire: ***Cuál es la potencia del BIP32 en Libwally?*** Bitcoind ya crea direcciones basadas en HD para usted, lo que significa que normalmente no tiene que preocuparse de derivar direcciones de esta manera. Sin embargo, usar las funciones BIP32 de Libwally puede ser muy útil si tiene una máquina fuera de línea donde necesitas derivar direcciones, posiblemente basadas en una semilla pasada de `bitcoind` a su dispositivo fuera de línea (o viceversa).
 
## ¿Qué es lo siguiente?

Más información sobre "Programar Bitcoin con Libwally" en [17.4: Usar PSBTs en Libwally](17_4_Usando_PSBTs_en_Libwally.md).
