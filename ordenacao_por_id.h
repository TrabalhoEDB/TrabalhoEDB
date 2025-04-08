#ifndef ORDENACAO_POR_ID_H
#define ORDENACAO_POR_ID_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 512

typedef struct {
    int id;
    char line[MAX_LEN];
} Gravacao;

int comparar_por_id(const void *a, const void *b) {
    return ((Gravacao *)a)->id - ((Gravacao *)b)->id;
}

void ordenar_por_id(const char *entrada, const char *saida) {
    FILE *file = fopen(entrada, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    FILE *output = fopen(saida, "w");
    if (!output) {
        perror("Erro ao criar o arquivo de saída");
        fclose(file);
        return;
    }

    char header[MAX_LEN];
    fgets(header, MAX_LEN, file);
    fprintf(output, "%s", header);

    int capacidade = 1000;
    Gravacao *records = malloc(capacidade * sizeof(Gravacao));
    int count = 0;

    while (fgets(records[count].line, MAX_LEN, file)) {
        sscanf(records[count].line, "%d;%*s", &records[count].id);
        count++;

        if (count >= capacidade) {
            capacidade *= 2;
            records = realloc(records, capacidade * sizeof(Gravacao));
        }
    }

    qsort(records, count, sizeof(Gravacao), comparar_por_id);

    for (int i = 0; i < count; i++) {
        fprintf(output, "%s", records[i].line);
    }

    free(records);
    fclose(file);
    fclose(output);
    printf("Arquivo ordenado por ID salvo como '%s'\n", saida);
}

#endif
