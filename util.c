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

int Comando_Selecionado(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "-aeroportos") == 0) {return 1;}//lista todos os aeroportos
        else if (strcmp(argv[1], "-voos") == 0) {return 2;}//lista todos os voos
        else if (strcmp(argv[1], "-limpar") == 0) {return 0;}//limpar o terminal
    } 
    else if (argc == 4) {
        if (strcmp(argv[3], "-L0") == 0) {return 3;}//lista de rotas diretas entre dois aeroportos
        else if (strcmp(argv[3], "-L1") == 0) {return 4;}//pedido de rotas com uma ligação/escala
        else if (strcmp(argv[3], "-L2") == 0) {return 5;}//pedido de rotas com duas ligação/escala
    } 
    else if (argc == 5) {
        if (strcmp(argv[3], "-L0") == 0 && strcmp(argv[4], "-TC") == 0) {return 3;}
        else if (strcmp(argv[3], "-L0") == 0 && strcmp(argv[4], "-TD") == 0) {return -3;} //lista de rotas diretas, ordenadas por hora de partida crescente 
        else if (strcmp(argv[3], "-L1") == 0 && strcmp(argv[4], "-TC") == 0) {return 6;}
        else if (strcmp(argv[3], "-L1") == 0 && strcmp(argv[4], "-TD") == 0) {return -6;}  //pedido de rotas com uma ligação e com preocupação de ligação horária
        else if (strcmp(argv[3], "-L2") == 0 && strcmp(argv[4], "-TC") == 0) {return 7;}
        else if (strcmp(argv[3], "-L2") == 0 && strcmp(argv[4], "-TD") == 0) {return -7;} //Lista de rotas diretas, ordenadas por hora de partida decrescente 
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
        buscarArmazenarVoosDiretosAux(raizAirline->raizVoo, IATA1, IATA2, raizRotas);
        
        buscarArmazenarVoosDiretos(raizAirline->direita, IATA1, IATA2, raizRotas);
    }
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosDiretosAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosDiretosAux(raizVoo->esquerda, IATA1, IATA2, raizRotas);
        Voo *voo = raizVoo->voo;
        // Verificar se o voo é direto de IATA1 para IATA2
        if ( (strcmp(voo->partida->identificador_IATA, IATA1) == 0) && (strcmp(voo->chegada->identificador_IATA, IATA2) == 0)) {
            // Armazenar o voo direto na estrutura Rotas
            Rotas *novaRota = criarRotas();
            novaRota->voo1 = voo;
            *raizRotas = inserirRotasNode(*raizRotas, novaRota);
        }
        
        buscarArmazenarVoosDiretosAux(raizVoo->direita, IATA1, IATA2, raizRotas);
    }
}
//
void procurarArmazenarVoosComUmaEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComUmaEscala(raizAirline, IATA1, IATA2, raizRotas);
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizAirline != NULL) {
        AirlineNode *raizAirlineOriginal = raizAirline;
        buscarArmazenarVoosComUmaEscala(raizAirline->esquerda, IATA1, IATA2, raizRotas);
        buscarArmazenarVoosComUmaEscalaAux(raizAirline->raizVoo, IATA1, IATA2, raizRotas, raizAirlineOriginal);
        buscarArmazenarVoosComUmaEscala(raizAirline->direita, IATA1, IATA2, raizRotas);
    }
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscalaAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, AirlineNode *raizAirline) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComUmaEscalaAux(raizVoo->esquerda, IATA1, IATA2, raizRotas, raizAirline);
        Voo *voo1 = raizVoo->voo;
        if ( (strcmp(voo1->partida->identificador_IATA, IATA1) == 0) && (strcmp(voo1->chegada->identificador_IATA, IATA1) != 0) && (strcmp(voo1->chegada->identificador_IATA, IATA2) != 0)) {
            procurarArmazenarVoosComUmaEscalaAux(raizAirline, voo1->chegada->identificador_IATA, IATA2, raizRotas, &voo1);             
        }
        buscarArmazenarVoosComUmaEscalaAux(raizVoo->direita, IATA1, IATA2, raizRotas, raizAirline);
    }
}

void procurarArmazenarVoosComUmaEscalaAux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo) {
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComUmaEscalaAux_Aux(raizAirline, IATA1, IATA2, raizRotas, voo);
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscalaAux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo) {
    if (raizAirline != NULL) {
        buscarArmazenarVoosComUmaEscalaAux_Aux(raizAirline->esquerda, IATA1, IATA2, raizRotas, voo);
        buscarArmazenarVoosComUmaEscalaAux_AuxAux(raizAirline->raizVoo, IATA1, IATA2, raizRotas, voo);
        buscarArmazenarVoosComUmaEscalaAux_Aux(raizAirline->direita, IATA1, IATA2, raizRotas, voo);
    }
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscalaAux_AuxAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComUmaEscalaAux_AuxAux(raizVoo->esquerda, IATA1, IATA2, raizRotas, voo);
        Voo *vooAux = raizVoo->voo;
        // Verificar se o voo é direto de IATA1 para IATA2
        if ( (strcmp(vooAux->partida->identificador_IATA, IATA1) == 0) && (strcmp(vooAux->chegada->identificador_IATA, IATA2) == 0)) {
            Rotas *rota = criarRotas();
            rota->voo1 = (*voo);
            rota->voo2 = vooAux;
            *raizRotas = inserirRotasNode(*raizRotas, rota);
        }
        buscarArmazenarVoosComUmaEscalaAux_AuxAux(raizVoo->direita, IATA1, IATA2, raizRotas, voo);
    }
}
//
void procurarArmazenarVoosComDuasEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    // Verificar se a raiz das Airlines não é nula
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    
    // Buscar e armazenar voos diretos de IATA1 para IATA2
    buscarArmazenarVoosComDuasEscala(raizAirline, IATA1, IATA2, raizRotas);
}

// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscala(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizAirline != NULL) {
        AirlineNode *raizAirlineOriginal = raizAirline;
        buscarArmazenarVoosComDuasEscala(raizAirline->esquerda, IATA1, IATA2, raizRotas);
        // Verificar os voos da Airline atual
        buscarArmazenarVoosComDuasEscalaAux(raizAirline->raizVoo, IATA1, IATA2, raizRotas, raizAirlineOriginal);
        
        buscarArmazenarVoosComDuasEscala(raizAirline->direita, IATA1, IATA2, raizRotas);
    }    
}

// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, AirlineNode *raizAirline) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComDuasEscalaAux(raizVoo->esquerda, IATA1, IATA2, raizRotas, raizAirline);
        Voo *voo1 = raizVoo->voo;
        if ( (strcmp(voo1->partida->identificador_IATA, IATA1) == 0) && (strcmp(voo1->chegada->identificador_IATA, IATA1) != 0) && (strcmp(voo1->chegada->identificador_IATA, IATA2) != 0)) {
            Rotas *rota = criarRotas();
            rota->voo1 = voo1;
            procurarArmazenarVoosComDuasEscalaAux(raizAirline, IATA1, IATA2, raizRotas, &rota);             
        }
        buscarArmazenarVoosComDuasEscalaAux(raizVoo->direita, IATA1, IATA2, raizRotas, raizAirline);
    }
}

void procurarArmazenarVoosComDuasEscalaAux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
     if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComDuasEscalaAux_Aux(raizAirline, IATA1, IATA2, raizRotas, rota);

}
//
void buscarArmazenarVoosComDuasEscalaAux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizAirline != NULL) {
        AirlineNode *AirlineOriginal2 = raizAirline;
        buscarArmazenarVoosComDuasEscalaAux_Aux(raizAirline->esquerda, IATA1, IATA2, raizRotas, rota);
        buscarArmazenarVoosComDuasEscalaAux_AuxAux(raizAirline->raizVoo, IATA1, IATA2, raizRotas, rota, AirlineOriginal2);
        buscarArmazenarVoosComDuasEscalaAux_Aux(raizAirline->direita, IATA1, IATA2, raizRotas, rota);
    }
}

// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux_AuxAux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota, AirlineNode *raizAirline) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComDuasEscalaAux_AuxAux(raizVoo->esquerda, IATA1, IATA2, raizRotas,rota, raizAirline);
        Voo *voo2 = raizVoo->voo;
        if ( (strcmp(voo2->partida->identificador_IATA, (*rota)->voo1->chegada->identificador_IATA) == 0) && (strcmp(voo2->chegada->identificador_IATA, IATA1) != 0) && (strcmp(voo2->chegada->identificador_IATA, IATA2) != 0) && (strcmp(voo2->chegada->identificador_IATA, voo2->partida->identificador_IATA) != 0) ) {
            (*rota)->voo2 = voo2;
            procurarArmazenarVoosComDuasEscalaAux_Aux(raizAirline, voo2->chegada->identificador_IATA, IATA2, raizRotas, rota);             
        }
        buscarArmazenarVoosComDuasEscalaAux_AuxAux(raizVoo->direita, IATA1, IATA2, raizRotas, rota, raizAirline);
    }
}   
//
void procurarArmazenarVoosComDuasEscalaAux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComDuasEscalaAux_Aux_Aux(raizAirline, IATA1, IATA2, raizRotas, rota);
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux_Aux_Aux(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizAirline != NULL) {
        buscarArmazenarVoosComDuasEscalaAux_Aux_Aux(raizAirline->esquerda, IATA1, IATA2, raizRotas, rota);
        buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux(raizAirline->raizVoo, IATA1, IATA2, raizRotas, rota);
        buscarArmazenarVoosComDuasEscalaAux_Aux_Aux(raizAirline->direita, IATA1, IATA2, raizRotas, rota);
    }
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux(raizVoo->esquerda, IATA1, IATA2, raizRotas, rota);
        Voo *voo3 = raizVoo->voo;
        if ( (strcmp(voo3->partida->identificador_IATA, (*rota)->voo2->chegada->identificador_IATA) == 0) && (strcmp(voo3->chegada->identificador_IATA, IATA2) == 0)) {
            (*rota)->voo3 = voo3;
            *raizRotas = inserirRotasNode(*raizRotas, (*rota));
        }

        buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux(raizVoo->direita, IATA1, IATA2, raizRotas, rota);
   }
}
// LIGACAO
void procurarArmazenarVoosComUmaEscala_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComUmaEscala_LIG(raizAirline, IATA1, IATA2, raizRotas);
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscala_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizAirline != NULL) {
        AirlineNode *raizAirlineOriginal = raizAirline;
        buscarArmazenarVoosComUmaEscala_LIG(raizAirline->esquerda, IATA1, IATA2, raizRotas);
        buscarArmazenarVoosComUmaEscalaAux_LIG(raizAirline->raizVoo, IATA1, IATA2, raizRotas, raizAirlineOriginal);
        buscarArmazenarVoosComUmaEscala_LIG(raizAirline->direita, IATA1, IATA2, raizRotas);
    }
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscalaAux_LIG(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, AirlineNode *raizAirline) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComUmaEscalaAux_LIG(raizVoo->esquerda, IATA1, IATA2, raizRotas, raizAirline);
        Voo *voo1 = raizVoo->voo;
        if ( (strcmp(voo1->partida->identificador_IATA, IATA1) == 0) && (strcmp(voo1->chegada->identificador_IATA, IATA1) != 0) && (strcmp(voo1->chegada->identificador_IATA, IATA2) != 0)) {
            procurarArmazenarVoosComUmaEscalaAux_LIG(raizAirline, voo1->chegada->identificador_IATA, IATA2, raizRotas, &voo1);             
        }
        buscarArmazenarVoosComUmaEscalaAux_LIG(raizVoo->direita, IATA1, IATA2, raizRotas, raizAirline);
    }
}

void procurarArmazenarVoosComUmaEscalaAux_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo) {
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComUmaEscalaAux_Aux_LIG(raizAirline, IATA1, IATA2, raizRotas, voo);
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscalaAux_Aux_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo) {
    if (raizAirline != NULL) {
        buscarArmazenarVoosComUmaEscalaAux_Aux_LIG(raizAirline->esquerda, IATA1, IATA2, raizRotas, voo);
        buscarArmazenarVoosComUmaEscalaAux_AuxAux_LIG(raizAirline->raizVoo, IATA1, IATA2, raizRotas, voo);
        buscarArmazenarVoosComUmaEscalaAux_Aux_LIG(raizAirline->direita, IATA1, IATA2, raizRotas, voo);
    }
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComUmaEscalaAux_AuxAux_LIG(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Voo **voo) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComUmaEscalaAux_AuxAux_LIG(raizVoo->esquerda, IATA1, IATA2, raizRotas, voo);
        Voo *vooAux = raizVoo->voo;
        // Verificar se o voo é direto de IATA1 para IATA2
        if ( (strcmp(vooAux->partida->identificador_IATA, IATA1) == 0) && (strcmp(vooAux->chegada->identificador_IATA, IATA2) == 0)
              && (strcmp(vooAux->horaPart, (*voo)->horaCheg) > 0)) {
            Rotas *rota = criarRotas();
            rota->voo1 = (*voo);
            rota->voo2 = vooAux;
            *raizRotas = inserirRotasNode(*raizRotas, rota);
        }
        buscarArmazenarVoosComUmaEscalaAux_AuxAux_LIG(raizVoo->direita, IATA1, IATA2, raizRotas, voo);
    }
}
//
void procurarArmazenarVoosComDuasEscala_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    // Verificar se a raiz das Airlines não é nula
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    
    // Buscar e armazenar voos diretos de IATA1 para IATA2
    buscarArmazenarVoosComDuasEscala_LIG(raizAirline, IATA1, IATA2, raizRotas);
}

// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscala_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas) {
    if (raizAirline != NULL) {
        AirlineNode *raizAirlineOriginal = raizAirline;
        buscarArmazenarVoosComDuasEscala_LIG(raizAirline->esquerda, IATA1, IATA2, raizRotas);
        // Verificar os voos da Airline atual
        buscarArmazenarVoosComDuasEscalaAux_LIG(raizAirline->raizVoo, IATA1, IATA2, raizRotas, raizAirlineOriginal);
        
        buscarArmazenarVoosComDuasEscala_LIG(raizAirline->direita, IATA1, IATA2, raizRotas);
    }    
}

// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux_LIG(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, AirlineNode *raizAirline) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComDuasEscalaAux_LIG(raizVoo->esquerda, IATA1, IATA2, raizRotas, raizAirline);
        Voo *voo1 = raizVoo->voo;
        if ( (strcmp(voo1->partida->identificador_IATA, IATA1) == 0) && (strcmp(voo1->chegada->identificador_IATA, IATA1) != 0) && (strcmp(voo1->chegada->identificador_IATA, IATA2) != 0)) {
            Rotas *rota = criarRotas();
            rota->voo1 = voo1;
            procurarArmazenarVoosComDuasEscalaAux_LIG(raizAirline, IATA1, IATA2, raizRotas, &rota);             
        }
        buscarArmazenarVoosComDuasEscalaAux_LIG(raizVoo->direita, IATA1, IATA2, raizRotas, raizAirline);
    }
}

void procurarArmazenarVoosComDuasEscalaAux_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
     if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComDuasEscalaAux_Aux_LIG(raizAirline, IATA1, IATA2, raizRotas, rota);

}
//
void buscarArmazenarVoosComDuasEscalaAux_Aux_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizAirline != NULL) {
        AirlineNode *AirlineOriginal2 = raizAirline;
        buscarArmazenarVoosComDuasEscalaAux_Aux_LIG(raizAirline->esquerda, IATA1, IATA2, raizRotas, rota);
        buscarArmazenarVoosComDuasEscalaAux_AuxAux_LIG(raizAirline->raizVoo, IATA1, IATA2, raizRotas, rota, AirlineOriginal2);
        buscarArmazenarVoosComDuasEscalaAux_Aux_LIG(raizAirline->direita, IATA1, IATA2, raizRotas, rota);
    }
}

// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux_AuxAux_LIG(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota, AirlineNode *raizAirline) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComDuasEscalaAux_AuxAux_LIG(raizVoo->esquerda, IATA1, IATA2, raizRotas,rota, raizAirline);
        Voo *voo2 = raizVoo->voo;
        if ( (strcmp(voo2->partida->identificador_IATA, (*rota)->voo1->chegada->identificador_IATA) == 0) && (strcmp(voo2->chegada->identificador_IATA, IATA1) != 0) && (strcmp(voo2->chegada->identificador_IATA, IATA2) != 0) && (strcmp(voo2->chegada->identificador_IATA, voo2->partida->identificador_IATA) != 0) 
              && (strcmp(voo2->horaPart, (*rota)->voo1->horaCheg) > 0)) {
            (*rota)->voo2 = voo2;
            procurarArmazenarVoosComDuasEscalaAux_Aux_LIG(raizAirline, voo2->chegada->identificador_IATA, IATA2, raizRotas, rota);             
        }
        buscarArmazenarVoosComDuasEscalaAux_AuxAux_LIG(raizVoo->direita, IATA1, IATA2, raizRotas, rota, raizAirline);
    }
}   
//
void procurarArmazenarVoosComDuasEscalaAux_Aux_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizAirline == NULL) {
        printf("Erro: Não há nenhuma Airline cadastrada.\n");
        return;
    }
    buscarArmazenarVoosComDuasEscalaAux_Aux_Aux_LIG(raizAirline, IATA1, IATA2, raizRotas, rota);
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux_Aux_Aux_LIG(AirlineNode *raizAirline, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizAirline != NULL) {
        buscarArmazenarVoosComDuasEscalaAux_Aux_Aux_LIG(raizAirline->esquerda, IATA1, IATA2, raizRotas, rota);
        buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux_LIG(raizAirline->raizVoo, IATA1, IATA2, raizRotas, rota);
        buscarArmazenarVoosComDuasEscalaAux_Aux_Aux_LIG(raizAirline->direita, IATA1, IATA2, raizRotas, rota);
    }
}
// Função auxiliar para buscar e armazenar voos diretos
void buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux_LIG(VooNode *raizVoo, char *IATA1, char *IATA2, RotasNode **raizRotas, Rotas **rota) {
    if (raizVoo != NULL) {
        buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux_LIG(raizVoo->esquerda, IATA1, IATA2, raizRotas, rota);
        Voo *voo3 = raizVoo->voo;
        if ( (strcmp(voo3->partida->identificador_IATA, (*rota)->voo2->chegada->identificador_IATA) == 0) && (strcmp(voo3->chegada->identificador_IATA, IATA2) == 0)
              && (strcmp(voo3->horaPart, (*rota)->voo2->horaCheg) > 0)) {
            (*rota)->voo3 = voo3;
            *raizRotas = inserirRotasNode(*raizRotas, (*rota));
        }

        buscarArmazenarVoosComDuasEscalaAux_AuxAux_Aux_LIG(raizVoo->direita, IATA1, IATA2, raizRotas, rota);
   }
}





//
void imprimirRotas(RotasNode *raizRotas) {
    if (raizRotas != NULL) {
        imprimirRotas(raizRotas->esquerda);
        if (raizRotas->voos->voo1 !=NULL)
        {
            printf("Voo1\n");
            imprimirVoo(raizRotas->voos->voo1); // Imprimir o voo1 da rota
        }
        if (raizRotas->voos->voo2 !=NULL)
        {
            printf("Voo2\n");
            imprimirVoo(raizRotas->voos->voo2); // Imprimir o voo1 da rota
        }
        if (raizRotas->voos->voo3 !=NULL)
        {
            printf("Voo3\n");
            imprimirVoo(raizRotas->voos->voo3); // Imprimir o voo1 da rota
        }
        imprimirRotas(raizRotas->direita);
    }
}

void imprimirRotas2(RotasNode *raizRotas) {
    if (raizRotas != NULL) {
        imprimirRotas(raizRotas->direita);
        if (raizRotas->voos->voo1 !=NULL)
        {
            printf("Voo1\n");
            imprimirVoo(raizRotas->voos->voo1); // Imprimir o voo1 da rota
        }
        if (raizRotas->voos->voo2 !=NULL)
        {
            printf("Voo2\n");
            imprimirVoo(raizRotas->voos->voo2); // Imprimir o voo1 da rota
        }
        if (raizRotas->voos->voo3 !=NULL)
        {
            printf("Voo3\n");
            imprimirVoo(raizRotas->voos->voo3); // Imprimir o voo1 da rota
        }
        imprimirRotas(raizRotas->esquerda);
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

