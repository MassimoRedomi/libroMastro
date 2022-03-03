#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>/*aggiunge i boolean var*/
#include <math.h>

#define clear() printf("\033[H\033[J")

#include <unistd.h> /*Header file per sleep()*/
#include <pthread.h> /*per fare i thread*/

struct readStruct{
   int SO_USERS_NUM;/*numero di processi utente*/
   int SO_NODES_NUM;/*numero di processi nodo*/
   int SO_BUDGET_INIT;/*budget iniziale di ciascun processo utente*/
   int SO_REWARD;/*la percentuale di reward pagata da ogni utente per il processamento di una transazione*/
   int SO_MIN_TRANS_GEN_NSEC;/*minimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente*/
   int SO_MAX_TRANS_GEN_NSEC;/*massimo valore del tempo che trascorre fra la generazione di una transazione e la seguente da parte di un utente*/
   int SO_RETRY;/*numero massimo di fallimenti consecutivi nella generazione di transazioni dopo cui un processo utente termina*/
   int SO_TP_SIZE;/*numero massimo di transazioni nella transaction pool dei processi nodo*/
   int SO_MIN_TRANS_PROC_NSEC;/*minimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo*/
   int SO_MAX_TRANS_PROC_NSEC;/*massimo valore del tempo simulato(nanosecondi) di processamento di un blocco da parte di un nodo*/
   int SO_REGISTRY_SIZE;
   int SO_SIM_SEC;/*durata della simulazione*/
   int SO_FRIENDS_NUM;/*solo per la versione full. numero di nodi amici dei processi nodo (solo per la versione full)*/
   int SO_HOPS;/*solo per la versione full. numero massimo di inoltri di una transazione verso nodi amici prima che il master creai un nuovo nodo*/ 
}configurazione;

struct Transazione{
   int sender;
   int receiver;
   float quantita;
   float reward;
};

void* utente(void* conf){

}
struct Utente{
   int id;
   float budget;
};

struct Nodo{
   int id;
   /*unsigned Transazione block[];*/
};

/*Un picollo metodo che fa un fgets(con gli stessi parametri e lo 
ritorna come un valore intero*/
int readAndInt(char *str, int n, FILE *stream){
   fgets(str,n,stream);
   printf("%d\n",atoi(str));/*manual debug*/
   return atoi(str);
}
/*funzione che cerca la maniera */
struct readStruct readconf(char fileName[]){
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
      configurazione.SO_REGISTRY_SIZE = readAndInt(line,10,file);
      configurazione.SO_SIM_SEC = readAndInt(line,10,file);
      configurazione.SO_FRIENDS_NUM = readAndInt(line,10,file);
      configurazione.SO_HOPS = readAndInt(line,10,file);
   }
   fclose(file);/*chiusura del file.*/
   return lec;
}

int main(int argc,char *argv[]){
   if(argc<2){
      printf("si aspettava un file con la configurazione.\n");
      exit(EXIT_FAILURE);
   }else if(argc>2){
      printf("troppi argomenti.\n");
      exit(EXIT_FAILURE);
   }else{
      printf("tutto a posto\n");
      readconf(argv[1]);
   }
   return 0;
}
