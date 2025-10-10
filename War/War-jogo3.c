#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da estrutura que representa um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Prototipação das funções
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void exibirMissao(char* missao);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* territorios, int tamanho);
void liberarMemoria(char* missao);

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    Territorio* territorios = (Territorio*) malloc(5 * sizeof(Territorio));
    if (!territorios) {
        printf("Erro ao alocar memória para territórios.\n");
        return 1;
    }

    char* missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar ao menos 3 territórios",
        "Ter mais de 10 tropas em um único território",
        "Conquistar um território inimigo"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Aloca dinamicamente a missão do jogador
    char* missaoJogador = (char*) malloc(100 * sizeof(char));
    if (!missaoJogador) {
        printf("Erro ao alocar memória para missão.\n");
        free(territorios);
        return 1;
    }

    atribuirMissao(missaoJogador, missoes, totalMissoes);
    exibirMissao(missaoJogador);

    printf("\n=== Cadastro de Territórios ===\n\n");
    for (int i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);

        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // Limpa o buffer
        printf("\n");
    }

    printf("=== Início do Jogo ===\n");

    // Simula um ataque simples entre os dois primeiros territórios
    atacar(&territorios[0], &territorios[1]);

    // Verifica se a missão foi cumprida
    if (verificarMissao(missaoJogador, territorios, 5)) {
        printf("\nParabéns! Você cumpriu sua missão: %s\n", missaoJogador);
    } else {
        printf("\nMissão ainda não cumprida. Continue jogando.\n");
    }

    printf("\n=== Estado Atual do Mapa ===\n");
    exibirMapa(territorios, 5);

    liberarMemoria(missaoJogador);
    free(territorios);
    return 0;
}

// Função que sorteia uma missão para o jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibe a missão uma única vez
void exibirMissao(char* missao) {
    printf("Sua missão é: %s\n", missao);
}

// Verifica se a missão foi cumprida (lógica simples inicial)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    // Exemplo 1: "Controlar ao menos 3 territórios"
    if (strcmp(missao, "Controlar ao menos 3 territórios") == 0) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0) { // jogador azul como exemplo
                contador++;
            }
        }
        return contador >= 3;
    }

    // Exemplo 2: "Ter mais de 10 tropas em um único território"
    if (strcmp(missao, "Ter mais de 10 tropas em um único território") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0 && mapa[i].tropas > 10) {
                return 1;
            }
        }
    }

    // Outras missões podem ser adicionadas com lógica semelhante
    return 0;
}

// Simula um ataque entre territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n%s está atacando %s...\n", atacante->nome, defensor->nome);

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: ambos os territórios pertencem ao mesmo jogador.\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado atacante: %d\n", dadoAtacante);
    printf("Dado defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Vitória do atacante!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= defensor->tropas;
    } else {
        printf("Defensor resistiu ao ataque!\n");
        atacante->tropas -= 1;
    }
}

// Exibe o estado atual dos territórios
void exibirMapa(Territorio* territorios, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("Território %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }
}

// Libera memória alocada dinamicamente
void liberarMemoria(char* missao) {
    free(missao);
}
