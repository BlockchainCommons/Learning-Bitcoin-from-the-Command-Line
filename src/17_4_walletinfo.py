from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
from pprint import pprint
import logging

#logging.basicConfig()
#logging.getLogger("BitcoinRPC").setLevel(logging.DEBUG)
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = "StandUp"
rpc_pass = "6305f1b2dbb3bc5a16cd0f4aac7e1eba"
rpc_host = "127.0.0.1"
rpc_client = AuthServiceProxy(f"http://{rpc_user}:{rpc_pass}@{rpc_host}:18332", 
timeout=120)

# Look Up Wallet

wallet_info = rpc_client.getwalletinfo()
print("---------------------------------------------------------------")
print("Wallet Info:")
print("-----------")
pprint(wallet_info)
print("---------------------------------------------------------------\n")

## List UTXOs

utxos = rpc_client.listunspent()
print("Utxos: ")
print("-----")
pprint(utxos)
print("------------------------------------------\n")

## Select a UTXO - first one selected here

utxo_txid = utxos[0]['txid']

## Get UTXO Hex

utxo_hex = rpc_client.gettransaction(utxo_txid)['hex']

## Get tx Details

utxo_tx_details = rpc_client.decoderawtransaction(utxo_hex)
print("Details of Utxo with txid:", utxo_txid)
print("---------------------------------------------------------------")
print("UTXO Details:")
print("------------")
pprint(utxo_tx_details)
print("---------------------------------------------------------------\n")
