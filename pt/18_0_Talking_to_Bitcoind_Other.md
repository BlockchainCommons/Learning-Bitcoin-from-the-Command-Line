# Capítulo 17: Conversando com o Bitcoind com Outras Linguagens

Agora devemos ter uma base sólida para trabalhar com Bitcoin usando a linguagem C, não apenas usando as bibliotecas RPC, JSON e ZMQ para interagir diretamente com o `bitcoind`, mas também, utilizando as bibliotecas Libwally para complementar esse trabalho. O C é uma ótima linguagem para prototipagem e abstração, porém provavelmente não é o que estamos usando para programar. Este capítulo, portanto, faz uma apresentação rápida por seis outras linguagens de programação, demonstrando a funcionalidade mais básica do Bitcoin em cada uma, permitindo que possamos expandir as lições da linha de comando e do C para a linguagem de programação que desejarmos escolha.

Cada uma das seções contém aproximadamente as mesmas informações, o foco será: Criar uma conexão RPC, examinar a carteira, criando um novo endereço e criar uma transação. No entanto, há alguma mudança entre as linguagens, mostrando diferentes aspectos dos comandos RPC do Bitcoin em diferentes exemplos. Em particular, algumas linguagens usam a metodologia fácil do `sendtoaddress`, enquanto outras usam uma metodologia mais difícil para a criação de uma transação bruta do zero.

## Objetivos deste capítulo

Depois de trabalhar neste capítulo, um desenvolvedor será capaz de:

   * Preparar ambientes de desenvolvimento para o Bitcoin para uma variedade de linguagens;
   * Usar as funções da carteira em várias linguagens;
   * Usar funções de transação em uma variedade de linguagens.
   
Os objetivos secundários do capítulo incluem a capacidade de:

  * Saiber mais sobre o Bitcoin RPC por meio de interações com uma variedade de linguagens.
   
## Tabela de Conteúdo

  * [Seção 1: Acessando o Bitcoind com Go](17_1_Accessing_Bitcoind_with_Go.md)
  * [Seção 2: Acessando o Bitcoind com Java](17_2_Accessing_Bitcoind_with_Java.md)
  * [Seção 3: Acessando o Bitcoind com NodeJS](17_3_Accessing_Bitcoind_with_NodeJS.md)
  * [Seção 4: Acessando o Bitcoind com Python](17_4_Accessing_Bitcoind_with_Python.md)
  * [Seção 5: Acessando o Bitcoind com Rust](17_5_Accessing_Bitcoind_with_Rust.md)
  * [Seção 6: Acessando o Bitcoind com Swift](17_6_Accessing_Bitcoind_with_Swift.md)