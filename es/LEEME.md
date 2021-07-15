# Aprendiendo Bitcoin desde la Línea de Comandos 2.0.1
*Por Christopher Allen and Shannon Appelcline*

![alt text](https://www.blockchaincommons.com/images/projects/lbtc-screen.png)

Aprender Bitcoin desde la línea de comandos es un tutorial para trabajar con Bitcoin (y Lightning) que enseña mediante la interacción directa con los propios servidores, como la forma más sólida y segura de comenzar a trabajar con criptomonedas.

> NOTA: Este es un borrador en progreso, por lo que puedo obtener algunos comentarios de los primeros revisores. Aún no está listo para usarse.

Este tutorial asume que usted tiene algunos conocimientos previos mínimos sobre cómo usar la interfaz de línea de comandos. Si no es así, hay muchos tutoriales disponibles y tengo uno para usuarios de Mac en [https://github.com/ChristopherA/intro-mac-command-line.](https://github.com/ChristopherA/intro-mac-command-line)

## Traducciones

* [Inglés](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line) - versión original
* [Portugués](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/portuguese-translation/pt) - en proceso

Si le gustaría hacer su propia traducción, por favor, vea [Contribuyendo](#contribuyendo), abajo.

## Tabla de Contenidos

### PARTE UNO: PREPARÁNDOSE PARA BITCOIN

**Estado:** Finalizado. Actualizado para la versión 0.20.

* [1.0: Introducción a la programación con Bitcoin Core y Lightning](01_0_Introduccion.md)
    * [Interludio: Introduciendo Bitcoin](01_1_Introduciendo_Bitcoin.md)
* [2.0: Configuración de un Servidor Privado Virtual](02_0_Configurando_un_Bitcoin-Core_VPS.md)
  * [2.1: Configuración de un Servidor Privado Virtual de Bitcoin-Core con Bitcoin Standup](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md)
  * [2.2: Configuración de una maquina Bitcoin-Core por otros medios](02_2_Configurando_Bitcoin_Core_Otros.md)

### PARTE DOS: USANDO BITCOIN-CLI

**Estado:** Finalizado. Actualizado para la versión 0.20.

* [3.0: Entendiendo Su Configuración Bitcoin](03_0_Entendiendo_Su_Configuracion_Bitcoin.md)
  * [3.1: Verificando Su Configuración Bitcoin](03_1_Verificando_Su_Configuracion_Bitcoin.md)
  * [3.2: Conociendo Su Configuración Bitcoin](03_2_Conociendo_Su_Configuracion_Bitcoin.md)
  * [3.3: Configurando Su Billetera](03_3_Configurando_Su_Billetera.md)
    * [Interludio: Usando Variables de la Linea de Comandos](03_3_Interludio_Usando_Variables_Linea_Comando.md)
  * [3.4: Recibiendo una Transacción](03_4_Recibiendo_una_Transaccion.md)
  * [3.5: Entendiendo El Descriptor](03_5_Entendiendo_El_Descriptor.md)
* [4.0: Enviando Transacciones Bitcoin](04_0_Enviando_Transacciones_Bitcoin.md)
  * [4.1: Enviando Monedas de la Forma Fácil](04_1_Enviando_Monedas_de_la_Forma_Facil.md)
  * [4.2: Creando una Transacción Cruda](04_2_Creando_una_Transaccion_Cruda.md)
     * [Interludio: Usando JQ](04_2__Interludio_Usando_JQ.md)
  * [4.3: Creando una Transacción Cruda con Argumentos Ingresados con Nombre](04_3_Creando_una_Transaccion_Cruda_con_Argumentos_Ingresados_con_Nombre.md)
  * [4.4: Enviando Monedas con una Transacción Cruda](04_4_Enviando_Monedas_con_una_Transaccion_Cruda.md)
     * [Interludio: Usando Curl](04_4__Interludio_Usando_Curl.md)
  * [4.5: Enviando Monedas con Transacciones Crudas Automatizadas](04_5_Enviando_Monedas_con_Transacciones_Crudas_Automatizadas.md)
  * [4.6: Creando una Transacción Segwit](04_6_Creando_una_Transaccion_Segwit.md)
* [5.0: Controlando Transacciones Bitcoin](05_0_Controlando_Transacciones_Bitcoin.md)
  * [5.1 Vigilando por Transacciones Estancadas](05_1_Vigilando_por_Transacciones_Estancadas.md)
  * [5.2: Reenviando una Transacción con RBF](05_2_Reenviando_a_Transaccion_con_RBF.md)
  * [5.3: Financiando una Transacción con CPFP](05_3_Financiando_una_Transaccion_con_CPFP.md)
* [6.0: Expandiendo las Transacciones Bitcoin con Multifirmas](06_0_Expandiendo_las_Transacciones_Bitcoin_con_Multifirmas.md)
  * [6.1: Enviando una Transacción a una Dirección Multifirma](06_1_Enviando_una_Transaccion_a_una_Direccion_Multifirma.md)
  * [6.2: Gastando una Transacción con una Dirección Multifirma](06_2_Gastando_una_Transaccion_con_una_Direccion_Multifirma.md)
  * [6.3: Enviando y Gastando una Multifirma Automatizada](06_3_Enviando_una_Multifirma_Automatizada.md)
* [7.0: Expandiendo las Transacciones Bitcoin con PSBTs](07_0_Expandiendo_las_Transacciones_Bitcoin_con_PSBTs.md)
  * [7.1: Creando una Transacción Bitcoin Parcialmente Firmada](07_1_Creando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md)
  * [7.2: Usando una Transacción Bitcoin Parcialmente Firmada](07_2_Usando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md)
  * [7.3: Integrando con Hardware Wallets](07_3_Integrando_con_Hardware_Wallets.md)
* [8.0: Expandiendo Transacciones Bitcoin en Otros Sentidos](08_0_Expandiendo_Bitcoin_Transacciones_Otros.md)  
  * [8.1: Enviando una Transacción con Bloqueo de Tiempo](08_1_Enviando_una_Transaccion_con_Bloqueo_de_Tiempo.md)
  * [8.2: Enviando una Transacción con Datos](08_2_Enviando_una_Transaccion_con_Datos.md)
  
### PARTE TRES: BITCOIN SCRIPTING

**Estado:** Finalizado. Actualizado para 0.20 y btcdeb.

* [9.0: Introduciendo Bitcoin Scripts](09_0_Introduciendo_Bitcoin_Scripts.md)
  * [9.1: Entendiendo la Base de las Transacciones](09_1_Entendiendo_la_Base_de_las_Transacciones.md)
  * [9.2: Ejecutando un Script Bitcoin](09_2_Ejecutando_un_Script_Bitcoin.md)
  * [9.3: Probando un Script Bitcoin](09_3_Probando_un_Script_Bitcoin.md)
  * [9.4: Codificando una P2PKH](09_4_Codificando_una_P2PKH.md)
  * [9.5: Codificando una P2WPKH](09_5_Codificando_una_P2WPKH.md)
* [10.0: Embebiendo Bitcoin Scripts en Transacciones P2SH](10_0_Embebiendo_Bitcoin_Scripts_en_Transacciones_P2SH.md)
  * [10.1: Entendiendo la Base de P2SH](10_1_Entendiendo_la_Base_de_P2SH.md)
  * [10.2: Construyendo la Estructura de P2SH](10_2_Construyendo_la_Estructura_de_P2SH.md)
  * [10.3: Ejecutando un Script Bitcoin con P2SH](10_3_Ejecutando_un_Script_Bitcoin_con_P2SH.md)
  * [10.4: Codificando una Multifirma](10_4_Codificando_una_Multifirma.md)
  * [10.5: Codificando un Script Segwit](10_5_Codificando_un_Script_Segwit.md)
  * [10.6: Gastando una Transacción P2SH](10_6_Gastando_una_Transaccion_P2SH.md)
* [11.0: Potenciando Bloqueos de Tiempo con Bitcoin Scripts](11_0_Potenciando_Bloqueos_de_Tiempo_con_Bitcoin_Scripts.md)
  * [11.1: Entendiendo las Opciones de los Bloqueos de Tiempo](11_1_Entendiendo_las_Opciones_de_los_Bloqueos_de_Tiempo.md)
  * [11.2: Usando CLTV en Scripts](11_2_Usando_CLTV_en_Scripts.md)
  * [11.3: Usando CSV en Scripts](11_3_Usando_CSV_en_Scripts.md)
* [12.0: Expandiendo Scripts Bitcoin](12_0_Expandiendo_Scripts_Bitcoin.md)
  * [12.1: Usando Scripts Condicionales](12_1_Usando_Scripts_Condicionales.md)
  * [12.2: Usando Otros Comandos de Scripting](12_2_Usando_Otros_Comandos_de_Scripting.md)
* [13.0: Diseñando Bitcoin Scripts Reales](13_0_Disenando_Bitcoin_Scripts_Reales.md)
  * [13.1: Escribiendo Puzzles Scripts](13_1_Escribiendo_Puzzle_Scripts.md)
  * [13.2: Escribiendo Scripts Multifirma Complejos](13_2_Escribiendo_Scripts_Multifirma_Complejos.md)
  * [13.3: Potenciando Bitcoin con Scripts](13_3_Potenciando_Bitcoin_con_Scripts.md)

### PARTE CUATRO: PRIVACIDAD

**Estado:** Finalizado.

* [14.0: Usando Tor](14_0_Usando_Tor.md)
  * [14.1: Verificando Su Configuración Tor](14_1_Verificando_Su_Configuracion_Tor.md)
  * [14.2: Cambiando Sus Servicios Bitcoin Ocultos](14_2_Cambiando_Sus_Servicios_Bitcoin_Ocultos.md)
  * [14.3: Agregando Servicios SSH Ocultos](14_3_Agregando_Servicios_SSH_Ocultos.md)

* [15.0: Usando i2p](15_0_Usando_i2p.md)
    * [15.1: Bitcoin Core como un servicio I2P (Proyecto Internet Invisible)](15_1_i2p_service.md)

### PARTE CINCO: PROGRAMANDO CON RPC

**Estado:** Finalizado.

* [16.0: Hablando a Bitcoind con C](16_0_Hablando_a_Bitcoind_con_C.md)
  * [16.1: Accediendo a Bitcoind en C con las Bibliotecas RPC](16_1_Accediendo_a_Bitcoind_en_C_con_las_Bibliotecas_RPC.md)
  * [16.2: Programando Bitcoind en C con las Bibliotecas RPC](16_2_Programando_Bitcoind_en_C_con_las_Bibliotecas_RPC.md)
  * [16.3: Recibiendo Notificaciones de Bitcoind en C con las Bibliotecas ZMQ](16_3_Recibiendo_Notificaciones_de_Bitcoind_en_C_con_las_Bibliotecas_ZMQ.md)
* [17.0: Programando Bitcoin con Libwally](17_0_Programando_Bitcoin_con_Libwally.md)
   * [17.1: Configurando Libwally](17_1_Configurando_Libwally.md)
   * [17.2: Usando BIP39 en Libwally](17_2_Usando_BIP39_en_Libwally.md)
   * [17.3: Usando BIP32 en Libwally](17_3_Usando_BIP32_en_Libwally.md)
   * [17.4: Usando PSBTs en Libwally](17_4_Usando_PSBTs_en_Libwally.md)
   * [17.5: Usando Scripts en Libwally](17_5_Usando_Scripts_en_Libwally.md)
   * [17.6: Usando Otras Funciones en Libwally](17_6_Usando_Otras_Funciones_en_Libwally.md)
   * [17.7: Integrando Libwally y Bitcoin-CLI](17_7_Integrando_Libwally_y_Bitcoin-CLI.md)
* [18.0: Hablando a Bitcoind con Otros Lenguajes](18_0_Hablando_a_Bitcoind_con_Otros_Lenguajes.md)
  * [18.1: Accediendo a Bitcoind con Go](18_1_Accediendo_a_Bitcoind_con_Go.md)
  * [18.2: Accediendo a Bitcoind con Java](18_2_Accediendo_a_Bitcoind_con_Java.md)
  * [18.3: Accediendo a Bitcoind con Node JS](18_3_Accediendo_a_Bitcoind_con_NodeJS.md)
  * [18.4: Accediendo a Bitcoind con Python](18_4_Accediendo_a_Bitcoind_con_Python.md)
  * [18.5: Accediendo a Bitcoind con Rust](18_5_Accediendo_a_Bitcoind_con_Rust.md)
  * [18.6: Accediendo a Bitcoind con Swift](18_6_Accediendo_a_Bitcoind_con_Swift.md)

### PARTE SEIS: USANDO LIGHTNING-CLI

**Estado:** Finalizado.

* [19.0: Entendiendo Su Configuración Lightning](19_0_Entendiendo_Su_Configuracion_Lightning.md)
  * [19.1: Verificando Su Configuración c-lightning](19_1_Verificando_Su_Configuracion_Lightning.md)
  * [19.2: Conociendo Su Configuración c-lightning](19_2_Conociendo_Su_Configuracion_Lightning.md)
     * [Interludio: Accediendo a un Segundo Nodo Lightning](19_2__Interludio_Accediendo_a_un_Segundo_Nodo_Lightning.md)
  * [19.3: Creando un Canal en Lightning](19_3_Creando_un_Canal_en_Lightning.md)
* [20.0: Usando Lightning](20_0_Usando_Lightning.md)
  * [20.1: Generando una Solicitud de Pago](20_1_Generando_una_Solicitud_de_Pago.md)
  * [20.2: Pagando una Factura](20_2_Pagando_una_Factura.md)
  * [20.3: Cerrando un Canal Lightning]((20_3_Cerrando_un_Canal_Lightning.md)
  * [20.4: Expandiendo la Red Lightning](20_4_Expandiendo_la_Red_Lightning.md)
   
### APÉNDICES

**Estado:** Finalizado.

* [Apéndices](A0_Apendices.md)
  * [Apéndice I: Entendiendo la Configuración Inicial de Bitcoin](A1_0_Entendiendo_la_Configuracion_Inicial_de_Bitcoin.md)
  * [Apéndice II: Compilando Bitcoin desde la Fuente](A2_0_Compilando_Bitcoin_desde_la_Fuente.md)
  * [Apéndice III: Usando Bitcoin Regtest](A3_0_Usando_Bitcoin_Regtest.md)
  
## Estado - Beta

v2.0.1 de **Aprendiendo Bitcoin desde la Línea de Comandos** esta completa. Puede haber aun reviciones o ediciones de terceros, pero puede ser usada para aprender.

Estamos considerando tentativamente aquello que podríamos incluir en una [v3.0](TODO-30.md) del curso. Si le gustaría apoyar un trabajo de este estilo, conviértase en un [GitHub Sponsor](https://github.com/sponsors/BlockchainCommons) o apoyenos en nuestro [BTCPay Server](https://btcpay.blockchaincommons.com/), y háganos saber que fue debido a **Aprendiendo Bitcoin**.

## Origen, Autores, Copyright & Licencias

A menos que sea señalado de otra manera (ya sea en este [LEEME.md](./LEEME.md) o en los comentarios en los encabezados de los archivos) el contenido de este repositorio es Copyright © 2020 por Blockchain Commons, LLC, y esta bajo licencia [CC-BY](./LICENSE-CC-BY-4.0.md).

## Apoyo Financiero

*Aprendiendo Bitcoin desde la Línea de Comandos* es un proyecto de [Blockchain Commons](https://www.blockchaincommons.com/). Somos orgullosamente una corporación para beneficio social sin fines de lucro comprometida con el código abierto y el desarrollo abierto. Nuestro trabajo es financiado enteramente por donaciones y camaradería colaborativa con gente como usted. Toda contribución va a ser destinada a construir herramientas abiertas, tecnologías y técnicas que sostengan una avanzada cadena de bloques, una infraestructura de internet segura y promuevan la web abierta.

Para apoyar financieramente el futuro desarrollo de *Aprendiendo Bitcoin desde la Línea de Comandos* y otros proyectos, por favor, considere convertirse en un patrocinador de Blockchain Commons a través de un patrocinio mensual continuo como [GitHub Sponsor](https://github.com/sponsors/BlockchainCommons). También puede apoyar Blockchain Commons con bitcoins en nuestro [BTCPay Server](https://btcpay.blockchaincommons.com/).

## Contribuyendo <a name="contribuyendo"></a>

Animamos las contribuciones publicas a través de issues y pull requests! Por favor, revise [CONTRIBUYENDO.md](./CONTRIBUYENDO.md) para mas detalles sobre nuestro proceso de desarrollo. Todas las contribuciones a este repositorio requieren un [Acuerdo de Licencia de Colaborador](./CLA.md) firmado mediante GPG.

Si le gustaría proveer una traducción de Aprendiendo Bitcoin desde la Línea de Comandos, por favor, vea también [TRADUCIENDO.md](./TRADUCIENDO.md).

### Debates

El mejor lugar para hablar acerca de Blockchain Commons y sus proyectos es en nuestras áreas de debate en GitHub.

[**Debates de Blockchain Commons**](https://github.com/BlockchainCommons/Community/discussions). Para desarrolladores, pasantes y patrocinadores de Blockchain Commons, por favor usar las áreas de debate de [Repositorio Comunitario](https://github.com/BlockchainCommons/Community) para hablar sobre asuntos generales de Blockchain Commons, el programa de pasantías, o temas distintos al [Sistema Gordiano](https://github.com/BlockchainCommons/Gordian/discussions) o los [Estándares de las wallets](https://github.com/BlockchainCommons/AirgappedSigning/discussions), cada uno de los cuales tiene sus propias áreas de debate.

### Otras Preguntas & Problemas

Como una comunidad de código abierto y desarrollo abierto, Blockchain Commons no tiene los recursos para proveer apoyo directo a nuestros proyectos. Por favor, considere las áreas de debate como un lugar donde podría obtener respuesta a sus preguntas. Alternativamente, use por favor las características de este repositorio [issues](./issues). Desafortunadamente, no podemos hacer ninguna promesa sobre el tiempo de respuesta.

Si su compañía requiere soporte para usar nuestros proyectos, por favor, siéntase libre de contactarnos directamente sobre opciones. Nosotros tal vez estemos en condiciones de ofrecerle un contrato para soporte de alguno de nuestros contribuyentes, o estemos en condiciones de dirigirlo hacia otra entidad la cual pueda ofrecerle el soporte contractual que usted necesita.

### Créditos

Las siguientes personas contribuyeron directamente a este repositorio. Usted puede agregar su nombre involucrándose. El primer paso es aprender a contribuir desde nuestra documentación [CONTRIBUYENDO.md](./CONTRIBUYENDO.md).


| Nombre              | Rol                | Github                                            | Correo Electrónico                                 | GPG Huella Digital                                    |
| ----------------- | ------------------- | ------------------------------------------------- | ------------------------------------- | -------------------------------------------------- |
| Christopher Allen | Autor Principal | [@ChristopherA](https://github.com/ChristopherA) | \<ChristopherA@LifeWithAlacrity.com\> | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |
| Shannon Appelcline | Autor Principal | [@shannona](https://github.com/shannona) | \<shannon.appelcline@gmail.com\> | 7EC6 B928 606F 27AD | 


Contribuciones adicionales listadas abajo:

| Rol                   | Nombre                                             |
| --------------------- | -------------------------------------------------- |
| ***Contribuyentes:*** | <ul><li>[gg2001](https://github.com/gg2001) (Go, Node.js sections)</li><li>[gorazdko](https://github.com/gorazdko) (Rust section)</li><li>[Javier Vargas](https://github.com/javiervargas) (C, Java, Lightning, Tor sections)</li><li>[jodobear](https://github.com/jodobear) (Appendix: Compiling Bitcoin, Python section)</li></ul> |
| ***Revisores:***      | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Patrocinadores:*** | Blockstream Corporation                            |


# Divulgación Responsable

Queremos mantener todo nuestro software seguro para todo el mundo. Si descubre alguna vulnerabilidad de seguridad, apreciaríamos su ayuda en revelarla a nosotros de una manera responsable. Desafortunadamente no somos capaces de ofrecer recompensas por errores en este momento.

Le pedimos que nos brinde su buena fe y use sus mejores esfuerzos para no filtrar información que pueda dañar a algún usuario, sus datos, o nuestra comunidad de desarrolladores. Por favor, denos una cantidad de tiempo razonable para arreglar el asunto antes de publicarlo. No defraude a nuestros usuarios en el proceso de descubrimiento. Prometemos no emprender acciones legales contra investigadores que señalen problemas dado que realicen su mejor esfuerzo para seguir estas pautas.

### Reportando una Vulnerabilidad

Por favor, reporte vulnerabilidades de seguridad sospechosas en privado vía un correo electrónico a ChristopherA@BlockchainCommons.com (no use este correo para soporte). Por favor, NO cree issues públicamente visibles por vulnerabilidades de seguridad sospechosas.

Las siguientes claves pueden ser usadas para comunicar información sensible a los desarrolladores:

| Nombre            | Huella Digital                                     |
| ----------------- | -------------------------------------------------- |
| Christopher Allen | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |

Puede importar una llave ejecutando el siguiente comando con esa huella digital en particular: `gpg --recv-keys "<fingerprint>"` Asegúrese que coloca comillas alrededor de las huellas digitales que contienen espacios.
