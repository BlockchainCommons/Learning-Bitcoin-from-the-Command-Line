const RpcAgent = require('bcrpc');
agent = new RpcAgent({port: 18332, user: 'StandUp', pass: '6305f1b2dbb3bc5a16cd0f4aac7e1eba'});

agent.getReceivedByAddress('tb1q04q2wzlhfqlrnz95ynfj7gp4t3yynrj0542smv', function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});

agent.getWalletInfo(function (err, walletInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(walletInfo.result);
});
