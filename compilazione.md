# Compilazione

La compilazione avviene tramite MAKEFILE:
### configurazione 1:  
SO_BLOCK_SIZE = 100

SO_REGISTRY_SIZE = 1000

```makefile Makefile
1:
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=100 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
```

### configurazione 2:
SO_BLOCK_SIZE = 10

SO_REGISTRY_SIZE = 10000

```makefile Makefile
2:
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=10000 main.c -lm -o main
```

### configurazione 3:
SO_BLOCK_SIZE = 10

SO_REGISTRY_SIZE = 1000

```makefile Makefile
3:
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
```

### configurazione custom:
Questa configurazione viene pensato per modificare i valori 
```makefile Makefile
#setting con entrata libera per block size e registry size
custom:
	@echo -n "SO_BLOCK_SIZE: "
	@read block
	@echo -n "SO_REGISTRY_SIZE: "
	@read registry
	gcc -std=c89 -pthread -g -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE = $(block) -DSO_REGISTRY_SIZE = $(registry) main.c -lm -o main
	
```


## significato di ogni flag
 - std=c89: Stabilisce il linguagio standard C89.

 - pthread: Stabilisce il binario per processare threads.
 - pedantic

 - D_GNU_SOURCE: Abilita estensioni GNU agli standard C e OS supportati dalla libreria GNU C.
 - SO_BLOCK_SIZE: La grandezza del blocco nella simulzaione.

 - SO_REGISTRY_SIZE: La grandezza massima del libro mastro.

 - lm: Compila con le librerie condivise.


# Esecuzione del programma
Dopo di aver compilato il programma solo ci manca inizializzarlo.
Per questo si puo fare due maniere diverse: passando un file con 
tutta la configurazione, o scriverla manualmente.

## con file di configurazione
Nel caso di inviare un file configuration. si passa come argomento di
esecuzione.
```sh
./main conf1.dat
```

## scritura manuale 
Per scrivere la configurazione a manualmente si deve scrivere come secondo 
argomento la parola "mano" o "manuale".
```sh
./main manual
```


## aggiunge segnali
nel caso delle segnali per forzare certe transazioni, non è obbligatorio per inizializare il programma, ma se si aspetta fare questo si aggiunge un terzo argomento con l'indirizzo del file con tutte le transazioni che si aspettano. 

```sh
./main conf1.dat transactions.dat
```
con il file __transactions.dat__ come il file con tutte le transazioni programmate.
