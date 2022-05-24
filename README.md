# Compilazione

La compilazione avviene tramite MAKEFILE:
### configurazione 1:  
SO_BLOCK_SIZE = 100
SO_REGISTRY_SIZE = 1000
```makefile Makefile
1:
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=100 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
```

### configurazione 2:
SO_BLOCK_SIZE = 10
SO_REGISTRY_SIZE = 10000
```makefile Makefile
2:
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=10000 main.c -lm -o main
```

### configurazione 3:
SO_BLOCK_SIZE = 10
SO_REGISTRY_SIZE = 1000
```makefile Makefile
3:
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
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
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE = $(block) -DSO_REGISTRY_SIZE = $(registry) main.c -lm -o main
	
```


## significato di ogni flag
 - std=c89: set the language standard C89.

 - pthread
Setting the binary for thread processing.

 - pedantic

 - D_GNU_SOURCE: enables GNU extensions to the C and OS standards supported by the GNU C library.

 - SO_BLOCK_SIZE: The size of the block in the simulation.


 - SO_REGISTRY_SIZE: The max size of the libro mastro.

 - lm: Compiles against the shared library.


# Esecuzione del programma
Dopo di aver compilato il programma solo ci manca inizializzarlo.
Per questo si puo fare due maniere diverse: passando un file con 
tutta la configurazione, o scriverla manualmente.

## con file di configurazione
Nel caso di inviare un file configuration. si passa come argomento di
esecuzione.
```sh
./main conf1.dat
```

## scritura manuale 
Per scrivere la configurazione a manualmente si deve scrivere come secondo 
argomento la parola "mano" o "manuale".
```sh
./main manual
```


## aggiunge segnali
nel caso delle segnali per forzare certe transazioni, non è obbligatorio per inizializare il programma, ma se si aspetta fare questo si aggiunge un terzo argomento con l'indirizzo del file con tutte le transazioni che si aspettano. 
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
#define MIN(z,w) ((z<w)?z:w) /*min between to parameters*/
#define boolString(b) ((b) ? "True":"False")/*make the %b*/
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
    fscanf(file,"%d",&configurazione.SO_USERS_NUM);
    printf("SO_USERS_NUM: %d\n",configurazione.SO_USERS_NUM);
    fscanf(file,"%d",&configurazione.SO_NODES_NUM);
    printf("SO_NODES_NUM: %d\n",configurazione.SO_NODES_NUM);
    fscanf(file,"%d",&configurazione.SO_BUDGET_INIT);
    printf("SO_BUDGET_INIT: %d\n",configurazione.SO_BUDGET_INIT);
    fscanf(file,"%d",&configurazione.SO_REWARD);
    printf("SO_REWARD: %d\n",configurazione.SO_REWARD);
    fscanf(file,"%d",&configurazione.SO_MIN_TRANS_GEN_NSEC);
    printf("SO_MIN_TRANS_GEN_NSEC: %d\n",configurazione.SO_MIN_TRANS_GEN_NSEC);
    fscanf(file,"%d",&configurazione.SO_MAX_TRANS_GEN_NSEC);
    printf("SO_MAX_TRANS_GEN_NSEC: %d\n",configurazione.SO_MAX_TRANS_GEN_NSEC);
    fscanf(file,"%d",&configurazione.SO_RETRY);
    printf("SO_RETRY: %d\n",configurazione.SO_RETRY);
    fscanf(file,"%d",&configurazione.SO_TP_SIZE);
    printf("SO_TP_SIZE: %d\n",configurazione.SO_TP_SIZE);
    fscanf(file,"%d",&configurazione.SO_MIN_TRANS_PROC_NSEC);
    printf("SO_MIN_TRANS_PROC_NSEC: %d\n",configurazione.SO_MIN_TRANS_PROC_NSEC);
    fscanf(file,"%d",&configurazione.SO_MAX_TRANS_PROC_NSEC);
    printf("SO_MAX_TRANS_PROC_NSEC: %d\n",configurazione.SO_MAX_TRANS_PROC_NSEC);
    fscanf(file,"%d",&configurazione.SO_SIM_SEC);
    printf("SO_SIM_SEC: %d\n",configurazione.SO_SIM_SEC);
    fscanf(file,"%d",&configurazione.SO_FRIENDS_NUM);
    printf("SO_FRIENDS_NUM: %d\n",configurazione.SO_FRIENDS_NUM);
    fscanf(file,"%d",&configurazione.SO_HOPS);
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
	return rand() % max +min;
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
extern sem_t mainSem;

```


### Sincronizzazione tra Processi
```c Node.c
/*variabili condivise tra diversi thread.*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */
extern int *poolsizelist;  /*un registro del dimensioni occupate pool transaction*/
extern bool *checkNode;

extern Configurazione configurazione;
extern time_t startSimulation;
extern pthread_t *nodi_id;       /*lista dei processi nodi*/

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
    int id = (int)conf;
    int i;
    int counterBlock=0;/*contatore della quantita di transazioni nel blocco*/
    int sommaBlocco=0; /*somma delle transazioni del blocco atuale*/
    Transazione blocco[SO_BLOCK_SIZE];
    Transazione pool[1000];/*stabilisce 1000 come la grandezza massima del pool, cmq si ferma in configurazione.SO_TP_SIZE*/
    Transazione finalReward;
    int mythr; 
    int semvalue;/*valore del semaforo*/
    int *amici = calloc(configurazione.SO_FRIENDS_NUM, sizeof(int));
    for(i=0;i<configurazione.SO_FRIENDS_NUM;i++){
        do{
            amici[i] = randomInt(0,configurazione.SO_FRIENDS_NUM);
        }while(amici[i]==i);
    }
    sem_post(&mainSem);
    sem_init(&semafori[id],configurazione.SO_USERS_NUM,1);/*inizia il semaforo in 1*/
	rewardlist[id]=0;/*set il reward di questo nodo in 0*/
    poolsizelist[id]=0;/*set full space available*/
    checkNode[id] = true;
    /*mythr = pthread_self();
    /*printf("Nodo #%d creato nel thread %d\n",id,mythr);*/
    
    /*inizio del funzionamento*/
    while(checkNode[id]){
    
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
	        }else{
                checkNode[id]=false;
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
extern sem_t mainSem;

```

#### Sincronizzazione tra Processi

Importa tutte le variabili del Main 

```c User.c
/*variabili condivise tra diversi thread.*/
extern int *budgetlist;     /*un registro del budget di ogni utente*/
extern bool *checkUser;
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

## trova nodo
serve per trovare un nodo libero per fare la transazione.
```c User.c
/*cerca un nodo libero per fare la trasazione.*/
int nodoLibero(int id){
    int nodo;
    int retry = 0;
    do{
        nodo = randomInt(0,configurazione.SO_NODES_NUM);
        if( retry > configurazione.SO_RETRY){
            printf("L'utenete %d non ha trovato nessun nodo libero\n",id);
            checkUser[id]= false;
            pthread_cancel(utenti_id[id]);
        }
        retry++;
    }while(sem_trywait(&semafori[nodo])<0 && checkUser[id]);
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
	}while(altroUtente==id || !checkUser[altroUtente]);
	transaccion.receiver = altroUtente;
	/*calcola il timestamp in base al tempo di simulazione.*/
	transaccion.timestamp = difftime(time(0),startSimulation);

	return transaccion;
}

```

## Processo Utente Principale

```c User.c
/*PROCESSO UTENTE:*/
void* utente(void *conf){
	int id = (int)conf;                       /*Id processo utente*/
    int i;
    pthread_t mythr = pthread_self();          /*Pid thread processo utente*/
    int lastUpdate = 0;                        /*questo controlla l'ultima versione del libro mastro*/
    int retry=0;
	/*setting default values delle variabili condivise*/
    checkUser[id] = true;
	budgetlist[id] = configurazione.SO_BUDGET_INIT;
    sem_post(&mainSem);

	/*printf("Utente #%d creato nel thread %d\n",id,mythr);*/
    

	while(retry < configurazione.SO_RETRY){
    
		lastUpdate = userUpdate(id,lastUpdate);  /*Aggiorniamo Budgetdel Processo Utente*/
    
		if(budgetlist[id]>=2){                   /*Condizione Budget >= 2*/                                
			Transazione transaction;              /*Creiamo una nuova transazione*/
            retry = 0;
			transaction = generateTransaction(id);/*Chiamiamo la func generateTransaction*/
    
			/*scelglie un nodo libero a caso*/
            mailbox[nodoLibero(id)] = transaction;
            budgetlist[id] -= transaction.quantita;
        }else{
			retry++;
		}
    
		randomSleep( configurazione.SO_MIN_TRANS_GEN_NSEC , configurazione.SO_MAX_TRANS_GEN_NSEC);
    
		if(retry >= configurazione.SO_RETRY || !checkUser[i] ){/*Se raggiunge il n° max di tentativi*/
			printf("utente %d fermato\n",id);       /*ferma il procceso*/
		}
    }
}
```

# Prints

## memoria condivise
```c print.c
/*variabili degli utenti*/
extern int *budgetlist;
extern bool *checkUser;

/*variabili degli utenti*/
extern int *rewardlist;
extern int *rewardlist;
extern int *poolsizelist;
extern bool *checkNode;

```
## Compare Function
Metodo che compara due valori e restituisce un numero positivo se b è piu
grande di a e negativo se b è piu piccolo di a:
```c print.c
int cmpfunc(const void  *a, const void *b) {
    return(budgetlist[*((int*)b)]-budgetlist[*((int*)a)]);
}
```


## Sort risults
Metodo di ordinamento del dei processi in modo decrescente
(dal piu grande al piu piccolo).

```c print.c
int * sort(){
    int dim=MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM);
    int *r=malloc(sizeof(int)*dim);
    int i;
    for(i=0; i<dim; i++)
        r[i]=i;
    
    qsort(r, dim, sizeof(int), cmpfunc);
    return r;
}
```


## PrintStatus Nodes and Users
Questo metodo non solo mostra lo stato di tutti gli
utenti e nodi, ritorna anche una variabile boolean per identificare
se ci sono ancora utenti disponibili.

```c print.c
bool printStatus(int nstamp){
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
    int *pa;
    int dim=MIN(configurazione.SO_USERS_NUM, nstamp);
    pa=sort();
    printf("\n\n");
    printf("------------------------------------------------------------------------\n");
    printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  Status ||\n");
    printf("||===============================|##|=================================||\n");
    
    /*Stampa risultati*/
    for(i=0; i<configurazione.SO_USERS_NUM; i++){
        sommaBudget += budgetlist[*(pa+i)];

        checkUser[*(pa+i)] ? activeUsers++ : inactiveUsers++;

        if(i<dim){
            printf("||%10d|%10d|%9s|#",*(pa+i),budgetlist[*(pa+i)], boolString(checkUser[*(pa+i)]));
        }

        if(i< configurazione.SO_NODES_NUM){
            sommaRewards+=rewardlist[i];
            checkNode[i] ? activeNodes++ : inactiveNodes++;
            printf("#|%11d|%11d|%9s||\n", i, rewardlist[i],boolString(checkNode[i]));
        }else if(i<dim){
            printf("#|           |           |         ||\n");
        }
    }

    printf("------------------------------------------------------------------------\n");
    printf("||  Active Users  |  Tot Budget  |##|  Active Nodes  |   Tot Rewards  ||\n");
    printf("||%16d|%14d|##|%16d|%16d||\n",activeUsers,sommaBudget,activeNodes, sommaRewards);
    printf("\n");
    
    return activeUsers!=0;
}
```

## final print
```c print.c
void finalprint(){
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
 
    printf("\n\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  p_size | Status ||\n");
    printf("||===============================|##|==========================================||\n");
    for(i=0; i< configurazione.SO_USERS_NUM; i++){
        sommaBudget += budgetlist[i];

        checkUser[i] ? activeUsers++ : inactiveUsers++;

        printf("||%10d|%10d|%9s|#",i,budgetlist[i], boolString(checkUser[i]));

        if(i< configurazione.SO_NODES_NUM){
            sommaRewards+=rewardlist[i];

            checkNode[i] ? activeNodes++ : inactiveNodes++;
            printf("#|%11d|%11d|%9d|%8s||\n", i, rewardlist[i],poolsizelist[i],boolString(checkNode[i]));
        }else{
            printf("#|           |           |         |        ||\n");
        }
    }
    printf("---------------------------------------------------------------------------------\n");
    printf("||   Active Users  |   Inactive Users  |##|   Active Nodes  |  Inactive Nodes  ||\n");
    printf("||%17d|%19d|##|%17d|%18d||\n",activeUsers,inactiveUsers, activeNodes, inactiveNodes);
    printf("||-----------------------------------------------------------------------------||\n");
    printf("||    Tot Rewards  |%59d||\n",sommaRewards);
    printf("||-----------------------------------------------------------------------------||\n");
    printf("||    Tot Budgets  |%59d||\n",sommaBudget);
    printf("||-----------------------------------------------------------------------------||\n");
    printf("||    Tot Block    |%59d||\n", libroCounter);
    printf("---------------------------------------------------------------------------------\n");
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
#include "print.c"
```

# Controllo LIBRO MASTRO

## Creazione del Libro_Mastro e Variabili:
- __libroluck__: Semaforo per accedere alla scrittura del libroMastro.
- __libroCounter__: Contatore della quantità di blocchi scritti nel libroMastro.

```c main.c
Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
int libroCounter=0;/*Counter controlla la quantitta di blocchi*/
sem_t libroluck;/*Luchetto per accedere solo a un nodo alla volta*/
sem_t mainSem;

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
int *budgetlist;     /*un registro del budget di ogni utente*/
bool *checkUser;
int *rewardlist;     /*un registro pubblico del reward totale di ogni nodo.*/
int *poolsizelist;   /*un registro del dimensioni occupate pool transaction*/
sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
Transazione *mailbox;/*struttura per condividere */
bool *checkNode;

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
    
        sem_init(&mainSem,configurazione.SO_NODES_NUM+configurazione.SO_USERS_NUM,1);
        startSimulation = time(0);/* el tiempo de ahora*/
        sem_init(&libroluck,0,1);/*inizia il semaforo del libromastro*/
    
        /*generatore dei nodi*/
        poolsizelist=calloc(configurazione.SO_TP_SIZE , sizeof(int));
        rewardlist=calloc(configurazione.SO_NODES_NUM , sizeof(int));
        semafori=calloc(configurazione.SO_NODES_NUM , sizeof(sem_t));
        mailbox=calloc(configurazione.SO_NODES_NUM , ((4 * sizeof(int)) + sizeof(double)));
        nodi_id = calloc(configurazione.SO_NODES_NUM , sizeof(pthread_t));
        checkNode = calloc(configurazione.SO_NODES_NUM , sizeof(bool));
        for(i=0;i<configurazione.SO_NODES_NUM;i++){
			pthread_create(&nodi_id[i],NULL,nodo,(void *)i);
            sem_wait(&mainSem);
        }

        /*generatore dei utenti*/
        budgetlist=calloc(configurazione.SO_USERS_NUM , sizeof(int));
        utenti_id = calloc(configurazione.SO_USERS_NUM , sizeof(pthread_t));
        checkUser = calloc(configurazione.SO_USERS_NUM , sizeof(bool));
        for(i=0;i<configurazione.SO_USERS_NUM;i++){
			pthread_create(&utenti_id[i],NULL,utente,(void *)i);
            sem_wait(&mainSem);
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

            if(!printStatus(40)){
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
        finalprint();
    
        /*kill all the threads*/
        for(i=0; i<configurazione.SO_NODES_NUM ; i++){
			pthread_cancel(nodi_id[i]);
		}
        for(i=0; i<configurazione.SO_USERS_NUM; i++){
            pthread_cancel(utenti_id[i]);
        }
    
		/*
        printf("numero di blocchi: %d\n\n",libroCounter);
		solo por confirmar al final
		for(i=0;i<libroCounter*SO_BLOCK_SIZE;i++){
			prinTrans(libroMastro[i]); per ora non mostro tutte transazioni
        }
        */
        
	}
	return 0;
}
```
