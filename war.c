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
#define TOTAL_MISSOES 3

struct Territorios{
    char *nome;
    char *cor;
    int tropas;
    int areas;
    int dono;
    int sequencia;
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

void exibirMapa(const struct Territorios *territorio, int qtd){

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

        territorio[i].areas = 1;
        territorio[i].dono = i;

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
    printf("Ataque (%s) : %d | Defesa (%s): %d\n", atacante->cor,dado_atacante,defensor->cor,dado_defensor);
    
    
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
        atacante->sequencia++;
        printf("\nVITORIA DO ATAQUE! O defensor perdeu 1 tropa!");
        if (defensor->tropas > 0) defensor->tropas--;

        if (defensor->tropas < 1 && strcmp(defensor->cor, atacante->cor) !=0 ){
            printf("\nO Exercito %s conquistou o terreno %s", atacante->cor, defensor->nome);
            defensor->cor = atacante->cor;
            atacante->areas++;
             if (defensor->areas > 0) defensor->areas--;
            defensor->dono = atacante->dono;
            
        }
        // Defesa vence
    } else {
        atacante->sequencia = 0;
        printf("\nVITORIA DA DEFESA! O atacante perdeu 1 tropa!");
        if (atacante->tropas > 0 ) atacante->tropas--;
        if (atacante->tropas < 1 && strcmp(atacante->cor, defensor->cor) !=0){
            printf("\nO Exercito %s conquistou o terreno %s", defensor->cor, atacante->nome);
            atacante->cor =defensor->cor;
            defensor->areas++;
            if (atacante->areas > 0 ) atacante->areas--;
            atacante->dono = defensor->dono;

            
        }
    }
}

void faseDeAtaque(struct Territorios *territorio, int qtd){

    int idx_atacante, idx_defensor;
    printf("\n--- FASE DE ATAQUE --- \n");
    printf("Escolha o territorio atacante ( 1 a %d, ou 0 para CANCELAR ): ", qtd);
    scanf("%d", &idx_atacante);
    LimparBufferEntrada();

    if (idx_atacante == 0){
        printf("\nRetornando ao menu principal...");
        return;
    }
    
    printf("Escolha o territorio defensor ( 1 a %d, ou 0 para CANCELAR ): ", qtd);
    scanf("%d", &idx_defensor);
    LimparBufferEntrada();

    if (idx_defensor == 0){
        printf("\nRetornando ao menu principal...");
        return;
    }


    simularAtaque(&territorio[idx_atacante - 1], &territorio[idx_defensor - 1]);


    printf("\nPressione Enter para o próximo turno...");
    getchar();
      

    

}

void exibirMenuPrincipal(){

    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1 - ATACAR!!\n");
    printf("2 - Verificar Missao.\n");
    printf("0 - Sair\n");
    printf("Escolha sua ação: ");

}

int sortearMissao( int totalMissoes){

    int indice = rand() % totalMissoes;
    return indice;

}

void exibirMissao(char *missao[], int id_missao, struct Territorios *territorio){

    printf("\n--- SUA MISSÃO (Exercito %s) ---\n", territorio->cor);
    printf("%s\n", missao[id_missao]);
}

int verificarVitoria(int id_missao, struct Territorios *territorio, int id_jogador, int qtd_territorios){
    

    switch (id_missao){
        case 0:
            int america = 0, europa = 0;
            
            for (int i = 0; i < qtd_territorios; i++){
                if (strcmp(territorio[i].nome, "Americas") == 0 && territorio[i].dono == id_jogador){
                    america = 1;
                }
                if (strcmp(territorio[i].nome, "Europa") == 0 && territorio[i].dono == id_jogador){
                    europa = 1;
                }
            }
            
            if (america && europa){
                return 1;
            }
            break;
        case 1:
            if (territorio[id_jogador].sequencia == 3){
                return 1;
            }
            break;
        case 2:
            if (territorio[id_jogador].areas == 3){
                return 1;
            }
            break;
        default:
            return 0;
            break;
    }
    return 0;
}



int main() {

    srand(time(NULL));
    char* missoes[] = {
        "Conquistar os territorios da America e Europa.",
        "Realizar 3 ataques bem sucedidos, em sequencia.",
        "Conquistar +2 Territorios.",
    };

    int id_missao = sortearMissao(TOTAL_MISSOES);
    // int id_missao = 1; // input para teste de missão
    struct Territorios *territorio;
    int qtd_territorios, opcao;
    
    
    printf("\nDefina a quantidade de territorios: ");
    scanf("%d", &qtd_territorios);
    LimparBufferEntrada();
    
    if (alocarMapa(&territorio, qtd_territorios) != 0 ) return 1;
    
    for (int i = 0; i < qtd_territorios; i++) {
        territorio[i].nome = (char *) malloc(MAX_char * sizeof(char));
        territorio[i].cor  = (char *) malloc(MAX_CORES * sizeof(char));
    }
    
    
    
    // Inputs teste para pular a fase de cadastro
    
    // territorio[0].nome = "Americas";
    // territorio[0].cor = "Amarelo";
    // territorio[0].tropas = 10;
    
    // territorio[1].nome = "Europa";
    // territorio[1].cor = "Verde";
    // territorio[1].tropas = 10;
    
    // territorio[2].nome = "Asia";
    // territorio[2].cor = "Vermelho";
    // territorio[2].tropas = 10;
    
    // territorio[3].nome = "Africa";
    // territorio[3].cor = "Azul";
    // territorio[3].tropas = 10;
    
    // territorio[4].nome = "Oceania";
    // territorio[4].cor = "Branco";
    // territorio[4].tropas = 10;

    // for (int i = 0; i < qtd_territorios; i++){
    //     territorio[i].areas = 1;
    //     territorio[i].dono = i;
    // }
    // ------------------------------------------------------

    inicializarTerritorios(territorio, qtd_territorios); // Cadastro de territorios
    
    int id_jogador = rand() % qtd_territorios; // Sorteia aleatoriamente quem será o jogador da rodada.
    
    do{
        exibirMapa(territorio, qtd_territorios);
        exibirMissao(missoes, id_missao, &territorio[id_jogador]);
        exibirMenuPrincipal();
        scanf("%d",&opcao);
        LimparBufferEntrada();
        switch (opcao){
            case 0:
                printf("\nSaindo do sistema...");
                break;
            case 1:
                faseDeAtaque(territorio, qtd_territorios);
                break;
            case 2:
                exibirMissao(missoes, id_missao, &territorio[id_jogador]);
                if (verificarVitoria(id_missao, territorio,id_jogador,qtd_territorios)){
                printf("\nPARABÉNS !! Você cumpriu sua missão. \n");
                break;
                } else {
                    printf("\nVocê ainda não cumpriu sua missão!\n");
                }
                printf("\nPressione Enter para continuar..");
                getchar();

        }
        if (territorio[id_jogador].tropas == 0){
            printf("\nVOCÊ PERDEU! Todas as suas tropas foram elimidadas. \n");
            break;
        }

        if (verificarVitoria(id_missao, territorio,id_jogador,qtd_territorios)){
            printf("\nPARABÉNS !! Você cumpriu sua missão. \n");
            break;
        }
    }while (opcao != 0);

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
