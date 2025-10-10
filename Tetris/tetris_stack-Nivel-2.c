#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura da peça
typedef struct {
    char nome;
    int id;
} Peca;

// Tamanhos da fila e pilha
#define TAM_FILA 5
#define TAM_PILHA 3

// Fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int inicio, fim, quantidade;
} Fila;

// Pilha de reserva
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// Funções da fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->quantidade = 0;
}

int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }
int filaVazia(Fila *f) { return f->quantidade == 0; }

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->pecas[f->fim] = p;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

void exibirFila(Fila *f) {
    printf("Fila de pecas: ");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    int i, pos = f->inicio;
    for (i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->pecas[pos].nome, f->pecas[pos].id);
        pos = (pos + 1) % TAM_FILA;
    }
    printf("\n");
}

// Funções da pilha
void inicializarPilha(Pilha *p) { p->topo = -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("⚠️ Pilha de reserva cheia!\n");
        return;
    }
    p->pecas[++p->topo] = x;
}

Peca desempilhar(Pilha *p) {
    Peca vazio = {'-', -1};
    if (pilhaVazia(p)) {
        printf("⚠️ Pilha vazia!\n");
        return vazio;
    }
    return p->pecas[p->topo--];
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--)
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    printf("\n");
}

// Gerar nova peça
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// Programa principal
int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    int idAtual = 0, opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < TAM_FILA; i++)
        enfileirar(&fila, gerarPeca(idAtual++));

    do {
        printf("\n===== ESTADO ATUAL =====\n");
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("=========================\n");

        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("🎮 Jogando [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(idAtual++));
                } else printf("⚠️ Nenhuma peca na fila!\n");
                break;

            case 2:
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("💾 Reservando [%c %d]\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(idAtual++));
                } else printf("⚠️ Nao foi possivel reservar!\n");
                break;

            case 3:
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("🔧 Usando [%c %d]\n", usada.nome, usada.id);
                } else printf("⚠️ Nenhuma peca reservada!\n");
                break;

            case 0:
                printf("👋 Encerrando o jogo.\n");
                break;

            default:
                printf("❌ Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}
