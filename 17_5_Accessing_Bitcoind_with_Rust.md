# 17.5: Accessing Bitcoind with Rust

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This section explains how to interact with `bitcoind` using the Rust programming language and the [`bitcoincore-rpc` crate](https://github.com/rust-bitcoin/rust-bitcoincore-rpc).

## Setting Up Rust

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

### Setting Up `bitcoincore-rpc`

For most programming languages, you need to install a Bitcoin RPC library before you create your first project, but here you'll do it as part of your project creation.

### Creating a `bitcoincore-rpc` Project

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

## Building Your Connection

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

As usual, make sure to insert your proper user name and password from `~/.bitcoin/bitcoin.conf`. Here, they're placed as the arguments for `Auth::UserPass`.

> :link: **TESTNET vs MAINNET:** And, as usual, use port 8332 for mainnet.

When you're done, you should also close your connection:
```rust
    let _ = rpc.stop().unwrap();
```

`cargo run` should successfully compile and run the example with one warning  `warning: unused variable: rpc`

### Making an RPC Call

RPC calls made made using the `rpc` `Client` that you created:

```rust
let mining_info = rpc.get_mining_info().unwrap();
println!("{:#?}", mining_info);
```
Generally, the words in the RPC call are separated by `_`s. A complete list is available at the [crate docs](https://docs.rs/bitcoincore-rpc/0.11.0/bitcoincore_rpc/trait.RpcApi.html).

If we wanted we could close the connection:

```rust
    let _ = rpc.stop().unwrap();
```

### Making an RPC Call with Arguments

Sending an RPC call with arguments using Rust just requires knowing how the function is laid out. For example, the `get_block` function is defined as follows in the [docs](https://docs.rs/bitcoincore-rpc/0.11.0/bitcoincore_rpc/trait.RpcApi.html#method.get_block):

```rust
fn get_block(&self, hash: &BlockHash) -> Result<Block>
```
We just need to allow it to borrow a blockhash, which can be retrieved (for example) by `get_best_block_hash` for the newest (and most reliable).

Here's the complete code to retrieve a block hash, turn that into a block, and print it.
```
    let hash = rpc.get_best_block_hash().unwrap();
    let block = rpc.get_block(&hash).unwrap();
    
    println!("{:?}", block);
```

> :note: **NOTE:** Another possible call that we considered for this section was `get_address_info`, but unfortunately as of this writing, the `bitcoincore-rpc` function doesn't work with recent versions of Bitcoin Core due to the crate not addressing the latest API changes in Bitcoin Core. We expect this will be solved in the next crate's release, but in the meantime, _caveat programmer_.

### Running Your Code

You can access the [src code](src/17_5_main-getinfo.rs) and run it. Unfortunately, the "Block" info will come out a bit ugly because we don't have a library to prettify it.
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

### Look Up Funds

We can look up our funds without optional arguments like so:

```rust
let balance = rpc.get_balance(None, None).unwrap();
println!("Balance: {:?} BTC", balance.as_btc());
```

```vim
Balance: 3433.71692741 BTC
```

### Create an Address

Here is an example of calling an RPC method with the optional arguments specified, i.e.
a label and an address type:

```rust
// Generate a new address
let myaddress = rpc
    .get_new_address(Option::Some("BlockchainCommons"), Option::Some(json::AddressType::Bech32))
    .unwrap();
println!("address: {:?}", myaddress);
```

If we have inspected our function's definition we bring the missing things into
scope. Otherwise the compiler will hint us to do so:

```rust
use bitcoincore_rpc::{json, Auth, Client, RpcApi};
```

Program execution results in:

```vim
address: bcrt1q0y0dk70lut5l3y4f0fe52am23egfmr63dejy9r
```

Now, we would like to have some bitcoins to our newly generated address. Since
we are on the `Regtest` network we can generate them ourselves:

```rust
// Generate 101 blocks to our address
let _ = rpc.generate_to_address(101, &myaddress);
```

## Create a Transaction

First, we list unspent transactions. Let's look at those with at least 3 BTC and take the first one:

```rust
let unspent = rpc
.list_unspent(
    None,
    None,
    None,
    None,
    Option::Some(json::ListUnspentQueryOptions {
        minimum_amount: Option::Some(Amount::from_btc(3.0).unwrap()),
        maximum_amount: None,
        maximum_count: None,
        minimum_sum_amount: None,
    }),
)
.unwrap();

let selected_tx = &unspent[0];

println!("selected unspent transaction: {:#?}", selected_tx);
```
Here it is:

```vim
selected unspent transaction: ListUnspentResultEntry {
    txid: 34e283eb5b52c66aba9766bdda46eb038bc1138e992b593c22f7cbf1d2e9ba10,
    vout: 0,
    address: Some(
        bcrt1q7lju6c0ynwerch0te4saxwxgm70ltd3lr9vj6l,
    ),
    label: Some(
        "",
    ),
    redeem_script: None,
    witness_script: None,
    script_pub_key: Script(OP_0 OP_PUSHBYTES_20 f7e5cd61e49bb23c5debcd61d338c8df9ff5b63f),
    amount: Amount(625000000 satoshi),
    confirmations: 4691,
    spendable: true,
    solvable: true,
    descriptor: None,
    safe: true,
}
```

This will require to bring another structure into scope:

```rust
use bitcoincore_rpc::bitcoin::{Address, Amount};
```

We can now populate some variables: the available amount and
the utxo, the recipient's address and the amount we want to send.

```rust
let unspent_amount = selected_tx.amount;

let selected_utxos = json::CreateRawTransactionInput {
txid: selected_tx.txid,
vout: selected_tx.vout,
sequence: None,
};

let recipient = Address::from_str("bcrt1q6rhpng9evdsfnn833a4f4vej0asu6dk5srld6x").unwrap();
println!("recipient: {:?}", recipient);

// send all bitcoin in the UTXO except a minor value which will be paid to miners
let amount = unspent_amount - Amount::from_btc(0.00001).unwrap();

let mut output = HashMap::new();
output.insert(
"bcrt1q6rhpng9evdsfnn833a4f4vej0asu6dk5srld6x".to_string(),
amount,
);
```

Another trait is necessary for the output variable: HashMap. It allows us to store
values by key which we need to represent `{address : amount}` information.

```rust
use std::collections::HashMap;
```

We are ready to create a raw transaction:

```rust
let unsigned_tx = rpc
.create_raw_transaction(&[selected_utxos], &output, None, None)
.unwrap();

println!("unsigned tx {:#?}", unsigned_tx);
```

Here it is:

```vim
unsigned tx Transaction {
    version: 2,
    lock_time: 0,
    input: [
        TxIn {
            previous_output: OutPoint {
                txid: 34e283eb5b52c66aba9766bdda46eb038bc1138e992b593c22f7cbf1d2e9ba10,
                vout: 0,
            },
            script_sig: Script(),
            sequence: 4294967295,
            witness: [],
        },
    ],
    output: [
        TxOut {
            value: 624999000,
            script_pubkey: Script(OP_0 OP_PUSHBYTES_20 d0ee19a0b9636099ccf18f6a9ab3327f61cd36d4),
        },
    ],
}
```

Finally, we can sign and broadcast our transaction:

```rust
// sign transaction
let signed_tx = rpc
.sign_raw_transaction_with_wallet(&unsigned_tx, None, None)
.unwrap();

println!("singed tx {:?}", signed_tx.transaction().unwrap());

// broadcast transaction
let txid_sent = rpc
.send_raw_transaction(&signed_tx.transaction().unwrap())
.unwrap();

println!("{:?}", txid_sent);
```

```vim
singed tx Transaction { version: 2, lock_time: 0, input: [TxIn { previous_output: OutPoint { txid: 34e283eb5b52c66aba9766bdda46eb038bc1138e992b593c22f7cbf1d2e9ba10, vout: 0 }, script_sig: Script(), sequence: 4294967295, witness: [[48, 68, 2, 32, 85, 113, 140, 197, 142, 140, 122, 26, 174, 71, 94, 152, 76, 104, 5, 111, 113, 192, 179, 1, 58, 6, 27, 141, 18, 50, 217, 53, 154, 26, 5, 98, 2, 32, 53, 148, 139, 57, 234, 151, 71, 149, 134, 202, 160, 136, 15, 144, 103, 232, 134, 37, 136, 184, 117, 159, 235, 92, 59, 102, 197, 213, 67, 64, 89, 207, 1], [3, 4, 197, 157, 36, 136, 177, 169, 182, 219, 121, 187, 251, 153, 207, 165, 173, 117, 142, 93, 181, 107, 185, 97, 10, 168, 210, 148, 67, 127, 246, 229, 12]] }], output: [TxOut { value: 624999000, script_pubkey: Script(OP_0 OP_PUSHBYTES_20 d0ee19a0b9636099ccf18f6a9ab3327f61cd36d4) }] }
5d2f1b7c6fc29967d820532c46200b35f62b6e6f8da614ae86922c20167f6d0e
```

## For More Information

You can now mine a block and try to see for yourself if the last transaction is really in the block.
If you need help look at the crate's [documentation](https://crates.io/crates/bitcoincore-rpc) or run some tests in its [repository](https://github.com/rust-bitcoin/rust-bitcoincore-rpc).

## Summary

We have shown how to access `bitcoind` in `Rust` and send a transaction
on the `Bitcoin Regtest Network` explaining all the steps required.
