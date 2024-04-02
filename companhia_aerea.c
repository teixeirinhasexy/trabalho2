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

Airline* criarAirline() {
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

    //int comparacao = strcmp(voo->horaPart, raizVoo->voo->horaPart);
    float comparacao = (voo->distancia) - (raizVoo->voo->distancia);
    if (comparacao < 0) {
        raizVoo->esquerda = inserirNoVoo((raizVoo->esquerda), voo);
    } else if (comparacao > 0) {
        raizVoo->direita = inserirNoVoo((raizVoo->direita), voo);
    } else {
        // A cidade do novo aeroporto é a mesma que a cidade do aeroporto existente
        // Inserir o novo aeroporto na lista encadeada dentro do nó da árvore
        VooNode *novoNo = criarVooNode(voo);
        novoNo->esquerda = raizVoo->esquerda; // Aponta para a lista existente
        raizVoo->esquerda = novoNo; // Atualiza a referência para a nova lista
    }

    return raizVoo;
}

AirlineNode *inserirNoAirline(AirlineNode *raizAirline, AirlineNode *novoAirlineNode) {
    if (raizAirline == NULL) {
        // Se a raiz é nula, o novo nó se torna a nova raiz
        return novoAirlineNode;
    }

    // Comparar os nomes das Airlines
    int comparacao = strcmp(novoAirlineNode->airline->nome, raizAirline->airline->nome);

    if (comparacao < 0) {
        // Se o nome da nova Airline for menor, inserir à esquerda
        raizAirline->esquerda = inserirNoAirline(raizAirline->esquerda, novoAirlineNode);
    } else if (comparacao > 0) {
        // Se o nome da nova Airline for maior, inserir à direita
        raizAirline->direita = inserirNoAirline(raizAirline->direita, novoAirlineNode);
    } else {
        // Se as Airlines forem iguais, não fazemos nada (ou podemos tratar de outra forma)
        printf("Erro: Airline já existe na árvore.\n");
    }

    return raizAirline;
}


void lerVoosAirlines (char *nomeArquivo, AirlineNode **raizAirline, AeroportoNode *raizAeroporto){
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }
    AirlineNode *novoAirlineNode = NULL;
    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strstr(linha, "AIRLINE:") != NULL) {
            novoAirlineNode = NULL;
            Airline *novoAirline = criarAirline();
            sscanf(linha, "AIRLINE: %s", novoAirline->nome);
            AirlineNode *airlinenode = criarAirlineNode(novoAirline);
            novoAirlineNode = airlinenode;
            *raizAirline = inserirNoAirline(*raizAirline, airlinenode);
        }
        else {
            Voo *novoVoo = criarVoo();
            char IATA_part[4], IATA_cheg[4]; 
            if (sscanf(linha, "%s %s %s %s %s",
                   novoVoo->codigo, IATA_part, novoVoo->horaPart, IATA_cheg, novoVoo->horaCheg) != 5)
            {
                free(novoVoo);
                continue;
            }
            
            novoVoo->partida = procurarAeroportoPorIATA(raizAeroporto, IATA_part);
            novoVoo->chegada = procurarAeroportoPorIATA(raizAeroporto, IATA_cheg);
            novoVoo->distancia = calcularDistancia(novoVoo->partida, novoVoo->chegada);
            // Verifica se há uma Airline válida antes de tentar acessar sua raiz de Voo
            if (novoAirlineNode != NULL) {
                novoAirlineNode->raizVoo = inserirNoVoo(novoAirlineNode->raizVoo, novoVoo);
            } else {
                printf("Erro: Não há Airline válida para adicionar o voo.\n");
                free(novoVoo); // Liberar a memória do Voo, já que não será usado
            }
        }
    }
    fclose(arquivo);
}

void libertarArvoreVoo(VooNode *raizVoo) {
    if (raizVoo != NULL) {
        libertarArvoreVoo(raizVoo->esquerda);
        libertarArvoreVoo(raizVoo->direita);
        free(raizVoo->voo); // Liberar a memória alocada para o Voo
        free(raizVoo);
    }
}

void libertarArvoreAirline(AirlineNode *raizAirline) {
    if (raizAirline != NULL) {
        libertarArvoreAirline(raizAirline->esquerda);
        libertarArvoreAirline(raizAirline->direita);
        libertarArvoreVoo(raizAirline->raizVoo);
        free(raizAirline->airline); // Liberar a memória alocada para a Airline
        free(raizAirline);
    }
}

void imprimirVoo(Voo *voo) {
    printf("nº:%s ", voo->codigo);
    printf("Partida:%s %s ", voo->partida->identificador_IATA, voo->horaPart);
    printf("Chegada:%s %s ", voo->chegada->identificador_IATA, voo->horaCheg);
    printf("Distancia:%.2f\n", voo->distancia);
}

void imprimirEmOrdemVoo (VooNode *raizVoo) {
    if (raizVoo != NULL) {
        imprimirEmOrdemVoo(raizVoo->esquerda);
        imprimirVoo(raizVoo->voo);
        imprimirEmOrdemVoo(raizVoo->direita);
    }
}

void imprimirAirline(Airline *airline, AirlineNode *raizAirline) {
    printf("\nAIRLINE: %s\n", airline->nome);
    printf("\n--Número-- ----Partida---- -----Chegada----- ----Distancia----\n");
    imprimirEmOrdemVoo(raizAirline->raizVoo);
}


void imprimirEmOrdemAirline(AirlineNode *raizAirline) {
    if (raizAirline != NULL) {
        imprimirEmOrdemAirline(raizAirline->esquerda);
        imprimirAirline(raizAirline->airline, raizAirline);
        imprimirEmOrdemAirline(raizAirline->direita);
    }
}
