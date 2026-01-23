# 18.5: Accessing Bitcoind with Rust

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This section explains how to interact with `bitcoind` using the Rust programming language and the [`bitcoincore-rpc` crate](https://github.com/rust-bitcoin/rust-bitcoincore-rpc).

## Set Up Rust

You need to install both Rust and Cargo.

They can be installed via `curl`. Just use the "default" installation:
```vim
$ curl https://sh.rustup.rs -sSf | sh
```
If everything goes well, you should see:
```vim
Rust is installed now. Great!
```
You'll then need to either logout and back in, or else add Cargo's binary directory to your path by hand:
```
$ source $HOME/.cargo/env
```

### Set Up `bitcoincore-rpc`

For most programming languages, you need to install a Bitcoin RPC library before you create your first project, but here you'll do it as part of your project creation.

### Create a `bitcoincore-rpc` Project

You can create a new project using `cargo new btc_test`:

```
$ cargo new btc_test
     Created binary (application) `btc_test` package
```

This will create a `btc_test` directory that contains a "hello world" source-code example in `src/main.rs` and a `Cargo.toml` file. 

You'll compile and run your code with `cargo run`:
```
$ cd btc_test
$ cargo run
   Compiling btc_test v0.1.0 (/home/standup/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 0.14s
     Running `target/debug/btc_test`
Hello, world!
```

> :information_source: **NOTE:** if you run into error `linker ‘cc’ not found`, you'll have to install a
C compiler. If on Linux, go ahead and install the [development tools](https://www.ostechnix.com/install-development-tools-linux/).

In order to access the `bitcoincore-rpc` crate (library), you must add it to your `Cargo.toml`
file under the section `dependencies`:

```rust
[dependencies]
bitcoincore-rpc = "0.11.0"
```

When you `cargo run` again, it will install the crate and
its (numerous) dependencies.
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

When you are using `bitcoin-rpc`, you will typically need to include the following:
```
use bitcoincore_rpc::{Auth, Client, RpcApi};
```

## Build Your Connection

To create a Bitcoin `RPC client`, modify the `src/main.rs`:

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

As usual, make sure to insert your proper user name and password from `~/.bitcoin/bitcoin.conf`. Here, they're used as the arguments for `Auth::UserPass`.

> :link: **TESTNET vs MAINNET:** And, as usual, use port 8332 for mainnet.

When you're done, you should also close your connection:
```rust
    let _ = rpc.stop().unwrap();
```

`cargo run` should successfully compile and run the example with one warning  `warning: unused variable: rpc`

### Make an RPC Call

RPC calls are made using the `rpc` `Client` that you created:

```rust
let mining_info = rpc.get_mining_info().unwrap();
println!("{:#?}", mining_info);
```
Generally, the words in the RPC call are separated by `_`s. A complete list is available at the [crate docs](https://crates.io/crates/bitcoincore-rpc).

### Make an RPC Call with Arguments

Sending an RPC call with arguments using Rust just requires knowing how the function is laid out. For example, the `get_block` function is defined as follows in the [docs](https://docs.rs/bitcoincore-rpc/0.11.0/bitcoincore_rpc/trait.RpcApi.html#method.get_block):

```rust
fn get_block(&self, hash: &BlockHash) -> Result<Block>
```
You just need to allow it to borrow a blockhash, which can be retrieved (for example) by `get_best_block_hash`.

Here's the complete code to retrieve a block hash, turn that into a block, and print it.
```
    let hash = rpc.get_best_block_hash().unwrap();
    let block = rpc.get_block(&hash).unwrap();
    
    println!("{:?}", block);
```

> **NOTE:** Another possible call that we considered for this section was `get_address_info`, but unfortunately as of this writing, the `bitcoincore-rpc` function doesn't work with recent versions of Bitcoin Core due to the crate not addressing the latest API changes in Bitcoin Core. We expect this will be solved in the next crate's release, but in the meantime, _caveat programmer_.

### Run Your Code

You can access the [src code](src/18_5_main-getinfo.rs) and run it. Unfortunately, the "Block" info will come out a bit ugly because this example doesn't include a library to prettify it.
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

## Look Up Funds

You can look up funds without optional arguments using the `get_balance` function:

```rust
let balance = rpc.get_balance(None, None).unwrap();
println!("Balance: {:?} BTC", balance.as_btc());
```
As shown, the `as_btc()` function helps to output the balance in a readable form:
```
Balance: 3433.71692741 BTC
```

## Create an Address

Creating an address demonstrates how to make an RPC call with multiple optional arguments specified (e.g., a label and an address type).

```rust
// Generate a new address
let myaddress = rpc
    .get_new_address(Option::Some("BlockchainCommons"), Option::Some(json::AddressType::Bech32))
    .unwrap();
println!("address: {:?}", myaddress);
```
This will also require you to bring the `json` definition into scope:
```rust
use bitcoincore_rpc::{json, Auth, Client, RpcApi};
```

## Send a Transaction

You now have everything you need to create a transaction, which will be done in five parts:

1. List UTXOs
2. Populate Variables
3. Create Raw Transaction
4. Sign Transaction
5. Send Transaction

### 1. List UTXOs

To start the creation of a transaction, you first find a UTXO to use. The following takes the first UTXO with at least 0.01 BTC

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
This will require bringing more structures into scope:

```rust
use bitcoincore_rpc::bitcoin::{Address, Amount};
```

Note that you're passing `list_unspent` five variables. The first four (`minconf`, `maxconf`, `addresses`, and `include_unsafe`) aren't used here. The fifth is `query_options`, which we haven't used before, but has some powerful filtering options, including the ability to only look at UTXOs with a certain minimum (or maximum) value.

### 2. Populate Variables

To begin populating the variables that you'll need to create a new transaction, you create the input from the `txid` and the `vout` of the UTXO that you selected:
```rust
let selected_utxos = json::CreateRawTransactionInput {
    txid: selected_tx.txid,
    vout: selected_tx.vout,
    sequence: None,
};
```
Next, you can calculate the amount you're going to spend by subtracting a mining fee from the funds in the UTXO:
```
// send all bitcoin in the UTXO except a minor value which will be paid to miners
let unspent_amount = selected_tx.amount;
let amount = unspent_amount - Amount::from_btc(0.00001).unwrap();
```
Finally, you can create a hash map of the address and the amount to form the output:
```
let mut output = HashMap::new();
output.insert(
    myaddress.to_string(),
    amount,
);
```
Another trait is necessary for the output variable: `HashMap`. It allows you to store
values by key, which you need to represent `{address : amount}` information.

```rust
use std::collections::HashMap;
```

### 3. Create Raw Transaction

You are ready to create a raw transaction:
```rust
let unsigned_tx = rpc
    .create_raw_transaction(&[selected_utxos], &output, None, None)
    .unwrap();
```
### 4. Sign Transaction

Signing your transaction can be done with a simple use of `sign_raw_transaction_with_wallet`:
```rust
let signed_tx = rpc
    .sign_raw_transaction_with_wallet(&unsigned_tx, None, None)
    .unwrap();

println!("signed tx {:?}", signed_tx.transaction().unwrap());
```

### 5. Send Transaction

Finally, you can broadcast the transaction:

```rust
let txid_sent = rpc
    .send_raw_transaction(&signed_tx.transaction().unwrap())
    .unwrap();

println!("{:?}", txid_sent);
```

### Run Your Code

You can now run the complete code from the [src](src/18_5_main-sendtx.rs).

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

## Summary: Accessing Bitcoind with Rust

`bitcoincore-rpc` is a simple and robust crate that will allow you to interact with Bitcoin RPC using Rust. However, as of this writing it has fallen behind Bitcoin Core, which might cause some issues with usage.

## What's Next?

Learn more about "Talking to Bitcoin in Other Languages" in [18.6: Accessing Bitcoin with Swift](18_6_Accessing_Bitcoind_with_Swift.md).

