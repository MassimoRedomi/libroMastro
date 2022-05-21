# DOMANDE PER IL PROFESSORE
- [ ] è valido limitare la grandezza delle transazioni da 2 a budget/2, per prolongare la vita dell'utente attivo?
## Condizione di compilazione
- [ ]  si puo aggiungere piu orgomenti alla compilazione?

- [ ]  si mettono le variabili nella compilazione come flags?

- [ ] c'è bisogno di fare piu cose nel Makefile?
## Documentazione del codice
- [ ] si puo usare org-mode o Markdown o unicamente si puo comentare il codice?

## Ingresso Variabili

- [x] le variabili si mettono come lettura di file o scritto a mano, avvisare che i due procedimenti sono gia creati.

- [x] si usa int o float per il budget e quantita di transazione?

#  [Main](main.md)

- [x] Lettura ogni istante.

- [ ] comunicar el main con hilos(30 pts).

- [ ] revivir procesos zombies que vienen llamados por otros nodos(30 pts).

- [x] sync time al publico.

- [x] asignar tid individual para cada hilo.

- [x] separar la lista de threads en 2: utenti e nodi.

- [x] eliminar el puntero del pthread_create.

- [x] condicionar la continuidad de la simulacion
   - [x] Todos los usuarios estan desactivados.
   - [x] El libro Mastro llego a su limite de bloques.

- [x] limpiar la cantidad de usuarios en pantalla
    idea:(espero opiniones)
    se sono meno di 300 utenti: mostra tutto.
    Altrimenti: mostra solo gli utenti che hanno meno di di SO_RETRY tentativi.

- [x] print final
    - [x] motivo del final de la ejecucion.
    - [x] bilancio di ogni processo utente , compresi quelli che sono terminati.
    - [x] bilancio di ogni processo nodo.
    - [x] numero di processi utente terminati prematuramente
    - [x] numero di blocchi nel libro mastro
    - [x] per ogni processo nodo, numero di transazioni ancora presenti nella transaction pool
 - [ ] trheads
    - [ ] establecer esquema de roundRobin
    - [ ] 

# [UTENTE](User.md)
- [x] construir transacción.

- [x] busqueda del nodo libre.

- [x] agregar delay nanoseconds aleatorio al final de utente.

- [x] cambiar el sistema de tentativi, saltar una alerta cuando llega al numero de intentos maximo.

- [x] hacer transacciones en el mismo loop que se actualiza.

- [x] funcion de aggiornamento con libro mastro.

- [x] hacer transaccion en una funcion.

- [x] sacar el ID en base a una busqueda en la lista de hilos del main.

- [x] limpiar los outputs

- [x] quitar la somma debug

- [x] revisar porqué los nuemeros se disparan con el tiempo(en alguna parte se esta sumando mas de una vez) motivo: hacia transacciones muy grandes.

# [NODO](Node.md)

- [x] change the semaphores for real semaphores.

- [x] cambiar tiempo de espera del nodo. No de una transaccion a otra, sino que entre un bloque y otro.

- [x] cambiar la suma de rewards a una lista externa para permitir que lo lea el proceso maestro.

- [x] sacar el ID en base a una busqueda en la lista de hilos del main.

- [ ] fragmentar la funcion principal.

- [x] limpiar los outputs

- [ ] crear lista de nodos amigos a los que se les puede  enviar una transaccion cuando la pool de nuestra transaccion este ocupada(30 pts).

- [ ] enviar transaccion al nodo amigo(30 pts).

# [STRUCTS](Structs.md)
- [ ] riparare il warning di randomSleep
