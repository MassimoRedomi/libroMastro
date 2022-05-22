
# Prints

## memoria condivise
```c print.c
/*variabili degli utenti*/
extern int *budgetlist;
extern bool *checkUser;

/*variabili degli utenti*/
extern int *rewardlist;
extern int *rewardlist;
extern int *poolsizelist;
extern bool *checkNode;

```
## Compare Function
Metodo che compara due valori e restituisce un numero positivo se b è piu
grande di a e negativo se b è piu piccolo di a:
```c print.c
int cmpfunc(const void  *a, const void *b) {
    return(budgetlist[*((int*)b)]-budgetlist[*((int*)a)]);
}
```


## Sort risults
Metodo di ordinamento del dei processi in modo decrescente
(dal piu grande al piu piccolo).

```c print.c
int * sort(){
    int dim=MAX(configurazione.SO_USERS_NUM, configurazione.SO_NODES_NUM);
    int *r=malloc(sizeof(int)*dim);
    int i;
    for(i=0; i<dim; i++)
        r[i]=i;
    
    qsort(r, dim, sizeof(int), cmpfunc);
    return r;
}
```


## PrintStatus Nodes and Users
Questo metodo non solo mostra lo stato di tutti gli
utenti e nodi, ritorna anche una variabile boolean per identificare
se ci sono ancora utenti disponibili.

```c print.c
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
    int dim=MIN(configurazione.SO_USERS_NUM, nstamp);
    pa=sort();
    printf("\n\n");
    printf("------------------------------------------------------------------------\n");
    printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  Status ||\n");
    printf("||===============================|##|=================================||\n");
    
    /*Stampa risultati*/
    for(i=0; i<configurazione.SO_USERS_NUM; i++){
        sommaBudget += budgetlist[*(pa+i)];

        checkUser[*(pa+i)] ? activeUsers++ : inactiveUsers++;

        if(i<dim){
            printf("||%10d|%10d|%9s|#",*(pa+i),budgetlist[*(pa+i)], boolString(checkUser[*(pa+i)]));
        }

        if(i< configurazione.SO_NODES_NUM){
            sommaRewards+=rewardlist[i];
            checkNode[i] ? activeNodes++ : inactiveNodes++;
            printf("#|%11d|%11d|%9s||\n", i, rewardlist[i],boolString(checkNode[i]));
        }else if(i<dim){
            printf("#|           |           |         ||\n");
        }
    }

    printf("------------------------------------------------------------------------\n");
    printf("||  Active Users  |  Tot Budget  |##|  Active Nodes  |   Tot Rewards  ||\n");
    printf("||%16d|%14d|##|%16d|%16d||\n",activeUsers,sommaBudget,activeNodes, sommaRewards);
    printf("\n");
    
    return activeUsers!=0;
}
```

## final print
```c print.c
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
 
    printf("\n\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("||  User_ID |  Budget  |  Status |##|  Node_ID  |  Rewards  |  p_size | Status ||\n");
    printf("||===============================|##|==========================================||\n");
    for(i=0; i< configurazione.SO_USERS_NUM; i++){
        sommaBudget += budgetlist[i];

        checkUser[i] ? activeUsers++ : inactiveUsers++;

        printf("||%10d|%10d|%9s|#",i,budgetlist[i], boolString(checkUser[i]));

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
}
```
