#include "cidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compararCidades(const void *a, const void *b) {
    return ((Cidade *)a)->Posicao - ((Cidade *)b)->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        perror("Erro de alocacao de memoria");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &estrada->T);
    fscanf(file, "%d", &estrada->N);

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        perror("Erro de alocacao de memoria");
        fclose(file);
        free(estrada);
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        if (fscanf(file, "%d ", &estrada->C[i].Posicao) != 1) {
            fprintf(stderr, "Erro ao ler posicao da cidade %d\n", i);
            fclose(file);
            free(estrada->C);
            free(estrada);
            return NULL;
        }
        fgets(estrada->C[i].Nome, 256, file);
        estrada->C[i].Nome[strcspn(estrada->C[i].Nome, "\n")] = '\0'; 

        for (int j = 0; j < i; j++) {
            if (estrada->C[i].Posicao == estrada->C[j].Posicao) {
                fprintf(stderr, "Erro: posicao da cidade duplicada\n");
                fclose(file);
                free(estrada->C);
                free(estrada);
                return NULL;
            }
        }
    }

    fclose(file);
    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada)
        return -1.0; 


    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    double menorVizinhanca = estrada->T; 

    for (int i = 0; i < estrada->N; i++) {
        double inicio = (i == 0) ? 0 : (estrada->C[i].Posicao + estrada->C[i-1].Posicao) / 2.0;
        double fim = (i == estrada->N - 1) ? estrada->T : (estrada->C[i].Posicao + estrada->C[i+1].Posicao) / 2.0;
        double vizinhanca = fim - inicio;

        if (vizinhanca < menorVizinhanca)
            menorVizinhanca = vizinhanca;
    }

    free(estrada->C);
    free(estrada);

    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada)
        return NULL;


    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    double menorVizinhanca = estrada->T; 
    char *cidadeMenor = NULL;

    for (int i = 0; i < estrada->N; i++) {
        double inicio = (i == 0) ? 0 : (estrada->C[i].Posicao + estrada->C[i-1].Posicao) / 2.0;
        double fim = (i == estrada->N - 1) ? estrada->T : (estrada->C[i].Posicao + estrada->C[i+1].Posicao) / 2.0;
        double vizinhanca = fim - inicio;

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;

            cidadeMenor = malloc(strlen(estrada->C[i].Nome) + 1);
            if (!cidadeMenor) {
                perror("Erro de alocação de memória");
                free(estrada->C);
                free(estrada);
                return NULL;
            }

            strcpy(cidadeMenor, estrada->C[i].Nome);
        }
    }

    free(estrada->C);
    free(estrada);

    return cidadeMenor;
}


int main() {
    const char *nomeArquivo = "estradas.txt";

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    char *cidadeMenor = cidadeMenorVizinhanca(nomeArquivo);

    printf("Menor vizinhanca de estrada: %.2f\n", menorVizinhanca);
    printf("Cidade com menor vizinhanca: %s\n", cidadeMenor);

    return 0;
}