// include/escalonador.h
#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#define MAX_PROCESSOS 100

/* Estrutura de um processo */
struct processo {
    int pid;
    int chegada;
    int duracao;
    int restante;         // usado no SRTN
    int inicio;
    int fim;
    int tempo_espera;
    int tempo_turnaround;
    int tempo_resposta;   // opcional, se quiser calcular tempo de resposta
    // Adicionado para SRTN: armazena a sequência de execução
    int exec_seq[100]; // Max 100 execuções para um processo
    int num_exec;
};

/* Protótipos dos algoritmos */
void escalonador_fifo(struct processo processos[], int n);
void escalonador_sjf(struct processo processos[], int n);
void escalonador_srtn(struct processo processos[], int n);

#endif // ESCALONADOR_H
