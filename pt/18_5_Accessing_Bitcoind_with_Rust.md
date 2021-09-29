# 18.5: Acessando o Bitcoind com Rust

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta seção explica como interagir com o `bitcoind` usando a linguagem de programação Rust e o [`bitcoincore-rpc` crate](https://github.com/rust-bitcoin/rust-bitcoincore-rpc).

## Configurando o Rust

Precisaremos instalar o Rust e o Cargo.

Eles podem ser instalados via `curl`. Basta usar a instalação "padrão":
```vim
$ curl https://sh.rustup.rs -sSf | sh
```
Se tudo correr bem, veremos:
```vim
Rust is installed now. Great!
```
Em seguida, precisaremos fazer o logout e o login novamente ou então adicionar o diretório binário do Cargo ao nosso caminho manualmente:
```
$ source $HOME/.cargo/env
```

### Configurando o `bitcoincore-rpc`

Para a maioria das linguagens de programação, precisamos instalar uma biblioteca Bitcoin RPC antes de criar nosso primeiro projeto, mas aqui iremos fazê-lo como parte da criação do nosso projeto.

### Criando um Projeto `bitcoincore-rpc`

Podemos criar um novo projeto usando `cargo new btc_test`:

```
$ cargo new btc_test
     Created binary (application) `btc_test` package
```

Isso criará um diretório `btc_test` que contém um exemplo de código-fonte "hello world" que está no `src/main.rs` e um arquivo `Cargo.toml`.

Vamos compilar e executar nosso código com `cargo run`:
```
$ cd btc_test
$ cargo run
   Compiling btc_test v0.1.0 (/home/standup/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 0.14s
     Running `target/debug/btc_test`
Hello, world!
```

> :information_source: **NOTA:** se encontrarmos o erro `linker‘ cc ’not found`, teremos que instalar um
Compilador C. Se estivermos no Linux, podemos instalar com as [ferramentas de desenvolvimento](https://www.ostechnix.com/install-development-tools-linux/).

Para acessar o crate (biblioteca) `bitcoincore-rpc`, devemos adicioná-lo ao nosso arquivo `Cargo.toml` na seção `dependencies`:

```rust
[dependencies]
bitcoincore-rpc = "0.11.0"
```

Quando usarmos o comando `cargo run` novamente, ele irá instalar os crates e suas (numerosas) dependências.
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

Quando estivermos usando o `bitcoin-rpc`, normalmente precisaremos incluir o seguinte:
```
use bitcoincore_rpc::{Auth, Client, RpcApi};
```

## Construindo Nossa Conexão

Para criarmos um `RPC client` do Bitcoin, vamos modificar o `src/main.rs`:


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

Como de costume, vamos nos certificar de inserir nosso nome de usuário e senha corretos no `~/.bitcoin/bitcoin.conf`. Aqui, eles serão usados ​​como argumentos para `Auth :: UserPass`.

> :link: **TESTNET vs MAINNET:** E, como de costume, use a porta 8332 para a Mainnet.

Quando terminarmos, também devemos fechar nossa conexão:
```rust
    let _ = rpc.stop().unwrap();
```

O `cargo run` deve compilar e executar o exemplo com sucesso com um aviso `warning: unused variable: rpc`

### Fazendo uma Chamada RPC

As chamadas RPC são feitas usando o `rpc Client` que criamos:

```rust
let mining_info = rpc.get_mining_info().unwrap();
println!("{:#?}", mining_info);
```
Geralmente, as palavras na chamada RPC são separadas por `_`s. Uma lista completa está disponível na [documentação do crate](https://crates.io/crates/bitcoincore-rpc).

### Fazendo uma Chamada RPC com Argumentos

O envio de uma chamada RPC com argumentos usando Rust requer apenas o conhecimento de como a função é apresentada. Por exemplo, a função `get_block` é definida da seguinte forma na [documentação](https://docs.rs/bitcoincore-rpc/0.11.0/bitcoincore_rpc/trait.RpcApi.html#method.get_block):

```rust
fn get_block(&self, hash: &BlockHash) -> Result<Block>
```
Só precisamos permitir que ele pegue emprestado um blockhash, que pode ser recuperado (por exemplo) usando o comando `get_best_block_hash`.

Aqui está o código completo para recuperar um hash de bloco, transformando-o em um bloco e imprimi-lo.
```
    let hash = rpc.get_best_block_hash().unwrap();
    let block = rpc.get_block(&hash).unwrap();
    
    println!("{:?}", block);
```

> **NOTA:** Outra chamada possível que consideramos para esta seção é a `get_address_info`, mas, infelizmente, no momento de escrita, a função `bitcoincore-rpc` não funciona com versões recentes do Bitcoin Core devido ao crate não abordar as mais recentes mudanças de API no Bitcoin Core. Esperamos que isso seja resolvido no próximo lançamento do crate, mas, enquanto isso programador, _cuidado_.

### Executando Nosso Código

Podemos acessar o [código src](../src/18_5_main-getinfo.rs) e executá-lo. Infelizmente, a informação do "Block" sairá um pouco feia porque este exemplo não inclui uma biblioteca para embelezá-la.

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

## Procurando por Fundos

Podemos procurar fundos sem argumentos opcionais usando a função `get_balance`:

```rust
let balance = rpc.get_balance(None, None).unwrap();
println!("Balance: {:?} BTC", balance.as_btc());
```
Conforme mostrado, a função `as_btc()` ajuda a gerar o saldo em um formato legível:
```
Balance: 3433.71692741 BTC
```

## Criando um Endereço

A criação de um endereço demonstra como fazer uma chamada RPC com vários argumentos opcionais especificados (por exemplo, um rótulo e um tipo de endereço).

```rust
// Gerar um novo endereço
let myaddress = rpc
    .get_new_address(Option::Some("BlockchainCommons"), Option::Some(json::AddressType::Bech32))
    .unwrap();
println!("address: {:?}", myaddress);
```
Isso também exigirá que tragamos a definição `json` para o escopo:
```rust
use bitcoincore_rpc::{json, Auth, Client, RpcApi};
```

## Enviando uma Transação

Agora temos tudo de que precisamos para criar uma transação, o que será feito em cinco partes:

1. Listar UTXOs;
2. Preencher variáveis;
3. Criar transação bruta;
4. Assinar transação;
5. Enviar transação.

### 1. Listando os UTXOs

Para iniciar a criação de uma transação, primeiro encontramos um UTXO para usar. O seguinte pega o primeiro UTXO com pelo menos 0,01 BTC:


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
Isso nos exigirá trazer mais estruturas para o escopo:

```rust
use bitcoincore_rpc::bitcoin::{Address, Amount};
```

Podemos observar que estamos passando cinco variáveis ​​ao comando `list_unspent`. Os primeiros quatro (`minconf`,` maxconf`, `address` e `include_unsafe`) não são usados ​​aqui. O quinto é o `query_options`, que não usamos antes, mas temos algumas opções de filtragem poderosas, incluindo a capacidade de olhar apenas para os UTXOs com um certo valor, mínimo ou máximo.

### 2. Preenchendo as Variáveis

Para começar a preencher as variáveis ​​que precisaremos para criar uma nova transação, criamos a entrada do `txid` e do `vout` ao UTXO que selecionamos:
```rust
let selected_utxos = json::CreateRawTransactionInput {
    txid: selected_tx.txid,
    vout: selected_tx.vout,
    sequence: None,
};
```
Em seguida, podemos calcular a quantia que vamos gastar subtraindo uma taxa de mineração dos fundos no UTXO:
```rust
// enviar todo o bitcoin no UTXO exceto um valor pequeno que será pago aos mineradores
let unspent_amount = selected_tx.amount;
let amount = unspent_amount - Amount::from_btc(0.00001).unwrap();
```
Por fim, podemos criar um mapa do hash do endereço e da quantidade para formar a saída:
```rust
let mut output = HashMap::new();
output.insert(
    myaddress.to_string(),
    amount,
);
```
Outra característica é necessária para a variável de saída: o `HashMap`. Ele nos permite armazenar
valores por chave, que iremos precisar para representar a informação `{address: amount}`.

```rust
use std::collections::HashMap;
```

### 3. Criando a Transação Bruta

Agora estamos prontos para criar uma transação bruta:
```rust
let unsigned_tx = rpc
    .create_raw_transaction(&[selected_utxos], &output, None, None)
    .unwrap();
```
### 4. Assinando a Transação

A assinatura da transação pode ser feita com um simples uso do `sign_raw_transaction_with_wallet`:
```rust
let signed_tx = rpc
    .sign_raw_transaction_with_wallet(&unsigned_tx, None, None)
    .unwrap();

println!("signed tx {:?}", signed_tx.transaction().unwrap());
```

### 5. Enviando a Transação

Finalmente, podemos transmitir a transação:
```rust
let txid_sent = rpc
    .send_raw_transaction(&signed_tx.transaction().unwrap())
    .unwrap();

println!("{:?}", txid_sent);
```

### Executando Nosso Código

Agora podemos executar o código completo do [src](../src/18_5_main-sendtx.rs).


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

## Resumo: Acessando o Bitcoind com Rust

O `bitcoincore-rpc` é um crate simples e robusto que nos permitirá interagir com o Bitcoin RPC usando Rust. No entanto, no momento em que este livro foi escrito, ele ficou para trás no Bitcoin Core, o que pode causar alguns problemas de uso.

## O Que Vem Depois?

Vamos aprender mais sobre "Conversando com o Bitcoind com Outras Linguagens" na seção [§18.6: Acessando o Bitcoind com Swift](18_6_Accessing_Bitcoind_with_Swift.md).