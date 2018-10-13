/* TRABALHO 4 - Escalonador de Processos
Programa feito para a Disciplina "Introducao a Ciencia da Computacao" que simula um escalonador de processos usando os algoritmos "ROUND-ROBIN", "FIRST-IN FIRST-OUT", "SHORTEST REMAINING TIME FIRST",
e "PRIORIDADE FIXA PREEMPTIVA", juntamente com uma logica "SUB,RUN,SET" explicada abaixo.

        Giovanni P. Meloni, ICMC, 2017.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int id;
    int tr; // Tempo de reconhecimento
    int trfifo;
    int te; // Tempo de execucao
    int priori;
    int chrono; // Um trigger
} processo;



int main(){
    processo* simula = NULL;
    int n;
    int tempo_max = 0;
    int num_proc = 0;
    int pos;
    int fifo = 0;
    char f;
    char linha[1000];
    //  --Lendo a entrada.
    while((fgets(linha, 1000, stdin)) != NULL){
        pos = 0;
        simula = (processo*) realloc(simula, (num_proc+1) * sizeof(processo));
        n = sscanf(&linha[pos],"%d %d %d %d", &(simula[num_proc].id), &(simula[num_proc].tr), &(simula[num_proc].te), &(simula[num_proc].priori));
        //O maximo de execucoes sera a somatoria de (Tempo de Rec + Tempo de Exec)
        if (n>0) {
            tempo_max += simula[num_proc].te + simula[num_proc].tr;
            simula[num_proc].trfifo = simula[num_proc].tr;
            simula[num_proc].chrono = 0;
            num_proc++;
        }
        else{
            sscanf(&linha[pos], "%c", &f);
            if (f == 'f') fifo = 1;
        }
    }
    simula = realloc(simula, (num_proc)*sizeof(processo));

    if (!fifo){
        //  --Ordenando conforme a prioridade e ID.
        for(int i = 0; i<num_proc; i++){
            int prioridadezero = 0;
            processo max = simula[i];
            int k=i;
            for(int j = i+1; j < num_proc; j++){
                if (!prioridadezero && simula[j].priori == 0){
                    max = simula[j];
                    k = j;
                    prioridadezero = 1;
                }
                if (!prioridadezero && simula[j].priori > max.priori) {
                    max = simula[j];
                    k = j;
                }
                else if ( simula[j].priori == max.priori){
                    if (simula[j].id < max.id){
                        max = simula[j];
                        k = j;
                    }
                }
            }
            if (max.id != simula[i].id) {
                processo aux = simula[i];
                simula[i] = max;
                simula[k] = aux;
            }
        }

        /*  --Simular os processos usando a logica "SUB,RUN,SET"
         O altoritmo segue basicamente as seguintes etapas:
            - Retira 1 da variavel que conta o Tempo de Reconhecimento.
            - Caso o processo possua a variavel "Chrono" ativada, ele eh executado e retira 1 da variavel Tempo de Execucao.
            - Se o Tempo de reconhecimento for 0, a variavel "Chrono" eh ativada.
            - Volta a primeira etapa.
        */
        int reset = 0;
        int pt = 0; //Um indice responsavel por saber qual eh o processo atual, pode ser 0 ate (num_proc-1).

        for(int i = 0; i < tempo_max; i++){
            int exec = 1; // No maximo, um processo pode ser executado por iteracao, essa variavel fica responsavel por isso.
            /*  SUB
                   A subtracao deve ocorrer em todos os processos, pois eh o tempo de reconhecimento, varios podem ser reconhecidos ao mesmo tempo.
            */
            for(int j=0; j<num_proc; j++){
                if (simula[j].tr > 0) simula[j].tr--;
            }

            /*  RUN
                    Deve ser verificado toda a lista circular (1 ciclo), com cuidado para nao entrar em um loop, em busca do proximo processo disponivel para ser executado, lembrando que, em RUN,
                apenas UM processo pode ser executado por vez. Apos a execucao de um processo apto, a variavel "Chrono" desse processo deve ser resetada, indicando que esse processo acabou, e o
                indice de controle deve ser atualizado.
                    Se a prioridade for 0, nao atualiza o indice de controle.
            */
            if (reset && simula[pt].priori != simula[pt-1].priori){

                pt = 0;
                reset = 0;
            }
            for(int j=0; j<=num_proc && exec; j++){
                int permanece = 0;
                int npt = pt;
                npt = (npt + j) % num_proc;
                if (simula[npt].chrono) {
                    simula[npt].te--;
                    if (!simula[npt].priori) permanece = 1;
                    if (!simula[npt].te){
                        simula[npt].chrono = 0;
                        printf("%d %d\n", simula[npt].id, i);
                    }
                    exec = 0;
                    if (!permanece){
                        pt = (npt + 1) % num_proc;
                    }
                    else pt = npt;
                }
            }

            /*  SET
                Todos os processos devem ser verificados se estao eleitos a terem sua variavel "Chrono" setada para 1. Caso ja nao tenha sido setada, o tempo de reconhecimento tenha atingido 0
            e ainda tenha tempo de execucao restante.
            */
            for(int j=0; j<num_proc; j++){
                if(!simula[j].chrono && !simula[j].tr && simula[j].te > 0){
                    simula[j].chrono = 1;
                    if (simula[j].priori > simula[pt].priori) reset = 1;
                }
            }
        }
    }
    //Logica para o FIFO, um pouco diferente da logica "SUB, RUN, SET"
    else{
        processo* fila = NULL;
        fila = (processo*) malloc(num_proc * sizeof(processo));
        int pos_fila = 0;// no inicio nao existe fila
        int elem_fila = 0; //quantos elementos tem na fila
        int iteracoes = 0;
        int k=0;

        //A maior diferenca sera a parte SELEC/ORDENA, diferente do anterior, sempre devo checar se a lista esta em ordem caso um novo processo seja reconhecido.
        for(int i=0; i<num_proc; i++){
            processo frente = simula[i];
            for(int j=i+1; j<num_proc; j++){
                if(simula[j].tr < frente.tr){
                    frente = simula[j];
                    k = j;
                }
                else if (simula[j].tr == frente.tr){
                    if (simula[j].id < frente.id){
                        frente = simula[j];
                        k = j;
                    }
                }
            }
            if (frente.id != simula[i].id){
                processo aux = simula[i];
                simula[i] = frente;
                simula[k] = aux;
            }
            fila[i] = frente;
            elem_fila = i+1;
        }

        while (pos_fila < num_proc){

            /*  SUB
                    A subtracao deve ocorrer em todos os processos, pois eh o tempo de reconhecimento, varios podem ser reconhecidos ao mesmo tempo.
            */
            for(int j=0; j<elem_fila; j++){
               if (fila[j].tr >0) fila[j].tr--;
            }

            /*  RUN
                    O processo no inicio da fila sera executado ATE TERMINAR, e entao passara para o proximo.
            */
            if (fila[pos_fila].chrono){
                fila[pos_fila].te--;
                if(!fila[pos_fila].te){
                    fila[pos_fila].chrono = 0;
                    printf("%d %d\n", fila[pos_fila].id, iteracoes);
                    pos_fila++;
                }
            }

            /*  SET
                    Todos os processos devem ser verificados se estao eleitos a terem sua variavel "Chrono" setada para 1. Caso ja nao tenha sido setada e o tempo de reconhecimento tenha atingido 0
            */
            for (int i=0; i<elem_fila; i++){
                if(!fila[i].chrono && !fila[i].tr && fila[i].te > 0) fila[i].chrono = 1;
            }

            iteracoes++;
        }
        free(fila);
    }
    free(simula);
    return 0;
}
