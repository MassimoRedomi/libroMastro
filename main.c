#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>/*aggiunge i boolean var*/

#include <unistd.h> /*Header per sleep()*/
#include <pthread.h> /*per fare i thread*/

#define clear() printf("\033[H\033[J") /*clear the screen*/

/*strutruttura della configurazione.*/
struct readStruct{
   int SO_USERS_NUM;/*numero di processi utente*/
   int SO_NODES_NUM;/*numero di processi nodo*/
   int SO_BUDGET_INIT;/*budget iniziale di ciascun processo utente*/
   int SO_REWARD;/*la percentuale di reward pagata da ogni utente per il processamento di una transazione*/
   int SO_MIN_TRANS_GEN_NSEC;/*minimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente*/
   int SO_MAX_TRANS_GEN_NSEC;/*massimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente*/
   int SO_RETRY;/*numero massimo di fallimenti consecutivi nella generazione di transazioni dopo cui un processo utente termina*/
   int SO_TP_SIZE;/*numero massimo di transazioni nella transaction pool dei processi nodo*/
   /*int SO_BLOCK_SIZE;/*numero di transazioni contenute in un blocco*/
   int SO_MIN_TRANS_PROC_NSEC;/*minimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo*/
   int SO_MAX_TRANS_PROC_NSEC;/*massimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo*/
   /*int SO_REGISTRY_SIZE;/*numero massimo di blocchi nel libro mastro*/
   int SO_SIM_SEC;/*durata della simulazione*/
   int SO_FRIENDS_NUM;/*solo per la versione full. numero di nodi amici dei processi nodo (solo per la versione full)*/
   int SO_HOPS;/*solo per la versione full. numero massimo di inoltri di una transazione verso nodi amici prima che il master creai un nuovo nodo*/ 
}configurazione;

struct Transazione{
   time_t timestamp;
   int sender;
   int receiver;
   int quantita;
   int reward;
};

struct Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];
int libroCounter=0;

int *listUtenti;
void* utente(void* conf){
   int budget = configurazione.SO_BUDGET_INIT;
   int range = configurazione.SO_MAX_TRANS_GEN_NSEC - configurazione.SO_MIN_TRANS_GEN_NSEC;
   int *id = (int *)conf;
   int mythr = pthread_self();
   int tentativi = 0;
   listUtenti[*id] = budget;
   printf("Utente #%d creato nel thread %d\n",*id,mythr);
   while(tentativi<configurazione.SO_RETRY){
      if(budget>2){
         /*qui va la struttura della transazione*/

      }else{
         tentativi++;
      }
      /*nanosleep((rand() % (range + 1)) + configurazione.SO_MIN_TRANS_GEN_NSEC);*/
      tentativi++;
   }
}

int *semafori;
struct Transazione *mailbox;
void* nodo(void* conf){
   int i;
   int counterBlock=0;/*contatore della quantita di transazioni nel blocco*/
   int counterPool=0;/*contatore della quantita di transazioni nella pool*/
   int range = configurazione.SO_MAX_TRANS_PROC_NSEC - configurazione.SO_MIN_TRANS_PROC_NSEC;
   struct Transazione blocco[SO_BLOCK_SIZE];
   struct Transazione pool[configurazione.SO_TP_SIZE];
   int mythr; 
   int *id = (int *)conf;
   semafori[*id]=0;
   mythr = pthread_self();
   printf("Nodo #%d creato nel thread %d\n",*id,mythr);
   while(semafori[*id]!=-1){
      if(semafori[*id]==1){
         /*scrivo la nuova transazione nel blocco e nella pool*/
         pool[counterPool]=mailbox[*id];
	 blocco[counterBlock]=mailbox[*id];
	 
	 /*incremento i contatori di posizione di pool e block*/
	 counterBlock++;
	 counterPool++;
	 
	 if(counterBlock == SO_BLOCK_SIZE - 1){
	    /*si aggiunge una nuova transazione come chiusura del blocco*/
	    for(i=0;i< SO_BLOCK_SIZE;i++){
	       libroMastro[(libroCounter * SO_BLOCK_SIZE) + i] = blocco[i];
	    }
	    /*si spostano i contatori*/
	    counterBlock=0;
	    libroCounter++;
	 }
	 
      }
      if(counterPool>= configurazione.SO_TP_SIZE){
         semafori[*id]=-1;/*il nodo si dichiara morto nel semaforo perche la pool è piena*/
      }else{
         semafori[*id]=0;/*stabilisco il semaforo come di nuovo disponibile*/
      }
   }
}

/*Un picollo metodo che fa un fgets(con gli stessi parametri e lo 
ritorna come un valore intero*/
int readAndInt(char *str, int n, FILE *stream){
   fgets(str,n,stream);
   printf("%d\n",atoi(str));/*manual debug*/
   return atoi(str);
}
/*funzione che cerca la maniera di leggere il config file.
/*metodo basato in codice di stackoverflow per leggere file come
una unica struttura.*/
void readconf(char fileName[]){
   /*secondo lo std c89 tutte le variabile devono 
   essere dichiarate prima del primo codice */
   struct readStruct lec;
   FILE *file= fopen(fileName, "r");

   if(!file){
      printf("non si trova il config file.\n");
      exit(EXIT_FAILURE);
   }else{
      char line[20];/*str per prendere le righe*/

      /*inserisco le variabili riga a riga alla struttura.*/
      configurazione.SO_USERS_NUM = readAndInt(line,10,file);
      configurazione.SO_NODES_NUM = readAndInt(line,10,file);
      configurazione.SO_BUDGET_INIT = readAndInt(line,10,file);
      configurazione.SO_REWARD = readAndInt(line,10,file);
      configurazione.SO_MIN_TRANS_GEN_NSEC = readAndInt(line,10,file);
      configurazione.SO_MAX_TRANS_GEN_NSEC = readAndInt(line,10,file);
      configurazione.SO_RETRY = readAndInt(line,10,file);
      configurazione.SO_TP_SIZE = readAndInt(line,10,file);
      configurazione.SO_MIN_TRANS_PROC_NSEC = readAndInt(line,10,file);
      configurazione.SO_MAX_TRANS_PROC_NSEC = readAndInt(line,10,file);
      /*configurazione.SO_REGISTRY_SIZE = readAndInt(line,10,file);*/
      configurazione.SO_SIM_SEC = readAndInt(line,10,file);
      configurazione.SO_FRIENDS_NUM = readAndInt(line,10,file);
      configurazione.SO_HOPS = readAndInt(line,10,file);
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
   clear();

}

int main(int argc,char *argv[]){
   int i;
   pthread_t tid;
   if(argc<2){
      printf("si aspettava un file con la configurazione o il commando 'manual'.\n");
      exit(EXIT_FAILURE);
   }else if(argc>2){
      printf("troppi argomenti.\n");
      exit(EXIT_FAILURE);
   }else{
      /*in caso di voler inserire i valori a mano*/
      if( strcmp(argv[1],"mano")==0 || strcmp(argv[1],"manual")==0 ){
         writeConf();
      }else{
         readconf(argv[1]);/*lettura del file*/
      }
      
      /*now that we have all the variables we can start the process
      master*/

      /*libroMastro=malloc(configurazione.SO_BLOCK_SIZE * configurazione.SO_REGISTRY_SIZE * (4 * sizeof(int)) * sizeof(time_t));*/
      /*generatore dei nodi*/
      semafori=malloc(configurazione.SO_NODES_NUM * sizeof(int));
      mailbox=malloc(configurazione.SO_NODES_NUM * (4 * sizeof(int)) * sizeof(time_t));
      for(i=0;i<configurazione.SO_NODES_NUM;i++){
         pthread_create(&tid,NULL,nodo,(void *)&i);
      }
      /*generatore dei utenti*/
      listUtenti=malloc(configurazione.SO_USERS_NUM * sizeof(int));
      for(i=0;i<configurazione.SO_USERS_NUM;i++){
         pthread_create(&tid,NULL,utente,(void *)&i);
      }
      sleep(configurazione.SO_SIM_SEC);

   }
   return 0;
}
