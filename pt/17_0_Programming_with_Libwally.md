# Capítulo 16: Programando com Libwally

O capítulo anterior apresentou três bibliotecas C, para RPC, JSON e ZMQ, todas destinadas a interagir diretamente com o `bitcoind`, assim como você vem fazendo desde o início. Mas, às vezes você pode querer codificar sem acesso direto a um `bitcoind`. Isso pode ser devido a um cliente offline ou apenas porque você deseja manter algumas funcionalidades internas de seu programa C. Você também pode querer se aprofundar na funcionalidade da carteira, como criação de palavras mnemônicas ou derivação de endereços. É aí que entra Libwally: é uma biblioteca de carteira para C, C ++, Java, NodeJS ou Python, com wrappers também disponíveis para outras linguagens, como Swift.

Este capítulo aborda a funcionalidade possível dentro do Libwally, a maioria das quais complementa o trabalho que você fez através do acesso RPC ao `bitcoind`, mas algumas das quais o replicam. Ele também mostra como integrar esse trabalho com os clientes RPC com os quais você está mais familiarizado. No entanto, observe que esta é apenas uma introdução básica ao Libwally. Vários de seus conjuntos de funções mais importantes são destacados, mas nunca fazemos mais do que enfiar os pés no chão. Se você acha suas funções úteis ou intrigantes, então você precisará se aprofundar muito mais profundamente do que este curso pode cobrir.

## Objetivos deste capítulo

Depois de trabalhar neste capítulo, um desenvolvedor será capaz de:

   * Usar as funções da carteira com o Libwally;
   * Realizar manipulações de PSBTs e transações com o Libwally;
   * Implementar designs que combinem o Libwally e o RPC.
   
Os objetivos secundários do capítulo incluem a capacidade de:

  * Compreender palavras mnemônicas BIP39;
  * Conhecer mais sobre as carteiras hierárquicas BIP32;
  * Sintetizar a profundidade funcional do Libwally.
   
## Tabela de Conteúdo

  * [Seção 1: Configurando o Libwally](16_1_Setting_Up_Libwally.md)
  * [Seção 2: Usando o BIP39 no Libwally](16_2_Using_BIP39_in_Libwally.md)
  * [Seção 3: Usando o BIP32 no Libwally](16_3_Using_BIP32_in_Libwally.md)
  * [Seção 4: Usando o PSBTs no Libwally](16_4_Using_PSBTs_in_Libwally.md)
  * [Seção 5: Usando Scripts no Libwally](16_5_Using_Scripts_in_Libwally.md)
  * [Seção 6: Usando Outras Funções no Libwally](16_6_Using_Other_Functions_in_Libwally.md)
  * [Seção 7: Integrando o Libwally ao Bitcoin-CLI](16_7_Integrating_Libwally_and_Bitcoin-CLI.md)