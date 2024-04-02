#ifndef ROTAS_H
#define ROTAS_H

#include "aeroporto.h"




typedef struct Voo {
    char codigo[10];
    Aeroporto *partida;
    Aeroporto *chegada;
    char horaPart[6];
    char horaCheg[6];
    float distancia;
} Voo;

typedef struct VooNode {
    Voo *voo;
    struct VooNode *esquerda;
    struct VooNode *direita ;
} VooNode ;


// Definindo uma estrutura para representar uma companhia aérea
typedef struct Airline {
    char nome[50];
} Airline;

typedef struct AirlineNode {
    VooNode *raizVoo;
    Airline *airline;
    struct AirlineNode *esquerda;
    struct AirlineNode *direita;
} AirlineNode;

#endif // ROTAS_H


void lerVoosAirlines(char *nomeArquivo, VooNode **raizVoo, AirlineNode **raizAirline, AeroportoNode *raiz);
void libertarArvoreAirline(AirlineNode *raizAirline);
void libertarArvoreVoo(VooNode *raizVoo);
void imprimirAirline(AirlineNode *raizAirline);
void imprimirEmOrdemAirline(AirlineNode *raizAirline);
void imprimirEmOrdemVoo (VooNode *raizVoo);
void imprimirVoo(Voo *voo);