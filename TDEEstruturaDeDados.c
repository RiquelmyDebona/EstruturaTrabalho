#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_MAX_LINHA 200
#define TAMANHO_ALFABETO 20

#define CHAR_PARA_INDICE(c) ((int)c - (int)'0')


typedef struct{

    char nomeApp[80];
    char idApp[50];
    char avaliacao[3];
    char instalacoes[11];

} REGISTRO;

typedef struct {
    char idApp[50];
    int posicao;
} INDICE_IDAPP;

typedef struct {
    char nomeApp[80];
    int posicao;
} INDICE_NOMEAPP;

typedef struct Node {
    char instalacoes[15];
    struct Node* esquerda;
    struct Node* direita;
    int altura;
    int posicao;
} No;

 struct NodoTrie{
    struct NodoTrie *filhos[TAMANHO_ALFABETO];
    bool isFimDaPalavra;
    int posicao;
};


int getAltura(No* no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

int fatorBalanceamento(No* no) {
    if (no == NULL)
        return 0;
    return getAltura(no->esquerda) - getAltura(no->direita);
}
int max(int a, int b) {
    return (a > b) ? a : b;
}

No* rotacaoDireita(No* y) {
    No* x = y->esquerda;
    No* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = 1 + max(getAltura(y->esquerda), getAltura(y->direita));
    x->altura = 1 + max(getAltura(x->esquerda), getAltura(x->direita));

    return x;
}

No* rotacaoEsquerda(No* x) {
    No* y = x->direita;
    No* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = 1 + max(getAltura(x->esquerda), getAltura(x->direita));
    y->altura = 1 + max(getAltura(y->esquerda), getAltura(y->direita));

    return y;
}

No* inserir(No* raiz, char instalacoes[15], int posicao) {
    if (raiz == NULL) {
        No* novoNo = (No*)malloc(sizeof(No));
        strcpy(novoNo->instalacoes, instalacoes);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        novoNo->altura = 1;
        novoNo->posicao = posicao;
        return novoNo;
    }

    if (strcmp(instalacoes, raiz->instalacoes) < 0)
        raiz->esquerda = inserir(raiz->esquerda, instalacoes, posicao);
    else if (strcmp(instalacoes, raiz->instalacoes) > 0)
        raiz->direita = inserir(raiz->direita, instalacoes, posicao);
    else
        return raiz; // Duplicatas não são permitidas

    raiz->altura = 1 + max(getAltura(raiz->esquerda), getAltura(raiz->direita));

    int balance = fatorBalanceamento(raiz);

    // Casos de rotação
    if (balance > 1) {
        if (strcmp(instalacoes, raiz->esquerda->instalacoes) < 0) {
            return rotacaoDireita(raiz);
        }
    }
    if (balance < -1) {
        if (strcmp(instalacoes, raiz->direita->instalacoes) > 0) {
            return rotacaoEsquerda(raiz);
        }
    }

    return raiz;
}

No* buscar(No* raiz, char instalacoes[15]) {
    if (raiz == NULL || strcmp(instalacoes, raiz->instalacoes) == 0){
            return raiz;
    }

    if (strcmp(instalacoes, raiz->instalacoes) < 0){
       if(raiz->esquerda == NULL){
        printf("Valor nao encontrado\n");
        return NULL;
       }
       else{
            return buscar(raiz->esquerda, instalacoes);
       }

    }
    if(raiz->direita == NULL){
        printf("Valor nao encontrado\n");
        return NULL;
    }
    else{
            return buscar(raiz->direita, instalacoes);
    }

}

void pesquisarNoArquivo(No* resultado){

    REGISTRO registro;

    FILE* arquivoDados = fopen("C:\\Users\\ritch\\Desktop\\Dados.bin", "rb");

    fseek(arquivoDados, resultado->posicao * sizeof(REGISTRO), SEEK_SET);
    fread(&registro, sizeof(REGISTRO), 1, arquivoDados);

    printf("Nome: %s\nNumero de instalacoes: %s\n", registro.nomeApp, registro.instalacoes);

    fclose(arquivoDados);
}


char aleatorio[10] = "┬";
int inserirRegistrosBinarios(FILE *arquivoBinario, REGISTRO *registro, char linha[TAM_MAX_LINHA], FILE *dados) {

    int registros = 0;

    while (fgets(linha, TAM_MAX_LINHA, dados) != NULL) {
        sscanf(linha, "%[^¨]¨%[^¨]¨%[^¨]¨%[^\n]\n", registro->nomeApp, registro->idApp, registro->avaliacao, registro->instalacoes);
        if(strcmp(registro->avaliacao," ") == 0){
            strcpy(registro->avaliacao,"0");
        }

        if(strcmp(registro->instalacoes," ") == 0){
            strcpy(registro->instalacoes,"0");
        }

        fwrite(registro, sizeof(REGISTRO), 1, arquivoBinario);
        registros++;

    }
    return registros;
}

void pesquisaBinaria(char arquivoDadosBinario[50], char id[50], int numeroRegistros) {
    REGISTRO registroPesquisa, registro;
    strcpy(registroPesquisa.idApp,id);
    FILE* dadosBinario = fopen(arquivoDadosBinario, "rb");
    fread(&registro, sizeof(REGISTRO), 1, dadosBinario);
    if (dadosBinario == NULL) {
        printf("Não foi possível abrir o arquivo de dados\n");
        exit(1);
    }

    // Calcula o número total de registros e o tamanho de cada registro
    int tamanhoRegistro = sizeof(REGISTRO);

    // Inicia a busca binária
    int inicio = 0;
    int fim = numeroRegistros - 1;
    int meio;
    int encontrado = 0;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        fseek(dadosBinario, meio*tamanhoRegistro, SEEK_SET);
        fread(&registro, sizeof(REGISTRO), 1, dadosBinario);

        int comparacao = strcmp(registroPesquisa.idApp, registro.idApp);

        printf("Ids comparadas: %s e %s\n", registroPesquisa.idApp, registro.idApp);

        if (comparacao == 0) {
                printf("\n\n\n");
            printf("Nome app: %s\nId app: %s\nAvaliacao: %s\nNumero de instalacoes: %s\n", registro.nomeApp, registro.idApp, registro.avaliacao, registro.instalacoes);
            encontrado = 1;
            break;
        } else if (comparacao < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    if (encontrado == 0) {
        printf("Registro não encontrado.\n");
    }

    fclose(dadosBinario);
}


int criarIndice(char *nomeArquivoDados, char *nomeArquivoIndice, int campo) {
    FILE *arquivoDados = fopen(nomeArquivoDados, "rb");
    FILE *arquivoIndice = fopen(nomeArquivoIndice, "wb");

    if (arquivoDados == NULL || arquivoIndice == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        exit(1);
    }

    REGISTRO registro;
    INDICE_IDAPP indiceIdApp;
    INDICE_NOMEAPP indiceNomeApp;

    int posicao = 0;
    while (fread(&registro, sizeof(REGISTRO), 1, arquivoDados) == 1) {
        if (campo == 1) {
            strcpy(indiceIdApp.idApp, registro.idApp);
            indiceIdApp.posicao = posicao;
            fwrite(&indiceIdApp, sizeof(INDICE_IDAPP), 1, arquivoIndice);
        } else if (campo == 2) {
            strcpy(indiceNomeApp.nomeApp, registro.nomeApp);
            indiceNomeApp.posicao = posicao;
            fwrite(&indiceNomeApp, sizeof(INDICE_NOMEAPP), 1, arquivoIndice);
        }

        posicao++;
    }

    fclose(arquivoDados);
    fclose(arquivoIndice);

    return posicao;
}

void buscarNoIndice(char *nomeArquivoIndice, char *chave, int campo, int numIndices) {
    FILE* arquivoIndice = fopen(nomeArquivoIndice, "rb");
    REGISTRO registro;

    if (arquivoIndice == NULL) {
        printf("Erro ao abrir o arquivo de índice.\n");
        exit(1);
    }

    if (campo == 1) {
        INDICE_IDAPP indiceIdApp;

         int inicio = 0;
        int fim = numIndices;
        while (inicio <= fim) {
            int meio = (inicio + fim) / 2;
            fseek(arquivoIndice, meio * sizeof(INDICE_IDAPP), SEEK_SET);
            fread(&indiceIdApp, sizeof(INDICE_IDAPP), 1, arquivoIndice);

            int comparacao = strcmp(chave, indiceIdApp.idApp);
            if (comparacao == 0) {
                FILE *arquivoDados = fopen("C:\\Users\\ritch\\Desktop\\Dados.bin", "rb");
                if (arquivoDados == NULL) {
                    printf("Erro ao abrir o arquivo de dados.\n");
                    fclose(arquivoIndice);
                    exit(1);
                }

                fseek(arquivoDados, indiceIdApp.posicao * sizeof(REGISTRO), SEEK_SET);
                fread(&registro, sizeof(REGISTRO), 1, arquivoDados);

                // Faça o que desejar com os dados encontrados
                printf("Nome do App: %s\n", registro.nomeApp);
                printf("Id do App: %s\n", registro.idApp);
                printf("Numero de instalacoes: %s\n", registro.instalacoes);


                fclose(arquivoDados);
                fclose(arquivoIndice);
                return;
             } else if (comparacao < 0) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    } else if (campo == 2) {
        INDICE_NOMEAPP indiceNomeApp;
         int inicio = 0;
        int fim = numIndices;
        while (inicio <= fim) {
            int meio = (inicio + fim) / 2;
            fseek(arquivoIndice, meio * sizeof(INDICE_IDAPP), SEEK_SET);
            fread(&indiceNomeApp, sizeof(INDICE_IDAPP), 1, arquivoIndice);

            int comparacao = strcmp(chave, indiceNomeApp.nomeApp);
            if (comparacao == 0) {
                FILE *arquivoDados = fopen("C:\\Users\\ritch\\Desktop\\Dados.bin", "rb");
                if (arquivoDados == NULL) {
                    printf("Erro ao abrir o arquivo de dados.\n");
                    fclose(arquivoIndice);
                    exit(1);
                }

                fseek(arquivoDados, indiceNomeApp.posicao * sizeof(REGISTRO), SEEK_SET);
                fread(&registro, sizeof(REGISTRO), 1, arquivoDados);

                // Faça o que desejar com os dados encontrados
                printf("Nome do App: %s\n", registro.nomeApp);
                printf("Id do App: %s\n", registro.idApp);
                printf("Numero de instalacoes: %s\n", registro.instalacoes);


                fclose(arquivoDados);
                fclose(arquivoIndice);
                return;
             } else if (comparacao < 0) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }

    printf("Registro não encontrado.\n");
    fclose(arquivoIndice);
}

struct NodoTrie *obterNo(void){
    struct NodoTrie *pNo = NULL;

    pNo = (struct NodoTrie *)malloc(sizeof(struct NodoTrie));

    if (pNo)
    {
        int i;

        pNo->isFimDaPalavra = false;

        for (i = 0; i < TAMANHO_ALFABETO; i++)
            pNo->filhos[i] = NULL;
    }

    return pNo;
}

void inserirTrie(struct NodoTrie *raiz, char *chave, int posicao)
{
    int nivel;
    int tamanho = strlen(chave);

    struct NodoTrie *pRastreio = raiz;

    for (nivel = 0; nivel < tamanho; nivel++)
    {
        int indice = CHAR_PARA_INDICE(chave[nivel]);
        if (!pRastreio->filhos[indice])
            pRastreio->filhos[indice] = obterNo();

        pRastreio = pRastreio->filhos[indice];
    }

    pRastreio->isFimDaPalavra = true;
    pRastreio->posicao = posicao;
}

int buscarTrie(struct NodoTrie *raiz, char *chave)
{
    int nivel;
    int tamanho = strlen(chave);
    int indice;
    struct NodoTrie *pRastreio = raiz;
    char palavraEncontrada[256]; // Defina o tamanho conforme necessário

    for (nivel = 0; nivel < tamanho; nivel++)
    {
        indice = CHAR_PARA_INDICE(chave[nivel]);

        if (!pRastreio->filhos[indice]){
            printf("Avaliacao nao encontrada\n");
            return -1;
        }


        palavraEncontrada[nivel] = chave[nivel];
        pRastreio = pRastreio->filhos[indice];
    }

    palavraEncontrada[nivel] = '\0'; // Final da string
    if (pRastreio->isFimDaPalavra) {
        printf("Palavra encontrada: %s\n", palavraEncontrada);
        return pRastreio->posicao;
    } else {
        printf("Avaliacao nao encontrada\n");
        return -1; // A string é um prefixo, não uma palavra completa
    }
}


int main(){

    printf("Carregando...\n");
    FILE* dados = fopen("C:\\Users\\ritch\\Desktop\\DadosTDE.txt", "r");

    if (dados == NULL) {
        printf("Não foi possível abrir o arquivo de filmes\n");
        exit(1);
    }



    FILE* arquivoBinario = fopen("C:\\Users\\ritch\\Desktop\\Dados.bin", "wb");

    if (arquivoBinario == NULL) {
        printf("Não foi possível abrir o arquivo de filmes\n");
        exit(1);
    }


    REGISTRO registro;

    char linha[TAM_MAX_LINHA];

    int numRegistros = 0;
    numRegistros = inserirRegistrosBinarios(arquivoBinario, &registro, linha, dados);

    fclose(dados);
    fclose(arquivoBinario);
    /*************************************************************/
    // Criar índices em arquivos
    int numIndApp, numIndNome;
    numIndApp = criarIndice("C:\\Users\\ritch\\Desktop\\Dados.bin", "C:\\Users\\ritch\\Desktop\\indice_idapp.bin", 1);
    numIndNome = criarIndice("C:\\Users\\ritch\\Desktop\\Dados.bin", "C:\\Users\\ritch\\Desktop\\indice_nomeapp.bin", 2);
    /************************************************************/

    //Criar índices em memória
    No* raiz = NULL;
    FILE* file = fopen("C:\\Users\\ritch\\Desktop\\Dados.bin", "rb");

    if (file == NULL) {
    printf("Erro ao abrir o arquivo de dados\n");
    exit(1);
}

    REGISTRO record;
    int i=0, posicao = 0;
    while (i<92){
    fread(&record, sizeof(REGISTRO), 1, file);
    raiz = inserir(raiz, record.instalacoes, posicao);
    i++;
    posicao++;
    printf("%d\n", i);
    }

    fclose(file);

    //Criar TRIE
    /*struct NodoTrie *raizTrie = obterNo();

    file = fopen("C:\\Users\\ritch\\Desktop\\Dados.bin", "rb");

    if (file == NULL) {
    printf("Erro ao abrir o arquivo de dados\n");
    exit(1);
    }

    i=0, posicao = 0;
    while (i<92){
    fread(&record, sizeof(REGISTRO), 1, file);
    inserirTrie(raizTrie, record.avaliacao, posicao);
    i++;
    posicao++;
    printf("%d\n", i);
    }

    fclose(file);

        */


    int escolha;
    char id[50];
    char idAppBusca[50];
    char nomeAppBusca[80];
    char numInstalacoes[15];
    char strAvaliacao[20];
    int posicaoTrie;

    do {
        printf("Menu de Opcoes:\n");
        printf("1. Consultar dado no arquivo de dados (busca binária)\n");
        printf("2. Buscar por idApp (arquivo de índices)\n");
        printf("3. Buscar por avaliacao\n");
        printf("4. Buscar  por AVL (memória)\n");
        printf("5. Buscar  por tabela HASH (memória)\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);


        switch (escolha) {
            case 1:
                //Age.checker.jp
                printf("Digite uma id para pesquisar (Ex: Age.checker.jp)");
                scanf("%s", id);
                pesquisaBinaria("C:\\Users\\ritch\\Desktop\\Dados.bin", id, numRegistros);
                break;
            case 2:
                //Age.checker.jp
                printf("Digite o id do app:");
                scanf("%s", idAppBusca);
                buscarNoIndice("C:\\Users\\ritch\\Desktop\\indice_idapp.bin", idAppBusca, 1, numIndApp);
                break;
            case 3:
                //Luscher Test
                printf("Digite o nome do app:");
                scanf("%s", nomeAppBusca);
                buscarNoIndice("C:\\Users\\ritch\\Desktop\\indice_nomeapp.bin", nomeAppBusca, 2, numIndNome);
                break;
            case 4:
                printf("Digite o numero de instalacoes no formato '100+'\n");
                scanf("%s", numInstalacoes);
                No* resultado = buscar(raiz, numInstalacoes);

                if (resultado) {
                    pesquisarNoArquivo(resultado);
                } else {
                    printf("Instalacoes nao encontradas.\n");
                }

                break;
            case 5:
                printf("Digite a avaliacao:\n");
                scanf("%s", strAvaliacao);

                //posicaoTrie = buscarTrie(raizTrie, strAvaliacao);
                if(posicaoTrie != -1){
                    printf("Posicao: %d", posicaoTrie);
                }


                break;
            case 6:

                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (escolha != 6);

    return 0;
}

 /********************************************/
    //C:\Users\ritch\Desktop\DadosTDE.txt
    //C:\Users\ritch\Desktop\Dados.bin
    //Age.checker.jp
    //Luscher Test
    //C:\Users\ritch\Desktop\indice_idapp.bin
    //C:\Users\ritch\Desktop\indice_nomeapp.bin
    /********************************************/
