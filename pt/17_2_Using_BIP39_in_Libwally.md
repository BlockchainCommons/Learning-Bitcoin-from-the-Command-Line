# 17.2: Usando BIP39 na Libwally

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Um dos maiores poderes da Libwally é que ela pode revelar o trabalho oculto da geração de seeds, chaves privadas e, também, de endereços. Para começar, ela suporta o [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), que é o BIP que define códigos mnemônicos para o Bitcoin, algo que era totalmente incompatível, na época, com o Bitcoin Core.

> :book: ***O que é um código mnemônico?*** Os endereços de Bitcoin (e suas chaves privadas correspondentes e as suas seeds) são listas longas e ininteligíveis de caracteres e números que não apenas são impossíveis de serem lembrados, mas também fáceis de serem digitados erroneamente. Os códigos mnemônicos são uma solução para isso que permitem aos usuários gravar 12 (ou 24) palavras em inglês, algo que é muito menos sujeito a erros. Esses códigos podem ser usados para restaurar totalmente uma seed BIP32 que é a base de uma carteira Determinística Hierárquica (HD).

> :book: ***O que é uma seed?*** Nós falamos brevemente das seeds na seção [§3.5: Compreendendo o Descritor](03_5_Understanding_the_Descriptor.md). É o número aleatório usado para gerar uma sequência inteira de chaves privadas (e, portanto, endereços) em uma carteira HD. Voltaremos às seeds na próxima seção, onde falaremos sobre as carteiras HD e a Libwally. Por enquanto, apenas é necessário saber que um código mnemônico BIP39 corresponde à seed para uma carteira HD do BIP32.

## Criando Códigos Mnemônicos

Todas as chaves Bitcoin começam com a entropia. Este primeiro uso da Libwally, e os mnemônicos BIP39, mostram como gerar entropia e obter um código mnemônico a partir disso.

> :book: ***O que é entropia?*** Entropia é uma maneira sofisticada de dizer aleatoriedade, mas é uma aleatoriedade medida cuidadosamente que é usada como a base de um número aleatório gerado verdadeiramente (no inglês, true-random-number generated, TRG). É medido em "bits", com mais bits de entropia resultando em mais aleatoriedade (e, portanto, mais proteção para o que está sendo gerado). Para o Bitcoin, a entropia é a base de nossa seed, que em uma carteira HD gera todos os seus endereços.

Sempre começaremos a trabalhar com a Libwally inicializando a biblioteca e testando os resultados, conforme demonstrado pela primeira vez na seção [§17.1](17_1_Setting_Up_Libwally.md):
```
  int lw_response;

  lw_response = wally_init(0);

  if (lw_response) {

    printf("Error: Wally_init failed: %d\n",lw_response);
    exit(-1);
    
  }
```
Agora estamos prontos para a entropia.

### Criando Entropia

Usando o `libsodium`, podemos criar entropia com o comando `randombytes_buf`:
```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);
```
Este exemplo, que será a única maneira de usarmos a biblioteca `libsodium`, cria 16 bytes de entropia. Geralmente, para criar um código mnemônico seguro, devemos usar entre 128 e 256 bits de entropia, que é 16 a 32 bytes.

> :warning: **AVISO:** Mais uma vez, certifique-se de estar muito confortável com o método de geração de entropia antes de usá-lo em um programa do mundo real.

### Traduzindo para um Mnemônico

16 bytes de entropia são suficientes para criar um código Mnemônico de 12 caracteres, que é feito com a função `bip39_mnemonic_from_bytes` da Libwally:
```
  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
```
Observe que temos que passar o tamanho do byte, então se quisermos aumentar o tamanho da entropia, para gerar uma frase mnemônica mais longa, também precisaríamos aumentar o valor nesta função.

> **NOTA:** Existem listas de palavras mnemônicas para diferentes idiomas! O padrão é usar a lista do idioma inglês, que é a variável `NULL` nesses comandos mnemônicos da Libwally, mas podemos, alternativamente, solicitar um idioma diferente!

É isso! Nós criamos uma frase mnemônica!

> :book: ***Como a frase mnemônica é criada?*** Podemos aprender mais sobre isso no [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), mas, se preferir, Greg Walker tem um [excelente exemplo](https://learnmeabitcoin.com/technical/mnemonic): de maneira geral, adicionamos um checksum e convertemos cada conjunto de 11 bits em uma palavra da lista de palavras. Podemos fazer isso com os comandos `bip39_get_wordlist` e `bip39_get_word` se não confiarmos no comando `bip39_mnemonic_from_bytes`.

### Traduzindo para uma Seed

Existem algumas funções, como o `bip32_key_from_seed` (que veremos na próxima seção) que requerem que tenhamos os bits ao invés do Mnemônico. As duas coisas são funcionalmente idênticas: se temos a seed, podemos gerar o mnemônico e vice-versa.

Se precisarmos gerar a seed a partir do nosso mnemônico, basta usar o comando `bip39_mnemonic_to_seed`:
```
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;
  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
Observe que todas as seeds do BIP39 têm 512 bytes; no entanto, devemos definir o tamanho de nossa variável apropriadamente e passar o tamanho para o `bip39_mnemonic_to_seed`.

### Vendo a Nossa Seed

Se quisermos ver como nossa seed é em hexadecimal, podemos usar a função `wally_hex_from_bytes` para transformar nossa seed em um código hexadecimal legível (mas isso não é muito bom para pessoas):
```
  char *seed_hex;
  wally_hex_from_bytes(seed,sizeof(seed),&seed_hex);
  printf("Seed: %s\n",seed_hex);
```
Se dizermos tudo certo, devemos obter uma seed de 64 bytes. (Essa é a variável `BIP39_SEED_LEN_512` que estamos usando, que define um comprimento de seed padrão de 512 bits ou 64 bytes.)

> :warning: **ATENÇÃO:** Definitivamente devemos testar se o comprimento da nossa seed é de 64 bytes de alguma forma, porque é fácil errar em alguma coisa, por exemplo, usando o tipo de variável errado quando executamos o `bip39_mnemonic_to_seed`.

## Testando o Código Mnemônico

O código completo para gerar entropia, gerar um mnemônico BIP39, validar o mnemônico e gerar uma seed pode ser encontrado no [diretório src/](../src/17_2_genmnemonic.c). Podemos fazer o download e compilar:
```
$ cc genmnemonic.c -lwallycore -lsodium -o genmnemonic
```
Então podemos executar o teste:
```
Mnemonic: parent wasp flight sweet miracle inject lemon matter label column canyon trend
Mnemonic validated!
Seed: 47b04cfb5d8fd43d371497f8555a27a25ca0a04aafeb6859dd4cbf37f6664b0600c4685c1efac29c082b1df29081f7a46f94a26f618fc6fd38d8bc7b6cd344c7
```

## Resumo: Usando BIP39 na Libwally

O BIP39 nos permite gerar um conjunto de 12-24 palavras Mnemônicas a partir de uma seed (e a biblioteca Libwally também permite que a validemos!).

> :fire: ***Qual é o poder do BIP39?*** Seeds de Bitcoin e chaves privadas estão sujeitas a todos os tipos de perda. Se digitarmos errado um único dígito, nosso dinheiro será perdido para sempre. Palavras mnemônicas são uma forma muito mais amigável de representar os mesmos dados, mas como são palavras no idioma de escolha do usuário, são menos sujeitas a erros. O poder do BIP39 é, portanto, melhorar a acessibilidade, usabilidade e segurança do Bitcoin.

> :fire: ***Qual é o poder do BIP39 na Libwally?*** O Bitcoind atualmente não suporta palavras mnemônicas, então usar a Libwally pode permitir que geremos palavras mnemônicas em conjunto com endereços mantidos no `bitcoind`, embora como veremos na seção §17.7, é necessário um pouco de gambiarra para importar suas chaves para o Bitcoin Core.

## O Que Vem Depois?

Vamos aprender mais sobre "Programando o Bitcoind usando o Libwally" na seção [§17.3: Usando o BIP32 no Libwally](17_3_Using_BIP32_in_Libwally.md).