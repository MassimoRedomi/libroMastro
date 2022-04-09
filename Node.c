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

/*struttura della configurazione.*/
typedef struct Transazione{
   double timestamp;/*Quando viene effettuata la transazione.*/
   int sender;      /*Utente che ha generato la transazione.*/
   int receiver;    /*Utente destinatario de la somma.*/
   int quantita;    /*Quantita di denaro inviata.*/
   int reward;      /*denaro dal sender al nodo che processa la transazione.*/
}Transazione;

extern Configurazione configurazione;

extern Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
extern int libroCounter;/*Counter controlla la quantitta di blocchi*/
extern sem_t libroluck;/*luchetto per accedere solo un nodo alla volta*/

/*variabili condivise tra diversi thread.*/
extern int *listUtenti;     /*thread id di ogni utente*/
extern int *budgetlist;     /*un registro del budget di ogni utente*/
extern int *rewardlist;     /*un registro publico del reward totale di ogni nodo.*/
extern sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
extern Transazione *mailbox;/*struttura per condividere */
extern Configurazione configurazione;
extern time_t startSimulation;

void* nodo(void* conf){
   /*creazioni dei dati del nodo*/
   int i;
   int counterBlock=0;/*contatore della quantita di transazioni nel blocco*/
   int counterPool=0;/*contatore della quantita di transazioni nella pool*/
   int sommaBlocco=0;/*somma delle transazioni del blocco atuale*/
   int range = configurazione.SO_MAX_TRANS_PROC_NSEC - configurazione.SO_MIN_TRANS_PROC_NSEC;
   Transazione blocco[SO_BLOCK_SIZE];
   Transazione pool[configurazione.SO_TP_SIZE];
   Transazione finalReward;
   int mythr; 
   int *id = (int *)conf;
   int semvalue;/*valore del semaforo*/
   sem_init(&semafori[*id],0,0);/*inizializa il semaforo in 0*/
   rewardlist[*id]=0;
   mythr = pthread_self();
   printf("Nodo #%d creato nel thread %d\n",*id,mythr);
   
   /*inizio del funzionamento*/
   while(counterPool < configurazione.SO_TP_SIZE){
   
      /*aggiorno il valore del semaforo*/
      sem_getvalue(&semafori[*id],&semvalue);
      if(semvalue!=0){
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
	 }
	 
      }
      if(counterPool < configurazione.SO_TP_SIZE){
         sem_post(&semafori[*id]);/*stabilisco il semaforo come di nuovo disponibile*/
      }
   }
}
