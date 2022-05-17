#include "Node.c"

extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck;/*luchetto per accedere solo un nodo alla volta*/


/*variabili condivise tra diversi thread.*/
extern int *retrylist ;     /*thread id di ogni utente*/
extern int *budgetlist;     /*un registro del budget di ogni utente*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern int *poolsizelist;  /*un registro del dimensioni occupate pool transaction*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */

/*variabili simplificati a due array*/
extern userStruct *userList;
extern nodeStruct *nodeList;

extern Configurazione configurazione;
extern time_t startSimulation;
extern pthread_t *utenti_id;      /*lista id dei processi utenti*/

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

/*Trova thread id in utenti_id*/
int trovaId(){
    int id;
    for(id=0;id<configurazione.SO_USERS_NUM;id++){
        if(utenti_id[id] == pthread_self()){
            return id;
        }
    }
}

/*cerca un nodo libero per fare la trasazione.*/
int nodoLibero(int id){
    int nodo;
    do{
        nodo = randomInt(0,configurazione.SO_NODES_NUM);
        if( retrylist[id] > configurazione.SO_RETRY){
            printf("L'utenete %d non ha trovato nessun nodo libero\n",id);
            pthread_cancel(utenti_id[id]);
        }
        retrylist[id]++;
    }while(sem_trywait(&semafori[nodo])<0);
    
    if( retrylist[id] <= configurazione.SO_RETRY ){
        retrylist[id] = 0;
    }

    return nodo;
}

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

/*PROCESSO UTENTE:*/
void* utente(void *conf){
	int id = trovaId();                       /*Id processo utente*/
    int i;
    pthread_t mythr = pthread_self();          /*Pid thread processo utente*/
    int lastUpdate = 0;                        /*questo controlla l'ultima versione del libro mastro*/

	/*setting default values delle variabili condivise*/
    retrylist[id] = 0; /*stabilisco in 0 il numero di tentativi*/
	budgetlist[id] = configurazione.SO_BUDGET_INIT;
    userList[id].thread = pthread_self();
    userList[id].budget = configurazione.SO_BUDGET_INIT;
    userList[id].retry  = 0;
    userList[id].stato  = true;

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
