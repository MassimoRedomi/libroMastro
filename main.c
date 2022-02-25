#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>/*aggiunge i boolean var*/
#include <math.h>

#define clear() printf("\033[H\033[J")

#include <unistd.h> /*Header file per sleep()*/
#include <pthread.h> /*per fare i thread*/

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
