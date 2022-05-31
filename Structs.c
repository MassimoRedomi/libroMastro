
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

extern Configurazione configurazione;

/*Funzione che cerca la maniera di leggere il config file.*/
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
        fscanf(file,"%d",&configurazione.SO_USERS_NUM);
        printf("SO_USERS_NUM: %d\n",configurazione.SO_USERS_NUM);
        fscanf(file,"%d",&configurazione.SO_NODES_NUM);
        printf("SO_NODES_NUM: %d\n",configurazione.SO_NODES_NUM);
        fscanf(file,"%d",&configurazione.SO_BUDGET_INIT);
        printf("SO_BUDGET_INIT: %d\n",configurazione.SO_BUDGET_INIT);
        fscanf(file,"%d",&configurazione.SO_REWARD);
        printf("SO_REWARD: %d\n",configurazione.SO_REWARD);
        fscanf(file,"%d",&configurazione.SO_MIN_TRANS_GEN_NSEC);
        printf("SO_MIN_TRANS_GEN_NSEC: %d\n",configurazione.SO_MIN_TRANS_GEN_NSEC);
        fscanf(file,"%d",&configurazione.SO_MAX_TRANS_GEN_NSEC);
        printf("SO_MAX_TRANS_GEN_NSEC: %d\n",configurazione.SO_MAX_TRANS_GEN_NSEC);
        fscanf(file,"%d",&configurazione.SO_RETRY);
        printf("SO_RETRY: %d\n",configurazione.SO_RETRY);
        fscanf(file,"%d",&configurazione.SO_TP_SIZE);
        printf("SO_TP_SIZE: %d\n",configurazione.SO_TP_SIZE);
        fscanf(file,"%d",&configurazione.SO_MIN_TRANS_PROC_NSEC);
        printf("SO_MIN_TRANS_PROC_NSEC: %d\n",configurazione.SO_MIN_TRANS_PROC_NSEC);
        fscanf(file,"%d",&configurazione.SO_MAX_TRANS_PROC_NSEC);
        printf("SO_MAX_TRANS_PROC_NSEC: %d\n",configurazione.SO_MAX_TRANS_PROC_NSEC);
        fscanf(file,"%d",&configurazione.SO_SIM_SEC);
        printf("SO_SIM_SEC: %d\n",configurazione.SO_SIM_SEC);
        fscanf(file,"%d",&configurazione.SO_FRIENDS_NUM);
        printf("SO_FRIENDS_NUM: %d\n",configurazione.SO_FRIENDS_NUM);
        fscanf(file,"%d",&configurazione.SO_HOPS);
        printf("SO_HOPS: %d\n",configurazione.SO_HOPS);
    }
    fclose(file);/*chiusura del file.*/
}

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
    
}

/*struttura della configurazione.*/
typedef struct Transazione{
    double timestamp;/*Quando viene effettuata la transazione.*/
    int sender;      /*Utente che ha generato la transazione.*/
    int receiver;    /*Utente destinatario de la somma.*/
    int quantita;    /*Quantita di denaro inviata.*/
    int reward;      /*denaro dal sender al nodo che processa la transazione.*/
	
}Transazione;

void prinTrans(Transazione t){
	printf("%f: %d -> %d: %d\n",t.timestamp,t.sender,t.receiver,t.quantita);
}


int randomInt(int min, int max){
	return rand() % max +min;
}
    
long randomlong(int min, int max){
	return 0L + (rand() % max +min);
}

/*si ferma per una quantita random di nano secondi*/
void randomSleep(int min, int max){
    struct timespec sec,nano = {0,randomlong(min,max)};
    nanosleep(&sec,&nano);
}

