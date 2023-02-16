# 18.5: Acceso a Bitcoind con Rust

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente 
de revisión. Lectura con advertencias

Esta sección explica cómo interactuar con `bitcoind` usando el lenguaje de programación Rust y la [caja `bitcoincore-rpc`](https://github.com/rust-bitcoin/rust-bitcoincore-rpc).

## Establecer Rust
Es necesario instalar tanto Rust como Cargo.

Se pueden instalar a través de `curl`. Simplemente utilice la instalación "predeterminada"
```vim
$ curl https://sh.rustup.rs -sSf | sh
```
Si todo va bien, usted debería ver:
```vim
Rust is installed now. Great!
```
A continuación, tendrá que cerrar la sesión y volver a entrar, o bien añadir el directorio binario de Cargo a su ruta a mano:
```
$ source $HOME/.cargo/env
```
### Configurar `bitcoincore-rpc`

Para la mayoría de los lenguajes de programación, necesita instalar una biblioteca RPC de Bitcoin antes de crear su primer proyecto, pero aquí usted lo hara como parte de la creación de su proyecto.

### Crear un proyecto de bitcoincore-rpc

Puede crear un nuevo proyecto usando `cargo new btc_test`:
```
$ cargo new btc_test
     Created binary (application) `btc_test` package
```
Esto creará un directorio `btc_test` que contiene un ejemplo de código fuente "hello world" en `src/main.rs` y un archivo `Cargo.toml` . 

Compilará y ejecutará su código con `cargo run`:
```
$ cd btc_test
$ cargo run
   Compiling btc_test v0.1.0 (/home/standup/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 0.14s
     Running `target/debug/btc_test`
Hello, world!
```
> :information_source: **NOTA:** si se encuentra con un error `linker Žcc' no encontrado`, tendrá que instalar un
Compilador C. Si está en Linux, adelante e instale las [herramientas de desarrollo](https://www.ostechnix.com/install-development-tools-linux/).

Para acceder a la caja (biblioteca) de `bitcoincore-rpc`, debe añadirla a su `Cargo.toml` en la sección `dependencies`:
```rust
[dependencies]
bitcoincore-rpc = "0.11.0"
```
Cuando usted `carga ejecutar` de nuevo, se instalará la caja y sus (numerosas) dependencias.
```
$ cargo run
    Updating crates.io index
   ...
   Compiling bitcoin v0.23.0
   Compiling bitcoincore-rpc-json v0.11.0
   Compiling bitcoincore-rpc v0.11.0
   Compiling btc_test v0.1.0 (/home/standup/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 23.56s
     Running `target/debug/btc_test`
Hello, world!
```
Cuando esté usando `bitcoin-rpc`, normalmente necesitará incluir lo siguiente:
```
use bitcoincore_rpc::{Auth, Client, RpcApi};
```
## Construya su conexión

Para crear un cliente de Bitcoin `RPC`, modifique `src/main.rs`:
```rust
use bitcoincore_rpc::{Auth, Client, RpcApi};

fn main() {
    let rpc = Client::new(
        "http://localhost:18332".to_string(),
        Auth::UserPass("StandUp".to_string(), "password".to_string()),
    )
    .unwrap();
}
```
Como siempre, asegurese de insertar su nombre de usuario y contraseña desde `~/. bitcoin/bitcoin.conf`. Aquí, se utilizan como argumentos para `Auth::UserPass`.

> :link: **TESTNET vs MAINNET:** Y, como de costumbre, usar el puerto 8332 para mainnet.

Cuando termine, también debe cerrar su conexión:
```rust
    let _ = rpc.stop().unwrap();
```
`cargo run` debe compilar con éxito y ejecutar el ejemplo con una advertencia  `warning: variable no utilizada: rpc`

### Hacer una llamada RPC

Las llamadas RPC se hacen usando el cliente `rpc` `que usted creó:
```rust
let mining_info = rpc.get_mining_info().unwrap();
println!("{:#?}", mining_info);
```
Generalmente, las palabras en la llamada RPC están separadas por `_`s. Una lista completa está disponible en [documentos crate](https://crates.io/crates/bitcoincore-rpc).

### Hacer una llamada RPC con argumentos

Enviar una llamada RPC con argumentos usando Rust solo requiere saber cómo se presenta la función. Por ejemplo, la función `get_block` se define como sigue en [docs](https://docs.rs/bitcoincore-rpc/0.11.0/bitcoincore_rpc/trait.RpcApi.html#method.get_block):
```rust
fn get_block(&self, hash: &BlockHash) -> Result<Block>
```
Sólo tiene que permitir que tome prestado un blockhash, que puede ser recuperado (por ejemplo) por `get_best_block_hash`.

Aquí está el código completo para recuperar un hash de bloque, convertirlo en un bloque, e imprimirlo.
```
    let hash = rpc.get_best_block_hash().unwrap();
    let block = rpc.get_block(&hash).unwrap();
    
    println!("{:?}", block);
```
> **NOTA:** Otra llamada posible que consideramos para esta sección fue `get_address_info`, pero desafortunadamente en este momento, la función `bitcoincore-rpc` no funciona con las versiones recientes de Bitcoin Core debido a que la caja no aborda los últimos cambios de API en Bitcoin Core. Esperamos que esto se resuelva en el próximo lanzamiento de la caja, pero mientras tanto, _programador con advertencias_.

### Ejecute su código

Puede acceder al [código src](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_5_main-getinfo.rs) y ejecutarlo. Desafortunadamente, la información de "Block" saldrá un poco fea porque este ejemplo no incluye una biblioteca para embellecerla.
```
$ cargo run 
   Compiling btc_test v0.1.0 (/home/standup/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 1.61s
     Running `target/debug/btc_test`
GetMiningInfoResult {
    blocks: 1832335,
    current_block_weight: None,
    current_block_tx: None,
    difficulty: 4194304.0,
    network_hash_ps: 77436285865245.1,
    pooled_tx: 4,
    chain: "test",
    warnings: "Warning: unknown new rules activated (versionbit 28)",
}
Block { header: BlockHeader { version: 541065216, prev_blockhash: 000000000000027715981d5a3047daf6819ea3b8390b73832587594a2074cbf5, merkle_root: 4b2e2c2754b6ed9cf5c857a66ed4c8642b6f6b33b42a4859423e4c3dca462d0c, time: 1599602277, bits: 436469756, nonce: 218614401 }, txdata: [Transaction { version: 1, lock_time: 0, input: [TxIn { previous_output: OutPoint { txid: 0000000000000000000000000000000000000000000000000000000000000000, vout: 4294967295 }, script_sig: Script(OP_PUSHBYTES_3 8ff51b OP_PUSHBYTES_22 315448617368263538434f494e1d00010320a48db852 OP_PUSHBYTES_32 <push past end>), sequence: 4294967295, witness: [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]] }], output: [TxOut { value: 19721777, script_pubkey: Script(OP_HASH160 OP_PUSHBYTES_20 011beb6fb8499e075a57027fb0a58384f2d3f784 OP_EQUAL) }, TxOut { value: 0, script_pubkey: Script(OP_RETURN OP_PUSHBYTES_36 aa21a9ed63363f3620ab5e38b8860a50c84050e5ec31af3636bbd73f01ba9f14103100ee) }] }, Transaction { version: 2, lock_time: 1832282, input: [TxIn { previous_output: OutPoint { txid: cbf880f73d421baf0aa4f0d28e63ba00e5bc6bd934b91eb0641354ce5ca42f7e, vout: 0 }, script_sig: Script(OP_PUSHBYTES_22 00146b8dbd32e5deb90d22934e1513bae6e70156cd50), sequence: 4294967294, witness: [[48, 68, 2, 32, 13, 89, 205, 30, 67, 24, 196, 83, 65, 224, 44, 138, 98, 58, 81, 135, 132, 209, 23, 166, 23, 44, 3, 228, 95, 102, 166, 214, 62, 38, 155, 147, 2, 32, 119, 2, 34, 246, 148, 255, 166, 10, 90, 52, 242, 32, 74, 241, 123, 148, 89, 199, 197, 3, 152, 134, 242, 215, 109, 61, 241, 241, 13, 70, 86, 207, 1], [2, 192, 145, 170, 206, 55, 4, 36, 138, 145, 217, 50, 19, 73, 130, 136, 245, 131, 184, 142, 239, 75, 13, 67, 17, 177, 57, 86, 151, 139, 89, 35, 109]] }], output: [TxOut { value: 1667908, script_pubkey: Script(OP_HASH160 OP_PUSHBYTES_20 908ca2b8b49ccf53efa2226afa85f6cc58dfd7e7 OP_EQUAL) }, TxOut { value: 9093, script_pubkey: Script(OP_DUP OP_HASH160 OP_PUSHBYTES_20 42ee67664ce16edefc68ad0e4c5b7ce2fc2ccc18 OP_EQUALVERIFY OP_CHECKSIG) }] },  ...] }
```
## Buscar fondos

Puede buscar fondos sin argumentos opcionales usando la función `get_balance` :
```rust
let balance = rpc.get_balance(None, None).unwrap();
println!("Balance: {:?} BTC", balance.as_btc());
```
Como se muestra, la función `as_btc()` ayuda a mostrar el balance de forma legible:
```
Balance: 3433.71692741 BTC
```
## Crear una dirección

Crear una dirección demuestra cómo hacer una llamada RPC con múltiples argumentos opcionales especificados (por ejemplo, una etiqueta y un tipo de dirección).
```rust
// Generate a new address
let myaddress = rpc
    .get_new_address(Option::Some("BlockchainCommons"), Option::Some(json::AddressType::Bech32))
    .unwrap();
println!("address: {:?}", myaddress);
```
Esto también requerirá que lleve la definición `json` al ámbito:
```rust
use bitcoincore_rpc::{json, Auth, Client, RpcApi};
```
## Enviar una transacción

Ahora tiene todo lo que necesita para crear una transacción, que se hará en cinco partes:

1. Lista de los UXTOS
2. Variables de población
3. Crear transacción sin procesar
4. Transacción por señas
5. Enviar transacción

### 1. Lista UTXOs

Para iniciar la creación de una transacción, primero se debe encontrar un UTXO a utilizar. Lo siguiente toma el primer UTXO con al menos 0.01 BTC
```rust
let unspent = rpc
.list_unspent(
    None,
    None,
    None,
    None,
    Option::Some(json::ListUnspentQueryOptions {
        minimum_amount: Option::Some(Amount::from_btc(0.01).unwrap()),
        maximum_amount: None,
        maximum_count: None,
        minimum_sum_amount: None,
    }),
)
.unwrap();

let selected_tx = &unspent[0];

println!("selected unspent transaction: {:#?}", selected_tx);
```
Para ello será necesario ampliar las estructuras:
```rust
use bitcoincore_rpc::bitcoin::{Address, Amount};
```
Tenga en cuenta que está pasando cinco variables a `list_unspent`. Las primeras cuatro (`minconf`, `maxconf`, `addresses`, y `include_unsafe`) no se utilizan aquí. El quinto es `query_options`, que no hemos utilizado antes, pero tiene algunas opciones de filtrado potentes, incluyendo la capacidad de mirar sólo a UTXOs con un cierto valor mínimo (o máximo).

### 2. Poblar variables

Para comenzar a poblar las variables que necesitará para crear una nueva transacción, cree la entrada desde el `txid` y el `vout` del UTXO que seleccionó:
```rust
let selected_utxos = json::CreateRawTransactionInput {
    txid: selected_tx.txid,
    vout: selected_tx.vout,
    sequence: None,
};
```
A continuación, puede calcular la cantidad que va a gastar restando una cuota minera de los fondos de la UTXO:
```
// send all bitcoin in the UTXO except a minor value which will be paid to miners
let unspent_amount = selected_tx.amount;
let amount = unspent_amount - Amount::from_btc(0.00001).unwrap();
```
Finalmente, puede crear un mapa hash de la dirección y la cantidad para formar la salida:
```
let mut output = HashMap::new();
output.insert(
    myaddress.to_string(),
    amount,
);
```
Otro rasgo es necesario para la variable de salida: `HashMap`. Le permite almacenar valores por clave, que necesita para representar la información `{dirección : cantidad}`.

```rust
use std::collections::HashMap;
```
### 3. Crear transacción sin procesar

Está listo para crear una transacción sin procesar:
```rust
let unsigned_tx = rpc
    .create_raw_transaction(&[selected_utxos], &output, None, None)
    .unwrap();
```
### 4. Firmar la Transacción

La firma de su transacción se puede hacer con un simple uso de `sign_raw_transaction_with_wallet`:
```rust
let signed_tx = rpc
    .sign_raw_transaction_with_wallet(&unsigned_tx, None, None)
    .unwrap();

println!("signed tx {:?}", signed_tx.transaction().unwrap());
```
### 5. Enviar transacción

Por último, puede transmitir la transacción:
```rust
let txid_sent = rpc
    .send_raw_transaction(&signed_tx.transaction().unwrap())
    .unwrap();

println!("{:?}", txid_sent);
```
### Ejecute su código

Ahora puede ejecutar el código completo desde [src](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/src/18_5_main-sendtx.rs).
```
$ cargo run
   Compiling btc_test v0.1.0 (/home/standup/btc_test)
warning: unused variable: `unspent_amount`
  --> src/main.rs:86:9
   |
86 |     let unspent_amount = selected_tx.amount;
   |         ^^^^^^^^^^^^^^ help: if this is intentional, prefix it with an underscore: `_unspent_amount`
   |
   = note: `#[warn(unused_variables)]` on by default

warning: 1 warning emitted

    Finished dev [unoptimized + debuginfo] target(s) in 2.11s
     Running `target/debug/btc_test`
Balance: 0.01031434 BTC
address: tb1qx5jz36xgt9q2rkh4daee8ewfj0g5z05v8qsua2
selected unspent transaction: ListUnspentResultEntry {
    txid: 84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e,
    vout: 1,
    address: Some(
        tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4,
    ),
    label: None,
    redeem_script: None,
    witness_script: None,
    script_pub_key: Script(OP_0 OP_PUSHBYTES_20 1e127c28a5d696c01aee1adaa8662396a5902fff),
    amount: Amount(1029734 satoshi),
    confirmations: 1246,
    spendable: true,
    solvable: true,
    descriptor: Some(
        "wpkh([ce0c7e14/0\'/1\'/26\']02c581259ba7e6aef6d7ea23adb08f7c7f10c4c678f2e097a4074639e7685d4805)#j3pctfhf",
    ),
    safe: true,
}
unsigned tx Transaction {
    version: 2,
    lock_time: 0,
    input: [
        TxIn {
            previous_output: OutPoint {
                txid: 84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e,
                vout: 1,
            },
            script_sig: Script(),
            sequence: 4294967295,
            witness: [],
        },
    ],
    output: [
        TxOut {
            value: 1028734,
            script_pubkey: Script(OP_0 OP_PUSHBYTES_20 352428e8c85940a1daf56f7393e5c993d1413e8c),
        },
    ],
}
signed tx Transaction { version: 2, lock_time: 0, input: [TxIn { previous_output: OutPoint { txid: 84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e, vout: 1 }, script_sig: Script(), sequence: 4294967295, witness: [[48, 68, 2, 32, 98, 230, 199, 113, 156, 242, 158, 42, 148, 229, 239, 44, 9, 226, 127, 219, 72, 51, 26, 135, 44, 212, 179, 200, 213, 63, 56, 167, 0, 55, 236, 235, 2, 32, 41, 43, 30, 109, 60, 162, 124, 67, 20, 126, 4, 107, 124, 95, 9, 200, 132, 246, 147, 235, 176, 55, 59, 45, 190, 18, 211, 201, 143, 62, 163, 36, 1], [2, 197, 129, 37, 155, 167, 230, 174, 246, 215, 234, 35, 173, 176, 143, 124, 127, 16, 196, 198, 120, 242, 224, 151, 164, 7, 70, 57, 231, 104, 93, 72, 5]] }], output: [TxOut { value: 1028734, script_pubkey: Script(OP_0 OP_PUSHBYTES_20 352428e8c85940a1daf56f7393e5c993d1413e8c) }] }
b0eda3517e6fac69e58ae315d7fe7a1981e3a858996cc1e3135618cac9b79d1a
```
## Resumen: Acceso a Bitcoind con Rust

`bitcoincore-rpc` es una caja simple y robusta que le permitirá interactuar con Bitcoin RPC usando Rust. Sin embargo, a partir de esta escritura se ha quedado atrás Bitcoin Core, lo que podría causar algunos problemas con el uso.

## ¿Qué es lo siguiente?

Obtenga más información sobre "Cómo hablar con Bitcoin en otros lenguajes" en [18.6: Cómo acceder a Bitcoin con Swift](18_6_Accediendo_a_Bitcoind_con_Swift.md).
