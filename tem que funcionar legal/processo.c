
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

Processo* ler_processos_csv(const char* nome_arquivo, int* quantidade) {
    FILE* arq = fopen(nome_arquivo, "r");
    if (!arq) {
        perror("Erro ao abrir o arquivo CSV");
        return NULL;
    }

    int capacidade = 1000;
    Processo* processos = malloc(capacidade * sizeof(Processo));
    char linha[1024];
    int count = 0;

    fgets(linha, sizeof(linha), arq); // cabeçalho

    while (fgets(linha, sizeof(linha), arq)) {
        if (count >= capacidade) {
            capacidade *= 2;
            processos = realloc(processos, capacidade * sizeof(Processo));
        }

        char* token;
        token = strtok(linha, ",");
        processos[count].id = atoi(token);

        token = strtok(NULL, ",");
        strncpy(processos[count].numero, token, sizeof(processos[count].numero));

        token = strtok(NULL, ",");
        strncpy(processos[count].data_ajuizamento, token, sizeof(processos[count].data_ajuizamento));

        token = strtok(NULL, ",");
        processos[count].qtd_classes = 0;
        if (token[0] == '"') token++;
        if (token[0] == '{') token++;
        char* classe_token = strtok(token, ",}");
        while (classe_token && processos[count].qtd_classes < MAX_CLASSES) {
            processos[count].id_classe[processos[count].qtd_classes++] = atoi(classe_token);
            classe_token = strtok(NULL, ",}");
        }

        token = strtok(NULL, ",");
        processos[count].qtd_assuntos = 0;
        if (token[0] == '"') token++;
        if (token[0] == '{') token++;
        char* assunto_token = strtok(token, ",}");
        while (assunto_token && processos[count].qtd_assuntos < MAX_ASSUNTOS) {
            processos[count].id_assunto[processos[count].qtd_assuntos++] = atoi(assunto_token);
            assunto_token = strtok(NULL, ",}");
        }

        token = strtok(NULL, ",
");
        processos[count].ano_eleicao = atoi(token);

        count++;
    }

    fclose(arq);
    *quantidade = count;
    return processos;
}

void ordenar_por_id(Processo* processos, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processos[j].id > processos[j + 1].id) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

int converter_data_para_int(const char* data_str) {
    int ano, mes, dia;
    sscanf(data_str, "%d-%d-%d", &ano, &mes, &dia);
    return ano * 10000 + mes * 100 + dia;
}

void ordenar_por_data(Processo* processos, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int data_j = converter_data_para_int(processos[j].data_ajuizamento);
            int data_j1 = converter_data_para_int(processos[j + 1].data_ajuizamento);
            if (data_j < data_j1) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

int contar_por_classe(Processo* processos, int n, int classe_busca) {
    int contador = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            if (processos[i].id_classe[j] == classe_busca) {
                contador++;
                break;
            }
        }
    }
    return contador;
}

int contar_assuntos_distintos(Processo* processos, int n) {
    int assuntos[10000] = {0};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            assuntos[processos[i].id_assunto[j]] = 1;
        }
    }
    int count = 0;
    for (int i = 0; i < 10000; i++) {
        if (assuntos[i]) count++;
    }
    return count;
}

void listar_processos_varios_assuntos(Processo* processos, int n) {
    for (int i = 0; i < n; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("ID: %d | Numero: %s | Assuntos: %d\n", processos[i].id, processos[i].numero, processos[i].qtd_assuntos);
        }
    }
}

int dias_em_tramitacao(Processo* processos, int n, int id_busca) {
    for (int i = 0; i < n; i++) {
        if (processos[i].id == id_busca) {
            struct tm data = {0};
            sscanf(processos[i].data_ajuizamento, "%d-%d-%d", &data.tm_year, &data.tm_mon, &data.tm_mday);
            data.tm_year -= 1900;
            data.tm_mon -= 1;
            time_t ajuizamento = mktime(&data);
            time_t agora = time(NULL);
            return (int) difftime(agora, ajuizamento) / (60 * 60 * 24);
        }
    }
    return -1;
}
