# 17.1: Configurando a Libwally

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta primeira seção explicará como fazer o download da Biblioteca Libwally C e como colocá-la em funcionamento.

> :book: ***O que é a Libwally?*** A Libwally é uma biblioteca de primitivas útil para a criação de carteiras que é totalmente multiplataforma, de modo que as mesmas funções possam ser usadas em qualquer lugar. Há uma [documentação online](https://wally.readthedocs.io/en/latest/), caso esteja interessado. A Libwally está disponível como parte do [Elements Project](https://github.com/ElementsProject) da Blockstream.

## Instalando a Libwally

Como de costume, precisaremos de alguns pacotes no nosso sistema:
```
$ sudo apt-get install git
$ sudo apt-get install dh-autoreconf
```
Podemos então fazer o download do Libwally com base no seu repositório Git:
```
$ git clone https://github.com/ElementsProject/libwally-core
```
Depois, podemos começar com o processo de configuração.
```
$ ./tools/autogen.sh
```
Como no ```libbitcoinrpc```, podemos querer instalar no caminho `/usr/include` e no `/usr/lib` para facilitar o uso. Basta modificar a linha apropriada no arquivo `configure` do programa:
```
< ac_default_prefix=/usr
---
> ac_default_prefix=/usr/local
```
Depois, podemos terminar nossa preparação:
```
$ ./configure
$ make
```
Agora, podemos verificar se os testes estão funcionando:
```
$ make check
Making check in src
make[1]: Entering directory '/home/standup/libwally-core/src'
Making check in secp256k1
make[2]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make  check-TESTS
make[3]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make[4]: Entering directory '/home/standup/libwally-core/src/secp256k1'
============================================================================
Testsuite summary for libsecp256k1 0.1
============================================================================
# TOTAL: 0
# PASS:  0
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[3]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Entering directory '/home/standup/libwally-core/src'
make  check-TESTS check-local
make[3]: Entering directory '/home/standup/libwally-core/src'
make[4]: Entering directory '/home/standup/libwally-core/src'
PASS: test_bech32
PASS: test_psbt
PASS: test_psbt_limits
PASS: test_tx
============================================================================
Testsuite summary for libwallycore 0.7.8
============================================================================
# TOTAL: 4
# PASS:  4
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src'
make[3]: Nothing to be done for 'check-local'.
make[3]: Leaving directory '/home/standup/libwally-core/src'
make[2]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Entering directory '/home/standup/libwally-core'
make[1]: Nothing to be done for 'check-am'.
make[1]: Leaving directory '/home/standup/libwally-core'
```
Finalmente, podemos instalar:
```
$ sudo make install
```

## Nos Preparando Para a Libwally

Então, como usamos a Libwally em um programa? Como de costume, precisaremos incluir os arquivos e vincular as bibliotecas apropriadas em nosso código.

### Incluindo os Arquivos

Há um número considerável de arquivos que podemos incluir:

```
$ ls /usr/include/wally*
/usr/include/wally_address.h  /usr/include/wally_bip39.h   /usr/include/wally_elements.h  /usr/include/wally_script.h
/usr/include/wally_bip32.h    /usr/include/wally_core.h    /usr/include/wally.hpp	  /usr/include/wally_symmetric.h
/usr/include/wally_bip38.h    /usr/include/wally_crypto.h  /usr/include/wally_psbt.h	  /usr/include/wally_transaction.h
```
Felizmente, os nomes dos arquivos correspondem amplamente às seções da [documentação](https://wally.readthedocs.io/en/latest/), então devemos ser capazes de incluir os arquivos corretos com base no que estamos fazendo, depois de incluir o onipresente `wally_core.h`.

### Vinculando as Bibliotecas

Também precisaremos vincular as bibliotecas apropriadas:
```
$ ls /usr/lib/libsecp* /usr/lib/libwally*
/usr/lib/libsecp256k1.a   /usr/lib/libwallycore.la  /usr/lib/libwallycore.so.0
/usr/lib/libsecp256k1.la  /usr/lib/libwallycore.so  /usr/lib/libwallycore.so.0.0.0
```
Usaremos principalmente a `libwallycore`.

## Configurando um Programa Libwally

Comparado com algumas das bibliotecas anteriores, a Libwally é ridiculamente fácil de ser inicializada:
```
lw_response = wally_init(0);
```
E então, quando terminarmos, há uma função útil para limpar qualquer memória alocada:
```
wally_cleanup(0);
```
Em ambos os casos, o argumento é para flags, mas atualmente está definido como `0`.

## Testando um Programa de Teste da Libwally

O diretório src contém o arquivo [testwally.c](../src/17_1_testwally.c), que apenas mostra como funcionam as funções de inicialização e de limpeza.

Podemos compilá-lo da seguinte maneira:
```
$ cc testwally.c -lwallycore -o testwally
```
Depois, podemos executá-lo:
```
$ ./testwally
Startup: 0
```
O valor "Startup" é o retorno do comando `wally_init`. O valor `0` pode inicialmente parecer desanimador, mas é o que desejamos ver no momento:
```
include/wally_core.h:#define WALLY_OK      0 /** Success */
```

## Instalando o Libsodium

Também precisamos instalar o Libsodium para obter acesso a um gerador de números aleatórios de alta qualidade para fins de teste.

> :warning: **AVISO:** A geração de números aleatórios pode ser um dos maiores pontos de vulnerabilidade em qualquer software do Bitcoin. Se fizermos isso de maneira errada, podemos expor nossos usuários a ataques porque eles acabam tendo chaves privadas do Bitcoin inseguras, e isso não é um [problema teórico](https://github.com/BlockchainCommons/SmartCustodyBook/blob/master/manuscript/03-adversaries.md#adversary-systemic-key-compromise). A BlockchainInfo gerou incorretamente 0,0002% das suas chaves, o que resultou na perda temporária de 250 Bitcoins. Resumindo: precisamos nos certificar de estar totalmente confortável com a geração de números aleatórios. Podemos usar o Libsodium ou qualquer outro método TRNG ainda mais robusto.

Podemos baixar um [Libsodium tarball](https://download.libsodium.org/libsodium/releases/) e seguir as instruções em [instalação do Libsodium](https://doc.libsodium.org/installation) para deixarmos tudo pronto em nosso computador.

Primeiro, descompactamos:
```
$ tar xzfv /tmp/libsodium-1.0.18-stable.tar.gz 
```
Então, ajustamos o arquivo `configure` exatamente como fizemos nas outras bibliotecas até o momento:
```
< ac_default_prefix=/usr
---
> ac_default_prefix=/usr/local
```
Finalmente, usamos os comandos `make`,`check` e `install`:
```
$ make
$ make check
...
============================================================================
Testsuite summary for libsodium 1.0.18
============================================================================
# TOTAL: 77
# PASS:  77
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
...
$ sudo make install
```
Este curso usará apenas `libsodium` para um pequeno (mas crucial!) bit de geração de entropia, mas precisamos prestar muita atenção na próxima seção.

## Resumo: Configurando a Libwally

Ao instalar os includes e as bibliotecas da Libwally (e do Libsodium), ganhamos acesso a uma série de funções criptográficas e de carteira, que podem complementar nossas bibliotecas RPC e ZMG (ou nossa linha de comando `bitcoin-cli`).

Então, o que exatamente podemos fazer agora? É para dar essa resposta que temos todas as seções deste capítulo.

## O Que Vem Depois?

Vamos aprender mais sobre "Programando o Bitcoin com Libwally" na seção [§17.2: Usando BIP39 na Libwally](17_2_Using_BIP39_in_Libwally.md).