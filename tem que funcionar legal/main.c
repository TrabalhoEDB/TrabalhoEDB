#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

int main() {
    int opcao;
    const char *entrada = "csv.csv";

    int total = 0;
    Processo* processos = ler_processos_csv(entrada, &total);

    if (!processos || total == 0) {
        printf("Nenhum processo lido.\n");
        return 1;
    }

    do {
        printf("1 - Ordenar por ID e salvar\n");
        printf("2 - Ordenar por Data e salvar\n");
        printf("3 - Contar processos por Classe\n");
        printf("4 - Contar Assuntos distintos\n");
        printf("5 - Listar com mais de um Assunto\n");
        printf("6 - Dias em tramitação por ID\n");
        printf("7 - Listar todos os processos\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                ordenar_por_id(processos, total);
                salvar_em_csv("OrdenadoID.csv", processos, total);
                break;

            case 2:
                ordenar_por_data(processos, total);
                salvar_em_csv("OrdenadoData.csv", processos, total);
                break;

            case 3: {
                int classe;
                printf("Digite o ID da classe: ");
                scanf("%d", &classe);
                int total_classe = contar_por_classe(processos, total, classe);
                printf("Total com classe %d: %d\n", classe, total_classe);
                break;
            }

            case 4: {
                int distintos = contar_assuntos_distintos(processos, total);
                printf("Assuntos distintos: %d\n", distintos);
                break;
            }

            case 5:
                listar_processos_varios_assuntos(processos, total);
                break;

            case 6: {
                int id;
                printf("Digite o ID do processo: ");
                scanf("%d", &id);
                dias_em_tramitacao(processos, total, id);
                break;
            }

            case 7:
                listar_processos(processos, total);
                break;

            case 0:
                printf("Encerrando programa.\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    free(processos);
    return 0;
}
