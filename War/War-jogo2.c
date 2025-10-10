#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ================================
// ESTRUTURA PRINCIPAL
// ================================
typedef struct {
    char nome[30];   // Nome do território
    char cor[10];    // Cor (ou dono) do território
    int tropas;      // Quantidade de tropas
} Territorio;

// ================================
// FUNÇÕES
// ================================

// Função responsável por cadastrar os territórios dinamicamente
void cadastrarTerritorios(Territorio* mapa, int qtd) {
    printf("=== Cadastro de Territórios ===\n\n");
    for (int i = 0; i < qtd; i++) {
        printf("Território %d:\n", i + 1);

        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // limpar buffer
        printf("\n");
    }
}

// Função que exibe todos os territórios
void exibirTerritorios(Territorio* mapa, int qtd) {
    printf("\n=== Territórios Registrados ===\n\n");
    for (int i = 0; i < qtd; i++) {
        printf("Território %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exército: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
        printf("-----------------------------\n");
    }
}

// Função que simula o ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n--- Simulação de Ataque ---\n");
    printf("%s (%s) está atacando %s (%s)\n",
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);

    // Rolagem de dados (1 a 6) para ambos
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    // Resultado do ataque
    if (dadoAtacante > dadoDefensor) {
        printf(">> O atacante venceu a batalha!\n");
        strcpy(defensor->cor, atacante->cor); // defensor muda de cor (perde o território)
        defensor->tropas = atacante->tropas / 2; // metade das tropas do atacante ocupam o território
        atacante->tropas /= 2; // atacante perde metade das tropas
    } else if (dadoAtacante < dadoDefensor) {
        printf(">> O defensor resistiu ao ataque!\n");
        atacante->tropas--; // atacante perde uma tropa
    } else {
        printf(">> Empate! Nenhuma tropa foi perdida.\n");
    }
}

// Função para liberar memória
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso.\n");
}

// ================================
// FUNÇÃO PRINCIPAL
// ================================
int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios

    int qtd;
    printf("Digite o número de territórios a cadastrar: ");
    scanf("%d", &qtd);
    getchar(); // limpar buffer

    // Alocação dinâmica de memória
    Territorio* mapa = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Cadastro e exibição inicial
    cadastrarTerritorios(mapa, qtd);
    exibirTerritorios(mapa, qtd);

    // Simulação de ataque
    int atk, def;
    printf("\nEscolha o número do território atacante: ");
    scanf("%d", &atk);
    printf("Escolha o número do território defensor: ");
    scanf("%d", &def);

    // Verificação de validade
    if (atk < 1 || atk > qtd || def < 1 || def > qtd || atk == def) {
        printf("Escolhas inválidas!\n");
        liberarMemoria(mapa);
        return 0;
    }

    // Verifica se o atacante e defensor têm cores diferentes
    if (strcmp(mapa[atk - 1].cor, mapa[def - 1].cor) == 0) {
        printf("\nUm território não pode atacar outro da mesma cor!\n");
        liberarMemoria(mapa);
        return 0;
    }

    atacar(&mapa[atk - 1], &mapa[def - 1]); // chamada com ponteiros
    exibirTerritorios(mapa, qtd); // mostra resultado final

    liberarMemoria(mapa);
    return 0;
}
