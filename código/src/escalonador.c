#include <stdio.h>
#include <stdlib.h> // Para qsort
#include <string.h> // Para memcpy
#include "escalonador.h"

// Função auxiliar para copiar processos (necessário para o SRTN)
void copiar_processos(struct processo destino[], struct processo origem[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

// Algoritmo FIFO
void escalonador_fifo(struct processo processos[], int n) {
    // Ordena os processos por tempo de chegada para FIFO
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processos[j].chegada > processos[j+1].chegada) {
                struct processo temp = processos[j];
                processos[j] = processos[j+1];
                processos[j+1] = temp;
            }
        }
    }

    int tempo_atual = 0;
    for (int i = 0; i < n; i++) {
        // Se o tempo atual for menor que a chegada do processo, avança o tempo
        if (tempo_atual < processos[i].chegada) {
            tempo_atual = processos[i].chegada;
        }
        processos[i].inicio = tempo_atual;
        processos[i].fim = tempo_atual + processos[i].duracao;
        processos[i].tempo_espera = processos[i].inicio - processos[i].chegada;
        processos[i].tempo_turnaround = processos[i].fim - processos[i].chegada;
        tempo_atual = processos[i].fim;

        // Para o gráfico, FIFO executa continuamente
        processos[i].num_exec = 0;
        for (int t = processos[i].inicio; t < processos[i].fim; t++) {
            if (processos[i].num_exec < 100) { // Evita estouro de buffer
                processos[i].exec_seq[processos[i].num_exec++] = t;
            }
        }
    }
}

// Algoritmo SJF (Não Preemptivo)
void escalonador_sjf(struct processo processos[], int n) {
    int tempo_atual = 0;
    int processos_finalizados = 0;
    int visitado[n]; // Marca se o processo já foi executado

    for (int i = 0; i < n; i++) {
        visitado[i] = 0; // Inicializa todos como não visitados
    }

    while (processos_finalizados < n) {
        int menor_duracao_idx = -1;
        int min_duracao = -1;

        // Encontra o processo com menor duração que já chegou e ainda não foi executado
        for (int i = 0; i < n; i++) {
            if (!visitado[i] && processos[i].chegada <= tempo_atual) {
                if (menor_duracao_idx == -1 || processos[i].duracao < min_duracao) {
                    min_duracao = processos[i].duracao;
                    menor_duracao_idx = i;
                }
            }
        }

        if (menor_duracao_idx == -1) {
            // Nenhum processo disponível para execução, avança o tempo
            tempo_atual++;
        } else {
            // Executa o processo encontrado
            processos[menor_duracao_idx].inicio = tempo_atual;
            processos[menor_duracao_idx].fim = tempo_atual + processos[menor_duracao_idx].duracao;
            processos[menor_duracao_idx].tempo_espera = processos[menor_duracao_idx].inicio - processos[menor_duracao_idx].chegada;
            processos[menor_duracao_idx].tempo_turnaround = processos[menor_duracao_idx].fim - processos[menor_duracao_idx].chegada;
            tempo_atual = processos[menor_duracao_idx].fim;
            visitado[menor_duracao_idx] = 1;
            processos_finalizados++;

            // Para o gráfico, SJF executa continuamente
            processos[menor_duracao_idx].num_exec = 0;
            for (int t = processos[menor_duracao_idx].inicio; t < processos[menor_duracao_idx].fim; t++) {
                if (processos[menor_duracao_idx].num_exec < 100) { // Evita estouro de buffer
                    processos[menor_duracao_idx].exec_seq[processos[menor_duracao_idx].num_exec++] = t;
                }
            }
        }
    }
}

// Algoritmo SRTN (Shortest Remaining Time Next)
void escalonador_srtn(struct processo processos[], int n) {
    int tempo_atual = 0;
    int processos_finalizados = 0;
    int restante[n]; // Tempo restante de cada processo
    int inicio_real[n]; // Tempo real de início de cada processo (primeira vez que executa)

    // Inicializa tempos restantes e sequências de execução
    for (int i = 0; i < n; i++) {
        restante[i] = processos[i].duracao;
        processos[i].num_exec = 0; // Zera o contador de execuções para o gráfico
        inicio_real[i] = -1; // -1 indica que ainda não começou
    }

    while (processos_finalizados < n) {
        int menor_restante_idx = -1;
        int min_restante = -1;

        // Encontra o processo com o menor tempo restante que já chegou
        for (int i = 0; i < n; i++) {
            if (restante[i] > 0 && processos[i].chegada <= tempo_atual) {
                if (menor_restante_idx == -1 || restante[i] < min_restante) {
                    min_restante = restante[i];
                    menor_restante_idx = i;
                }
            }
        }

        if (menor_restante_idx == -1) {
            // Nenhum processo disponível para execução, avança o tempo
            tempo_atual++;
        } else {
            // Se o processo está começando a executar pela primeira vez
            if (inicio_real[menor_restante_idx] == -1) {
                inicio_real[menor_restante_idx] = tempo_atual;
            }

            // Marca o tempo atual como um ponto de execução para o gráfico
            if (processos[menor_restante_idx].num_exec < 100) { // Evita estouro de buffer
                processos[menor_restante_idx].exec_seq[processos[menor_restante_idx].num_exec++] = tempo_atual;
            }

            restante[menor_restante_idx]--; // Decrementa o tempo restante
            tempo_atual++; // Avança o tempo

            // Se o processo terminou de executar
            if (restante[menor_restante_idx] == 0) {
                processos[menor_restante_idx].fim = tempo_atual;
                processos[menor_restante_idx].inicio = inicio_real[menor_restante_idx]; // Define o início real
                processos[menor_restante_idx].tempo_espera = processos[menor_restante_idx].fim - processos[menor_restante_idx].chegada - processos[menor_restante_idx].duracao;
                processos[menor_restante_idx].tempo_turnaround = processos[menor_restante_idx].fim - processos[menor_restante_idx].chegada;
                processos_finalizados++;
            }
        }
    }
}
