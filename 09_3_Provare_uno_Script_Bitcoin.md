# 9.3: Provare uno Script Bitcoin

Bitcoin Scripting consente un notevole controllo aggiuntivo sulle transazioni Bitcoin, ma è anche alquanto pericoloso. Come descriveremo nel [Capitolo 10.1](10_1_Comprendere_la_Base_di_P2SH.md), gli script effettivi sono in qualche modo isolati dalla rete Bitcoin, il che significa che è possibile scrivere uno script e farlo accettare dalla rete anche se è impossibile riscattarlo quello script! Quindi, devi testare a fondo i tuoi script prima di investire i tuoi soldi in essi.

Questo capitolo descrive quindi un metodo fondamentale per testare gli script Bitcoin, che utilizzeremo anche per esempi occasionali nel resto di questa sezione.

## Installa btcdeb

Bitcoin Script Debugger (`btcdeb`) di @kallewoof è uno dei metodi più affidabili che abbiamo trovato per eseguire il debug degli script Bitcoin. Richiede, tuttavia, la configurazione di C++ e alcuni altri accessori sul computer, quindi offriremo anche alcune altre opzioni verso la fine di questo capitolo.

Per prima cosa devi clonare il repository GitHub `btcdeb`, che richiederà anche l'installazione di `git` se non lo hai ancora
.
```
$ sudo apt-get install git
$ git clone https://github.com/bitcoin-core/btcdeb.git
```
Tieni presente che quando esegui `git clone` copierà `btcdeb` nella tua directory corrente. Abbiamo scelto di farlo nella nostra directory '~standup'.
```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  btcdeb  laanwj-releases.asc  SHA256SUMS.asc
```
Successivamente è necessario installare il C++ richiesto e gli altri pacchetti.
```
$ sudo apt-get install autoconf libtool g++ pkg-config make
```
Dovresti anche installare `readline`, poiché ciò rende il debugger molto più semplice da usare supportando la cronologia utilizzando le frecce su/giù, il movimento da sinistra a destra, il completamento automatico utilizzando la scheda e altre buone interfacce utente.
```
$ sudo apt-get install libreadline-dev
```
Ora sei pronto per compilare e installare `btcdeb`:
```
$ cd btcdeb
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```
Dopo tutto ciò, dovresti avere una copia di `btcdeb`:
```
$ which btcdeb
/usr/local/bin/btcdeb
```

## Usa btcdeb

`btcdeb` funziona come un debugger standard. Richiede uno script (così come un numero qualsiasi di voci dello stack) come argomento di avvio. È quindi possibile 'correre' attraverso lo script.

Se invece lo avvii senza argomenti, otterrai semplicemente un interprete in cui puoi eseguire i comandi `exec [opcode]` per eseguire direttamente le azioni.

### Usa btcdeb per un esempio di addizione

L'esempio seguente mostra l'uso di `btcdeb` per l'esempio di addizione della sezione precedente, "1 2 OP_ADD"
```
$ btcdeb '[1 2 OP_ADD]' 
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 1 is interpreted as a numeric value; use OP_1 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
3 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
1       | 
2       | 
OP_ADD  | 
#0000 1
```
Mostra il nostro script iniziale, eseguito dall'alto verso il basso, e mostra anche cosa verrà eseguito successivamente nello script.

Digitiamo `step` e avanza di un passo prendendo il primo elemento nello script e inserendolo nello stack:

```
btcdeb> step
		<> PUSH stack 01
script  |  stack 
--------+--------
2       | 01
OP_ADD  | 
#0001 2
```
E ancora:
```
btcdeb> step
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
        | 01
#0002 OP_ADD
```
Ora eseguiamo `OP_ADD` e c'è grande eccitazione perché il codice operativo estrae i primi due elementi dallo stack, li somma insieme, quindi inserisce la loro somma nello stack.
```
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script  |  stack 
--------+--------
        | 03
```
Ed è qui che finisce il nostro script, senza più nulla da eseguire e uno `03` in cima al nostro stack come risultato dello script.

> **NOTA:** `btcdeb` ti consente di ripetere il comando precedente premendo invio. Lo faremo negli esempi successivi, quindi non sorprenderti se il prompt `btcdeb>` non contiene nulla come input. Si tratta semplicemente di ripetere il comando precedente (spesso `step`).

### Usa btcdeb per un esempio di sottrazione

La sezione precedente includeva anche un esempio di sottrazione leggermente più complesso di Scripting: `3 2 OP_ADD 4 OP_SUB`. Ecco come appare:

```
$ btcdeb '[3 2 OP_ADD 4 OP_SUB]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 3 is interpreted as a numeric value; use OP_3 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 4 is interpreted as a numeric value; use OP_4 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
5 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
3       | 
2       | 
OP_ADD  | 
4       | 
OP_SUB  | 
#0000 3
btcdeb> step
		<> PUSH stack 03
script  |  stack 
--------+--------
2       | 03
OP_ADD  | 
4       | 
OP_SUB  | 
#0001 2
btcdeb> 
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
4       | 03
OP_SUB  | 
#0002 OP_ADD
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script  |  stack 
--------+--------
4       | 05
OP_SUB  | 
#0003 4
btcdeb> 
		<> PUSH stack 04
script  |  stack 
--------+--------
OP_SUB  |      04
        | 05
#0004 OP_SUB
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script  |  stack 
--------+--------
        | 01
```
Torneremo a `btcdeb` di tanto in tanto e rimarrà uno strumento eccellente per testare i tuoi script.

### Usa la potenza di btcdeb

`btcdeb` ha anche alcune funzioni più potenti, come `print` e `stack`, che mostrano lo script e lo stack in qualsiasi momento.

Ad esempio, nello script sopra, una volta che sei passato al comando "OP_ADD", puoi vedere quanto segue:
```
btcdeb> print
    #0000 3
    #0001 2
 -> #0002 OP_ADD
    #0003 4
    #0004 OP_SUB
btcdeb> stack
<01>	02	(top)
<02>	03
```
L'uso di questi comandi può rendere più semplice vedere cosa sta succedendo e dove ti trovi.

> :warning: **AVVISO:** `btcdeb` è molto più complesso da usare se stai cercando di verificare le firme. Vedi [Controllo della firma con btcdeb](https://github.com/bitcoin-core/btcdeb/blob/master/doc/btcdeb.md#signature-checking). Questo è vero per qualsiasi test di script, quindi non lo consigliamo se stai cercando di verificare un `OP_CHECKSIG` o un `OP_CHECKMULTISIG`.

## Prova uno script online

Esistono anche alcuni simulatori web che puoi utilizzare per testare gli script online. Possono essere superiori a uno strumento da riga di comando offrendo un output più grafico, ma scopriamo anche che tendono ad avere dei difetti.

In passato abbiamo cercato di fornire linee guida dettagliate sull'utilizzo di siti come [Script Playground](http://www.crmarsh.com/script-playground/) o [Bitcoin Online Script Debugger](https://bitcoin-script-debugger.visvirial.com/), ma diventano obsoleti e/o scompaiono troppo rapidamente per poter stare al passo con loro.

Supponiamo che questi debugger abbiano il vantaggio di mostrare le cose visivamente e di dirti esplicitamente se uno script riesce (si sblocca) o fallisce (rimane bloccato). Supponiamo che abbiano degli svantaggi con le firme, dove molti di loro restituiscono sempre "true" per i test di firma oppure hanno meccanismi molto macchinosi per incorporarli.

## Prova uno script con Bitcoin

Anche con un ottimo strumento come `btcdeb` o risorse transitorie come i vari tester di script online, non lavori con la realtà. Non puoi garantire che seguano le regole di consenso di Bitcoin, il che significa che non puoi garantire i loro risultati. Ad esempio, Script Playground afferma esplicitamente di ignorare un bug implicito nelle multifirme Bitcoin. Ciò significa che qualsiasi codice multisig testato con successo su Script Playground si romperà nel mondo reale.

Quindi l'unico modo per testare _veramente_ gli script Bitcoin è provarli su Testnet.

E come lo fai? Si dà il caso che questo sia l'argomento del [Capitolo 10](110_0_Inserire_Scripts_di_Bitcoin_in_Transazioni_P2SH.md), che esamina l'introduzione di questi script astratti nel mondo reale di Bitcoin incorporandoli nelle transazioni P2SH. (Ma anche in questo caso, probabilmente avrai bisogno di un'API per inviare la tua transazione P2SH sulla rete Bitcoin, quindi i test completi saranno ancora possibili in futuro.)

_Quali_ siano gli altri metodi di test che hai utilizzato, testare uno script su Testnet dovrebbe essere il tuo test finale _prima_ di inserire lo script su Mainnet. Non fidarti che il tuo codice sia corretto; non limitarti a guardarlo. Non fidarti nemmeno dei simulatori o debugger che hai utilizzato. Farlo è un altro ottimo modo per perdere fondi su Bitcoin.

## Riepilogo: Provare uno script Bitcoin

Dovresti installare `btcdeb` come strumento da riga di comando per testare i tuoi script Bitcoin. Al momento della stesura di questo articolo, produce risultati accurati che possono passare attraverso l'intero processo di scripting. Puoi anche consultare alcuni siti online per una rappresentazione più visiva. Quando avrai finito, dovrai andare su testnet per assicurarti che le cose funzionino correttamente, prima di eseguire la distribuzione in modo più generale.

## Qual è il prossimo argomento?

Continua "Presentazione degli script Bitcoin" con il nostro primo esempio reale: [Capitolo 9.4:09_4_Programmare una P2PKH](09_4_Programmare_una_P2PKH.md).
