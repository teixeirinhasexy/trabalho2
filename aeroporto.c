#include "aeroporto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*----------------------------------------------------------------------------------------
| Nome: criarAeroporto
| Ação: Aloca um bloco de memória do tamanho do tipo Aeroporto utilizando a função calloc
|       que inicializa todos os bits a zero, não sendo necessário inicializar os campos da
        estrutura Aeroporto. Verifica se a alocação de memória foi bem sucedida. Inicializa
        o ponteiro 'prox' do novo aeroporto como NULL.
| Devolve: O ponteiro para o novoAeroporto.
------------------------------------------------------------------------------------------*/
Aeroporto* criarAeroporto() {
    Aeroporto *novoAeroporto = (Aeroporto *)calloc(1, sizeof(Aeroporto));
    if (novoAeroporto == NULL) {
        printf("Erro ao alocar memória para o aeroporto.\n");
        exit(1);
    }
    novoAeroporto->prox = NULL;
    return novoAeroporto;
}

/*----------------------------------------------------------------------------------------
| Nome: obterValorLetra
| Ação: Recebe como argumento uma letra e converte essa letra para maiuscula utilizando a
        função toupper. Subtrai o caractere 'A' do caractere maiúsculo resultante para 
        obter um deslocamento em relação à letra 'A'. O deslocamento é convertido para
         o tipo ValorLetra
| Devolve: Devolve o deslocamento convertido para o tipo ValorLetra
------------------------------------------------------------------------------------------*/
ValorLetra obterValorLetra(char letra) {
    return (ValorLetra)(toupper(letra) - 'A');
}

// Função para inserir um aeroporto na lista mantendo a ordem alfabética da cidade
void inserirAeroporto(Aeroporto **listaAeroportos, Aeroporto *novoAeroporto) {
    Aeroporto *atual = *listaAeroportos;
    Aeroporto *anterior = NULL;

    ValorLetra valorNovaCidade = obterValorLetra(novoAeroporto->cidade[0]);

    // Percorrer a lista até encontrar o local onde o novo aeroporto deve ser inserido
    while (atual != NULL && obterValorLetra(atual->cidade[0]) < valorNovaCidade) {
        anterior = atual;
        atual = atual->prox;
    }

    // Inserir o novo aeroporto na lista
    if (anterior == NULL) {
        novoAeroporto->prox = *listaAeroportos;
        *listaAeroportos = novoAeroporto;
    } else {
        anterior->prox = novoAeroporto;
        novoAeroporto->prox = atual;
    }
}

// Função para validar uma linha do arquivo de entrada
/*bool validarLinha(char *linha, int numLinha, int *numErros, int linhasErro[]) {
    // Verificar se todos os parâmetros foram lidos corretamente
    char ICAO[5], IATA[4], cidade[50];
    int latitude[3], longitude[3], timeZone;
    char latitude_dir, longitude_dir;

    int numParam = sscanf(linha, "%4s %3s %d %d %d %c %d %d %d %c %49s %d", ICAO, IATA, &latitude[0], &latitude[1], &latitude[2], &latitude_dir, &longitude[0], &longitude[1], &longitude[2], &longitude_dir, cidade, &timeZone);
    
    if ( numParam != 12 || latitude_dir != 'N' || latitude_dir != 'S' || longitude_dir != 'E' || longitude_dir != 'W' || timeZone < -12 || timeZone > 14 || strlen(ICAO) != 4 || strlen(IATA) != 3) {
    linhasErro[*numErros] = numLinha;
    (*numErros)++;
    return false;
    }

    return true;
}
*/

// Função para ler o arquivo e preencher a lista dinâmica com os aeroportos
void lerAeroportos(char *nomeArquivo, Aeroporto **listaAeroportos, int *numErros, int linhasErro[]) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }
    char linha[100];
    int numLinha = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        numLinha++;
        if (!validarLinha(linha, numLinha, numErros, linhasErro)) {
            continue;
        }

        Aeroporto *novoAeroporto = criarAeroporto();
        sscanf(linha, "%s %s %d %d %d %c %d %d %d %c %s %d",
               novoAeroporto->identificador_ICAO,
               novoAeroporto->identificador_IATA,
               &novoAeroporto->localizacao.latitude_hora,
               &novoAeroporto->localizacao.latitude_minuto,
               &novoAeroporto->localizacao.latitude_segundo,
               &novoAeroporto->localizacao.latitude_dir,
               &novoAeroporto->localizacao.longitude_hora,
               &novoAeroporto->localizacao.longitude_minuto,
               &novoAeroporto->localizacao.longitude_segundo,
               &novoAeroporto->localizacao.longitude_dir,
               novoAeroporto->cidade,
               &novoAeroporto->timeZone);

        inserirAeroporto(listaAeroportos, novoAeroporto);
    }

    fclose(arquivo);
}

// Função para liberar a memória alocada para a lista dinâmica de aeroportos
void liberarAeroportos(Aeroporto *listaAeroportos) {
    Aeroporto *temp;
    while (listaAeroportos != NULL) {
        temp = listaAeroportos;
        listaAeroportos = listaAeroportos->prox;
        free(temp);
    }
}


void imprimirAeroportos(Aeroporto *listaAeroportos) {
    Aeroporto *temp = listaAeroportos;
    while (temp != NULL) {
        printf("ICAO: %s\n", temp->identificador_ICAO);
        printf("IATA: %s\n", temp->identificador_IATA);
        printf("Latitude: %d %d %d %c\n", temp->localizacao.latitude_hora, temp->localizacao.latitude_minuto, temp->localizacao.latitude_segundo, temp->localizacao.latitude_dir);
        printf("Longitude: %d %d %d %c\n", temp->localizacao.longitude_hora, temp->localizacao.longitude_minuto, temp->localizacao.longitude_segundo, temp->localizacao.longitude_dir);
        printf("Cidade: %s\n", temp->cidade);
        printf("TimeZone: %d\n", temp->timeZone);
        printf("\n");
        temp = temp->prox;
    }
}
