#include <stdio.h>
#include "ordenacao_por_id.h"
#include "ordenacao_por_data.h"

int main() {
    int opcao;
    const char *entrada = "processo_043_202409032338.csv";

    do {
        printf("\nEscolha uma opção:\n");
        printf("1 - Ordenar por ID\n");
        printf("2 - Ordenar por Data\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                ordenar_por_id(entrada, "OrdenadoID.csv");
                break;
            case 2:
                ordenar_por_data(entrada, "OrdenadoData.csv");
                break;
            case 0:
                printf("Encerrando programa.\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
