#ifndef AEROPORTO_H
#define AEROPORTO_H
#define MAX_ERROS 100

#include <stdbool.h>

//enum para representar os valores associados a cada letra do alfabeto - utilizado para ordenar o aeroportos por ordem alfabética
typedef enum {
    A = 0, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z
} ValorLetra;


typedef struct Localizcao {
    int latitude_hora;
    int latitude_minuto;
    int latitude_segundo;
    char latitude_dir;
    int longitude_hora;
    int longitude_minuto;
    int longitude_segundo;
    char longitude_dir;    
} Localizacao;

//estrutura para representar um aeroporto
typedef struct Aeroporto {
    char identificador_ICAO[5]; //identificador ICAO
    char identificador_IATA[4]; //identificador IATA
    Localizacao localizacao;
    char cidade[50]; //cidade onde se encontra o aeroporto
    int timeZone; //time zone (-12 a 14)
    struct Aeroporto *prox; //ponteiro para o próximo aeroporto na lista
} Aeroporto;

//função para criar um novo aeroporto
Aeroporto* criarAeroporto();

//função para inserir um aeroporto na lista mantendo a ordem alfabética da cidade
void inserirAeroporto(Aeroporto **listaAeroportos, Aeroporto *novoAeroporto);

//função para validar uma linha do arquivo de entrada
bool validarLinha(char *linha, Aeroporto *Aeroporto , int numLinha, int *numErros, int linhasErro[]);

//função para ler o arquivo e preencher a lista dinâmica com os aeroportos
void lerAeroportos(char *nomeArquivo, Aeroporto **listaAeroportos, int *numErros, int linhasErro[]);

//função para liberar a memória alocada para a lista dinâmica de aeroportos
void liberarAeroportos(Aeroporto *listaAeroportos);

//função para imprimir os aeroportos
void imprimirAeroportos(Aeroporto *listaAeroportos);

#endif
