
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];             // Nome do cômodo
    struct Sala *esquerda;     // Caminho para a esquerda
    struct Sala *direita;      // Caminho para a direita
} Sala;

// Função que cria uma sala dinamicamente
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função que permite a exploração da mansão
void explorarSalas(Sala* salaAtual) {
    char escolha;

    while (salaAtual != NULL) {
        printf("\nVocê está na: %s\n", salaAtual->nome);

        // Verifica se a sala é um nó folha (sem caminhos)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Fim da exploração!\n");
            break;
        }

        printf("Escolha um caminho:\n");
        if (salaAtual->esquerda != NULL)
            printf("  [e] Ir para a esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf("  [d] Ir para a direita (%s)\n", salaAtual->direita->nome);
        printf("  [s] Sair da exploração\n> ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// Função principal: monta o mapa da mansão e inicia o jogo
int main() {
    // Criação manual da árvore binária da mansão
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");
    Sala* escritorio = criarSala("Escritório");
    Sala* porao = criarSala("Porão");

    // Ligações entre as salas (estrutura da árvore)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = escritorio;
    cozinha->direita = porao;

    // Introdução ao jogador
    printf("=====================================\n");
    printf("      DETECTIVE QUEST - MANSÃO\n");
    printf("=====================================\n");
    printf("Você está prestes a explorar a mansão!\n");
    printf("Começando pelo Hall de Entrada...\n");

    // Inicia a exploração
    explorarSalas(hall);

    // Libera memória (boa prática)
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(escritorio);
    free(porao);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
