// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (NÍVEL MESTRE)
// ============================================================================
//
// OBJETIVOS (VERSÃO FINAL):
// - Alocação dinâmica do mapa e modularização completa.
// - Simulação de batalhas com rolagem de dados aleatórios.
// - Sistema de 5 missões diferentes sorteadas no início do jogo.
// - Foco em: design, organização, clareza e lógica de jogo.
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// --- Constantes Globais ---
#define TAM_NOME 30
#define TAM_COR 10
#define NUM_MISSOES 5

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
void gameSetup(int* totalTerritorios);
Territorio* alocarMapa(int tamanho);
void cadastrarTerritorios(Territorio* mapa, int tamanho);
void gameLoop(Territorio* mapa, int tamanho, const char* corJogador, int idMissao);
void exibirTerritorios(const Territorio* mapa, int tamanho);
void exibirMissao(int idMissao);
int faseDeAtaque(Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
int verificarVitoriaMissao(const Territorio* mapa, int tamanho, int idMissao, const char* corJogador);
void liberarMemoria(Territorio* mapa);
void limparBufferEntrada();
int sortearMissao();

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    printf("==============================================\n");
    printf("           WAR ESTRUTURADO - NÍVEL MESTRE     \n");
    printf("==============================================\n\n");

    int totalTerritorios;
    gameSetup(&totalTerritorios);

    // Alocação dinâmica
    Territorio* mapa = alocarMapa(totalTerritorios);
    if (mapa == NULL) {
        printf("Erro: falha ao alocar memória!\n");
        return 1;
    }

    // Cadastro dos territórios
    cadastrarTerritorios(mapa, totalTerritorios);

    // Seleção da cor do jogador
    char corJogador[TAM_COR];
    printf("\nDigite a cor do seu exército: ");
    fgets(corJogador, TAM_COR, stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0';

    // Sorteio de missão
    int idMissao = sortearMissao();

    // Loop principal do jogo
    gameLoop(mapa, totalTerritorios, corJogador, idMissao);

    liberarMemoria(mapa);
    printf("\nJogo encerrado. Até a próxima batalha!\n");
    return 0;
}

// ============================================================================
//                      CONFIGURAÇÃO E GERENCIAMENTO
// ============================================================================

void gameSetup(int* totalTerritorios) {
    printf("==============================================\n");
    printf("           PREPARAÇÃO DO GAME - WAR           \n");
    printf("==============================================\n\n");

    do {
        printf("Número de territórios: ");
        scanf("%d", totalTerritorios);
        limparBufferEntrada();

        if (*totalTerritorios < 5) {
            printf("Deve haver pelo menos 5 territórios!\n");
        }
    } while (*totalTerritorios < 5);
}

Territorio* alocarMapa(int tamanho) {
    return (Territorio*) calloc(tamanho, sizeof(Territorio));
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// ============================================================================
//                           CADASTRO E EXIBIÇÃO
// ============================================================================

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
        limparBufferEntrada();

        printf("Território '%s' (%s) com %d tropas cadastrado!\n",
               mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirTerritorios(const Territorio* mapa, int tamanho) {
    printf("\n%-3s | %-25s | %-10s | %-6s\n", "ID", "Nome", "Cor", "Tropas");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-3d | %-25s | %-10s | %-6d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// ============================================================================
//                               MISSÕES
// ============================================================================

int sortearMissao() {
    return (rand() % NUM_MISSOES) + 1;
}

void exibirMissao(int idMissao) {
    printf("\n==============================================\n");
    printf("                  SUA MISSÃO                  \n");
    printf("==============================================\n");

    switch (idMissao) {
        case 1:
            printf("1 - Destruir o exército Verde.\n");
            break;
        case 2:
            printf("2 - Conquistar 3 territórios.\n");
            break;
        case 3:
            printf("3 - Dominar todos os territórios.\n");
            break;
        case 4:
            printf("4 - Ter pelo menos 20 tropas somadas no total.\n");
            break;
        case 5:
            printf("5 - Controlar ao menos 2 territórios Azuis.\n");
            break;
    }
}

// ============================================================================
//                             LOOP DO JOGO
// ============================================================================

void gameLoop(Territorio* mapa, int tamanho, const char* corJogador, int idMissao) {
    int opcao;
    int venceu = 0;

    do {
        printf("\n==============================================\n");
        printf("                 ESTADO DO MAPA               \n");
        printf("==============================================\n");
        exibirTerritorios(mapa, tamanho);
        exibirMissao(idMissao);

        printf("\n1 - Atacar\n");
        printf("2 - Verificar Missão\n");
        printf("0 - Sair\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, tamanho);
                break;
            case 2:
                venceu = verificarVitoriaMissao(mapa, tamanho, idMissao, corJogador);
                if (venceu)
                    printf("\n🎉 Missão cumprida! Você venceu o jogo!\n");
                else
                    printf("\n❌ Missão ainda não concluída.\n");
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0 && !venceu);
}

// ============================================================================
//                          MECÂNICA DE BATALHA
// ============================================================================

int faseDeAtaque(Territorio* mapa, int tamanho) {
    int idAtacante, idDefensor;

    printf("\nEscolha o território ATACANTE (1-%d): ", tamanho);
    scanf("%d", &idAtacante);
    limparBufferEntrada();

    printf("Escolha o território DEFENSOR (1-%d): ", tamanho);
    scanf("%d", &idDefensor);
    limparBufferEntrada();

    if (idAtacante < 1 || idAtacante > tamanho || idDefensor < 1 || idDefensor > tamanho) {
        printf("IDs inválidos!\n");
        return 0;
    }

    if (idAtacante == idDefensor) {
        printf("Um território não pode atacar a si mesmo!\n");
        return 0;
    }

    if (strcmp(mapa[idAtacante - 1].cor, mapa[idDefensor - 1].cor) == 0) {
        printf("\nNão é possível atacar um território aliado (%s)!\n", mapa[idAtacante - 1].cor);
        return 0;
    }

    atacar(&mapa[idAtacante - 1], &mapa[idDefensor - 1]);
    return 0;
}

void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n==============================================\n");
    printf("                INÍCIO DA BATALHA             \n");
    printf("==============================================\n");
    printf("Atacante: %s (%s) - Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) - Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);

    if (atacante->tropas <= 1) {
        printf("\nO território atacante não tem tropas suficientes!\n");
        return;
    }

    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\n  Dado do Ataque: %d\n", dadoAtaque);
    printf("   Dado da Defesa: %d\n", dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\nO atacante venceu a rodada!\n");
        defensor->tropas -= 1;

        if (defensor->tropas <= 0) {
            printf("O território %s foi conquistado pelo exército %s!\n",
                   defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            int tropasTransferidas = atacante->tropas / 2;
            if (tropasTransferidas < 1) tropasTransferidas = 1;
            defensor->tropas = tropasTransferidas;
            atacante->tropas -= tropasTransferidas;
        }
    } else {
        printf("\nO defensor resistiu!\n");
        atacante->tropas -= 1;
    }

    printf("\n--- Resultado após a batalha ---\n");
    printf("%s (%s) - Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("%s (%s) - Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);
}

// ============================================================================
//                       VERIFICAÇÃO DE MISSÕES
// ============================================================================

int verificarVitoriaMissao(const Territorio* mapa, int tamanho, int idMissao, const char* corJogador) {
    switch (idMissao) {
        case 1: { // destruir o exército verde
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, "Verde") == 0)
                    return 0;
            }
            return 1;
        }
        case 2: { // conquistar 3 territórios
            int total = 0;
            for (int i = 0; i < tamanho; i++)
                if (strcmp(mapa[i].cor, corJogador) == 0)
                    total++;
            return (total >= 3);
        }
        case 3: { // dominar todos
            for (int i = 0; i < tamanho; i++)
                if (strcmp(mapa[i].cor, corJogador) != 0)
                    return 0;
            return 1;
        }
        case 4: { // somar 20 tropas
            int soma = 0;
            for (int i = 0; i < tamanho; i++)
                if (strcmp(mapa[i].cor, corJogador) == 0)
                    soma += mapa[i].tropas;
            return (soma >= 20);
        }
        case 5: { // controlar 2 territórios azuis
            int cont = 0;
            for (int i = 0; i < tamanho; i++)
                if (strcmp(mapa[i].cor, "Azul") == 0 && strcmp(corJogador, "Azul") == 0)
                    cont++;
            return (cont >= 2);
        }
    }
    return 0;
}

// ============================================================================
//                            FUNÇÕES UTILITÁRIAS
// ============================================================================

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
