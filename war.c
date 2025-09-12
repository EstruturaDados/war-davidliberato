// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CORES 10
#define MAX_char 30

struct Territorios{
    char *nome;
    char *cor;
    int tropas;
};

void LimparBufferEntrada(){
    int c;
    while ((c = getchar())!= '\n' && c != EOF);
}

void liberarMemoria(struct Territorios *territorio, int qtd){
      for (int i = 0; i < qtd; i++) {
        free(territorio[i].nome);
        free(territorio[i].cor);
    }
    free(territorio);
}

void exibirMapa(struct Territorios *territorio, int qtd){

    printf("\n=================================\n");
    printf("   MAPA DO MUNDO - ESTADO ATUAL \n");
    printf("=================================\n\n");

    // LISTANDO TERRITORIOS

    for (int i = 0; i < qtd; i++){

        printf("%d. %s (Exercito %s, Tropas: %d)\n", 
            i+1,
            territorio[i].nome,
            territorio[i].cor,
            territorio[i].tropas);

    };
}

int alocarMapa(struct Territorios **territorio, int qtd){

    *territorio = (struct Territorios *) calloc (qtd, sizeof(struct Territorios));

    if (*territorio == NULL){
        printf("Erro: falha ao alocar memoria. \n");
        return 1;
    }
    return 0;
}

void inicializarTerritorios(struct Territorios *territorio, int qtd){
    printf("=================================================\n");
    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo. \n", qtd);

    for (int i = 0; i < qtd; i++){
        printf("\n--- Cadastrar Território %d --- \n\n", i + 1);

        printf("Nome do Território: ");
        fgets(territorio[i].nome,MAX_char, stdin);
        printf("Cor do Exercito: ");
        fgets(territorio[i].cor,MAX_CORES, stdin);

        territorio[i].nome[strcspn(territorio[i].nome, "\n")] = '\0';
        territorio[i].cor[strcspn(territorio[i].cor, "\n")] = '\0';
        
        do {
        printf("Número de Tropas: ");
        scanf("%d", &territorio[i].tropas);
        LimparBufferEntrada();

        if (territorio[i].tropas < 0) {
            printf("Valor inválido! O mínimo é 0.\n");
        }
        } while (territorio[i].tropas < 0);
    }
    printf("\nCadastro concluido com Sucesso!\n");
}

int girarDado(){
    int dado = (rand() % 6) + 1;
    return dado;
}

void simularAtaque(struct Territorios *atacante, struct Territorios *defensor){
    int dado_atacante = girarDado();
    int dado_defensor = girarDado();
    printf("\n--- RESULTADO DA BATALHA --- \n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->cor,dado_atacante);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->cor,dado_defensor);

    
    
    if (dado_atacante == dado_defensor){
        printf("\nEMPATE! Nenhum exercito perdeu tropas! \n");
        return;
    }


    if (atacante->cor == defensor->cor){
        printf("Este territorio já pertence ao Exercito %s", atacante->cor);
        return;
    }

      //Ataque Vence
    if (dado_atacante > dado_defensor){
        printf("\nVITORIA DO ATAQUE! O defensor perdeu 1 tropa!");
        if (defensor->tropas > 0) defensor->tropas--;

        if (defensor->tropas < 1 && strcmp(defensor->cor, atacante->cor) !=0 ){
            printf("\nO Exercito %s conquistou o terreno %s", atacante->cor, defensor->nome);
            defensor->cor = atacante->cor;
        }
        // Defesa vence
    } else {
        printf("\nVITORIA DA DEFESA! O atacante perdeu 1 tropa!");
        if (atacante->tropas > 0 ) atacante->tropas--;
        if (atacante->tropas < 1 && strcmp(atacante->cor, defensor->cor) !=0){
            printf("\nO Exercito %s conquistou o terreno %s", defensor->cor, atacante->nome);
            atacante->cor =defensor->cor;
        }
    }
}



int main() {

    srand(time(NULL));

    struct Territorios *territorio;
    int qtd_territorios;

    printf("\nDefina a quantidade de territorios: ");
    scanf("%d", &qtd_territorios);
    LimparBufferEntrada();

    if (alocarMapa(&territorio, qtd_territorios) != 0 ) return 1;

    for (int i = 0; i < qtd_territorios; i++) {
        territorio[i].nome = (char *) malloc(MAX_char * sizeof(char));
        territorio[i].cor  = (char *) malloc(MAX_CORES * sizeof(char));
    }

    

    // Inputs teste

    // territorio[0].nome = "Americas";
    // territorio[0].cor = "Amarelo";
    // territorio[0].tropas = 3;

    // territorio[1].nome = "Europa";
    // territorio[1].cor = "Verde";
    // territorio[1].tropas = 2;

    // territorio[2].nome = "Asia";
    // territorio[2].cor = "Vermelho";
    // territorio[2].tropas = 4;

    // territorio[3].nome = "Africa";
    // territorio[3].cor = "Azul";
    // territorio[3].tropas = 3;

    // territorio[4].nome = "Oceania";
    // territorio[4].cor = "Branco";
    // territorio[4].tropas = 2;

    int sair = 0;
    int idx_atacante, idx_defensor;

    inicializarTerritorios(territorio, qtd_territorios);
    
    do {
        exibirMapa(territorio, qtd_territorios);
        printf("\n--- FASE DE ATAQUE --- \n");
        printf("Escolha o territorio atacante ( 1 a %d, ou 0 para sair ): ", qtd_territorios);
        scanf("%d", &idx_atacante);
        LimparBufferEntrada();

        if (idx_atacante == 0){
            printf("\nSaindo do sistema...");
            sair = 1;
            break;
        }
        
        printf("Escolha o territorio defensor ( 1 a %d, ou 0 para sair ): ", qtd_territorios);
        scanf("%d", &idx_defensor);
        LimparBufferEntrada();

        if (idx_defensor == 0){
            printf("\nSaindo do sistema...");
            sair = 1;
            break;
        }


        simularAtaque(&territorio[idx_atacante - 1], &territorio[idx_defensor - 1]);


        printf("\nPressione Enter para o próximo turno...");
        getchar();
      

    }while (sair != 1);
    
    


    // faseDeAtaque();
 

    

    
    
   


    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(territorio, qtd_territorios);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
