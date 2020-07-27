# 13.6: Close channel

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

In this chapter you'll be able to close a channel and learn what it means and how to do it using `lightning-cli close` command-line interface.   The close RPC command attempts to close the channel cooperatively with the peer, or unilaterally after unilateraltimeout argument expires measured in seconds and the output will be sent to the address controlled by your c-lightning wallet if you don't specify one.

### Channel close

Close a channel means you and your counterparty will send their agreed-upon channel balance to the blockchain whereby you must pay transaction fees and must wait for the transaction to be mined.


