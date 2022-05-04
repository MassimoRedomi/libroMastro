# Compilazione

La compilazione avviene tramite MAKEFILE:
### configurazione 1:  
SO_BLOCK_SIZE = 100
SO_REGISTRY_SIZE = 1000
```makefile Makefile
1:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=100 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
```
con debug
```makefile Makefile
g1:
	gcc -std=c89 -g -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=100 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
```


### configurazione 2:
SO_BLOCK_SIZE = 10
SO_REGISTRY_SIZE = 10000
```makefile Makefile
2:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=10000 main.c -lm -o main
	
```

### configurazione 3:
SO_BLOCK_SIZE = 10
SO_REGISTRY_SIZE = 1000
```makefile Makefile
3:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
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
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE = $(block) -DSO_REGISTRY_SIZE = $(registry) main.c -lm -o main
	
```


## significato di ogni flag
## std=c89
set the language standard C89.

## pthread
Setting the binary for thread processing.

## pedantic

## D_GNU_SOURCE
enables GNU extensions to the C and OS standards supported by the 
GNU C library.

## SO_BLOCK_SIZE
The size of the block in the simulation.


## SO_REGISTRY_SIZE
The max size of the libro mastro.

### -lm
Compiles against the shared library.
