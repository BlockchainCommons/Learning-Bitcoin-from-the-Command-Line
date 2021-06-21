# Usando variáveis de linha de comando

A seção anterior demonstrou vários comandos de linha de comando usados sem ofuscação ou interferência. No entanto, geralmente essa não é a melhor maneira de executar Bitcoin na linha de comando. Como você está lidando com variáveis longas, complexas e ilegíveis, é fácil cometer um erro se você estiver copiando essas variáveis (ou, satoshi forfend, se você as estiver digitando manualmente). Como essas variáveis podem significar a diferença entre receber e perder dinheiro real, você não _quer_ cometer erros. Por esses motivos, sugerimos enfaticamente o uso de variáveis de linha de comando para salvar endereços, assinaturas ou outras cadeias de informações longas sempre que for razoável.

Se estiver usando `bash`, você pode salvar as informações em uma variável como esta:
```
$ VARIABLE=$(command)
```

Esta é uma substituição de comando simples, o equivalente a `VARIABLE = command`. O comando entre parênteses é executado e, em seguida, atribuído à VARIÁVEL.

Para criar um novo endereço, seria assim:
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress "" legacy)
```
Esses comandos limpam a variável NEW_ADDRESS_1, apenas para ter certeza, e então a preenchem com os resultados do comando `bitcoin-cli getnewaddress`.

Você pode então usar o comando `echo` do seu shell para ver o seu (novo) endereço:
```
$ echo $NEW_ADDRESS_1
mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE
```
Como seu endereço está em uma variável, agora você pode assinar facilmente uma mensagem para esse endereço, sem se preocupar em digitar o endereço incorretamente. É claro que você também salvará essa assinatura em uma variável!
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
$ echo $NEW_SIG_1
IPYIzgj+Rg4bxDwCyoPiFiNNcxWHYxgVcklhmN8aB2XRRJqV731Xu9XkfZ6oxj+QGCRmTe80X81EpXtmGUpXOM4=
```
O restante deste tutorial usará esse estilo de armazenar informações em variáveis quando for prático.

> :book: ***Quando não é prático usar variáveis de linha de comando?*** Variáveis de linha de comando não são práticas se você precisar usar as informações em algum lugar diferente da linha de comando. Por exemplo, salvar sua assinatura pode não ser útil se você apenas tiver que enviá-la a outra pessoa por e-mail. Além disso, alguns comandos futuros produzirão objetos JSON em vez de informações simples, e as variáveis não podem ser usadas para capturar essas informações ... pelo menos não sem um _pouco_ de mais trabalho.

## Resumo: Usando variáveis de linha de comando

Variáveis de shell podem ser usadas para manter longas strings, minimizando as chances de erros.

## Mas o que vem a seguir?

Continue "Compreendendo a configuração do seu node Bitcoin" com [3.4: Recebendo uma transação](03_4_Receiving_a_Transaction.md).