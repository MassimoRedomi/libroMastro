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
