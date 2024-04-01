#ifndef ROTAS_H
#define ROTAS_H

#include "aeroporto.h"

typedef struct HorasVoo {
    int hora_partida_hora;
    int hora_partida_minuto;
    int hora_chegada_hora;
    int hora_chegada_minuto;
} HorasVoo;


typedef struct Voo {
    char codigo[10];
    Aeroporto *partida;
    Aeroporto *chegada;
    HorasVoo horasvoo;
    float distancia;
    struct Voo *prox; // Ponteiro para o próximo voo na lista
} Voo;

// Definindo uma estrutura para representar uma companhia aérea
typedef struct Airline {
    char nome[50];
    Voo *voos; // Lista de voos da companhia aérea
    struct Airline *prox; // Ponteiro para a próxima companhia aérea na lista
} Airline;


void lerVoos(char *nomeArquivo, Airline **listaAirlines, Aeroporto *listaAeroportos);
void libertarAirlines(Airline *listaAirlines);
void libertarVoos(Voo *listaVoos);
void imprimirRotas(Airline *listaAirlines);
void imprimir_voo(Voo *voo);

#endif // ROTAS_H
