# Capítulo 5: Controlando as transações de Bitcoin

O envio de uma transação nem sempre termina com um "viveram felizes para sempre". Usando os protocolos RBF (Replace-By-Fee, ou Substituindo-A -Taxa no português) e CPFP (Child-Pays-For-Parent, ou Filho-Paga-Pelo-Pai), um desenvolvedor pode continuar a controlar a transação após ela ter sido enviada, para melhorar a eficiência ou para recuperar transações que estava presas na _mempool_. Esses métodos irão começar a mostrar o verdadeiro poder do Bitcoin.

## Objetivos deste capítulo

Depois de trabalhar neste capítulo, um desenvolvedor será capaz de:

   * Decidir se o RBF ou o CPFP pode ajudar uma transação;
   * Criar uma transação de substituição usando o RBF;
   * Criar novas transações usando o protocolo CPFP.

Os objetivos secundários do capítulo incluem a capacidade de:

   * Entender a Mempool;
   * Entender a diferença entre o RBF e o CPFP;
   * Planejar a taxa do RBF.

## Tabela de conteúdo
   
   * [Seção 1: Observando as transações presas na mempool](05_1_Watching_for_Stuck_Transactions.md)
   * [Seção 2: Reenviando uma transação com o RBF](05_2_Resending_a_Transaction_with_RBF.md)
   * [Seção 3: Financiando uma transação com o CPFP](05_3_Funding_a_Transaction_with_CPFP.md)