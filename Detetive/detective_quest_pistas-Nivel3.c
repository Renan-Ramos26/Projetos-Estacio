#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10

// Struct da sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Struct do nó da BST para pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura de tabela hash simples
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* proximo;
} HashNode;

// Função para criar sala
Sala* criarSala(char nome[], char pista[], Sala* esquerda, Sala* direita) {
    Sala* s = (Sala*)malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista);
    s->esquerda = esquerda;
    s->direita = direita;
    return s;
}

// Função para inserir pista na BST
PistaNode* inserirPista(PistaNode* raiz, char pista[]) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// Função para exibir BST em ordem
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Função hash simples
int hashFunction(char* pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % HASH_SIZE;
}

// Inserir na hash table
void inserirNaHash(HashNode* tabela[], char pista[], char suspeito[]) {
    int idx = hashFunction(pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[idx];
    tabela[idx] = novo;
}

// Buscar suspeito na hash table
char* encontrarSuspeito(HashNode* tabela[], char pista[]) {
    int idx = hashFunction(pista);
    HashNode* atual = tabela[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

// Explorar salas e coletar pistas
void explorarSalas(Sala* atual, PistaNode** bstPistas, HashNode* tabela[]) {
    if (atual == NULL)
        return;

    printf("\nVocê está na sala: %s\n", atual->nome);
    if (strlen(atual->pista) > 0) {
        printf("Você encontrou uma pista: %s\n", atual->pista);
        *bstPistas = inserirPista(*bstPistas, atual->pista);
    }

    char escolha;
    printf("Escolha: esquerda (e), direita (d) ou sair (s): ");
    scanf(" %c", &escolha);

    if (escolha == 'e')
        explorarSalas(atual->esquerda, bstPistas, tabela);
    else if (escolha == 'd')
        explorarSalas(atual->direita, bstPistas, tabela);
    else
        return;
}

// Contar quantas pistas apontam para um suspeito
int contarPistas(PistaNode* raiz, HashNode* tabela[], char suspeito[]) {
    if (raiz == NULL)
        return 0;
    int total = 0;
    char* s = encontrarSuspeito(tabela, raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0)
        total++;
    total += contarPistas(raiz->esquerda, tabela, suspeito);
    total += contarPistas(raiz->direita, tabela, suspeito);
    return total;
}

// Liberar memória BST
void liberarBST(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->esquerda);
        liberarBST(raiz->direita);
        free(raiz);
    }
}

// Liberar memória árvore de salas
void liberarSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// Liberar memória hash
void liberarHash(HashNode* tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = tabela[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

int main() {
    // Criar hash table
    HashNode* tabela[HASH_SIZE] = {NULL};

    // Associar pistas a suspeitos
    inserirNaHash(tabela, "pegada molhada", "Sr. Verde");
    inserirNaHash(tabela, "carta rasgada", "Sra. Azul");
    inserirNaHash(tabela, "chave perdida", "Sr. Verde");
    inserirNaHash(tabela, "poeira de ouro", "Srta. Rosa");
    inserirNaHash(tabela, "pegada molhada", "Sr. Verde"); // exemplo duplicado

    // Criar mapa da mansão
    Sala* sala3 = criarSala("Biblioteca", "chave perdida", NULL, NULL);
    Sala* sala4 = criarSala("Cozinha", "poeira de ouro", NULL, NULL);
    Sala* sala2 = criarSala("Sala de Estar", "carta rasgada", sala3, sala4);
    Sala* sala5 = criarSala("Jardim", "pegada molhada", NULL, NULL);
    Sala* hall = criarSala("Hall de Entrada", "", sala2, sala5);

    // BST para pistas coletadas
    PistaNode* bstPistas = NULL;

    printf("Bem-vindo(a) ao Detective Quest!\nExplore a mansão e colete pistas.\n");
    explorarSalas(hall, &bstPistas, tabela);

    printf("\n--- Pistas coletadas (em ordem alfabética) ---\n");
    exibirPistas(bstPistas);

    char suspeito[50];
    printf("\nIndique o suspeito que você acusa: ");
    scanf(" %[^\n]", suspeito);  // <-- Corrigido

    int pistasEncontradas = contarPistas(bstPistas, tabela, suspeito);
    if (pistasEncontradas >= 2)
        printf("Acusação correta! %d pistas indicam %s.\n", pistasEncontradas, suspeito);
    else
        printf("Acusação insuficiente. Apenas %d pista(s) indicam %s.\n", pistasEncontradas, suspeito);

    // Liberar memória
    liberarBST(bstPistas);
    liberarSalas(hall);
    liberarHash(tabela);

    return 0;
}
