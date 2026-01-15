# 15.1: Bitcoin Core come servizio I2P (Invisible Internet Project)

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Piuttosto che utilizzare il servizio basato su proxy di Tor per garantire la privacy delle tue comunicazioni Bitcoin, potresti invece desiderare di usare I2P, che è progettato per agire come una rete privata all'interno di internet, piuttosto che offrire semplicemente un accesso privato ai servizi internet.

## Comprendere le Differenze

Tor e I2P offrono entrambi accesso privato ai servizi online, ma con routing e database diversi e con architetture differenti per i relay. Poiché i servizi nascosti (come l'accesso a Bitcoin) sono fondamentali nel design di I2P, questi sono stati meglio ottimizzati:

|  | Tor | I2P |
| :---         |     :---:      |          ---: |
| Routing   | [Onion](https://www.onion-router.net/)     | [Garlic](https://geti2p.net/en/docs/how/garlic-routing)    |
| Network Database     | Trusted [Directory Servers](https://blog.torproject.org/possible-upcoming-attempts-disable-tor-network)       | [Distributed network database](https://geti2p.net/en/docs/how/network-database)      |
| Relay     | **Two-way** encrypted connections between each Relay       | **One-way** connections between every server in its tunnels      |
| Hidden services     | Slow       | Fast      |

Un confronto più dettagliato può essere trovato su [geti2p.net](https://geti2p.net/en/comparison/tor).

### Comprendere i Compromessi per Limitare le Connessioni in Uscita

Ci sono [compromessi](https://bitcoin.stackexchange.com/questions/107060/tor-and-i2p-tradeoffs-in-bitcoin-core) se si sceglie di supportare solo I2P, solo Tor, o entrambi. Queste configurazioni, che limitano le connessioni in uscita sulla rete clearnet, sono fatte in Bitcoin Core utilizzando l'argomento `onlynet` nel tuo `bitcoin.conf`.

* `onlynet=onion`, che limita le connessioni in uscita a Tor, può esporre un nodo ad attacchi Sybil e può creare partizionamento della rete, a causa delle connessioni limitate tra Tornet e la clearnet.
* `onlynet=onion` e `onlynet=i2p` in congiunzione, che esegue il servizio Onion con il servizio I2P, è sperimentale per ora.

## Installare I2P

Per installare I2P, dovresti assicurarti che le tue porte siano correttamente configurate e poi puoi continuare con il processo di configurazione.

### Preparare le Porte

Per usare I2P, dovrai aprire le seguenti porte, che sono richieste da I2P:

1. **Outbound (Internet facing):** viene selezionata una porta casuale tra 9000 e 31000. È meglio se tutte queste porte sono aperte per le connessioni in uscita, cosa che non influisce sulla tua sicurezza.
- Puoi controllare lo stato del firewall usando `sudo ufw status verbose`, che non dovrebbe negare le connessioni in uscita per impostazione predefinita.
2. Inbound (Internet facing): opzionale. Una varietà di porte in entrata sono elencate nei [documenti di I2P](https://geti2p.net/en/faq#ports).
- Per la massima privacy, è preferibile disabilitare l'accettazione delle connessioni in entrata.

### Eseguire I2P

Il seguente comando eseguirà i servizi I2P di Bitcoin Core:

1. Installa `i2pd` su Ubuntu:



   ```
   sudo add-apt-repository ppa:purplei2p/i2pd
   sudo apt-get update
   sudo apt-get install i2pd
   ```

 
Per l'installazione su altri sistemi operativi, vedi [questi documenti](https://i2pd.readthedocs.io/en/latest/user-guide/install/).

2. [Esegui](https://i2pd.readthedocs.io/en/latest/user-guide/run/) il servizio I2P:


   ```
   $ sudo systemctl start i2pd.service
   ```


3. Controlla che I2P stia funzionando. Dovresti vederlo sulla porta 7656:


   ```
   $ ss -nlt

   State   Recv-Q   Send-Q     Local Address:Port      Peer Address:Port  Process

   LISTEN  0        4096           127.0.0.1:7656           0.0.0.0:*
   ```


4. Aggiungi le seguenti righe in `bitcoin.conf`: 


   ```
   i2psam=127.0.0.1:7656
   debug=i2p
   ```
L'opzione di log, `debug=i2p`, viene utilizzata per registrare informazioni aggiuntive nel log di debug sulla tua configurazione e connessioni I2P. La posizione predefinita di questo file di debug su Linux è: `~/.bitcoin/debug.log`.

5. Riavvia `bitcoind`



   ```
   $ bitcoind
   ```


6. Controlla `debug.log` per vedere se I2P è stato configurato correttamente, o se sono comparsi errori nei log.  
   ```
   2021-06-15T20:36:16Z i2paccept thread start
   2021-06-15T20:36:16Z I2P: Creating SAM session with 127.0.0.1:7656

   2021-06-15T20:36:56Z I2P: SAM session created: session id=3e0f35228b, my address=bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333
   2021-06-15T20:36:56Z AddLocal(bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333,4)
   ```
   
   
L'indirizzo I2P è menzionato nei log, terminando con _b32.i2p_. Per esempio `bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333`.

7. Conferma che `i2p_private_key` è stato creato nella directory dei dati di Bitcoin Core. La prima volta che Bitcoin Core si connette al router I2P, il suo indirizzo I2P (e la corrispondente chiave privata) sarà automaticamente generato e salvato in un file chiamato *i2p_private_key*:

   ```
   ~/.bitcoin/testnet3$ ls

   anchors.dat  chainstate         i2p_private_key  settings.json
   banlist.dat  debug.log          mempool.dat      wallets
   blocks       fee_estimates.dat  peers.dat
   ```


8. Controlla che `bitcoin-cli -netinfo` o `bitcoin-cli getnetworkinfo` restituisca l'indirizzo I2P:


   ```
   Local addresses
   bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p     port  18333    score      4
   ```
  

Ora il tuo server Bitcoin è accessibile attraverso la rete I2P al tuo nuovo indirizzo locale.

## Sommario: Bitcoin Core come servizio I2P (Invisible Internet Project)

È sempre bene avere alternative per la privacy e non dipendere esclusivamente da Tor per eseguire Bitcoin Core come servizio nascosto. Poiché I2P è stato recentemente aggiunto in Bitcoin Core, non molte persone lo utilizzano. Sperimenta con esso e segnala bug se trovi problemi.

> :information_source: **NOTA:** Per l'implementazione ufficiale di i2prouter in Java, visita la [pagina di download di I2P](https://geti2p.net/en/download) e segui le istruzioni per il tuo sistema operativo. Una volta installato, apri una finestra di terminale e digita `i2prouter start`. Poi visita `127.0.0.1:7657` nel tuo browser per abilitare SAM. Per farlo, seleziona: "Configura Homepage", poi "Clienti", e infine seleziona il "Pulsante Play" accanto a SAM application Bridge. Sul lato sinistro della pagina, dovrebbe esserci una luce verde accanto a "Clienti Condivisi".

Passa a "Programmare con RPC" con [Capitolo 16: Parlare a Bitcoind con C](16_0_Parlare_a_Bitcoind_con_C.md).

Oppure, se non sei un programmatore, puoi saltare a [Capitolo 19: Comprendere la  Configurazione Lightning](19_0_Comprendere_la_Configurazione_Lightning.md) per continuare la tua formazione sulla linea di comando con il Lightning Network.
