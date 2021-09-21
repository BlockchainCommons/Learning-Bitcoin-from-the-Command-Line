# Usando Variáveis de Linha de Comando

A sessão anterior demonstrou vários comandos de linha de comando usados sem ofuscação ou interferência. No entanto, geralmente essa não é a melhor maneira de executar o Bitcoin usando a linha de comando. Como estamos lidando com variáveis longas, complexas e difíceis de serem lidas, é fácil cometer um erro se estivermos copiando essas variáveis (ou, perder alguns satoshis, se as digitarmos manualmente). Como essas variáveis podem significar a diferença entre receber e perder dinheiro quando usarmos a Mainnet, não _queremos_ cometer erros. Por esses motivos, sugerimos enfaticamente o uso de variáveis de linha de comando para salvar endereços, assinaturas ou outras cadeias de informações longas sempre que isso for razoável.

Se estiver usando `bash`, podemos salvar as informações em uma variável como esta:
```
$ VARIABLE=$(command)
```

Esta é uma substituição de um comando simples, o equivalente a `VARIABLE = command`. O comando entre parênteses é executado e, em seguida, atribuído à VARIABLE.

Para criar um novo endereço, seria assim:
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress "" legacy)
```
Esses comandos limpam a variável NEW_ADDRESS_1, apenas para ter certeza, e então a preenchem com os resultados do comando `bitcoin-cli getnewaddress`.

Podemos então usar o comando `echo` do shell para vermos nosso (novo) endereço:
```
$ echo $NEW_ADDRESS_1
mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE
```
Como nosso endereço está em uma variável, agora podemos assinar facilmente uma mensagem para esse endereço, sem nos preocupar em digitar o endereço incorretamente. É claro que também salvaremos essa assinatura em uma variável!
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
$ echo $NEW_SIG_1
IPYIzgj+Rg4bxDwCyoPiFiNNcxWHYxgVcklhmN8aB2XRRJqV731Xu9XkfZ6oxj+QGCRmTe80X81EpXtmGUpXOM4=
```
O restante deste tutorial usará esse estilo de armazenamento de informações quando for mais prático.

> :book: ***Quando não é prático usar variáveis de linha de comando?*** Variáveis de linha de comando não são práticas se precisarmos usar as informações em algum lugar diferente da linha de comando. Por exemplo, salvar a assinatura pode não ser útil se tivermos que enviá-la a outra pessoa por e-mail. Além disso, alguns comandos futuros produzirão objetos JSON ao invés de informações simples, e as variáveis não podem ser usadas para capturar essas informações, ao menos não sem um _pouco_ mais de mais trabalho.

## Resumo: Usando Variáveis de Linha de Comando

Variáveis de shell podem ser usadas para manter longas strings, minimizando as chances de erros.

## O Que Vem Depois?

Vamos continuar "Compreendendo Nossa Configuração do Bitcoin" na sessão [3.4: Recebendo uma Transação](03_4_Receiving_a_Transaction.md).