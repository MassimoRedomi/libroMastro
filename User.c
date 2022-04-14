#include "Node.c"

extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck;/*luchetto per accedere solo un nodo alla volta*/

/*variabili condivise tra diversi thread.*/
extern int *retrylist ;     /*thread id di ogni utente*/
extern int *budgetlist;     /*un registro del budget di ogni utente*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */
extern Configurazione configurazione;
extern time_t startSimulation;

/*aggiornamento del budget in base al libro.*/
int userUpdate(int id, int lastUpdate){
   int i;
   int budget = budgetlist[id];
   while(lastUpdate != libroCounter){
      for(i=lastUpdate*SO_BLOCK_SIZE; i < (lastUpdate+1)*SO_BLOCK_SIZE; i++){
         if(libroMastro[i].receiver == id){
	    budget += libroMastro[i].quantita;
	 }
      }
      lastUpdate++;
   }
   budgetlist[id] = budget;
   return lastUpdate;
}

Transazione generateTransaction(int id){
   Transazione transaccion;
   int i;
   transaccion.sender = id;
   transaccion.quantita = (rand() % ((budgetlist[id] - 2) + 1)) + 2;/*set quantita a caso*/
   transaccion.reward   = transaccion.quantita * configurazione.SO_REWARD/100;/*percentuale de la quantita*/
 
   /*se il reward non arriva a 1, allora diventa 1*/
   if(transaccion.reward < 1){
      transaccion.reward = 1;
   }
	 
   /*ricerca del riceiver*/
   /*debo reparar lo de los intentos*/
   do{
      i= rand() % configurazione.SO_USERS_NUM;
   }while(i==id || retrylist[i]<configurazione.SO_RETRY);
   transaccion.receiver = i;

   /*calculate tr from simulation*/
   transaccion.timestamp = difftime(time(0),startSimulation);

   return transaccion;
}

/*PROCESSO UTENTE:*/
void* utente(void *conf){
   int *id = (int*)conf;                        /*Id processo utente*/
   int i;
   /*Lasso di tempo massimo durata transazione:*/
   int range = configurazione.SO_MAX_TRANS_GEN_NSEC - configurazione.SO_MIN_TRANS_GEN_NSEC;
   int mythr = pthread_self();                /*Pid thread processo utente*/
   int tentativi = 0;                         /*tentativi massimo per creazione di una transazione*/
   int lastUpdate = 0;                        /*questo controlla l'ultima versione del libro mastro*/
   
   printf("Utente #%d creato nel thread %d\n",*id,mythr);

   while(retrylist[*id]<configurazione.SO_RETRY){
      printf("nueva transaccion de %d\n",*id);
      
      lastUpdate = userUpdate(*id,lastUpdate);  /*Aggiorniamo Budget del Processo Utente*/

      if(budgetlist[*id]>=2){                   /*Condizione Budget >= 2*/                                
         
         Transazione transaction;              /*Creiamo una nuova transazione*/
	 transaction = generateTransaction(*id);/*Chiamiamo la func generateTransaction*/
	 
	      
	 /*scelglie un nodo libero a caso*/
	 do{
	    i = rand() % configurazione.SO_NODES_NUM;/*Assegnamo ad i, id random nodo*/
	    retrylist[*id]++;
	    if(retrylist[*id] >= configurazione.SO_RETRY){
	        pthread_exit(NULL);
	        break;
	    }
	 }while(sem_trywait(&semafori[i])<=0);
	 /*prueba de transaccion*/
	 
	 if(retrylist[*id] < configurazione.SO_RETRY){
            sem_wait(&semafori[i]);           /*blocco con il semaforo*/
	    prinTrans(transaction);
	    budgetlist[*id] -= transaction.quantita;
            mailbox[i] = transaction;         /*Inseriamo nel MailBox del nostro Nodo la transazione*/
	    retrylist[*id] = 0;
	 }else{
	    pthread_exit(NULL);
	    printf("l'utente %d ha superato la cuantita di tentativi\n",*id);
	 }
	 
      }else{
         retrylist[*id]++;
      }

      /*usleep((rand() % (range + 1)) + configurazione.SO_MIN_TRANS_GEN_NSEC);/*Tempo di Attesa Random della trasazione*/
      randomSleep(configurazione.SO_MIN_TRANS_GEN_NSEC,configurazione.SO_MAX_TRANS_GEN_NSEC);

      if(retrylist[*id] >= configurazione.SO_RETRY){/*Se raggiunge il n° max di tentativi*/
         printf("utente %d fermato",*id);       /*ferma il procceso*/
      }
   }
}
