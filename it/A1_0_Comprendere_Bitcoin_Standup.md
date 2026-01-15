# Appendice 1: Comprendere Bitcoin Standup

Il [Capitolo 2.1: Configurare Bitcoin Core VPS con StackScript](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md) spiega il processo di creazione di un nodo Bitcoin utilizzando i [Bitcoin-Standup-Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). La seguente appendice spiega cosa fanno le principali sezioni dello script. Potresti voler seguire in [Linode Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) in un'altra finestra.

## Passo 1: Nome Host

Il nome del tuo host è memorizzato in `/etc/hostname` e impostato con il comando `hostname`. Appare anche in `/etc/hosts`.

## Passo 2: Fuso Orario

Il fuso orario del tuo host è memorizzato in `/etc/timezone`, poi un file appropriato da `/usr/share/zoneinfo/` viene copiato in `/etc/localtime`.

## Passo 3: Aggiornamento di Debian

Il gestore di pacchetti `apt-get` viene utilizzato per aggiornare la tua macchina e per installare `gnupg`, il generatore di numeri casuali `haveged` e il firewall non complicato `ufw`.

La tua macchina è configurata per rimanere automaticamente aggiornata con `echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean true" | debconf-set-selections`.

## Passo 4: Configurazione di un Utente

Viene creato un utente `standup`, che sarà utilizzato per le tue applicazioni Bitcoin. Ha anche permessi `sudo`, permettendoti di eseguire azioni privilegiate con questo account.

Se hai fornito una chiave SSH, ti permetterà di accedere a questo account (altrimenti, devi usare la password creata durante la configurazione).

Se hai fornito un indirizzo IP, l'accesso `ssh` sarà limitato a tale indirizzo, per `/etc/hosts.allow`.

## Passo 5: Configurazione di Tor

Tor è installato per fornire servizi protetti (nascosti) per accedere ai comandi RPC di Bitcoin attraverso il tuo server. Vedi il [Capitolo 14.1: Verificare la Configurazione Tor](14_1_Verificare_la_Configurazione_Tor.md) per ulteriori informazioni sulla tua Configurazione Tor.

Se hai fornito un client autorizzato per i servizi nascosti, l'accesso sarà limitato a quella chiave, per `/var/lib/tor/standup/authorized_clients`. Se non lo hai fatto, il [Capitolo 14.2](14_2_Cambiare_Bitcoin_Hidden_Services.md) spiega come farlo in un secondo momento.

## Passo 6: Installazione di Bitcoin

Bitcoin è installato in `~standup/.bitcoin`. La tua configurazione è memorizzata in `~standup/.bitcoin/bitcoin.conf`.

Assicurati che i checksum siano verificati per il [Capitolo 2.1: Configurare Bitcoin Core VPS con StackScript](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md), altrimenti potresti essere esposto a un attacco alla catena di approvvigionamento.

## Passo 7: Installazione di QR Encoder

Per mantenere tutto compatibile con [GordianSystem](https://github.com/BlockchainCommons/GordianSystem) viene creato un codice QR in `/qrcode.png`. Questo può essere letto da un client QuickConnect come [GordianWallet](https://github.com/BlockchainCommons/GordianWallet-iOS).

## Conclusione — Comprendere Bitcoin Standup

Bitcoin Standup utilizza script per cercare di eguagliare gran parte della funzionalità di un [GordianNode](https://github.com/BlockchainCommons/GordianNode-macOS). Dovrebbe fornirti un ambiente Bitcoin sicuro costruito su una base di Bitcoin Core e Tor per le comunicazioni RPC.

## Cosa c'è di prossimo?

Se eri nel processo di creazione di un nodo Bitcoin per l'uso in questo corso, dovresti tornare al[Capitolo 2.1: Configurare Bitcoin Core VPS con StackScript](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md).

Se stai leggendo le appendici, continua con [Appendice A2: Compilare Bitcoin dal Codice Fonte](A2_0_Compilare_Bitcoin_dal_Codice_Fonte.md).

