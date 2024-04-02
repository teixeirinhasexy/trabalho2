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


