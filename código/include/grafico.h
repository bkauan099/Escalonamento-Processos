// include/grafico.h
#ifndef GRAFICO_H
#define GRAFICO_H

#include <gtk/gtk.h>
#include <cairo.h>            /* Header do Cairo para desenhar */
#include "escalonador.h"      /* Para saber o que é struct processo */
#include "widgets.h"          /* Para usar struct Widgets */

void desenhar_grafico(GtkDrawingArea *area,
                      cairo_t       *cr,
                      int            width,
                      int            height,
                      gpointer       user_data);

/* Abre uma janela com o gráfico Gantt
 * @param widgets: ponteiro contendo processos e contagem
 */
void exibir_grafico(struct Widgets *widgets);

#endif // GRAFICO_H
