/*
 detective_quest_pistas.c
 Expansão do Detective Quest:
 - Mapa da mansão representado por árvore binária (salas)
 - Cada sala pode conter uma pista (string)
 - Pistas coletadas são armazenadas em UMA ÁRVORE BINÁRIA DE BUSCA (BST)
 - Exploração interativa (e: esquerda, d: direita, s: sair)
 - Ao sair, exibe todas as pistas coletadas em ordem alfabética
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------- Estruturas ---------------------- */

/* Nó da árvore de salas (mapa da mansão) */
typedef struct Sala {
    char nome[64];
    char pista[128];       /* string vazia ("") se não houver pista */
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* Nó da BST de pistas */
typedef struct PistaNode {
    char pista[128];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* ---------------------- Prototipos ---------------------- */

/* Salas (mapa) */
Sala* criarSala(const char *nome, const char *pista);
void liberarMapa(Sala *raiz);

/* BST de pistas */
PistaNode* novoNoPista(const char *pista);
int inserirPista(PistaNode **raiz, const char *pista); /* retorna 1 se inseriu, 0 se já existia */
void exibirPistasEmOrdem(PistaNode *raiz);
void liberarPistas(PistaNode *raiz);

/* Exploração */
void explorarSalasComPistas(Sala *inicio, PistaNode **arvorePistas);

/* Utilitários */
void limparEntrada(void);

/* ---------------------- Implementação ---------------------- */

/* Aloca e inicializa uma sala com nome e pista (pista pode ser NULL ou "") */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (s == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria para sala.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(s->nome, nome, sizeof(s->nome)-1);
    s->nome[sizeof(s->nome)-1] = '\0';
    if (pista != NULL) {
        strncpy(s->pista, pista, sizeof(s->pista)-1);
        s->pista[sizeof(s->pista)-1] = '\0';
    } else {
        s->pista[0] = '\0';
    }
    s->esquerda = s->direita = NULL;
    return s;
}

/* Libera todas as salas (pós-ordem) */
void liberarMapa(Sala *raiz) {
    if (raiz == NULL) return;
    liberarMapa(raiz->esquerda);
    liberarMapa(raiz->direita);
    free(raiz);
}

/* Cria nó para BST de pistas */
PistaNode* novoNoPista(const char *pista) {
    PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
    if (n == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria para pista.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->pista, pista, sizeof(n->pista)-1);
    n->pista[sizeof(n->pista)-1] = '\0';
    n->esq = n->dir = NULL;
    return n;
}

/* Insere pista na BST se ainda não existir.
   Retorna 1 se inseriu, 0 se já existia (evita duplicatas). */
int inserirPista(PistaNode **raiz, const char *pista) {
    if (pista == NULL || pista[0] == '\0') return 0; /* nada a inserir */
    if (*raiz == NULL) {
        *raiz = novoNoPista(pista);
        return 1;
    }
    int cmp = strcmp(pista, (*raiz)->pista);
    if (cmp == 0) {
        return 0; /* já existe */
    } else if (cmp < 0) {
        return inserirPista(&((*raiz)->esq), pista);
    } else {
        return inserirPista(&((*raiz)->dir), pista);
    }
}

/* Exibe pistas em ordem alfabética (in-order traversal) */
void exibirPistasEmOrdem(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistasEmOrdem(raiz->esq);
    printf("- %s\n", raiz->pista);
    exibirPistasEmOrdem(raiz->dir);
}

/* Libera memória da BST de pistas (pós-ordem) */
void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esq);
    liberarPistas(raiz->dir);
    free(raiz);
}

/* Limpa qualquer resto de entrada no stdin até newline */
void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Exploração interativa: começa em 'inicio' e coleta pistas em arvorePistas */
void explorarSalasComPistas(Sala *inicio, PistaNode **arvorePistas) {
    Sala *atual = inicio;
    char escolha;
    printf("\nIniciando exploracao. Digite 'e' (esquerda), 'd' (direita) ou 's' (sair).\n");
    while (atual != NULL) {
        printf("\nVoce esta na: %s\n", atual->nome);
        if (atual->pista[0] != '\0') {
            printf("Pista encontrada: %s\n", atual->pista);
            int inseriu = inserirPista(arvorePistas, atual->pista);
            if (inseriu) {
                printf("(Pista registrada no caderno do detetive.)\n");
            } else {
                printf("(Pista ja registrada anteriormente.)\n");
            }
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        /* Se for folha, permitir apenas 's' para sair ou permitir retroceder?
           Requisito pede que o jogador escolha e/d/s a qualquer momento; aqui mostramos opções
        */
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Nao ha caminhos a seguir nesta sala.\n");
            printf("Digite 's' para sair, ou 'e'/'d' para tentar (opcoes inexistentes sera' informada): ");
        } else {
            printf("Escolha um caminho:\n");
            if (atual->esquerda != NULL) printf("  [e] Ir para a esquerda (%s)\n", atual->esquerda->nome);
            if (atual->direita != NULL)  printf("  [d] Ir para a direita (%s)\n", atual->direita->nome);
            printf("  [s] Sair da exploracao\n> ");
        }

        escolha = getchar();
        limparEntrada();

        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Caminho a esquerda nao existe. Tente outra opcao.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Caminho a direita nao existe. Tente outra opcao.\n");
            }
        } else if (escolha == 's') {
            printf("Voce decidiu encerrar a exploracao.\n");
            break;
        } else {
            printf("Opcao invalida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

/* ---------------------- Main: monta o mapa e roda o jogo ---------------------- */
int main(void) {
    /* Montagem estática do mapa da mansão com pistas */
    Sala *hall = criarSala("Hall de Entrada", "Chave enferrujada perto do tapete.");
    Sala *salaEstar = criarSala("Sala de Estar", "Retrato antigo com olhos riscados.");
    Sala *cozinha = criarSala("Cozinha", "Faca com resquicios vermelhos encontrada na pia.");
    Sala *biblioteca = criarSala("Biblioteca", "Livro aberto na pagina 217, marcador de papel rasgado.");
    Sala *jardim = criarSala("Jardim", "Pegadas molhadas que levam ao portao.");
    Sala *escritorio = criarSala("Escritorio", "Bilhete: 'Nos veremos a meia-noite'.");
    Sala *porao = criarSala("Porão", "Marcas de arrasto no chao.");

    /* Conecta as salas (estrutura da árvore) */
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = escritorio;
    cozinha->direita = porao;

    /* BST de pistas vazia inicialmente */
    PistaNode *arvorePistas = NULL;

    /* Introdução */
    printf("=====================================\n");
    printf("      DETECTIVE QUEST - MANSÃO\n");
    printf("=====================================\n");
    printf("Colete pistas enquanto explora a mansão.\n");
    printf("As pistas serão registradas e exibidas em ordem alfabética ao final.\n");

    /* Explorar e coletar pistas */
    explorarSalasComPistas(hall, &arvorePistas);

    /* Exibir pistas coletadas em ordem alfabética */
    printf("\n===== Pistas coletadas (em ordem alfabetica) =====\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistasEmOrdem(arvorePistas);
    }
    printf("=================================================\n");

    /* Limpar memória */
    liberarMapa(hall);
    liberarPistas(arvorePistas);

    printf("\nObrigado por jogar Detective Quest - Pistas!\n");
    return 0;
}
