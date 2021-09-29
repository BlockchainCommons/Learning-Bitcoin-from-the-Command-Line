# 17.3: Usando BIP32 na Libwally

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Na seção [§17.2](17_2_Using_BIP39_in_Libwally.md), fomos capazes de usar entropia para gerar uma semente e nosso mnemônico relacionado. Como devemos nos lembrar da seção [§3.5: Compreendendo o Descritor](03_5_Understanding_the_Descriptor.md), uma seed é a base de uma Carteira Determinística Hierárquica (no inglês Hierchical Deterministic, HD), onde aquela única seed pode ser usada para gerar muitos endereços. Então, como passamos da seed para os endereços reais? É aí que entra o [BIP32](https://en.bitcoin.it/wiki/BIP_0032).

## Criando uma Raiz HD

Para criar um endereço HD, é necessário começar com uma seed e, em seguida, descer na hierarquia até o ponto em que criamos os endereços.

Isso começa com bastante facilidade, gerando uma seed, o que já fizemos na seção anterior:
```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);

  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
  
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
### Gerando uma Chave Raiz

Com uma seed em mãos, podemos gerar uma chave mestra estendida com a função `bip32_key_from_seed_alloc` (ou, alternativamente, com o comando `bip32_key_from_seed`, que não faz o `alloc`):
```
  struct ext_key *key_root;  
  lw_response = bip32_key_from_seed_alloc(seed,sizeof(seed),BIP32_VER_TEST_PRIVATE,0,&key_root);
```
Como podemos ver, precisaremos dizer ao comando qual versão da chave retornar, neste caso `BIP32_VER_TEST_PRIVATE`, uma chave privada testnet.

> :link: **TESTNET vs MAINNET:** Na mainnet, iríamos passar `BIP32_VER_MAIN_PRIVATE`.

### Gerando a xpub e a xprv

Sempre que tivermos uma chave em mãos, podemos transformá-la em chaves xpub ou xprv para distribuição com o comando `bip32_key_to_base58`. Basta dizer se desejamos uma chave `PRIVATE` (xprv) ou `PUBLIC` (xpub):
```
  char *xprv;
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PRIVATE, &xprv);

  char *xpub;  
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PUBLIC, &xpub);
```

## Compreendendo a Hierarquia

Antes de prosseguir, precisamos entender como funciona a hierarquia de uma carteira HD. Conforme discutido na seção [§3.5](03_5_Understanding_the_Descriptor.md), um caminho de derivação descreve a árvore que seguimos para obter uma chave hierárquica, então `[0/1/0]` é o 0º filho do 1º filho do 0º filho de uma chave raiz. Às vezes, parte dessa derivação é marcada com `'`s ou `h`s para mostrar derivações endurecidas, que aumentam a segurança: `[0'/1'/0']`.

No entanto, para carteiras HD, cada um desses níveis da hierarquia é usado de uma forma muito específica. Isso foi definido originalmente no [BIP44](https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki) e foi atualizado posteriormente para o Segwit no [BIP84](https://github.com/bitcoin/bips/blob/master/bip-0084.mediawiki).

Ao todo, um caminho de derivação BIP32 é definido para ter cinco níveis:

1. **Propósito.** Isso geralmente é definido como `44'` ou `84'`, dependendo do BIP que estamos seguindo;
2. **Moeda.** Para bitcoins Mainnet, é `0'`, para testnet é `1'`;
3. **Conta.** Uma carteira pode conter várias contas discretas, começando com `0'`;
4. **Troco.** Os endereços externos (para distribuição) são definidos como `0`, enquanto os endereços internos (para troco) são definidos como `1`;
5. **Índice.** O enésimo endereço da hierarquia, começando com `0`.

Então, na testnet, o endereço zero para um endereço externo para a conta zero para moedas testnet usando os padrões BIP84 é `[m/84'/1'/0'/0/0]`. Esse é o endereço que criaremos neste momento.

> :link: **TESTNET vs MAINNET:** Para a mainnet, seria `[m/84'/0'/0'/0/0]`.

### Entendendo a Hierarquia no Bitcoin Core

Estaremos usando a hierarquia acima para todas as chaves HD na Libwally, mas observe que este padrão não é usado no `bitcoin-cli` do Bitcoin Core, que ao invés disso usa `[m/0'/0'/0']` para o enésimo endereço externo e `[m/0'/1'/0']` para o enésimo endereço de troco.

## Gerando um Endereço

Para gerar um endereço, devemos explorar toda a hierarquia.

### Gerando uma Chave de Conta

Uma maneira de fazer isso é usar a função `bip32_key_from_parent_path_alloc` para descer vários níveis de uma hierarquia. Podemos incorporar os níveis em uma matriz:
```
  uint32_t path_account[] = {BIP32_INITIAL_HARDENED_CHILD+84, BIP32_INITIAL_HARDENED_CHILD+1, BIP32_INITIAL_HARDENED_CHILD};
```
Aqui, veremos a enésima filha endurecida (essa é a conta) ou a primeira filha endurecida (que são as moedas testnet) da 84ª filha endurecida (esse é o padrão BIP84): `[m/84'/1'/0']`.

Podemos então usar esse caminho para gerar uma nova chave a partir da nossa antiga chave:
```
  struct ext_key *key_account;
  lw_response = bip32_key_from_parent_path_alloc(key_root,path_account,sizeof(path_account),BIP32_FLAG_KEY_PRIVATE,&key_account);
```
Sempre que tiviermos uma nova chave, poderemos usá-la para gerar novas chaves xprv e xpub, se desejarmos:
```
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PRIVATE, &a_xprv);
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PUBLIC, &a_xpub);
```

### Gerando uma Chave de Endereço

Alternativamente, podemos usar a função `bip32_key_from_parent_alloc`, que apenas desce um nível da hierarquia por vez. O exemplo a seguir desce para o enésimo filho da chave da conta (que é o endereço externo) e, em seguida, o enésimo filho dessa chave. Isso seria útil porque poderíamos continuar gerando o primeiro endereço, o segundo endereço e assim por diante a partir dessa chave externa:
```
  struct ext_key *key_external;  
  lw_response = bip32_key_from_parent_alloc(key_account,0,BIP32_FLAG_KEY_PRIVATE,&key_external);

  struct ext_key *key_address;  
  lw_response = bip32_key_from_parent_alloc(key_external,0,BIP32_FLAG_KEY_PRIVATE,&key_address);
```
> :warning: **AVISO:** Em algum ponto desta hierarquia, podemos decidir gerar o `BIP32_FLAG_KEY_PUBLIC` ao invés do `BIP32_FLAG_KEY_PRIVATE`. Obviamente, essa decisão será baseada na nossa segurança e nas nossas necessidades, mas precisamos lembrar de que só precisamos de uma chave pública para gerar o endereço real.

### Gerando um Endereço

Finalmente, estamos prontos para gerar um endereço a partir de sua chave final. Tudo que fazemos é executar `wally_bip32_to_addr_segwit` usando nossa chave final e uma descrição de que tipo de endereço é este.
```
  char *segwit;
  lw_response = wally_bip32_key_to_addr_segwit(key_address,"tb",0,&segwit);

  printf("[m/84'/1'/0'/0/0]: %s\n",segwit);
```  

> :link: **TESTNET vs MAINNET:** O argumento `tb` define um endereço Testnet. Para a Mainnet, usamos o `bc`.

Há também uma função `wally_bip32_key_to_address`, que pode ser usada para gerar um endereço legado ou um endereço Segwit aninhado.

## Testando o Código HD

O código para esses exemplos HD pode, como de costume, ser encontrado no [diretório src/](../src/17_3_genhd.c).

Podemos compilá-lo e testá-lo:
```
$ cc genhd.c -lwallycore -lsodium -o genhd
$ ./genhd
Mnemonic: behind mirror pond finish borrow wood park foam guess mail regular reflect
Root xprv key: tprv8ZgxMBicQKsPdLFXmZ6VegTxcmeieNpRUq8J2ahXxSaK2aF7CGqAc14ZADLjdHJdCr8oR2Zng9YH1x1A7EBaajQLVGNtxc4YpFejdE3wyj8
Root xpub key: tpubD6NzVbkrYhZ4WoHKfCm64685BoAeoi1L48j5K6jqNiNhs4VspfeknVgRLLiQJ3RkXiA9VxguUjmEwobtmrXNbhXsPHfm9W5HJR9DKRGaGJ2
Account xprv key: tprv8yZN7h6SPvJXrhAk56z6cwHQE6qZBRreB9fqqZJ1Xd1nLci3Rw8HTmqNkpFNgf3eZx8hYzhFWafUhHSt3HgF13aHvCE6kveS7gZAyfQwMDi
Account xpub key: tpubDWFQG78gYHzCkACXxkeh2LwWo8MVLm3YkTGd85LJwtpBB6xp4KwseGTEvxjeZNhnCNPdfZqRcgcZZAka4tD3xGS2J53WKHPMRhG357VKsqT
[m/84'/1'/0'/0/0]: tb1q0knqq26ek59pfl7nukzqr28m2zl5wn2f0ldvwu
```

## Resumo: Usando BIP32 na Libwally

Uma carteira HD permite gerar um grande número de chaves a partir de uma única seed. Agora sabemos como essas chaves são organizadas no BIP44, BIP84 e no Bitcoin Core, além de como derivá-las, começando com uma seed ou palavras mnemônicas.

> :fire: ***Qual é o poder do BIP32?*** As chaves são o elemento mais difícil (e mais perigoso) da maioria das operações criptográficas. Se as perdermos, perderemos tudo o que a chave protegeu. O BIP32 garante que só precisamos conhecer uma chave, a semente, ao invés de um grande número de chaves diferentes para endereços diferentes.

> :fire: ***Qual é o poder do BIP32 na Libwally?*** O Bitcoind já faz a criação de endereços baseada em HD para nós, o que significa que normalmente não precisamos nos preocupar em derivar endereços dessa maneira. No entanto, usar as funções BIP32 da Libwally pode ser muito útil se tivermos uma máquina offline onde precisamos derivar endereços, possivelmente com base em uma semente passada do `bitcoind` para nosso dispositivo offline (ou vice-versa).

## O Que Vem Depois?

Vamos aprender mais sobre "Programando Bitcoin com Libwally" na seção [§17.4: Usando PSBTs na Libwally](17_4_Using_PSBTs_in_Libwally.md).