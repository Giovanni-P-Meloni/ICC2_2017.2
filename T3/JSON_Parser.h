#ifndef JSON_PARSER_H_INCLUDED
#define JSON_PARSER_H_INCLUDED

typedef struct{
    int QtdObj;
    int QtdArray;
    int QtdPair;
    int QtdStr;
    int QtdNum;
    int QtdTru;
    int QtdFal;
    int QtdNull;
} quantidades;


/*Funcao que reseta a quantidade de vezes que aparecem os terminais ou nao terminais;
    Parametros:
        -Endereco da struct quantidades (quantidades*)
*/
void reseta_Quantidades(quantidades* q);

/*Funcao que verifica a validade Lexica da expressao, segundo a gramatica e produz os tokens
    Parametros:
        -linha da expressao (string)
        -Posicao de onde comeco a verificar (int)
        |
        |-> Comeco a verificar da posicao ATUAL
    Retorno(int):
        0- Se for invalido
        1- Se for String
        2- Se for Number
        3- Se for True
        4- Se for False
        5- Se for Null
*/
int anaLex(char* linha, int* atual, quantidades* Qtd);


/*Funcao responsavel por fazer a analise sintatica do ELEMENTO || Elements : ( , Value ? ) *
    Parametros:
        -string da expressao(char*)
        -variavel que guarda a posicao na string(int*)
        -Struct que conta o numero de vezes que aparecem os tokens e os nao terminais
*/
int isElem(char* linha, int* atual, quantidades* Qtd);


/*Funcao responsavel por fazer a analise sintatica do PAIR || Pair : STRING : Value
    Parametros:
        -string da expressao(char*)
        -variavel que guarda a posicao na string(int*)
        -Struct que conta o numero de vezes que aparecem os tokens e os nao terminais
*/
int isPair(char* linha, int* atual, quantidades* Qtd);


/*Funcao responsavel por fazer a analise sintatica do MEMBRO ||  Members : Pair ( , Pair ) *
    Parametros:
        -string da expressao(char*)
        -variavel que guarda a posicao na string(int*)
        -Struct que conta o numero de vezes que aparecem os tokens e os nao terminais
*/
int isMem(char* linha, int* atual, quantidades* Qtd);


/*Funcao que eh chamada caso encontre um abre chaves ("{"), entao verifica se eh um Objeto, segundo a gramatica "Object : { Members ? }"
    Paramatros:
        -linha da expressao (string)
        -posicao de onde comeco a verificar (int)
        |
        |-> verifico APOS o abre chaves
    Retorno (int):
        - 1 Se for um objeto
        - 0 Caso contrario
*/
int isObj(char* linha, int* atual, quantidades* Qtd);


/*Funcao que eh chamada caso encontre um abre colchetes ("["), entao verifica se eh um Array, segundo a gramatica "Array : [ Elements ? ]"
    Paramatros:
        -linha da expressao (string)
        -posicao de onde comeco a verificar (int)
        |
        |-> verifico APOS o abre colchetes
    Retorno (int):
        - 1 Se for um Array
        - 0 Caso contrario
*/
int isArray(char* linha, int* atual, quantidades* Qtd);


/*Funcao responsavel por fazer a analise sintatica do VALUE || Value : Object | Array | STRING | INTEGER | REAL | TRUE | FALSE | NULL
    Parametros:
        -string da expressao(char*)
        -variavel que guarda a posicao na string(int*)
        -Struct que conta o numero de vezes que aparecem os tokens e os nao terminais
*/
int isValue(char *linha, int* atual, quantidades *Qtd);

#endif // JSON_PARSER_H_INCLUDED
