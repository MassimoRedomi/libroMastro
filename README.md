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

extern Configurazione configurazione;
#define clear() printf("\033[H\033[J") /*clear the screen*/
#define MAX(x,y) ((x>y)?x:y) /*max between to parameters*/

```

Questa struttura è gia dichiarata con la variabile <span class="underline">configurazione</span> 
perche solo c'è una lettura delle variabili di configurazione.
#  Lettura Configurazione

## Legge File
```c Structs.c
/*Un metodo che fa un fgets(con gli stessi parametri e lo 
ritorna come un valore intero)*/
int readAndInt(char *str, int n, FILE *stream){
	fgets(str,n,stream);
    return atoi(str);
}
/*Funzione che cerca la maniera di leggere il config file.
metodo basato in codice di stackoverflow per leggere file come
una unica struttura.*/
void readconf(char fileName[]){
	/*secondo lo std c89 tutte le variabile devono 
    essere dichiarate prima del primo codice */
    FILE *file= fopen(fileName, "r");
    
    if(!file){
	      printf("non si trova il config file.\n");
          exit(EXIT_FAILURE);
    }else{
      char line[20];/*str per prendere le righe*/
    
    /*Inserisco le variabili riga per riga alla struttura.*/
    configurazione.SO_USERS_NUM = readAndInt(line,20,file);
    printf("SO_USERS_NUM: %d\n",configurazione.SO_USERS_NUM);
    configurazione.SO_NODES_NUM = readAndInt(line,20,file);
    printf("SO_NODES_NUM: %d\n",configurazione.SO_NODES_NUM);
    configurazione.SO_BUDGET_INIT = readAndInt(line,20,file);
    printf("SO_BUDGET_INIT: %d\n",configurazione.SO_BUDGET_INIT);
    configurazione.SO_REWARD = readAndInt(line,20,file);
    printf("SO_REWARD: %d\n",configurazione.SO_REWARD);
    configurazione.SO_MIN_TRANS_GEN_NSEC = readAndInt(line,20,file);
    printf("SO_MIN_TRANS_GEN_NSEC: %d\n",configurazione.SO_MIN_TRANS_GEN_NSEC);
    configurazione.SO_MAX_TRANS_GEN_NSEC = readAndInt(line,20,file);
    printf("SO_MAX_TRANS_GEN_NSEC: %d\n",configurazione.SO_MAX_TRANS_GEN_NSEC);
    configurazione.SO_RETRY = readAndInt(line,20,file);
    printf("SO_RETRY: %d\n",configurazione.SO_RETRY);
    configurazione.SO_TP_SIZE = readAndInt(line,20,file);
    printf("SO_TP_SIZE: %d\n",configurazione.SO_TP_SIZE);
    configurazione.SO_MIN_TRANS_PROC_NSEC = readAndInt(line,20,file);
    printf("SO_MIN_TRANS_PROC_NSEC: %d\n",configurazione.SO_MIN_TRANS_PROC_NSEC);
    configurazione.SO_MAX_TRANS_PROC_NSEC = readAndInt(line,20,file);
    printf("SO_MAX_TRANS_PROC_NSEC: %d\n",configurazione.SO_MAX_TRANS_PROC_NSEC);
    configurazione.SO_SIM_SEC = readAndInt(line,20,file);
    printf("SO_SIM_SEC: %d\n",configurazione.SO_SIM_SEC);
    configurazione.SO_FRIENDS_NUM = readAndInt(line,20,file);
    printf("SO_FRIENDS_NUM: %d\n",configurazione.SO_FRIENDS_NUM);
    configurazione.SO_HOPS = readAndInt(line,20,file);
    printf("SO_HOPS: %d\n",configurazione.SO_HOPS);
    }
    fclose(file);/*chiusura del file.*/
}

```

## Scrittura Manuale

Forse per la parte di prove. possiamo cambiare la intro delle variabili.
probabilmente cancelliamo questo alla fine del progetto.
l'idea e' poter inserire le variabili a mano

```c Structs.c
/*scritura manuale dei valori del sistema.*/
void writeConf(){
	printf("inserendo il parametro 'mano' o 'manual' si attiva il inserimento manuale dei valori\n\n");
    printf("SO_USERS_NUM: ");
    scanf("%d",&configurazione.SO_USERS_NUM);
    printf("SO_NODES_NUM: ");
    scanf("%d",&configurazione.SO_NODES_NUM);
    printf("SO_BUDGET_INIT: ");
    scanf("%d",&configurazione.SO_BUDGET_INIT);
    printf("SO_REWARD: ");
    scanf("%d",&configurazione.SO_REWARD);
    printf("SO_MIN_TRANS_GEN_NSEC: ");
    scanf("%d",&configurazione.SO_MIN_TRANS_GEN_NSEC);
    printf("SO_MAX_TRANS_GEN_NSEC: ");
    scanf("%d",&configurazione.SO_MAX_TRANS_GEN_NSEC);
    printf("SO_RETRY: ");
    scanf("%d",&configurazione.SO_RETRY);
    printf("SO_TP_SIZE: ");
    scanf("%d",&configurazione.SO_TP_SIZE);
    printf("SO_MIN_TRANS_PROC_NSEC: ");
    scanf("%d",&configurazione.SO_MIN_TRANS_PROC_NSEC);
    printf("SO_MAX_TRANS_PROC_NSEC: ");
    scanf("%d",&configurazione.SO_MAX_TRANS_PROC_NSEC);
    printf("SO_SIM_SEC: ");
    scanf("%d",&configurazione.SO_SIM_SEC);
    printf("SO_FRIENDS_NUM: ");
    scanf("%d",&configurazione.SO_FRIENDS_NUM);
    printf("SO_HOPS: ");
    scanf("%d",&configurazione.SO_HOPS);
    clear();
    
}

```


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
	printf("%f: %d -> %d: %d\n",t.timestamp,t.sender,t.receiver,t.quantita);
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

```c Structs.c

int randomInt(int min, int max){
	return (rand() % max) +min;
}
    
long randomlong(int min, int max){
	return (long)(rand() % max +min);
}

```

## randomSleep

funzione di nanosleep con un rango tra due numeri:
min

```c Structs.c
/*si ferma per una quantita random di nano secondi*/
void randomSleep(int min, int max){
    nanosleep((const struct timespec[]){{0,randomlong(min,max)}},NULL);
}

```
# Node

## Importa Variabili Globali
Importa funzioni e strutture di [Structs](Structs.md)
```c Node.c
#include "Structs.c"
```

### Controllo del LIBRO_MASTRO

Import del libroMastro e tutte le variabili:
- __libroluck__:   Semaforo per accedere alla scrittura del libroMastro.
- __libroCounter__: Contatore che indica la quantità di blocchi scritti nel libroMastro.

```c Node.c
extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck;/*luchetto per accedere solo un nodo alla volta*/

```


### Sincronizzazione tra Processi
```c Node.c
/*variabili condivise tra diversi thread.*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */
extern int *poolsizelist;  /*un registro del dimensioni occupate pool transaction*/
extern Configurazione configurazione;
extern time_t startSimulation;
extern pthread_t *nodi_id;       /*lista dei processi nodi*/

```


## Trova thread id
Questo metodo cerca la posizione del nodo in base alla posizione del 
thread nella lista nodi_id
```c Node.c
/*Trova thread id in nodi_id*/
int trovaNid(){
    int i;
    for(i=0;i<configurazione.SO_NODES_NUM;i++){
        if(nodi_id[i] == pthread_self()){
            return i;
        }
    }
}

```

## transazione di riasunto
Questo metodo genera l'ultima transazione del blocco.
questa transazione fa un riasunto di tutto quello che ha guadagnato il nodo in 
questo blocco. 
```c Node.c
/*funzione dell'ultima transazione del blocco.*/
Transazione riasunto(int id, int somma){
    Transazione transaction;
    transaction.sender    = -1;
    transaction.receiver  = id; /*id del nodo*/
    transaction.quantita  = somma; /*la somma di tutto il reward generato*/
    transaction.timestamp = difftime(time(0),startSimulation);/*quanto tempo ha passato dal inizio della simulazione.*/
    return transaction;
}
```


## Funzione Principale
```c Node.c
void* nodo(void *conf){
	/*creazioni dei dati del nodo*/
    int id = trovaNid();
    int i;
    int counterBlock=0;/*contatore della quantita di transazioni nel blocco*/
    int sommaBlocco=0; /*somma delle transazioni del blocco atuale*/
    Transazione blocco[SO_BLOCK_SIZE];
    Transazione pool[1000];/*stabilisce 1000 come la grandezza massima del pool, cmq si ferma in configurazione.SO_TP_SIZE*/
    Transazione finalReward;
    int mythr; 
    int semvalue;/*valore del semaforo*/
    sem_init(&semafori[id],configurazione.SO_USERS_NUM,1);/*inizia il semaforo in 1*/
	rewardlist[id]=0;/*set il reward di questo nodo in 0*/
    poolsizelist[id]=0;/*set full space available*/
    mythr = pthread_self();
    /*printf("Nodo #%d creato nel thread %d\n",id,mythr);*/
    
    /*inizio del funzionamento*/
    while(poolsizelist[id] < configurazione.SO_TP_SIZE){
    
		/*aggiorno il valore del semaforo*/
        sem_getvalue(&semafori[id],&semvalue);
        if(semvalue <= 0){
            /*printf("hay algo en el mailbox #%d\n",id);*/
			/*scrivo la nuova transazione nel blocco e nella pool*/
	    	 pool[poolsizelist[id]]=mailbox[id];
	    	 blocco[counterBlock]=mailbox[id];
    
	    	 /*somma il reward*/
	    	 sommaBlocco    += blocco[counterBlock].reward;
	    	 rewardlist[id] += blocco[counterBlock].reward;/*si mette al registro publico totale*/
    
	    	 /*incremento i contatori di posizione di pool e block*/
	    	 counterBlock++;
	    	 poolsizelist[id]++;

	    	 if(counterBlock == SO_BLOCK_SIZE - 1){
	    	    /*si aggiunge una nuova transazione come chiusura del blocco*/
	    	    blocco[counterBlock]=riasunto(id, sommaBlocco);/*aggiunge la transazione al blocco.*/
    
	    	    sem_wait(&libroluck);
	    	    for(i=0;i< SO_BLOCK_SIZE;i++){
	    	       libroMastro[(libroCounter * SO_BLOCK_SIZE) + i] = blocco[i];
	    	    }
	    	    /*si spostano i contatori*/
	    	    libroCounter++;
	    	    sem_post(&libroluck);
	    	    counterBlock=0;
	    	    sommaBlocco=0;
				randomSleep(configurazione.SO_MIN_TRANS_PROC_NSEC,configurazione.SO_MAX_TRANS_PROC_NSEC);
	    	    /*free(&mailbox[id]);*/
    
	    	      
	    	}
            if(poolsizelist[id] < configurazione.SO_TP_SIZE){
                sem_post(&semafori[id]);/*stabilisco il semaforo come di nuovo disponibile*/
	        }
    
		}
    
	}
}
```
# Utente
## import
Si importa il codice di Node.c che importa tutte le
funzioni di [Node](Node.md) e [Structs](Structs.md).

```c User.c
#include "Node.c"

```

### Importa Variabili Globali
#### Controllo del Libro_Mastro
Importazione del libroMastro e tutte le variabili:
- __libroluck__:   Semaforo per accedere alla scrittura del libroMastro.
- __libroCounter__:Contatore che indica la quantità di blocchi scritti nel libroMastro.

```c User.c
extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck;/*luchetto per accedere solo un nodo alla volta*/


```

#### Sincronizzazione tra Processi

Importa tutte le variabili del Main 

```c User.c
/*variabili condivise tra diversi thread.*/
extern int *retrylist ;     /*thread id di ogni utente*/
extern int *budgetlist;     /*un registro del budget di ogni utente*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern int *poolsizelist;  /*un registro del dimensioni occupate pool transaction*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */
extern Configurazione configurazione;
extern time_t startSimulation;
extern pthread_t *utenti_id;      /*lista id dei processi utenti*/

```

## Aggiornamento Libro_Mastro

L'aggiornamento tramite Libro_Mastro avviene tramie una sola funzione. 

```c User.c
/*aggiornamento del budget in base al libro.*/
int userUpdate(int id, int lastUpdate){
	int i;
    while(lastUpdate < libroCounter){
		for(i=lastUpdate*SO_BLOCK_SIZE; i < (lastUpdate+1)*SO_BLOCK_SIZE; i++){
			if(libroMastro[i].receiver == id && libroMastro[i].sender != -1){
	    	    budgetlist[id] += libroMastro[i].quantita - libroMastro[i].reward;
	    	 }
        }
        lastUpdate++;
	}
    return lastUpdate;
}

```
## Trova thread id
Questo metodo cerca la posizione dell'utente in base alla posizione del 
thread nella lista utenti_id(User id)
```c User.c
/*Trova thread id in utenti_id*/
int trovaId(){
    int id;
    for(id=0;id<configurazione.SO_USERS_NUM;id++){
        if(utenti_id[id] == pthread_self()){
            return id;
        }
    }
}

```

## trova nodo
serve per trovare un nodo libero per fare la transazione.
```c User.c
/*cerca un nodo libero per fare la trasazione.*/
int nodoLibero(int id){
    int nodo;
    do{
        nodo = randomInt(0,configurazione.SO_NODES_NUM);
        if( retrylist[id] > configurazione.SO_RETRY){
            printf("L'utenete %d non ha trovato nessun nodo libero",id);
            pthread_exit(NULL);
        }
        retrylist[id]++;
    }while(sem_trywait(&semafori[nodo])<0);
    
    if( retrylist[id] <= configurazione.SO_RETRY ){
        retrylist[id] = 0;
    }

    return nodo;
}

```

## Generatore di Transazione
comprime tutto il processo di generare la transazione.
```c User.c
Transazione generateTransaction(int id){
    int altroUtente;
	Transazione transaccion;
    transaccion.sender   = id;
    transaccion.quantita = randomInt(2,budgetlist[id]/2);/*set quantita a caso*/
	transaccion.reward   = transaccion.quantita * configurazione.SO_REWARD/100;/*percentuale de la quantita*/
    
	/*se il reward non arriva a 1, allora diventa 1*/
    if(transaccion.reward < 1){
		transaccion.reward = 1;
    }
    
    /*ricerca del riceiver*/
    /*debo reparar lo de los intentos*/
    do{
		altroUtente= randomInt(0,configurazione.SO_USERS_NUM);
	}while(altroUtente==id || retrylist[altroUtente] > configurazione.SO_RETRY);
	transaccion.receiver = altroUtente;
	/*calcola il timestamp in base al tempo di simulazione.*/
	transaccion.timestamp = difftime(time(0),startSimulation);
    retrylist[id] = 0;

	return transaccion;
}

```

## Processo Utente Principale

```c User.c
/*PROCESSO UTENTE:*/
void* utente(void *conf){
	int id = trovaId();                       /*Id processo utente*/
    int i;
    pthread_t mythr = pthread_self();          /*Pid thread processo utente*/
    int lastUpdate = 0;                        /*questo controlla l'ultima versione del libro mastro*/

	/*setting default values delle variabili condivise*/
    retrylist[id] = 0; /*stabilisco in 0 il numero di tentativi*/
	budgetlist[id] = configurazione.SO_BUDGET_INIT;

	/*printf("Utente #%d creato nel thread %d\n",id,mythr);*/
    

	while(retrylist[id]<configurazione.SO_RETRY){
    
		lastUpdate = userUpdate(id,lastUpdate);  /*Aggiorniamo Budgetdel Processo Utente*/
    
		if(budgetlist[id]>=2){                   /*Condizione Budget >= 2*/                                
    
			Transazione transaction;              /*Creiamo una nuova transazione*/
			transaction = generateTransaction(id);/*Chiamiamo la func generateTransaction*/
    
			/*scelglie un nodo libero a caso*/
            mailbox[nodoLibero(id)] = transaction;
            budgetlist[id] -= transaction.quantita;
        }else{
			retrylist[id]++;
		}
    
		randomSleep( configurazione.SO_MIN_TRANS_GEN_NSEC , configurazione.SO_MAX_TRANS_GEN_NSEC);
    
		if(retrylist[id] >= configurazione.SO_RETRY){/*Se raggiunge il n° max di tentativi*/
			printf("utente %d fermato\n",id);       /*ferma il procceso*/
		}
    }
}
```

# Headers
## Basic libraries
```c main.c
#include <stdio.h>  /*Standard input-output header*/
#include <stdlib.h> /*Libreria Standard*/  
#include <time.h>   /*Acquisizione e manipolazione del tempo*/
#include <stdbool.h>/*Aggiunge i boolean var*/
#include <string.h>/*Standar library for string type*/

```

## Specific Libraries
```c main.c
#include <unistd.h>      /*Header per sleep()*/
#include <pthread.h>     /*Creazione/Modifica thread*/
#include <semaphore.h>   /*Aggiunge i semafori*/

```

## Funzioni Utente
importando le funzioni di [User.c](User.md) sono incluse anche le funzioni di [Nodo](Node.md) e [Structs](Structs.md).
```c main.c
#include "User.c"
```

# Controllo LIBRO MASTRO

## Creazione del Libro_Mastro e Variabili:
- __libroluck__: Semaforo per accedere alla scrittura del libroMastro.
- __libroCounter__: Contatore della quantità di blocchi scritti nel libroMastro.

```c main.c
Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
int libroCounter=0;/*Counter controlla la quantitta di blocchi*/
sem_t libroluck;/*Luchetto per accedere solo a un nodo alla volta*/

```


# Funzioni in parallelo
questo spazio è riservato alle funzioni del multithread

## Memoria condivisa (work in progress)

i semafori vengono usati per gestire il flusso del programma
e ad evitare che i processi accedano contemporaneamente ai dati. 
Un Semaforo ha 3 stati:

###  0 avanti
Il processo puo accedere direttamente al dato.


### <0 aspetta
Il processo aspetta per accedere al dato
o in alternativa sceglie unn'altra via per l'accesso.

### external resources

1.  General Semaphore Example:
    <https://www.delftstack.com/howto/c/semaphore-example-in-c/>
2.  trywait:
    <https://stackoverflow.com/questions/27294954/how-to-use-sem-trywait>


### Lista Semafori e altri Dati Condivisi tra i threads:

```c main.c
/*variabili condivise tra diversi thread.*/
int *retrylist;      /*numero di tentativi di ogni utente*/
int *budgetlist;     /*un registro del budget di ogni utente*/
int *rewardlist;     /*un registro pubblico del reward totale di ogni nodo.*/
int *poolsizelist;   /*un registro del dimensioni occupate pool transaction*/
sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
Transazione *mailbox;/*struttura per condividere */
time_t startSimulation;
pthread_t *utenti_id;     /*lista id di processi utenti*/
pthread_t *nodi_id;     /*lista id di processi nodi  */
Configurazione configurazione;
```


# Main


## PrintStatus Nodes and Users
Questo metodo non solo mostra lo stato di tutti gli
utenti e nodi, ritorna anche una variabile boolean per identificare
se ci sono ancora utenti disponibili.

```c main.c

bool printStatus(){

    /*User var*/
    int activeUsers=0;
    int inactiveUsers=0;
    int sommaBudget=0;
    bool ActiveU;

    /*Node var*/
    int activeNodes=0;
    int inactiveNodes=0;
    int sommaRewards=0;
    bool ActiveN;

    /*Share var*/
    int i=0;

    /*Attributi*/
    printf("\n\n");
    printf("|| User_ID | Budget | Status |##| Node_ID | Rewards | Status ||\n");

    /*Stampa risultati*/
    for(i=0; i<MAX(configurazione.SO_NODES_NUM,configurazione.SO_USERS_NUM); i++){
        if(i<configurazione.SO_USERS_NUM && i<configurazione.SO_NODES_NUM){
            sommaRewards+=rewardlist[i];
            sommaBudget+=budgetlist[i];

            ActiveU = retrylist[i]<configurazione.SO_RETRY;
            if(ActiveU)
              activeUsers++;
            else
              inactiveUsers++;

            ActiveN = poolsizelist[i] < configurazione.SO_TP_SIZE;
            if(ActiveN)
              activeNodes++;
            else
              inactiveNodes++;

            printf("||%9d|%8d|%8s|##|%9d|%9d|%8s||\n", i , budgetlist[i] , ActiveU?"True  ":"False " , i ,rewardlist[i] , ActiveN?"True  ":"False ");
        }else if(i<configurazione.SO_USERS_NUM && i>=configurazione.SO_NODES_NUM){

            sommaBudget+=budgetlist[i];

            ActiveU = retrylist[i]<configurazione.SO_RETRY;
            if(ActiveU)
              activeUsers++;
            else
              inactiveUsers++;

            printf("||%9d|%8d|%8s|##|         |         |        ||\n", i , budgetlist[i] , ActiveU?"True  ":"False ");
        }else if(i>=configurazione.SO_USERS_NUM && i<configurazione.SO_NODES_NUM){

            sommaRewards+=rewardlist[i];

            ActiveN = poolsizelist[i] < configurazione.SO_TP_SIZE;
            if(ActiveN)
              activeNodes++;
            else
              inactiveNodes++;

            printf("||         |        |        |##|%9d|%9d|%8s||\n", i ,rewardlist[i] , ActiveN?"True  ":"False ");
        }
    }
    printf("\n\n");
    return activeUsers!=0;
}

```

## Main Function

```c main.c
int main(int argc,char *argv[]){
    int i;
	float now;
    bool test;
    int counterAttivi;

    srand((unsigned) time(0)); /*aleatorio*/

    if(argc<2){
	    printf("si aspettava un file con la configurazione o il commando 'manual'.\n");
        exit(EXIT_FAILURE);
    }else if(argc>2){
		printf("troppi argomenti.\n");
		exit(EXIT_FAILURE);
    }else{
		/*in caso di voler inserire i valori a mano*/
		if( strcmp(argv[1],"mano")==0 || strcmp(argv[1],"manual")==0 ){
			writeConf();
		}else{
	    	 readconf(argv[1]);/*lettura del file*/
        }
    
        /*now that we have all the variables we can start the process
        master*/
    
        startSimulation = time(0);/* el tiempo de ahora*/
        sem_init(&libroluck,0,1);/*inizia il semaforo del libromastro*/
    
        /*generatore dei nodi*/
        poolsizelist=malloc(configurazione.SO_TP_SIZE * sizeof(int));
        rewardlist=malloc(configurazione.SO_NODES_NUM * sizeof(int));
        semafori=malloc(configurazione.SO_NODES_NUM * sizeof(sem_t));
        mailbox=malloc(configurazione.SO_NODES_NUM * ((4 * sizeof(int)) + sizeof(double)));
        nodi_id = malloc(configurazione.SO_NODES_NUM * sizeof(pthread_t));
        for(i=0;i<configurazione.SO_NODES_NUM;i++){
			pthread_create(&nodi_id[i],NULL,nodo,NULL);
        }

        /*generatore dei utenti*/
        retrylist =malloc(configurazione.SO_USERS_NUM * sizeof(int));
        budgetlist=malloc(configurazione.SO_USERS_NUM * sizeof(int));
        utenti_id = malloc(configurazione.SO_USERS_NUM * sizeof(pthread_t));
        for(i=0;i<configurazione.SO_USERS_NUM;i++){
			pthread_create(&utenti_id[i],NULL,utente,NULL);
        }
    
		/*now start the master process*/
		now = difftime(time(0), startSimulation);

		while(now < configurazione.SO_SIM_SEC){
			sleep(1);
			clear();
    
			/*show last update*/
	    	printf("ultimo aggiornamento: %.2f/%d\n",difftime(time(0),startSimulation),configurazione.SO_SIM_SEC);

	    	now = difftime(time(0), startSimulation);
            
            if(libroCounter > SO_REGISTRY_SIZE){
                printf("%f: libro mastro pieno\n",now);
                break;
            }

            if(!printStatus()){
                printf("tutti gli utenti sono disattivati");
                break;
            }

        }
    
        /*kill all the threads*/
        for(i=0; i<configurazione.SO_NODES_NUM ; i++){
			pthread_cancel(nodi_id[i]);
		}
        for(i=0; i<configurazione.SO_USERS_NUM; i++){
            pthread_cancel(utenti_id[i]);
        }
    
		/*printf("numero di blocchi: %d\n\n",libroCounter);*/
		/*solo por confirmar al final*/
		for(i=0;i<libroCounter*SO_BLOCK_SIZE;i++){
			/*prinTrans(libroMastro[i]); per ora non mostro tutte transazioni*/
        }
    
	}
	return 0;
}
```
# Compilazione

La compilazione avviene tramite MAKEFILE:
### configurazione 1:  
SO_BLOCK_SIZE = 100
SO_REGISTRY_SIZE = 1000
```makefile Makefile
1:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=100 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
```
con debug
```makefile Makefile
g1:
	gcc -std=c89 -g -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=100 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
```


### configurazione 2:
SO_BLOCK_SIZE = 10
SO_REGISTRY_SIZE = 10000
```makefile Makefile
2:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=10000 main.c -lm -o main
	
```

### configurazione 3:
SO_BLOCK_SIZE = 10
SO_REGISTRY_SIZE = 1000
```makefile Makefile
3:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
```

### configurazione custom:
Questa configurazione viene pensato per modificare i valori 
```makefile Makefile
#setting con entrata libera per block size e registry size
custom:
	@echo -n "SO_BLOCK_SIZE: "
	@read block
	@echo -n "SO_REGISTRY_SIZE: "
	@read registry
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE = $(block) -DSO_REGISTRY_SIZE = $(registry) main.c -lm -o main
	
```


## significato di ogni flag
## std=c89
set the language standard C89.

## pthread
Setting the binary for thread processing.

## pedantic

## D_GNU_SOURCE
enables GNU extensions to the C and OS standards supported by the 
GNU C library.

## SO_BLOCK_SIZE
The size of the block in the simulation.


## SO_REGISTRY_SIZE
The max size of the libro mastro.

### -lm
Compiles against the shared library.
