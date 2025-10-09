
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ============================
// ESTRUTURA DE DADOS
// ============================

// Estrutura que representa um item do inventário
typedef struct {
    char nome[30];     // Nome do item
    char tipo[20];     // Tipo do item (arma, munição, cura etc)
    int quantidade;    // Quantidade do item
} Item;

// ============================
// PROTÓTIPOS DAS FUNÇÕES
// ============================
void inserirItem(Item mochila[], int *tamanho);
void removerItem(Item mochila[], int *tamanho);
void listarItens(Item mochila[], int tamanho);
void buscarItem(Item mochila[], int tamanho);

// ============================
// FUNÇÃO PRINCIPAL
// ============================
int main() {
    Item mochila[10];  // Vetor com capacidade para 10 itens
    int tamanho = 0;   // Quantidade atual de itens
    int opcao;

    printf("=== SISTEMA DE INVENTÁRIO - MOCHILA DE LOOT ===\n");

    do {
        printf("\nEscolha uma opção:\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1:
                inserirItem(mochila, &tamanho);
                break;
            case 2:
                removerItem(mochila, &tamanho);
                break;
            case 3:
                listarItens(mochila, tamanho);
                break;
            case 4:
                buscarItem(mochila, tamanho);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ============================
// FUNÇÕES DO SISTEMA
// ============================

// Função para inserir um item na mochila
void inserirItem(Item mochila[], int *tamanho) {
    if (*tamanho >= 10) {
        printf("\nA mochila está cheia! Remova algum item primeiro.\n");
        return;
    }

    printf("\n=== Cadastro de Item ===\n");

    printf("Nome do item: ");
    fgets(mochila[*tamanho].nome, sizeof(mochila[*tamanho].nome), stdin);
    mochila[*tamanho].nome[strcspn(mochila[*tamanho].nome, "\n")] = '\0';

    printf("Tipo do item (arma, munição, cura...): ");
    fgets(mochila[*tamanho].tipo, sizeof(mochila[*tamanho].tipo), stdin);
    mochila[*tamanho].tipo[strcspn(mochila[*tamanho].tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &mochila[*tamanho].quantidade);
    getchar();

    (*tamanho)++; // Incrementa o número de itens cadastrados
    printf("\nItem adicionado com sucesso!\n");

    listarItens(mochila, *tamanho);
}

// Função para remover um item pelo nome
void removerItem(Item mochila[], int *tamanho) {
    if (*tamanho == 0) {
        printf("\nA mochila está vazia! Nada para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja remover: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        for (int i = encontrado; i < *tamanho - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*tamanho)--;
        printf("\nItem '%s' removido com sucesso!\n", nomeBusca);
    } else {
        printf("\nItem não encontrado!\n");
    }

    listarItens(mochila, *tamanho);
}

// Função para listar todos os itens
void listarItens(Item mochila[], int tamanho) {
    printf("\n=== Itens na Mochila ===\n");
    if (tamanho == 0) {
        printf("A mochila está vazia!\n");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função de busca sequencial pelo nome do item
void buscarItem(Item mochila[], int tamanho) {
    if (tamanho == 0) {
        printf("\nA mochila está vazia! Nada para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\nItem não encontrado na mochila.\n");
}
