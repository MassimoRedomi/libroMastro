---
author: Bini, Radicioni, Schifanella
title: Descrizione del progetto
---

# 5 Descrizione del progetto: versione minima (voto max 24 su 30)

Si intende simulare un libro mastro contenente i dati di transazioni
monetarie fra diversi utenti. A tal fine sono presenti i seguenti
processi:

-   un processo master che gestisce la simulazione, la creazione degli
    altri processi, etc.
-   SO~USERSNUM~ processi utente che possono inviare denaro ad altri
    utenti attraverso una transazione
-   SO~NODESNUM~ processi nodo che elaborano, a pagamento, le
    transazioni ricevute.

## 5.1 Le transazioni

Una transazione è caratterizzata dalle seguenti informazioni: •
timestamp della transazione con risoluzione dei nanosecondi (si veda
funzione clock~gettime~(...)) • sender (implicito, in quanto è l'utente
che ha generato la transazione) • receiver, utente destinatario della
somma • quantità di denaro inviata. • reward, denaro pagato dal sender
al nodo che processa la transazione.

La transazione è inviata dal processo utente che la genera ad uno dei
processi nodo, scelto a caso.

## 5.2 Processi utente

I processi utente sono responsabili della creazione e invio delle
transazioni monetarie ai processi nodo. Ad ogni processo utente è
assegnato un buget iniziale SO~BUDGETINIT~. Durante il proprio ciclo di
vita, un processo utente svolge iterativamente le seguenti operazioni:

### 1. Calcola il bilancio corrente a partire dal budget iniziale e facendo la somma algebrica delle entrate e delle

uscite registrate nelle transazioni presenti nel libro mastro,
sottraendo gli importi delle transazioni spedite ma non ancora
registrate nel libro mastro.

• Se il bilancio è maggiore o uguale a 2, il processo estrae a caso: --
Un altro processo utente destinatario a cui inviare il denaro -- Un nodo
a cui inviare la transazione da processare -- Un valore intero compreso
tra 2 e il suo bilancio suddiviso in questo modo:

∗ il reward della transazione pari ad una percentuale SO~REWARD~ del
valore estratto, arrotondato, con un minimo di 1.

∗ l'importo della transazione sarà uguale al valore estratto sottratto
del reward

Esempio: l'utente ha un bilancio di 100. Estraendo casualmente un numero
fra 2 e 100, estrae 50. Se SO~REWARD~ è pari al 20 (ad indicare un
reward del 20%) allora con l'esecuzione della transazione l'utente
trasferirà 40 all'utente destinatario, e 10 al nodo che avrà processato
con successo la transazione.

• Se il bilancio è minore di 2, allora il processo non invia alcuna
transazione

### 2. Invia al nodo estratto la transazione e attende un intervallo di tempo (in nanosecondi) estratto casualmente tra SO~MINTRANSGENNSEC~ e massimo SO~MAXTRANSGENNSEC~.

Inoltre, un processo utente deve generare una transazione anche in
risposta ad un segnale ricevuto (la scelta del segnale è a discrezione
degli sviluppatori).

Se un processo non riesce ad inviare alcuna transazione per SO~RETRY~
volte consecutive, allora termina la sua esecuzione, notificando il
processo master.

## 5.3 Processi nodo

Ogni processo nodo memorizza privatamente la lista di transazioni
ricevute da processare, chiamata transaction pool, che può contenere al
massimo SO~TPSIZE~ transazioni, con SO~TPSIZE~ \> SO~BLOCKSIZE~. Se la
transaction pool del nodo è piena, allora ogni ulteriore transazione
viene scartata e quindi non eseguita. In questo caso, il sender della
transazione scartata deve esserne informato.

Le transazioni sono processate da un nodo in blocchi. Ogni blocco
contiene esattamente SO~BLOCKSIZE~ transazioni da processare di cui
SO~BLOCKSIZE~−1 transazioni ricevute da utenti e una transazione di
pagamento per il processing (si veda sotto).

Il ciclo di vita di un nodo può essere cosı̀ definito: • Creazione di un
blocco candidato -- Estrazione dalla transaction pool di un insieme di
SO~BLOCKSIZE~−1 transazioni non ancora presenti nel libro mastro. --
Alle transazioni presenti nel blocco, il nodo aggiunge una transazione
di reward, con le seguenti caratteristiche:

-   timestamp: il valore attuale di clock~gettime~(...)

```{=html}
<!-- -->
```
-   sender : -1 (definire una MACRO...)
-   receiver : l'dentificatore del nodo corrente
-   quantità: la somma di tutti i reward delle transazioni incluse nel
    blocco
-   reward : 0

• Simula l'elaborazione di un blocco attraverso una attesa non attiva di
un intervallo temporale casuale espresso in nanosecondi compreso tra
SO~MINTRANSPROCNSEC~ e SO~MAXTRANSPROCNSEC~.

• Una volta completata l'elaborazione del blocco, scrive il nuovo blocco
appena elaborato nel libro mastro, ed elimina le transazioni eseguite
con successo dal transaction pool.

## 5.4 Libro mastro

Il libro mastro è la struttura condivisa da tutti i nodi e gli utenti,
ed è deputata alla memorizzazione delle transazioni eseguite. Una
transazione si dice confermata solamente quando entra a far parte del
libro mastro. Più in dettaglio, il libro mastro è formato da una
sequenza di lunghezza massima SO~REGISTRYSIZE~ di blocchi consecutivi.
All'interno di ogni blocco sono contenute esattamente SO~BLOCKSIZE~
transazioni. Ogni blocco è identificato da un identificatore intero
progressivo il cui valore iniziale è impostato a 0. Una transazione è
univocamente identificata dalla tripletta (timestamp, sender, receiver).
Il nodo che aggiunge un nuovo blocco al libro mastro è responsabile
anche dell'aggiornamento dell'identificatore del blocco stesso.

## 5.5 Stampa

Ogni secondo il processo master stampa:

• numero di processi utente a nodo attivi • il budget corrente di ogni
processo utente e di ogni processo nodo, cosı̀ come registrato nel libro
mastro (inclusi i processi utente terminati). Se il numero di processi è
troppo grande per essere visualizzato, allora viene stampato soltanto lo
stato dei processi più significativi: quelli con maggior e minor budget.

## 5.6 Terminazione della simulazione

La simulazione terminerà in uno dei seguenti casi:

• sono trascorsi SO~SIMSEC~ secondi • la capacità del libro mastro si
esaurisce (il libro mastro può contenere al massimo SO~REGISTRYSIZE~
blocchi) • tutti i processi utente sono terminati.

Alla terminazione, il processo master obbliga tutti i processi nodo e
utente a terminare, e stamperà un riepilogo della simulazione,
contenente almeno queste informazioni:

• ragione della terminazione della simulazione • bilancio di ogni
processo utente, compresi quelli che sono terminati prematuramente •
bilancio di ogni processo nodo • numero dei processi utente terminati
prematuramente • numero di blocchi nel libro mastro • per ogni processo
nodo, numero di transazioni ancora presenti nella transaction pool

# 6 Descrizione del progetto: versione "normal" (max 30)

All'atto della creazione da parte del processo master, ogni nodo riceve
un elenco di SO~NUMFRIENDS~ nodi amici. Il ciclo di vita di un processo
nodo si arricchisce quindi di un ulteriore step:

• periodicamente ogni nodo seleziona una transazione dalla transaction
pool che è non ancora presente nel libro mastro e la invia ad un nodo
amico scelto a caso (la transazione viene eliminata dalla transaction
pool del nodo sorgente)

Quando un nodo riceve una transazione, ma ha la transaction pool piena,
allora esso provvederà a spedire tale transazione ad uno dei suoi amici
scelto a caso. Se la transazione non trova una collocazione entro
SO~HOPS~ l'ultimo nodo che la riceve invierà la transazione al processo
master che si occuperà di creare un nuovo processo nodo che contiene la
transazione scartata come primo elemento della transaction pool.
Inoltre, il processo master assegna al nuovo processo nodo
SO~NUMFRIENDS~ processi nodo amici scelti a caso. Inoltre, il processo
master sceglierà a caso altri SO~NUMFRIENDS~ processi nodo già
esistenti, ordinandogli di aggiungere alla lista dei loro amici il
processo nodo appena creato.

# 7 Configurazione

I seguenti parametri sono letti a tempo di esecuzione, da file, da
variabili di ambiente, o da stdin (a discrezione degli studenti):

  variable                                description
  --------------------------------------- ------------------------------------------------------------------------------------------------------------------
  SO~USERSNUM~                            numero di processi utente
  SO~NODESNUM~                            numero di processi nodo
  SO~BUDGETINIT~                          budget iniziale di ciascun processo utente
  SO~REWARD~                              la percentuale di reward pagata da ogni utente per il processamento di una transazione
  SO~MINTRANSGENNSEC~                     minimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente
  SO~MAXTRANSGENNSEC~                     massimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente
  SO~RETRY~                               numero massimo di fallimenti consecutivi nella generazione di transazioni dopo cui un processo utente termina
  SO~TPSIZE~                              numero massimo di transazioni nella transaction pool dei processi nodo
  SO~MINTRANSPROCNSEC~,                   minimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo
  SO~MAXTRANSPROCNSEC~                    massimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo
  SO~REGISTRYSIZE~                        numero massimo di blocchi nel libro mastro.
  SO~SIMSESC~                             durata della simulazione
  SO~NUMFRIENDS~ (solo versione max 30)   numero di nodi amici dei processi nodo (solo per la versione full)
  SO~HOPS~ (solo versione max 30)         numero massimo di inoltri di una transazione verso nodi amici prima che il master creai un nuovo nodo

Un cambiamento dei precedenti parametri non deve determinare una nuova
compilazione dei sorgenti. Invece, i seguenti parametri sono letti a
tempo di compilazione:

  variable           description
  ------------------ ----------------------------------------------
  SO~REGISTRYSIZE~   numero massimo di blocchi nel libro mastro
  SO~BLOCKSIZE~      numero di transazioni contenute in un blocco

La seguente tabella elenca valori per alcune configurazioni di esempio
da testare. Si tenga presente che il progetto deve poter funzionare
anche con altri parametri.

  parametro                      letto a...     conf #1     conf #2     conf #3
  ------------------------------ -------------- ----------- ----------- -----------
  SO~USERSNUM~                   run time       100         1000        20
  SO~NODESNUM~                   run time       10          10          10
  SO~BUDGETINIT~                 run time       1000        1000        10000
  SO~REWARD~ \[0-100\]           run time       1           20          1
  SO~MINTRANSGENNSEC~\[nsec\]    run time       100000000   100000000   100000000
  SO~MAXTRANSGENNSEC~\[nsec\]    run time       200000000   100000000   200000000
  SO~RETRY~                      run time       20          2           10
  SO~TPSIZE~                     run time       1000        20          100
  SO~BLOCKSIZE~                  compile time   100         10          10
  SO~MINTRANSPROCNSEC~\[nsec\]   run time       10000000    10000000    
  SO~MAXTRANSPROCNSEC~{nsec\]    run time       20000000    10000000    
  SO~REGISTRYSIZE~               compile time   1000        10000       1000
  SO~SIMSEC~\[sec\]              run time       10          20          20
  SO~FRIENDSNUM~                 run time       3           5           3
  SO~HOPS~                       run time       10          2           10

# 8 Requisiti implementativi

Il progetto (sia in versione "minimal" che "normal") deve • essere
realizzato sfruttando le tecniche di divisione in moduli del codice, •
essere compilato mediante l'utilizzo dell'utility make • massimizzare il
grado di concorrenza fra processi • deallocare le risorse IPC che sono
state allocate dai processi al termine del gioco • essere compilato con
almeno le seguenti opzioni di compilazione: [gcc -std=c89
-pedantic]{.underline}

• poter eseguire correttamente su una macchina (virtuale o fisica) che
presenta parallelismo (due o più processori).

Per i motivi introdotti a lezione, ricordarsi di definire la macro
~GNUSOURCE~ o compilare il progetto con il flag -D~GNUSOURCE~.
