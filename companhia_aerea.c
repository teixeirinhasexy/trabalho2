#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "companhia_aerea.h"
#include "aeroporto.h"

// Função para calcular a distância entre dois aeroportos
float calcularDistancia(Aeroporto *aeroporto1, Aeroporto *aeroporto2) {
    // Constante para o raio médio da Terra em quilômetros
    const float RAIO_TERRA = 6371.0;
    // Converter as coordenadas geográficas para radianos
    float latA = (aeroporto1->localizacao.latitude_hora + aeroporto1->localizacao.latitude_minuto / 60.0 + aeroporto1->localizacao.latitude_segundo / 3600.0) * (aeroporto1->localizacao.latitude_dir == 'S' ? -1 : 1) * M_PI / 180.0;
    float lonA = (aeroporto1->localizacao.longitude_hora + aeroporto1->localizacao.longitude_minuto / 60.0 + aeroporto1->localizacao.longitude_segundo / 3600.0) * (aeroporto1->localizacao.longitude_dir == 'W' ? -1 : 1) * M_PI / 180.0;
    float latB = (aeroporto2->localizacao.latitude_hora + aeroporto2->localizacao.latitude_minuto / 60.0 + aeroporto2->localizacao.latitude_segundo / 3600.0) * (aeroporto2->localizacao.latitude_dir == 'S' ? -1 : 1) * M_PI / 180.0;
    float lonB = (aeroporto2->localizacao.longitude_hora + aeroporto2->localizacao.longitude_minuto / 60.0 + aeroporto2->localizacao.longitude_segundo / 3600.0) * (aeroporto2->localizacao.longitude_dir == 'W' ? -1 : 1) * M_PI / 180.0;
    // Calcular a distância entre os aeroportos usando a fórmula do haversine
    float dlon = lonB - lonA, dlat = latB - latA;
    float a = sin(dlat / 2) * sin(dlat / 2) + cos(latA) * cos(latB) * sin(dlon / 2) * sin(dlon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float distancia = RAIO_TERRA * c;

    return distancia;
}

Voo* criarVoo() {
    Voo *novoVoo = (Voo *)calloc(1, sizeof(Voo));
    if (novoVoo == NULL) {
        printf("Erro ao alocar memória para o voo.\n");
        exit(1);
    }
    return novoVoo;
}

Voo* criarAirline() {
    Airline *novoAirline = (Airline *)calloc(1, sizeof(Airline));
    if (novoAirline == NULL) {
        printf("Erro ao alocar memória para a Airline.\n");
        exit(1);
    }
    return novoAirline;
}

VooNode *criarVooNode(Voo *voo) {
    VooNode *novoVooNode = (VooNode *)calloc(1, sizeof(VooNode));
    if (novoVooNode == NULL) {
        printf("Erro ao alocar memória para o nó do voo.\n");
        exit(1);
    }
    novoVooNode->voo = voo;
    novoVooNode->direita = NULL;
    novoVooNode->esquerda = NULL;
    return novoVooNode;
}

AirlineNode *criarAirlineNode(Airline *airline) {
    AirlineNode *novoAirlineNode = (AirlineNode *)calloc(1, sizeof(AirlineNode));
    if (novoAirlineNode == NULL) {
        printf("Erro ao alocar memória para o nó da Airline.\n");
        exit(1);
    }
    novoAirlineNode->airline = airline;
    novoAirlineNode->direita = NULL;
    novoAirlineNode->esquerda = NULL;
    return novoAirlineNode;
}

VooNode *inserirNoVoo(VooNode *raizVoo, Voo *voo) {
    if (raizVoo == NULL) {
        return criarVooNode(voo);
    }

    int comparacao = strcmp(voo->codigo, raizVoo->voo->codigo);

    if (comparacao < 0) {
        raizVoo->esquerda = inserirNoVoo(raizVoo->esquerda, voo);
    } else if (comparacao > 0) {
        raizVoo->direita = inserirNoVoo(raizVoo->direita, voo);
    } else {
        // A cidade do novo aeroporto é a mesma que a cidade do aeroporto existente
        // Inserir o novo aeroporto na lista encadeada dentro do nó da árvore
        VooNode *novoNo = criarVooNode(voo);
        novoNo->esquerda = raizVoo->esquerda; // Aponta para a lista existente
        raizVoo->esquerda = novoNo; // Atualiza a referência para a nova lista
    }

    return raizVoo;
}

AirlineNode *inserirNoAirline(AirlineNode *raizAirline, Airline *airline) {
    if (raizAirline == NULL) {
        return criarAirlineNode(airline);
    }

    int comparacao = strcmp(airline->nome, raizAirline->airline->nome);

    if (comparacao < 0) {
        raizAirline->esquerda = inserirNoAirline(raizAirline->esquerda, airline);
    } else if (comparacao > 0) {
        raizAirline->direita = inserirNoAirline(raizAirline->direita, airline);
    } else {
        // A cidade do novo aeroporto é a mesma que a cidade do aeroporto existente
        // Inserir o novo aeroporto na lista encadeada dentro do nó da árvore
        AirlineNode *novoNo = criarAirlineNode(airline);
        novoNo->esquerda = raizAirline->esquerda; // Aponta para a lista existente
        raizAirline->esquerda = novoNo; // Atualiza a referência para a nova lista
    }

    return raizAirline;
}

void lerVoosAirlines (char *nomeArquivo, VooNode **raizVoo, AirlineNode **raizAirline, Airline *temp){
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }

    char linha[100];
    char nomeCompanhia[50];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strstr(linha, "AIRLINE:") != NULL) {
            Airline *novoAirline = criarAirline();
            sscanf(linha, "AIRLINE: %s", novoAirline->nome);
            *raizAirline = inserirNoAirline(*raizAirline, novoAirline);
            temp = novoAirline;
        }
        
        else {
            Voo *novoVoo = criarVoo();
            sscanf(linha, "%s %s %s %s %s",
                   novoVoo->codigo, novoVoo->partida, novoVoo->horaPart, novoVoo->chegada, novoVoo->horaCheg);
            *raizVoo = inserirNoVoo(*raizVoo, novoVoo);
            temp->raizVoo = raizVoo;
            
        }
    }
    fclose(arquivo);
}

void libertarArvoreVoo(VooNode *raizVoo) {
    if (raizVoo != NULL) {
        libertarArvoreAirline(raizVoo->esquerda);
        libertarArvoreAirline(raizVoo->direita);
        free(raizVoo->voo); // Liberar a memória alocada para o Aeroporto
        free(raizVoo);
    }
}

void libertarArvoreAirline(AirlineNode *raizAirline) {
    if (raizAirline != NULL) {
        libertarArvoreAirline(raizAirline->esquerda);
        libertarArvoreAirline(raizAirline->direita);
        libertarArvoreVoo(raizAirline->airline->raizVoo);
        free(raizAirline->airline); // Liberar a memória alocada para o Aeroporto
        free(raizAirline);
    }
}

void imprimirVoo(Voo *voo) {
    printf("Codico voo: %s", voo->codigo);
}

void imprimirEmOrdemVoo (VooNode *raizVoo) {
    if (raizVoo != NULL) {
        imprimirEmOrdemVoo(raizVoo->esquerda);
        imprimirVoo(raizVoo->voo);
        imprimirEmOrdemVoo(raizVoo->direita);
    }
}

void imprimirAirline(Airline *airline) {
    printf("\nAIRLINE: %s" , airline->nome);
    imprimirEmOrdemVoo(airline->raizVoo);
}

void imprimirEmOrdemAirline(AirlineNode *raizAirline) {
    if (raizAirline != NULL) {
        imprimirEmOrdemAirline(raizAirline->esquerda);
        imprimirAirline(raizAirline->airline);
        imprimirEmOrdemAirline(raizAirline->direita);
    }
}