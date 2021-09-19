# Capítulo 15: Usando o I2P

Existem alternativas ao Tor. Uma é o Projeto Internet Invisível (I2P, ou Invisible Internet Project em inglês), uma camada de rede privada e totalmente criptografada. Ele utiliza um [banco de dados de rede](https://geti2p.net/pt-br/docs/how/network-database) distribuído e túneis criptografados unidirecionais entre os usuários. A maior diferença do I2P para o Tor é que o segundo é, em essência, uma rede de proxy que oferece serviços de internet de uma forma privada, enquanto que o I2P é fundamentalmente uma rede separada que oferece serviços I2P apenas à rede I2P, criando uma "rede dentro de uma rede". No entanto, podemos querer tê-la apenas como uma alternativa, para que não dependamos apenas do Tor.

No momento, o I2P não está instalada com o [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts) porque o suporte ao I2P foi adicionado recentemente ao Bitcoin Core. No entanto, neste capítulo, iremos explicar como podemos instalá-lo manualmente.

## Objetivos para Este Capítulo

Depois de trabalharmos neste capítulo, um desenvolvedor será capaz de:

- Executar  o Bitcoin Core como um serviço I2P.

Objetivos secundários incluem a habilidade de:

- Compreender a rede I2P;
- Aprender a diferença entre o Tor e o I2P.

## Tabela de Conteúdo

* [Seção Um: Bitcoin Core como um Serviço I2P (Projeto Internet Invisível)](15_1_i2p_service.md)