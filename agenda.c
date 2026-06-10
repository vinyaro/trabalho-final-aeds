#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 50

/* =========================================================
   1. DEFINIÇÃO DAS ESTRUTURAS (STRUCTS)
   ========================================================= */

// LISTA ENCADEADA para os números de telefone
typedef struct telefone {
    char numero[20];
    struct telefone *prox;
} Telefone;

// ÁRVORE BINÁRIA DE BUSCA para os contatos
typedef struct contato {
    char nome[100];
    Telefone *lista_telefones;
    struct contato *esq, *dir;
} Contato;

// TABELA HASH para busca reversa (Número -> Contato)
typedef struct no_hash {
    char numero[20];
    Contato *contato_alvo;
    struct no_hash *prox;
} NoHash;

// Tabela Hash Global
NoHash *tabela_hash[TAM_HASH];

/* =========================================================
   2. FUNÇÕES DE APOIO E HASH
   ========================================================= */

int funcao_hash(char *numero) {
    int soma = 0;
    for (int i = 0; numero[i] != '\0'; i++) soma += numero[i];
    return soma % TAM_HASH;
}

void inserir_na_hash(char *numero, Contato *c) {
    int indice = funcao_hash(numero);
    NoHash *novo = (NoHash*) malloc(sizeof(NoHash));
    strcpy(novo->numero, numero);
    novo->contato_alvo = c;
    novo->prox = tabela_hash[indice];
    tabela_hash[indice] = novo;
}

void remover_da_hash(char *numero) {
    int indice = funcao_hash(numero);
    NoHash *atual = tabela_hash[indice];
    NoHash *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->numero, numero) == 0) {
            if (anterior == NULL) tabela_hash[indice] = atual->prox;
            else anterior->prox = atual->prox;
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

// Atualiza a Hash se o endereço do contato na árvore mudar (essencial na remoção)
void atualizar_ponteiros_hash(Contato *contato) {
    Telefone *t = contato->lista_telefones;
    while (t != NULL) {
        int indice = funcao_hash(t->numero);
        NoHash *atual = tabela_hash[indice];
        while (atual != NULL) {
            if (strcmp(atual->numero, t->numero) == 0) {
                atual->contato_alvo = contato;
            }
            atual = atual->prox;
        }
        t = t->prox;
    }
}

/* =========================================================
   3. OPERAÇÕES DA ÁRVORE (CADASTRO E BUSCA)
   ========================================================= */

Contato* criar_contato(char *nome) {
    Contato *novo = (Contato*) malloc(sizeof(Contato));
    strcpy(novo->nome, nome);
    novo->lista_telefones = NULL;
    novo->esq = novo->dir = NULL;
    return novo;
}

void adicionar_telefone(Contato *c, char *numero) {
    Telefone *novo = (Telefone*) malloc(sizeof(Telefone));
    strcpy(novo->numero, numero);
    novo->prox = c->lista_telefones;
    c->lista_telefones = novo;
    inserir_na_hash(numero, c);
}

Contato* inserir_na_arvore(Contato *raiz, char *nome, char *primeiro_tel) {
    if (raiz == NULL) {
        Contato *novo = criar_contato(nome);
        adicionar_telefone(novo, primeiro_tel);
        return novo;
    }
    if (strcmp(nome, raiz->nome) < 0) raiz->esq = inserir_na_arvore(raiz->esq, nome, primeiro_tel);
    else if (strcmp(nome, raiz->nome) > 0) raiz->dir = inserir_na_arvore(raiz->dir, nome, primeiro_tel);
    return raiz;
}

Contato* encontrar_minimo(Contato* raiz) {
    Contato* atual = raiz;
    while (atual && atual->esq != NULL) atual = atual->esq;
    return atual;
}

Contato* remover_contato(Contato *raiz, char *nome) {
    if (raiz == NULL) return NULL;

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = remover_contato(raiz->esq, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = remover_contato(raiz->dir, nome);
    else {
        // Encontrou!
        if (raiz->esq == NULL || raiz->dir == NULL) {
            Contato *temp = raiz->esq ? raiz->esq : raiz->dir;
            Telefone *t = raiz->lista_telefones;
            while(t) {
                remover_da_hash(t->numero);
                Telefone *aux = t;
                t = t->prox;
                free(aux);
            }
            free(raiz);
            return temp;
        }

        Contato *temp = encontrar_minimo(raiz->dir);
        
        // Limpa telefones atuais do nó que vai ser substituído
        Telefone *t_limpa = raiz->lista_telefones;
        while(t_limpa) { remover_da_hash(t_limpa->numero); Telefone *aux = t_limpa; t_limpa = t_limpa->prox; free(aux); }

        strcpy(raiz->nome, temp->nome);
        raiz->lista_telefones = temp->lista_telefones;
        atualizar_ponteiros_hash(raiz);

        temp->lista_telefones = NULL; // Impede que a recursão apague a lista movida
        raiz->dir = remover_contato(raiz->dir, temp->nome);
    }
    return raiz;
}

void busca_por_nome(Contato *raiz, char *nome) {
    if (raiz == NULL) { printf("Contato nao encontrado.\n"); return; }
    if (strcmp(nome, raiz->nome) == 0) {
        printf("Contato: %s | Telefones: ", raiz->nome);
        Telefone *t = raiz->lista_telefones;
        while(t) { printf("%s ", t->numero); t = t->prox; }
        printf("\n");
    } else if (strcmp(nome, raiz->nome) < 0) busca_por_nome(raiz->esq, nome);
    else busca_por_nome(raiz->dir, nome);
}

void busca_reversa(char *numero) {
    int indice = funcao_hash(numero);
    NoHash *atual = tabela_hash[indice];
    while (atual != NULL) {
        if (strcmp(atual->numero, numero) == 0) {
            printf("O numero %s pertence a: %s\n", numero, atual->contato_alvo->nome);
            return;
        }
        atual = atual->prox;
    }
    printf("Numero nao encontrado na base.\n");
}

void listar_alfabetico(Contato *raiz) {
    if (raiz != NULL) {
        listar_alfabetico(raiz->esq);
        printf("- %s\n", raiz->nome);
        listar_alfabetico(raiz->dir);
    }
}

/* =========================================================
   4. LIMPEZA TOTAL DE MEMÓRIA (FREE ALL)
   ========================================================= */

void limpar_arvore(Contato *raiz) {
    if (raiz == NULL) return;
    limpar_arvore(raiz->esq);
    limpar_arvore(raiz->dir);
    Telefone *t = raiz->lista_telefones;
    while(t) { Telefone *aux = t; t = t->prox; free(aux); }
    free(raiz);
}

void limpar_hash() {
    for(int i=0; i<TAM_HASH; i++) {
        NoHash *atual = tabela_hash[i];
        while(atual) { NoHash *aux = atual; atual = atual->prox; free(aux); }
    }
}

/* =========================================================
   5. FUNÇÃO MAIN: MENU INTERATIVO
   ========================================================= */

int main() {
    Contato *agenda = NULL;
    for(int i=0; i<TAM_HASH; i++) tabela_hash[i] = NULL;
    int opcao;
    char nome[100], tel[20];

    do {
        printf("\n--- AGENDA AEDS ---\n1. Adicionar Contato\n2. Remover Contato\n3. Buscar por Nome\n4. Busca Reversa (Tel)\n5. Listar Todos\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch(opcao) {
            case 1:
                printf("Nome: "); fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
                printf("Telefone: "); fgets(tel, 20, stdin); tel[strcspn(tel, "\n")] = 0;
                agenda = inserir_na_arvore(agenda, nome, tel);
                break;
            case 2:
                printf("Nome para remover: "); fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
                agenda = remover_contato(agenda, nome);
                break;
            case 3:
                printf("Nome para buscar: "); fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
                busca_por_nome(agenda, nome);
                break;
            case 4:
                printf("Telefone para busca reversa: "); fgets(tel, 20, stdin); tel[strcspn(tel, "\n")] = 0;
                busca_reversa(tel);
                break;
            case 5:
                printf("\nContatos em ordem alfabetica:\n");
                listar_alfabetico(agenda);
                break;
        }
    } while (opcao != 0);

    // Limpeza final antes de encerrar
    limpar_arvore(agenda);
    limpar_hash();
    printf("Memoria liberada. Ate logo!\n");

    return 0;
}