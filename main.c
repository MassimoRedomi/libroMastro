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
   int SO_SIM_SESC;/*durata della simulazione*/
   int SO_NUM_FRIENDS;/*solo per la versione full. numero di nodi amici dei processi nodo (solo per la versione full)*/
   int SO_HOPS;/*solo per la versione full. numero massimo di inoltri di una transazione verso nodi amici prima che il master creai un nuovo nodo*/ 
}configurazione;

struct Utente{
   int id;
   float budget;
};

struct Transazione{
   struct Utente sender;
   struct Utente receiver;
   float quantita;
   float reward;
};

struct Nodo{
   int id;
   /*unsigned Transazione block[];*/
};

int main(int argc,char *argv[]){
   if(argc<2){
      printf("si aspettava un file con la configurazione.\n");
      exit(EXIT_FAILURE);
   }else if(argc>2){
      printf("troppi argomenti.\n");
      exit(EXIT_FAILURE);
   }else{
      printf("tutto a posto\n");
   }
   return 0;
}
