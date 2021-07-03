# 3.5: Comprensión del descriptor

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con precaución.

Es posible que haya notado un extraño campo `desc:` en el comando `listunspent` de la sección anterior. Esto es de qué se trata (y cómo se puede usar para transferir direcciones).

> :advertencia: **ADVERTENCIA DE VERSIÓN:** Esta es una innovación de Bitcoin Core v 0.17.0 que ha continuado expandiéndose a través de Bitcoin Core 0.20.0. La mayoría de los comandos de esta sección son de 0.17.0, pero el `importmulti` actualizado que admite descriptores es de 0.18.0.

## Conozca la transferencia de direcciones

La mayor parte de este curso supone que está trabajando completamente desde un solo nodo donde administra su propia billetera, enviando y recibiendo pagos con las direcciones creadas por esa billetera. Sin embargo, no es necesariamente así como funciona el ecosistema de Bitcoin más grande. Allí, es más probable que mueva direcciones entre billeteras e incluso configure billeteras para vigilar los fondos controlados por diferentes billeteras.

Ahí es donde entran los descriptores. Son más útiles si está interactuando con _otro_ software distinto a Bitcoin Core, y realmente necesita apoyarse en este tipo de función de compatibilidad: consulte [§6.1](https://github.com/BlockchainCommons /Learning-Bitcoin-from-the-Command-Line/blob/master/06_1_Sending_a_Transaction_to_a_Multisig.md) para ver un ejemplo del mundo real de cómo tener la capacidad de los descriptores es fundamental.

El movimiento de direcciones entre carteras se solía centrar en `xpub` y` xprv`, y todavía se admiten.

> :book: ***¿Qué es xprv?*** Una clave privada extendida. Esta es la combinación de una clave privada y un código de cadena. Es una clave privada de la que se puede derivar una secuencia completa de claves privadas secundarias.

> :book: ***¿Qué es xpub?*** Una clave pública extendida. Esta es la combinación de una clave pública y un código de cadena. Es una clave pública de la que se puede derivar una secuencia completa de claves públicas secundarias.

El hecho de que pueda tener una "secuencia completa de claves secundarias ..." revela el hecho de que "xpub" y "xprv" no son claves estándar como hemos estado hablando hasta ahora. En cambio, son claves jerárquicas que se pueden usar para crear familias completas de claves, basadas en la idea de HD Wallets.

> :libro: ***¿Qué es una billetera HD?*** La mayoría de las billeteras modernas se basan en [BIP32: billeteras deterministas jerárquicas](https://github.com/bitcoin/bips/blob/master/bip-0032. mediawiki). Se trata de un diseño jerárquico en el que se puede utilizar una única semilla para generar una secuencia completa de claves. La billetera completa se puede restaurar a partir de esa semilla, en lugar de requerir la restauración de cada clave privada.

> :book: ***¿Qué es una ruta de derivación?*** Cuando tiene claves jerárquicas, necesita poder definir claves individuales como descendientes de una semilla. Por ejemplo, `[0]` es la clave 0, `[0/1]` es el primer hijo de la clave 0, `[1/0/1]` es el primer nieto del hijo cero de la primera clave. Algunas claves también contienen un `'` después del número, para mostrar que están endurecidas, lo que las protege de un ataque específico que se puede usar para derivar un `xprv` de un`xpub`. No necesita preocuparse por los detalles, aparte del hecho de que esos `'` s le causarán problemas de formato cuando trabaje desde la línea de comandos.

`xpubs` y` xprvs` resultaron ser insuficientes cuando los tipos de claves públicas se multiplicaron bajo la [expansión SegWit](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/04_6_Creating_a_Segwit_Transaction .md), de ahí la necesidad de los "descriptores de salida".

> :book: ***¿Qué es un descriptor de salida?*** Una descripción precisa de cómo derivar una dirección de Bitcoin a partir de una combinación de una función y una o más entradas a esa función.

La introducción de funciones en los descriptores es lo que los hace poderosos, porque se pueden usar para transferir todo tipo de direcciones, desde las direcciones heredadas con las que estamos trabajando ahora hasta las direcciones Segwit y multifirma que encontraremos más adelante. Una función individual coincide con un tipo particular de dirección y se correlaciona con reglas específicas para generar esa dirección.

## Capturar un descriptor

Los descriptores son visibles en varios comandos como `listunspent` y`getaddressinfo`:

```
$ bitcoin-cli getaddressinfo ms7ruzvL4atCu77n47dStMb3of6iScS8kZ
{
  "address": "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ",
  "scriptPubKey": "76a9147f437379bcc66c40745edc1891ea6b3830e1975d88ac",
  "ismine": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": false,
  "pubkey": "03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388",
  "iscompressed": true,
  "ischange": false,
  "timestamp": 1592335136,
  "hdkeypath": "m/0'/0'/18'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
    ""
  ]
}
```

Aquí el descriptor es `pkh ([d6043800 / 0 '/ 0' / 18 '] 03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388) # 4ahsl9pk`.

## Entender un descriptor

Un descriptor se divide en varias partes:

```
function([derivation-path]key)#checksum
```

Esto es lo que todo eso significa:
* **Función.** La función que se utiliza para crear una dirección a partir de esa tecla. En estos casos, es `pkh`, que es la dirección heredada P2PKH estándar que conoció en [§3.3: Configuración de su billetera](03_3_Setting_Up_Your_Wallet.md). De manera similar, una dirección P2WSH SegWit usaría `wsh` y una dirección P2WPKH usaría` wpkh`.
* **Ruta de derivación.** Esto describe qué parte de una billetera HD se está exportando. En este caso, es una semilla con la huella digital `d6043800` y luego el hijo 18 del hijo 0 del hijo 0 (` 0'/ 0'/18'`) de esa semilla. También puede haber una derivación adicional después de la clave: `función ([ruta de derivación] clave / más-derivación) # suma de comprobación`
   * Vale la pena señalar aquí que si alguna vez obtiene una ruta de derivación sin una huella digital, puede inventarla. Es solo que si hay uno existente, debe coincidir, porque si alguna vez regresa al dispositivo que creó la huella digital, deberá tener el mismo.
* **Clave**. La clave o claves que se están transfiriendo. Esto podría ser algo tradicional como un `xpub` o` xprv`, podría ser simplemente una clave pública para una dirección, como en este caso, podría ser un conjunto de direcciones para una firma múltiple, o podría ser otra cosa. Estos son los datos centrales: la función explica qué hacer con ellos.
* **Suma de comprobación**. Los descriptores están destinados a ser transferibles por humanos. Esta suma de comprobación asegura que lo haga bien.

Consulte [Información de Bitcoin Core sobre la compatibilidad con descriptores](https://github.com/bitcoin/bitcoin/blob/master/doc/descriptors.md) para obtener más información.

Además de darle la suma de verificación, este comando también verifica la validez del descriptor y proporciona información útil como comprobar si un descriptor contiene claves privadas.

Uno de los poderes de un descriptor es poder derivar una dirección de forma regular. Esto se hace con el RPC `deriveaddresses`.

```
$ bitcoin-cli deriveaddresses "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk"
[
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ"
]
```

Notará que regresa a la dirección con la que comenzamos (como debería).

## Importar un descriptor

Pero lo realmente importante de un descriptor es que puede llevarlo a otra máquina (remota) e importarlo. Esto se hace con el RPC `importmulti` usando la opción`desc`:

```
remote$ bitcoin-cli importmulti '[{"desc": "pkh([d6043800/0'"'"'/0'"'"'/18'"'"']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk", "timestamp": "now", "watchonly": true}]'
[
  {
    "success": true
  }
]
```
Primero, notará nuestro primer uso realmente desagradable de las comillas. Cada `'` en la ruta de derivación tuvo que ser reemplazado por `'"'"'`. Solo espere tener que hacer eso si está manipulando un descriptor que contiene una ruta de derivación. (La otra opción es intercambiar el `'` con una `h` por el endurecido, pero eso cambiará su suma de verificación, por lo que si lo prefiere por su facilidad de uso, deberá obtener una nueva suma de verificación con `getdescriptorinfo` .)

En segundo lugar, observará que marcamos esto como `solo vigilancia` o `watchonly`. Eso es porque sabemos que es una clave pública, por lo que no podemos gastar con ella. Si no hubiéramos podido ingresar esta bandera, `importmulti` nos habría ayudado a decirnos algo como: `Faltan algunas claves privadas, las salidas se considerarán solo de observación. Si es intencional, especifique la bandera de solo vigilancia`.

> :book: ***¿Qué es una dirección de solo reloj?*** Una dirección de solo reloj le permite ver las transacciones relacionadas con una dirección (o con toda una familia de direcciones si usó un `xpub`), pero no gastar fondos en esas direcciones.

Usando `getaddressesbylabel`, ¡ahora podemos ver que nuestra dirección ha sido importada correctamente a nuestra máquina remota!

```
remote$ bitcoin-cli getaddressesbylabel ""
{
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ": {
    "purpose": "receive"
  }
}
```

## Resumen: comprensión del descriptor

Los descriptores le permiten pasar claves públicas y claves privadas entre billeteras, pero más que eso, le permiten definir direcciones de manera precisa y correcta y derivar direcciones de muchos tipos diferentes a partir de un formato de descripción estandarizado.

> :fire: ***¿Cuál es el poder de los descriptores?*** Los descriptores le permiten importar y exportar semillas y claves. Eso es genial si quiere moverse entre diferentes carteras. Como desarrollador, también le permiten crear el tipo preciso de direcciones que le interesa crear. Por ejemplo, lo usamos en [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2/blob/master/Docs/How-it-works.md) para generar un multifirma a partir de tres semillas.

Haremos un uso real de los descriptores en [§7.3] (07_3_Integrating_with_Hardware_Wallets.md), cuando estemos importando direcciones desde una billetera de hardware.

## ¿Que sigue?

Avance a través de "bitcoin-cli" con el [Capítulo cuatro: Envío de transacciones de Bitcoin] (04_0_Sending_Bitcoin_Transactions.md).
