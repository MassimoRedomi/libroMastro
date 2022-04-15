#  lista de preguntas para el profesor
## condizione di compilazione
[ ]  si puo aggiungere piu orgomenti alla compilazione?
[ ]  si mettono le variabili nella compilazione come flags?
[ ] c'è bisogno di fare piu cose nel Makefile?
## Documentazione del codice
[ ] si puo usare org-mode o Markdown o unicamente si puo comentare il codice?
## ingresso di variabili
[x] le variabili si mettono come lettura di file o scritto a mano
		avisar que ya tengo igualmente escrito ambos procedimientos.
[x] si usa int o float per il budget e quantita di transazione?

#  [Main](main.md)
[x] lectura cada un segundo
[ ] comunicar el main con hilos(30 pts)
[x]  sync time al publico
[x] asignar tid individual para cada hilo. Probablemente es el motivo de que los hilos no vayan bien en la transaccion.
[ ] separar la lista de threads en 2: utenti e nodi.
[ ] eliminar el puntero del pthread_create

# [UTENTE](User.md)
[x] construir transacción 
[x] busqueda del nodo libre
[x] agregar delay nanoseconds aleatorio al final de utente 
[ ] cambiar el sistema de tentativi, saltar una alerta cuando llega al numero de intentos maximo.
[x] hacer transacciones en el mismo loop que se actualiza
[x] funcion de aggiornamento con libro mastro
[x] hacer transaccion en una funcion
[ ] sacar el ID en base a una busqueda en la lista de hilos del main

# [NODO](Node.md)
[x] change the semaphores for real semaphores.
[x]cambiar tiempo de espera del nodo. No de una transaccion a otra, sino que entre un bloque y otro.
[x] cambiar la suma de rewards a una lista externa para permitir que lo lea el proceso maestro
[ ] sacar el ID en base a una busqueda en la lista de hilos del main