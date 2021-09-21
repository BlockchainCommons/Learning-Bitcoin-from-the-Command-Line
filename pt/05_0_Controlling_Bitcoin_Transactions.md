# Capítulo 5: Controlando Transações no Bitcoin

O envio de uma transação nem sempre termina com um "viveram felizes para sempre". Usando os protocolos RBF (Replace-By-Fee, ou Substituindo-A -Taxa no português) e CPFP (Child-Pays-For-Parent, ou Filho-Paga-Pelo-Pai), um desenvolvedor pode continuar a controlar a transação após ela ter sido enviada, para melhorar a eficiência ou para recuperar transações que estava presas na _mempool_. Esses métodos irão começar a mostrar o verdadeiro poder do Bitcoin.

## Objetivos deste Capítulo

Depois de trabalhar neste capítulo, um desenvolvedor será capaz de:

   * Decidir se o RBF ou o CPFP pode ajudar uma transação;
   * Criar uma transação de substituição usando o RBF;
   * Criar novas transações usando o protocolo CPFP.

Os objetivos secundários do capítulo incluem a capacidade de:

   * Entender a Mempool;
   * Entender a diferença entre o RBF e o CPFP;
   * Planejar a taxa do RBF.

## Tabela de Conteúdo
   
   * [Seção 1: Atentando-se para Transações Presas](05_1_Watching_for_Stuck_Transactions.md)
   * [Seção 2: Reenviando uma Transação com RBF](05_2_Resending_a_Transaction_with_RBF.md)
   * [Seção 3: Financiando uma Transação com CPFP](05_3_Funding_a_Transaction_with_CPFP.md)