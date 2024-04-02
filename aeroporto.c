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
    return novoAeroporto;
}

AeroportoNode *criarAeroportoNode(Aeroporto *aeroporto) {
    AeroportoNode *novoAeroportoNode = (AeroportoNode *)calloc(1, sizeof(AeroportoNode));
    if (novoAeroportoNode == NULL) {
        printf("Erro ao alocar memória para o nó do aeroporto.\n");
        exit(1);
    }
    novoAeroportoNode->aeroporto = aeroporto;
    novoAeroportoNode->direita = NULL;
    novoAeroportoNode->esquerda = NULL;
    return novoAeroportoNode;
}

/*----------------------------------------------------------------------------------------
| Nome: obterValorLetra
| Ação: Recebe como argumento uma letra e converte essa letra para maiuscula utilizando a
        função toupper. Subtrai o caractere 'A' do caractere maiúsculo resultante para 
        obter um deslocamento em relação à letra 'A'. O deslocamento é convertido para
         o tipo ValorLetra
| Devolve: Devolve o deslocamento convertido para o tipo ValorLetra
------------------------------------------------------------------------------------------*/

AeroportoNode *inserirNo(AeroportoNode *raiz, Aeroporto *aeroporto) {
    if (raiz == NULL) {
        return criarAeroportoNode(aeroporto);
    }

    int comparacao = strcmp(aeroporto->cidade, raiz->aeroporto->cidade);

    if (comparacao < 0) {
        raiz->esquerda = inserirNo(raiz->esquerda, aeroporto);
    } else if (comparacao > 0) {
        raiz->direita = inserirNo(raiz->direita, aeroporto);
    } else {
        // A cidade do novo aeroporto é a mesma que a cidade do aeroporto existente
        // Inserir o novo aeroporto na lista encadeada dentro do nó da árvore
        AeroportoNode *novoNo = criarAeroportoNode(aeroporto);
        novoNo->esquerda = raiz->esquerda; // Aponta para a lista existente
        raiz->esquerda = novoNo; // Atualiza a referência para a nova lista
    }

    return raiz;
}

// Função para validar uma linha do arquivo de entrada
bool validarLinha(char *linha, Aeroporto *novoAeroporto, int numLinha, int *numErros, int linhasErro[]) {
    if ((sscanf(linha, "%s %s %d %d %d %c %d %d %d %c %s %d",
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
               &novoAeroporto->timeZone)) != 12 ||
        (novoAeroporto->localizacao.latitude_dir != 'N' && novoAeroporto->localizacao.latitude_dir != 'S') ||
        (novoAeroporto->localizacao.longitude_dir != 'W' && novoAeroporto->localizacao.longitude_dir != 'E') ||
        novoAeroporto->timeZone < -12 || novoAeroporto->timeZone > 14) {
        linhasErro[*numErros] = numLinha;
        (*numErros)++;
        return false;
    }

    return true;
}

void lerAeroportos(char *nomeArquivo, AeroportoNode **raiz, int *numErros, int linhasErro[]) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }
    char linha[100];
    int numLinha = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        numLinha++;
        Aeroporto *novoAeroporto = criarAeroporto();
        if (!validarLinha(linha, novoAeroporto, numLinha, numErros, linhasErro)) {
            free(novoAeroporto);
            continue;
        }
        *raiz = inserirNo(*raiz, novoAeroporto);
    }

    fclose(arquivo);
}

void libertarArvore(AeroportoNode *raiz) {
    if (raiz != NULL) {
        libertarArvore(raiz->esquerda);
        libertarArvore(raiz->direita);
        free(raiz->aeroporto); // Liberar a memória alocada para o Aeroporto
        free(raiz);
    }
}

void imprimirEmOrdem(AeroportoNode *raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esquerda);
        imprimirAeroporto(raiz->aeroporto);
        imprimirEmOrdem(raiz->direita);
    }
}

void imprimirAeroporto(Aeroporto *aeroporto) {
    printf("ICAO: %s\n", aeroporto->identificador_ICAO);
    printf("IATA: %s\n", aeroporto->identificador_IATA);
    printf("Latitude: %d %d %d %c\n", aeroporto->localizacao.latitude_hora, aeroporto->localizacao.latitude_minuto, aeroporto->localizacao.latitude_segundo, aeroporto->localizacao.latitude_dir);
    printf("Longitude: %d %d %d %c\n", aeroporto->localizacao.longitude_hora, aeroporto->localizacao.longitude_minuto, aeroporto->localizacao.longitude_segundo, aeroporto->localizacao.longitude_dir);
    printf("Cidade: %s\n", aeroporto->cidade);
    printf("TimeZone: %d\n", aeroporto->timeZone);
    printf("\n");
}

Aeroporto* buscarAeroportoPorIATA(AeroportoNode *raiz, char *codigoIATA) {
    if (raiz == NULL) {
        return NULL; // Se a raiz for nula, o aeroporto não foi encontrado
    }

    // Comparar o código IATA atual com o código desejado
    int comparacao = strcmp(raiz->aeroporto->identificador_IATA, codigoIATA);

    // Se encontrarmos o aeroporto com o código IATA desejado, retornamos a raiz
    if (comparacao == 0) {
        return raiz->aeroporto;
    }

    // Se o código IATA desejado for menor, procuramos na subárvore esquerda
    if (comparacao > 0) {
        return buscarAeroportoPorIATA(raiz->esquerda, codigoIATA);
    }
    // Se o código IATA desejado for maior, procuramos na subárvore direita
    else {
        return buscarAeroportoPorIATA(raiz->direita, codigoIATA);
    }
}
