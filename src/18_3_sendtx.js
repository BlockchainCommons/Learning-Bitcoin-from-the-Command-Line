const RpcAgent = require('bcrpc');
agent = new RpcAgent({port: 18332, user: 'StandUp', pass: '6305f1b2dbb3bc5a16cd0
f4aac7e1eba'});

agent.getNewAddress('-addresstype', 'legacy', function (err, newAddress) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log("Recipient: " + newAddress.result);

  agent.sendToAddress(newAddress.result, 0.00001, function(err, txid) {
    if (err)
      throw Error(JSON.stringify(err));
    console.log("TXID: " + txid.result);

    agent.getTransaction(txid.result, function (err, transaction) {
      if (err)
        throw Error(JSON.stringify(err));
      console.log(transaction.result);
    });	
  });
    
});
