# Capitolo 14.2: Cambiare_Bitcoin_Hidden_Services

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Hai un servizio Tor funzionante, ma nel tempo potresti volerlo resettare o modificarlo in altro modo.

## Mettere in Sicurezza i tuoi Servizi Nascosti

Tor ti consente di limitare quali client possono comunicare con i tuoi servizi nascosti. Se non hai già autorizzato il tuo client durante la configurazione del server, alla prima opportunità dovresti fare quanto segue:

1. Richiedi la tua Chiave Pubblica di Autenticazione Tor V3 dal tuo client. (In [GordianWallet](https://github.com/BlockchainCommons/GordianWallet-iOS), è disponibile nel menu delle impostazioni.)
2. Vai alla sottodirectory appropriata per il servizio nascosto di Bitcoin, che se hai utilizzato Bitcoin Standup è `/var/lib/tor/standup/`.
3. Vai alla sottodirectory `authorized_clients`.
4. Aggiungi un file chiamato `[qualsiasi_nome].auth`. `[qualsiasi_nome]` può essere davvero qualsiasi cosa.
5. Inserisci la chiave pubblica (e nient'altro) nel file.

Una volta aggiunto un file `.auth` alla sottodirectory `authorized_client`, solo i client autorizzati potranno comunicare con quel servizio nascosto. Puoi aggiungere circa 330 diverse chiavi pubbliche per abilitare client diversi.

## Resettare l'Indirizzo Onion di `bitcoind`

Se mai desideri resettare il tuo indirizzo onion per `bitcoind`, rimuovi semplicemente la `onion_private_key` nella tua directory dei dati, come `~/.bitcoin/testnet`:

```
$ cd ~/.bitcoin/testnet
$ rm onion_private_key 
```
Quando riavvii, verrà generato un nuovo indirizzo onion:
```
2020-07-22T23:52:27Z tor: Got service ID pyrtqyiqbwb3rhe7, advertising service pyrtqyiqbwb3rhe7.onion:18333
2020-07-22T23:52:27Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_private_key
```

## Resettare l'Indirizzo Onion dell'RPC

Se desideri resettare il tuo indirizzo onion per l'accesso RPC, cancella allo stesso modo la `HiddenServiceDirectory` appropriata e riavvia Tor:

```
$ sudo rm -rf /var/lib/tor/standup/
$ sudo /etc/init.d/tor restart
```


> :warning: **ATTENZIONE:** Resettare il tuo indirizzo onion dell'RPC disconnetterà eventuali portafogli mobili o altri servizi che hai connesso utilizzando l'API Quicklink. Fai questo con estrema cautela.

## Forzare `bitcoind` a Usare Tor

Infine, puoi forzare `bitcoind` a usare onion aggiungendo quanto segue al tuo `bitcoin.conf`:

```
proxy=127.0.0.1:9050
listen=1
bind=127.0.0.1
onlynet=onion
```
Dovrai quindi aggiungere nodi seed basati su onion o altri nodi alla tua configurazione, ancora una volta modificando il `bitcoin.conf`:
```
seednode=address.onion
seednode=address.onion
seednode=address.onion
seednode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
```
Consulta [Bitcoin Onion Nodes](https://github.com/emmanuelrosa/bitcoin-onion-nodes) per un elenco e un esempio su come aggiungerli.

Successivamente, riavvia `tor` e `bitcoind`.

Ora dovresti comunicare esclusivamente su Tor. Ma, a meno che tu non ti trovi in uno stato ostile, questo livello di anonimato probabilmente non è necessario. Inoltre, non è particolarmente raccomandato: potresti ridurre notevolmente il numero dei tuoi peer potenziali, invitando problemi di censura o addirittura di correlazione. Potresti anche sperimentare ritardi. E, questa configurazione potrebbe darti un falso senso di anonimato che in realtà non esiste sulla rete Bitcoin.

> :warning: **ATTENZIONE:** Questa configurazione non è testata! Usala a tuo rischio e pericolo!

## Riepilogo: Cambiare Bitcoin Hidden Services

Probabilmente non avrai bisogno di modificare i tuoi servizi Onion una volta verificati, ma in caso di necessità, ecco come resettare un indirizzo Tor che è stato compromesso o come passare all'uso esclusivo di Tor per il tuo `bitcoind`.

## Cosa Viene Dopo?

Continua a "Comprendere Tor" col [Capitolo 14.3: Aggiungere SSH Hidden Services](14_3_Aggiungere_SSH_Hidden_Services.md).
