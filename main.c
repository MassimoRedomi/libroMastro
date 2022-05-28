#include <stdio.h>  /*Standard input-output header*/
#include <stdlib.h> /*Libreria Standard*/  
#include <time.h>   /*Acquisizione e manipolazione del tempo*/
#include <stdbool.h>/*Aggiunge i boolean var*/
#include <string.h>/*Standar library for string type*/

#include <unistd.h>      /*Header per sleep()*/
#include <pthread.h>     /*Creazione/Modifica thread*/
#include <semaphore.h>   /*Aggiunge i semafori*/

#include "User.c"
#include "print.c"
Transazione libroMastro[SO_REGISTRY_SIZE * SO_BLOCK_SIZE];/*libro mastro dove si scrivono tutte le transazioni.*/
int libroCounter=0;/*Counter controlla la quantitta di blocchi*/
sem_t libroluck;/*Luchetto per accedere solo a un nodo alla volta*/
sem_t mainSem;

/*variabili condivise tra diversi thread.*/
int *budgetlist;     /*un registro del budget di ogni utente*/
bool *checkUser;     /*mostra lo stato di ogni utente.*/
sem_t UserStartSem;  /*un semaforo dedicato unicamente per iniziare processi utente*/

int *rewardlist;     /*un registro pubblico del reward totale di ogni nodo.*/
int *poolsizelist;   /*un registro del dimensioni occupate pool transaction*/
sem_t *semafori;     /*semafori per accedere/bloccare un nodo*/
sem_t NodeStartSem;  /*un semaforo dedicato unicamente per iniziare processi nodo*/
Transazione *mailbox;/*struttura per condividere */
bool *checkNode;     /*lista che mostra i nodi che sono attivi.*/

Transazione mainMailbox;

time_t startSimulation;
pthread_t *utenti_id;     /*lista id di processi utenti*/
pthread_t *nodi_id;     /*lista id di processi nodi  */
Configurazione configurazione;


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
    mailbox[nodoLibero(programmato.sender)] = programmato;/*assegno la transazione in un mailbox*/

    budgetlist[programmato.sender] -= programmato.quantita;
    printf("Segnale ->");
    prinTrans(programmato);
}

void* gestore(){
    int i;
    int semvalue;

    while(difftime(time(0), startSimulation) < configurazione.SO_SIM_SEC){
        sem_getvalue(&mainSem,&i);
        if(semvalue <=0){
            sem_post(&mainSem);
            /*array temporali*/
            int *tempPoolsize= calloc(configurazione.SO_NODES_NUM+1,sizeof(int));
            int *tempRewardList= calloc(configurazione.SO_NODES_NUM+1,sizeof(int));
            sem_t *tempSemafori = calloc(configurazione.SO_NODES_NUM+1,sizeof(sem_t));
            Transazione *tempmailbox = calloc(configurazione.SO_NODES_NUM+1, ((4*sizeof(int))+sizeof(double)));
            pthread_t *tempThreads = calloc(configurazione.SO_NODES_NUM+1,sizeof(pthread_t));
            bool *tempcheck =  calloc(configurazione.SO_NODES_NUM+1,sizeof(bool));

            /*copia tutti i valori*/
            for(i=0;i<configurazione.SO_NODES_NUM;i++){
                tempPoolsize[i]=poolsizelist[i];
                tempRewardList[i]=rewardlist[i];
                tempSemafori[i] = semafori[i];
                tempmailbox[i] = mailbox[i];
                tempThreads[i] = nodi_id[i];
                tempcheck[i] = checkNode[i];
            }

            /*riimpiaza le liste*/
            poolsizelist=tempPoolsize;
            rewardlist =tempRewardList;
            semafori =tempSemafori;
            mailbox = tempmailbox;
            nodi_id = tempThreads;
            checkNode = tempcheck;
            /*
            free(tempPoolsize);
            free(tempRewardList);
            free(tempSemafori);
            free(tempmailbox);
            free(tempThreads);
            free(tempcheck);
            */

            /*inizia il nuovo trhead*/
            pthread_create(&nodi_id[configurazione.SO_NODES_NUM],NULL,nodo,(void *)configurazione.SO_NODES_NUM);
            sem_wait(&NodeStartSem);

            mailbox[configurazione.SO_NODES_NUM] = mainMailbox;
            /*printf("nodo %d creato.\n",configurazione.SO_NODES_NUM);*/
            configurazione.SO_NODES_NUM++;
        }
    }
}

int main(int argc,char *argv[]){
    int i;
    float now;
    bool test;
    int semvalue;
    pthread_t thrGestore;


    /*variabili delle transazioni programmate*/
    int programmateCounter;
    bool *programmateChecklist;
    Transazione programmate[100];


    srand(time(0)); /*aleatorio*/

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
        sem_init(&libroluck,configurazione.SO_NODES_NUM,1);/*inizia il semaforo del libromastro*/
        sem_init(&mainSem,configurazione.SO_NODES_NUM+configurazione.SO_USERS_NUM,1);
        startSimulation = time(0);/* el tiempo de inicio*/
    
        /*generatore dei nodi*/
        sem_init(&NodeStartSem,configurazione.SO_NODES_NUM,1);
        poolsizelist=calloc(configurazione.SO_TP_SIZE , sizeof(int));
        rewardlist=calloc(configurazione.SO_NODES_NUM , sizeof(int));
        semafori=calloc(configurazione.SO_NODES_NUM , sizeof(sem_t));
        mailbox=calloc(configurazione.SO_NODES_NUM , ((4 * sizeof(int)) + sizeof(double)));
        nodi_id = calloc(configurazione.SO_NODES_NUM , sizeof(pthread_t));
        checkNode = calloc(configurazione.SO_NODES_NUM , sizeof(bool));
        for(i=0;i<configurazione.SO_NODES_NUM;i++){
            pthread_create(&nodi_id[i],NULL,nodo,(void *)i);
            sem_wait(&NodeStartSem);
        }

        pthread_create(&thrGestore,NULL,gestore,NULL);

        /*generatore dei utenti*/
        sem_init(&UserStartSem,configurazione.SO_USERS_NUM,1);
        budgetlist=calloc(configurazione.SO_USERS_NUM , sizeof(int));
        utenti_id = calloc(configurazione.SO_USERS_NUM , sizeof(pthread_t));
        checkUser = calloc(configurazione.SO_USERS_NUM , sizeof(bool));
        for(i=0;i<configurazione.SO_USERS_NUM;i++){
            pthread_create(&utenti_id[i],NULL,utente,(void *)i);
            sem_wait(&UserStartSem);
        }

        
        now = difftime(time(0), startSimulation);

        while(now < configurazione.SO_SIM_SEC){

            sleep(1);
            clear();

            /*show last update*/
            sem_getvalue(&mainSem,&semvalue);
            printf("ultimo aggiornamento: %.2f/%d\n",difftime(time(0),startSimulation),configurazione.SO_SIM_SEC);

            now = difftime(time(0), startSimulation);

            if(libroCounter > SO_REGISTRY_SIZE){
                printf("%f: libro mastro pieno\n",now);
                break;
            }
            

            test = printStatus(40);
            if(!test){
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
        finalprint();
    
        /*kill all the threads*/
        for(i=0; i<configurazione.SO_NODES_NUM ; i++){
            pthread_cancel(nodi_id[i]);
        }
        for(i=0; i<configurazione.SO_USERS_NUM; i++){
            pthread_cancel(utenti_id[i]);
        }
    }
    return 0;
}
