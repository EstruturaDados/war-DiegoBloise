// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (NÍVEL MESTRE)
// ============================================================================
//
// OBJETIVOS (VERSÃO FINAL):
// - Alocação dinâmica do mapa e modularização completa.
// - Simulação de batalhas com rolagem de dados aleatórios.
// - Sistema de 5 missões diferentes sorteadas no início do jogo.
// - Uso de malloc e ponteiros para armazenar missões.
// - Foco em: design, organização, clareza e lógica de jogo.
//
// ============================================================================
// AUTOR: Diego Bloise
// DATA: Outubro de 2025
// LINGUAGEM: C (ANSI C - padrão C99)
// COMPILAÇÃO (exemplo):
//     gcc war.c -o war
//
// EXECUÇÃO:
//     ./war
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

// ============================================================================
//                            ESTRUTURA DE DADOS
// ============================================================================

/**
 * @struct Territorio
 * @brief Representa um território no mapa do jogo.
 *
 * Cada território possui um nome, uma cor (exército ao qual pertence)
 * e a quantidade de tropas presentes nele.
 */
typedef struct {
    char nome[TAM_NOME]; /**< Nome do território */
    char cor[TAM_COR];   /**< Cor do exército que domina o território */
    int tropas;          /**< Quantidade de tropas alocadas */
} Territorio;

// ============================================================================
//                          PROTÓTIPOS DAS FUNÇÕES
// ============================================================================

/** @brief Configura o jogo solicitando o número de territórios. */
void gameSetup(int* totalTerritorios);

/** @brief Aloca dinamicamente o vetor de territórios (mapa). */
Territorio* alocarMapa(int tamanho);

/** @brief Cadastra os territórios e suas informações iniciais. */
void cadastrarTerritorios(Territorio* mapa, int tamanho);

/** @brief Loop principal do jogo, controlando ações e verificações. */
void gameLoop(Territorio* mapa, int tamanho, const char* corJogador, const char* missaoJogador);

/** @brief Exibe a lista de territórios cadastrados. */
void exibirTerritorios(const Territorio* mapa, int tamanho);

/** @brief Exibe a missão do jogador. */
void exibirMissao(const char* missao);

/** @brief Controla a fase de ataque entre territórios. */
int faseDeAtaque(Territorio* mapa, int tamanho);

/** @brief Executa o ataque entre dois territórios e atualiza o mapa. */
void atacar(Territorio* atacante, Territorio* defensor);

/** @brief Verifica se o jogador cumpriu sua missão específica. */
int verificarVitoriaMissao(const Territorio* mapa, int tamanho, const char* missao, const char* corJogador);

/** @brief Libera toda a memória alocada dinamicamente. */
void liberarMemoria(Territorio* mapa, char* missao);

/** @brief Limpa o buffer de entrada padrão. */
void limparBufferEntrada();

/** @brief Limpa o terminal (Windows/Linux). */
void limparTela();

/** @brief Pausa a execução até que o usuário pressione ENTER. */
void pausar();

/** @brief Sorteia uma missão e retorna uma string alocada dinamicamente. */
char* atribuirMissao();

/** @brief Identifica o tipo da missão sorteada. */
int identificarMissao(const char* missao);

// ============================================================================
//                           FUNÇÃO PRINCIPAL
// ============================================================================

/**
 * @brief Função principal do programa.
 *
 * Configura o ambiente, sorteia a missão, inicializa o mapa e
 * inicia o loop principal do jogo War estruturado.
 *
 * @return int Código de status do programa (0 = sucesso, 1 = erro).
 */
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    limparTela();
    printf("==============================================\n");
    printf("           WAR ESTRUTURADO - NÍVEL MESTRE     \n");
    printf("==============================================\n\n");

    int totalTerritorios;
    gameSetup(&totalTerritorios);

    // Alocação dinâmica do mapa
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

    // Sorteio e alocação dinâmica da missão
    char* missaoJogador = atribuirMissao();
    exibirMissao(missaoJogador);

    // Loop principal do jogo
    gameLoop(mapa, totalTerritorios, corJogador, missaoJogador);

    // Liberação de memória
    liberarMemoria(mapa, missaoJogador);

    printf("\nJogo encerrado. Até a próxima batalha!\n");
    return 0;
}

// ============================================================================
//                      CONFIGURAÇÃO E GERENCIAMENTO
// ============================================================================

/**
 * @brief Solicita e valida a quantidade de territórios do jogo.
 * @param totalTerritorios Ponteiro para armazenar o valor informado.
 */
void gameSetup(int* totalTerritorios) {
    printf("==============================================\n");
    printf("           PREPARAÇÃO DO GAME - WAR           \n");
    printf("==============================================\n\n");

    do {
        printf("Número de territórios: ");
        scanf("%d", totalTerritorios);
        limparBufferEntrada();

        if (*totalTerritorios < 5)
            printf("Deve haver pelo menos 5 territórios!\n");
    } while (*totalTerritorios < 5);
}

/**
 * @brief Aloca dinamicamente o vetor de territórios.
 * @param tamanho Quantidade total de territórios.
 * @return Ponteiro para o vetor alocado.
 */
Territorio* alocarMapa(int tamanho) {
    return (Territorio*) calloc(tamanho, sizeof(Territorio));
}

/**
 * @brief Libera toda a memória alocada no programa.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param missao Ponteiro para a string da missão.
 */
void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}

// ============================================================================
//                           CADASTRO E EXIBIÇÃO
// ============================================================================

/**
 * @brief Cadastra as informações de cada território.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho Quantidade de territórios.
 */
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    limparTela();
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

        pausar();
        limparTela();
    }
}

/**
 * @brief Exibe o estado atual dos territórios.
 */
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

/**
 * @brief Sorteia aleatoriamente uma missão e retorna uma cópia dinâmica.
 * @return Ponteiro para a missão sorteada.
 */
char* atribuirMissao() {
    const char* missoes[NUM_MISSOES] = {
        "Destruir o exército Verde.",
        "Conquistar 3 territórios.",
        "Dominar todos os territórios.",
        "Ter pelo menos 20 tropas somadas no total.",
        "Controlar ao menos 2 territórios Azuis."
    };

    int sorteada = rand() % NUM_MISSOES;
    char* missao = (char*) malloc(strlen(missoes[sorteada]) + 1);
    if (!missao) {
        printf("Erro ao alocar memória para a missão!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(missao, missoes[sorteada]);
    return missao;
}

/**
 * @brief Exibe a missão sorteada para o jogador.
 */
void exibirMissao(const char* missao) {
    printf("\n==============================================\n");
    printf("                  SUA MISSÃO                  \n");
    printf("==============================================\n");
    printf("%s\n", missao);
}

/**
 * @brief Retorna um identificador numérico com base no texto da missão.
 */
int identificarMissao(const char* missao) {
    if (strstr(missao, "Verde")) return 1;
    if (strstr(missao, "3 territórios")) return 2;
    if (strstr(missao, "todos os territórios")) return 3;
    if (strstr(missao, "20 tropas")) return 4;
    if (strstr(missao, "Azuis")) return 5;
    return 0;
}

// ============================================================================
//                              LOOP DO JOGO
// ============================================================================

/**
 * @brief Controla o fluxo principal de jogadas, ataques e verificação de vitória.
 */
void gameLoop(Territorio* mapa, int tamanho, const char* corJogador, const char* missaoJogador) {
    int opcao;
    int venceu = 0;

    do {
        limparTela();
        printf("==============================================\n");
        printf("                 ESTADO DO MAPA               \n");
        printf("==============================================\n");
        exibirTerritorios(mapa, tamanho);

        printf("\n1 - Atacar\n2 - Verificar Missão\n0 - Sair\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, tamanho);
                pausar();
                break;
            case 2:
                venceu = verificarVitoriaMissao(mapa, tamanho, missaoJogador, corJogador);
                if (venceu)
                    printf("\n🎉 Missão cumprida! Você venceu o jogo!\n");
                else
                    printf("\n❌ Missão ainda não concluída.\n");
                pausar();
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                pausar();
        }
    } while (opcao != 0 && !venceu);
}

// ============================================================================
//                          MECÂNICA DE BATALHA
// ============================================================================

/**
 * @brief Permite ao jogador escolher territórios para atacar e defender.
 */
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

/**
 * @brief Executa o ataque entre dois territórios com base em rolagem de dados.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    limparTela();
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
    printf("  Dado da Defesa: %d\n", dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\nO atacante venceu a rodada!\n");
        defensor->tropas -= 1;

        // Caso o defensor perca todas as tropas, o território muda de dono
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
//                        VERIFICAÇÃO DE MISSÕES
// ============================================================================

/**
 * @brief Avalia se a missão atual do jogador foi concluída.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarVitoriaMissao(const Territorio* mapa, int tamanho, const char* missao, const char* corJogador) {
    int idMissao = identificarMissao(missao);

    switch (idMissao) {
        case 1: // Destruir o exército verde
            for (int i = 0; i < tamanho; i++)
                if (strcmp(mapa[i].cor, "Verde") == 0)
                    return 0;
            return 1;

        case 2: // Conquistar 3 territórios
            { int total = 0;
              for (int i = 0; i < tamanho; i++)
                  if (strcmp(mapa[i].cor, corJogador) == 0)
                      total++;
              return (total >= 3);
            }

        case 3: // Dominar todos
            for (int i = 0; i < tamanho; i++)
                if (strcmp(mapa[i].cor, corJogador) != 0)
                    return 0;
            return 1;

        case 4: // Ter 20 tropas no total
            { int soma = 0;
              for (int i = 0; i < tamanho; i++)
                  if (strcmp(mapa[i].cor, corJogador) == 0)
                      soma += mapa[i].tropas;
              return (soma >= 20);
            }

        case 5: // Controlar 2 territórios azuis
            { int cont = 0;
              for (int i = 0; i < tamanho; i++)
                  if (strcmp(mapa[i].cor, "Azul") == 0 && strcmp(corJogador, "Azul") == 0)
                      cont++;
              return (cont >= 2);
            }
    }
    return 0;
}

// ============================================================================
//                           FUNÇÕES UTILITÁRIAS
// ============================================================================

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    limparBufferEntrada();
}
