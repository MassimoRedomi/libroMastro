# 4.Main
## Headers
### Basic libraries
```c main.c
#include <stdio.h>  /*Standard input-output header*/
#include <stdlib.h> /*Libreria Standard*/  
#include <time.h>   /*Acquisizione e manipolazione del tempo*/
#include <stdbool.h>/*Aggiunge i boolean var*/
#include <string.h>/*Standar library for string type*/

```

### Specific Libraries
```c main.c
#include <unistd.h>      /*Header per sleep()*/
#include <pthread.h>     /*Creazione/Modifica thread*/
#include <semaphore.h>   /*Aggiunge i semafori*/

```

### Funzioni Utente
importando le funzioni di [User.c](#6.Utente) sono incluse anche le funzioni di [Nodo](#5.Nodo) e [Structs](#3.Strutture).
```c main.c
#include "User.c"
#include "print.c"
```

## Controllo LIBRO MASTRO

### Creazione del Libro_Mastro e Variabili:
- __libroluck__: Semaforo per accedere alla scrittura del libroMastro.
- __libroCounter__: Contatore della quantità di blocchi scritti nel libroMastro.

```c main.c
Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
int libroCounter=0;/*Counter controlla la quantitta di blocchi*/
sem_t libroluck;/*Luchetto per accedere solo a un nodo alla volta*/
bool gestoreOccupato;

```



## Memoria condivisa 

In base a un grupo di variabili condivise si stabilisce un sistema di comunicazione tra i diversi processi. Questi dati condivisi servono ad altri processi in qualche momento, o sono dati servono al main per stampare lo stato dei processi.


### Lista Dati Condivisi tra i threads:

```c main.c
/*variabili condivise tra diversi thread.*/
int *budgetlist;     /*un registro del budget di ogni utente*/
bool *checkUser;     /*mostra lo stato di ogni utente.*/

int *rewardlist;     /*un registro pubblico del reward totale di ogni nodo.*/
int *poolsizelist;   /*un registro del dimensioni occupate pool transaction*/
sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
Transazione *mailbox;/*struttura per condividere */
bool *checkNode;     /*lista che mostra i nodi che sono attivi.*/

Transazione mainMailbox;
struct timespec startSimulation;

pthread_t *utenti_threads;     /*lista id di processi utenti*/
pthread_t *nodi_threads;     /*lista id di processi nodi  */
Configurazione configurazione;

```
## Transazioni programmate

Le transazioni programmate sono una lista di transazioni che vengono letti da un file che contiene una transazione per ogni riga. Ogni riga contiene lo timestamp ,sender, reciever e quantità della transazione. Quando è il momento del timeStamp della transazione viene creata una segnale dal main per forzare che l'utente sender faccia questa transazione.



### Lettura del file di transazioni pianificati
questa funzione non ha bisogno di ritornare un array perche può essere passato come parametro della funzione e si scrive direttamente nell'array. 
Per questo motivo il return della funzione ritornerà un valore intero che rappresenta la quantità di transazioni programmate.

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
        while(fscanf(file,"%ld %d %d %d",&programmate[i].timestamp,&programmate[i].sender,&programmate[i].receiver,&programmate[i].quantita) != EOF && i<100){
            programmate[i].reward = programmate[i].quantita * configurazione.SO_REWARD / 100;
            if(programmate[i].reward < 1){
                programmate[i].reward =1;
            }
            i++;
        }
    }
    return i;
}

```

### Segnale

La segnale è una maniera di forzare un'utente a fare una transazione già creata dal master con valori predefiniti.
```c main.c

/*segnale che forza una transazione di un'utente.*/
void segnale(Transazione programmato){
    mailbox[nodoLibero(programmato.sender)] = programmato;/*assegno la transazione in un mailbox*/

    budgetlist[programmato.sender] -= programmato.quantita;
    printf("Segnale ->");
    prinTrans(programmato);
}
```

## Nuovo nodo

Funzione che ridimensiona tutte le liste per dopo creare un nuovo nodo e inviare la transazione che non è stata  possibile condividere  con nessun altro nodo.
```c main.c

void* gestore(){
    int i;
    int semvalue;

    while(getTimeS() < configurazione.SO_SIM_SEC){
        if(gestoreOccupato){
            /*resize each list with realloc*/
            poolsizelist=realloc(poolsizelist,(configurazione.SO_NODES_NUM+1)*sizeof(int));
            rewardlist  =realloc(rewardlist ,(configurazione.SO_NODES_NUM+1)*sizeof(int));
            semafori     =realloc(semafori,(configurazione.SO_NODES_NUM+1)*sizeof(sem_t));
            checkNode    =realloc(checkNode,(configurazione.SO_NODES_NUM+1)*sizeof(bool));
            nodi_threads=realloc(nodi_threads,(configurazione.SO_NODES_NUM+1)*sizeof(pthread_t));
            mailbox=realloc(mailbox, (configurazione.SO_NODES_NUM+1)*6*sizeof(int));

            rewardlist[configurazione.SO_NODES_NUM]=0;
            poolsizelist[configurazione.SO_NODES_NUM]=0;

            /*inizia il nuovo trhead*/
            pthread_create(&nodi_threads[configurazione.SO_NODES_NUM],NULL,nodo,NULL);

            mailbox[configurazione.SO_NODES_NUM] = mainMailbox;

            /*si reapre il gestore di nuovi nodi*/
            gestoreOccupato=false;
            configurazione.SO_NODES_NUM++;
        }
        randomSleep(10,10);

    }
}

```

## Funzione Master

E' il metodo principale del progetto. Il suoi compiti sono:

- leggere la configurazione, sia file o manuale
- inizzializare tutta la memoria condivisa
- creare tutti i processi nodo e utente
- stampare l'informazione dei processi attivi
- creare un nodo nuovo quando nessun nodo riesce a prendere una transazione dopo HOPS volte.
- chiudere tutti i processi 

```c main.c
int main(int argc,char *argv[]){
    int i;
    struct timespec now;
    pthread_t thrGestore;


    /*variabili delle transazioni programmate*/
    int programmateCounter;
    bool *programmateChecklist;
    Transazione programmate[100];


    srand(time(0)); /*aleatorio*/

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
        if(argc == 3){
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
        sem_init(&libroluck,configurazione.SO_NODES_NUM,1);/*inizia il semaforo del libromastro*/

        gestoreOccupato=false;
        clock_gettime(CLOCK_REALTIME,&startSimulation);
    
        /*generatore dei nodi*/
        poolsizelist=calloc(configurazione.SO_NODES_NUM , sizeof(int));
        rewardlist=calloc(configurazione.SO_NODES_NUM , sizeof(int));
        semafori=calloc(configurazione.SO_NODES_NUM , sizeof(sem_t));
        mailbox=calloc(configurazione.SO_NODES_NUM , 6 * sizeof(int));
        nodi_threads = malloc(configurazione.SO_NODES_NUM * sizeof(pthread_t));
        checkNode = calloc(configurazione.SO_NODES_NUM , sizeof(bool));
        for(i=0;i<configurazione.SO_NODES_NUM;i++){
            pthread_create(&nodi_threads[i],NULL,nodo,NULL);
        }

        pthread_create(&thrGestore,NULL,gestore,NULL);

        /*generatore dei utenti*/
        budgetlist=calloc(configurazione.SO_USERS_NUM , sizeof(int));
        utenti_threads = calloc(configurazione.SO_USERS_NUM , sizeof(pthread_t));
        checkUser = calloc(configurazione.SO_USERS_NUM , sizeof(bool));
        for(i=0;i<configurazione.SO_USERS_NUM;i++){
            pthread_create(&utenti_threads[i],NULL,utente,NULL);
        }

        
        while(getTimeS() < configurazione.SO_SIM_SEC){

            sleep(1);
            clear();

            /*show last update*/
            printf("ultimo aggiornamento: %ld/%d\n",getTimeS(),configurazione.SO_SIM_SEC);


            if(libroCounter > SO_REGISTRY_SIZE){
                break;
            }
            

            if(!printStatus(40)){
                printf("tutti gli utenti sono disattivati");
                break;
            }

            /* transazioni programmate mancanti*/
            for(i=0; i< programmateCounter; i++){
                if(programmate[i].timestamp <= getTimeN() && programmateChecklist[i]){
                    segnale(programmate[i]);
                    programmateChecklist[i] = false;
                }
            }

        }
        finalprint();
    
    }
    return 0;
}
```
