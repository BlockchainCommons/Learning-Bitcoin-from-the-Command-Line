package com.blockchaincommons.lbtc;

import wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.MiningInfo;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient.AddressInfo;

public class App
{

    public static void main( String[] args ) throws Exception
    {

        BitcoindRpcClient rpcClient = new BitcoinJSONRPCClient("http://StandUp:6305f1b2dbb3bc5a16cd0f4aac7e1eba@localhost:18332");

	MiningInfo info = rpcClient.getMiningInfo();
	System.out.println("Mining Information");
	System.out.println("------------------");
	System.out.println("Chain......: " + info.chain());
	System.out.println("Blocks.....: " + info.blocks());
	System.out.println("Difficulty.: " + info.difficulty());
	System.out.println("Hash Power.: " + info.networkHashps());

	/* Replace this with an address from your system */
	String addr1 = "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo";
	
	AddressInfo addr1Info = rpcClient.getAddressInfo(addr1);
	System.out.println("Address: " + addr1Info.address());
	System.out.println("MasterFingerPrint: " + addr1Info.hdMasterFingerprint());
	System.out.println("HdKeyPath: " + addr1Info.hdKeyPath());
	System.out.println("PubKey: " + addr1Info.pubKey());

	System.out.println("Balance: " + rpcClient.getBalance());

    }
}
