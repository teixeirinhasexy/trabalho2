#include <stdio.h>
#include <stdlib.h>
#include "aeroporto.h"
#include "companhia_aerea.h"
#include "util.h"

int main(int argc, char *argv[]) {
    AeroportoNode *raiz = NULL;
    AirlineNode *raizAirline = NULL;
    RotasNode *raizRotas = NULL;
    int numErrosAeroportos = 0;
    int linhasErroAeroportos[MAX_ERROS];
    char selecao = Comando_Selecionado(argc, argv);

    if (argc == 1) {mostrar_comandos(); return 0;}

    lerAeroportos("aeroportos.txt", &raiz, &numErrosAeroportos, linhasErroAeroportos);
    lerVoosAirlines("rotas.txt", &raizAirline, raiz);
    if(argc >= 2) {
        switch (selecao) {
        case '0':
            system("clear");
            system("cls");
            break;

        case '1': 
            printf("Aeroportos:\n");
            imprimirEmOrdem(raiz);
            printf("\n");
            printf("Numero total de erros nos aeroportos: %d\n", numErrosAeroportos);
            printf("Linhas com erros nos aeroportos:");
            for (int i = 0; i < numErrosAeroportos; i++) {
                if (i == (numErrosAeroportos - 1)) {printf(" %d.\n", linhasErroAeroportos[i]);} 
                else {printf(" %d,", linhasErroAeroportos[i]);}
            }
            printf("\ncomand(-limpar) para limpar o ecra");
            break;

        case '2': 
            printf("Rotas:\n");
            imprimirEmOrdemAirline(raizAirline);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;

        case '3': 
            procurarArmazenarVoosDiretos(raizAirline, argv[1], argv[2], &raizRotas);
            imprimirRotas(raizRotas);
            libertarRotas(raizRotas);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
            
        case '4': 
            procurarArmazenarVoosComUmaEscala(raizAirline, argv[1], argv[2], &raizRotas);
            imprimirRotas(raizRotas);
            libertarRotas(raizRotas);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
        
        case '5': 
            procurarArmazenarVoosComDuasEscala(raizAirline, argv[1], argv[2], &raizRotas);
            imprimirRotas(raizRotas);
            libertarRotas(raizRotas);
            printf("\ncomand(-limpar) para limpar o ecra");
            break;
            
        case '6': 

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


    libertarArvore(raiz);
    libertarArvoreAirline(raizAirline);
    return 0;
}
