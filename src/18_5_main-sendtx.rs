use bitcoincore_rpc::{json, Auth, Client, RpcApi};
use bitcoincore_rpc::bitcoin::{Amount};
use std::collections::HashMap;

fn main() {
    let rpc = Client::new(
        "http://localhost:18332".to_string(),
        Auth::UserPass("StandUp".to_string(), "6305f1b2dbb3bc5a16cd0f4aac7e1eba".to_string()),
    )
    .unwrap();

    let balance = rpc.get_balance(None, None).unwrap();
    println!("Balance: {:?} BTC", balance.as_btc());

// 0. Generate a recipient address

    let myaddress = rpc
        .get_new_address(Option::Some("BlockchainCommons"), Option::Some(json::AddressType::Bech32))
        .unwrap();
    println!("address: {:?}", myaddress);

// 1. List UTXOs

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

// 2. Populate Variables

    let selected_utxos = json::CreateRawTransactionInput {
        txid: selected_tx.txid,
        vout: selected_tx.vout,
        sequence: None,
    };

    let unspent_amount = selected_tx.amount;
    let amount = unspent_amount - Amount::from_btc(0.00001).unwrap();

    let mut output = HashMap::new();
    output.insert(
        myaddress.to_string(),
        amount,
    );

// 3. Create Raw Transaction

    let unsigned_tx = rpc
        .create_raw_transaction(&[selected_utxos], &output, None, None)
        .unwrap();

    println!("unsigned tx {:#?}", unsigned_tx);

// 4. Sign Transaction

    let signed_tx = rpc
        .sign_raw_transaction_with_wallet(&unsigned_tx, None, None)
        .unwrap();

    println!("signed tx {:?}", signed_tx.transaction().unwrap());

// 5. Send Transaction

    let txid_sent = rpc
        .send_raw_transaction(&signed_tx.transaction().unwrap())
        .unwrap();

    println!("{:?}", txid_sent);

// 6. Cleanup

    let unspent_amount = selected_tx.amount;

}
