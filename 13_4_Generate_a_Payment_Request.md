
# 13.4: Generate a Payment request.

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

> :book: ***What is an invoice

Almost all payments made on the Lightning Network require an invoice, which is nothing more than a request for payment made by the recipient of the money and sent by different means to the user who will pay.  The format for a Lightning invoice uses bech32 encoding, which is already used for Bitcoin Segregated Witness and all payment requests are single use.

An invoice is made up of two parts,  one is human readable and other is 

- Human readable part: `ln` + (`lnbc` for Bitcoin mainnet, `lntb` for Bitcoin testnet, and `lnbcrt` for Bitcoin regtest) + data amount
      
- Data part : UNIX Timestamp + tagged parts include a payment hash, the pubkey of the payee node, an optional description of the payment, an expiration time, and some extra routing information.

For this example we've created a second lightning node using LND implementation.  We will refer to this instance using `lnd$ ` prompt shell to indicate place where we'll generate LND commands.   If you want to reproduce this steps you should use Bitcoin Standup to create a second machine and install LND.



