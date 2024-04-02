#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "aeroporto.h"
#include "companhia_aerea.h"

void mostrar_comandos() {
    system("cls");
    system("clear");
    printf("\nComandos disponiveis:\n");
    printf("-aeroportos : Lista todos os aeroportos\n");
    printf("-voos : Lista todos os voos\n");
    printf("-L0 : Lista de rotas diretas entre dois aeroportos\n");
    printf("-L1 : Pedido de rotas com uma ligacao/escala\n");
    printf("-L2 : Pedido de rotas com duas ligacoes/escalas\n");
}

char Comando_Selecionado(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "-aeroportos") == 0) {return '1';}//lista todos os aeroportos
        else if (strcmp(argv[1], "-voos") == 0) {return '2';}//lista todos os voos
        else if (strcmp(argv[1], "-limpar") == 0) {return '0';}//limpar o terminal
    } 
    else if (argc == 4) {
        if (strcmp(argv[3], "-L0") == 0) {return '3';}//lista de rotas diretas entre dois aeroportos
        else if (strcmp(argv[3], "-L1") == 0) {return '4';}//pedido de rotas com uma ligação/escala
        else if (strcmp(argv[3], "-L2") == 0) {return '5';}//pedido de rotas com duas ligação/escala
    } 
    else if (argc == 5) {
        if (strcmp(argv[3], "-L0") == 0 && strcmp(argv[4], "-TC") == 0) {return '9';} //lista de rotas diretas, ordenadas por hora de partida crescente 
        else if (strcmp(argv[3], "-L1") == 0 && strcmp(argv[4], "-TC") == 0) {return '7';} //pedido de rotas com uma ligação e com preocupação de ligação horária
        else if (strcmp(argv[3], "-L0") == 0 && strcmp(argv[4], "-TD") == 0) {return '6';} //Lista de rotas diretas, ordenadas por hora de partida decrescente 
        else if (argc == 6 && strcmp(argv[5], "-D") == 0) {return '8';} //edido de rotas com preocupação de ligação horária e com a menor distância
    }
}

Rotas* criarRotas() {
    Rotas *novaRotas = (Rotas *)calloc(1, sizeof(Rotas));
    if (novaRotas == NULL) {
        printf("Erro ao alocar memória para uma rota.\n");
        exit(1);
    }
    return novaRotas;
}

RotasNode *criarRotasNode(Rotas *voos) {
    RotasNode *novoRotasNode = (RotasNode *)calloc(1, sizeof(RotasNode));
    if (novoRotasNode == NULL) {
        printf("Erro ao alocar memória para o nó das rotas.\n");
        exit(1);
    }
    novoRotasNode->voos = voos;
    novoRotasNode->direita = NULL;
    novoRotasNode->esquerda = NULL;
    return novoRotasNode;
}

RotasNode *inserirRotasNode(RotasNode *raizRotas, Rotas *voos) {
    if (raizRotas == NULL) {
        return criarRotasNode(voos);
    }

    // Comparar as horas de partida dos voos para inserir na ordem crescente
    if (strcmp(voos->voo1->horaPart, raizRotas->voos->voo1->horaPart) < 0) {
        raizRotas->esquerda = inserirRotasNode(raizRotas->esquerda, voos);
    } else {
        raizRotas->direita = inserirRotasNode(raizRotas->direita, voos);
    }

    return raizRotas;
}

void procurarArmazenarVoosDiretos(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    // Verificar se a raiz das Airlines não é nula
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    
    // Buscar e armazenar voos diretos de IATA1 para IATA2
    buscarArmazenarVoosDiretos(raizAirline, IATA1, IATA2, raizRotas);
}

// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosDiretos(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizAirline != NULL) {
        buscarArmazenarVoosDiretos(raizAirline->esquerda, IATA1, IATA2, raizRotas);
        
        // Verificar os voos da Airline atual
        VooNode *raizVoo = raizAirline->raizVoo;
        while (raizVoo != NULL) {
            Voo *voo = raizVoo->voo;
            // Verificar se o voo é direto de IATA1 para IATA2
            if (strcmp(voo->partida->identificador_IATA, IATA1) == 0 && strcmp(voo->chegada->identificador_IATA, IATA2) == 0) {
                // Armazenar o voo direto na estrutura Rotas
                Rotas *novaRota = criarRotas();
                novaRota->voo1 = voo;
                *raizRotas = inserirRotasNode(*raizRotas, novaRota);
            }
            raizVoo = raizVoo->direita;
        }
        
        buscarArmazenarVoosDiretos(raizAirline->direita, IATA1, IATA2, raizRotas);
    }
}

// Função para inserir um novo nó de Rotas na árvore

void imprimirRotas(RotasNode *raizRotas) {
    if (raizRotas != NULL) {
        printf("merda");
        imprimirRotas(raizRotas->esquerda);
        imprimirVoo(raizRotas->voos->voo1); // Imprimir o voo1 da rota
        imprimirRotas(raizRotas->direita);
    }
    else {
        printf("raiz rotas fodida");
    }
}

void libertarRotas(RotasNode *raizRotas) {
    if (raizRotas != NULL) {
        libertarRotas(raizRotas->esquerda);
        libertarRotas(raizRotas->direita);
        free(raizRotas->voos); // Liberar a memória alocada para a estrutura Rotas
        free(raizRotas);
    }
}

