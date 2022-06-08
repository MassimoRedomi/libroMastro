# 5.Nodo

## Importa Variabili Globali
Importa funzioni e strutture di [Structs](#3.Strutture).
```c Node.c
#include "Structs.c"
#define defaultSender -1
```

### Controllo del LIBRO_MASTRO

Import del libroMastro e tutte le variabili:
- __libroluck__:   Semaforo per accedere alla scrittura del libroMastro.
- __libroCounter__: Contatore che indica la quantità di blocchi scritti nel libroMastro.

```c Node.c
extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck; /*luchetto per accedere solo un nodo alla volta*/

```

### Memoria Condivisa

Il nodo non ha bisogno delle variabili degli utenti. Quindi solo servono le variabili del main e dei altri nodi nodi.

```c Node.c
/*variabili condivise tra diversi thread.*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */
extern int *poolsizelist;   /*un registro del dimensioni occupate pool transaction*/
extern bool *checkNode;
extern pthread_t *nodi_threads;

extern Transazione mainMailbox;
extern bool gestoreOccupato;


extern Configurazione configurazione;

```

## trova ID del Nodo
Per colpa del pedantic nel [Makefile](#1.Compilazione) non possiamo fare un cast da integer a un puntatore void. Questo ci limita per passare argomenti a un thread, e per tanto anche ci impide passare l'ID al nodo come un argomento. Per questo motivo dobbiamo creare una funzione che trova l'ID del nodo in base alla posizione del thread nella lista nodi_threads. A differenza del trovaUtenteID, questa funzione inizia la ricerca da SO_NODES_NUM, lo facciamo per ridurre la quantità di cicli che fanno i nodi creati a metà simulazione da parte del main.
```c Node.c
/*cerca la posizione del thread del nodo.*/
int trovaNodoID(){
    int id;

    for(id=configurazione.SO_NODES_NUM; id>=0; id--){
        if(pthread_self() == nodi_threads[id]){
            break;
        }
    }
    return id;
}
```

## transazione di riassunto
Questo metodo genera l'ultima transazione del blocco. 
Questa transazione fa un riassunto di tutto quello che ha guadagnato il nodo in questo blocco. 

```c Node.c

/*funzione dell'ultima transazione del blocco.*/
Transazione riasunto(int id, int somma){
    Transazione transaction;
    transaction.sender    = defaultSender;
    transaction.receiver  = id; /*id del nodo*/
    transaction.quantita  = somma; /*la somma di tutto il reward generato*/
    transaction.timestamp = getTimeN();/*quanto tempo ha passato dal inizio della simulazione.*/
    return transaction;
}

```

## Invia transazione a nodo amico
Questa funzione invia una transazione a un amico o crea un nuovo nodo per inviarselo. 
```c Node.c
void inviaAdAmico(int *amici,int id){
    bool inviaAmico=true;
    int hops=0;
    int i;
    int len = sizeof(amici)/sizeof(int);
    while(inviaAmico){
        for(i=0; i<len && inviaAmico;i++){
            if(checkNode[amici[i]]){/*evito inviare a un nodo pieno.*/
                if(sem_trywait(&semafori[*(amici+i)])){
                    mailbox[amici[i]]=mailbox[id];
                    inviaAmico=false;
                }
            }
        }
        if(inviaAmico){
            /*printf("Il nodo %d non ha nessun amico\n",id);*/
            hops++;
            if(hops > configurazione.SO_HOPS){
                if(!gestoreOccupato){
                    gestoreOccupato=true;
                    mainMailbox=mailbox[id];
                    amici = realloc(amici,(len+1)*sizeof(int));
                    amici[len]= configurazione.SO_NODES_NUM;
                    hops=0;
                }
                inviaAmico=false;
            }
        }
    }
    sem_post(&semafori[id]);
}
```


## Funzione principale del nodo.
```c Node.c
void* nodo(){
	/*creazioni dei dati del nodo*/
    int id = trovaNodoID();
    int i;
    int hops=0;
    int counterBlock=0;/*contatore della quantita di transazioni nel blocco*/
    int sommaBlocco=0; /*somma delle transazioni del blocco atuale*/
    Transazione blocco[SO_BLOCK_SIZE];
    Transazione pool[1000];/*stabilisce 1000 come la grandezza massima del pool, cmq si ferma in configurazione.SO_TP_SIZE*/
    int semvalue;/*valore del semaforo*/
    int *amici = calloc(configurazione.SO_FRIENDS_NUM, sizeof(int));
    bool inviaAmico=true;
    for(i=0;i<configurazione.SO_FRIENDS_NUM;i++){
        do{
            amici[i] = randomInt(0,configurazione.SO_NODES_NUM);
        }while(amici[i]==id);
    }
    sem_init(&semafori[id],configurazione.SO_USERS_NUM,1);/*inizia il semaforo in 1*/
    rewardlist[id]=0;/*set il reward di questo nodo in 0*/
    poolsizelist[id]=0;/*set full space available*/
    checkNode[id] = true;
    
    /*inizio del funzionamento*/
    while(checkNode[id]){

        /*aggiorno il valore del semaforo*/
        sem_getvalue(&semafori[id],&semvalue);
        if(semvalue <= 0){

            /*scrivo la nuova transazione nel blocco e nella pool*/
            if(counterBlock==SO_BLOCK_SIZE/2 && inviaAmico){
                inviaAdAmico(amici,id);
                inviaAmico=false;
                continue;
             }
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
                    /*se hai bisogno di dimostrare che si scrive il libro mastro,
                    scomenta il seguente print. Questo stampa tutto il blocco quando si 
                    scrive nel libroMastro.*/
                    /*prinTrans(blocco[i]);*/
                }
                /*si spostano i contatori*/
                libroCounter++;
                sem_post(&libroluck);
                counterBlock=0;
                sommaBlocco=0;
                inviaAmico=true;
                randomSleep(configurazione.SO_MIN_TRANS_PROC_NSEC,configurazione.SO_MAX_TRANS_PROC_NSEC);
	    	    /*free(&mailbox[id]);*/


            }
            sem_post(&semafori[id]);/*stabilisco il semaforo come di nuovo disponibile*/
            if(poolsizelist[id] >= configurazione.SO_TP_SIZE){
                checkNode[id]=false;
            }

        }

    }
    /*nodo zombie*/
    while(getTimeS()<configurazione.SO_SIM_SEC){
        sem_getvalue(&semafori[id],&semvalue);
        if(semvalue <= 0){
            inviaAdAmico(amici,id);
        }
    }
}
```
