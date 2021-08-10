# 12.1: Usando Condicionais no Script

Há um aspecto importante na criação de Script de Bitcoin que é crucial para expandir o seu verdadeiro poder. Os condicionais permitem que criemos vários caminhos de execução.

## Entendendo as Verificações

Já vimos um condicional dentro do script, o ```op_verify``` (0x69). Ele retira o item superior na pilha e verifica se é verdade. Se não for, _ele termina a execução do script_.

A verificação é geralmente incorporada em outros opcodes. Já vimos o ```OP_EQUALVERIFY``` (0xad), o ```OP_CHECKLOCKTIMEVERIFY``` (0xb1) e o ```OP_CHECKSEQUENCEVERIFY``` (0xb2). Cada um desses opcodes faz nossa ação central (equal, checklocktime ou checksequence) e então faz uma verificação posteriormente. Os outros opcodes de verificação que ainda não vimos são: ```OP_NUMEQUALVERIFY``` (0x9d), ```OP_CHECKSIGVERIFY``` (0xad), e ```OP_CHECKMULTISIGVERIFY``` (0xaf).

Então, como o ```OP_VERIFY``` é um condicional? É o tipo mais poderoso de condicional. Usando o ```OP_VERIFY```, _se_ uma condição é verdadeira, o script continua executando, _senão_ o script pára a execução. É assim que verificamos as condições que são absolutamente necessárias para que um script tenha sucesso. Por exemplo, o script P2PKH (```OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG```) tem duas condições necessárias: (1) a chave pública fornecida precisa corresponder ao hash da chave pública e; (2) a assinatura fornecida precisa corresponder à essa chave pública. Um ```OP_EQUALVERIFY``` é usado para a comparação do hash da chave pública e a chave pública do hash porque é uma condição absolutamente necessária. Não _queremos_ que o script continue caso isto falhe.

Podemos notar que não há ```OP_VERIFY``` no final deste script (ou da maioria dos demais), apesar da condição final também ser necessária. Isso porque o Bitcoin efetivamente faz um ```OP_VERIFY``` no final de cada script, para garantir que o resultado final da pilha seja verdadeiro. Não podemos fazer um ```OP_VERIFY``` antes do final do script, porque precisamos deixar algo na pilha para ser testado!

## Compreendendo o If/Then (Se/Então)

O outro condicional principal no script do Bitcoin é o clássico ```OP_IF``` (0x63) / ```OP_ELSE``` (0x67) / ```OP_ENDIF``` (0x68). Este é o controle típico de fluxo: se o ```OP_IF``` detectar uma afirmação verdadeira, ele executa o bloco abaixo dele, caso contrário, se houver um ```OP_ELSE```, ele o executa; e o ```OP_ENDIF``` marca o final do bloco.

> :warning: **AVISO:** Estes condicionais tecnicamente são opcodes também, mas como são pequenos números, vamos deixar o prefixo do ```OP_``` desligado para manter a brevidade e a clareza. Assim, vamos escrever ```IF```, ```ELSE```, e ```ENDIF``` ao invés de ```OP_IF```, ```OP_ELSE```, e ```OP_ENDIF```.

### Compreendendo a Ordem do If/Then

Existem duas grandes sacadas nos condicionais. Eles dificultam a leitura e determinam os scripts se não tivermos cuidado.

Primeiro, o condicional ```IF``` verifica a verdade do que é _antes dele_ (em outras palavras, o que está na pilha), e não o que está depois dele.

Segundo, o condicional ```IF``` tende a estar no script de bloqueio e o que é verificado tende a estar no script de desbloqueio.

Claro, podemos dizer, é assim que funciona o Script do Bitcoin. Condicionais usam notação polonesa reversa e adotam o paradigma padrão de desbloqueio/bloqueio, assim como todos os scripts do bitcoin. Isso é tudo verdade, mas também é o contrário da maneira padrão de ler-se condicionais IF/ELSE em outras linguagens de programação. Assim, é fácil lermos errado, inconscientemente, os condicionais do Bitcoin.

Vamos observar o seguinte código: `IF OP_DUP OP_HASH160 <pubKeyHashA> ELSE OP_DUP OP_HASH160 <pubKeyHashB> ENDIF OP_EQUALVERIFY OP_CHECKSIG`.

Olhando para os condicionais na notação do prefixo podemos ler isso da seguinte maneira:
```
IF (OP_DUP) THEN
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 
ELSE 
    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB>
ENDIF  
 OP_EQUALVERIFY 
 OP_CHECKSIG
```
Então, podemos pensar, se o ```OP_DUP``` é verdadeiro, então nós vamos fazer o primeiro bloco, senão, o segundo. Mas isso não faz sentido! Por que o ```OP_DUP``` não executaria com sucesso?!

E, de fato, não faz nenhum sentido, porque acidentalmente lemos a declaração usando a notação errada. A leitura correta é:
```
IF 
    OP_DUP
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 
ELSE 
    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB> 
ENDIF 
 OP_EQUALVERIFY 
 OP_CHECKSIG
```
A declaração que avaliará para ```True``` ou ```False``` é colocada na pilha _antes_ de executar o ```IF```, então o bloco correto do código será executado com base naquele resultado.

Este exemplo específico de código é destinado a um simples multisig 1-de-2. O proprietário da ```<privKeyA>``` colocaria ```<signatureA> <pubKeyA> TRUE``` no script de desbloqueio, enquanto o proprietário da ```<privKeyB>``` colocaria ```<signatureB> <pubKeyB> FALSE``` no script de desbloqueio. Aquele que for rastreado como ```TRUE``` ou ```FALSE``` é o que é verificado pela instrução ```IF```/```ELSE```. Ele conta ao script qual o hash da chave pública é para verificar, então o ```OP_EQUALVERIFY``` e o ```OP_CHECKSIG``` no final fazem o verdadeiro trabalho.

### Executando um If/Then com Multisig

Com uma compreensão central dos condicionais do Bitcoin, estamos prontos para executar scripts os utilizando. Nós vamos começar criando uma ligeira variante do multisig 1-de-2 do exemplo, onde nossos usuários não precisam lembrar se eles são ```TRUE``` ou ```FALSE```. Ao invés disso, se necessário, o script verifica os hashes de chave pública, apenas exigindo um único sucesso:
```
OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL
IF
    OP_CHECKSIG
ELSE
    OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG    
ENDIF
```
Precisamos lembrar da notação polonesa reversa! Aquela instrução ```IF``` está se referindo ao ```OP_EQUAL``` antes dela, não ao `OP_CHECKSIG` posterior!

#### Executando a Parte Verdadeira

Veja como é executado se desbloquearmos com ```<signatureA> <pubKeyA>```:
```
Script: <signatureA> <pubKeyA> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```
Primeiro, colocamos as constantes na pilha:
```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> ]
```
Em seguida, nós executamos os primeiros comandos óbvios, ```OP_DUP``` e ```OP_HASH160```, e colocamos outra constante:
```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_DUP
Stack: [ <signatureA> <pubKeyA> <pubKeyA> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_HASH160
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> <pubKeyHashA> ]
```
Em seguida, executamos o ```OP_EQUAL```, que é o que vai alimentar o ```IF```:
```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashA> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureA> <pubKeyA> True ]
```
Agora, o ```IF``` executa, e desde que haja um ```TRUE```, ele só executa o primeiro bloco, eliminando todo o resto:
```
Script: OP_CHECKSIG
Running: True IF
Stack: [ <signatureA> <pubKeyA> ]
```
E o ```OP_CHECKSIG``` acabará sendo ```TRUE``` também:
```
Script: 
Running: <signatureA> <pubKeyA> OP_CHECKSIG
Stack: [ True ]
```

#### Executando a Parte Falsa

Veja como ele iria executar se fôssemos desbloquear com ```<signatureB> <pubKeyB>```:
```
Script: <signatureB> <pubKeyB> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```
Primeiro, colocamos as constantes na pilha:
```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> ]
```
Em seguida, executamos os primeiros comandos óbvios, ```OP_DUP``` e ```OP_HASH160```, e adicionamos outra constante:
```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashA> ]
```
Em seguida, executamos o ```OP_EQUAL```, que é o que vai alimentar o ```IF```:
```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashB> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureB> <pubKeyB> False ]
```
Uhul! O resultado foi ```FALSE``` porque o ```<pubKeyHashB>``` não é igual ao ```<pubKeyHashA>```. Agora, quando o ```IF``` for executado, ele vai pular para a instrução ```ELSE```:
```
Script: OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: False IF
Stack: [ <signatureB> <pubKeyB> ]
```
Depois, passamos por todo o imbróglio, começando com outro ```OP_DUP```, mas eventualmente o testando usando outro ```pubKeyHash```:
```
Script: OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashB> ]

Script:OP_CHECKSIG
Running: <pubKeyHashB> <pubKeyHashB> OP_EQUALVERIFY 
Stack: [ <signatureB> <pubKeyB> ]

Script: 
Running: <signatureB> <pubKeyB> OP_CHECKSIG
Stack: [ True ]
```
Isso provavelmente não é tão eficiente quanto um verdadeiro multisig do Bitcoin, mas é um bom exemplo de como os resultados adicionados à pilha devido a testes anteriores podem ser usados para alimentar condicionais futuros. Neste caso, é o fracasso da primeira assinatura que diz ao condicional que deve ir verificar a segunda.

## Entendendo os Demais Condicionais

Existem alguns outros condicionais para ser analisados. O maior deles é o ```OP_NOTIF``` (0x64), que é o oposto de ```OP_IF```. Ele executa o próximo bloco se o item superior for ```FALSE```. Um ```ELSE``` pode ser colocado junto, que como usual é executado se o primeiro bloco não for executado. Podemos ainda terminar com o ```OP_ENDIF```.

Há também um ```OP_IFDUP``` (0x73), que duplica o item de pilha superior somente se o resultado não for 0.

Essas opções são usadas com muito menos frequência do que a construção usando ```IF```/```ELSE```/```ENDIF```.

## Resumo: Usando Condicionais no Script

Os condicionais no script do Bitcoin permitem parar o script (usando o ```OP_VERIFY```) ou escolher diferentes ramos de execução (usando ```OP_IF```). No entanto, ler o ```OP_IF``` pode ser um pouco complicado. Precisamos lembrar de que é o item adicionado à pilha _antes_ do operador ```OP_IF``` ser executado que controla a sua execução. Esse item normalmente fará parte do script de desbloqueio (ou será um resultado direto de itens do script de desbloqueio).

> :fire: ***Qual é o poder dos condicionais?*** Os condicionais do script são o principal bloco de construção no Bitcoin Script. Eles transformam os scripts simples e estáticos do Bitcoin em scripts de Bitcoin complexos e dinâmicos que podem avaliar de maneira diferente com base em diferentes momentos, diferentes circunstâncias ou diferentes entradas de usuário. Em outras palavras, eles são o último pilar dos contratos inteligentes.

## O Que Vem Depois?

Vamos continuar "Expandindo os Scripts do Bitcoin" na seção [§12.2: Usando Outros Comandos no Script](12_2_using_other_script_commands.md).