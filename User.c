#include "Node.c"

extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck;/*luchetto per accedere solo un nodo alla volta*/

/*variabili condivise tra diversi thread.*/
extern int *budgetlist;     /*un registro del budget di ogni utente*/
extern bool *checkUser;
extern sem_t UserStartSem;

extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern int *poolsizelist;  /*un registro del dimensioni occupate pool transaction*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */

extern Configurazione configurazione;
extern pthread_t *utenti_threads;      /*lista id dei processi utenti*/

/*cerca la posizione del thread del utente.*/
int trovaUtenteID(){
    int id;
    for(id=0;id<configurazione.SO_USERS_NUM; id++){
        if(pthread_self() == utenti_threads[id]){
            break;
        }
    }
    return id;
}
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

/*cerca un nodo libero per fare la trasazione.*/
int nodoLibero(int id){
    int nodo;
    int retry = 0;
    do{
        nodo = randomInt(0,configurazione.SO_NODES_NUM);
        if( retry > configurazione.SO_RETRY){
            /*printf("L'utenete %d non ha trovato nessun nodo libero\n",id);*/
            checkUser[id]= false;
            pthread_cancel(utenti_threads[id]);
        }
        retry++;
    }while(sem_trywait(&semafori[nodo])<0 && checkUser[id]);
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
    }while(altroUtente==id || !checkUser[altroUtente]);
    transaccion.receiver = altroUtente;
    /*calcola il timestamp in base al tempo di simulazione.*/
    transaccion.timestamp = getTimeN();

    return transaccion;
}

/*PROCESSO UTENTE:*/
void* utente(){
    int id = trovaUtenteID();                       /*Id processo utente*/
    int i;
    pthread_t mythr = pthread_self();          /*Pid thread processo utente*/
    int lastUpdate = 0;                        /*questo controlla l'ultima versione del libro mastro*/
    int retry=0;
    /*setting default values delle variabili condivise*/
    checkUser[id] = true;
    budgetlist[id] = configurazione.SO_BUDGET_INIT;

    /*printf("Utente #%d creato nel thread %d\n",id,mythr);*/

    while(checkUser[id]){

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

        if(retry >= configurazione.SO_RETRY){/*Se raggiunge il n° max di tentativi*/
            /*printf("utente %d fermato\n",id);       /*ferma il procceso*/
            checkUser[id]=false;
        }
    }
}
