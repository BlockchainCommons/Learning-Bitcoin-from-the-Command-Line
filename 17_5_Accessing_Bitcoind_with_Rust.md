# 18.5: Accessing Bitcoind with Rust

## Setup

We'll need `Rust` and `Cargo`. Installing them is easy:

```vim
$ curl https://sh.rustup.rs -sSf | sh
```

If everything goes well, we should see:

```vim
Rust is installed now. Great!
```

To set `Bitcoin Regtest` network up and allow communication with our Rust program we
will be using the following `bitcoind` configuration in `bitcoin.conf`

```vim
regtest=1
server=1
rpcuser=bitcoin
rpcpassword=password
[test]
rpcport=18443
```

> Note: Never use a simple password like that when on Bitcoin Mainnet!

### Create a New Project

We create a new project with `cargo new btc_test`:

```vim
gorazd@gorazd-MS-7C37:~/Projects/BlockchainCommons$ cargo new btc_test
     Created binary (application) `btc_test` package
```

Let's move into the newly created project `btc_test`. We notice a "hello world" example
with the source code in `src/main.rs` and a `Cargo.toml` file. Let's run it with `cargo run`:

```vim
gorazd@gorazd-MS-7C37:~/Projects/BlockchainCommons/btc_test$ cargo run
   Compiling btc_test v0.1.0 (/home/gorazd/Projects/BlockchainCommons/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 0.14s
     Running `target/debug/btc_test`
Hello, world!
```

> Note: if you run into error “linker ‘cc’ not found”, you'll have to install a
C compiler. If on Linux, go ahead and install the [development tools](https://www.ostechnix.com/install-development-tools-linux/).


We will use `bitcoincore-rpc` crate (library), therefore we add it to our `Cargo.toml`
under section `dependencies` like so:

```rust
[dependencies]
bitcoincore-rpc = "0.11.0"
```

Running our example again will install our crate and
its dependencies.

```vim
gorazd@gorazd-MS-7C37:~/Projects/BlockchainCommons/btc_test$ cargo run
    Updating crates.io index
   ...
   Compiling bitcoin v0.23.0
   Compiling bitcoincore-rpc-json v0.11.0
   Compiling bitcoincore-rpc v0.11.0
   Compiling btc_test v0.1.0 (/home/gorazd/Projects/BlockchainCommons/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 23.56s
     Running `target/debug/btc_test`
Hello, world!
```

## Build Your Connection

Let us create a Bitcoin `RPC client` and modify the `main.rs`:

```rust
use bitcoincore_rpc::{Auth, Client};

fn main() {
    let rpc = Client::new(
        "http://localhost:18443".to_string(),
        Auth::UserPass("bitcoin".to_string(), "password".to_string()),
    )
    .unwrap();
}
```

`Cargo run` should successfully compile and run the example with one warning 
`warning: unused variable: rpc`


### Make an RPC Call

This is a simple RPC call without arguments:

```rust
let mining_info = rpc.get_mining_info().unwrap();
println!("{:#?}", mining_info);
```

The compiler will tell us to include traits into scope. So lets add them:

```rust
use bitcoincore_rpc::{Auth, Client, RpcApi};
```

If our properly configured `bitcoind` is running, executing our example should
result in:

```vim
gorazd@gorazd-MS-7C37:~/Projects/BlockchainCommons/btc_test$ cargo run
   Compiling btc_test v0.1.0 (/home/gorazd/Projects/BlockchainCommons/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 0.80s
     Running `target/debug/btc_test`
GetMiningInfoResult {
    blocks: 5167,
    current_block_weight: Some(
        0,
    ),
    current_block_tx: Some(
        0,
    ),
    difficulty: 0.00000000046565423739069247,
    network_hash_ps: 1.764705882352941,
    pooled_tx: 2,
    chain: "regtest",
    warnings: "",
}
```

If we wanted we could close the connection:

```rust
    let _ = rpc.stop().unwrap();
```


## Manipulate Your Wallet

### Look Up Addresses

Here we will make our first call with an argument. To see the type of an argument,
we want to look at the function definition:

```rust
fn get_address_info(&self, address: &Address) -> Result<json::GetAddressInfoResult> {
self.call("getaddressinfo", &[address.to_string().into()])
}
```

We see that our argument is of type `Address` and that it will be borrowed. Further,
looking at the structure `Address`, we notice a convenient `trait` implemented which
allows us to create an `Address` out of a string:

```rust
impl FromStr for Address {
    type Err = Error;

    fn from_str(s: &str) -> Result<Address, Error> {
```

Now that we now what structure and trait we are dealing with, we bring them into
scope

```rust
use bitcoincore_rpc::bitcoin::Address;
use std::str::FromStr;
```

so we can use them:

```rust
let addr = Address::from_str("bcrt1qanga5jxx845q82h9qgjfuedps92lktqv073qct").unwrap();
let addr_info = rpc.get_address_info(&addr).unwrap();
println!("{:?}", addr_info);
```

Running our program results in:

```vim
GetAddressInfoResult { address: bcrt1qanga5jxx845q82h9qgjfuedps92lktqv073qct, script_pub_key: Script(OP_0 OP_PUSHBYTES_20 ecd1da48c63d6803aae502249e65a18155fb2c0c), is_mine: Some(true), is_watchonly: Some(false), is_script: Some(false), is_witness: Some(true), witness_version: Some(0), witness_program: Some([236, 209, 218, 72, 198, 61, 104, 3, 170, 229, 2, 36, 158, 101, 161, 129, 85, 251, 44, 12]), script: None, hex: None, pubkeys: None, n_signatures_required: None, pubkey: Some(PublicKey { compressed: true, key: PublicKey(f895d610ab1ceddfd87814b1f7a911fee1135a9347d4fd1754a06ddf84757c5c527a90804949b025d7272bef4d58a1324c18d7a8f6b7ffa949447bcb6a225e6e) }), embedded: None, is_compressed: None, label: "lbl", timestamp: Some(1582063890), hd_key_path: Some(m/0'/0'/99'), hd_seed_id: Some(00b332a133c03c4e613f0106dc814bcc79af60ff), labels: [GetAddressInfoResultLabel { name: "lbl", purpose: Receive }] }
```

> Note: this call doesn't work with recent versions of Bitcoin Core due to the
crate not addressing the latest API changes in Bitcoin Core.
We expect it to be solved in the next crate's release.

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
