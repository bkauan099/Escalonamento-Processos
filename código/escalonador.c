#include <stdio.h>
#include "escalonador.h"

// Algoritmo FIFO
void escalonador_fifo(struct processo processos[], int n) {
    int tempo_atual = 0;
    for (int i = 0; i < n; i++) {
        if (tempo_atual < processos[i].chegada) {
            tempo_atual = processos[i].chegada;
        }
        processos[i].inicio = tempo_atual;
        processos[i].fim = tempo_atual + processos[i].duracao;
        processos[i].tempo_espera = processos[i].inicio - processos[i].chegada;
        processos[i].tempo_turnaround = processos[i].fim - processos[i].chegada;
        tempo_atual = processos[i].fim;
    }
}

// Algoritmo SJF
void escalonador_sjf(struct processo processos[], int n) {
    int tempo_atual = 0, processos_finalizados = 0;
    int visitado[n];

    for (int i = 0; i < n; i++) visitado[i] = 0;

    while (processos_finalizados < n) {
        int menor = -1;
        for (int i = 0; i < n; i++) {
            if (!visitado[i] && processos[i].chegada <= tempo_atual) {
                if (menor == -1 || processos[i].duracao < processos[menor].duracao) {
                    menor = i;
                }
            }
        }
        if (menor == -1) {
            tempo_atual++;
        } else {
            processos[menor].inicio = tempo_atual;
            processos[menor].fim = tempo_atual + processos[menor].duracao;
            processos[menor].tempo_espera = processos[menor].inicio - processos[menor].chegada;
            processos[menor].tempo_turnaround = processos[menor].fim - processos[menor].chegada;
            tempo_atual = processos[menor].fim;
            visitado[menor] = 1;
            processos_finalizados++;
        }
    }
}

// Algoritmo SRTN
void escalonador_srtn(struct processo processos[], int n) {
    int tempo_atual = 0, processos_finalizados = 0;
    int restante[n];

    for (int i = 0; i < n; i++) restante[i] = processos[i].duracao;

    while (processos_finalizados < n) {
        int menor = -1;
        for (int i = 0; i < n; i++) {
            if (restante[i] > 0 && processos[i].chegada <= tempo_atual) {
                if (menor == -1 || restante[i] < restante[menor]) {
                    menor = i;
                }
            }
        }
        if (menor == -1) {
            tempo_atual++;
        } else {
            if (processos[menor].inicio == -1) {
                processos[menor].inicio = tempo_atual;
            }
            restante[menor]--;
            tempo_atual++;

            if (restante[menor] == 0) {
                processos[menor].fim = tempo_atual;
                processos[menor].tempo_espera = processos[menor].fim - processos[menor].chegada - processos[menor].duracao;
                processos[menor].tempo_turnaround = processos[menor].fim - processos[menor].chegada;
                processos_finalizados++;
            }
        }
    }
}