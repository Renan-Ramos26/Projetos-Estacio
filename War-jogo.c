
#include <stdio.h>
#include <string.h>

// Definição da estrutura (struct) que representa um território
struct Territorio {
    char nome[30];   // Nome do território
    char cor[10];    // Cor do exército associado
    int tropas;      // Quantidade de tropas no território
};

int main() {
    struct Territorio territorios[5]; // Vetor que armazena 5 territórios
    int i;

    printf("=== Cadastro de Territórios ===\n\n");

    // Laço para cadastro dos 5 territórios
    for (i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);

        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        getchar(); // Limpa o buffer de entrada (remove o '\n' deixado pelo scanf)
        printf("\n");
    }

    // Exibição dos dados cadastrados
    printf("=== Dados dos Territórios Cadastrados ===\n\n");

    for (i = 0; i < 5; i++) {
        printf("Território %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }

    printf("Cadastro concluído com sucesso!\n");
    return 0;
}
