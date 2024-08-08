# 6.3: 06_3_Inviare_e_Ricevere_una_Multifirma_Automatizzata.md

La tecnica standard per creare indirizzi multifirma e per spendere i propri fondi è complessa, ma vale la pena esercitarsi per capire un po' di più su come funzionano e come è possibile manipolarli a un livello relativamente basso. Tuttavia, Bitcoin Core ha reso il multisig un po’ più semplice nelle nuove versioni.

> :warning: **AVVISO VERSIONE:** Il comando `addmultisigaddress` è disponibile in Bitcoin Core v 0.10 o successiva.

## Crea un indirizzo multisig nel tuo portafoglio

Per rendere i fondi inviati agli indirizzi multisig più facili da spendere, devi solo fare qualche preparazione utilizzando il comando `addmultisigaddress`. Probabilmente non è quello che vorresti fare se stessi scrivendo programmi per portafogli multisig, ma se stessi solo cercando di ricevere fondi a mano, potrebbe risparmiarti qualche disaggio.

### Raccogli le chiavi

Inizi creando indirizzi P2PKH e recuperando le chiavi pubbliche, come al solito, per ciascun utente che farà parte del multisig:
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

### Crea l'indirizzo Multisig ovunque

Successivamente crei il multisig su _ogni macchina che contribuisce con le firme_ utilizzando un nuovo comando, `addmultisigaddress`, invece di `createmultisig`. Questo nuovo comando salva alcune informazioni nel tuo portafoglio, rendendo molto più semplice spendere i soldi in seguito.
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
Come notato nella sezione precedente, attualmente non importa se si utilizzano indirizzi o chiavi pubbliche, quindi qui abbiamo mostrato l'altro meccanismo, mescolando i due. In ogni caso otterrai lo stesso indirizzo multisig. Tuttavia, _devi utilizzare lo stesso ordine_. Pertanto, è meglio che i membri del multisig controllino tra loro per assicurarsi che tutti abbiano ottenuto lo stesso risultato.

### Attenzione ai fondi

Successivamente, i membri del multisig dovranno comunque eseguire "importaddress" per controllare i fondi ricevuti sull'indirizzo multisig:
```
machine1$ bitcoin-cli -named importaddress address=tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex rescan="false"

machine2$ bitcoin-cli -named importaddress address=tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex rescan="false"
```

## Rispendi con una transazione automatizzata

Successivamente, sarai in grado di ricevere fondi sull'indirizzo multifirma normalmente. L'utilizzo di `addmultisigaddress` è semplicemente una questione burocratica da parte dei destinatari: un po' di contabilità per facilitargli la vita quando vogliono spendere i loro fondi.

Ma rende la vita molto più semplice. Poiché le informazioni sono state salvate nel portafoglio, i firmatari potranno spendere nuovamente i fondi inviati all'indirizzo multifirma esattamente come qualsiasi altro indirizzo... a parte la necessità di firmare su più macchine.

Inizi raccogliendo le tue variabili, ma non devi più preoccuparti di `scriptPubKey` o `redeemScript`.

Ecco una nuova transazione inviata al nostro nuovo indirizzo multisig:
```
machine1$ utxo_txid=b9f3c4756ef8159d6a66414a4317f865882ee04beb57a0f8349dafcc98f5acbc
machine1$ utxo_vout=0
machine1$ recipient=$(bitcoin-cli getrawchangeaddress)
```
Crea una transazione grezza:
```
machine1$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
```
Poi firmala:
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
Tieni presente che non dovevi più fornire ulteriore aiuto a `signrawtransactionwithkey`, perché tutte quelle informazioni extra erano già nel tuo portafoglio. Ancora più importante, non hai reso vulnerabili le tue chiavi private manipolandole direttamente. Invece il processo era _esattamente_ identico alla ripetizione di un normale UTXO, tranne per il fatto che alla fine la transazione non era completamente firmata.

### Firmalo su altre macchine

Il passaggio finale è esportare l'`hex` parzialmente firmato su qualsiasi altra macchina e firmarlo nuovamente:
```
machine2$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000 | jq -r '.hex')
```
Quando tutti quelli richiesti hanno firmato, sei pronto:
```
machine2$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d
```
Come per la scorciatoia discussa in [Capitolo 4.5: Inviare Monete con Transazione Grezza Automatizzatamd](04_5_Inviare_Monete_con_Transazione_Grezza_Automatizzata.md), il risultato è molto più semplice, ma si perde un po' di controllo del processo.

## Sommario: Invio e spesa di un Multisig automatizzato

Esiste un modo più semplice per spendere nuovamente i fondi inviati a indirizzi multisig che richiede semplicemente l'uso del comando `addmultisigaddress` quando crei il tuo indirizzo. Non dimostra le complessità della ripetizione della spesa P2SH e non ti dà un controllo espansivo, ma se vuoi solo ottenere i tuoi soldi, questa è la strada da percorrere.

## Qual è il prossimo argomento?

Scopri di più sulla "Espansione delle transazioni Bitcoin" nel [Capitolo 7: Ampliare le Transazioni Bitcoin con PSBTs](07_0_Ampliare_le_Transazioni_Bitcoin_con_PSBTs.md).
