
# Table of Contents

1.  [Compilazione](#org3d1550e)
2.  [Headers](#org490c68f)
    1.  [Basic libraries](#org0a118af)
    2.  [specific libraries](#org2762bdf)
    3.  [Importa le funzioni utente](#orgc6664f9)
    4.  [macro](#org1945bc4)
3.  [Controllo del LIBRO_MASTRO](#orgd0244e6)
4.  [Funzioni in parallelo](#orgdefd6a0)
    1.  [Memoria condivisa (work in progress)](#org7d59c22)
        1.  [external resources](#org57db06e)
        2.  [lista dei semafori ed altre dati condivise tra i diversi thread:](#org2d10bd3)
5.  [Lettura configurazione](#org3864e20)
    1.  [dichiaro variabile configurazione](#org84575e2)
    2.  [legge file](#orgf7155b2)
    3.  [Scrittura manuale](#orge34b092)
6.  [main](#org0f6ec9c)
    1.  [show users](#orgc632acf)
    2.  [Show nodes](#orgca2ba3c)
    3.  [main function](#org8034f83)




# Compilazione

La compilazione avviene tramite MAKEFILE:
```makefile Makefile
all:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
```


### std=c89
set the language standard C89.

### pthread
Setting the binary for thread processing.

### pedantic

### D_GNU_SOURCE
enables GNU extensions to the C and OS standards supported by the 
GNU C library.

### SO_BLOCK_SIZE
The size of the block in the simulation.


### SO_REGISTRY_SIZE
The max size of the libro mastro.

### -lm
Compiles against the shared library.


# Headers
## Basic libraries
``` c main.c
#include <stdio.h>  /*Standard input-output header*/
#include <stdlib.h> /*Libreria Standard*/  
#include <time.h>   /*Acquisizione e manipolazione del tempo*/
#include <stdbool.h>/*Aggiunge i boolean var*/

```

## specific libraries
```c main.c
#include <unistd.h>      /*Header per sleep()*/
#include <pthread.h>     /*Creazione/Modifica thread*/
#include <semaphore.h>   /*Aggiunge i semafori*/

```

## Importa le funzioni utente
importando le funzioni di [User.c](User.md), anche sono inclusi le funzioni di [Nodo](Node.md) e [Structs](Structs.md).
``` c main.c
#include "User.c"
```

## macro

Una macro è un frammento di codice a cui viene assegnato un nome.

``` c main.c
#define clear() printf("\033[H\033[J") /*clear the screen*/
```

# Controllo del LIBRO_MASTRO

Creazione del libroMastro e tutte le variabili:
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

### 0 avanti
Il processo puo accedere direttamente al dato.


### <0 aspetta
Il processo aspetta per accedere al dato
o in alternativa sceglie unn'altra via per l'accesso.

### external resources

1.  general semaphore example <https://www.delftstack.com/howto/c/semaphore-example-in-c/>
2.  trywait <https://stackoverflow.com/questions/27294954/how-to-use-sem-trywait>


### lista dei semafori ed altre dati condivise tra i diversi thread:

```c main.c
/*variabili condivise tra diversi thread.*/
int *retrylist;      /*numero di tentativi di ogni utente*/
int *budgetlist;     /*un registro del budget di ogni utente*/
int *rewardlist;     /*un registro pubblico del reward totale di ogni nodo.*/
sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
Transazione *mailbox;/*struttura per condividere */
time_t startSimulation;
```

# Lettura configurazione
## dichiaro variabile configurazione

```c main.c
Configurazione configurazione;
```

## legge file
```c main.c
/*Un metodo che fa un fgets(con gli stessi parametri e lo 
ritorna come un valore intero)*/
int readAndInt(char *str, int n, FILE *stream){
	fgets(str,n,stream);
    return atoi(str);
}
/*Funzione che cerca la maniera di leggere il config file.
/*metodo basato in codice di stackoverflow per leggere file come
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

## Scrittura manuale

Forse per la parte di prove. possiamo cambiare la intro delle variabili.
probabilmente cancelliamo questo alla fine del progetto.
l'idea e' poter inserire le variabili a mano

```c main.c
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

# main

## show users
```c main.c
void showUsers(){
	int i;
    int counterAttivi=0;
	bool test;
    printf("Utenti:\n");
	/*mostra il budget di ogni utente*/
	for(i=0; i<configurazione.SO_USERS_NUM; i++){
		test = retrylist[i]<configurazione.SO_RETRY;
  	    if(test)
 	       counterAttivi++;
   	    printf("%d) %d %s\t",i,budgetlist[i],test ? "true":"false");
   	    if(i%9==0)
   	       printf("\n");
	}
   	 printf("\nattivi: %d\n",counterAttivi);
}

```

## Show nodes
```c main.c
void showNodes(){
	int i;
 	int counterAttivi;
  	printf("\nnodi: \n");
    for(i=0; i<configurazione.SO_NODES_NUM; i++){
    	sem_getvalue(&semafori[i],&counterAttivi);
    	printf("%d) %d %d\t",i,rewardlist[i],counterAttivi);
    }
}

```

## main function

```c main.c
int main(int argc,char *argv[]){
    int i;
	float now;
    bool test;
    pthread_t *tid;
    int counterAttivi;
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
        sem_init(&libroluck,0,0);/*inizia il semaforo del libromastro*/
        tid=malloc((configurazione.SO_NODES_NUM + configurazione.SO_USERS_NUM) * sizeof(pthread_t));
    
        /*generatore dei nodi*/
        rewardlist=malloc(configurazione.SO_NODES_NUM * sizeof(int));
        semafori=malloc(configurazione.SO_NODES_NUM * sizeof(sem_t));
        mailbox=malloc(configurazione.SO_NODES_NUM * ((4 * sizeof(int)) + sizeof(time_t)));
        for(i=0;i<configurazione.SO_NODES_NUM;i++){
			sem_init(&semafori[i],configurazione.SO_USERS_NUM,1);
			rewardlist[i]=0;
			pthread_create(&tid[i],NULL,nodo,&i);
			usleep(200);
	    	/*pthread_join(tid[i],NULL);*/
        }
        /*generatore dei utenti*/
        retrylist =malloc(configurazione.SO_USERS_NUM * sizeof(int));
        budgetlist=malloc(configurazione.SO_USERS_NUM * sizeof(int));
        for(i=0;i<configurazione.SO_USERS_NUM;i++){
			retrylist[i] = 0;
			budgetlist[i] = configurazione.SO_BUDGET_INIT;
			pthread_create(&tid[configurazione.SO_NODES_NUM+i],NULL,utente,(void *)&i);
			usleep(200);
			/*pthread_join(tid[i],NULL);*/
        }
    
		/*now start the master process*/
		now = difftime(time(0), startSimulation);
		while(now < configurazione.SO_SIM_SEC){
			sleep(1);
			clear();
    
			/*show last update*/
	    	printf("ultimo aggiornamento: %.2f/%d\n",difftime(time(0),startSimulation),configurazione.SO_SIM_SEC);

	    	/*conta la quantita di utenti attivi*/
	    	showUsers();
    
	    	/*mostra i nodi con i suoi semafori */
	    	showNodes();
	    	printf("\n\n");
    
	    	now = difftime(time(0), startSimulation);
        }
    
        /*kill all the threads*/
        for(i=0;i<configurazione.SO_NODES_NUM + configurazione.SO_USERS_NUM; i++){
			pthread_cancel(tid[i]);
		}
    
		printf("numero di blocchi: %d\n\n",libroCounter);
		/*solo por confirmar al final*/
		for(i=0;i<libroCounter;i++){
			prinTrans(libroMastro[i]);
        }
    
	}
	return 0;
}
```
