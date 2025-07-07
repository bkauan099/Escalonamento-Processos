// grafico.c
#include <gtk/gtk.h>
#include "grafico.h" 
#include <cairo.h>
#include "escalonador.h"
#include "widgets.h"
#include <string.h> // Para strcmp

// Definição de algumas cores pastel
typedef struct {
    double r, g, b;
} Color;

// Array de cores pastel com contraste ligeiramente aumentado
const Color pastel_colors[] = {
    {0.95, 0.70, 0.70}, // Light Coral (mais contraste)
    {0.70, 0.95, 0.70}, // Light Green (mais contraste)
    {0.70, 0.70, 0.95}, // Light Blue (mais contraste)
    {0.95, 0.95, 0.70}, // Light Yellow (mais contraste)
    {0.95, 0.70, 0.95}, // Light Purple (mais contraste)
    {0.70, 0.95, 0.95}, // Light Cyan (mais contraste)
    {0.85, 0.60, 0.50}, // Peach (mais contraste)
    {0.60, 0.85, 0.50}, // Mint Green (mais contraste)
    {0.50, 0.60, 0.85}, // Lavender Blue (mais contraste)
    {0.85, 0.85, 0.60}  // Cream (mais contraste)
};
const int num_pastel_colors = sizeof(pastel_colors) / sizeof(pastel_colors[0]);


void desenhar_grafico(GtkDrawingArea *area,
    cairo_t       *cr,
    int            width,
    int            height,
    gpointer       user_data) {
    struct Widgets *w = user_data;
    int n = w->num_processos;
    struct processo *p = w->processos;
    const char *algoritmo_selecionado = w->algoritmo_selecionado;

    // Debug: Verifica qual algoritmo está sendo desenhado
    g_print("Algoritmo selecionado no grafico: %s\n", algoritmo_selecionado ? algoritmo_selecionado : "Nenhum");

    // Calcula o fim máximo para escalar dinamicamente
    int max_fim = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].fim > max_fim) {
            max_fim = p[i].fim;
        }
    }
    // Adiciona um pouco de margem no final do gráfico
    max_fim = (max_fim == 0) ? 10 : max_fim + 2; // Garante um valor mínimo para max_fim

    // Margens
    const int left_margin   = 50;
    const int top_margin    = 30;
    const int bar_height    = 40;
    const int spacing       = 10;
    const int time_height   = 20;

    // Espaço disponível para o gráfico de tempo
    int graf_width = width - left_margin - 20;

    // Unidade de largura por “tempo”
    double ux = (double)graf_width / max_fim;
    if (ux <= 0) ux = 1.0; // Evita divisão por zero ou valores não positivos

    // Fundo branco
    cairo_set_source_rgb(cr, 1,1,1);
    cairo_paint(cr);

    // Desenha cada processo como retângulo e PID
    for (int i = 0; i < n; i++) {
        // Seleciona a cor pastel para o processo
        Color current_color = pastel_colors[i % num_pastel_colors];
        double r = current_color.r;
        double g = current_color.g;
        double b = current_color.b;

        if (algoritmo_selecionado != NULL && strcmp(algoritmo_selecionado, "SRTN") == 0) {
            // Para SRTN, desenha cada segmento de execução
            for (int k = 0; k < p[i].num_exec; k++) {
                cairo_save(cr); // Salva o estado atual do Cairo

                double x_segmento = left_margin + p[i].exec_seq[k] * ux;
                double y = top_margin + i * (bar_height + spacing);
                double wdt_segmento = ux; // Cada segmento tem duração de 1 unidade de tempo

                cairo_set_source_rgb(cr, r, g, b); // Define a cor de preenchimento
                cairo_rectangle(cr, x_segmento, y, wdt_segmento, bar_height);
                cairo_fill_preserve(cr); // Preenche com a cor definida

                cairo_set_source_rgb(cr, 0,0,0); // Define a cor do contorno (preto)
                cairo_stroke(cr); // Desenha o contorno

                cairo_restore(cr); // Restaura o estado anterior do Cairo
            }
            // Desenha o PID uma vez, centralizado na primeira execução ou no início do processo
            if (p[i].num_exec > 0) {
                char label[16];
                snprintf(label, sizeof(label), "P%d", p[i].pid);
                // Centraliza o PID na primeira barra de execução
                double x_label = left_margin + p[i].exec_seq[0] * ux + 5; 
                double y_label = top_margin + i * (bar_height + spacing) + bar_height/2 + 5;
                cairo_set_source_rgb(cr, 0, 0, 0); // Cor do texto (preto)
                cairo_move_to(cr, x_label, y_label);
                cairo_show_text(cr, label);
            }

        } else {
            // Para FIFO e SJF, desenha uma única barra contínua
            double x = left_margin + p[i].inicio * ux;
            double y = top_margin + i * (bar_height + spacing);
            double wdt = p[i].duracao * ux;

            cairo_set_source_rgb(cr, r, g, b); // Define a cor de preenchimento
            cairo_rectangle(cr, x, y, wdt, bar_height);
            cairo_fill_preserve(cr); // Preenche com a cor definida

            cairo_set_source_rgb(cr, 0,0,0); // Define a cor do contorno (preto)
            cairo_stroke(cr); // Desenha o contorno

            // PID dentro do bloco
            char label[16];
            snprintf(label, sizeof(label), "P%d", p[i].pid);
            cairo_set_source_rgb(cr, 0, 0, 0); // Cor do texto (preto)
            cairo_move_to(cr, x + 5, y + bar_height/2 + 5);
            cairo_show_text(cr, label);
        }
    }

    // Desenha o eixo de tempo embaixo
    cairo_set_source_rgb(cr, 0,0,0);
    cairo_set_line_width(cr, 1.0);
    double y_axis = top_margin + n*(bar_height + spacing) + 5;
    cairo_move_to(cr, left_margin, y_axis);
    cairo_line_to(cr, left_margin + max_fim * ux, y_axis);
    cairo_stroke(cr);

    // Marcação de tempo
    for (int t = 0; t <= max_fim; t++) {
        double x = left_margin + t * ux;
        // risquinho
        cairo_move_to(cr, x, y_axis);
        cairo_line_to(cr, x, y_axis + 5);
        cairo_stroke(cr);

        // número do tempo
        char ts[8];
        snprintf(ts, sizeof(ts), "%d", t);
        cairo_move_to(cr, x - 3, y_axis + time_height);
        cairo_show_text(cr, ts);
    }
}


void exibir_grafico(struct Widgets *widgets) {
    GtkWidget *janela_grafico = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(janela_grafico), "Gráfico de Escalonamento");
    gtk_window_set_default_size(GTK_WINDOW(janela_grafico), 640, 480);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), desenhar_grafico, widgets, NULL);

    gtk_window_set_child(GTK_WINDOW(janela_grafico), drawing_area);
    gtk_window_present(GTK_WINDOW(janela_grafico));
}
