
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// ESTRUTURAS DE DADOS
// ==========================================

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// ==========================================
// FUNÇÕES PARA VETOR
// ==========================================

void inserirItemVetor(Item mochila[], int *tamanho) {
    if (*tamanho >= 10) {
        printf("\nA mochila (vetor) está cheia!\n");
        return;
    }
    printf("\n--- Inserir Item (Vetor) ---\n");
    printf("Nome: ");
    fgets(mochila[*tamanho].nome, 30, stdin);
    mochila[*tamanho].nome[strcspn(mochila[*tamanho].nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(mochila[*tamanho].tipo, 20, stdin);
    mochila[*tamanho].tipo[strcspn(mochila[*tamanho].tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &mochila[*tamanho].quantidade);
    getchar();
    (*tamanho)++;
    printf("Item adicionado!\n");
}

void listarItensVetor(Item mochila[], int tamanho) {
    printf("\n--- Itens no Vetor ---\n");
    for (int i = 0; i < tamanho; i++)
        printf("%d. %s (%s) - %d\n", i+1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
}

void removerItemVetor(Item mochila[], int *tamanho) {
    char nome[30];
    printf("Nome do item para remover: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < *tamanho-1; j++)
                mochila[j] = mochila[j+1];
            (*tamanho)--;
            printf("Item removido!\n");
            return;
        }
    }
    printf("Item não encontrado!\n");
}

void ordenarVetor(Item mochila[], int tamanho) {
    Item temp;
    for (int i = 0; i < tamanho-1; i++) {
        for (int j = i+1; j < tamanho; j++) {
            if (strcmp(mochila[i].nome, mochila[j].nome) > 0) {
                temp = mochila[i];
                mochila[i] = mochila[j];
                mochila[j] = temp;
            }
        }
    }
    printf("Itens ordenados!\n");
}

void buscarSequencialVetor(Item mochila[], int tamanho) {
    char nome[30];
    comparacoesSequencial = 0;
    printf("Nome do item para buscar: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    for (int i = 0; i < tamanho; i++) {
        comparacoesSequencial++;
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("Encontrado: %s (%s) - %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            printf("Comparações: %d\n", comparacoesSequencial);
            return;
        }
    }
    printf("Item não encontrado. Comparações: %d\n", comparacoesSequencial);
}

void buscarBinariaVetor(Item mochila[], int tamanho) {
    char nome[30];
    comparacoesBinaria = 0;
    printf("Nome do item para busca binária: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    int ini=0, fim=tamanho-1;
    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        comparacoesBinaria++;
        int cmp = strcmp(nome, mochila[meio].nome);
        if (cmp == 0) {
            printf("Encontrado: %s (%s) - %d\n", mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade);
            printf("Comparações: %d\n", comparacoesBinaria);
            return;
        }
        if (cmp < 0) fim = meio - 1;
        else ini = meio + 1;
    }
    printf("Item não encontrado. Comparações: %d\n", comparacoesBinaria);
}

// ==========================================
// FUNÇÕES PARA LISTA
// ==========================================

No* inserirItemLista(No* inicio) {
    No* novo = (No*)malloc(sizeof(No));
    printf("Nome: ");
    fgets(novo->dados.nome, 30, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo->dados.tipo, 20, stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    getchar();
    novo->proximo = inicio;
    printf("Item adicionado!\n");
    return novo;
}

No* removerItemLista(No* inicio) {
    char nome[30];
    printf("Nome do item para remover: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    No* atual = inicio;
    No* anterior = NULL;
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) {
        printf("Item não encontrado!\n");
        return inicio;
    }
    if (anterior == NULL) inicio = atual->proximo;
    else anterior->proximo = atual->proximo;
    free(atual);
    printf("Item removido!\n");
    return inicio;
}

void listarItensLista(No* inicio) {
    No* atual = inicio;
    int i=1;
    while (atual != NULL) {
        printf("%d. %s (%s) - %d\n", i++, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
}

void buscarSequencialLista(No* inicio) {
    char nome[30];
    printf("Nome do item para buscar: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    int comp = 0;
    No* atual = inicio;
    while (atual != NULL) {
        comp++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            printf("Encontrado: %s (%s) - %d\n", atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            printf("Comparações: %d\n", comp);
            return;
        }
        atual = atual->proximo;
    }
    printf("Item não encontrado. Comparações: %d\n", comp);
}

// ==========================================
// MAIN
// ==========================================

int main() {
    Item mochila[10];
    int tamanho = 0;
    No* lista = NULL;
    int op, estrutura;

    do {
        printf("\nEscolha estrutura:\n1 - Vetor\n2 - Lista Encadeada\n0 - Sair\n> ");
        scanf("%d", &estrutura);
        getchar();

        if (estrutura == 1) {
            do {
                printf("\n--- MENU VETOR ---\n1-Inserir\n2-Remover\n3-Listar\n4-Ordenar\n5-Busca Seq.\n6-Busca Binária\n0-Voltar\n> ");
                scanf("%d", &op);
                getchar();
                switch (op) {
                    case 1: inserirItemVetor(mochila, &tamanho); break;
                    case 2: removerItemVetor(mochila, &tamanho); break;
                    case 3: listarItensVetor(mochila, tamanho); break;
                    case 4: ordenarVetor(mochila, tamanho); break;
                    case 5: buscarSequencialVetor(mochila, tamanho); break;
                    case 6: buscarBinariaVetor(mochila, tamanho); break;
                }
            } while (op != 0);
        } else if (estrutura == 2) {
            do {
                printf("\n--- MENU LISTA ---\n1-Inserir\n2-Remover\n3-Listar\n4-Buscar\n0-Voltar\n> ");
                scanf("%d", &op);
                getchar();
                switch (op) {
                    case 1: lista = inserirItemLista(lista); break;
                    case 2: lista = removerItemLista(lista); break;
                    case 3: listarItensLista(lista); break;
                    case 4: buscarSequencialLista(lista); break;
                }
            } while (op != 0);
        }
    } while (estrutura != 0);

    printf("Encerrando...\n");
    return 0;
}
