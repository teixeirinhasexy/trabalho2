#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "companhia_aerea.h"

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


// Função para encontrar uma companhia aérea na lista
Airline *encontrarAirline(Airline *listaAirlines, char *nomeCompanhia) {
    Airline *atual = listaAirlines;
    // Percorrer a lista de companhias aéreas
    while (atual != NULL) {
        // Verificar se o nome da companhia aérea corresponde ao nome procurado
        if (strcmp(atual->nome, nomeCompanhia) == 0) {return atual;} // Retornar o ponteiro para a companhia aérea encontrada
        atual = atual->prox;
    }
    // Se não encontrar a companhia aérea, retorna NULL
    return NULL;
}


void inserirVoo(Airline *companhia, char *codigo, Aeroporto *partida, Aeroporto *chegada, float distancia, HorasVoo horas_voo) {
    Voo *novoVoo = calloc(1, sizeof(Voo));
    if (novoVoo == NULL) {
        printf("Erro ao alocar memória para o voo.\n");
        exit(1);
    }
    strcpy(novoVoo->codigo, codigo);
    novoVoo->partida = partida;
    novoVoo->horasvoo.hora_partida_hora = horas_voo.hora_partida_hora;
    novoVoo->horasvoo.hora_partida_minuto = horas_voo.hora_partida_minuto;
    novoVoo->chegada = chegada;
    novoVoo->horasvoo.hora_chegada_hora = horas_voo.hora_chegada_hora;
    novoVoo->horasvoo.hora_chegada_minuto = horas_voo.hora_chegada_minuto;
    novoVoo->distancia = distancia;
    novoVoo->prox = NULL;

    if (companhia->voos == NULL) {
        companhia->voos = novoVoo;
    } else {
        Voo *temp = companhia->voos;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novoVoo;
    }
}


void inserirAirline(Airline **listaAirlines, char *nome) {
    Airline *novaAirline = malloc(sizeof(Airline));
    if (novaAirline == NULL) {
        printf("Erro ao alocar memória para a companhia aérea.\n");
        exit(1);
    }
    strcpy(novaAirline->nome, nome);
    novaAirline->voos = NULL;
    novaAirline->prox = NULL;

    if (*listaAirlines == NULL) {
        *listaAirlines = novaAirline;
    } else {
        Airline *temp = *listaAirlines;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novaAirline;
    }
}

Aeroporto* encontrarAeroporto(Aeroporto *listaAeroportos, char *identificador_IATA) {
    Aeroporto *temp = listaAeroportos;
    while (temp != NULL) {
        if (strcmp(temp->identificador_IATA, identificador_IATA) == 0) {
            return temp;
        }
        temp = temp->prox;
    }
    return NULL; // Não encontrado
}
void lerVoos(char *nomeArquivo, Airline **listaAirlines, Aeroporto *listaAeroportos) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }

    char linha[100];
    char nomeCompanhia[50];
    Airline *companhiaAtual = NULL;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Verificar se a linha indica uma nova companhia aérea
        if (strstr(linha, "AIRLINE:") != NULL) {
            sscanf(linha, "AIRLINE: %s", nomeCompanhia);
            inserirAirline(listaAirlines, nomeCompanhia);
            companhiaAtual = encontrarAirline(*listaAirlines, nomeCompanhia);
        } else {
            // Ler informações do voo
            char codigoVoo[10], aeroportoPartida[4], aeroportoChegada[4];
            HorasVoo horasDoVoo;
            sscanf(linha, "%s %s %d:%d %s %d:%d",
                   codigoVoo, aeroportoPartida, &(horasDoVoo.hora_partida_hora), &(horasDoVoo.hora_partida_minuto),
                   aeroportoChegada, &(horasDoVoo.hora_chegada_hora), &(horasDoVoo.hora_chegada_minuto) );

            // Encontrar os aeroportos de partida e chegada na lista de aeroportos
            Aeroporto *aeroportoPartidaPtr = encontrarAeroporto(listaAeroportos, aeroportoPartida);
            Aeroporto *aeroportoChegadaPtr = encontrarAeroporto(listaAeroportos, aeroportoChegada);

            // Verificar se os aeroportos foram encontrados corretamente
            if (aeroportoPartidaPtr != NULL && aeroportoChegadaPtr != NULL) {
                // Calcular distância entre os aeroportos
                float distancia = calcularDistancia(aeroportoPartidaPtr, aeroportoChegadaPtr);

                // Inserir o voo na lista da companhia aérea
                inserirVoo(companhiaAtual, codigoVoo, aeroportoPartidaPtr, aeroportoChegadaPtr, distancia, horasDoVoo);
            } else {
                printf("Erro ao encontrar aeroporto de partida ou chegada para o voo %s.\n", codigoVoo);
            }
        }
    }

    fclose(arquivo);
}


void imprimir_voo(Voo *voo) {
    printf("Codigo do voo: %s\n", voo->codigo);
    printf("Aeroporto de partida: %s\n", voo->partida->identificador_IATA);
    printf("Horario de partida: %02d:%02d\n", voo->horasvoo.hora_partida_hora, voo->horasvoo.hora_partida_minuto);
    printf("Aeroporto de chegada: %s\n", voo->chegada->identificador_IATA);
    printf("Horario de chegada: %02d:%02d\n", voo->horasvoo.hora_chegada_hora, voo->horasvoo.hora_chegada_minuto);
    printf("Distancia total: %.2f km\n", voo->distancia);
    printf("\n");
}


void imprimirRotas(Airline *listaAirlines) {
    Airline *companhiaAtual = listaAirlines;
    while (companhiaAtual != NULL) {
        printf("Companhia: %s\n", companhiaAtual->nome);
        Voo *vooAtual = companhiaAtual->voos;
        while (vooAtual != NULL) {
            imprimir_voo(vooAtual);
            vooAtual = vooAtual->prox;
        }
        companhiaAtual = companhiaAtual->prox;
    }
}

void libertarVoos(Voo *listaVoos) {
    Voo *temp;
    while (listaVoos != NULL) {
        temp = listaVoos;
        listaVoos = listaVoos->prox;
        free(temp);
    }
}

// Função para liberar a memória alocada para a lista de companhias aéreas
void libertarAirlines(Airline *listaAirlines) {
    Airline *temp;
    while (listaAirlines != NULL) {
        temp = listaAirlines;
        listaAirlines = listaAirlines->prox;
        libertarVoos(temp->voos); // Libera a memória dos voos da companhia aérea
        free(temp);
    }
}


