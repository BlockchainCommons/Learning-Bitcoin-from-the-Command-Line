# Capítulo 3: Compreendendo Nossa Configuração do Bitcoin

Agora que estamos prontos para começar a trabalhar com a interface de linha de comando `bitcoin-cli`. Mas isso requer primeiro que entendamos a configuração do Bitcoin e os recursos da carteira, que é o que será explicado neste capítulo.

Neste e nos próximos capítulos, presumimos que uma VPS com Bitcoin instalado esteja sendo utilizada, e que iremos executar o `bitcoind`. Também presumimos que estamos conectados à testnet, permitindo o acesso a bitcoins sem usar fundos reais. Podemos fazer isso com Bitcoin Standup em linode.com, como vimos na sessão [2.1: Configurando um Bitcoin-Core VPS com Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), ou usando os demais métodos, descritos na sessão [2.2: Configurando uma Máquina Bitcoin Core de Outras Maneiras](02_2_Setting_Up_Bitcoin_Core_Other.md).

## Objetivos deste Capítulo

Depois de trabalhar neste capítulo, um desenvolvedor será capaz de:

   * Demonstrar que o node Bitcoin está instalado e atualizado;
   * Criar um endereço para receber bitcoins;
   * Usar os comandos básicos da carteira;
   * Criar um endereço a partir de um descritor.

Os objetivos secundários incluem a capacidade de:

   * Compreender o layout básico do arquivo Bitcoin;
   * Usar comandos informativos básicos;
   * Entender o que é um endereço Bitcoin;
   * Entender o que é uma carteira;
   * Entender como importar endereços.

## Tabela de Conteúdo

* [Seção Um: Verificando Nossa Configuração do Bitcoin](03_1_Verifying_Your_Bitcoin_Setup.md)
* [Seção Dois: Conhecendo Nossa Configuração do Bitcoin](03_2_Knowing_Your_Bitcoin_Setup.md)
* [Seção Três: Configurando Nossa Carteira](03_3_Setting_Up_Your_Wallet.md)
   * [Usando Variáveis de Linha de Comando](03_3__Interlude_Using_Command-Line_Variables.md)
* [Seção Quatro: Recebendo uma Transação](03_4_Receiving_a_Transaction.md)
* [Seção Cinco: Compreendendo o Descritor](03_5_Understanding_the_Descriptor.md)