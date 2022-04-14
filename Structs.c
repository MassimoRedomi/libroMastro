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

void prinTrans(Transazione t){
   printf("%f: %d %d %d\n",t.timestamp,t.sender,t.receiver,t.quantita);
}

int randomInt(int min, int max){
     return rand() % max +min;
}

long randomlong(int min, int max){
     return (long)(rand() % max +min);
}

/*si ferma per una quantita random di nano secondi*/
void randomSleep(int min, int max){
    nanosleep((const struct timespec[]){{0,randomlong(min,max)}},NULL);
}
