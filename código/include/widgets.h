// include/widgets.h
#ifndef WIDGETS_H
#define WIDGETS_H

#include <gtk/gtk.h>
#include "escalonador.h"

/* Guarda ponteiros para todos os widgets e dados compartilhados */
struct Widgets {
    GtkWidget      *entrada_pid;
    GtkWidget      *entrada_duracao;
    GtkWidget      *entrada_chegada;
    GtkWidget      *list_box;         /* lista de algoritmos */
    GtkWidget      *list_view;         /* lista de processos */
    GtkWidget      *drawing_area;
    char           *algoritmo_selecionado;
    struct processo *processos;        /* array alocado em main */
    int            num_processos;
};

#endif // WIDGETS_H
