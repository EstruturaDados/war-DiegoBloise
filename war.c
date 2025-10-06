// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS (NÍVEL AVENTUREIRO):
// - Substituir o vetor estático por alocação dinâmica com calloc.
// - Criar uma função para simular ataques entre dois territórios.
// - Usar números aleatórios para representar dados de batalha.
// - Utilizar ponteiros e modularização.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// --- Constantes Globais ---
#define TAM_NOME 30
#define TAM_COR 10

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
Territorio* alocarMapa(int tamanho);
void liberarMemoria(Territorio* mapa);
void cadastrarTerritorios(Territorio* mapa, int tamanho);
void exibirTerritorios(const Territorio* mapa, int tamanho);
int faseDeAtaque(Territorio* mapa, int tamanho);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int verificarVitoria(Territorio* mapa, int tamanho, Territorio* atacante);

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // inicializa gerador de números aleatórios

    printf("==============================================\n");
    printf("     PREPARAÇÃO DO GAME - WAR ESTRUTURADO     \n");
    printf("==============================================\n\n");

    int totalTerritorios;

    do {
        printf("Número de territórios: ");
        scanf("%d", &totalTerritorios);
        getchar();

        if (totalTerritorios < 2) {
           printf("Deve ser informado pelo menos 2 territórios para iniciar o game!\n");
        }

    } while (totalTerritorios < 2);

    // 1. Alocação dinâmica do mapa de territórios
    Territorio* mapa = alocarMapa(totalTerritorios);
    if (mapa == NULL) {
        printf("Erro: falha ao alocar memória para os territórios.\n");
        return 1;
    }

    // 2. Cadastro dos territórios
    cadastrarTerritorios(mapa, totalTerritorios);

    // 3. Loop de batalhas
    char continuar;
    int gameOver = 0;
    do {
        printf("\n==============================================\n");
        printf("                ESTADO DO MAPA                \n");
        printf("==============================================\n");
        exibirTerritorios(mapa, totalTerritorios);

        printf("\nDeseja realizar um ataque? (S/n): ");
        scanf(" %c", &continuar);
        getchar();

        if (continuar == 's' || continuar == 'S') {
            gameOver = faseDeAtaque(mapa, totalTerritorios);
        }

    } while (gameOver == 0);

    // 4. Liberação da memória
    liberarMemoria(mapa);

    printf("\nJogo encerrado. Até a próxima batalha!\n");
    return 0;
}

// --- Implementação das Funções ---

// Aloca dinamicamente o vetor de territórios com calloc
Territorio* alocarMapa(int tamanho) {
    Territorio* mapa = (Territorio*) calloc(tamanho, sizeof(Territorio));
    return mapa;
}

// Libera a memória alocada
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// Cadastra os territórios dinamicamente
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);

        printf("Nome do território: ");
        fgets(mapa[i].nome, TAM_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, TAM_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();

        printf("Território '%s' (%s) com %d tropas cadastrado!\n",
               mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Exibe os territórios atuais
void exibirTerritorios(const Territorio* mapa, int tamanho) {
    printf("\n%-3s | %-25s | %-10s | %-6s\n", "ID", "Nome", "Cor", "Tropas");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < tamanho; i++) {
        printf("%-3d | %-25s | %-10s | %-6d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Controla a escolha de ataque e defesa
int faseDeAtaque(Territorio* mapa, int tamanho) {
    int idAtacante, idDefensor;

    printf("\nEscolha o território ATACANTE (1-%d): ", tamanho);
    scanf("%d", &idAtacante);
    getchar();

    printf("Escolha o território DEFENSOR (1-%d): ", tamanho);
    scanf("%d", &idDefensor);
    getchar();

    // validação básica
    if (idAtacante < 1 || idAtacante > tamanho || idDefensor < 1 || idDefensor > tamanho) {
        printf("IDs inválidos! Escolha entre 1 e %d.\n", tamanho);
        return 0;
    }

    if (idAtacante == idDefensor) {
        printf("Um território não pode atacar a si mesmo!\n");
        return 0;
    }

    simularAtaque(&mapa[idAtacante - 1], &mapa[idDefensor - 1]);

    return verificarVitoria(mapa, tamanho, &mapa[idAtacante - 1]);
}

// Simula uma batalha entre dois territórios
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    printf("\n==============================================\n");
    printf("             INÍCIO DA BATALHA             \n");
    printf("==============================================\n");
    printf("Atacante: %s (%s) - Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) - Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);

    if (atacante->tropas <= 1) {
        printf("\nO território atacante não tem tropas suficientes para atacar!\n");
        return;
    }
    if (defensor->tropas <= 0) {
        printf("\nO território defensor já foi conquistado!\n");
        return;
    }

    // Rolagem de dados (1 a 6)
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\nDado do Ataque: %d\n", dadoAtaque);
    printf("Dado da Defesa: %d\n", dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\nO atacante venceu a rodada!\n");
        defensor->tropas -= 1;
        if (defensor->tropas <= 0) {
            printf("O território %s foi conquistado pelo exército %s!\n",
                   defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1;
            atacante->tropas -= 1;
        }
    } else {
        printf("\nO defensor resistiu ao ataque!\n");
        atacante->tropas -= 1;
    }

    printf("\n--- Resultado após a batalha ---\n");
    printf("%s (%s) - Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("%s (%s) - Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);
}

// Verifica se o atacante dominou todos os territórios
int verificarVitoria(Territorio* mapa, int tamanho, Territorio* atacante) {
    int territoriosDominados = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, atacante->cor) == 0) {
            territoriosDominados++;
        }
    }

    if (territoriosDominados == tamanho) {
        printf("\n==============================================\n");
        printf("          FIM DE JOGO - VITÓRIA TOTAL!         \n");
        printf("==============================================\n");
        printf("O exército %s conquistou todos os territórios!\n", atacante->cor);
        return 1; // game over
    }

    return 0; // jogo continua
}
