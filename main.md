
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
# Transazioni programmate(para quien me lea. traduzcanme por favor)
Las transacciones progrmadas son una lista de transacciones que vienen son leidos
desde un file externo que contiene una transaccion por linea; cada linea contiene
su timestamp, sender id, receiver id y cantidad. cuando sea la hora de cada 
transaccion, se hara una señal desde el main 

## Lettura del file di transazioni pianificati
questa funzione non ha bisogno di ritornare un array perche puo essere pasato
come parametro della funzione e si scrive direttamente nell'array. per questo
motivo il return della funzione ritornera un valore intero che rapressenta la
quantita di transazioni programmate.

```c main.c

/*legge le transazioni e gli scrive in un array di transazioni per scriverle 
dopo nel libro mastro.*/
int leggeLibroDiTransazioni(char fileName[], Transazione programmate[100]){
    int i = 0;
    FILE *file = fopen(fileName,"r");
    if(!file){
        printf("non si trova il libro di transazioni programmate.\n");
    }else{
        /*legge riga a riga fino alla fine(EOF), mettendo tutti le variabili nell'array 
        delle transazioni programmate.*/
        while(fscanf(file,"%lf %d %d %d",&programmate[i].timestamp,&programmate[i].sender,&programmate[i].receiver,&programmate[i].quantita) != EOF && i<100){
            programmate[i].reward = programmate[i].quantita * configurazione.SO_REWARD / 100;
            i++;
        }
    }
    return i;
}

```

## Segnale
La segnale è una maniera di forzare a un'utente a fare una transazione gia
creata dal master con valori predefiniti.
```c main.c

/*segnale che forza una transazione di un'utente.*/
void segnale(Transazione programmato){
    mailbox[nodoLibero(programmato.sender)] = programmato;/*assegno la transazione in un mailbox*/

    budgetlist[programmato.sender] -= programmato.quantita;
    printf("Segnale ->");
    prinTrans(programmato);
}
```

# Main


## PrintStatus Nodes and Users
Questo metodo non solo mostra lo stato di tutti gli
utenti e nodi, ritorna anche una variabile boolean per identificare
se ci sono ancora utenti disponibili.

```c main.c

bool printStatus(int all){

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
    int dim=0;

    /*Scegliamo dim della tabella*/
    if(all){
        dim=MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM);
    }else if(!all){
        dim=MIN(41, MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM));
    }
  
    /*Attributi*/
    printf("\n\n");
    printf("|| User_ID | Budget | Status |##| Node_ID | Rewards | Status ||\n");
    printf("||===========================|##|============================||\n");

    
    /*Stampa risultati*/
    for(i=0; i<dim; i++){
        ActiveU = retrylist[i]<configurazione.SO_RETRY;
        sommaBudget += budgetlist[i];
        if(ActiveU){
            activeUsers++;
        }else{
            inactiveUsers++;
        }

        printf("||%9d|%8d|%8s|#",i,budgetlist[i], ActiveU?"True  ":"False ");

        /*Se c'e un nodo da stampare*/
        if(i < configurazione.SO_NODES_NUM){
            sommaRewards+=rewardlist[i];
            ActiveN = poolsizelist[i] < configurazione.SO_TP_SIZE;
            if(ActiveN){
                activeNodes++;
            }else{
                inactiveNodes++;
            }
            printf("#|%9d|%9d|%8s||\n", i , rewardlist[i],ActiveN?"True  ":"False ");
        }else{
            /*se non deve mostrare piu' nodi, allora stampa a vuoto*/
            printf("#|         |         |        ||\n");
        }
    }
    if(!all){
        printf("---------------------------------------------------------------\n");
        printf("|| Active Users | Tot Budget |##| Active Nodes | Tot Rewards ||\n");
        printf("||%14d|%12d|##|%14d|%13d||\n",activeUsers,sommaBudget,activeNodes, sommaRewards);
        printf("\n");
    }else if(all){
        printf("---------------------------------------------------------------\n");
        printf("||   Active Users   |   Terminate Users   |    Tot Budget    ||\n");
        printf("||%18d|%21d|%18d||\n",activeUsers, inactiveUsers, sommaBudget);
        printf("||   Active Nodes   |   Terminate Nodes   |    Tot Rewards   ||\n");
        printf("||%18d|%21d|%18d||\n",activeNodes, inactiveNodes, sommaRewards);
        printf("\n");
    }

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

    /*variabili delle transazioni programmate*/
    int programmateCounter;
    bool *programmateChecklist;
    Transazione programmate[100];


    srand((unsigned) time(0)); /*aleatorio*/

    if(argc<2){
	    printf("si aspettava un file con la configurazione o il commando 'manual'.\n");
        exit(EXIT_FAILURE);
    }else if(argc>3){
		printf("troppi argomenti.\n");
		exit(EXIT_FAILURE);
    }else{
		/*in caso di voler inserire i valori a mano*/
		if( strcmp(argv[1],"mano")==0 || strcmp(argv[1],"manual")==0 ){
			writeConf();
		}else{
	    	 readconf(argv[1]);/*lettura del file*/
        }
    
        /*lettura di transazioni programmate*/
        if(argc >= 3){
            programmateCounter = leggeLibroDiTransazioni(argv[2], programmate);
            programmateChecklist = malloc(programmateCounter * sizeof(bool));
            for(i=0; i < programmateCounter; i++){
                programmateChecklist[i] = true;
            }
        }else{
            programmateCounter = 0;
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

            if(!printStatus(0)){
                printf("tutti gli utenti sono disattivati");
                break;
            }

            /* transazioni programmate mancanti*/
            for(i=0; i< programmateCounter; i++){
                if(programmate[i].timestamp <= now && programmateChecklist[i]){
                    segnale(programmate[i]);
                    programmateChecklist[i] = false;
                }
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
        /*Stampiamo i risultati finali*/
        clear();
        printStatus(1);

    
	}
	return 0;
}
```
