1:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=100 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
2:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=10000 main.c -lm -o main
	
3:
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE=10 -DSO_REGISTRY_SIZE=1000 main.c -lm -o main
	
#setting con entrata libera per block size e registry size
custom:
	@echo -n "SO_BLOCK_SIZE: "
	@read block
	@echo -n "SO_REGISTRY_SIZE: "
	@read registry
	gcc -std=c89 -pthread -pedantic -D_GNU_SOURCE -DSO_BLOCK_SIZE = $(block) -DSO_REGISTRY_SIZE = $(registry) main.c -lm -o main
	
