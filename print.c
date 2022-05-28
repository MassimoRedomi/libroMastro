/*macros per il print*/
#define clear() printf("\033[H\033[J") /*clear the screen*/
#define MAX(x,y) ((x>y)?x:y) /*max between to parameters*/
#define MIN(z,w) ((z<w)?z:w) /*min between to parameters*/
#define boolString(b) ((b) ? "True":"False")/*make the %b*/

/*variabili degli utenti*/
extern int *budgetlist;
extern bool *checkUser;

/*variabili degli utenti*/
extern int *rewardlist;
extern int *rewardlist;
extern int *poolsizelist;
extern bool *checkNode;

int cmpfunc(const void  *a, const void *b) {
    return(budgetlist[*((int*)b)]-budgetlist[*((int*)a)]);
}
int * sort(){
    int dim=configurazione.SO_USERS_NUM;
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
    int dim=MIN(MAX(configurazione.SO_USERS_NUM,configurazione.SO_NODES_NUM), nstamp);
    pa=sort();
    printf("\n\n");
    printf("------------------------------------------------------------------------\n");
    printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  Status ||\n");
    printf("||===============================|##|=================================||\n");
    
    /*Stampa risultati*/
    for(i=0; i<MAX(configurazione.SO_USERS_NUM,configurazione.SO_NODES_NUM); i++){
        if(i<configurazione.SO_USERS_NUM){
            sommaBudget += budgetlist[*(pa+i)];
            checkUser[*(pa+i)] ? activeUsers++ : inactiveUsers++;
            if(i<dim){
                printf("||%10d|%10d|%9s|#",pa[i],budgetlist[*(pa+i)], boolString(checkUser[*(pa+i)]));
            }
        }else if(i<dim){
            printf("||          |          |         |#");
        }


        if(i < configurazione.SO_NODES_NUM){
            sommaRewards+=rewardlist[i];
            checkNode[i] ? activeNodes++ : inactiveNodes++;
            if(i<dim){
                printf("#|%11d|%11d|%9s||\n", i, rewardlist[i],boolString(checkNode[i]));
            }

        }else if(i<dim){
            printf("#|           |           |         ||\n");
        }
    }

    printf("------------------------------------------------------------------------\n");
    printf("||  Active Users  |  Tot Budget  |##|  Active Nodes  |   Tot Rewards  ||\n");
    printf("||%16d|%14d|##|%16d|%16d||\n",activeUsers,sommaBudget,activeNodes, sommaRewards);
    printf("\n");
    
    return activeUsers>0;
}

void finalprint(){
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
    int dim = MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM);
 
    printf("\n\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  p_size | Status ||\n");
    printf("||===============================|##|==========================================||\n");
    for(i=0; i< dim; i++){
        if( i < configurazione.SO_USERS_NUM){
            sommaBudget += budgetlist[i];

            checkUser[i] ? activeUsers++ : inactiveUsers++;

            printf("||%10d|%10d|%9s|#",i,budgetlist[i], boolString(checkUser[i]));
        }else{
            printf("||          |          |         |#");
        }

        if(i< configurazione.SO_NODES_NUM){
            sommaRewards+=rewardlist[i];

            checkNode[i] ? activeNodes++ : inactiveNodes++;
            printf("#|%11d|%11d|%9d|%8s||\n", i, rewardlist[i],poolsizelist[i],boolString(checkNode[i]));
        }else{
            printf("#|           |           |         |        ||\n");
        }
    }
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

    /*motivo del termine*/
    if(activeUsers==0){
        printf("Motivo di chiusura:tutti gli utenti sono disattivati.\n");
    }else if(libroCounter >= SO_REGISTRY_SIZE){
        printf("Motivo di chiusura: libroMastro pieno.\n");
    }else{
        printf("Simulazione finita perfettamente.\n");
    }
}
