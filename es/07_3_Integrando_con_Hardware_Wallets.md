# 7.3: Integrando con Hardware Wallets

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

Uno de los mayores poderes de los PSBT es la capacidad de transferir transacciones a billeteras de hardware. Esta será una gran herramienta de desarrollo para usted si continúa programando con Bitcoin. Sin embargo, no puede probarlo ahora si está utilizando una de las configuraciones que sugerimos para este curso: una máquina virtual en Linode según [§2.1](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md) o una opción aún más remota como AWS según [§2.2](02_2_Configurando_Bitcoin_Core_Otros.md) — porque obviamente no tendrá forma de conectar una billetera de hardware a su máquina virtual remota.

> :book: ***¿Qué es una cartera de hardware?*** Una billetera de hardware es un dispositivo electrónico que mejora la seguridad de una criptomoneda al mantener todas las claves privadas en el dispositivo, en lugar de ponerlas en una computadora directamente conectada a Internet. Las billeteras de hardware tienen protocolos específicos para proporcionar interacciones en línea, generalmente administradas por un programa que habla con el dispositivo a través de un puerto USB. En este capítulo, administraremos una billetera de hardware con `bitcoin-cli` y el programa `hwy.py`.

Tiene tres opciones para avanzar a través de este capítulo sobre billeteras de hardware: (1) leer sin probar el código; (2) instale Bitcoin en una máquina local para probar completamente estos comandos; o (3) pasar directamente al [Capítulo 8: Expandiendo Transacciones Bitcoin en Otros Sentidos](08_0_Expandiendo_Bitcoin_Transacciones_Otros.md). Sugerimos la opción #1, pero si realmente quiere ensuciarse las manos, también daremos algo de apoyo a la #2 hablando sobre el uso de Macintosh (una plataforma de hardware compatible con [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup)) para realizar pruebas.

> :warning: **ADVERTENCIA DE VERSIÓN:** Los PSBT son una innovación de Bitcoin Core v 0.17.0. Las versiones anteriores de Bitcoin Core no podrán funcionar con el PSBT mientras esté en progreso (aunque aún podrán reconocer la transacción final). La interfaz HWI apareció en Bitcoin Core v 0.18.0, pero siempre que utilice nuestra configuración sugerida con Bitcoin Standup, debería funcionar.

La metodología descrita en este capítulo para la integración con una billetera de hardware depende de la [Interfaz de billetera de hardware de Bitcoin](https://github.com/bitcoin-core/HWI) lanzada a través de Bitcoin Core y se basa en las [instrucciones de instalación](https://github.com/bitcoin-core/HWI/blob/master/README.md) y [uso](https://hwi.readthedocs.io) que se encuentran allí.

> :warning: **ADVERTENCIA DE FRESCURA:** La interfaz HWI es muy nueva y cruda en los bordes a partir de Bitcoin Core v 0.20.0. Puede ser difícil de instalar correctamente y puede tener errores poco intuitivos. Lo que sigue es una descripción de una configuración funcional, pero tomó varios intentos para hacerlo bien y su configuración puede variar.

## Instale Bitcoin Core en una máquina local

_Si solo planea leer esta sección y no probar estos comandos hasta una fecha futura cuando tenga un entorno de desarrollo local, puede omitir esta subsección, que trata sobre la creación de una instalación de Bitcoin Core en una máquina local como una Mac o Máquina Linux._

Hay versiones alternativas del script Bitcoin Standup que usó para crear su VM que se instalará en MacOS o en una máquina Linux que no sea Linode.

Si tiene MacOS, puede instalar [Bitcoin Standup MacOS](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

Si tiene una máquina Linux local, puede instalar [Bitcoin Standup Linux Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/README.md).

Una vez que haya ejecutado Bitcoin Standup en su máquina local, querrá sincronizar la cadena de bloques "Testnet", asumiendo que continúa siguiendo la metodología estándar de este curso.

Usaremos Macintosh y Testnet para los ejemplos de esta sección.

### Cree un alias para Bitcoin-CLI

Cree un alias que ejecute `bitcoin-cli` desde el directorio correcto con los indicadores apropiados.

Aquí hay un ejemplo de alias de una Mac:
```
$ alias bitcoin-cli="~/StandUp/BitcoinCore/bitcoin-0.20.0/bin/bitcoin-cli -testnet"
```
Notará que no solo nos da la ruta completa, sino que también asegura que permanezcamos en Testnet.

## Instalar HWI en una máquina local

_Las siguientes instrucciones asumen nuevamente una Mac, y puede omitir nuevamente esta subsección si solo está leyendo este capítulo._

HWI es un programa de Bitcoin Core disponible en Python que se puede utilizar para interactuar con billeteras de hardware.

### Instalar Python

Debido a que HWI está escrito en `python`, necesitará instalarlo, así como algunos programas auxiliares.

Si aún no tiene las herramientas de línea de comando `xcode`, las necesitará:
```
$ xcode-select --install
```

Si aún no tiene el administrador de paquetes Homebrew, debería instalarlo también. Las instrucciones actuales están disponibles en [sitio de Homebrew](https://brew.sh/). Al momento de escribir estas líneas, simplemente necesita::
```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

Para una instalación por primera vez, también debe asegurarse de que su directorio `/usr/local/Frameworks` se haya creado correctamente:
```
$ sudo mkdir /usr/local/Frameworks
$ sudo chown $(whoami):admin /usr/local/Frameworks
```

Si tiene todo eso en su lugar, finalmente puede instalar Python:
```
$ brew install python
$ brew install libusb
```

### Instalar HWI

Ahora está listo para instalar HWI, que requiere clonar un repositorio de GitHub y ejecutar un script de instalación.

Si aun no tiene `git` instalado en su Mac, puede hacerlo simplemente intentando ejecutarlo así: `git --version`.

Luego puede clonar el repositorio HWI:
```
$ cd ~/StandUp
$ git clone https://github.com/bitcoin-core/HWI.git
```
Luego, debe instalar el paquete y sus dependencias:
```
$ cd HWI
HWI$ python3 setup.py install
```

### Crear un alias para HWI

También querrá crear un alias aquí, variará según la ubicación real de su instalación:
```
$ alias hwi="~/Standup/HWI/hwi.py --chain test"
```
Nuevamente, hemos incluido una referencia a testnet en este alias.

## Prepare su Ledger

_También tuvimos que elegir una plataforma de billetera de hardware para esta demostración de HWI. Nuestra elección fue el Ledger, que ha sido durante mucho tiempo nuestro banco de pruebas para billeteras de hardware. Consulte [Información de soporte del dispositivo de HWI](https://github.com/bitcoin-core/HWI/blob/master/README.md#device-support) para obtener una lista de otros dispositivos compatibles. Si utiliza un dispositivo que no sea Ledger, deberá evaluar sus propias soluciones para prepararlo para su uso en Testnet, pero de lo contrario debería poder continuar con el curso tal como está escrito._

Si está trabajando con Bitcoins en su libro mayor, probablemente no tendrá que hacer nada. (Pero no sugerimos que se use con este curso).

Para trabajar con monedas de Testnet, como sugiere este curso, deberá realizar algunas actualizaciones:

1. Vaya a Configuración en su aplicación Ledger Live (es el engranaje), vaya a la pestaña "Funciones experimentales" y active el "Modo de desarrollador".
2. Vaya al "Administrador" e instale "Prueba de Bitcoin". La versión actual requiere que primero instale "Bitcoin".
3. Vaya al "Administrador", desplácese hasta su nueva "Prueba de Bitcoin" y "Agregar cuenta"

## Enlace a su Ledger

Para que un libro mayor sea accesible, debe iniciar sesión con su PIN y luego abrir la aplicación que desea usar, en este caso, la aplicación "Prueba de Bitcoin". Es posible que deba repetir esto de vez en cuando si su Ledger entra en reposo.

Una vez que haya hecho eso, puede pedirle a HWI que acceda al Libro mayor con el comando `enumerate`::
```
$ hwi enumerate
[{"type": "ledger", "model": "ledger_nano_s", "path": "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/XHC1@14/XHC1@14000000/HS05@14100000/Nano S@14100000/Nano S@0/IOUSBHostHIDDevice@14100000,0", "fingerprint": "9a1d520b", "needs_pin_sent": false, "needs_passphrase_sent": false}]
```
Si recibe información en su dispositivo, ¡está listo! Como puede ver, verifica su tipo de billetera de hardware, proporciona otra información de identificación y le indica cómo comunicarse con el dispositivo. La `huella digital` (`9a1d520b`) es a lo que debe prestar especial atención, porque todas las interacciones con su billetera de hardware lo requerirán.

Si en su lugar obtuvo `[]`, entonces (1) no preparó su dispositivo Ledger ingresando su PIN y eligiendo la aplicación correcta, o (2) hay algo mal con su configuración de Python, probablemente una dependencia faltante: debería considerar desinstalarlo e intentarlo desde cero.

## Importar direcciones

La interacción con una billetera de hardware generalmente se divide en dos partes: buscar fondos y gastar fondos.

Puede buscar fondos importando direcciones desde su billetera de hardware a su nodo completo, usando HWI y `bitcoin-cli`.

### Crea una billetera

Para usar su billetera de hardware con `bitcoin-cli`, querrá crear una billetera con nombre específico en Bitcoin Core, usando el comando RPC `createwallet`, que es un comando que no hemos discutido anteriormente.
```
$ bitcoin-cli --named createwallet wallet_name="ledger" disable_private_keys="true"
{
  "name": "ledger",
  "warning": ""
}
```
En este caso, está creando un nuevo `ledger` de billetera sin claves privadas (ya que éstas terminarán en el dispositivo de Ledger).

> :book: ***¿Por qué nombrar billeteras?*** Hasta la fecha, este curso ha utilizado la billetera predeterminada ("") en Bitcoin Core. Esto está bien para muchos propósitos, pero es inadecuado si tiene una situación más compleja, como cuando está viendo claves desde una billetera de hardware. Aquí, queremos poder diferenciar de las claves de propiedad local (que se mantienen en la "" billetera) y las claves de propiedad remota (que se guardan en la billetera "ledger").

Ahora puede ver que la nueva billetera está en su lista de billeteras:
```
$ bitcoin-cli listwallets
[
  "",
  "ledger"
]
```
Debido a que ha creado una segunda billetera, algunos comandos ahora requerirán una marca `-rpcwallet =`, para especificar cuál está usando.

### Importar las claves

Ahora tiene que importar una lista de seguimiento de direcciones desde la billetera de hardware. Esto se hace con el comando `getkeypool` de HWI:
```
$ hwi -f 9a1d520b getkeypool 0 1000
[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]
```
Nos dirigimos a HWI con la `huella digital` y pedimos las primeras 1000 direcciones WPKH (Segwit nativo). El tipo de direcciones WPKH (native Segwit) es usado por defecto. A cambio, recibimos dos descriptores para el grupo de claves: uno para las direcciones de recepción y otro para las direcciones de cambio.

> :book: ***¿Qué es un grupo de claves?*** Un grupo de claves es un grupo de claves pregeneradas. Las billeteras HD modernas crean grupos de claves al continuar determinando nuevas direcciones jerárquicas basadas en la semilla original. La idea de los grupos de claves se implementó originalmente para facilitar los requisitos de respaldo de las billeteras. Esto permitió a un usuario generar un conjunto de claves y luego hacer una copia de seguridad de la billetera de inmediato, en lugar de requerir copias de seguridad después de que se creara cada nueva dirección. El concepto también ha demostrado ser muy útil en la actualidad, ya que permite importar un conjunto completo de direcciones futuras de un dispositivo a otro.

Los valores devueltos por `getkeypool` son el mismo tipo de descriptores que aprendimos en [§3.5: Entendiendo El Descriptor](03_5_Entendiendo_El_Descriptor.md). En ese momento, dijimos que eran más útiles para mover direcciones entre diferentes máquinas. Aquí está el ejemplo de la vida real: mover direcciones de una billetera de hardware al nodo Bitcoin Core, para que nuestra máquina conectada a la red pueda vigilar las claves que pertenecen a la billetera de hardware fuera de línea.

Tal como aprendió en [§3.5](03_5_Entendiendo_El_Descriptor.md), puede examinar estos descriptores con el RPC `getdescriptorinfo`:
```
$ bitcoin-cli getdescriptorinfo "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592"
{
  "descriptor": "wpkh([9a1d520b/84'/1'/0']tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#n65e7wjf",
  "checksum": "qttxy592",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Como era de esperar, _no_ tenemos `privatekeys`, porque las billeteras de hardware se aferran a ellas.

Con los descriptores en la mano, puede importar las claves a su nueva billetera `ledger` usando el RPC `importmulti` que también conoció en [§3.5](03_5_Entendiendo_El_Descriptor.md). En este caso, simplemente coloque la respuesta completa que recibió de HWI en `'`s.
```
$ bitcoin-cli -rpcwallet=ledger importmulti '[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]'
[
  {
    "success": true
  },
  {
    "success": true
  }
]
```
(Tenga en cuenta que HWI muestra de manera útil la ruta de derivación con `h`s para mostrar derivaciones reforzadas en lugar de `'`s, y calculó su suma de comprobación en consecuencia, de modo que no tengamos que hacer citas masivas como hicimos en §3.5).

_Podría_ ahora enumerar todas las direcciones de solo visualización que recibió usando el comando `getaddressesbylabel`. ¡Todas las 1000 direcciones de recepción están allí, en la billetera del `ledger`!
```
$ bitcoin-cli -rpcwallet=ledger getaddressesbylabel "" | more
{
  "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y": {
    "purpose": "receive"
  },
  "tb1qqzvrm6hujdt93qctuuev5qc4499tq9fdk0prwf": {
    "purpose": "receive"
  },
...
}
```
## Reciba una Transacción

Obviamente, recibir una transacción es sencillo. Utilice `getnewaddress` para solicitar una de esas direcciones importadas:
```
$ bitcoin-cli -rpcwallet=ledger getnewaddress
tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y
```
Luego envíele dinero.

El poder de HWI es que puede ver los pagos desde su nodo Bitcoin Core, en lugar de tener que conectar su billetera de hardware y consultarlo.
```
$ bitcoin-cli -rpcwallet=ledger listunspent
[
  {
    "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
    "vout": 1,
    "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr",
    "label": "",
    "scriptPubKey": "00142d4f139faa885304d15616a166d2d51574af4a5d",
    "amount": 0.01000000,
    "confirmations": 12,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/0]02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab)#9za8hlvk",
    "safe": true
  },
  {
    "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
    "vout": 0,
    "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y",
    "label": "",
    "scriptPubKey": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/569]030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5)#sx9haake",
    "safe": true
  }
]
```
## Cree una transacción con PSBT

Ver y recibir pagos es solo la mitad de la batalla. Es posible que también desee realizar pagos utilizando cuentas en su billetera de hardware. Este es el cuarto ejemplo de la vida real para el uso de PSBT, según el proceso descrito en [§7.1: Creando una Transacción Bitcoin Parcialmente Firmada](07_1_Creando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md). 

Los comandos funcionan exactamente igual. En este caso, use `walletcreatefundedpsbt` para formar su PSBT porque esta es una situación en la que no le importa qué UTXO se usan:
```
$ bitcoin-cli -named -rpcwallet=ledger walletcreatefundedpsbt inputs='''[]''' outputs='''[{"tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd":0.015}]'''
{
  "psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==",
  "fee": 0.00000209,
  "changepos": 1
}
```

Puede echar un vistazo al PSBT y verificar que parezca racional:
```
$ psbt="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA=="

$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "hash": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01500000,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      },
      {
        "value": 0.00499791,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 f4e8dde5db370898b57c84566e3f76098850817d",
          "hex": "0014f4e8dde5db370898b57c84566e3f76098850817d",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q7n5dmewmxuyf3dtus3txu0mkpxy9pqtacuprak"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 00193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "hex": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "type": "witness_v0_keyhash",
          "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/569"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2d4f139faa885304d15616a166d2d51574af4a5d",
          "hex": "00142d4f139faa885304d15616a166d2d51574af4a5d",
          "type": "witness_v0_keyhash",
          "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/0"
        }
      ]
    }
  ],
  "outputs": [
    {
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "03d942b59eea527d70bcd67981eb95d9fa9625269fd6eb0364e452ede59092c7a9",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/1/1"
        }
      ]
    }
  ],
  "fee": 0.00000209
}
```
Y como de costumbre, `analysepsbt` mostrará lo lejos que ha llegado:
```
$ bitcoin-cli analyzepsbt $psbt
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "00193c8bf25ef056f8d4571a1c3f65123e5fe788"
        ]
      }
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "2d4f139faa885304d15616a166d2d51574af4a5d"
        ]
      }
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "signer"
}
```
Debido a que importó ese conjunto de claves, `bitcoin-cli` tiene toda la información que necesita para completar las entradas, simplemente no puede firmar porque las claves privadas se guardan en la billetera de hardware.

Ahí es donde entra HWI, con el comando `signtx`. Simplemente envíe el PSBT:
```
$ hwi -f 9a1d520b signtx $psbt
```
Espere tener que jugar un poco con su billetera de hardware en este punto. El dispositivo probablemente le pedirá que confirme las entradas, las salidas y la tarifa. Cuando haya terminado, debería devolver un nuevo PSBT.

```
{"psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="}
$ psbt_f="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="
```

Cuando analice esto, verá que está listo para finalizar:
```
$ bitcoin-cli analyzepsbt $psbt_f
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "finalizer"
}
```

En este punto, regresa al territorio estándar:
```
$ bitcoin-cli finalizepsbt $psbt_f
{
  "hex": "02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000",
  "complete": true
}
$ hex=02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000
$ bitcoin-cli sendrawtransaction $hex
45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d
```
¡Ha enviado fondos con éxito utilizando las claves privadas guardadas en su billetera de hardware!

## Aprenda otros comandos HWI

Hay una variedad de otros comandos disponibles para usar con HWI. En el momento de escribir este artículo, son:
```
numerate,getmasterxpub,signtx,getxpub,signmessage,getkeypool,getdescriptors,displayaddress,setup,wipe,restore,backup,promptpin,togglepassphrase,sendpin
```

## Resumen: integración con billeteras de hardware
Las billeteras de hardware pueden ofrecer una mejor protección al mantener sus claves privadas fuera de línea, protegidas en el hardware. Afortunadamente, todavía hay una forma de interactuar con ellos usando `bitcoin-cli`. Simplemente instale HWI y luego le permitirá (1) importar claves públicas para verlas; y (2) firmar transacciones usando su billetera de hardware.

> :fire: ***¿Cuál es el poder de HWI?*** HWI le permite interactuar con billeteras de hardware utilizando todos los comandos de `bitcoin-cli` que ha aprendido hasta la fecha. Puede realizar transacciones sin procesar de cualquier tipo y luego enviar PSBT a billeteras de hardware para firmar. Por lo tanto, tiene todo el poder de Bitcoin Core, pero también tiene la seguridad de un dispositivo de hardware.

## ¿Que sigue?

Expanda más las transacciones de Bitcoin con el [Capítulo ocho: Expandiendo Transacciones Bitcoin en Otros Sentidos](08_0_Expandiendo_Bitcoin_Transacciones_Otros.md) .
