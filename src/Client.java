import java.math.*;
import java.nio.ByteBuffer;
import javax.xml.bind.DatatypeConverter;
import java.util.Hashtable;
import java.util.Enumeration;
import java.math.BigDecimal;
import wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.SignedRawTransaction;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.AddressInfo;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.RawTransactionSigningOrVerificationError;
import wf.bitcoin.javabitcoindrpcclient.*;
import wf.bitcoin.krotjson.HexCoder;
import java.text.DecimalFormat;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.Unspent;
import wf.bitcoin.javabitcoindrpcclient.BitcoinRawTxBuilder;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.Transaction;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.ExtendedTxInput;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.TxInput;
import java.util.ListIterator;
import java.net.URL;
import java.net.MalformedURLException;
import java.util.List;
import java.io.*;
import java.util.*;

public class Client{

	static BitcoinJSONRPCClient bitcoinClient = null;
	static BitcoinRawTxBuilder txb = null;
	static String address = "";
	
	public boolean connect(){
		String user = "user";
	        String password = "password";
		String host = "127.0.0.1";
		String port = "18443";  //regtest
//		String host = "192.168.0.22";  //testnet
//		String port = "18332";  //testnet
                try {
                        URL url = new URL("http://" + user + ':' + password + "@" + host + ":" + port + "/");
                        bitcoinClient = new BitcoinJSONRPCClient(url);
			return true;	
		}catch (MalformedURLException e) {
              		e.printStackTrace(); 
		 	return false; 
                }
	}

	public static void main(String args[]){
		BigDecimal txToAddr2Amount = new BigDecimal("0");
		BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
		Client rpcClient = new Client();
		bitcoin.connect();
		LinkedList<BitcoindRpcClient.ExtendedTxInput> inputList = new LinkedList<BitcoindRpcClient.ExtendedTxInput>();
		txb = new BitcoinRawTxBuilder(bitcoinClient);
//		String addr1 = bitcoinClient.getNewAddress();
		String addr1 = "bcrt1qs4ylwj2v5v0gq7eqzp9k9vxazdrkexhkghxpyp";   //regtest
		AddressInfo addr1Info = bitcoinClient.getAddressInfo(addr1);
		System.out.println("Address: " + addr1Info.address());
		System.out.println("HdKeyPath: " + addr1Info.hdKeyPath());
		System.out.println("PubKey: " + addr1Info.pubKey());
		System.out.println("MasterFingerPrint: " + addr1Info.hdMasterFingerprint());
		System.out.println("Balance: " + bitcoinClient.getBalance());
		
//		String addr1 = "tb1qrj3dgs6kh2mtr7ulgmlx3pvavqwtw6vxdk468y";   //testnet
		System.out.println("Created address addr1: " + addr1);
		String addr2 = bitcoinClient.getNewAddress();
		System.out.println("Created address addr2: " + addr2);
		List<String> generatedBlocksHashes = bitcoinClient.generateToAddress(110, addr1);
		System.out.println("Generated " + generatedBlocksHashes.size() + " blocks for addr1");
		List<Unspent> utxos = bitcoinClient.listUnspent(0, Integer.MAX_VALUE, addr1);
		System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
		/*for(int i =0;i<utxos.size();i++) {
			Unspent selectedUtxo = utxos.get(i);
			System.out.println("Selected UTXO which will be sent from addr1 to addr2: " + selectedUtxo);
			System.out.println("txid:" + selectedUtxo.txid());
			System.out.println("vout:" + selectedUtxo.vout());
			System.out.println("scriptPubKey:" + selectedUtxo.scriptPubKey());
			System.out.println("amount:" + selectedUtxo.amount());
			BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
			txToAddr2Amount = selectedUtxo.amount().subtract(estimatedFee);
		}*/
		TxInput in = utxos.get(0);
		txb.in(in);
		
		txToAddr2Amount = utxos.get(0).amount().subtract(estimatedFee); 
		txb.out(addr2, txToAddr2Amount);
		
		System.out.println("unsignedRawTx in amount: " + utxos.get(0).amount());
		System.out.println("unsignedRawTx out amount: " + txToAddr2Amount);
		
		String unsignedRawTxHex = txb.create();
		System.out.println("Created unsignedRawTx from addr1 to addr2: " + unsignedRawTxHex);
		
		SignedRawTransaction srTx = bitcoinClient.signRawTransactionWithKey(
				unsignedRawTxHex,
				Arrays.asList(bitcoinClient.dumpPrivKey(addr1)), 
				Arrays.asList(in),
				null);
		System.out.println("signedRawTx hex: " + srTx.hex());
		System.out.println("signedRawTx complete: " + srTx.complete());
		
		List<RawTransactionSigningOrVerificationError> errors = srTx.errors();
		if (errors != null)
		{
			System.out.println("Found errors when signing");

			for (RawTransactionSigningOrVerificationError error : errors)
			{
				System.out.println("Error: " + error);
			}
		}
		
		String sentRawTransactionID = bitcoinClient.sendRawTransaction(srTx.hex());
		System.out.println("Sent signedRawTx (txID): " + sentRawTransactionID);
		
	}
}
