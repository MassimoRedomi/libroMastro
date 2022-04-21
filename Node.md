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


<a id="org71aa49b"></a>

### Sincronizzazione tra Processi
```c Node.c
/*variabili condivise tra diversi thread.*/
extern int *listUtenti;     /*thread id di ogni utente*/
extern int *budgetlist;     /*un registro del budget di ogni utente*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */
extern Configurazione configurazione;
extern time_t startSimulation;
extern pthread_t *nid;       /*lista dei processi nodi*/

```

## Funzione Principale
```c Node.c
void* nodo(void *conf){
	/*creazioni dei dati del nodo*/
    int *id = (int *)conf;
    int i;
    int counterBlock=0;/*contatore della quantita di transazioni nel blocco*/
    int counterPool=0;/*contatore della quantita di transazioni nella pool*/
    int sommaBlocco=0;/*somma delle transazioni del blocco atuale*/
    int range = configurazione.SO_MAX_TRANS_PROC_NSEC - configurazione.SO_MIN_TRANS_PROC_NSEC;
    Transazione blocco[SO_BLOCK_SIZE];
    Transazione pool[1000];/*stabilisce 1000 come la grandezza massima del pool, cmq si ferma in configurazione.SO_TP_SIZE*/
    Transazione finalReward;
    int mythr; 
    int semvalue;/*valore del semaforo*/
    mythr = pthread_self();
    printf("Nodo #%d creato nel thread %d\n",*id,mythr);
    
    /*inizio del funzionamento*/
    while(counterPool < configurazione.SO_TP_SIZE){
    
		/*aggiorno il valore del semaforo*/
        sem_getvalue(&semafori[*id],&semvalue);
        if(semvalue<0){
			/*scrivo la nuova transazione nel blocco e nella pool*/
	    	 pool[counterPool]=mailbox[*id];
	    	 blocco[counterBlock]=mailbox[*id];
    
	    	 /*somma il reward*/
	    	 sommaBlocco = blocco[counterBlock].reward;
	    	 rewardlist[*id] += blocco[counterBlock].reward;/*si mette al registro publico totale*/
    
	    	 /*incremento i contatori di posizione di pool e block*/
	    	 counterBlock++;
	    	 counterPool++;
    
	    	 if(counterBlock == SO_BLOCK_SIZE - 1){
	    	    /*si aggiunge una nuova transazione come chiusura del blocco*/
	    	    finalReward.timestamp = difftime(time(0),startSimulation);/*momento attuale della simulazione*/
	    	    finalReward.sender = -1;/*-1*/
	    	    finalReward.receiver = *id;/*identificatore del nodo*/
	    	    finalReward.quantita = sommaBlocco;/*somma di tutti i reward*/
	    	    finalReward.reward = 0;
    
	    	    blocco[counterBlock]= finalReward;/*aggiunge la transazione al blocco.*/
    
	    	    sem_wait(&libroluck);
	    	    for(i=0;i< SO_BLOCK_SIZE;i++){
	    	       libroMastro[(libroCounter * SO_BLOCK_SIZE) + i] = blocco[i];
	    	    }
	    	    /*si spostano i contatori*/
	    	    libroCounter++;
	    	    sem_post(&libroluck);
	    	    counterBlock=0;
	    	    sommaBlocco=0;
	    	    usleep((rand() % (range + 1)) + configurazione.SO_MIN_TRANS_PROC_NSEC);
	    	    free(&mailbox[*id]);
    
	    	    if(counterPool < configurazione.SO_TP_SIZE){
	    	       sem_post(&semafori[*id]);/*stabilisco il semaforo come di nuovo disponibile*/
	    	    }  
	    	}
    
		}
    
	}
}
```
