# Capítulo tres: Comprensión de la configuración de Bitcoin

Ahora está listo para comenzar a trabajar con la interfaz de línea de comandos `bitcoin-cli`. Pero eso primero requiere que comprenda la configuración de Bitcoin y las características de su billetera, que es lo que se explicará en este capítulo.

Para este y futuros capítulos, suponemos que tiene un VPS con Bitcoin instalado, con la ejecución de `bitcoind`. También suponemos que está conectado a testnet, lo que le permite acceder a bitcoins sin utilizar fondos reales. Puede hacer esto con Bitcoin Standup en Linode.com en [§2.1: Setting up a Bitcoin-Core VPS with Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), o mediante otros medios, en [§2.2: Setting up a Bitcoin-Core Machine via Other Means](02_2_Setting_Up_Bitcoin_Core_Other.md).

##  Objetivos de este capítulo

Después de trabajar en este capítulo, un desarrollador podrá:

   * Demuestrar que su nodo Bitcoin está instalado y actualizado.
   * Crear una dirección para recibir fondos de Bitcoin.
   * Usar comandos básicos de billetera.
   * Crear una dirección a partir de un descriptor
   
Los objetivos de apoyo incluyen la capacidad de:

   * Comprender el diseño básico de archivos de Bitcoin.
   * Usar comandos informativos básicos.
   * Comprender qué es una dirección de Bitcoin.
   * Comprender qué es una billetera.
   * Comprender cómo importar direcciones.
   
## Tabla de contenidos

* [Sección uno: Verificación de la configuración de Bitcoin](03_1_Verifying_Your_Bitcoin_Setup.md)
* [Sección dos: Conocer la configuración de Bitcoin](03_2_Knowing_Your_Bitcoin_Setup.md)
* [Sección tres: Configuración de su billetera](03_3_Setting_Up_Your_Wallet.md)
   * [Interludio: uso de variables de la línea de comandos](03_3__Interlude_Using_Command-Line_Variables.md)
* [Sección cuatro: recibir una transacción](03_4_Receiving_a_Transaction.md)
* [Sección cinco: Comprensión del descriptor](03_5_Understanding_the_Descriptor.md)

