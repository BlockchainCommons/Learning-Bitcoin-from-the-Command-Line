# Capitolo 14.3: Aggiunta di Servizi Nascosti SSH

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Fino ad oggi, hai usato Tor con i tuoi servizi Bitcoin, ma puoi anche usarlo per proteggere altri servizi sulla tua macchina, migliorandone la sicurezza e la privacy. Questa sezione dimostra come fare introducendo un servizio nascosto `ssh` per accedere in remoto utilizzando Tor.

## Creare Servizi Nascosti SSH

I nuovi servizi vengono creati aggiungendoli al file `/etc/tor/torrc`:
:
```
$ su 
# cat >> /etc/tor/torrc << EOF
HiddenServiceDir /var/lib/tor/hidden-service-ssh/
HiddenServicePort 22 127.0.0.1:22
EOF
# exit
```
Ecco cosa significa:

* HiddenServiceDir: Indica che hai una directory di servizi nascosti con la configurazione necessaria in questo percorso.
* HiddenServicePort: Indica la porta tor da utilizzare; nel caso di SSH, questa è solitamente 22.

Dopo aver aggiunto le righe appropriate al tuo file `torrc`, dovrai riavviare Tor:

```
$ sudo /etc/init.d/tor restart
```


Dopo il riavvio, la tua `HiddenServiceDir` dovrebbe avere nuovi file come segue:

```
$ sudo ls -l /var/lib/tor/hidden-service-ssh
total 16
drwx--S--- 2 debian-tor debian-tor 4096 Jul 22 14:55 authorized_clients
-rw------- 1 debian-tor debian-tor   63 Jul 22 14:56 hostname
-rw------- 1 debian-tor debian-tor   64 Jul 22 14:55 hs_ed25519_public_key
-rw------- 1 debian-tor debian-tor   96 Jul 22 14:55 hs_ed25519_secret_key
```
Il file `hostname` in questa directory contiene il tuo nuovo ID onion:
```
$ sudo cat /var/lib/tor/hidden-service-ssh/hostname
qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
```
Puoi connetterti al servizio nascosto `ssh` utilizzando `torify` e quell'indirizzo:
```
$ torify ssh standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
The authenticity of host 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion (127.42.42.0)' can't be established.
ECDSA key fingerprint is SHA256:LQiWMtM8qD4Nv7eYT1XwBPDq8fztQafEJ5nfpNdDtCU.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion' (ECDSA) to the list of known hosts.
standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion's password: 
```
## Sommario: Aggiunta di Servizi Nascosti SSH

Ora che hai installato Tor e sai come usarlo, puoi aggiungere altri servizi a Tor. Basta aggiungere righe al tuo `torrc` (sul tuo server), quindi connettersi con `torify` (sul tuo client).

> :fire: ***Qual è il potere di Altri Servizi Nascosti?*** Ogni volta che accedi a un servizio sul tuo server da remoto, lasci tracce sulla rete. Anche se i dati sono crittografati da qualcosa come SSH (o TLS), gli osservatori sulla rete possono vedere da dove ti connetti, dove ti connetti e quale servizio stai usando. Questo importa? Questa è la domanda che devi farti. Ma se la risposta è "Sì", puoi proteggere la connessione con un servizio nascosto.

## Cosa succede dopo?

Per un tipo diverso di privacy, passa a "Usare i2p" con [Capitolo 15: Usare i2p](15_0_Usare_i2p.md).
