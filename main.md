
# Headers
## Basic libraries
```c main.c
#include <stdio.h>  /*Standard input-output header*/
#include <stdlib.h> /*Libreria Standard*/  
#include <time.h>   /*Acquisizione e manipolazione del tempo*/
#include <stdbool.h>/*Aggiunge i boolean var*/

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

## Show Users
Questo metodo non solo mostra lo stato di tutti gli
utenti, anche ritorna una variabile boolean per identificare
se ancora ci sono utenti disponibili.
```c main.c
bool userStatus(){

    int i=0;
    int activeUsers=0;
    int inactiveUsers=0;
    int sommaBudget=0;
    bool Active;

    printf("\n");
    printf("_____________________________");
    printf("| User_ID | Budget | Status |");

    for(i=0; i<configurazione.SO_USERS_NUM; i++){

        sommaBudget+=budgetlist[i];

        Active = retrylist[i]<configurazione.SO_RETRY;
  	    if(Active)
         activeUsers++;
        else
            inactiveUsers++;

        printf("| %d | %d |%s |",i ,budgetlist[i] ,Active?"True":"False");
    }

    printf("_____________________________");
    printf("|Active | Inactive | Total Budget |");
    printf("| %d | %d | %d |", activeUsers, inactiveUsers, sommaBudget);
    printf("_____________________________");

    return activeUsers!=0;
}

```

## Show Nodes
```c main.c
bool nodeStatus(){

    int i=0;
    int activeNodes=0;
    int inactiveNodes=0;
    int sommaRewards=0;
    bool Active;
    
    printf("\n");
    printf("_____________________________");
    printf("| Node_ID | Reward | Status |");

    for(i=0; i<configurazione.SO_NODES_NUM; i++){
        
        sommaRewards+=rewardlist[i];

        Active = poolsizelist[i] < configurazione.SO_TP_SIZE;
  	    if(Active)
            activeNodes++;
        else
            inactiveNodes++;

        printf("| %d | %d |%s |",i ,rewardlist[i] ,Active?"True":"False");
    }

    printf("_____________________________");
    printf("|Active | Inactive | Total Rewards | ");
    printf("| %d | %d | %d |",activeNodes,inactiveNodes,sommaRewards);
    printf("_____________________________");

    return activeNodes!=0;
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

	    	/*conta la quantita di utenti attivi*/
	    	test = userStatus();
    
	    	/*mostra i nodi con i suoi semafori */
	    	nodeStatus();
	    	printf("\n\n");
    
	    	now = difftime(time(0), startSimulation);
            
            if(libroCounter > SO_REGISTRY_SIZE){
                printf("%f: libro mastro pieno\n",now);
                break;
            }

            if(!test){
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
