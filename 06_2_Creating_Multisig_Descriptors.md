# 6.2: Creating a Multisig from Descriptors


standup@lbtc:~$ address3=$(bitcoin-cli getnewaddress)
standup@lbtc:~$ pubkey3=$(bitcoin-cli -named getaddressinfo address=$address3 | jq -r '.pubkey')

bash-3.2$ address4=$(bitcoin-cli getnewaddress)
bash-3.2$ pubkey4=$(bitcoin-cli -named getaddressinfo address=$address4 | jq -r '.pubkey')
bash-3.2$ echo $pubkey4
026137216df604722b973bb43d8ebffd10761a22b3dd98892904eb1163de8206ee


standup@lbtc:~$ pubkey4=026137216df604722b973bb43d8ebffd10761a22b3dd98892904eb1163de8206ee
standup@lbtc:~$ msdesc="wsh(multi(2,$pubkey3,$pubkey4))"


$ msdescwithcs=$(bitcoin-cli getdescriptorinfo $msdesc | jq -r '.descriptor')
standup@lbtc:~$ echo $msdescwithcs
wsh(multi(2,021c04d13d7047bb8413ecf2f3a7f3c7172eaffff00040a67eb6b10cd57046b958,026137216df604722b973bb43d8ebffd10761a22b3dd98892904eb1163de8206ee))#nvydugpc

bitcoin-cli loadwallet watch_multi
 bitcoin-cli -rpcwallet=watch_multi importdescriptors '[{ "desc": "$msdesc", "timestamp":1770329126 }]'

## Summary: Creating a Multisig from Descriptors

...


## What's Next?

Continue "Expanding Bitcoin Transactions" with [§6.3: Spending a
Transaction to a
Multisig](06_3_Spending_a_Transaction_to_a_Multisig.md).
