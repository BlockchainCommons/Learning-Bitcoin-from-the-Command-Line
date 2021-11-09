# 17.6: Usando Outras Funções na Libwally

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

A Libwally é uma biblioteca extensa que fornece uma quantidade considerável de funcionalidades relacionadas a carteiras, muitas delas não disponíveis por meio do `bitcoin-cli`. A seguir, está uma visão geral de algumas funcionalidades não abordadas anteriormente neste capítulo.

## Usando Funções Criptográficas

Diversas funções criptográficas podem ser acessadas diretamente na Libwally:

   * `wally_aes` - Usa criptografia ou descriptografia AES;
   * `wally_aes_cbc` - Usa criptografia ou descriptografia AES no modo CBC;
   * `wally_hash160` - Usa hash RIPEMD-160 (SHA-256);
   * `wally_scrypt` - Usa derivação de chave Scrypt;
   * `wally_sha256` - Usa hash SHA256;
   * `wally_sha256_midstate` - Usa SHA256 para fazer hash apenas do primeiro bloco de dados;
   * `wally_sha256d` - Conduz um hash duplo SHA256;
   * `wally_sha512` - Usa hash SHA512.

Existem também funções HMAC para os dois hashes SHA, que são usados ​​para gerar códigos de autenticação de mensagem com base nos hashes. Eles são usados no [BIP32](https://en.bitcoin.it/wiki/BIP_0032), entre outros lugares.

   * `wally_hmac_sha256`
   * `wally_hmac_sha512`

Funções adicionais cobrem derivação de chave PBKDF2 e matemática de curva elíptica.

## Usando Funções de Endereço

A Libwally contém várias funções que podem ser usadas para importar, exportar e traduzir endereços de Bitcoin.

Alguns convertem entre endereços e bytes `scriptPubKey`:

   * `wally_addr_segwit_from_bytes` - Converte um programa witness (em bytes) em um endereço Segwit;
   * `wally_addr_segwit_to_bytes` - Converte um endereço Segwit em um `scriptPubKey` (em bytes);
   * `wally_address_to_scriptpubkey` - Converte um endereço legado em um `scriptPubKey` (em bytes);
   * `wally_scriptpubkey_to_address` - Converte um `scriptPubKey` (em bytes) em um endereço legado.
   
Alguns estão relacionados ao formato de importação de carteira (WIF):

   * `wally_wif_from_bytes` - Converte uma chave privada (em bytes) para um WIF;
   * `wally_wif_is_uncompressed` - Determina se um WIF está descompactado;
   * `wally_wif_to_address` - Deriva um endereço P2PKH de um WIF;
   * `wally_wif_to_bytes` - Converte um WIF em uma chave privada (em bytes);
   * `wally_wif_to_public_key` - Deriva uma chave pública (em bytes) de um WIF.
   
## Usando Funções do BIP32

Existem funções adicionais de carteira HD do BIP32, além do que foi abordado na seção [§17.3: Usando o BIP32 na Libwally](17_3_Using_BIP32_in_Libwally.md).

   * `bip32_key_get_fingerprint` - Gera uma impressão digital BIP32 para uma chave estendida;
   * `bip32_key_serialize` - Transforma uma chave estendida em bytes serializados;
   * `bip32_key_strip_private_key` - Converte uma chave privada estendida em uma chave pública estendida;
   * `bip32_key_unserialize` - Transforma bytes serializados em uma chave estendida.

Existem também vários outros comandos, dependendo se desejamos alocar memória ou fazer com que a Libwally faça o `_alloc` para nós.

## Usando Funções do BIP38

O [BIP38](https://github.com/bitcoin/bips/blob/master/bip-0038.mediawiki) permite a criação de uma chave privada protegida por senha. Não o ensinamos porque consideramos perigoso inserir este tipo de fator humano no gerenciamento de chaves. Se duvida disso, consulte [#SmartCustody](https://www.smartcustody.com/index.html).

As principais funções são:

   * `bip38_from_private_key` - Codifica uma chave privada usando BIP38;
   * `bip38_to_private_key` - Decodifica uma chave privada usando BIP38.
   
## Usando Funções do BIP39

Algumas funções de palavras mnemônicas do BIP39 foram resumidas recentemente na seção [§17.2: Usando o BIP39 na Libwally](17_2_Using_BIP39_in_Libwally.md):

   * `bip39_get_languages` - Veja a lista de idiomas suportados;
   * `bit39_get_word` - Recupera uma palavra específica da lista de palavras de um idioma;
   * `bip39_get_wordlist` - Veja uma lista de palavras para um idioma.
   
## Usando Funções de PSBT

As listas da maioria das funções de PSBT podem ser encontradas na seção [17.4: Usando PSBTs na Libwally](17_4_Using_PSBTs_in_Libwally.md).

## Usando Funções de Script

A seção [§17.5: Usando Scripts na Libwally](17_5_Using_Scripts_in_Libwally.md) apenas tocou nas funções de scripts da Libwally.

Há outra função que permite determinar o tipo de script encontrado em uma transação:

   * `wally_scriptpubkey_get_type` - Determina o tipo de script de uma transação.

Depois, há uma série de funções que criam `scriptPubKey` a partir de bytes, `scriptSig` a partir de assinaturas e Witness a partir de bytes ou assinaturas.

   * `wally_script_push_from_bytes`
   * `wally_scriptpubkey_csv_2of2_then_1_from_bytes`
   * `wally_scriptpubkey_csv_2of3_then_2_from_bytes`
   * `wally_scriptpubkey_multisig_from_bytes`
   * `wally_scriptpubkey_op_return_from_bytes`
   * `wally_scriptpubkey_p2pkh_from_bytes`
   * `wally_scriptpubkey_p2sh_from_bytes`
   * `wally_scriptsig_multisig_from_bytes`
   * `wally_scriptsig_p2pkh_from_der`
   * `wally_scriptsig_p2pkh_from_sig`
   * `wally_witness_multisig_from_bytes`
   * `wally_witness_p2wpkh_from_der`
   * `wally_witness_p2wpkh_from_sig`
   * `wally_witness_program_from_bytes`

## Usando Funções de Transação

Também mal tocamos nas funções que podem ser usadas para criar e converter funções na seção [§17.5](17_5_Using_Scripts_in_Libwally.md).

Existem inúmeras funções informativas, algumas das mais interessantes são:

   * `wally_tx_get_length`
   * `wally_tx_get_total_output_satoshi`
   * `wally_tx_get_weight`
   
Também existem funções que afetam um `wally_tx`, um `wally_tx_input`, um `wally_tx_output`, ou um `wally_tx_witness_stack` e que criam assinaturas.

## Usando Funções do Elements

A Libwally pode ser compilada para ser usada com os Elements da Blockstream, que inclui acesso às funções dos ativos.

## Resumo: Usando Outras Funções na Libwally

Há muito mais coisas que podemos fazer com a Libwally, mais do que podemos abordar neste capítulo ou mesmo listar nesta seção. Notavelmente, podemos executar funções criptográficas, codificar chaves privadas, criar transações completas e usar Elements. A [documentação da Libwally](https://wally.readthedocs.io/en/latest/) é o lugar onde podemos obter mais informações, embora, no momento da criação deste livro, a documentação esteja limitada e desatualizada. Os cabeçalhos dos arquivos da Libwally são um backup se os documentos estiverem incompletos ou incorretos.

## O Que Vem Depois?

Vamos aprender mais sobre "Programando Bitcoin com Libwally" na seção [§17.7: Integrando Libwally e Bitcoin-CLI](17_7_Integrating_Libwally_and_Bitcoin-CLI.md).