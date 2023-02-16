# 11.1: Entendiendo las Opciones de los Bloqueos de Tiempo

En [§8.1: Enviando una Transacción con Bloqueo de Tiempo](08_1_Enviando_una_Transaccion_con_Bloqueo_de_Tiempo.md), `nLocktime` ofrecía una gran primera opción para bloquear transacciones así no podían ser gastadas hasta cierto punto en el futuro - basado tanto en el tiempo como en la altura de bloque. Pero, este no es el único modo de poner un bloqueo temporal sobre una transacción.

## Comprender las Limitaciones de nLockTime

`nLockTime` es una forma simple y poderosa de bloquear una transacción, pero tiene algunas limitaciones:

1. **No División.** `nLocktime` bloquea la transacción completa.
2. **No Retransmisión.** La mayoría de los nodos modernos no aceptaran un `nLockTime` dentro de la mempool hasta que este casi finalizado.
3. **No Scripts.** El original, simple uso de `nLockTime` no permitía su utilización en Scripts.
4. **No Protección.** `nLockTime` permite que los fondos sean gastados con una transacción diferente, no bloqueada.

El último item era el factor decisivo para `nLockTime`. Este prevenía una transacción de ser gastada, pero no prevenía que los mismos fondos fueran usados en una transacción diferente. Es decir, tenía sus casos de uso, pero todo dependía de la confianza.

## Comprender las Posibilidades de los Scripts con Bloqueos de Tiempo

En años mas recientes, Bitcoin Core se ha expandido para permitir la manipulación de los bloqueos de tiempo al nivel de opcode con _OP_CHECKLOCKTIMEVERIFY_ (CLTV) y _OP_CHECKSEQUENCEVERIFY_ (CSV). Ambos trabajan bajo una nueva metodología que potencia aún más a Bitcoin.

_Son Opcodes._ Porque son opcodes, CLTV y CSV pueden ser usados como parte de condiciones de canje mas complejas. Habitualmente son relacionadas con condicionales descriptos en el próximo capítulo.

_Bloquean Outputs._ Porque son opcodes que son incluidos en las transacciones como parte de `sigPubKey`, solo bloquean un solo output. Esto significa que las transacciones son aceptadas en toda la red Bitcoin y que los UTXOs usados para financiar esas transacciones son gastados. No hay vuelta atrás en una transacción temporalmente bloqueada con CLTV o CSV como hay con una simple `nLockTime`. Volver a gastar los UTXO resultantes requiere entonces que las condiciones del bloqueo de tiempo sean satisfechas.

Aquí hay un obstáculo para usar bloqueos de tiempo: _Son bloqueos de una vía._ Los Bloqueos de tiempo están diseñados para desbloquear fondos en un cierto tiempo. No pueden re-bloquear un fondo: una vez que un fondo bloqueado temporalmente se desbloquea, va a permanecer siempre disponible para gastar.

### Comprender las Posibilidades de CLTV

_OP_CHECKLOCKTIMEVERIFY_ o CLTV es compatible con el clásico `nLockTime`, pero en el nuevo paradigma basado en opcodes. Permite a un UTXO volverse accesible en un cierto tiempo o a una cierta altura de bloque.

CLTV fue detallado inicialmente en [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki).

### Comprender las Posibilidades de CSV

_OP_CHECKSEQUENCEVERIFY_ o CSV depende de un nuevo tipo de "bloqueo de tiempo relativo", el cual es configurado en el campo _nSequence_ de la transacción. Como siempre, puede ser configurado tanto con tiempo como con altura de bloque. Si es configurado como tiempo, "n", entonces una transacción con bloqueo de tiempo relativo es gastable "n x 512" segundos después de que su UTXO fue minado, y si es configurado como bloque, "n", entonces una transacción con bloqueo de tiempo es gastable "n" bloques después de que su UTXO fue minado.

El uso de `nSequence` para bloqueos de tiempo relativos fue detallado por primera vez en [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki), luego el opcode CSV fue agregado en [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

## Resumen: Entendiendo las Opciones de los Bloqueos de Tiempo

Tiene ahora 4 opciones de Bloqueo de Tiempo:

* `nLockTime` para mantener una transacción fuera de la cadena de bloques hasta un tiempo específico.
* `nSequence` para mantener una transacción fuera de la cadena de bloques hasta un tiempo relativo.
* CLTV para hacer un UTXO no gastable hasta un tiempo específico.
* CSV para hacer un UTXO no gastable hasta un tiempo relativo.

## ¿Que sigue?

Continúe "Potenciando los Bloqueos de Tiempo" con [§11.2: Usando CLTV en Scripts](11_2_Usando_CLTV_en_Scripts.md). 
