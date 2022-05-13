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
