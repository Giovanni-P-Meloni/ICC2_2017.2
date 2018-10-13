/*Programa que Simula um Parser de JSON, porem com apenas as duas etapas inicias: Analise Lexica e Analise Sintatica

    Giovanni P. Meloni, ICMC, 2017.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JSON_Parser.h"


int main(){

    char linha[100000];
    int valid = 1;
    int line = 0;
    int pos;
    quantidades Qtd;
    reseta_Quantidades(&Qtd);

    while(valid && (fgets(linha, 100000, stdin)) != NULL){
        pos = 0;
        line++;
        valid = isValue(linha, &pos, &Qtd);
    }

    if (!valid) fprintf (stdout, "Error line %d\n", line);
    fprintf (stdout, "Number of Objects: %d\n" , Qtd.QtdObj);
    fprintf (stdout, "Number of Arrays: %d\n" , Qtd.QtdArray);
    fprintf (stdout, "Number of Pairs: %d\n" , Qtd.QtdPair);
    fprintf (stdout, "Number of Strings: %d\n" , Qtd.QtdStr);
    fprintf (stdout, "Number of Numbers: %d\n" , Qtd.QtdNum);
    fprintf (stdout, "Number of Trues: %d\n" , Qtd.QtdTru);
    fprintf (stdout, "Number of Falses: %d\n" , Qtd.QtdFal);
    fprintf (stdout, "Number of Nulls: %d\n" , Qtd.QtdNull);
    return 0;
}
