#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JSON_Parser.h"

void reseta_Quantidades(quantidades* q){
    q->QtdObj = 0;
    q->QtdArray = 0;
    q->QtdPair = 0;
    q->QtdStr = 0;
    q->QtdNum = 0;
    q->QtdTru = 0;
    q->QtdFal = 0;
    q->QtdNull = 0;
}

int anaLex(char* linha, int* atual, quantidades* Qtd){

    int prcorre;
    int pos = *atual;
    int valid = 1;
    int tipo = 0;
    char carac;
    sscanf(&linha[pos], " %c%n", &carac, &prcorre);

    /*A condicao de ser numero eh complicada, portanto devo trata-la fora do switch
        NUMBER : int frac ? exp ?
        digit : [ 0 - 9 ]
        sign : ( + | - )
        int: sign ? ( 0 | [ 1 - 9 ] digit *)
        frac : . digit +
        exp: ( e | E ) sign ? digit +
    */
    //Checando a parte inteira
    if (carac == '+' || carac == '-' || carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9'){

        if (carac == '+' || carac == '-'){ // Para ser numero, nao basta ter apenas o sinal
            valid = 0;
            pos += prcorre;
            sscanf(&linha[pos], " %c%n", &carac, &prcorre);
            if (carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9') valid = 1;
        }
        else valid = 1;
        if (valid){  //Se entrar nessa condicao quer dizer que eh um numero INTEIRO VALIDO, falta checar o resto desse numero
            //Checando o resto da parte inteira
            pos += prcorre;
            sscanf(&linha[pos], " %c%n", &carac, &prcorre);
            while (carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9'){
                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &carac, &prcorre);
            }
            //Checando a parte fracionaria (se tiver, deve ser valida)
            // frac : . digit +
            if (carac == '.'){
                valid = 0;
                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                if (carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9') valid = 1;
                if (valid){ //Se entrar nessa condicao quer dizer que eh um numero VALIDO COM PARTE FRACIONARIA, falta checar o resto desse numero
                    //Checando o resto da parte fracionaria
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                    while (carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9'){
                        pos += prcorre;
                        sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                    }
                }
            }
            //Checando a parte exponecial (se tiver, deve ser valido)
            //exp: ( e | E ) sign ? digit +
            if (valid && (carac == 'e' || carac == 'E')){
                valid = 0;
                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                if (carac == '+' || carac == '-'){
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                    if (carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9') valid = 1;
                }
                else if (carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9') valid = 1;
                if (valid){ //Se entrar nessa condicao quer dizer que eh um numero VALIDO COM PARTE EXPONENCIAL, falta checar o resto desse numero
                    //Checando o resto da parte exponencial
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                    while (carac == '0' || carac == '1' || carac == '2' || carac == '3' || carac == '4' || carac == '5' || carac == '6' || carac == '7' || carac == '8' || carac == '9'){
                        pos += prcorre;
                        sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                    }
                }
            }
        }
        if (valid){
            tipo = 2;
            Qtd ->QtdNum++;
        }
    }
    else{
        char aux;
        int checa;
        switch (carac){
            //STRING : " ~ ( \ ? " | \ ( b | f | n | r | t | u | \ ) ? ) * "
            case '"':
                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                if (carac == '"') { //PODE estar errado, afinal isto --> "" eh uma string, mas isto --> """ nao
                        char provi;
                        sscanf(&linha[pos+prcorre], " %c", &provi);
                        if (provi == '"') valid = 0;
                }
                while (valid && carac != '"' ){ //!! PERIGO de um loop infinito ou ERRO, para consertar usa uma variavel de checagem no "sscanf"
                    if (carac == '\\'){//Encontrando uma barra, ppode ser que seja invalido, entao leia o que vem a seguir
                        pos += prcorre;
                        checa = sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                        if (carac == 'b' || carac == 'f' || carac == 'n' || carac == 'r' || carac == 't' || carac == 'u' || carac == '\\' || carac == '"') valid  = 0;
                    }
                    else{
                        pos += prcorre;
                        checa = sscanf(&linha[pos], " %c%n", &carac, &prcorre);
                    }
                    if (checa == EOF) valid = 0;
                }
                pos += prcorre;
                if (valid){
                    tipo = 1;
                    Qtd->QtdStr++;
                }
                break;
            //TRUE : true
            case 't':
                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                if (aux == 'r') {
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                }
                else valid = 0;
                if (valid && aux == 'u') {
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                }
                else valid = 0;
                if (valid && aux == 'e') valid = 1;

                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &aux, &prcorre); // checando se acaba mesmo em 'e' (Ex: true+ nao eh valido)
                if (aux != ',' && aux != '}' && aux != ']' && aux != ')') valid = 0;
                if (valid){
                    tipo = 3;
                    Qtd->QtdTru++;
                }
                break;
            //FALSE : false
            case 'f':
                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                if (aux == 'a'){
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                }
                else valid = 0;
                if (valid && aux == 'l') {
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                }
                else valid = 0;
                if (valid && aux == 's'){
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &aux, &prcorre);

                }
                else valid = 0;
                if (valid && aux == 'e') valid = 1;

                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &aux, &prcorre); // checando se acaba mesmo em 'e' (Ex: false+ nao eh valido)
                if (aux != ',' && aux != '}' && aux != ']' && aux != ')') valid = 0;
                if (valid){
                    tipo = 4;
                    Qtd->QtdFal++;
                }
                break;
            //NULL : null
            case 'n':
                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                if (aux == 'u') {
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                }
                else valid = 0;
                if (valid && aux == 'l') {
                    pos += prcorre;
                    sscanf(&linha[pos], " %c%n", &aux, &prcorre);
                }
                else valid = 0;
                if (valid && aux == 'l') valid = 1;

                pos += prcorre;
                sscanf(&linha[pos], " %c%n", &aux, &prcorre); // checando se acaba mesmo em 'l' (Ex: null+ nao eh valido)
                if (aux != ',' && aux != '}' && aux != ']' && aux != ')') valid = 0;
                if (valid){
                    tipo = 5;
                    Qtd->QtdNull++;
                }
                break;
            default:
                break;
        }
    }

    *atual = pos;
    return tipo;
}

int isElem(char* linha, int* atual, quantidades* Qtd){
    int valid =0;
    int continua;
    char carac;
    //Seguindo a Gramatica
    // Elements : ( , Value ? ) *
    sscanf(&linha[*atual], " %c", &carac);
    valid = isValue(linha, atual, Qtd);
    if(carac == ',' || valid){
        if (!valid) (*atual)++;
        valid = 1;
        sscanf(&linha[*atual], " %c", &carac);
        do {
            if (carac == ',') {
                continua = 1;
                (*atual)++;
                sscanf(&linha[*atual], " %c", &carac);
            }
            else if (isValue(linha, atual, Qtd)) {
                continua = 0;
                sscanf(&linha[*atual], " %c", &carac);
                if (carac == ',') continua = 1;
            }
            else continua = 0;
        }while (continua);
    }

    return valid;
}

int isPair(char* linha, int* atual, quantidades* Qtd){
    int prcorre;
    int valid = 0;
    char carac;
    //Pair : STRING : Value
    if (anaLex(linha, atual, Qtd) == 1){
        sscanf(&linha[*atual], " %c%n", &carac, &prcorre);
        if (carac == ':'){
            (*atual) += prcorre;
            valid = isValue(linha, atual, Qtd);
        }
    }

    if (valid){
        Qtd->QtdPair++;
    }
    return valid;
}

int isMem(char* linha, int* atual, quantidades* Qtd){
    int valid = 0;
    char carac;
    //Members : Pair ( , Pair ) *
    if (isPair(linha, atual, Qtd)){
        valid = 1;
        sscanf(&linha[*atual], " %c", &carac);
        while (carac == ','){
            (*atual)++;
            valid = isPair(linha, atual, Qtd);
            sscanf(&linha[*atual], " %c", &carac);
        }
    }
    //printf("\n\natual = %d\t'%c'\n\n",(*atual), carac);
    return valid;
}

int isObj(char* linha, int* atual, quantidades* Qtd){
    int valid = 0;
    char carac;
    //Se entrei nessa funcao, eh pq linha[0] = '{', entao pos = 1;
    //Object : { Members ? }
    sscanf(&linha[*atual], " %c", &carac);
    if (carac == '}') valid = 1;
    else {
        if (isMem(linha, atual, Qtd)){
                sscanf(&linha[*atual], " %c", &carac);
                //printf("\n\natual = %d\t'%c'\n\n",(*atual), carac);
                if (carac == '}') valid = 1;
        }
    }
    if (valid) (*atual)++;
    return valid;
}

int isArray(char* linha, int* atual, quantidades* Qtd){
    int valid = 0;
    char carac;
    //Se entrei nessa funcao, eh pq linha[0] = '[', entao pos = 1
    // Array : [ Elements ? ]
    sscanf(&linha[*atual],  " %c", &carac);
    if (carac == ']') valid = 1;
    else{
        if(isElem(linha, atual, Qtd)){
            sscanf(&linha[*atual], " %c", &carac);
            //printf("\n\natual = %d\t'%c'\n\n",(*atual), carac);
            if (carac == ']') valid = 1;
        }
    }
    if (valid) (*atual)++;
    return valid;
}

int isValue(char *linha, int* atual, quantidades *Qtd){
    //Seguindo a Gramatica "G: Value EOF"
    //Primeira coisa a verificar, se eh um Valor, se for, qual eh ele
    char c;
    int prcorre;
    int valid;
    sscanf(&linha[*atual], " %c%n", &c, &prcorre);
   // printf("atual = %d\t'%c'\n",(*atual), c);

    switch (c){
        case '{':
            (*atual) += prcorre;
            valid = isObj(linha, atual, Qtd);
            Qtd->QtdObj += valid;
            break;
        case '[':
            (*atual) += prcorre;
            valid = isArray(linha, atual, Qtd);
            Qtd->QtdArray += valid;
            break;
        default:
            valid = anaLex(linha, atual, Qtd);
            if (valid > 0) valid = 1;
            break;
    }
    return valid;
}

