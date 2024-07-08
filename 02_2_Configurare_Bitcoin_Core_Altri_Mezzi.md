# 2.2: Impostazione di una macchina Bitcoin-Core tramite altri mezzi

La sezione precedente, [Capitolo 2.1: Configurare Bitcoin-Core VPS con StackScript](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md), presupponeva che avresti creato un nodo completo su un VPS utilizzando uno Stackscript Linode. Tuttavia, puoi effettivamente installare Bitcoin-Core tramite qualsiasi altra metodologia di tua scelta e continuare a seguire i passaggi successivi di questo tutorial.

Di seguito sono riportate alcune altre metodologie di configurazione di cui siamo a conoscenza:

* *[Compilare Bitcoin dal Codice Fonte](A2_0_Compilare_Bitcoin_dal_Codice_Fonte.md).* Se preferisci compilare Bitcoin Core a mano, è trattato nell'Appendice 2.
* *[Usare GordianServer-macOS](https://github.com/BlockchainCommons/GordianServer-macOS).* Se hai un Mac moderno, puoi usare l'app *GordianNode* di Blockchain Commons, basata su *BitcoinStandup*, per installare un nodo completo sul tuo Mac.
* *[Usando altri script Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* Blockchain Commons offre anche una versione dello script Linode che hai usato che può essere eseguito dalla riga di comando su qualsiasi macchina Debian o Ubuntu. Questo tende a essere lo script all'avanguardia, il che significa che è più probabile che presenti nuove funzioni, come l'installazione Lightning.
* *[Impostazione di un nodo Bitcoin su AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally ha scritto un tutorial passo dopo passo per l'impostazione di Bitcoin-Core con Amazon Web Services (AWS).
* *[Impostazione di un nodo Bitcoin su un Raspberry Pi 3](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee spiega come impostare un nodo completo headless su un Raspberry Pi 3.

Assicurati di installare su una versione corrente del tuo sistema operativo, per evitare problemi in futuro. Al momento in cui scrivo, questo corso è stato testato su Debian 11.

## Cosa c'è dopo?

A meno che tu non voglia tornare a una delle altre metodologie per creare un nodo Bitcoin-Core, dovresti:

* Passare a "bitcoin-cli" con [Capitolo 3: Comprendere le Configurazioni di Bitcoin](03_0_Comprendere_le_Configurazioni_di_Bitcoin.md).
