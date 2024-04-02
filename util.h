#include "aeroporto.h"
#include "companhia_aerea.h"
#ifndef UTIL_H
typedef struct Rotas
{
    Voo *voo1;
    Voo *voo2;
    Voo *voo3;

} Rotas;

typedef struct RotasNode
{
    Rotas *voos;
    struct RotasNode *direita;
    struct RotasNode *esquerda;
} RotasNode;
#define UTIL_H


void mostrar_comandos();
char Comando_Selecionado(int argc, char *argv[]);
void libertarRotas(RotasNode *raizRotas);
void imprimirRotas(RotasNode *raizRotas);
void buscarArmazenarVoosDiretos(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);
RotasNode *inserirRotasNode(RotasNode *raizRotas, Rotas *voos);
void procurarArmazenarVoosDiretos(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);


#endif /* UTIL_H */

