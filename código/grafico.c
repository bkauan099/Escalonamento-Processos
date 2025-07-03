// grafico.c
#include <gtk/gtk.h>
#include "grafico.h"  // Incluir o header que define a interface do gráfico
#include <cairo.h>
#include "escalonador.h"  // Inclua a estrutura de processos do seu programa
#include "widgets.h"

void desenhar_grafico(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    // Depuração para garantir que a função está sendo chamada
    printf("Desenhando gráfico com %d processos\n", widgets->num_processos);

    // Definir fundo branco
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Desenhar os processos como blocos (Gantt)
    if (widgets->processos != NULL) {
        int x_offset = 50;  // Posição inicial
        int y_offset = 50;
        int largura_unidade = 50;

        for (int i = 0; i < widgets->num_processos; i++) {
            struct processo p = widgets->processos[i];  // Corrigir para struct processo

            // Definir cor do processo (aleatória para diferenciação)
            cairo_set_source_rgb(cr, 0.2 * (i + 1), 0.4, 0.6);
            cairo_rectangle(cr, x_offset + p.chegada * largura_unidade, y_offset, p.duracao * largura_unidade, 40);  // Corrigir membros
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, 0, 0, 0);
            cairo_stroke(cr);

            // Exibir PID
            char pid_text[10];
            snprintf(pid_text, sizeof(pid_text), "P%d", p.pid);
            cairo_move_to(cr, x_offset + p.chegada * largura_unidade + 5, y_offset + 25);
            cairo_show_text(cr, pid_text);
        }
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