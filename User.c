#include "Node.c"

extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck;/*luchetto per accedere solo un nodo alla volta*/


/*variabili condivise tra diversi thread.*/

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
	    	    userList[id].budget += libroMastro[i].quantita - libroMastro[i].reward;
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
        if( userList[id].retry > configurazione.SO_RETRY){
            userList[id].stato = false;
            printf("L'utenete %d non ha trovato nessun nodo libero\n",id);
            pthread_cancel(utenti_id[id]);
        }
        userList[id].retry++;
    }while(sem_trywait(&nodeList[nodo].semaforo)<0);
    
    if( userList[id].retry <= configurazione.SO_RETRY ){
        userList[id].retry = 0;
    }

    return nodo;
}

Transazione generateTransaction(int id){
    int altroUtente;
	Transazione transaccion;
    transaccion.sender   = id;
    transaccion.quantita = randomInt(2,userList[id].budget/2);/*set quantita a caso*/
	transaccion.reward   = transaccion.quantita * configurazione.SO_REWARD/100;/*percentuale de la quantita*/
    
	/*se il reward non arriva a 1, allora diventa 1*/
    if(transaccion.reward < 1){
		transaccion.reward = 1;
    }
    
    /*ricerca del riceiver*/
    /*debo reparar lo de los intentos*/
    do{
		altroUtente= randomInt(0,configurazione.SO_USERS_NUM);
	}while( altroUtente==id || !userList[altroUtente].stato );
	transaccion.receiver = altroUtente;
	/*calcola il timestamp in base al tempo di simulazione.*/
	transaccion.timestamp = difftime(time(0),startSimulation);

	return transaccion;
}

/*PROCESSO UTENTE:*/
void* utente(void *conf){
	int id = trovaId();                       /*Id processo utente*/
    int i;
    pthread_t mythr = pthread_self();          /*Pid thread processo utente*/
    int lastUpdate = 0;                        /*questo controlla l'ultima versione del libro mastro*/

	/*setting default values delle variabili condivise*/
    userList[id].budget = configurazione.SO_BUDGET_INIT;
    userList[id].retry  = 0;
    userList[id].stato  = true;

	/*printf("Utente #%d creato nel thread %d\n",id,mythr);*/
    

	while( userList[id].stato ){
    
		lastUpdate = userUpdate(id,lastUpdate);  /*Aggiorniamo Budgetdel Processo Utente*/
    
		if(userList[id].budget>=2){                   /*Condizione Budget >= 2*/                                
    
			Transazione transaction;              /*Creiamo una nuova transazione*/
			transaction = generateTransaction(id);/*Chiamiamo la func generateTransaction*/
    
			/*scelglie un nodo libero a caso*/
            nodeList[nodoLibero(id)].mailbox = transaction;
            userList[id].budget -= transaction.quantita;
        }else{
			userList[id].retry++;
		}
    
		randomSleep( configurazione.SO_MIN_TRANS_GEN_NSEC , configurazione.SO_MAX_TRANS_GEN_NSEC);
    
		if(userList[id].retry >= configurazione.SO_RETRY || !userList[id].stato ){/*Se raggiunge il n° max di tentativi*/
			printf("utente %d fermato\n",id);       /*ferma il procceso*/
            userList[id].stato = false;
		}
    }
}
