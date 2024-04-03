#include "aeroporto.h"
#include "companhia_aerea.h"
#ifndef UTIL_H
typedef struct Iatas
{
    char IATA1[4];
    char IATA2[4];

} Iatas;

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
RotasNode *inserirRotasNode(RotasNode *raizRotas, Rotas *voos);
void buscarArmazenarVoosDiretos(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);
void procurarArmazenarVoosDiretos(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);
void buscarArmazenarVoosDiretosAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas);

void buscarArmazenarVoosComUmaEscalaAux_AuxAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo2);
void buscarArmazenarVoosComUmaEscalaAux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo2);
void procurarArmazenarVoosComUmaEscalaAux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo2);
void buscarArmazenarVoosComUmaEscalaAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, AirlineNode *raizAirline);
void buscarArmazenarVoosComUmaEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);
void procurarArmazenarVoosComUmaEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);


// Cabeçalho da função principal
void procurarArmazenarVoosComDuasEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);
// Função auxiliar para buscar e armazenar voos com duas escalas
void buscarArmazenarVoosComDuasEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas);
// Função auxiliar para buscar voos com duas escalas
void buscarArmazenarVoosComDuasEscalaAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, AirlineNode *raizAirline);

void procurarArmazenarVoosComDuasEscalaAux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota);

void buscarArmazenarVoosComDuasEscalaAux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota);

void buscarArmazenarVoosComDuasEscalaAux_AuxAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota, AirlineNode *raizAirline);

void procurarArmazenarVoosComDuasEscalaAux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota);

void buscarArmazenarVoosComDuasEscalaAux_Aux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota);

void buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota);



#endif /* UTIL_H */

