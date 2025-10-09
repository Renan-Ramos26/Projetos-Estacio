
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
  Sistema de priorização e montagem de componentes da torre de fuga.
  - Implementa três algoritmos de ordenação:
    * Bubble sort por nome (strings)
    * Insertion sort por tipo (strings)
    * Selection sort por prioridade (inteiros)
  - Contabiliza comparações e mede tempo de execução (clock)
  - Permite busca binária por nome (após ordenação por nome)
  - Interface de menu para interação do jogador
*/

#define MAX_COMPONENTES 20

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // 1 a 10
} Componente;

/* Mostra todos os componentes no formato: índice, nome, tipo, prioridade */
void mostrarComponentes(Componente vet[], int n) {
    if (n == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    printf("\n--- Componentes (total: %d) ---\n", n);
    for (int i = 0; i < n; i++) {
        printf("%2d. %-28s | %-10s | prioridade: %d\n",
               i + 1, vet[i].nome, vet[i].tipo, vet[i].prioridade);
    }
    printf("-------------------------------\n");
}

/* Bubble sort por nome (alfabético). Retorna número de comparações e tempo em ms */
void bubbleSortNome(Componente vet[], int n, long *comparacoes, double *tempo_ms) {
    *comparacoes = 0;
    clock_t start = clock();

    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(vet[j].nome, vet[j + 1].nome) > 0) {
                Componente tmp = vet[j];
                vet[j] = vet[j + 1];
                vet[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; // otimização: se não houve troca, já está ordenado
    }

    clock_t end = clock();
    *tempo_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

/* Insertion sort por tipo (alfabético). Retorna comparações e tempo */
void insertionSortTipo(Componente vet[], int n, long *comparacoes, double *tempo_ms) {
    *comparacoes = 0;
    clock_t start = clock();

    for (int i = 1; i < n; i++) {
        Componente chave = vet[i];
        int j = i - 1;
        // mover elementos maiores que a chave para a direita
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(vet[j].tipo, chave.tipo) > 0) {
                vet[j + 1] = vet[j];
                j--;
            } else break;
        }
        vet[j + 1] = chave;
    }

    clock_t end = clock();
    *tempo_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

/* Selection sort por prioridade (menor para maior). Retorna comparações e tempo */
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes, double *tempo_ms) {
    *comparacoes = 0;
    clock_t start = clock();

    for (int i = 0; i < n - 1; i++) {
        int idxMin = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (vet[j].prioridade < vet[idxMin].prioridade) {
                idxMin = j;
            }
        }
        if (idxMin != i) {
            Componente tmp = vet[i];
            vet[i] = vet[idxMin];
            vet[idxMin] = tmp;
        }
    }

    clock_t end = clock();
    *tempo_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

/* Busca binária por nome. Requer que o vetor esteja ordenado por nome.
   Retorna índice (>=0) se encontrado, -1 caso contrário. Também preenche comparações. */
int buscaBinariaPorNome(Componente vet[], int n, const char chave[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(vet[meio].nome, chave);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

/* Função auxiliar para limpar newline deixado por fgets ou stdin */
void removerQuebra(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

/* Menu de cadastro de componentes (até MAX_COMPONENTES) */
void cadastrarComponentes(Componente vet[], int *n) {
    int qtd;
    printf("Quantos componentes deseja cadastrar? (max %d): ", MAX_COMPONENTES);
    if (scanf("%d", &qtd) != 1) { // validação simples
        while (getchar() != '\\n'); // limpar buffer
        printf("Entrada inválida.\n");
        return;
    }
    if (qtd < 1) {
        printf("Nenhum componente cadastrado.\n");
        getchar();
        return;
    }
    if (qtd > MAX_COMPONENTES) qtd = MAX_COMPONENTES;
    getchar(); // limpar newline

    for (int i = 0; i < qtd; i++) {
        printf("\nComponente %d:\n", i + 1);
        printf("Nome (ex: chip central): ");
        fgets(vet[i].nome, sizeof(vet[i].nome), stdin);
        removerQuebra(vet[i].nome);

        printf("Tipo (controle/suporte/propulsao/...): ");
        fgets(vet[i].tipo, sizeof(vet[i].tipo), stdin);
        removerQuebra(vet[i].tipo);

        int pr;
        do {
            printf("Prioridade (1-10): ");
            if (scanf("%d", &pr) != 1) {
                while (getchar() != '\\n');
                pr = -1;
            } else {
                while (getchar() != '\\n');
            }
            if (pr < 1 || pr > 10) printf("Prioridade inválida. Tente novamente.\n");
        } while (pr < 1 || pr > 10);
        vet[i].prioridade = pr;
    }
    *n = qtd;
    printf("\nCadastro concluído. %d componentes registrados.\n", *n);
}

/* Programa principal com menu interativo */
int main() {
    Componente componentes[MAX_COMPONENTES];
    int n = 0;
    int opcao;

    printf("=== MÓDULO: MONTAGEM DA TORRE DE FUGA - PRIORIDADES ===\n");

    do {
        printf("\nMenu principal:\n");
        printf("1 - Cadastrar componentes\n");
        printf("2 - Mostrar componentes\n");
        printf("3 - Ordenar por NOME (Bubble Sort)\n");
        printf("4 - Ordenar por TIPO (Insertion Sort)\n");
        printf("5 - Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6 - Buscar componente-CHAVE por nome (Busca Binária)\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\\n');
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        getchar(); // limpar buffer

        if (opcao == 1) {
            cadastrarComponentes(componentes, &n);
        } else if (opcao == 2) {
            mostrarComponentes(componentes, n);
        } else if (opcao == 3) {
            long comps = 0;
            double tempo = 0.0;
            bubbleSortNome(componentes, n, &comps, &tempo);
            printf("\nBubble Sort concluído: comparações = %ld, tempo = %.3f ms\n", comps, tempo);
            mostrarComponentes(componentes, n);
        } else if (opcao == 4) {
            long comps = 0;
            double tempo = 0.0;
            insertionSortTipo(componentes, n, &comps, &tempo);
            printf("\nInsertion Sort concluído: comparações = %ld, tempo = %.3f ms\n", comps, tempo);
            mostrarComponentes(componentes, n);
        } else if (opcao == 5) {
            long comps = 0;
            double tempo = 0.0;
            selectionSortPrioridade(componentes, n, &comps, &tempo);
            printf("\nSelection Sort concluído: comparações = %ld, tempo = %.3f ms\n", comps, tempo);
            mostrarComponentes(componentes, n);
        } else if (opcao == 6) {
            if (n == 0) {
                printf("Nenhum componente cadastrado.\n");
                continue;
            }
            // Requer que o vetor esteja ordenado por nome para busca binária
            printf("A busca binária exige que os componentes estejam ordenados por nome.\n");
            printf("Deseja ordenar por nome agora antes de buscar? (s/n): ");
            char resposta = getchar();
            while (getchar() != '\\n'); // limpar resto da linha
            if (resposta == 's' || resposta == 'S') {
                long comps = 0;
                double tempo = 0.0;
                bubbleSortNome(componentes, n, &comps, &tempo);
                printf("Ordenado por nome: comparações = %ld, tempo = %.3f ms\n", comps, tempo);
            } else {
                printf("Prosseguindo sem garantir ordenação (pode não funcionar corretamente).\n");
            }

            char chave[30];
            printf("\nDigite o nome do componente-chave para buscar: ");
            fgets(chave, sizeof(chave), stdin);
            removerQuebra(chave);

            long compsBusca = 0;
            int idx = buscaBinariaPorNome(componentes, n, chave, &compsBusca);
            if (idx >= 0) {
                printf("\nComponente encontrado no índice %d:\n", idx + 1);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
                printf("Comparações na busca binária: %ld\n", compsBusca);
            } else {
                printf("\nComponente não encontrado. Comparações: %ld\n", compsBusca);
            }
        } else if (opcao == 0) {
            printf("Encerrando módulo. Boa sorte na fuga!\n");
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
