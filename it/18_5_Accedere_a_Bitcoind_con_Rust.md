# 18.5 Accedere a Bitcoind con Rust

> :information_source: **NOTA:** Questa sezione è stata aggiunta recentemente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Questa sezione spiega come interagire con `bitcoind` utilizzando il linguaggio di programmazione Rust e il [`bitcoincore-rpc` crate](https://github.com/rust-bitcoin/rust-bitcoincore-rpc).

## Configurare Rust

È necessario installare sia Rust che Cargo.

Possono essere installati tramite `curl`. Utilizza semplicemente l'installazione "default":

```
$ curl https://sh.rustup.rs -sSf | sh
```

Se tutto va bene, dovresti vedere:

```
$ Rust is installed now. Great!
```

Dovrai quindi disconnetterti e ricollegarti, oppure aggiungere manualmente la directory binaria di Cargo al tuo percorso:

```
$ source $HOME/.cargo/env
```

### Configurare `bitcoincore-rpc`

Per la maggior parte dei linguaggi di programmazione, è necessario installare una libreria Bitcoin RPC prima di creare il tuo primo progetto, ma qui lo farai come parte della creazione del progetto.

### Creare un Progetto `bitcoincore-rpc`

Puoi creare un nuovo progetto usando `cargo new btc_test`:

```
$ cargo new btc_test
     Created binary (application) `btc_test` package
```
Questo creerà una directory `btc_test` che contiene un esempio di codice sorgente "hello world" in `src/main.rs` e un file `Cargo.toml`.

Compila ed esegui il codice con `cargo run`:

```
$ cd btc_test
$ cargo run
   Compiling btc_test v0.1.0 (/home/standup/btc_test)
    Finished dev [unoptimized + debuginfo] target(s) in 0.14s
     Running `target/debug/btc_test`
Hello, world!
```

> :information_source: **NOTA:** se riscontri l'errore `linker ‘cc’ not found`, dovrai installare un compilatore C. Se sei su Linux, procedi con l'installazione degli [strumenti di sviluppo](https://www.ostechnix.com/install-development-tools-linux/).

Per accedere al crate (libreria) `bitcoincore-rpc`, devi aggiungerlo al tuo file `Cargo.toml` nella sezione `dependencies`:

```
[dependencies]
bitcoincore-rpc = "0.11.0"
```

Quando esegui nuovamente `cargo run`, installerà il `crate` e le sue (numerose) dipendenze.

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

Quando usi `bitcoin-rpc`, in genere devi includere quanto segue:

```
use bitcoincore_rpc::{Auth, Client, RpcApi};
```

## Costruire la Connessione

Per creare un `RPC client` Bitcoin, modifica il file `src/main.rs`:

```
use bitcoincore_rpc::{Auth, Client, RpcApi};
fn main() {
    let rpc = Client::new(
        "http://localhost:18332".to_string(),
        Auth::UserPass("StandUp".to_string(), "password".to_string()),
    )
    .unwrap();
}
```

Come al solito, assicurati di inserire il tuo nome utente e password corretti da `~/.bitcoin/bitcoin.conf`. Qui, vengono utilizzati come argomenti per `Auth::UserPass`.

> :link: **TESTNET vs MAINNET:** E, come al solito, usa la porta 8332 per mainnet.

Quando hai finito, dovresti anche chiudere la connessione:

```
    let _ = rpc.stop().unwrap();
```

`cargo run` dovrebbe compilare ed eseguire correttamente l'esempio con un avviso `warning: unused variable: rpc`

### Effettuare una Chiamata RPC

Le chiamate RPC vengono effettuate utilizzando il `rpc` `Client` che hai creato:

```
let mining_info = rpc.get_mining_info().unwrap();
println!("{:#?}", mining_info);
```

Generalmente, le parole nella chiamata RPC sono separate da `_`. Un elenco completo è disponibile nella [documentazione del crate](https://crates.io/crates/bitcoincore-rpc).

### Effettuare una Chiamata RPC con Argomenti

Inviare una chiamata RPC con argomenti utilizzando Rust richiede solo di sapere come è strutturata la funzione. Ad esempio, la funzione `get_block` è definita come segue nella [documentazione](https://docs.rs/bitcoincore-rpc/0.11.0/bitcoincore_rpc/trait.RpcApi.html#method.get_block):

```
fn get_block(&self, hash: &BlockHash) -> Result<Block>
```

Devi solo consentire che prenda in prestito un blockhash, che può essere recuperato (ad esempio) con `get_best_block_hash`.

Ecco il codice completo per recuperare un block hash, trasformarlo in un block e stamparlo.

```
    let hash = rpc.get_best_block_hash().unwrap();
    let block = rpc.get_block(&hash).unwrap();
    
    println!("{:?}", block);
```

> **NOTA:** Un'altra chiamata possibile che abbiamo considerato per questa sezione era `get_address_info`, ma purtroppo, al momento della scrittura, la funzione `bitcoincore-rpc` non funziona con le versioni recenti di Bitcoin Core a causa del crate che non affronta gli ultimi cambiamenti dell'API in Bitcoin Core. Ci aspettiamo che questo venga risolto nella prossima release del crate, ma nel frattempo, _attenzione programmatore_.

### Esegui il Codice

Puoi accedere al [codice sorgente](src/18_5_main-getinfo.rs) ed eseguirlo. Sfortunatamente, le informazioni sul "Block" risulteranno un po' brutte perché questo esempio non include una libreria per renderle più leggibili.

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

## Controllare i Fondi

Puoi controllare i fondi senza argomenti opzionali utilizzando la funzione `get_balance`:

```
let balance = rpc.get_balance(None, None).unwrap();
println!("Balance: {:?} BTC", balance.as_btc());
```

Come mostrato, la funzione `as_btc()` aiuta a visualizzare il saldo in una forma leggibile:

```
Balance: 3433.71692741 BTC
```

## Creare un Indirizzo

Creare un indirizzo dimostra come effettuare una chiamata RPC con più argomenti opzionali specificati (ad esempio, un'etichetta e un tipo di indirizzo).

```
// Generate a new address
let myaddress = rpc
    .get_new_address(Option::Some("BlockchainCommons"), Option::Some(json::AddressType::Bech32))
    .unwrap();
println!("address: {:?}", myaddress);
```

Questo richiederà anche di portare la definizione `json` nel contesto:

```
use bitcoincore_rpc::{json, Auth, Client, RpcApi};
```

## Inviare una Transazione

Ora hai tutto ciò che ti serve per creare una transazione, che verrà effettuata in cinque parti:

1. Elenca gli UTXO
2. Popola le Variabili
3. Crea la Transazione Grezza
4. Firma la Transazione
5. Invia la Transazione

### 1. Elenca gli UTXO

Per iniziare la creazione di una transazione, devi prima trovare un UTXO da utilizzare. Il seguente codice prende il primo UTXO con almeno 0.01 BTC

```
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

Questo richiederà di portare più strutture nel contesto:

```
use bitcoincore_rpc::bitcoin::{Address, Amount};
```

Nota che stai passando a `list_unspent` cinque variabili. Le prime quattro (`minconf`, `maxconf`, `addresses`, e `include_unsafe`) non sono utilizzate qui. La quinta è `query_options`, che non abbiamo usato prima, ma ha alcune potenti opzioni di filtro, inclusa la possibilità di guardare solo agli UTXO con un certo valore minimo (o massimo).

### 2. Popola le Variabili

Per iniziare a popolare le variabili di cui avrai bisogno per creare una nuova transazione, crea l'input dal `txid` e dal `vout` dell'UTXO che hai selezionato:

```
let selected_utxos = json::CreateRawTransactionInput {
    txid: selected_tx.txid,
    vout: selected_tx.vout,
    sequence: None,
};
```

Successivamente, puoi calcolare l'importo che intendi spendere sottraendo una commissione di mining dai fondi nell'UTXO:

```
// send all bitcoin in the UTXO except a minor value which will be paid to miners
let unspent_amount = selected_tx.amount;
let amount = unspent_amount - Amount::from_btc(0.00001).unwrap();
```

Infine, puoi creare una mappa hash dell'indirizzo e dell'importo per formare l'output:

```
let mut output = HashMap::new();
output.insert(
    myaddress.to_string(),
    amount,
);
```

Un altro trait è necessario per la variabile di output: `HashMap`. Ti permette di memorizzare
valori per chiave, di cui hai bisogno per rappresentare le informazioni `{address : amount}`.

```
use std::collections::HashMap;
```

### 3. Crea la Transazione Grezza

Sei pronto per creare una transazione grezza:

```
let unsigned_tx = rpc
    .create_raw_transaction(&[selected_utxos], &output, None, None)
    .unwrap();
```

### 4. Firma la Transazione

Firmare la tua transazione può essere fatto con un semplice uso di `sign_raw_transaction_with_wallet`:

```
let signed_tx = rpc
    .sign_raw_transaction_with_wallet(&unsigned_tx, None, None)
    .unwrap();

println!("signed tx {:?}", signed_tx.transaction().unwrap());
```

### 5. Invia la Transazione

Infine, puoi trasmettere la transazione:

```
let txid_sent = rpc
    .send_raw_transaction(&signed_tx.transaction().unwrap())
    .unwrap();
println!("{:?}", txid_sent);
```

### Esegui il Codice

Ora puoi eseguire il codice completo dal [codice sorgente](src/18_5_main-sendtx.rs).

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

## Riepilogo: Accesso a Bitcoind con Rust

`bitcoincore-rpc` è un crate semplice e robusto che ti permetterà di interagire con Bitcoin RPC utilizzando Rust. Tuttavia, al momento della scrittura, è rimasto indietro rispetto a Bitcoin Core, il che potrebbe causare alcuni problemi di utilizzo.

## Cosa segue?

Scopri di più su "Parlare con Bitcoin in Altri Linguaggi" nel [Capitolo 18.6: Accedere a Bitcoind con Swift](18_6_Accedere_a_Bitcoind_con_Swift.md).
