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

void Listar_Rotas_Diretas(Airline *listaAirlines, char *iata_origem, char *iata_destino) {
    printf("Rotas diretas de %s para %s:\n", iata_origem, iata_destino);
    int encontrou_voos = 0;
    for (Airline *companhia = listaAirlines; companhia != NULL; companhia = companhia->prox) {
        for (Voo *v = companhia->voos; v != NULL; v = v->prox) {
            if (strcmp(v->partida->identificador_IATA, iata_origem) == 0 &&
                strcmp(v->chegada->identificador_IATA, iata_destino) == 0) {
                printf("Companhia aerea: %s\n", companhia->nome);
                printf("Voo: %s\n", v->codigo);
                printf("Horario de partida: %02d:%02d\n", v->horasvoo.hora_partida_hora, v->horasvoo.hora_partida_minuto);
                printf("Horario de chegada: %02d:%02d\n", v->horasvoo.hora_chegada_hora, v->horasvoo.hora_chegada_minuto);
                printf("Distancia: %.2f km\n", v->distancia);
                printf("\n");
                encontrou_voos = 1;
            }
        }
    }
    if (!encontrou_voos) {printf("Nenhum voo direto encontrado entre %s e %s.\n", iata_origem, iata_destino);}
}


void Listar_Rotas_Com_Uma_Escala(Airline *listaAirlines, char *iata_origem, char *iata_destino) {
    printf("\nRotas com uma escala de %s para %s:\n", iata_origem, iata_destino);

    // Percorra a lista de companhias aéreas
    for (Airline *companhia = listaAirlines; companhia != NULL; companhia = companhia->prox) {
        // Percorra a lista de voos da companhia aérea atual
        for (Voo *v1 = companhia->voos; v1 != NULL; v1 = v1->prox) {
            // Verifique se o voo tem origem em iata_origem
            if (strcmp(v1->partida->identificador_IATA, iata_origem) == 0) {
                // Verifique se há um voo de ligação para o destino
                for (Voo *v2 = companhia->voos; v2 != NULL; v2 = v2->prox) {
                    if (strcmp(v2->partida->identificador_IATA, v1->chegada->identificador_IATA) == 0 &&
                        strcmp(v2->chegada->identificador_IATA, iata_destino) == 0) {
                        // Imprima os detalhes dos voos encontrados
                        printf("\nCompanhia aerea: %s\n", companhia->nome);
                        printf("\nVoo de origem:\n");
                        imprimir_voo(v1);
                        printf("Voo de destino:\n");
                        imprimir_voo(v2);
                        return; // Uma escala encontrada, termine a função
                    }
                }
            }
        }
    }
}

void Listar_Rotas_Com_Duas_Escalas(Airline *listaAirlines, char *iata_origem, char *iata_destino) {
    printf("Rotas com duas escalas de %s para %s:\n", iata_origem, iata_destino);

    // Variável para controlar se foram encontradas rotas
    int encontrou_rotas = 0;

    // Percorra a lista de companhias aéreas
    for (Airline *companhia = listaAirlines; companhia != NULL; companhia = companhia->prox) {
        // Percorra a lista de voos da companhia aérea atual
        for (Voo *v1 = companhia->voos; v1 != NULL; v1 = v1->prox) {
            // Verifique se o voo tem origem em iata_origem
            if (strcmp(v1->partida->identificador_IATA, iata_origem) == 0 &&
                strcmp(v1->chegada->identificador_IATA, iata_destino) != 0) {
                // Verifique se há um segundo voo de ligação
                for (Voo *v2 = companhia->voos; v2 != NULL; v2 = v2->prox) {
                    if (strcmp(v2->partida->identificador_IATA, v1->chegada->identificador_IATA) == 0 &&
                        strcmp(v2->chegada->identificador_IATA, iata_destino) != 0 &&
                        strcmp(v2->chegada->identificador_IATA, v1->partida->identificador_IATA) != 0) { // Garantir que não é o destino final
                        // Verifique se há um terceiro voo de ligação
                        for (Voo *v3 = companhia->voos; v3 != NULL; v3 = v3->prox) {
                            if (strcmp(v3->partida->identificador_IATA, v2->chegada->identificador_IATA) == 0 &&
                                strcmp(v3->chegada->identificador_IATA, iata_destino) == 0) {
                                // Imprima os detalhes dos voos encontrados
                                printf("Companhia aerea: %s\n", companhia->nome);
                                printf("Voo de origem:\n");
                                imprimir_voo(v1);

                                printf("Voo de escala 1:\n");
                                imprimir_voo(v2);

                                printf("Voo de escala 2:\n");
                                imprimir_voo(v3);

                                encontrou_rotas = 1; // Indique que rotas foram encontradas
                                
                            }
                        }
                    }
                }
            }
        }
    }

    // Se nenhuma rota com duas escalas foi encontrada, imprima uma mensagem
    if (!encontrou_rotas) {
        printf("Nenhuma rota com duas escalas encontrada entre %s e %s.\n", iata_origem, iata_destino);
    }
    return;
}
