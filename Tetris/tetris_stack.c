
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5 // capacidade máxima da fila

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;  // posição da próxima remoção
    int tras;    // posição da próxima inserção
    int tamanho; // quantidade atual de peças na fila
} Fila;

// ---------- Funções da Fila ----------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

// Verifica se a fila está vazia
int estaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Verifica se a fila está cheia
int estaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // escolhe tipo aleatório
    nova.id = id;
    return nova;
}

// Insere nova peça (enqueue)
void enqueue(Fila *f, Peca p) {
    if (estaCheia(f)) {
        printf("\nFila cheia! Não é possível adicionar mais peças.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAM_FILA;
    f->elementos[f->tras] = p;
    f->tamanho++;
    printf("\nPeça [%c %d] adicionada à fila.\n", p.nome, p.id);
}

// Remove uma peça (dequeue)
void dequeue(Fila *f) {
    if (estaVazia(f)) {
        printf("\nFila vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca jogada = f->elementos[f->frente];
    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
    f->frente = (f->frente + 1) % TAM_FILA;
    f->tamanho--;
}

// Mostra todas as peças da fila
void mostrarFila(Fila *f) {
    if (estaVazia(f)) {
        printf("\nA fila está vazia.\n");
        return;
    }
    printf("\nFila de peças:\n");
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->elementos[idx].nome, f->elementos[idx].id);
    }
    printf("\n");
}

// ---------- Programa principal ----------
int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL)); // inicializa gerador de números aleatórios

    // Inicializa com 5 peças automáticas
    int contadorId = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(contadorId++));
    }

    int opcao;
    do {
        mostrarFila(&fila);
        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue(&fila);
                break;
            case 2:
                enqueue(&fila, gerarPeca(contadorId++));
                break;
            case 0:
                printf("\nEncerrando o jogo. Até a próxima!\n");
                break;
            default:
                printf("\nOpção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}
