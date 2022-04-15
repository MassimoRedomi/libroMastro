# structures

 Le strutture sono gruppi di variabili che rapressentano un
oggetto della vita reale.


## Configurazione

questa struttura solo serve per avere un archivio di dati ordinati
dei dati letti del file di configurazione. questi dati sono:

| variables                            | descripcion                                                                                                      |
| ------------------------------------ | ---------------------------------------------------------------------------------------------------------------- |
| SO_USERS_NUM                         | numero di processi untente                                                                                       |
| SO_NODES_NUM                         | numero di processi nodo                                                                                          |
| SO_BUDGETNUM                         | budget iniziale di ciascun processo utente                                                                       |
| SO_REWARD                            | a percentuale di reward pagata da ogni utente per il processamento di una transazione                            |
| SO_MIN_TRANS_GEN_NSEC                | minimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente  |
| SO_MAX_TRANS_GEN_NSEC                | massimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente |
| SO_RETRY                             | numero massimo di fallimenti consecutivi nella generazione di transazioni dopo cui un processo utente termina    |
| SO_TPSIZE                            | numero massimo di transazioni nella transaction pool dei processi nodo                                           |
| SO_BLOCKSIZE                         | numero di transazioni contenute in un blocco                                                                     |
| SO_MIN_TRANS_PROC                    | minimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo                  |
| SO_MAX_TRANS_PROC                    | massimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo                 |
| SO_REGISTRY_SIZE                     | numero massimo di blocchi nel libro mastro.                                                                      |
| SO_SIM_SEC                           | durata della simulazione.                                                                                        |
| SO_NUM_FRIENDS(solo versione max 30) | numero di nodi amici dei processi nodo(solo per la versione full)                                                |
| SO_HOPS                              | numero massimo di inoltri di una transazione verso nodi amici prima che il master creai un nuovo nodo            |


Anche è vero che si poteva leggere tutte le variabili senza metterlo
in una sola struttura. ma mi sembra molto piu ordinato mettendo tutto 
cosi.

```c Structs.c

/*struttura della configurazione.*/
typedef struct Configurazione{
	int SO_USERS_NUM;          /*numero di processi utente*/
    int SO_NODES_NUM;          /*numero di processi nodo*/
    int SO_BUDGET_INIT;        /*budget iniziale di ciascun processo utente*/
    int SO_REWARD;             /*la percentuale di reward pagata da ogni utente per il processamento di una transazione*/
    int SO_MIN_TRANS_GEN_NSEC; /*minimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente*/
    int SO_MAX_TRANS_GEN_NSEC; /*massimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente*/
    int SO_RETRY;              /*numero massimo di fallimenti consecutivi nella generazione di transazioni dopo cui un processo utente termina*/
    int SO_TP_SIZE;            /*numero massimo di transazioni nella transaction pool dei processi nodo*/
    int SO_MIN_TRANS_PROC_NSEC;/*minimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo*/
    int SO_MAX_TRANS_PROC_NSEC;/*massimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo*/
    int SO_SIM_SEC;            /*durata della simulazione*/
    int SO_FRIENDS_NUM;        /*solo per la versione full. numero di nodi amici dei processi nodo (solo per la versione full)*/
    int SO_HOPS;               /*solo per la versione full. numero massimo di inoltri di una transazione verso nodi amici prima che il master creai un nuovo nodo*/ 
}Configurazione;
```

Questa struttura è gia dichiarata con la variabile <span class="underline">configurazione</span> 
perche solo c'è una lettura delle variabili di configurazione.


## Transazione

Una transazione è caratterizzata dalle seguenti informazioni:

| variabile | descrizione                                           |
| --------- | ----------------------------------------------------- |
| timestamp | Quando viene effetuata la transazione.                |
| sender    | Utente che ha generato la transazione.                |
| receiver  | Utente destinatario della somma.                      |
| quantita  | Quantita di denaro inviata.                           |
| reward    | denaro dal sender al nodo che processa la transazione |

La transazione è inviata dal processo utente che la genera ad uno 
dei processi nodo, scelto a caso.

```c Structs.c
/*struttura della configurazione.*/
typedef struct Transazione{
	double timestamp;/*Quando viene effettuata la transazione.*/
	int sender;      /*Utente che ha generato la transazione.*/
    int receiver;    /*Utente destinatario de la somma.*/
    int quantita;    /*Quantita di denaro inviata.*/
    int reward;      /*denaro dal sender al nodo che processa la transazione.*/
	
}Transazione;

```


## printTrans

```c Structs.c
void prinTrans(Transazione t){
	printf("%f: %d %d %d\n",t.timestamp,t.sender,t.receiver,t.quantita);
}

```

## RandomInt & RandomLong

Le due funzioni servono per lanciare un numero aleatorio tra min e 
max. In ogni caso si usano le stesse variabili:

- min: il numero minimo del rango.
- max: il numero massimo del rango.

__randomInt__ serve per simplificare ogni volta che si fa una scelta a 
caso dentro di ogni thread.
__randomlong__ per ora solo serve per il random sleep.

```c Struc.c

int randomInt(int min, int max){
	return rand() % max +min;
}
    
long randomlong(int min, int max){
	return (long)(rand() % max +min);
}

```

## randomSleep

funzione di nanosleep con un rango tra due numeri:
min

```c Struct.c
    /*si ferma per una quantita random di nano secondi*/
    void randomSleep(int min, int max){
        nanosleep((const struct timespec[]){{0,randomlong(min,max)}},NULL);
    }

```
