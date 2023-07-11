#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct Aluno
{
    int matricula;
    char nome[50];
    float frequencia;
    float notas[3];
    float media;
    char resultado;
};

struct Aluno alunos[100];
int totalAlunos = 0;
int disciplinaEncerrada = 0;

void limparTela()
{
    system("clear || cls");
}

void pausar()
{
    printf("\nPressione Enter para continuar...");
    getchar();
}

void exibirMenuPrincipal()
{
    limparTela();
    printf("***************************************************************************************************\n");
    printf("\t\tGESTAO DE AVALIACOES - ALGORITMOS E PROGRAMACAO\n");
    printf("***************************************************************************************************\n");
    printf("1- Lancar avaliacoes e frequencias\n");
    printf("2- Consultar avaliacoes e frequencias lancadas\n");
    printf("3- Gerar relatorio de fechamento\n");
    printf("4- Apresentar estatisticas\n");
    printf("5- Sair do programa\n");
    printf("\n");
}

void lancarAvaliacoes()
{
    limparTela();
    printf("***************************************************************************************************\n");
    printf("\t\tTELA DE LANCAMENTOS DE NOTAS E FREQUENCIAS\n");
    printf("***************************************************************************************************\n");
    if (totalAlunos >= 100)
    {
        printf("Limite de alunos atingido.\n\n");
        pausar();
        return;
    }

    if (disciplinaEncerrada)
    {
        printf("A disciplina já foi encerrada. Não é possível lançar novas avaliações.\n\n");
        pausar();
        return;
    }

    struct Aluno aluno;

    printf("Matricula do aluno: ");
    scanf("%d", &aluno.matricula);
    getchar();

    printf("Nome do aluno: ");
    fgets(aluno.nome, 50, stdin);
    aluno.nome[strcspn(aluno.nome, "\n")] = '\0';

    printf("Frequencia do aluno (em porcentagem): ");
    scanf("%f", &aluno.frequencia);
    getchar();

    for (int i = 0; i < 3; i++)
    {
        printf("Nota %d do aluno: ", i + 1);
        scanf("%f", &aluno.notas[i]);
        getchar();
    }

    aluno.media = (aluno.notas[0] + aluno.notas[1] + aluno.notas[2]) / 3;

    if (aluno.frequencia >= 75 && aluno.media >= 6)
    {
        aluno.resultado = 'A';
    }
    else
    {
        aluno.resultado = 'R';
    }

    alunos[totalAlunos] = aluno;
    totalAlunos++;

    FILE *arquivo;
    arquivo = fopen("lancamentos.txt", "a");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de lancamentos.\n\n");
        pausar();
        return;
    }

    fprintf(arquivo, "%d;%s;%.2f;%.2f,%.2f,%.2f;%.2f;%c\n",
            aluno.matricula, aluno.nome, aluno.frequencia,
            aluno.notas[0], aluno.notas[1], aluno.notas[2],
            aluno.media, aluno.resultado);

    fclose(arquivo);

    printf("\nAvaliacoes lancadas com sucesso!\n\n");
    pausar();
}

void exibirLancamentos()
{
    limparTela();
    printf("***************************************************************************************************\n");
    printf("\t\tTELA DE LANCAMENTOS DE NOTAS E FREQUENCIAS\n");
    printf("***************************************************************************************************\n");

    if (totalAlunos == 0)
    {
        printf("Nenhum aluno foi lancado ate o momento.\n\n");
        pausar();
        return;
    }

    printf("Matricula\tNome\t\t\tFrequencia\tNota 1\tNota 2\tNota 3\tMedia\tResultado\n");
    printf("-----------------------------------------------------------------------------------------------------\n");

    FILE *arquivo;
    arquivo = fopen("lancamentos.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de lancamentos.\n\n");
        pausar();
        return;
    }

    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo))
    {
        int matricula;
        char nome[50];
        float frequencia;
        float notas[3];
        float media;
        char resultado;

        sscanf(linha, "%d;%[^;];%f;%f,%f,%f;%f;%c",
               &matricula, nome, &frequencia,
               &notas[0], &notas[1], &notas[2],
               &media, &resultado);

        printf("%-10d\t%-20s\t%.2f%%\t\t%.2f\t%.2f\t%.2f\t%.2f\t%c\n",
               matricula, nome, frequencia,
               notas[0], notas[1], notas[2], media, resultado);
    }

    fclose(arquivo);

    printf("\n");
    pausar();
}

void gerarRelatorio()
{
    //setlocale(LC_ALL, "pt_BR.UTF-8");
    limparTela();
    printf("***************************************************************************************************\n");
    printf("\t\tTELA DE GERACAO DO RELATORIO DE FECHAMENTO\n");
    printf("***************************************************************************************************\n");

    if (totalAlunos == 0)
    {
        printf("Nenhum aluno foi lancado ate o momento.\n\n");
        pausar();
        return;
    }

    if (disciplinaEncerrada)
    {
        printf("O relatório de fechamento já foi gerado anteriormente.\n\n");
        pausar();
        return;
    }

    char resposta;

    printf("Deseja realmente encerrar a disciplina? (S/N): ");
    scanf(" %c", &resposta);
    getchar();

    if (resposta != 'S' && resposta != 's')
    {
        printf("Operacao cancelada.\n\n");
        pausar();
        return;
    }

    FILE *arquivoLancamentos;
    arquivoLancamentos = fopen("lancamentos.txt", "r");

    if (arquivoLancamentos == NULL)
    {
        printf("Erro ao abrir o arquivo de lancamentos.\n\n");
        pausar();
        return;
    }

    FILE *arquivoRelatorio;
    arquivoRelatorio = fopen("relatorio.txt", "w");

    if (arquivoRelatorio == NULL)
    {
        printf("Erro ao criar o arquivo de relatorio.\n\n");
        fclose(arquivoLancamentos);
        pausar();
        return;
    }

    fprintf(arquivoRelatorio, "RELATORIO DE FECHAMENTO\n\n");
    fprintf(arquivoRelatorio, "Matricula\tNome\t\t\tFrequencia\tNotas\t\t\tMedia\tResultado\n");

    char linha[200];
    while (fgets(linha, sizeof(linha), arquivoLancamentos))
    {
        int matricula;
        char nome[50];
        float frequencia;
        float notas[3];
        float media;
        char resultado;

        sscanf(linha, "%d;%[^;];%f;%f,%f,%f;%f;%c",
               &matricula, nome, &frequencia,
               &notas[0], &notas[1], &notas[2],
               &media, &resultado);

        fprintf(arquivoRelatorio, "%d\t\t%s\t\t%.2f%%\t\t%.2f, %.2f, %.2f\t%.2f\t%c\n",
                matricula, nome, frequencia,
                notas[0], notas[1], notas[2],
                media, resultado);
    }

    fclose(arquivoLancamentos);
    fclose(arquivoRelatorio);

    arquivoLancamentos = fopen("lancamentos.txt", "w");
    fclose(arquivoLancamentos);

    printf("Relatorio de fechamento gerado com sucesso! O arquivo 'relatorio.txt' foi criado.\n\n");
    pausar();

    disciplinaEncerrada = 1;
}
void apresentarEstatisticas()
{
    limparTela();
    printf("***************************************************************************************************\n");
    printf("\t\tTELA DE ESTATISTICAS\n");
    printf("***************************************************************************************************\n");
    if (totalAlunos == 0)
    {
        printf("Nenhum aluno foi lancado ate o momento.\n\n");
        pausar();
        return;
    }

    int alunosAprovados = 0;
    int alunosReprovados = 0;
    float mediaGeral = 0;

    for (int i = 0; i < totalAlunos; i++)
    {
        struct Aluno aluno = alunos[i];
        mediaGeral += aluno.media;

        if (aluno.resultado == 'A')
        {
            alunosAprovados++;
        }
        else
        {
            alunosReprovados++;
        }
    }

    mediaGeral /= totalAlunos;

    printf("Total de alunos: %d\n", totalAlunos);
    printf("Alunos aprovados: %d\n", alunosAprovados);
    printf("Alunos reprovados: %d\n", alunosReprovados);
    printf("Media geral da turma: %.2f\n\n", mediaGeral);
    pausar();
}


int main()
{
    int opcao;

    do
    {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            if (disciplinaEncerrada)
            {
                printf("A disciplina já foi encerrada. Não é possível lançar novas avaliações e frequências.\n\n");
                pausar();
            }
            else
            {
                lancarAvaliacoes();
            }
            break;
        case 2:
            exibirLancamentos();
            break;
        case 3:
            gerarRelatorio();
            break;
        case 4:
            apresentarEstatisticas();
            break;
        case 5:
            printf("\nSaindo do programa...\n");
            break;
        default:
            printf("\nOpcao invalida. Tente novamente.\n");
            pausar();
            break;
        }

    } while (opcao != 5);

    return 0;
}
