#ifndef BANCARIO_H_INCLUDED
#define BANCARIO_H_INCLUDED

/*
A cada operacao realizada, o sistema deve registrar: tal operacao,o terminal utilizado, o
banco responsavel pela operacao, e variacoes de saldo (caso haja). Alem disso, ao final do
dia, o sistema deve permitir que o registro de operacoes seja auditado, exibindo o fluxo de
dinheiro (entrada e saıda) de cada terminal, para cada banco atendido.
*/

#define n_bancos 4

typedef struct B{
    int codigo;
    int consulta;
    float saque;
    float deposito;
    float t_perda;
    float t_ganho;
} banco;

//Struct que armazena o relatorio
typedef struct R{
    //Relatorio dos saques
    int banco_s[100]; //Em qual banco fez o saque
    float quantia_s[100]; //Quantia do saque
    int pos_s; // ultima posicao no vetor
    //Relatorio dos depositos
    int banco_d[100]; //Em qual banco fez o deposito
    float quantia_d[100];//Quantia do deposito
    int pos_d;
    //Relatorio das consultas
    int banco_c[100];
    int pos_c;
    //Relatorio das transferencias
    int banco_t_or[100];//De qual banco foi feita a transferencia
    float quantia_t[100];//Quantia de transferenciua
    int banco_t_des[100];//Para qual banco foi feita transferencia
    int pos_t;

    char ult_trans;//Ultima operacao realizada
    int saque_f; //Numero de saques falhos
    int depo_f; //Numero de depositos falhos
    int cons_f; //Numero de consultas falhas
    int trans_f; //Numero de transferencias falhas
} relatorio;

typedef struct T{
    banco b[n_bancos];
    relatorio log;
    float lucro;
} terminal;

/*Funcao que reseta os terminais para comecar o dia
    Parametros:
        -Endereco do vetor de terminais (terminal**)
        -numero de terminais (int)
    Retorno:
*/
void reseta_terminal(terminal** t, int n_term);

/*Funcao que atualiza o relatorio do dia conforme os processos ocorrem
    Parametros:
        -Endereco do vetor de terminais (terminal**)
        -Numero do terminal (int)
        -Codigo do banco(int)
        -Banco de destino(int) (usado para transferencias)
        -Operacao (char)
        -Quantia transitada (float)
    Retorno:
*/
void atualiza_relatorio(terminal** t, int c_terminal, int c_banco, int c_banco_des, char tran, float quantia);

/*Funcao que trata o erro quando é encontrado a linha escrita "ERRO". A operação anterior deve ser desconsiderada para o banco.
Porém deve permanecer no relatorio.
    Parametros:
        -Endereco do vetor de terminais (terminal**)
        -Numero do terminal (int)
    Retorno:
*/
void tratamento_erro(terminal** t, int c_terminal);

/*Funcao chamada no fim do dia, ou seja, quando nao ha mais operacoes a serem feitas
Todas as informacoes do relatorio devem ser passadas para o banco para serem contabilizadas
    Parametros:
        -Endereco do vetor de terminais (terminal**)
        -Hash Table que contem o codigo do banco correspondente ao seu Hash (int*)
        -Numero de terminais(int)
        -Numero de bancos(int)
    Retorno:
*/
void processamento_operacoes(terminal** t, int* h_table, int n_term, int n_banc);

/*Funcao que imprime na tela o relatorio da operação requerida, o usuario seleciona qual operação e quantas delas
devem ser mostradas (em ordem que foram feitas durante o dia)
    Parametros:
        -Endereco do vetor de terminais (terminal**)
        -Hash Table que contem o codigo do banco correspondente ao seu Hash (int*)
        -Numero de terminais(int)
        -Quantas operacoes (int)
        -Qual operacao (char)
*/
void auditoria(terminal** t, int* h_table, int c_terminal, int quantidade, char op);

/*Funcao que transforma o codigo/ID dos bancos em um outro valor.
    Parametros:
        -Numero a ser transformado(int)
    Retorno:
        -Novo numero(int)
*/
int hash_func(int num);

#endif // BANCARIO_H_INCLUDED
