# Capítulo 16: Hablar con Bitcoind con C

Mientras trabajamos con Bitcoin Scripts, llegamos a los límites de lo que es posible con bitcoin-cli: actualmente no se puede usar para generar transacciones que contengan scripts inusuales. Los scripts de Shell tampoco son buenos para algunas cosas, como la creación de programas de escucha que sondean constantemente. Afortunadamente, existen otras formas de acceder a la red Bitcoin: API de programación.

Esta sección se centra en tres bibliotecas diferentes que pueden usarse como base de la programación C sofisticada: una biblioteca RPC y una biblioteca JSON juntas le permiten recrear mucho de lo que hizo en scripts de shell, pero ahora usando C; mientras que una biblioteca ZMQ lo vincula a notificaciones, algo a lo que no ha podido acceder anteriormente. (El próximo capítulo cubrirá una biblioteca aún más sofisticada llamada Libwally, para terminar este vistazo introductorio a la programación de Bitcoin con C.)

## Objetivos de este capítulo

Después de trabajar en este capítulo, un desarrollador podrá:

   * Crear programas en C que usen RPC para hablar con Bitcoind
   * Crear programas en C que usen ZMQ para hablar con Bitcoind
   
Los objetivos de apoyo incluyen la capacidad de:

   * Comprender cómo usar una biblioteca RPC
   * Comprender cómo usar una biblioteca JSON
   * Comprender las capacidades de ZMQ
   * Comprender cómo usar una biblioteca ZMQ
   
## Tabla de contenido

  * [Sección uno: Acceso a Bitcoind en C con bibliotecas RPC](16_1_Accediendo_a_Bitcoind_en_C_con_las_Bibliotecas_RPC.md)
  * [Sección dos: Programación de Bitcoind en C con bibliotecas RPC](16_2_Programando_Bitcoind_en_C_con_las_Bibliotecas_RPC.md)
  * [Sección tres: Recibir notificaciones en C con bibliotecas ZMQ](16_3_Recibiendo_Notificaciones_de_Bitcoind_en_C_con_las_Bibliotecas_ZMQ.md)
