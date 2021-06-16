package com.blockchaincommons.lbtc;

import java.math.BigDecimal;
import java.util.*;

import wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoinRawTxBuilder;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.*;

public class App
{

    public static void main( String[] args ) throws Exception
    {

	BitcoindRpcClient rpcClient = new BitcoinJSONRPCClient("http://StandUp:6305f1b2dbb3bc5a16cd0f4aac7e1eba@localhost:18332");

	/* 1. Prepare two address */
	
	/* Use an address on your system with UTXOs */
	String addr1 = "tb1qdqkc3430rexxlgnma6p7clly33s6jjgay5q8np";
	System.out.println("Used address addr1: " + addr1);

	String addr2 = rpcClient.getNewAddress();
	System.out.println("Created address addr2: " + addr2);

	/* 2. Find a UTXO */
	
	List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
	System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");

	/* 3. Create a Transaction */
	
	BitcoinRawTxBuilder txb = new BitcoinRawTxBuilder(rpcClient);

	/* 3.1 Fill the Input */
	
	TxInput in = utxos.get(0);
	txb.in(in);

	/* 3.2 Fill the Output */
	
	BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
	BigDecimal txToAddr2Amount = utxos.get(0).amount().subtract(estimatedFee); 
	txb.out(addr2, txToAddr2Amount);

	System.out.println("unsignedRawTx in amount: " + utxos.get(0).amount());
	System.out.println("unsignedRawTx out amount: " + txToAddr2Amount);

	/* 4. Sign the Transaction */
	
	String unsignedRawTxHex = txb.create();
	System.out.println("Created unsignedRawTx from addr1 to addr2: " + unsignedRawTxHex);

	SignedRawTransaction srTx = rpcClient.signRawTransactionWithKey(
				unsignedRawTxHex,
				Arrays.asList(rpcClient.dumpPrivKey(addr1)),
				Arrays.asList(in),
				null);
	System.out.println("signedRawTx hex: " + srTx.hex());
	System.out.println("signedRawTx complete: " + srTx.complete());

	/* 5. Send the Transaction */
	
	String sentRawTransactionID = rpcClient.sendRawTransaction(srTx.hex());
	System.out.println("Sent signedRawTx (txID): " + sentRawTransactionID);

    }
}
