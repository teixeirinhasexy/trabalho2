CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic

all: rotas2024

rotas2024: main.o aeroporto.o companhia_aerea.o util.o
	$(CC) $(CFLAGS) -o rotas2024 main.o aeroporto.o companhia_aerea.o util.o -lm

main.o: main.c aeroporto.h companhia_aerea.h util.h
	$(CC) $(CFLAGS) -c main.c

aeroporto.o: aeroporto.c aeroporto.h
	$(CC) $(CFLAGS) -c aeroporto.c

companhia_aerea.o: companhia_aerea.c companhia_aerea.h aeroporto.h
	$(CC) $(CFLAGS) -c companhia_aerea.c

util.o: util.c util.h aeroporto.h companhia_aerea.h
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f *.o rotas2024
