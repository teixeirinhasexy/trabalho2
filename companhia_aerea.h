#ifndef ROTAS_H
#define ROTAS_H

#include "aeroporto.h"




typedef struct Voo {
    char codigo[10];
    Aeroporto *partida;
    Aeroporto *chegada;
    char horaPart[5];
    char horaCheg[5];
    float distancia;
} Voo;

typedef struct vooNode {
    Voo *voo;
    struct VooNode *esquerda;
    struct VooNode *direita ;
} VooNode ;


// Definindo uma estrutura para representar uma companhia aérea
typedef struct Airline {
    char nome[50];
    VooNode *raizVoo; // Lista de voos da companhia aérea
} Airline;

typedef struct AirlineNode {
    Airline *airline;
    struct AirlineNode *esquerda;
    struct AirlineNode *direita;
} AirlineNode;

#endif // ROTAS_H


void lerVoosAirlines(char *nomeArquivo, VooNode **raizVoo, AirlineNode **raizAirline, Airline *temp);
void libertarArvoreAirline(AirlineNode *raizAirline);
void libertarArvoreVoo(VooNode *raizVoo);
void imprimirAirline(Airline *airline);
void imprimirEmOrdemAirline(AirlineNode *raizAirline);
void imprimirEmOrdemVoo (VooNode *raizVoo);
void imprimirVoo(Voo *voo);