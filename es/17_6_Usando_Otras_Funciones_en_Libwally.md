# 17.6: Uso de otras funciones en Libwally

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Libwally es una extensa biblioteca que proporciona una considerable cantidad de funcionalidad relacionada con la cartera, gran parte de ella no está disponible a través de `bitcoin-cli`. A continuación se ofrece una visión general de algunas funcionalidades no cubiertas anteriormente en este capítulo.

## Usar funciones criptográficas

Se puede acceder directamente a varias funciones criptográficas desde Libwally:

   * `wally_aes` - Usar cifrado o descifrado AES
   * `wally_aes_cbc` - Usar cifrado o descifrado AES en modo CBC
   * `wally_hash160` - Usar el hash RIPEMD-160(SHA-256)
   * `wally_scrypt` - Utilizar la derivación de clave Scrypt
   * `wally_sha256` - utilizar el hash SHA256
   * `wally_sha256_midstate` - Utilizar SHA256 para hash sólo el primer trozo de datos
   * `wally_sha256d` - Realizar un doble hash SHA256
   * `wally_sha512` - utilizar el hash SHA512

También hay funciones HMAC para los dos hashes SHA, que se utilizan para generar códigos de autenticación de mensajes basados en los hashes. Se utilizan en [BIP32](https://en.bitcoin.it/wiki/BIP_0032), entre otros lugares.

   * `wally_hmac_sha256`
   * `wally_hmac_sha512`

Las funciones adicionales cubren la derivación de clave PBKDF2 y la matemática de curva elíptica.

## Usar funciones de dirección

Libwally contiene una serie de funciones que se pueden utilizar para importar, exportar y traducir direcciones de Bitcoin.

Algunos convierten de ida y vuelta entre direcciones y bytes `scriptPubKey`:

   * `wally_addr_segwit_from_bytes` - Convertir un programa testigo (en bytes) en una dirección Segwit
   * `wally_addr_segwit_to_bytes` - Convertir una dirección Segwit en un `scriptPubKey` (en bytes)
   * `wally_address_to_scriptpubkey` - Convertir una dirección heredada en un `scriptPubKey`(en bytes)
   * `wally_scriptpubkey_to_address` - Convertir un scriptPubKey` (en bytes) en una dirección heredada

Algunos se refieren al formato de importación de la cartera (WIF):

   * `wally_wif_from_bytes` - Convertir una clave privada (en bytes) en una WIF
   * `wally_wif_is_uncompressed` - Determina si un WIF está descomprimido
   * `wally_wif_to_address` - Derivar una dirección P2PKH de un WIF
   * `wally_wif_to_bytes` - Convertir un WIF a una clave privada (en bytes)
   * `wally_wif_to_public_key` - Derivar una clave pública (en bytes) de un WIF

## Usar funciones BIP32

Hay funciones adicionales de la cartera BIP32 HD, más allá de lo que estaba cubierto en [§17.3: Usar BIP32 en Libwally](17_3_Usando_BIP32_en_Libwally.md).

   * `bip32_key_get_fingerprint` - Generar una huella digital BIP32 para una clave extendida
   * `bip32_key_serialize` - Transformar una clave extendida en bytes seriados
   * `bip32_key_strip_private_key` - Convertir una clave privada extendida en una clave pública extendida
   * `bip32_key_unserialize` - Transformar bytes seriados en una clave extendida

También hay varios numerosos dependiendo de si desea asignar memoria o tener a Libwally haciendo el `_alloc` por usted.

## Usar funciones BIP38

[BIP38](https://github.com/bitcoin/bips/blob/master/bip-0038.mediawiki) permite la creación de clave privada protegida con contraseña. No lo enseñamos porque consideremos peligroso insertar este tipo de factor humano en la gestión clave. Véase [#SmartCustody](https://www.smartcustody.com/index.html).

Las principales funciones son:
   * `bip38_from_private_key` - Codificar una clave privada usando BIP38
   * `bip38_to_private_key` - Decodificar una clave privada usando BIP38

## Usar funciones BIP39

Algunas funciones de palabras mnemónicas de BIP39 fueron vistas en detalle en [§17.2: Usando BIP39 en Libwally](17_2_Usando_BIP39_en_Libwally.md):

   * `bip39_get_languages` - Ver una lista de idiomas soportados
   * `bit39_get_word` - Recuperar una palabra específica de la lista de palabras de un idioma
   * `bip39_get_wordlist` - Ver una lista de palabras para un idioma

## Usar funciones PSBT

Los listados de la mayoría de las funciones de PSBT se pueden encontrar en [17.4: Usando PSBTs en Libwally](17_4_Usando_PSBTs_en_Libwally.md).

## Usar funciones de script

[§17.5: Usar scripts en Libwally](17_5_Usando_Scripts_en_Libwally.md) apenas tocó las funciones de Scripts de Libwally.

Hay otra función que le permite determinar el tipo de script que se encuentra en una transacción:

   * `wally_scriptpubkey_get_type` - Determinar el tipo de script de una transacción.

Luego hay un montón de funciones que crean `scriptPubKey` a partir de bytes, `scriptSig` a partir de firmas, y Testigos a partir de bytes o firmas.

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

## Usar funciones de transacción

También apenas tocamos las funciones que se pueden usar para crear y convertir funciones en [§17.5](17_5_Usando_Scripts_en_Libwally.md).

Hay numerosas funciones informativas, algunas de las más interesantes de las cuales son:

   * `wally_tx_get_length`
   * `wally_tx_get_total_output_satoshi`
   * `wally_tx_get_weight`
   
También hay funciones que afectan a `wally_tx`, a `wally_tx_input`, a `wally_tx_output`, o a `wally_tx_witness_stack` y que crean firmas.
## Usar funciones de elementos

Libwally se puede compilar para ser utilizado con los elementos de Blockstream, que incluye acceso a sus funciones de activos.

## Resumen: Uso de otras funciones en Libwally

Hay mucho más que usted puede hacer con Libwally, más de lo que puede ser cubierto en este capítulo o incluso enumerado en esta sección. Notablemente, puede realizar funciones criptográficas, codificar claves privadas, construir transacciones completas y usar elementos. Los [documentos Libwally](https://wally.readthedocs.io/en/latest/) son el lugar para ir a por más información, aunque a partir de este escrito son a la vez limitados y anticuados. Los archivos de cabecera de Libwally son una copia de seguridad si los documentos están incompletos o mal.

## ¿Qué es lo siguiente?

Termine de aprender sobre "Programar Bitcoin con Libwally" en [§17.7: Integrar Libwally y Bitcoin-CLI](17_7_Integrando_Libwally_y_Bitcoin-CLI.md).
