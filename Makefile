all:
	gcc -std=c89 -D_GNU_SOURCE -pedantic main.c -lm -o main
testing:
	gcc test.c -lm -o test
	./test
