# 16.1: Acceso a Bitcoind en C con bibliotecas RPC

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con advertencias.

Ya ha visto una forma alternativa de acceder a los puertos RPC de Bitcoind: `curl`, que se cubrió en un [Interludio del Capítulo 4](04_4__Interludio_Usando_Curl.md). Interactuar con  `bitcoind` a través de una biblioteca RPC en C no es diferente a eso, solo necesita algunas buenas bibliotecas para ayudarlo. Esta sección presenta un paquete llamado  `libbitcoinrpc`, que le permite acceder al puerto  `bitcoind`.  Utiliza una biblioteca `curl` para acceder a los datos y usa la biblioteca `jansson`  para codificar y decodificar el JSON.

## Configurar libbitcoinrpc

