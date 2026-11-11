/*
 * PSE em Acao - Sistema de Planejamento e Acompanhamento de Acoes de Saude na Escola
 * AEP 2026 - Engenharia de Software / Analise e Desenvolvimento de Sistemas - UniCesumar
 *
 * Integrantes:
 *   Guilherme Bispo Garcia de Oliveira - RA 26005126-2
 *   Henry Hashimoto Tanaka             - RA 26002577-2
 *
 * Compilar: gcc pse_em_acao.c -o pse_em_acao -Wall -Wextra
 * Executar: ./pse_em_acao (Linux/macOS)    ou   pse_em_acao.exe (Windows)
 *
 * Usa somente dados ficticios e coletivos sobre as acoes. Nao armazena nome,
 * diagnostico, prontuario ou qualquer dado individual de estudantes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACOES 50
#define MAX_PARTICIPANTES 99999
#define TAM_TEXTO 61
#define TAM_DATA 11
#define TAM_LINHA 256
#define QTD_EXEMPLOS 8

#define PLANEJADA 1
#define REALIZADA 2
#define CANCELADA 3

typedef struct {
    int codigo;
    char escola[TAM_TEXTO];
    char tema[TAM_TEXTO];
    char publicoAlvo[TAM_TEXTO];
    char responsavel[TAM_TEXTO];
    char dataPrevista[TAM_DATA];
    int qtdPrevista;
    int qtdEfetiva;
    int situacao;
} Acao;

const char NOMES_SITUACAO[4][10] = {"-", "PLANEJADA", "REALIZADA", "CANCELADA"};

/* Funcionalidades do menu */
void exibirMenu(int total);
int cadastrarAcao(Acao acoes[], int total);
void listarAcoes(Acao acoes[], int total);
void pesquisarAcoes(Acao acoes[], int total);
void atualizarSituacao(Acao acoes[], int total);
void gerarResumo(Acao acoes[], int total);
void exibirResumoPorEscola(Acao acoes[], int total);
int carregarDadosDemonstracao(Acao acoes[], int total);

/* Leitura e validacao de entradas */
void lerLinha(char destino[], int tamanho);
void removerEspacosExtremos(char texto[]);
int lerInteiro(const char rotulo[]);
int lerInteiroNoIntervalo(const char rotulo[], int minimo, int maximo);
int lerQuantidade(const char rotulo[], int minimo);
void lerTextoObrigatorio(const char rotulo[], char destino[], int tamanho);
int dataValida(const char data[]);
void lerData(const char rotulo[], char destino[]);
int lerSituacao(void);
int lerParticipantesEfetivos(int qtdPrevista);

int confirmar(const char pergunta[]);
void pausar(void);

/* Funcoes auxiliares */
int buscarIndicePorCodigo(Acao acoes[], int total, int codigo);
int contemTexto(const char texto[], const char trecho[]);
int textosIguais(const char a[], const char b[]);
double calcularPercentual(int parte, int todo);
void exibirAcao(Acao acao);
Acao criarAcao(int codigo, const char escola[], const char tema[], const char publico[],
               const char responsavel[], const char data[], int prevista, int efetiva,
               int situacao);

int main(void) {
    Acao acoes[MAX_ACOES];
    int total = 0;
    int opcao;
    int executando = 1;

     printf("\nAVISO DE PRIVACIDADE: use somente dados ficticios e coletivos.\n");
     printf("Nao registre nomes, diagnosticos ou dados individuais de estudantes.\n");

     while (executando) {
         exibirMenu(total);
         opcao = lerInteiro("Escolha uma opcao: ");

          switch (opcao) {
              case 1:
                  total = cadastrarAcao(acoes, total);
                  break;
              case 2:
                  listarAcoes(acoes, total);
                  break;
              case 3:
                  pesquisarAcoes(acoes, total);
                  break;
              case 4:
                  atualizarSituacao(acoes, total);
                  break;
              case 5:
                  gerarResumo(acoes, total);
                  break;
              case 6:
                  total = carregarDadosDemonstracao(acoes, total);
                  break;
              case 0:
                  if (confirmar("Os dados em memoria serao perdidos. Deseja sair? (S/N): ")) {
                      executando = 0;
                  }
                  break;
              default:
                  printf("\n[ERRO] Opcao inexistente. Escolha um numero de 0 a 6.\n");
          }

          if (executando) {
              pausar();
          }
     }

     printf("\nPrograma encerrado. Obrigado por usar o PSE em Acao!\n");
     return 0;
}

/* ===================== FUNCIONALIDADES DO MENU ===================== */

void exibirMenu(int total) {
    printf("\n============================================================\n");
    printf("   PSE EM ACAO - Acoes de Saude na Escola\n");
    printf("   Acoes cadastradas: %d de %d\n", total, MAX_ACOES);
    printf("============================================================\n");
    printf(" 1 - Cadastrar acao\n");
    printf(" 2 - Listar acoes\n");
    printf(" 3 - Pesquisar acao (codigo, escola ou tema)\n");
    printf(" 4 - Atualizar situacao / registrar participantes\n");
    printf(" 5 - Gerar resumo geral\n");

     printf(" 6 - Carregar dados de demonstracao (ficticios)\n");
     printf(" 0 - Sair\n");
     printf("------------------------------------------------------------\n");
}

int cadastrarAcao(Acao acoes[], int total) {
    Acao nova;

     printf("\n===== CADASTRAR ACAO DO PSE =====\n");
     if (total >= MAX_ACOES) {
         printf("[ERRO] Limite de armazenamento atingido (%d acoes). Cadastro nao realizado.\n",
                MAX_ACOES);
         return total;
     }

     nova.codigo = lerInteiro("Codigo da acao (numero inteiro maior que zero): ");
     while (nova.codigo <= 0 || buscarIndicePorCodigo(acoes, total, nova.codigo) != -1) {
         if (nova.codigo <= 0) {
             printf("[ERRO] O codigo deve ser maior que zero.\n");
         } else {
             printf("[ERRO] Ja existe uma acao com o codigo %d. Informe outro.\n", nova.codigo);
         }
         nova.codigo = lerInteiro("Codigo da acao: ");
     }

    lerTextoObrigatorio("Escola: ", nova.escola, TAM_TEXTO);
    lerTextoObrigatorio("Tema (ex.: Vacinacao, Saude bucal, Cultura de paz): ", nova.tema,
TAM_TEXTO);
    lerTextoObrigatorio("Publico-alvo (ex.: 6o ao 9o ano): ", nova.publicoAlvo, TAM_TEXTO);
    lerTextoObrigatorio("Responsavel (equipe ou setor, ex.: UBS Centro - ESF 01): ",
                        nova.responsavel, TAM_TEXTO);
    lerData("Data prevista (DD/MM/AAAA): ", nova.dataPrevista);
    nova.qtdPrevista = lerQuantidade("Quantidade prevista de participantes: ", 1);

     nova.situacao = lerSituacao();
     nova.qtdEfetiva = 0;
     if (nova.situacao == REALIZADA) {
         nova.qtdEfetiva = lerParticipantesEfetivos(nova.qtdPrevista);
     }

     acoes[total] = nova;
     total++;
     printf("\n[OK] Acao %d cadastrada com sucesso! Total de acoes: %d.\n", nova.codigo, total);
     return total;
}

void listarAcoes(Acao acoes[], int total) {
    int i;

     printf("\n===== ACOES CADASTRADAS (%d) =====\n", total);
     if (total == 0) {
         printf("Nenhuma acao cadastrada. Use a opcao 1 ou a opcao 6.\n");
         return;
     }
     for (i = 0; i < total; i++) {
         exibirAcao(acoes[i]);
     }
     printf("------------------------------------------------------------\n");
}

void pesquisarAcoes(Acao acoes[], int total) {
    int tipo, codigo, indice, i;
    int encontradas = 0;
    char termo[TAM_LINHA];

     printf("\n===== PESQUISAR ACOES =====\n");
     if (total == 0) {
         printf("Nenhuma acao cadastrada para pesquisar.\n");
         return;
     }

     printf(" 1 - Por codigo\n 2 - Por escola\n 3 - Por tema\n");
     tipo = lerInteiroNoIntervalo("Tipo de pesquisa: ", 1, 3);

     if (tipo == 1) {

          codigo = lerInteiro("Codigo: ");
          indice = buscarIndicePorCodigo(acoes, total, codigo);
          if (indice == -1) {
              printf("[AVISO] Nenhuma acao encontrada com o codigo %d.\n", codigo);
          } else {
              exibirAcao(acoes[indice]);
              printf("------------------------------------------------------------\n");
          }
          return;
     }

     if (tipo == 2) {
         lerTextoObrigatorio("Escola (nome ou parte do nome): ", termo, TAM_LINHA);
     } else {
         lerTextoObrigatorio("Tema (nome ou parte do nome): ", termo, TAM_LINHA);
     }

     for (i = 0; i < total; i++) {
         if ((tipo == 2 && contemTexto(acoes[i].escola, termo)) ||
             (tipo == 3 && contemTexto(acoes[i].tema, termo))) {
             exibirAcao(acoes[i]);
             encontradas++;
         }
     }

     if (encontradas == 0) {
         printf("[AVISO] Nenhuma acao encontrada para \"%s\".\n", termo);
     } else {
         printf("------------------------------------------------------------\n");
         printf("%d acao(oes) encontrada(s).\n", encontradas);
     }
}

void atualizarSituacao(Acao acoes[], int total) {
    int codigo, indice, novaSituacao;

     printf("\n===== ATUALIZAR SITUACAO / REGISTRAR PARTICIPANTES =====\n");
     if (total == 0) {
         printf("Nenhuma acao cadastrada para atualizar.\n");
         return;
     }

     codigo = lerInteiro("Codigo da acao: ");
     indice = buscarIndicePorCodigo(acoes, total, codigo);
     if (indice == -1) {
         printf("[ERRO] Acao com codigo %d nao encontrada.\n", codigo);
         return;
     }

     printf("Acao selecionada:\n");
     exibirAcao(acoes[indice]);
     printf("------------------------------------------------------------\n");

    novaSituacao = lerSituacao();
    if (novaSituacao == REALIZADA) {
        acoes[indice].qtdEfetiva = lerParticipantesEfetivos(acoes[indice].qtdPrevista);
    } else {
        if (acoes[indice].situacao == REALIZADA) {
            printf("[AVISO] A acao deixou de estar realizada: participantes registrados foram zerados.\n");
        }
        acoes[indice].qtdEfetiva = 0;
    }
    acoes[indice].situacao = novaSituacao;

     printf("[OK] Acao %d atualizada para %s.\n", codigo, NOMES_SITUACAO[novaSituacao]);
}

void gerarResumo(Acao acoes[], int total) {
    int i;
    int planejadas = 0, realizadas = 0, canceladas = 0;
    int previstoRealizadas = 0, efetivoTotal = 0, previstoPlanejadas = 0;

     printf("\n===== RESUMO GERAL DAS ACOES =====\n");
     if (total == 0) {

          printf("Nenhuma acao cadastrada. Nao ha dados para resumir.\n");
          return;
     }

     for (i = 0; i < total; i++) {
         if (acoes[i].situacao == PLANEJADA) {
             planejadas++;
             previstoPlanejadas += acoes[i].qtdPrevista;
         } else if (acoes[i].situacao == REALIZADA) {
             realizadas++;
             previstoRealizadas += acoes[i].qtdPrevista;
             efetivoTotal += acoes[i].qtdEfetiva;
         } else {
             canceladas++;
         }
     }

    printf("%-40s %d\n", "Total de acoes cadastradas:", total);
    printf("%-40s %d (%.1f%%)\n", " Planejadas:", planejadas, calcularPercentual(planejadas,
total));
    printf("%-40s %d (%.1f%%)\n", " Realizadas:", realizadas, calcularPercentual(realizadas,
total));
    printf("%-40s %d (%.1f%%)\n", " Canceladas:", canceladas, calcularPercentual(canceladas,
total));

     printf("\nParticipacao nas acoes realizadas:\n");
     printf("%-40s %d\n", " Participantes previstos:", previstoRealizadas);
     printf("%-40s %d\n", " Participantes efetivos:", efetivoTotal);
     if (realizadas > 0) {
         printf("%-40s %.1f%%\n", " Percentual de participacao:",
                calcularPercentual(efetivoTotal, previstoRealizadas));
     } else {
         printf("%-40s %s\n", " Percentual de participacao:", "sem acoes realizadas");
     }

     printf("\n%-40s %d\n", "Previstos em acoes ainda planejadas:", previstoPlanejadas);

     exibirResumoPorEscola(acoes, total);
}

void exibirResumoPorEscola(Acao acoes[], int total) {
    int i, j, jaContada, qtdAcoes, qtdRealizadas, participantes;

     printf("\nParticipantes atendidos por escola:\n");
     printf("%-30s %6s %11s %14s\n", "ESCOLA", "ACOES", "REALIZADAS", "PARTICIPANTES");

     for (i = 0; i < total; i++) {
         jaContada = 0;
         for (j = 0; j < i; j++) {
             if (textosIguais(acoes[j].escola, acoes[i].escola)) {
                 jaContada = 1;
             }
         }
         if (jaContada) {
             continue;
         }

          qtdAcoes = 0;
          qtdRealizadas = 0;
          participantes = 0;
          for (j = i; j < total; j++) {
              if (textosIguais(acoes[j].escola, acoes[i].escola)) {
                  qtdAcoes++;
                  if (acoes[j].situacao == REALIZADA) {
                      qtdRealizadas++;
                      participantes += acoes[j].qtdEfetiva;
                  }
              }
          }
          printf("%-30.30s %6d %11d %14d\n", acoes[i].escola, qtdAcoes, qtdRealizadas, participantes);
     }
}

int carregarDadosDemonstracao(Acao acoes[], int total) {
    Acao exemplos[QTD_EXEMPLOS];

     int i;
     int inseridas = 0, ignoradas = 0;

     exemplos[0] = criarAcao(101, "EM Jardim das Flores", "Vacinacao", "6o ao 9o ano",
                             "UBS Centro - ESF 01", "08/04/2026", 180, 164, REALIZADA);
     exemplos[1] = criarAcao(102, "EM Jardim das Flores", "Saude bucal", "1o ao 5o ano",
                             "UBS Centro - Saude Bucal", "14/05/2026", 150, 142, REALIZADA);
     exemplos[2] = criarAcao(103, "EE Prof. Ana Ribeiro", "Vacinacao", "Estudantes de 9 a 14 anos",
                             "UBS Vila Nova", "21/05/2026", 120, 97, REALIZADA);
     exemplos[3] = criarAcao(104, "EE Prof. Ana Ribeiro", "Cultura de paz", "Ensino medio",
                             "Coordenacao PSE", "18/06/2026", 200, 0, CANCELADA);
     exemplos[4] = criarAcao(105, "EM Vila Esperanca", "Alimentacao saudavel", "Educacao infantil",
                             "Equipe eMulti", "12/08/2026", 90, 88, REALIZADA);
     exemplos[5] = criarAcao(106, "EM Vila Esperanca", "Saude mental", "8o e 9o ano",
                             "Equipe eMulti - Psicologia", "24/11/2026", 60, 0, PLANEJADA);
     exemplos[6] = criarAcao(107, "CEEBJA Novo Horizonte", "Vacinacao", "Turmas da EJA",
                             "UBS Centro - ESF 01", "02/12/2026", 70, 0, PLANEJADA);
     exemplos[7] = criarAcao(108, "EM Jardim das Flores", "Atividade fisica", "3o ao 5o ano",
                             "UBS Centro - ESF 01", "09/12/2026", 110, 0, PLANEJADA);

     printf("\n===== CARREGAR DADOS DE DEMONSTRACAO =====\n");
     for (i = 0; i < QTD_EXEMPLOS; i++) {
         if (total >= MAX_ACOES) {
             printf("[AVISO] Limite de armazenamento atingido. Carga interrompida.\n");
             break;
         }
         if (buscarIndicePorCodigo(acoes, total, exemplos[i].codigo) != -1) {
             ignoradas++;
         } else {
             acoes[total] = exemplos[i];
             total++;
             inseridas++;
         }
     }

     printf("[OK] %d acao(oes) ficticia(s) carregada(s). %d ignorada(s) por codigo ja existente.\n",
            inseridas, ignoradas);
     return total;
}

/* ===================== LEITURA E VALIDACAO DE ENTRADAS ===================== */

void lerLinha(char destino[], int tamanho) {
    int i = 0;
    int c;

     if (fgets(destino, tamanho, stdin) == NULL) {
         printf("\nEntrada encerrada. Finalizando o programa.\n");
         exit(0);
     }

     while (destino[i] != '\0' && destino[i] != '\n') {
         i++;
     }
     if (destino[i] == '\n') {
         destino[i] = '\0';
     } else {
         /* linha maior que o buffer: descarta o restante para nao afetar a proxima leitura */
         c = getchar();
         while (c != '\n' && c != EOF) {
             c = getchar();
         }
     }
     removerEspacosExtremos(destino);
}

void removerEspacosExtremos(char texto[]) {
    int inicio = 0;
    int fim = (int)strlen(texto) - 1;
    int i;

     while (texto[inicio] != '\0' && isspace((unsigned char)texto[inicio])) {
         inicio++;
     }
     while (fim >= inicio && isspace((unsigned char)texto[fim])) {

         fim--;
     }
     for (i = 0; inicio + i <= fim; i++) {
         texto[i] = texto[inicio + i];
     }
     texto[i] = '\0';
}

int lerInteiro(const char rotulo[]) {
    char linha[TAM_LINHA];
    char sobra;
    int valor;

     while (1) {
         printf("%s", rotulo);
         lerLinha(linha, TAM_LINHA);

          if (strlen(linha) == 0) {
              printf("[ERRO] Campo obrigatorio. Digite um numero.\n");
          } else if (strlen(linha) > 9) {
              printf("[ERRO] Numero muito grande.\n");
          } else if (sscanf(linha, "%d %c", &valor, &sobra) != 1) {
              printf("[ERRO] Entrada invalida. Digite apenas numeros inteiros.\n");
          } else {
              return valor;
          }
     }
}

int lerInteiroNoIntervalo(const char rotulo[], int minimo, int maximo) {
    int valor = lerInteiro(rotulo);

     while (valor < minimo || valor > maximo) {
         printf("[ERRO] Valor fora do intervalo permitido (%d a %d).\n", minimo, maximo);
         valor = lerInteiro(rotulo);
     }
     return valor;
}

int lerQuantidade(const char rotulo[], int minimo) {
    int valor = lerInteiro(rotulo);

     while (valor < minimo || valor > MAX_PARTICIPANTES) {
         if (valor < 0) {
             printf("[ERRO] A quantidade nao pode ser negativa.\n");
         } else if (valor < minimo) {
             printf("[ERRO] A quantidade deve ser no minimo %d.\n", minimo);
         } else {
             printf("[ERRO] Quantidade acima do limite permitido (%d).\n", MAX_PARTICIPANTES);
         }
         valor = lerInteiro(rotulo);
     }
     return valor;
}

void lerTextoObrigatorio(const char rotulo[], char destino[], int tamanho) {
    char linha[TAM_LINHA];

     while (1) {
         printf("%s", rotulo);
         lerLinha(linha, TAM_LINHA);

          if (strlen(linha) == 0) {
              printf("[ERRO] Campo obrigatorio. Nao pode ficar vazio.\n");
          } else if ((int)strlen(linha) > tamanho - 1) {
              printf("[ERRO] Texto muito longo. Use no maximo %d caracteres.\n", tamanho - 1);
          } else {
              strcpy(destino, linha);
              return;
          }
     }
}

int dataValida(const char data[]) {
    int diasNoMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

     int dia, mes, ano, i;

     if (strlen(data) != 10 || data[2] != '/' || data[5] != '/') {
         return 0;
     }
     for (i = 0; i < 10; i++) {
         if (i != 2 && i != 5 && !isdigit((unsigned char)data[i])) {
             return 0;
         }
     }

     dia = (data[0] - '0') * 10 + (data[1] - '0');
     mes = (data[3] - '0') * 10 + (data[4] - '0');
     ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100 + (data[8] - '0') * 10 + (data[9] - '0');

     if (ano < 2000 || ano > 2100 || mes < 1 || mes > 12) {
         return 0;
     }
     if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) {
         diasNoMes[2] = 29;
     }
     return dia >= 1 && dia <= diasNoMes[mes];
}

void lerData(const char rotulo[], char destino[]) {
    char linha[TAM_LINHA];

     while (1) {
         printf("%s", rotulo);
         lerLinha(linha, TAM_LINHA);

        if (strlen(linha) == 0) {
            printf("[ERRO] Campo obrigatorio. Nao pode ficar vazio.\n");
        } else if (!dataValida(linha)) {
            printf("[ERRO] Data invalida. Use DD/MM/AAAA com uma data existente (ex.: 15/03/2026).\n");
        } else {
            strcpy(destino, linha);
            return;
        }
    }
}

int lerSituacao(void) {
    printf("Situacao: 1 - Planejada | 2 - Realizada | 3 - Cancelada\n");
    return lerInteiroNoIntervalo("Escolha a situacao: ", PLANEJADA, CANCELADA);
}

int lerParticipantesEfetivos(int qtdPrevista) {
    int efetiva = lerQuantidade("Quantidade efetiva de participantes: ", 0);

     if (efetiva > qtdPrevista) {
         printf("[AVISO] Participacao acima do previsto (%d de %d). Valor mantido.\n",
                efetiva, qtdPrevista);
     }
     return efetiva;
}

int confirmar(const char pergunta[]) {
    char resposta[TAM_LINHA];

     while (1) {
         printf("%s", pergunta);
         lerLinha(resposta, TAM_LINHA);

          if (strlen(resposta) == 1 && toupper((unsigned char)resposta[0]) == 'S') {
              return 1;
          }
          if (strlen(resposta) == 1 && toupper((unsigned char)resposta[0]) == 'N') {
              return 0;
          }
          printf("[ERRO] Responda apenas S ou N.\n");
     }
}

void pausar(void) {
    char linha[TAM_LINHA];

     printf("\nPressione ENTER para voltar ao menu...");
     lerLinha(linha, TAM_LINHA);
}

/* ===================== FUNCOES AUXILIARES ===================== */

int buscarIndicePorCodigo(Acao acoes[], int total, int codigo) {
    int i;

     for (i = 0; i < total; i++) {
         if (acoes[i].codigo == codigo) {
             return i;
         }
     }
     return -1;
}

/* Verifica se "trecho" aparece em "texto", sem diferenciar maiusculas e minusculas */
int contemTexto(const char texto[], const char trecho[]) {
    int i, j;
    int tamTexto = (int)strlen(texto);
    int tamTrecho = (int)strlen(trecho);

     for (i = 0; i + tamTrecho <= tamTexto; i++) {
         j = 0;
         while (j < tamTrecho &&
                tolower((unsigned char)texto[i + j]) == tolower((unsigned char)trecho[j])) {
             j++;
         }
         if (j == tamTrecho) {
             return 1;
         }
     }
     return 0;
}

int textosIguais(const char a[], const char b[]) {
    return strlen(a) == strlen(b) && contemTexto(a, b);
}

double calcularPercentual(int parte, int todo) {
    if (todo == 0) {
        return 0.0;
    }
    return parte * 100.0 / todo;
}

void exibirAcao(Acao acao) {
    printf("------------------------------------------------------------\n");
    printf("Codigo: %d | Situacao: %s\n", acao.codigo, NOMES_SITUACAO[acao.situacao]);
    printf(" Escola ........: %s\n", acao.escola);
    printf(" Tema ..........: %s\n", acao.tema);
    printf(" Publico-alvo ..: %s\n", acao.publicoAlvo);
    printf(" Responsavel ...: %s\n", acao.responsavel);
    printf(" Data prevista .: %s\n", acao.dataPrevista);
    if (acao.situacao == REALIZADA) {
        printf(" Participantes .: %d de %d previstos (%.1f%%)\n", acao.qtdEfetiva,
               acao.qtdPrevista, calcularPercentual(acao.qtdEfetiva, acao.qtdPrevista));
    } else {
        printf(" Participantes .: %d previstos\n", acao.qtdPrevista);
    }
}

Acao criarAcao(int codigo, const char escola[], const char tema[], const char publico[],
               const char responsavel[], const char data[], int prevista, int efetiva,
               int situacao) {
    Acao acao;

     acao.codigo = codigo;
     strcpy(acao.escola, escola);
     strcpy(acao.tema, tema);
     strcpy(acao.publicoAlvo, publico);

     strcpy(acao.responsavel, responsavel);
     strcpy(acao.dataPrevista, data);
     acao.qtdPrevista = prevista;
     acao.qtdEfetiva = efetiva;
     acao.situacao = situacao;
     return acao;
}
