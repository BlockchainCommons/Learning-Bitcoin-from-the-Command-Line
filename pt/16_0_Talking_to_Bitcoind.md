# Capítulo 16: Conversando com Bitcoind com C

Enquanto trabalhamos com Bitcoin Scripts, atingimos os limites do que era possível com o `bitcoin-cli`: Atualmente, ele não pode ser usado para gerar transações contendo scripts incomuns. Os scripts shell também não são bons para algumas coisas, como criar programas de escuta que estão constantemente em polling. Felizmente, existem outras maneiras de acessar a rede Bitcoin: Através de APIs programáveis.

Esta seção se concentra em três diferentes bibliotecas que podem ser usadas como base de programação C sofisticada: Uma biblioteca RPC e uma biblioteca JSON que juntas permitem recriar muito do que fazemos nos scripts de shell, porém, usando C; enquanto uma biblioteca ZMQ nos conecta a notificações, algo que não conseguiríamos acessar até agora. (O próximo capítulo cobrirá uma biblioteca ainda mais sofisticada chamada Libwally, para finalizar esta introdução à programação do Bitcoin com C).

## Objetivos deste capítulo

Depois de trabalhar neste capítulo, um desenvolvedor será capaz de:

   * Criar programas C que usam RPC para conversar com o Bitcoind;
   * Criar programas C que usam ZMQ para conversar com o Bitcoind.

Os objetivos secundários do capítulo incluem a capacidade de:

   * Entender como usar uma biblioteca RPC;
   * Entender como usar uma biblioteca JSON;
   * Compreender as capacidades do ZMQ;
   * Entender como usar uma biblioteca ZMQ.

## Tabela de Conteúdo

  * [Seção 1: Acessando o Bitcoind em C com Bibliotecas RPC](16_1_Accessing_Bitcoind_with_C.md)
  * [Seção 2: Programando o Bitcoind em C com Bibliotecas RPC](16_2_Programming_Bitcoind_with_C.md)
  * [Seção 3: Recebendo Notificações em C com Bibliotecas ZMQ](16_3_Receiving_Bitcoind_Notifications_with_C.md)
   