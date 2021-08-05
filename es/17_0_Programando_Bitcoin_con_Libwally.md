# Capítulo 17: Programación con Libwally

El capítulo anterior presentó tres Bibliotecas C, para RPC, JSON y ZMQ, todas las cuales están destinadas a interactuar directamente con `bitcoind`, tal como lo ha estado haciendo desde el principio. Pero a veces es posible que desee codificar sin acceso directo a un `bitcoind`. Esto puede ser debido a un cliente fuera de línea, o simplemente porque desea mantener alguna funcionalidad interna en su programa C. También es posible que desee entrar en una funcionalidad de cartera más profunda, como la creación de palabras mnemónicas o la derivación de direcciones. Ahí es donde entra Libwally: es una biblioteca de carteras para C, C++, Java, NodeJS o Python, con envolturas también disponibles para otros idiomas, como Swift.
Este capítulo toca la funcionalidad posible dentro de Libwally, la mayoría de los cuales complementan el trabajo que ha hecho a través del acceso RPC a `bitcoind`, pero algunos de los cuales lo replican. También muestra cómo integrar ese trabajo con los clientes RPC con los que está más familiarizado. Sin embargo, tenga en cuenta que esta es sólo la introducción más cruda a Libwally. Varios de sus conjuntos de funciones más importantes están resaltados, pero nunca hacemos más que meter los dedos del pie. Si encuentra sus funciones útiles o intrigantes, entonces tendrá que profundizar mucho más de lo que este curso puede cubrir.

## Objetivos para este capítulo

Después de trabajar en este capítulo, un desarrollador podrá:

   * Utilizar funciones de cartera con Libwally
   * Realizar manipulaciones de PSBTs y transacciones con Libwally
   * Implementar diseños que mezclen Libwally y trabajo RPC

Los objetivos de apoyo incluyen la capacidad de:

  * Entender las palabras mnemónicas de BIP39
  * Conozca más sobre las carteras jerárquicas BIP32
  * Resumir la profundidad funcional de Libwally

## Tabla de contenidos
  * [Sección Uno: Configuración de Libwally](17_1_Configurando_Libwally.md)
  * [Sección Dos: Usar BIP39 en Libwally](17_2_Usando_BIP39_en_Libwally.md)
  * [Sección Tres: Usar BIP32 en Libwally](17_3_Usando_BIP32_en_Libwally.md)
  * [Sección Cuatro: Uso de PSBTs en Libwally](17_4_Usando_PSBTs_en_Libwally.md) 
  * [Sección Cinco: Usar scripts en Libwally](17_5_Usando_Scripts_en_Libwally.md)
  * [Sección Seis: Usar otras funciones en Libwally](17_6_Usando_Otras_Funciones_en_Libwally.md)
  * [Sección Siete: Integración de Libwally y Bitcoin-CLI](17_7_Integrando_Libwally_y_Bitcoin-CLI.md)
