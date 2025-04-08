#ifndef ORDENACAO_POR_DATA_H
#define ORDENACAO_POR_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 512

typedef struct {
    char line[MAX_LEN];
    int data;
} Registro;

int converter_data(const char *data_str) {
    int dia, mes, ano;
    sscanf(data_str, "%d-%d-%d", &ano, &mes, &dia);
    return ano * 10000 + mes * 100 + dia;
}

int comparar_data_decrescente(const void *a, const void *b) {
    return ((Registro *)b)->data - ((Registro *)a)->data;
}

void ordenar_por_data(const char *entrada, const char *saida) {
    FILE *entrada_arq = fopen(entrada, "r");
    if (!entrada_arq) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    FILE *saida_arq = fopen(saida, "w");
    if (!saida_arq) {
        perror("Erro ao criar o arquivo de saída");
        fclose(entrada_arq);
        return;
    }

    char cabecalho[MAX_LEN];
    fgets(cabecalho, MAX_LEN, entrada_arq);
    fprintf(saida_arq, "%s", cabecalho);

    int coluna_data = -1;
    char *copia_cabecalho = strdup(cabecalho);
    char *token = strtok(copia_cabecalho, ";");
    for (int i = 0; token; i++, token = strtok(NULL, ";")) {
        if (strcmp(token, "data_ajuizamento") == 0) {
            coluna_data = i;
            break;
        }
    }
    free(copia_cabecalho);

    if (coluna_data == -1) {
        fprintf(stderr, "Coluna 'data_ajuizamento' não encontrada.\n");
        fclose(entrada_arq);
        fclose(saida_arq);
        return;
    }

    Registro *registros = malloc(1000 * sizeof(Registro));
    int capacidade = 1000, total = 0;

    while (fgets(registros[total].line, MAX_LEN, entrada_arq)) {
        char *linha = strdup(registros[total].line);
        char *campo = strtok(linha, ",");
        for (int i = 0; i < coluna_data; i++) campo = strtok(NULL, ",");
        registros[total].data = campo ? converter_data(campo) : 0;
        free(linha);

        total++;
        if (total >= capacidade) {
            capacidade *= 2;
            registros = realloc(registros, capacidade * sizeof(Registro));
        }
    }

    qsort(registros, total, sizeof(Registro), comparar_data_decrescente);

    for (int i = 0; i < total; i++) {
        fputs(registros[i].line, saida_arq);
    }

    free(registros);
    fclose(entrada_arq);
    fclose(saida_arq);
    printf("Arquivo ordenado por data salvo como '%s'\n", saida);
}

#endif
