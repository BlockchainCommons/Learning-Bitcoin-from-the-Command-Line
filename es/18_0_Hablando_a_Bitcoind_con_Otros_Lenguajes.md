# Capítulo 18: Hablando con Bitcoind con otros lenguajes

Ahora debería tener una base sólida para trabajar con Bitcoin en C, no solo usando bibliotecas RPC, JSON y ZMQ para interactuar directamente con `bitcoind`, sino también utilizando las bibliotecas Libwally para complementar ese trabajo. Y C es un gran lenguaje para la creación de prototipos y la abstracción pero probablemente no es en lo que usted esta programando. Este capítulo por lo tanto toma un recorrido de torbellino de otros seis lenguajes de programación, demostrando la funcionalidad de Bitcoin más básica en cada uno y permitiéndole ampliar las lecciones de la línea de comandos y C al lenguaje de programación de su elección.
Cada una de las secciones contiene aproximadamente la misma información, centrada en: crear una conexión RPC; examinar la cartera; crear una nueva dirección, y crear una transacción. Sin embargo, hay cierta variedad entre los idiomas, mostrando diferentes aspectos de los comandos RPC de Bitcoin en diferentes ejemplos. En particular, algunos lenguajes utilizan la sencilla metodología de `sendtoaddress` mientras que otros utilizan la dura metodología de crear una transacción en bruto desde cero.

## Objetivos para este capítulo

Después de trabajar en este capítulo, un desarrollador podrá:

   * Preparar los entornos de desarrollo de Bitcoin para una variedad de lenguajes
   * Utilizar las funciones de la cartera en una variedad de lenguajes
   * Usar funciones de transacción en una variedad de lenguajes
   
Los objetivos de apoyo incluyen la capacidad de:

  * Entender más sobre Bitcoin RPC a través de interacciones con una variedad de lenguajes

## Tabla de contenidos
  * [Sección Uno: Acceso a Bitcoind con Go](18_1_Accediendo_a_Bitcoind_con_Go.md)
  * [Sección Dos: Acceso a Bitcoind con Java](18_2_Accediendo_a_Bitcoind_con_Java.md)
  * [Sección Tres: Acceso a Bitcoind con NodeJS](18_3_Accediendo_a_Bitcoind_con_NodeJS.md)
  * [Sección Cuatro: Acceder a Bitcoind con Python](18_4_Accediendo_a_Bitcoind_con_Python.md)
  * [Sección Cinco: Acceso a Bitcoind con Rust](18_5_Accediendo_a_Bitcoind_con_Rust.md)
  * [Sección Seis: Acceso a Bitcoind con Swift](18_6_Accediendo_a_Bitcoind_con_Swift.md)
