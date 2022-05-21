extern userStruct *userList;
extern nodeStruct *nodeList;
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

