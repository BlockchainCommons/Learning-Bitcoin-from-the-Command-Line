# 2.2: Configurando um node Bitcoin Core por outros métodos

Este documento explica como configurar um VPS (Virtual Private Server) para executar um node Bitcoin em linode.com, instalado usando um StackScript automatizado do [Bitcoin Standup project](https://github.com/BlockchainCommons/Bitcoin-Standup -Scripts). Você só precisa inserir alguns comandos e inicializar seu VPS. Quase imediatamente após inicializar, você encontrará seu novo node Bitcoin baixando blocos alegremente.

A seguir estão outras metodologias de configuração que conhecemos:

* *[Compilando da fonte](A2_0_Compiling_Bitcoin_from_Source.md).* Se você preferir compilar o Bitcoin Core manualmente, isso é abordado no Apêndice 2.
* *[Usando GordianNode-macOS](https://github.com/BlockchainCommons/GordianNode-macOS).* Se você tem um Mac moderno, pode usar o Software GordianNode do Blockchain Commons, desenvolvido por BitocinStandup, para instalar um nó completo em seu Mac.
* *[Usando outro Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* Blockchain Commons também oferece uma versão do script Linode que você usou, que pode ser executado a partir da linha de comando em qualquer máquina Debian ou Ubuntu. Este tende a ser o script de ponta, o que significa ser provável que apresentem novas funções, como a instalação do Lightning.
* *[Configurando um Bitcoin node na AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally escreveu um tutorial passo a passo para configurar Bitcoin-Core com Amazon Web Services (AWS).
* *[Configurando um node Bitcoin em um Raspberry Pi3](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee explica como configurar um full-node Bitcoin em um Raspberry Pi 3.

## Qual é o próximo?

A menos que você queira retornar a uma das outras metodologias para criar um seu node Bitcoin, você deve:

   * Mova-se para "bitcoin-cli" [Capitulo três: Compreendendo a configuração do seu node Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).
