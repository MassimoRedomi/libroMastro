#include <stdio.h>  /*Standard input-output header*/
#include <stdlib.h> /*Libreria Standard*/  
#include <time.h>   /*Acquisizione e manipolazione del tempo*/
#include <stdbool.h>/*Aggiunge i boolean var*/
#include <string.h>/*Standar library for string type*/

#include <unistd.h>      /*Header per sleep()*/
#include <pthread.h>     /*Creazione/Modifica thread*/
#include <semaphore.h>   /*Aggiunge i semafori*/

#include "User.c"
Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
int libroCounter=0;/*Counter controlla la quantitta di blocchi*/
sem_t libroluck;/*Luchetto per accedere solo a un nodo alla volta*/

/*variabili condivise tra diversi thread.*/
userStruct *userList;
nodeStruct *nodeList;

pthread_t *utenti_id;   /*lista id di processi utenti*/
pthread_t *nodi_id;     /*lista id di processi nodi  */
Configurazione configurazione;
time_t startSimulation;

/*legge le transazioni e gli scrive in un array di transazioni per scriverle 
dopo nel libro mastro.*/
int leggeLibroDiTransazioni(char fileName[], Transazione programmate[100]){
    int i = 0;
    FILE *file = fopen(fileName,"r");
    if(!file){
        printf("non si trova il libro di transazioni programmate.\n");
    }else{
        /*legge riga a riga fino alla fine(EOF), mettendo tutti le variabili nell'array 
        delle transazioni programmate.*/
        while(fscanf(file,"%lf %d %d %d",&programmate[i].timestamp,&programmate[i].sender,&programmate[i].receiver,&programmate[i].quantita) != EOF && i<100){
            programmate[i].reward = programmate[i].quantita * configurazione.SO_REWARD / 100;
            i++;
        }
    }
    return i;
}


/*segnale che forza una transazione di un'utente.*/
void segnale(Transazione programmato){
    nodeList[nodoLibero(programmato.sender)].mailbox = programmato;/*assegno la transazione in un mailbox*/

    userList[programmato.sender].budget -= programmato.quantita;
    printf("Segnale ->");
    prinTrans(programmato);
}
int cmpfunc(const void *a, const void *b) {
    return(userList[*((int*)b)].budget-userList[*((int*)a)].budget);
}
int * sort(){
    int dim=MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM);
    int *r=malloc(sizeof(int)*dim);
    int i;

    for(i=0; i<dim; i++)
        r[i]=i;
    
    qsort(r, dim, sizeof(int), cmpfunc);

    return r;
}

bool printStatus(int nstamp){

    /*User var*/
    int activeUsers=0;
    int inactiveUsers=0;
    int sommaBudget=0;
    bool ActiveU;

    /*Node var*/
    int activeNodes=0;
    int inactiveNodes=0;
    int sommaRewards=0;
    bool ActiveN;

    /*Share var*/
    int i=0;
    int *pa;
    int dim=MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM);

    pa=sort();
    if(nstamp==dim){
        /*Attributi*/
        printf("\n\n");
        printf("---------------------------------------------------------------------------------\n");
        printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  Status | P_Size ||\n");
        printf("||===============================|##|==========================================||\n");

    }else{
        /*Attributi*/
        printf("\n\n");
        printf("------------------------------------------------------------------------\n");
        printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  Status ||\n");
        printf("||===============================|##|=================================||\n");
    }

    
    /*Stampa risultati*/
    for(i=0; i<MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM); i++){

        if(i<configurazione.SO_USERS_NUM){
            sommaBudget += userList[*(pa+i)].budget;
            userList[*(pa+i)].stato?activeUsers++:inactiveUsers++;
            if(i<nstamp/2){
                printf("||%10d|%10d|%9s|#",*(pa+i),userList[*(pa+i)].budget, B(userList[*(pa+i)].stato));
            }else if(i>(nstamp/2)-1 && i<nstamp){
                printf("||%10d|%10d|%9s|#",*((pa+dim)-(i-nstamp/2)),userList[*(pa+dim)-(i-nstamp/2)].budget, B(userList[*(pa+dim)-(i-nstamp/2)].stato));
            }
        }else if(i<nstamp){
            printf("||          |          |         |#\n");
        }

        if(i< configurazione.SO_NODES_NUM){
            sommaRewards+=nodeList[i].reward;
            nodeList[i].stato?activeNodes++:inactiveNodes++;
            if(i<nstamp){
                 printf("#|%11d|%11d|%9s|", i, nodeList[i].reward,B(nodeList[i].stato));
                 if(nstamp==dim){
                     printf("%8d||\n",nodeList[i].poolsize);
                 }else{
                     printf("|\n");
                 }
            }
        }else if(i<nstamp){
            if(nstamp==dim){
                printf("#|           |           |         |        ||\n");
            }else{
                printf("#|           |           |         ||\n");
            }
        }
    }
    if(nstamp==dim){
        printf("---------------------------------------------------------------------------------\n");
        printf("||   Active Users  |   Inactive Users  |##|   Active Nodes  |  Inactive Nodes  ||\n");
        printf("||%17d|%19d|##|%17d|%18d||\n",activeUsers,inactiveUsers, activeNodes, inactiveNodes);
        printf("||-----------------------------------------------------------------------------||\n");
        printf("||    Tot Rewards  |%59d||\n",sommaRewards);
        printf("||-----------------------------------------------------------------------------||\n");
        printf("||    Tot Budgets  |%59d||\n",sommaBudget);
        printf("||-----------------------------------------------------------------------------||\n");
        printf("||    Tot Block    |%59d||\n", libroCounter);
        printf("---------------------------------------------------------------------------------\n");


    }else{
        printf("------------------------------------------------------------------------\n");
        printf("||  Active Users  |  Tot Budget  |##|  Active Nodes  |   Tot Rewards  ||\n");
        printf("||%16d|%14d|##|%16d|%16d||\n",activeUsers,sommaBudget,activeNodes, sommaRewards);
        printf("\n");
    }
    

    return activeUsers!=0;
}

int main(int argc,char *argv[]){
    int i;
	float now;
    bool test;
    int counterAttivi;

    /*variabili delle transazioni programmate*/
    int programmateCounter;
    bool *programmateChecklist;
    Transazione programmate[100];


    srand((unsigned) time(0)); /*aleatorio*/

    if(argc<2){
	    printf("si aspettava un file con la configurazione o il commando 'manual'.\n");
        exit(EXIT_FAILURE);
    }else if(argc>3){
		printf("troppi argomenti.\n");
		exit(EXIT_FAILURE);
    }else{
		/*in caso di voler inserire i valori a mano*/
		if( strcmp(argv[1],"mano")==0 || strcmp(argv[1],"manual")==0 ){
			writeConf();
		}else{
	    	 readconf(argv[1]);/*lettura del file*/
        }
    
        /*lettura di transazioni programmate*/
        if(argc >= 3){
            programmateCounter = leggeLibroDiTransazioni(argv[2], programmate);
            programmateChecklist = malloc(programmateCounter * sizeof(bool));
            for(i=0; i < programmateCounter; i++){
                programmateChecklist[i] = true;
            }
        }else{
            programmateCounter = 0;
        }
 
        /*now that we have all the variables we can start the process
        master*/
    
        startSimulation = time(0);/* el tiempo de ahora*/
        sem_init(&libroluck,0,1);/*inizia il semaforo del libromastro*/
    
        /*generatore dei nodi*/
        /*somma di tutte le variabili dei nodi*/
        nodeList= malloc(configurazione.SO_NODES_NUM *((6*sizeof(int))+sizeof(double) + sizeof(sem_t)+ sizeof(bool)));
        nodi_id = malloc(configurazione.SO_NODES_NUM * sizeof(pthread_t));
        for(i=0;i<configurazione.SO_NODES_NUM;i++){
			pthread_create(&nodi_id[i],NULL,nodo,NULL);
        }

        /*generatore dei utenti*/
        userList  = malloc(configurazione.SO_USERS_NUM * (2 * sizeof(int) + sizeof(bool)));
        utenti_id = malloc(configurazione.SO_USERS_NUM * sizeof(pthread_t));
        for(i=0;i<configurazione.SO_USERS_NUM;i++){
			pthread_create(&utenti_id[i],NULL,utente,NULL);
        }
    
		/*now start the master process*/
		now = difftime(time(0), startSimulation);

		while(now < configurazione.SO_SIM_SEC){

			sleep(1);
            clear();
    
			/*show last update*/
	    	printf("ultimo aggiornamento: %.2f/%d\n",difftime(time(0),startSimulation),configurazione.SO_SIM_SEC);

	    	now = difftime(time(0), startSimulation);
            
            if(libroCounter >= SO_REGISTRY_SIZE){
                printf("%f: libro mastro pieno\n",now);
                break;
            }

            if(!printStatus(40)){
                printf("tutti gli utenti sono disattivati");
                break;
            }

            /* transazioni programmate mancanti*/
            for(i=0; i< programmateCounter; i++){
                if(programmate[i].timestamp <= now && programmateChecklist[i]){
                    segnale(programmate[i]);
                    programmateChecklist[i] = false;
                }
            }


        }

        clear();
        printStatus(MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM));
    
	}
	return 0;
}
