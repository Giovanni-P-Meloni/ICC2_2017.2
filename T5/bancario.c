#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bancario.h"


void reseta_terminal(terminal** t, int n_term){
    for (int i=0; i < n_term; i++){
        for (int j=0; j<n_bancos; j++){
            (*t)[i].b[j].deposito = 0;
            (*t)[i].b[j].saque = 0;
            (*t)[i].b[j].t_ganho = 0;
            (*t)[i].b[j].t_perda = 0;
        }
        (*t)[i].log.pos_s = -1;
        (*t)[i].log.pos_d = -1;
        (*t)[i].log.pos_t = -1;
        (*t)[i].log.pos_c = -1;
        (*t)[i].log.ult_trans = ' ';
        (*t)[i].lucro = 0;
        (*t)[i].log.saque_f = 0;
        (*t)[i].log.depo_f = 0;
        (*t)[i].log.cons_f = 0;
        (*t)[i].log.trans_f = 0;
    }
}

void atualiza_relatorio(terminal** t, int c_terminal, int c_banco, int c_banco_des, char tran, float quantia){
    int pos_s, pos_d, pos_c, pos_t;
    switch (tran){
        case 'S':
            pos_s = ++((*t)[c_terminal-1].log.pos_s);
            (*t)[c_terminal-1].log.banco_s[pos_s] = c_banco; //Lembrando, o que esta sendo guardado eh o HASH do banco
            (*t)[c_terminal-1].log.quantia_s[pos_s] = quantia;
            break;
        case 'D':
            pos_d = ++((*t)[c_terminal-1].log.pos_d);
            (*t)[c_terminal-1].log.banco_d[pos_d] = c_banco; //Lembrando, o que esta sendo guardado eh o HASH do banco
            (*t)[c_terminal-1].log.quantia_d[pos_d] = quantia;
            break;
        case 'C':
            pos_c = ++((*t)[c_terminal-1].log.pos_c);
            (*t)[c_terminal-1].log.banco_c[pos_c] = c_banco; //Lembrando, o que esta sendo guardado eh o HASH do banco
            break;
        case 'T':
            pos_t = ++((*t)[c_terminal-1].log.pos_t);
            (*t)[c_terminal-1].log.banco_t_or[pos_t] = c_banco; //Lembrando, o que esta sendo guardado eh o HASH do banco
            (*t)[c_terminal-1].log.quantia_t[pos_t] = quantia;
            (*t)[c_terminal-1].log.banco_t_des[pos_t] = c_banco_des;
            break;
        default:
            break;
    }
    (*t)[c_terminal-1].log.ult_trans = tran;
}

void tratamento_erro(terminal** t, int c_terminal){
        char error = (*t)[c_terminal-1].log.ult_trans;

        switch(error){
            case 'S':
                (*t)[c_terminal-1].log.pos_s--;
                (*t)[c_terminal-1].log.saque_f++;
                break;
            case 'D':
                (*t)[c_terminal-1].log.pos_d--;
                (*t)[c_terminal-1].log.depo_f++;
                break;
            case 'C':
                (*t)[c_terminal-1].log.pos_c--;
                (*t)[c_terminal-1].log.cons_f++;
                break;
            case 'T':
                (*t)[c_terminal-1].log.pos_t--;
                (*t)[c_terminal-1].log.trans_f++;
                break;
            default:
                break;
        }
}

void processamento_operacoes(terminal** t, int* h_table, int n_term, int n_banc){
    float total_s[4] = {0};
    float total_d[4] = {0};
    int total_c[4] = {0};
    float total_t_ganho[4] = {0};
    float total_t_perda[4] = {0};
    float total_lucro = 0;
    int t_erros[5] = {0};// 0 total, 1 saque, 2 dep, 3 cons, 4 trans
    int erros[5] = {0}; // // 0 total, 1 saque, 2 dep, 3 cons, 4 trans

    for(int i=0; i<n_term; i++){
        for (int k=0; k<= (*t)[i].log.pos_s; k++){
            (*t)[i].b[(*t)[i].log.banco_s[k]].saque += (*t)[i].log.quantia_s[k];
            (*t)[i].lucro += 3;
        }

        for (int k=0; k<= (*t)[i].log.pos_d; k++){
            (*t)[i].b[(*t)[i].log.banco_d[k]].deposito += (*t)[i].log.quantia_d[k];
            (*t)[i].lucro += 3;
        }

        for (int k=0; k<= (*t)[i].log.pos_c; k++){
            (*t)[i].b[(*t)[i].log.banco_c[k]].consulta++;
            (*t)[i].lucro += 3;
        }
        for (int k=0; k<= (*t)[i].log.pos_t; k++){
            if ((*t)[i].log.banco_t_or[k] != (*t)[i].log.banco_t_des[k]){
                (*t)[i].b[(*t)[i].log.banco_t_or[k]].t_perda += (*t)[i].log.quantia_t[k];
                (*t)[i].b[(*t)[i].log.banco_t_des[k]].t_ganho += (*t)[i].log.quantia_t[k];
            }
            (*t)[i].lucro += 3;
        }
        printf("===TERMINAL %d===\n", i+1);
        for (int j=0; j<n_banc; j++){
            if((*t)[i].b[j].saque != 0 || (*t)[i].b[j].deposito != 0 || (*t)[i].b[j].t_ganho != 0 || (*t)[i].b[j].t_perda != 0) printf("Banco %d: Moeda +%.2f -%.2f Transferencia +%.2f -%.2f\n", h_table[j], (*t)[i].b[j].deposito, (*t)[i].b[j].saque, (*t)[i].b[j].t_ganho, (*t)[i].b[j].t_perda);
            total_s[j] += (*t)[i].b[j].saque;
            total_d[j] += (*t)[i].b[j].deposito;
            total_c[j] += (*t)[i].b[j].consulta;
            total_t_ganho[j] += (*t)[i].b[j].t_ganho;
            total_t_perda[j] += (*t)[i].b[j].t_perda;
        }
        printf("Lucro obtido: %.2f\n", (*t)[i].lucro);
        total_lucro += (*t)[i].lucro;

        if ((*t)[i].log.saque_f != 0) printf("Erros de saque: %d\n", (*t)[i].log.saque_f);
        erros[1] = (*t)[i].log.saque_f;
        t_erros[1] += erros[1];
        if ((*t)[i].log.depo_f != 0) printf("Erros de deposito: %d\n", (*t)[i].log.depo_f);
        erros[2] = (*t)[i].log.depo_f;
        t_erros[2] += erros[2];
        if ((*t)[i].log.cons_f != 0) printf("Erros de consulta: %d\n", (*t)[i].log.cons_f);
        erros[3] = (*t)[i].log.cons_f;
        t_erros[3] += erros[3];
        if ((*t)[i].log.trans_f != 0) printf("Erros de transferencia: %d\n", (*t)[i].log.trans_f);
        erros[4] = (*t)[i].log.trans_f;
        t_erros[4] += erros[4];

        erros[0] = erros[1] + erros[2] + erros[3] + erros[4];
        t_erros[0] += erros[0];
        if (erros[0] != 0) printf("Total de erros: %d\n", erros[0]);
    }
    printf("===TOTAL===\n");
    for(int j=0; j<n_banc; j++){
        if (total_d[j] != 0 || total_s[j] != 0 || total_t_ganho[j] != 0 || total_t_perda[j] != 0) printf("Banco %d: Moeda +%.2f -%.2f Transferencia +%.2f -%.2f\n", h_table[j], total_d[j], total_s[j], total_t_ganho[j], total_t_perda[j]);
    }
    printf("Lucro obtido: %.2f\n", total_lucro);
    if (t_erros[1] != 0) printf("Erros de saque: %d\n", t_erros[1]);
    if (t_erros[2] != 0) printf("Erros de deposito: %d\n", t_erros[2]);
    if (t_erros[3] != 0) printf("Erros de consulta: %d\n", t_erros[3]);
    if (t_erros[4] != 0) printf("Erros de transferencia: %d\n", t_erros[4]);
    if (t_erros[0] != 0 ) printf("Total de erros: %d\n", t_erros[0]);
}

void auditoria(terminal** t, int* h_table, int c_terminal, int quantidade, char op){
    switch(op){
        case 'S':
            printf("===SAQUE TERMINAL %d===\n", c_terminal);
            printf("Mostrando primeiros %d resultados\n", quantidade);
            if ((*t)[c_terminal-1].log.pos_s == -1) {
                printf("Sem resultados\n");
                return;
            }
            else if ((*t)[c_terminal-1].log.pos_s+1 < quantidade) quantidade = ((*t)[c_terminal-1].log.pos_s) +1;
            for (int i=0; i<quantidade; i++){
                printf("%d- Banco %d %.2f\n", i+1, h_table[(*t)[c_terminal-1].log.banco_s[i]],(*t)[c_terminal-1].log.quantia_s[i]);
            }
            break;
        case 'D':
            printf("===DEPOSITO TERMINAL %d===\n", c_terminal);
            printf("Mostrando primeiros %d resultados\n", quantidade);
            if ((*t)[c_terminal-1].log.pos_d == -1) {
                printf("Sem resultados\n");
                return;
            }
            else if ((*t)[c_terminal-1].log.pos_d+1 < quantidade) quantidade = ((*t)[c_terminal-1].log.pos_d) +1;
            for (int i=0; i<quantidade; i++){
                printf("%d- Banco %d %.2f\n", i+1, h_table[(*t)[c_terminal-1].log.banco_d[i]],(*t)[c_terminal-1].log.quantia_d[i]);
            }
            break;
        case 'T':
            printf("===TRANSFERENCIA TERMINAL %d===\n", c_terminal);
            printf("Mostrando primeiros %d resultados\n", quantidade);
            if ((*t)[c_terminal-1].log.pos_t == -1) {
                printf("Sem resultados\n");
                return;
            }
            else if ((*t)[c_terminal-1].log.pos_t+1 < quantidade) quantidade = ((*t)[c_terminal-1].log.pos_t) +1;
            for (int i=0; i<quantidade; i++){
                printf("%d- Banco origem %d Banco destino %d %.2f\n", i+1, h_table[(*t)[c_terminal-1].log.banco_t_or[i]], h_table[(*t)[c_terminal-1].log.banco_t_des[i]], (*t)[c_terminal-1].log.quantia_t[i]);
            }
            break;
        default:
            break;
    }
}

int hash_func(int num){
    int h =((num % 33) % 4);
    if (h==1) h--;
    else if (h==0) h++;
    return h;
}
