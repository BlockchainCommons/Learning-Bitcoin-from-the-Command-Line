 # 6.3: Enviando & Gastando una Multisig automatizada

La técnica estándar para crear direcciones de multifirma y para gastar sus fondos es compleja, pero es un ejercicio que vale la pena para comprender un poco más sobre cómo funcionan y cómo puedes manipularlas a un nivel relativamente bajo. Sin embargo, Bitcoin Core ha facilitado un poco las multisigs en los nuevos lanzamientos. 

> :warning: **AVISO DE VERSION:** El comando `addmultisigaddress` está disponible en Bitcoin Core v 0.10 o superior.

## Crear una Dirección Multisig en Tu Billetera

Para que los fondos enviados a direcciones multisig sean más fáciles de gastar, solo necesita hacer algunos preparativos usando el comando `addmultisigaddress`. Probablemente no sea lo que te gustaría hacer si estuvieras escribiendo programas de billetera multifirma, pero si solo estuvieras tratando de recibir algunos fondos a mano, podría ahorrarte algunos tirones de pelo.

### Recolectar las Claves
Empiezas creando direcciones P2PKH y recuperando claves públicas, como es habitual, para cada usuario que formará parte de la multisig:
```
machine1$ address3=$(bitcoin-cli getnewaddress)
machine1$ echo $address3
tb1q4ep2vmakpkkj6mflu94x5f94q662m0u5ad0t4w
machine1$ bitcoin-cli -named getaddressinfo address=$address3 | jq -r '.pubkey'
0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc

machine2$ address4=$(bitcoin-cli getnewaddress)
machine2$ echo $address4
tb1qa9v5h6zkhq8wh0etnv3ae9cdurkh085xufl3de
machine2$ bitcoin-cli -named getaddressinfo address=$address4 | jq -r '.pubkey'
02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f
```

### Crear la Dirección Multisig en Cualquier Lugar

A continuación, creas la multisig en _cada máquina que aporta firmas_ usando un nuevo comando, `addmultisigaddress`, en lugar de `createmultisig`. Este nuevo comando guarda parte de la información en tu billetera, lo que hace que sea mucho más fácil gastar el dinero después.
```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$address3'","02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f"]'''
{
  "address": "tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex",
  "redeemScript": "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae",
  "descriptor": "wsh(multi(2,[d6043800/0'/0'/15']0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc,[e9594be8]02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f))#wxn4tdju"
}

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc","'$address4'"]'''
{
  "address": "tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex",
  "redeemScript": "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae",
  "descriptor": "wsh(multi(2,[ae42a66f]0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc,[fe6f2292/0'/0'/2']02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f))#cc96c5n6"
}
```
Como se señaló en la sección anterior, actualmente no importa si usas direcciones o claves públicas, por lo que mostramos el otro mecanismo aquí, mezclando los dos. Obtendrás la misma dirección multifirma de cualquier manera. Sin embargo, _debes utilizar el mismo orden_. Por lo tanto, es mejor que los miembros de la multifirma se comprueben entre ellos para asegurarse de que todos obtuvieron el mismo resultado.

### Watch for Funds

Afterward, the members of the multisig will still need to run `importaddress` to watch for funds received on the multisig address:
```
machine1$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8 rescan="false"

machine2$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8 rescan="false"
```

## Respend with an Automated Transaction

Afterward, you will be able to receive funds on the multisignature address as normal. The use of `addmultisigaddress` is simply a bureaucratic issue on the part of the recipients: a bit of bookkeeping to make life easier for them when they want to spend their funds.

But, it makes life a lot easier. Because information was saved into the wallet, the signers will be able to respend the funds sent to the multisignature address exactly the same as any other address ... other than the need to sign on multiple machines.

You start by collecting your variables, but you no longer need to worry about `scriptPubKey` or `redeemScript`.

Here's a new transaction sent to our new multisig address:
```
machine1$ utxo_txid=b9f3c4756ef8159d6a66414a4317f865882ee04beb57a0f8349dafcc98f5acbc
machine1$ utxo_vout=0
machine1$ recipient=$(bitcoin-cli getrawchangeaddress)
```
You create a raw transaction:
```
machine1$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
```
Then you sign it:
```
machine1$ bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex
{
  "hex": "02000000000101bcacf598ccaf9d34f8a057eb4be02e8865f817434a41666a9d15f86e75c4f3b90000000000ffffffff0188130000000000001600144f93c831ec739166ea425984170f4dc6bac75829040047304402205f84d40ba16ff49e60a7fc9228ef5917473aae1ab667dad01e113ca0fef3008b02201a50da2c65f38798aea94bcbd5bbf065bc1e38de44bacee69d525dcddcc11bba01004752210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae00000000",
  "complete": false,
  "errors": [
    {
      "txid": "b9f3c4756ef8159d6a66414a4317f865882ee04beb57a0f8349dafcc98f5acbc",
      "vout": 0,
      "witness": [
        "",
        "304402205f84d40ba16ff49e60a7fc9228ef5917473aae1ab667dad01e113ca0fef3008b02201a50da2c65f38798aea94bcbd5bbf065bc1e38de44bacee69d525dcddcc11bba01",
        "",
        "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae"
      ],
      "scriptSig": "",
      "sequence": 4294967295,
      "error": "CHECK(MULTI)SIG failing with non-zero signature (possibly need more signatures)"
    }
  ]
}

```
Note that you no longer had to give `signrawtransactionwithkey` extra help, because all of that extra information was already in your wallet. Most importantly, you didn't make your private keys vulnerable by directly manipulating them. Instead the process was _exactly_ the same as respending a normal UTXO, except that the transaction wasn't fully signed at the end.

### Sign It On Other Machines

The final step is exporting the partially signed `hex` to any other machines and signing it again:
```
machine2$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000 | jq -r '.hex')
```
When everyone that's required has signed, you're off to the races:
```
machine2$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d
```
As with the shortcut discussed in [§4.5: Sending Coins with Automated Raw Transactions](04_5_Sending_Coins_with_Automated_Raw_Transactions.md), the result is a lot easier, but you lose some control in the process.

## Summary: Sending & Spending an Automated Multisig

There's an easier way to respend funds sent to multisig addresses that simply requires use of the `addmultisigaddress` command when you create your address. It doesn't demonstrate the intricacies of P2SH respending, and it doesn't give you expansive control, but if you just want to get your money, this is the way to go.

## What's Next?

Learn more about "Expanding Bitcoin Transactions" with [Chapter Seven: Expanding Bitcoin Transactions with PSBTs](07_0_Expanding_Bitcoin_Transactions_PSBTs.md).
