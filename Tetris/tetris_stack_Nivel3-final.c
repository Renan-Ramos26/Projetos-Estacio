/*
  tetris_stack_final.c
  Gerenciador avançado de peças - Fila circular (5) + Pilha (3)
  Operações suportadas:
    1 - Jogar peça (dequeue)
    2 - Enviar peça da fila para a pilha (reservar)
    3 - Usar peça reservada (pop)
    4 - Trocar peça da frente da fila com o topo da pilha
    5 - Trocar os 3 primeiros da fila com as 3 da pilha (quando possível)
    0 - Sair
  Observações:
    - A cada remoção da fila (jogar ou reservar) a fila é reposta com uma nova peça gerada automaticamente
    - A pilha tem capacidade limitada a 3 peças
    - Peças têm 'nome' (char: 'I','O','T','L') e 'id' (inteiro único)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Configurações das estruturas */
#define TAM_FILA 5
#define TAM_PILHA 3

/* Representação de uma peça */
typedef struct {
    char nome; /* 'I', 'O', 'T', 'L' */
    int id;    /* identificador único */
} Peca;

/* Fila circular */
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;    /* índice do elemento da frente */
    int fim;       /* índice do último inserido */
    int quantidade;/* quantidade atual na fila */
} Fila;

/* Pilha LIFO */
typedef struct {
    Peca itens[TAM_PILHA];
    int topo; /* -1 quando vazia */
} Pilha;

/* ----- Protótipos ----- */
void inicializarFila(Fila *f);
int filaVazia(const Fila *f);
int filaCheia(const Fila *f);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void mostrarFila(const Fila *f);

void inicializarPilha(Pilha *p);
int pilhaVazia(const Pilha *p);
int pilhaCheia(const Pilha *p);
void empilhar(Pilha *p, Peca x);
Peca desempilhar(Pilha *p);
void mostrarPilha(const Pilha *p);

Peca gerarPeca(int id);
void mostrarEstado(const Fila *f, const Pilha *p);

void jogarPeca(Fila *f, Pilha *p, int *idCounter);
void reservarPeca(Fila *f, Pilha *p, int *idCounter);
void usarPecaReservada(Pilha *p);
void trocarAtual(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);

/* ----- Implementação: fila ----- */
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->quantidade = 0;
}

int filaVazia(const Fila *f) {
    return f->quantidade == 0;
}

int filaCheia(const Fila *f) {
    return f->quantidade == TAM_FILA;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->itens[f->fim] = p;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca vazio = {'-', -1};
    if (filaVazia(f)) return vazio;
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

void mostrarFila(const Fila *f) {
    printf("Fila de peças\t");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    int pos = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->itens[pos].nome, f->itens[pos].id);
        pos = (pos + 1) % TAM_FILA;
    }
    printf("\n");
}

/* ----- Implementação: pilha ----- */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(const Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(const Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = x;
}

Peca desempilhar(Pilha *p) {
    Peca vazio = {'-', -1};
    if (pilhaVazia(p)) return vazio;
    return p->itens[p->topo--];
}

void mostrarPilha(const Pilha *p) {
    printf("Pilha de reserva\t(Topo -> base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

/* ----- Geração automática de peças ----- */
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

/* ----- Operações de alto nível ----- */
void mostrarEstado(const Fila *f, const Pilha *p) {
    printf("\nEstado atual:\n\n");
    mostrarFila(f);
    mostrarPilha(p);
    printf("\n");
}

/* Jogar peça: remove da frente da fila (dequeue), gera nova peça e enfileira.
   Peça removida não volta ao jogo. */
void jogarPeca(Fila *f, Pilha *p, int *idCounter) {
    (void)p; /* pilha não usada aqui, mas mantida por consistência */
    if (filaVazia(f)) {
        printf("⚠️ Fila vazia. Não há peça para jogar.\n");
        return;
    }
    Peca jogada = desenfileirar(f);
    printf("Ação: Jogar peça [%c %d]\n", jogada.nome, jogada.id);
    /* gerar nova peça e enfileirar para manter a fila cheia quando possível */
    Peca nova = gerarPeca((*idCounter)++);
    enfileirar(f, nova);
}

/* Reservar peça: move da frente da fila para o topo da pilha (se houver espaço)
   e gera nova peça para repor a fila. */
void reservarPeca(Fila *f, Pilha *p, int *idCounter) {
    if (filaVazia(f)) {
        printf("⚠️ Fila vazia. Não há peça para reservar.\n");
        return;
    }
    if (pilhaCheia(p)) {
        printf("⚠️ Pilha cheia. Não é possível reservar.\n");
        return;
    }
    Peca frente = desenfileirar(f);
    empilhar(p, frente);
    printf("Ação: Reservar peça [%c %d] na pilha.\n", frente.nome, frente.id);
    Peca nova = gerarPeca((*idCounter)++);
    enfileirar(f, nova);
}

/* Usar peça reservada: desempilha e descarta (simula uso) */
void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("⚠️ Pilha vazia. Não há peça reservada para usar.\n");
        return;
    }
    Peca usada = desempilhar(p);
    printf("Ação: Usar peça reservada [%c %d]\n", usada.nome, usada.id);
}

/* Trocar atual: troca a peça da frente da fila com o topo da pilha */
void trocarAtual(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("⚠️ Fila vazia. Não há peça na frente para trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("⚠️ Pilha vazia. Não há peça no topo para trocar.\n");
        return;
    }
    int idxFront = f->inicio;
    Peca temp = f->itens[idxFront];
    f->itens[idxFront] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("Ação: Troca realizada entre frente da fila e topo da pilha.\n");
}

/* Troca múltipla: alterna as 3 primeiras da fila com as 3 da pilha
   Requer f->quantidade >= 3 e p->topo >= 2 (pelo menos 3 elementos na pilha) */
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->quantidade < 3) {
        printf("⚠️ Fila não possui 3 elementos para troca múltipla.\n");
        return;
    }
    if (p->topo < 2) {
        printf("⚠️ Pilha não possui 3 elementos para troca múltipla.\n");
        return;
    }
    int idx0 = f->inicio;
    int idx1 = (f->inicio + 1) % TAM_FILA;
    int idx2 = (f->inicio + 2) % TAM_FILA;
    int pti0 = p->topo;   /* topo */
    int pti1 = p->topo - 1;
    int pti2 = p->topo - 2;

    /* Salva os 3 primeiros da fila */
    Peca f0 = f->itens[idx0];
    Peca f1 = f->itens[idx1];
    Peca f2 = f->itens[idx2];

    /* Salva os 3 da pilha */
    Peca p0 = p->itens[pti0];
    Peca p1 = p->itens[pti1];
    Peca p2 = p->itens[pti2];

    /* Coloca os da pilha no início da fila */
    f->itens[idx0] = p0;
    f->itens[idx1] = p1;
    f->itens[idx2] = p2;

    /* Coloca os antigos da fila na pilha (mantendo ordem: antigo f0 vai para topo, etc.) */
    p->itens[pti0] = f0;
    p->itens[pti1] = f1;
    p->itens[pti2] = f2;

    printf("Ação: Troca múltipla realizada entre 3 primeiros da fila e 3 da pilha.\n");
}

/* ----- Programa principal ----- */
int main(void) {
    srand((unsigned)time(NULL));
    Fila fila;
    Pilha pilha;
    int idCounter = 0;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Preenche a fila inicial com TAM_FILA peças */
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idCounter++));
    }

    /* Opcional: pré-popular a pilha (comentado para começar vazia)
    empilhar(&pilha, gerarPeca(idCounter++));
    empilhar(&pilha, gerarPeca(idCounter++));
    */

    do {
        mostrarEstado(&fila, &pilha);

        printf("Opções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\\n'); /* limpar entrada inválida */
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        getchar(); /* consumir '\n' */

        switch (opcao) {
            case 1:
                jogarPeca(&fila, &pilha, &idCounter);
                break;
            case 2:
                reservarPeca(&fila, &pilha, &idCounter);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o gerenciador. Até mais!\n");
                break;
            default:
                printf("Opção desconhecida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
