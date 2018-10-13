/*Programa que, a partir de uma arvore, verifica se houve ou nao colisao entre dois objetos (objeto colisor sendo um ponto)

        Giovanni P. Meloni, ICMC, 2017.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	double x;
	double y;
	double z;
} coordenada;

typedef struct{
	int size;
 	coordenada* root; // a raiz, isto eh, o "primeiro octante"(a caixa delimitadora).
} tree;

/* Funcao que checha as coordenadas dos vertices dos triangulos, e ve quais dessas sao as maiores e menores.
    Parametros:
        - Coordenada de um dos vertices de um triangulo.
        - endereco da struct que guarda o vertice "mais distante" V (Xmax, Ymax, Zmax)
        - endereco da struct que guarda o vertice "mais proximo" Vo (Xmin, Ymin, Zmin)
    Retorno:
        - void
*/
void MaxMinSemCaixa(coordenada** triangulos, coordenada* max, coordenada* min, int ntriang){
    //Primeiro, checar o eixo x
    double compmax = triangulos[0][0].x;
    double compmin = triangulos[0][0].x;
    for(int i=0; i<ntriang; i++){
        for(int j=0; j<3; j++){
            if (triangulos[i][j].x > compmax) compmax = triangulos[i][j].x;
            if (triangulos[i][j].x < compmin) compmin = triangulos[i][j].x;
        }
    }
    //Retornando os valores
    (*max).x = compmax;
    (*min).x = compmin;

    //Depois, checar o eixo y
    compmax = triangulos[0][0].y;
    compmin = triangulos[0][0].y;
    for(int i=0; i<ntriang; i++){
        for(int j=0; j<3; j++){
            if (triangulos[i][j].y > compmax) compmax = triangulos[i][j].y;
            if (triangulos[i][j].y < compmin) compmin = triangulos[i][j].y;
        }
    }
    //Retornando os valores
    (*max).y = compmax;
    (*min).y = compmin;

    //Por ultimo, checar o eixo z
    compmax = triangulos[0][0].z;
    compmin = triangulos[0][0].z;
    for(int i=0; i<ntriang; i++){
        for(int j=0; j<3; j++){
            if (triangulos[i][j].z > compmax) compmax = triangulos[i][j].z;
            if (triangulos[i][j].z < compmin) compmin = triangulos[i][j].z;
        }
    }
    //Retornando os valores
    (*max).z = compmax;
    (*min).z = compmin;
}

/*Funcao que recebe "n" vertices, e calcula qual o maior e menor
    Parametros:
        - O vetor de struct que contem os vertices. (Lembre-se: cada vertice eh uma coordenada, ou seja, struct com 3 valores do tipo double)
        - endereco da struct que guarda o vertice "mais distante" V (Xmax, Ymax, Zmax)
        - endereco da struct que guarda o vertice "mais proximo" Vo (Xmin, Ymin, Zmin)
        - O numero de vertices
    Retorno:
        - void
*/
void MaxMinDosVertices(coordenada* vertice, coordenada* max, coordenada* min, int vertices){
    double compmax;
    double compmin;

    //Primeiro, checar o eixo x
    compmax = vertice[0].x;
    compmin = vertice[0].x;

    for(int i=1; i<vertices; i++){
        if (vertice[i].x > compmax) compmax = vertice[i].x;
        if (vertice[i].x < compmin) compmin = vertice[i].x;
    }
    // Retornando os valores
    (*max).x = compmax;
    (*min).x = compmin;


    //Depois, checar o eixo y
    compmax = vertice[0].y;
    compmin = vertice[0].y;

    for(int i=1; i<vertices; i++){
        if (vertice[i].y > compmax) compmax = vertice[i].y;
        if (vertice[i].y < compmin) compmin = vertice[i].y;
    }
    //retornando os valores
    (*max).y = compmax;
    (*min).y = compmin;


    //Por Ultimo, checar o eixo z
    compmax = vertice[0].z;
    compmin = vertice[0].z;

    for(int i=1; i<vertices; i++){
        if (vertice[i].z > compmax) compmax = vertice[i].z;
        if (vertice[i].z < compmin) compmin = vertice[i].z;
    }
    // Retornando os valores
    (*max).z = compmax;
    (*min).z = compmin;
}

/*Funcao que checa se o projetil esta na caixa delimitadora, uma vez dentro nao importa quantas vezes divido este mesmo quadrado (recursao) ele sempre estara la.
    Parametros:
        - Coordenada do projetil
        - Vertice de max
        - Vertice de min
    Retorno:
        - 1 Se projetil estiver na caixa
        - 0 Caso contrario
*/
int ProjetilECaixa(coordenada projetil, coordenada max, coordenada min){
    if(projetil.x >= min.x && projetil.x <= max.x){
        if(projetil.y >= min.y && projetil.y <= max.y){
            if(projetil.z >= min.z && projetil.z <= max.z) return 1;
        }
    }
    return 0;
}

/* Funcao que calcula a distancia de ponto a plano (plano do triangulo fornecido) e compara com a distancia da diagonal do octante
    Parametros:
        - Vetor de coordenadas que formam o triangulo
        - Coordenada com o maior vertice do octante
        - Coordenada com o menor vertice do octante
    Retorno:
        - 1 Se a distancia de ponto a plano for menor do que a diagonal
        - 0 Caso contrario
*/
int MalhaNoOctante(coordenada* vertice, coordenada min, coordenada max){
    // Calculando o centro do octante em questao
    coordenada centro;
    centro.x = min.x + (max.x - min.x)/2;
    centro.y = min.y + (max.y - min.y)/2;
    centro.z = min.z + (max.z - min.z)/2;

    coordenada pontoP;
    coordenada pontoQ;
    coordenada pontoR;
    pontoP = vertice[0];
    pontoQ = vertice[1];
    pontoR = vertice[2];

    coordenada vetorU;
    vetorU.x = pontoQ.x - pontoP.x;
    vetorU.y = pontoQ.y - pontoP.y;
    vetorU.z = pontoQ.z - pontoP.z;

    coordenada vetorV;
    vetorV.x = pontoR.x - pontoP.x;
    vetorV.y = pontoR.y - pontoP.y;
    vetorV.z = pontoR.z - pontoP.z;

    coordenada vetorCentro;
    vetorCentro.x = centro.x - pontoP.x;
    vetorCentro.y = centro.y - pontoP.y;
    vetorCentro.z = centro.z - pontoP.z;

    coordenada normal;
    normal.x = (vetorU.y*vetorV.z) - (vetorV.y*vetorU.z);
    normal.y = (vetorU.z*vetorV.x) - (vetorV.z*vetorU.x);
    normal.z = (vetorU.x*vetorV.y) - (vetorV.x*vetorU.y);

    // "partecima" e "partebaixo" se referem as partes da formula de "Distancia de ponto a plano" sendo a "partecima" o dividendo e
    // a "partebaixo" o divisor.
    double partecima = (double)((vetorCentro.x*normal.x) + (vetorCentro.y*normal.y) + (vetorCentro.z*normal.z));
    if (partecima < 0) partecima =  partecima*(-1); // necessito do modulo da partecima
    double partebaixo = sqrt((normal.x*normal.x) + (normal.y*normal.y) + (normal.z*normal.z));
    double projecaoort = partecima/partebaixo; // <- essa eh a distancia de Ponto a Plano.

    double diagonal = sqrt(pow(max.x - centro.x,2) + pow(max.y - centro.y, 2)+ pow(max.z - centro.z, 2));
    printf("diagonal: %.8lf, distanciaPaP: %lf\n", diagonal, projecaoort);
    if (projecaoort <= diagonal) return 1;
   return 0;
}

/* Funcao recursiva que divide a caixa delimitadora "n" vezes, uma vez atinge esse numero n de repeticoes verifica se o objeto esta naquele octante.
    Parametros:
        - Coordenada do projetil
        - Vertice de Maximo
        - Vertice de Minimo
        - Numero de iteracoes
        - Contador (para saber qual iteracao esta)

    Retorno:
        - 1 Se o objeto estive no octante
        - 0 Caso contrario
*/
int ObjetoECaixa(coordenada projetil, coordenada max, coordenada min, int iteracoes, int count, coordenada** triangulo, int ntriang){
    count++;
    if(count <= iteracoes) {

    coordenada centro;
    centro.x = min.x + (max.x - min.x)/2;
    centro.y = min.y + (max.y - min.y)/2;
    centro.z = min.z + (max.z - min.z)/2;

    if (projetil.x >= centro.x){
        min.x = centro.x;
        if(projetil.y >= centro.y){
            min.y = centro.y;
            if(projetil.z >= centro.z) min.z = centro.z;
            else max.z = centro.z;
        }
        else {
            max.y = centro.y;
            if (projetil.z >= centro.z) min.z = centro.z;
            else max.z = centro.z;
        }
    }
    else {
        max.x = centro.x;
        if(projetil.y >= centro.y){
            min.y = centro.y;
            if(projetil.z >= centro.z) min.z = centro.z;
            else max.z = centro.z;
        }
        else {
            max.y = centro.y;
            if(projetil.z >= centro.z) min.z = centro.z;
            else max.z = centro.z;
        }
    }
   printf("\noctante %d:  max: (%lf,%lf,%lf), min:(%lf,%lf,%lf)\n", count, max.x,max.y,max.z,min.x,min.y,min.z);
    int colisao = 0;
    for(int i=0; i<ntriang && !colisao; i++){
            colisao = MalhaNoOctante(triangulo[i], min, max);

    }
        if(colisao) return ObjetoECaixa(projetil, max, min, iteracoes, count, triangulo, ntriang);
        else return 0;
    }
    return 1;
}

/* Funcao que libera a memoria
    Parametros:
        - Endereco dos ponteiros
        - numero de triangulos alocados
        - OBS: So preciso liberar o "*root" se a caixa foi passado no comeco do codigo, entao verificarei.
    Retorno:
        - void
*/
void LiberaMemoria(coordenada*** triangulos, coordenada** vertices, int numdetri, int verific){
    for(int i=0; i<numdetri; i++){
        free((*triangulos)[i]);
        (*triangulos)[i] = NULL;
    }
    free(*triangulos);
    *triangulos = NULL;
    if (!verific){
        free(*vertices);
        *vertices = NULL;
    }
}

int main(){

	// Declarando variaveis
	int caixa, ntriang, colisao;
	coordenada projetil;
	coordenada max, min; // pontos de max e min do objeto alvo
	coordenada** triangulo = NULL; // Uma matriz que guarda cada triangulo, e cada triangulo possui 3 coordenadas
	tree octree; // possui as informacoes da arvore

	// Inicializando variaveis
	colisao = 0;
	max.x = 0;
	max.y = 0;
	max.z = 0;
	min.x = 0;
	min.y = 0;
	min.z = 0;

    // Recebendo dados
	scanf("%d", &caixa); // caso seja necessario implementar a caixa ou nao;
	scanf("%d", &octree.size); // até qual nodo deve verificar.
	scanf("%d", &ntriang); // quantidade de triangulos que descrevem o objeto.
	scanf("%lf %lf %lf", &projetil.x, &projetil.y, &projetil.z); // coordenada do projetil

	/*  - O usuario passa "n" triangulos (vetor de triangulos)      | Matriz de struct
        - Cada triangulo possui 3 coordenadas (vetor de coordenadas)| Vetor de uma struct
        - Cada coordenada possui 3 numeros reais (vetor de double)  | Coordenada eh uma struct	*/
	triangulo = (coordenada**) malloc(ntriang * sizeof(coordenada*)); // alocando espaco para os triangulos, (os ponteiros)
	for (int i=0; i<ntriang; i++){
		triangulo[i] = (coordenada*) malloc(3*sizeof(coordenada)); // alocando espaco para as 3 coordenadas que definem um triangulo
        for(int j=0; j<3; j++){
            scanf("%lf %lf %lf", &triangulo[i][j].x, &triangulo[i][j].y, &triangulo[i][j].z); // uma das coordenadas do triangulo
        }
	}
	// Verificar se o programa deve implementar a caixa (ou seja, se o usuario nao passou os vertices da caixa, apenas os triangulos)
    if (caixa){
        MaxMinSemCaixa(triangulo, &max, &min, ntriang);
    }
    // Recebo a caixa, portanto so preciso dos vertices desta.
    else {
        octree.root = (coordenada*) malloc(8*(sizeof(coordenada)));
        for(int i=0; i<8; i++){
            scanf("%lf %lf %lf", &octree.root[i].x, &octree.root[i].y, &octree.root[i].z);
         }
         MaxMinDosVertices(octree.root, &max, &min, 8);

    }
    /*      Neste ponto do codigo, possuo os pontos de maximo e minimo da caixa, independente se o usuario as forneceu ou nao. Apenas possuindo os pontos de maximo e minimo sou capaz de
        checar se o projetil esta na caixa, e contruir/achar os vertices da caixa caso precise. */
     printf("max da caixa: (%lf,%lf,%lf)\nmin da caixa: (%lf,%lf,%lf)\n", max.x, max.y, max.z, min.x, min.y, min.z);
    colisao = ProjetilECaixa(projetil, max, min);
    printf("coordenadas do projetil: (%lf,%lf,%lf)\n", projetil.x, projetil.y, projetil.z);
    printf("Se o projetil esta na caixa: %d\n", colisao);

    if (colisao) colisao = ObjetoECaixa(projetil, max, min, octree.size, 0, triangulo, ntriang); // Se o projetil estiver na caixa delimitadora, continue
    else {                                                                   // Caso contrario imprima "0", Libere memoria, e finalize o programa
        printf("0\n");
        LiberaMemoria(&triangulo, &(octree.root), ntriang, caixa);
        return 0;
    }
    if (colisao) printf("1\n"); // Se o objeto estiver no octante imprima "1" na tela
    else printf("0\n");         // Caso contrario imprima "0" na tela
    LiberaMemoria(&triangulo, &(octree.root), ntriang, caixa);
    return 0;
}
