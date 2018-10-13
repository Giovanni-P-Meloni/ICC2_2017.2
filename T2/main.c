/*
Programa que percorre um dado labirinto com segmentos, que ligam pontos e camaras, procurando uma saida e retornando todos os caminhos possiveis
ordenadamente

Obs: A ordenação do meu programa aparenta estar errada segundo o caso 7 no run codes,
o que é estranho pois ele ordena de maneira correta até a metade dos caminhos, depois inverte dois a dois os ultimos 4 caminhos.

Giovanni P. Meloni, ICMC, 2017
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    int tamanho;
    int *indices;
    double somaG;
    double pontuacao;
} resposta;

typedef struct{
    double x;
    double y;
    int temcam; // booleano
    int saida; // booleano
}ponto;

/*Funcao que calcula a distancia de ponto a ponto e retorna um arredondamento para int (e.g: resposta for >= 2,5 o resultado sera 3)
    Parametros:
        - Vetor de pontos (ponto**)
        - Vetor de caminho percorrido(int**)

    Retorno:
        -Distancia (int)
*/
double SomaGeodes(ponto** p, int** indices, int tam){
    double resposta = 0;
    for(int i=0; i<tam-1; i++){
        int ind2 = (*indices)[i+1];
        int ind1 =  (*indices)[i];
        resposta += sqrt(pow(((*p)[ind2 - 1].x - (*p)[ind1 - 1].x), 2) + pow(((*p)[ind2 - 1].y - (*p)[ind1 - 1].y), 2));
    }

    return resposta;
}

/* Funcao que cria os pontos
    Parametros:
        - Vetor de pontos (ponto**)
        - Quantidade de pontos (int)
*/
void CriaPontos(ponto** p, int NP){

    *p = (ponto*) malloc(NP * sizeof(ponto));
    //   - Os pontos
    for(int i=0; i<NP; i++){
        scanf("%lf %lf", &((*p)[i].x), &((*p)[i].y));
        (*p)[i].saida = 0;
        (*p)[i].temcam = 0;
    }
}

/*Funcao que da as caracteristicas do ponto (camaras)
    Parametros:
        - Vetor de pontos (ponto**)
        - Quantidade de camaras (int)
*/
void InicializaPontos(ponto** p, int NC){

    //   - Onde tem camaras
    for(int i=0; i<NC; i++){
        int j, k;
        // Primeiro recebo o indice do ponto que tem a camara, depois se ela tem saida ou nao;
        scanf("%d %d", &j, &k);
        (*p)[j-1].temcam = 1;
        (*p)[j-1].saida = k;
    }
}

/*Funcao que cria uma matriz quadrada relacionado ao numero de pontos
    Parametros:
        - Matriz (int***)
        - Matriz do tesouro (int***)
        - Quantidade de pontos (int)
*/
void CriaLabi(int ***labirinto, int ***labirintonew, int NP){
    *labirintonew = (int**) malloc(NP * sizeof(int*));
    *labirinto = (int**) malloc(NP * sizeof(int*));
    for (int i=0; i < NP; i++){
        (*labirinto)[i] = (int*) calloc(NP, NP * sizeof(int));
        (*labirintonew)[i] = (int*) calloc(NP, NP * sizeof(int));
    }
}

/*Funcao que Inicializa o labirinto, cada posicao representa o segmento (e.g: a posicao (2,3) se refere ao segmento que liga o ponto de indice 3 com o ponto de indice 4)
OBS: a matriz vai de 0 ate NP-1, os indices vao de 1 ate NP
    Parametros:
        - Matriz labirinto (int***)
        - Vetor de pontos (ponto**)
        - Quantidade de segmentos (int)
*/
void InicializaLabi(int ***labirinto,int ***labirintonew, int NS){
    for (int i=0; i < NS; i++){
        //Primeiro recebo o indice do ponto 1, depois o indice a quem o ponto 1 se conecta.
        int indice1, indice2;
        scanf("%d %d", &indice1, &indice2);
        if (indice1 >= 0 && indice2 >= 0){
            (*labirinto)[indice1 - 1][indice2 - 1] = 1;//tanto a ida
            (*labirinto)[indice2 - 1][indice1 - 1] = 1;//quanto a volta
            (*labirintonew)[indice1 - 1][indice2 - 1] = 1;//tanto a ida
            (*labirintonew)[indice2 - 1][indice1 - 1] = 1;//quanto a volta
        }
    }
}

/*Funcao que cria um novo labirinto para ser substituido pelo labirinto original quando o jogador alcancar a camara do tesouro
    Parametros:
        - Matriz (***int)
        - Vetor de pontos(ponto**)
        - Quantidade de acoes (int)
*/
void NovoLabi(int ***labirinto, int NA){
    for(int i =0; i < NA; i++){
        int indice1, indice2;
        scanf("%d %d", &indice1, &indice2);
        if (indice1 >= 0 && indice2 >= 0){
            (*labirinto)[indice1 - 1][indice2 - 1] = 1;//tanto a ida
            (*labirinto)[indice2 - 1][indice1 - 1] = 1;//quanto a volta
        }
        else{
            if (indice1<0) indice1 = (-1)*indice1;
            if (indice2<0) indice2 = (-1)*indice2;
            (*labirinto)[indice1 - 1][indice2 - 1] = 0;//tanto a ida
            (*labirinto)[indice2 - 1][indice1 - 1] = 0;//quanto a volta
        }
    }
}

/*Funcao que ordena as respostas segundo os requerimentos
    Parametros:
        -Vetor de struct resposta (resposta**)
        -Quantidade de diferentes respostas(int)
*/
void OrdenaResposta(resposta **respostas, int num_de_r){
    int e;
    do{
        e=0;
        for(int i =0;i<num_de_r-1;i++){
            if ((*respostas)[i].pontuacao < (*respostas)[i+1].pontuacao){
                resposta aux = (*respostas)[i];
                (*respostas)[i] = (*respostas)[i+1];
                (*respostas)[i+1] = aux;
                e = 1;
            }
            else if((*respostas)[i].pontuacao == (*respostas)[i+1].pontuacao){
                if ((*respostas)[i].somaG > (*respostas)[i+1].somaG){
                    resposta aux = (*respostas)[i];
                    (*respostas)[i] = (*respostas)[i+1];
                    (*respostas)[i+1] = aux;
                    e = 1;
                }
                else if((*respostas)[i].somaG == (*respostas)[i+1].somaG){
                    if((*respostas)[i].tamanho > (*respostas)[i+1].tamanho){
                        resposta aux = (*respostas)[i];
                        (*respostas)[i] = (*respostas)[i+1];
                        (*respostas)[i+1] = aux;
                        e = 1;
                    }
                    else if((*respostas)[i].tamanho == (*respostas)[i+1].tamanho){
                        int tam = (*respostas)[i].tamanho;
                        int foi=0;
                        for(int j=0; j<tam && !foi;j++){
                            if ((*respostas)[i].indices[j] > (*respostas)[i+1].indices[j]){
                                 resposta aux = (*respostas)[i];
                                (*respostas)[i] = (*respostas)[i+1];
                                (*respostas)[i+1] = aux;
                                e = 1;
                                foi = 1;
                            }
                        }
                    }
                }
            }
        }
        num_de_r--;
    }while (e);
}

/*Funcao que desaloca a Heap
    Parametros:
        -Matriz Labirinto(int***)
        -Matriz LabirintoNovo(int***)
        -Vetor de pontos(ponto**)
        -Vetor de caminho percorrido(int**)
        -Vetor de respostas(resposta**)
        -Quantidade de pontos(int)
        -Quantidade de respostas(int)
*/
void DesalocaMem(int ***labirinto1, int *** labirinto2, ponto** pontos, int** caminho, resposta **resposta, int NP, int solucoes){
    for(int i=0; i<NP; i++){
        free((*labirinto1)[i]);
        (*labirinto1)[i] = NULL;
        free((*labirinto2)[i]);
        (*labirinto2)[i] = NULL;
    }
    for(int i=0; i<solucoes;i++){
        free((*resposta)[i].indices);
    }
    free(*labirinto1);
    *labirinto1 = NULL;
    free(*labirinto2);
    *labirinto2 = NULL;
    free(*pontos);
    *pontos = NULL;
    free(*caminho);
    *caminho = NULL;
    free(*resposta);
    *resposta = NULL;
}

int main(){

    // Declarando variaveis
    int **labirinto;
    int **novolabirinto;
    int ***lab;
    ponto *pontos = NULL;
    resposta *respostas = NULL;
    int *caminho = NULL;


    int NP, NC, NS, NA, T, indice_tesouro, inicio, pos, prox;

    // Inicializando variaveis:
   // printf("Quantos Pontos:\n");
    scanf("%d", &NP);
    CriaPontos(&pontos, NP);

   // printf("Quantas Camaras:\n");
    scanf("%d", &NC);
    InicializaPontos(&pontos, NC);

    //printf("Quantos seg:\n");
    scanf("%d", &NS);
    CriaLabi(&labirinto, &novolabirinto, NP);
    InicializaLabi(&labirinto, &novolabirinto, NS);

    scanf("%d %d", &indice_tesouro, &T);
    scanf("%d", &NA);
    NovoLabi(&novolabirinto, NA);

    //Comeco do caminho
   // printf("Qual o inicio:\n");
    scanf("%d", &inicio);
    int tam = 1;
    int novo_tam = 0;
    double pontuacao = 0;
    int solucoes = 0;
    int tem_tes = 0;//variavel que indica em qual mapa estou
    int retorno = 0;
    pos = inicio;
    lab = &labirinto;
    caminho = (int*) malloc(tam*sizeof(int));
    caminho[0] = pos;

    for(int i=0; i<NP; i++){
        if (labirinto[indice_tesouro-1][i] == 1) labirinto[indice_tesouro-1][i] = 0;
        if (novolabirinto[i][indice_tesouro-1] == 1) novolabirinto[i][indice_tesouro-1] = 0;
    }

    // Lembrando: prox e pos sao INDICES, indices nao podem ser 0;
    // segmento com valor 0 na matriz eh IMUTAVEL
    do{
        //  1*- Faz operacoes dependendo se for retorno ou ida
        if(!retorno){ // sendo uma ida, checar normalmente
            if (pos == indice_tesouro){//se for uma cam do tesouro:
                lab = &novolabirinto;//mude o mapa
                pontuacao += T; //pegue o tesouro
                tem_tes = 1;
                novo_tam = tam;
            }
            if(pontos[pos-1].saida){//se for saida, adiciona ao numero de solucoes
                solucoes++;
                respostas = (resposta*) realloc(respostas, solucoes*sizeof(resposta));
                respostas[solucoes-1].tamanho = tam;
                respostas[solucoes-1].indices = malloc(tam*sizeof(int));
                for(int i=0; i<tam; i++){
                    respostas[solucoes-1].indices[i] = caminho[i];
                }
                double sumG = SomaGeodes(&pontos, &caminho, tam);
                respostas[solucoes-1].somaG = sumG;
                respostas[solucoes-1].pontuacao = pontuacao - sumG;
            }
        }
        else{ // esta em um retorno, checa "de maneira inversa"
            if(pos == indice_tesouro){ // Se passa pela camara do tesouro
                int eh_posvl = 0;
                for(int i =0; i<NP && !eh_posvl; i++){ // checa para ver se ter outros caminhos possiveis para ser percorrido com o tesouro
                    if((*lab)[pos-1][i] == 1) eh_posvl = 1;
                }
                if(!eh_posvl){ //nao tendo outro caminho possivel
                    for(int i=0; i<NP; i++){
                        if((*lab)[pos-1][i] == 2) (*lab)[pos-1][i] = 1;
                    }
                    lab = &labirinto;// volte o mapa
                    pontuacao -= T;//retira o tesouro
                    tem_tes = 0;
                }
            }
        }
        //  2*- Procura qual a proxima camara
        //Procuro um caminho valido, se o achar, prox vai ser a camara a qual esse caminho liga.
        //Porem a camara pode ja ter sido visitada, se for, entao prox volta a ser 0 (nao tem caminho).
        //Se prox ainda for 0 procuro outro caminho. (continuo o laco)
        prox = 0;
        for(int i=0; i<NP && !prox; i++){
            if ((*lab)[pos-1][i] == 1) {
                prox = i+1;
                int n = 1;
                if(tem_tes) n = novo_tam;
                for(int j = n; j<tam; j++){
                    if(caminho[j] == prox) prox = 0;
                }
            }
        }

        //  3*- Faz operacoes dependendo se houver um lugar para ir.
        if (prox > 0){ // significa que ha uma posicao valida.
            retorno = 0;
            // abrir espaco pra nova posicao
            tam++;
            caminho = (int*) realloc(caminho, tam*sizeof(int));
            caminho[tam-1] = prox;
            //segmento foi percorrido
            if((*lab)[pos-1][prox-1] != 0)(*lab)[pos-1][prox-1] = 2;
            if((*lab)[prox-1][pos-1] != 0)(*lab)[prox-1][pos-1] = 2;
        }
        else{ //significa que nao ha caminhos novos, devo comecar a voltar
            retorno = 1;
            if ((tam-2) < 0) prox = inicio;
            else{
                prox = caminho[tam-2];
                //retiro uma posicao
                tam--;
                caminho = (int*) realloc(caminho, tam*sizeof(int));
                //segmento foi retornado
                for(int i=0; i<NP; i++){
                    if((*lab)[pos-1][i]==2) (*lab)[pos-1][i] = 1;
                }
            }
        }

        pos = prox;
    }while (tem_tes || prox!=inicio);

    OrdenaResposta(&respostas, solucoes);
    //Arredondamento
    for(int i=0; i<solucoes; i++){
        respostas[i].somaG = round(respostas[i].somaG);
        respostas[i].pontuacao = round(respostas[i].pontuacao);
    }

    printf("%d %d\n", indice_tesouro, T);
    for(int i =0; i<solucoes; i++){
        printf("%d", respostas[i].tamanho);
        for(int j=0; j<respostas[i].tamanho; j++){
            printf(" %d", respostas[i].indices[j]);
        }
        printf(" %d", (int)respostas[i].somaG);
        printf(" %d\n", (int)respostas[i].pontuacao);
    }

    DesalocaMem(&labirinto, &novolabirinto, &pontos, &caminho, &respostas, NP, solucoes);
    return 0;
}
