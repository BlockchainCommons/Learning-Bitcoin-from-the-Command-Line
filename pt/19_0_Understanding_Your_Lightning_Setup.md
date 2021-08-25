# Capítulo 18: Compreendendo a configuração da Lightning

> :information_source: **NOTA:** Este é um rascunho que está em andamento. Seu objetivo é que possa obter alguns comentários dos revisores iniciais. Ainda não está pronto para ser produzido.

O capítulo anterior concluiu nosso trabalho com o Bitcoin propriamente dito, por meio do CLI, scripts e linguagens de programação. No entanto, existem muitos outros utilitários dentro do ecossistema Bitcoin. Neste capítulo e no próximo, iremos cobrir o que pode ser o maior e mais importante deles, a Lightning Network. Aqui, começaremos a trabalhar com a interface de linha de comando `lightning-cli`, entendendo a configuração do c-lightning e dos seus recursos, incluindo alguns exemplos e configuração básica.

## Objetivos deste capítulo

Depois de trabalhar neste capítulo, um desenvolvedor será capaz de:

   * Avaliar se um node c-lightning está instalado e atualizado;
   * Executar comandos básicos de uma carteira Lightning ;
   * Criar um canal na Lightning.
   
Os objetivos secundários do capítulo incluem a capacidade de:

   * Compreender a configuração básica da Lightning;
   * Compreender a interação dos canais Lightning;
   * Entender como usar a Lightning.

## Tabela de Conteúdo

* [Seção 1: Verificando a configuração do c-lightning](18_1_Verifying_Your_Lightning_Setup.md)
* [Seção 2: Conhecendo a configuração do c-lightning](18_2_Knowing_Your_lightning_Setup.md)
   * [Adendo: Acessando um segundo node Lightning](18_2__Interlude_Accessing_a_Second_Lightning_Node.md)
* [Seção 3: Criando um canal na Lightning](18_3_Setting_Up_a_Channel.md)