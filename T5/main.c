/*Programa que Simula 4 terminais de bancos, os saques, depositos, transferencias consultas e auditorias juntamente com um Hash para o ID dos bancos

    Giovanni P. Meloni, ICMC, 2017.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bancario.h"

int main(){

        //Declarando Variaveis
    char linha[1000];
    terminal* term;
    float quantia;
    int pos, num, c_banco, c_banco_des, c_terminal;
    int audi = 0;
    int n_banc = 4;
    int n_term = 4;
    int bancos[n_banc];
    char op;

        //Setando o terminal para o inicio do dia
    for(int i=0; i<n_banc; i++) bancos[i] = 0;
    term = (terminal*) malloc(n_term * sizeof(terminal)); ///FREE
    reseta_terminal(&term, n_term);

        //Leitura da entrada
    /*Existem  CASOS a serem tratados:
        1- ATUALIZA RELATORIO, nesse caso basta ler a operacao, suas especificacoes, e por fim mandar para o relatorio. Nesse caso pode acontecer
    de ser a operacao de Transferencia, nesse SUB-CASO, eh necessario mais uma informacao: para qual banco esta sendo transferido a quantia.
        2- AUDITORIA, nesse caso eh necessario imprimir na tela o relatorio da operacao "X", "Y" vezes. Ambas (operacao, e quantidade) sao entradas
    fornecidas pelo usuario.
        3- ERRO, nesse caso quando eh encontrado uma linha de ERRO, a operacao anterior deve ser desconsiderada para o banco e usuario, porem deve
    permanecer no relatorio.
    */

    while (fgets(linha, 1000, stdin) != NULL){
        c_banco_des = -1;
        pos = 0;
        if (sscanf(&linha[pos], "%d%n", &c_terminal, &num)) {
            pos += num;
            if (audi == 0 && (sscanf(&linha[pos], "%d%n", &c_banco, &num))){
                pos += num;
                int aux = hash_func(c_banco);
                if (bancos[aux] == 0) bancos[aux] = c_banco;
                c_banco = aux;
                sscanf(&linha[pos], " %c%n", &op, &num);
                pos += num;
                int pos_aux = pos;
                //Caso a operacao seja de transferencia, pode ser para um banco diferente, portanto preciso pegar o codigo dele.
                if (op == 'T') {
                    sscanf(&linha[pos_aux], "%f%n", &quantia, &num);
                    pos_aux += num;
                    if (((int)strlen(linha)-1) != pos_aux){
                        sscanf(&linha[pos_aux], "%f%n", &quantia, &num);
                        sscanf(&linha[pos], "%d%n", &c_banco_des, &num);
                        int aux = hash_func(c_banco_des);
                        if (bancos[aux] == 0) bancos[aux] = c_banco_des;
                        c_banco_des = aux;
                    }
                    //Se a operacao de transferencia for para o mesmo banco, so precisarei pegar o valor.
                    else {
                        c_banco_des = c_banco;
                        pos += num;
                    }
                }
                else{
                    sscanf(&linha[pos], "%f%n", &quantia, &num);
                    pos += num;
                }
                atualiza_relatorio(&term, c_terminal, c_banco, c_banco_des, op, quantia);
            }
            else {//Caso c_banco nao receba o numero esperado, significa que foi encontrado um char, ou seja, ele quer fazer a auditoria da operacao 'X'. Uma vez que entrar nessa parte do codigo, ficara ate o fim da execucao.
                int qtdade;
                if (audi == 0){
                    processamento_operacoes(&term, bancos, n_term, n_banc);
                    printf("===AUDITORIA===\n");
                    audi = 1;
                }
                sscanf(&linha[pos], " %c%n", &op, &num); //Qual operacao a ser auditada
                pos += num;
                sscanf(&linha[pos], " %d%n", &qtdade, &num);//Quantas devem ser impressas
                pos += num;
                auditoria(&term, bancos, c_terminal, qtdade, op);
            }
        }
        else {//Caso c_terminal nao receba o numero esperado, significa que foi encontrado um char, ou a letra 'E' de "ERRO". Tratamento do ERRO
            tratamento_erro(&term, c_terminal);
        }
    }

    if (audi == 0) processamento_operacoes(&term, bancos, n_term, n_banc);

    return 0;
    free(term);
}
