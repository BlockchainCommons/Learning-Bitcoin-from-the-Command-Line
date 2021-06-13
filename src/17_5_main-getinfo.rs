use bitcoincore_rpc::{Auth, Client, RpcApi};

fn main() {
    let rpc = Client::new(
        "http://localhost:18332".to_string(),
        Auth::UserPass("StandUp".to_string(), "6305f1b2dbb3bc5a16cd0f4aac7e1eba"
.to_string()),
    )
    .unwrap();

    let mining_info = rpc.get_mining_info().unwrap();
    println!("{:#?}", mining_info);

    let hash = rpc.get_best_block_hash().unwrap();
    let block = rpc.get_block(&hash).unwrap();
    println!("{:?}", block);

    let _ = rpc.stop().unwrap();

}
