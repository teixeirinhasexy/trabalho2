#include <stdio.h>
#include <stdlib.h>
#include "aeroporto.h"
#include "companhia_aerea.h"
#include "util.h"

int main(int argc, char *argv[]) {
    
    Aeroporto *listaAeroportos = NULL;
    Airline *listaAirlines = NULL;
    int numErrosAeroportos = 0;
    int linhasErroAeroportos[MAX_ERROS];
    char selecao = Comando_Selecionado(argc, argv);

    if (argc == 1) {mostrar_comandos(); return 0;}

    lerAeroportos("aeroportos.txt", &listaAeroportos, &numErrosAeroportos, linhasErroAeroportos);
    lerVoos("rotas.txt" , &listaAirlines, listaAeroportos);
    
    if(argc >= 2) {
        switch (selecao) {
        case '0':
            system("clear");
            system("cls");
            break;
        case '1': /* Lista todos os aeroportos */
            printf("Aeroportos:\n");
            imprimirAeroportos(listaAeroportos);
            printf("\n");
            printf("Numero total de erros nos aeroportos: %d\n", numErrosAeroportos);
            printf("Linhas com erros nos aeroportos:");
            for (int i = 0; i < numErrosAeroportos; i++) {
                if (i == (numErrosAeroportos - 1)) {printf(" %d.\n", linhasErroAeroportos[i]);} 
                else {printf(" %d,", linhasErroAeroportos[i]);}
            }
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
        case '2': /* Lista todos os voos */
            printf("Rotas:\n");
            imprimirRotas(listaAirlines);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
        case '3': /* Lista de rotas diretas entre dois aeroportos */
            Listar_Rotas_Diretas(listaAirlines, argv[1], argv[2]);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
        case '4': /* Pedido de rotas com uma ligação/escala */
            Listar_Rotas_Com_Uma_Escala(listaAirlines, argv[1], argv[2]);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
        
        case '5':  /* Pedido de rotas com duas ligação/escala */
            Listar_Rotas_Com_Duas_Escalas(listaAirlines, argv[1], argv[2]);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
            
        case '6': /* Pedido de rotas com uma ligação/escala */

            break;

        case '7': /* Pedido de rotas com uma ligação e com preocupação de ligação horária */
            /* Implementação para listar rotas com uma escala e horários compatíveis */
            break;

        case '8': /* Pedido de rotas com preocupação de ligação horária e com a menor distância */
            /* Implementação para listar rotas com critérios de tempo e distância */
            break;

        default:
            printf("Comando nao reconhecido ou falta de comandos.\n");
            break;
        }

    }

    liberarAeroportos(listaAeroportos);
    libertarAirlines(listaAirlines);
    return 0;
}
