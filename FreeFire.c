#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define NOME_TAM 30
#define TIPO_TAM 20

// =======================================================
// Struct Componente
// =======================================================
typedef struct {
    char nome[NOME_TAM];
    char tipo[TIPO_TAM];
    int prioridade; // 1..10
} Componente;

// =======================================================
// Assinaturas das funções
// =======================================================
void cadastrarComponentes(Componente arr[], int *n);
void mostrarComponentes(Componente arr[], int n);
void limparString(char *s);
double medirTempo(void (*algoritmo)(Componente[], int, long *), Componente arr[], int n, long *comparacoes);

void bubbleSortNome(Componente arr[], int n, long *comparacoes);
void insertionSortTipo(Componente arr[], int n, long *comparacoes);
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes);

int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes);

// =======================================================
// Função main: menu interativo
// =======================================================
int main() {
    Componente componentes[MAX_COMPONENTES];
    int n = 0;
    int opcao;
    int ordenadoPorNome = 0; // flag para permitir busca binária
    char chave[NOME_TAM];

    printf("=== Sistema de Priorizacao: Torre de Resgate ===\n\n");

    cadastrarComponentes(componentes, &n);

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1 - Mostrar componentes\n");
        printf("2 - Bubble Sort por NOME (ordenacao estável, O(n^2))\n");
        printf("3 - Insertion Sort por TIPO (estável, O(n^2))\n");
        printf("4 - Selection Sort por PRIORIDADE (O(n^2))\n");
        printf("5 - Busca Binaria por NOME (requer ordenacao por nome)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) { // proteção básica
            printf("Entrada invalida.\n");
            // limpar stdin
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
        }
        getchar(); // consome '\n' remanescente

        if (opcao == 1) {
            mostrarComponentes(componentes, n);
        }
        else if (opcao == 2) {
            long comp = 0;
            double segundos = medirTempo(bubbleSortNome, componentes, n, &comp);
            printf("\n-- Resultado: Bubble Sort por NOME --\n");
            printf("Comparacoes: %ld\n", comp);
            printf("Tempo decorrido: %.6f segundos\n", segundos);
            mostrarComponentes(componentes, n);
            ordenadoPorNome = 1;
        }
        else if (opcao == 3) {
            long comp = 0;
            double segundos = medirTempo(insertionSortTipo, componentes, n, &comp);
            printf("\n-- Resultado: Insertion Sort por TIPO --\n");
            printf("Comparacoes: %ld\n", comp);
            printf("Tempo decorrido: %.6f segundos\n", segundos);
            mostrarComponentes(componentes, n);
            ordenadoPorNome = 0; // ordenacao por tipo quebra ordenacao por nome
        }
        else if (opcao == 4) {
            long comp = 0;
            double segundos = medirTempo(selectionSortPrioridade, componentes, n, &comp);
            printf("\n-- Resultado: Selection Sort por PRIORIDADE --\n");
            printf("Comparacoes: %ld\n", comp);
            printf("Tempo decorrido: %.6f segundos\n", segundos);
            mostrarComponentes(componentes, n);
            ordenadoPorNome = 0; // ordenacao por prioridade quebra ordenacao por nome
        }
        else if (opcao == 5) {
            if (!ordenadoPorNome) {
                printf("ERRO: Para usar busca binaria por nome, ordene primeiro por NOME (opcao 2).\n");
            } else {
                printf("Digite o nome do componente-chave para buscar: ");
                if (fgets(chave, NOME_TAM, stdin) != NULL) {
                    chave[strcspn(chave, "\n")] = '\0';
                    long comp = 0;
                    int pos = buscaBinariaPorNome(componentes, n, chave, &comp);
                    if (pos == -1) {
                        printf("Componente '%s' NAO encontrado.\n", chave);
                    } else {
                        printf("Componente encontrado na posicao %d:\n", pos + 1);
                        printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                               componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
                    }
                    printf("Comparacoes (busca binaria): %ld\n", comp);
                }
            }
        }
        else if (opcao == 0) {
            printf("Encerrando sistema. Boa sorte na fuga!\n");
        }
        else {
            printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// =======================================================
// Função: cadastrarComponentes
// Entrada interativa de até MAX_COMPONENTES componentes
// =======================================================
void cadastrarComponentes(Componente arr[], int *n) {
    char buffer[16];
    int qtd = 0;

    printf("Quantos componentes deseja cadastrar? (1-%d): ", MAX_COMPONENTES);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    qtd = atoi(buffer);
    if (qtd < 1) qtd = 1;
    if (qtd > MAX_COMPONENTES) qtd = MAX_COMPONENTES;

    for (int i = 0; i < qtd; i++) {
        printf("\n--- Cadastro %d de %d ---\n", i+1, qtd);

        printf("Nome do componente: ");
        if (fgets(arr[i].nome, NOME_TAM, stdin) == NULL) {
            arr[i].nome[0] = '\0';
        } else {
            arr[i].nome[strcspn(arr[i].nome, "\n")] = '\0';
        }

        printf("Tipo (controle/suporte/propulsao/...): ");
        if (fgets(arr[i].tipo, TIPO_TAM, stdin) == NULL) {
            arr[i].tipo[0] = '\0';
        } else {
            arr[i].tipo[strcspn(arr[i].tipo, "\n")] = '\0';
        }

        // prioridade com validação
        int p = 0;
        do {
            char linha[16];
            printf("Prioridade (1-10): ");
            if (fgets(linha, sizeof(linha), stdin) == NULL) { p = 1; break; }
            p = atoi(linha);
            if (p < 1 || p > 10) {
                printf("Valor invalido. Informe entre 1 e 10.\n");
            }
        } while (p < 1 || p > 10);
        arr[i].prioridade = p;
    }

    *n = qtd;
    printf("\nCadastro concluido: %d componentes registrados.\n", *n);
}

// =======================================================
// Função: mostrarComponentes
// Exibe vetor formatado com índice, nome, tipo e prioridade
// =======================================================
void mostrarComponentes(Componente arr[], int n) {
    if (n == 0) {
        printf("\nNenhum componente registrado.\n");
        return;
    }

    printf("\n--- Componentes (%d) ---\n", n);
    printf("%-4s | %-28s | %-12s | %-10s\n", "ID", "NOME", "TIPO", "PRIORIDADE");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-4d | %-28s | %-12s | %-10d\n",
               i + 1,
               arr[i].nome,
               arr[i].tipo,
               arr[i].prioridade);
    }
}

// =======================================================
// Função utilitária: medirTempo
// Recebe um ponteiro para algoritmo do tipo:
//   void alg(Componente[], int, long *comparacoes)
// Executa o algoritmo, preenche comparacoes e retorna tempo em segundos.
// =======================================================
double medirTempo(void (*algoritmo)(Componente[], int, long *), Componente arr[], int n, long *comparacoes) {
    clock_t inicio = clock();
    *comparacoes = 0;
    algoritmo(arr, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / (double)CLOCKS_PER_SEC;
}

// =======================================================
// Algoritmos de ordenacao e contagem de comparacoes
// Cada função atualiza *comparacoes com o numero de comparacoes feitas
// =======================================================

// Bubble sort por nome (lexicográfico)
// Está implementado de forma simples (O(n^2)), geralmente estável.
void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;

    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; // já ordenado
    }
}

// Insertion sort por tipo (lexicográfico)
// Bom para conjuntos quase ordenados; algoritmo estável.
void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = arr[i];
        int j = i - 1;
        // comparar tipos
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = chave;
    }
}

// Selection sort por prioridade (menor prioridade primeiro)
// Não é estável por definição simples, mas é intuitivo para selecionar mínimos.
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Componente tmp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = tmp;
        }
    }
}

// =======================================================
// Busca binária por nome (REQUIRE: arr está ordenado por nome)
// Retorna índice (0..n-1) ou -1 se não encontrado. Conta comparacoes.
// =======================================================
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes) {
    int esq = 0, dir = n - 1;
    *comparacoes = 0;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        (*comparacoes)++;
        int cmp = strcmp(arr[meio].nome, chave);
        if (cmp == 0) return meia = meio, meio; // found
        else if (cmp < 0) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}
