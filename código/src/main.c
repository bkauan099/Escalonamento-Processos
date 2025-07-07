#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "escalonador.h"
#include "grafico.h"
#include "widgets.h"

#define MAX_PROCESSOS 100

struct processo processos[MAX_PROCESSOS];
int num_processos = 0;

void on_algoritmo_selecionado(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data) {
    if (row != NULL) {
        GtkWidget *label = gtk_list_box_row_get_child(row);
        const gchar *texto = gtk_label_get_text(GTK_LABEL(label));
        g_print("Algoritmo selecionado: %s\n", texto);
    }
}

static void adicionar_processo(GtkWidget *widget, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    if (!widgets || !widgets->entrada_pid || !widgets->entrada_duracao || !widgets->entrada_chegada || !widgets->list_view) {
        g_warning("Erro: Um ou mais widgets são NULL!");
        return;
    }

    const gchar *texto_pid = gtk_editable_get_text(GTK_EDITABLE(widgets->entrada_pid));
    const gchar *texto_duracao = gtk_editable_get_text(GTK_EDITABLE(widgets->entrada_duracao));
    const gchar *texto_chegada = gtk_editable_get_text(GTK_EDITABLE(widgets->entrada_chegada));

    if (!texto_pid || !texto_duracao || !texto_chegada || *texto_pid == '\0' || *texto_duracao == '\0' || *texto_chegada == '\0') {
        g_warning("Erro: Campos de entrada vazios.");
        return;
    }

    int pid = atoi(texto_pid);
    int duracao = atoi(texto_duracao);
    int chegada = atoi(texto_chegada);

    struct processo novo_processo = {
        .pid = pid,
        .duracao = duracao,
        .chegada = chegada,
        .restante = duracao,
        .inicio = -1,
        .fim = -1,
        .tempo_espera = 0,
        .tempo_turnaround = 0,
        .tempo_resposta = -1
    };

    if (widgets->num_processos < MAX_PROCESSOS) {
        widgets->processos[widgets->num_processos++] = novo_processo;
    } else {
        g_warning("Erro: Limite de processos alcançado.");
        return;
    }

    char buffer[100];
    snprintf(buffer, sizeof(buffer), "PID: %d, Duração: %d, Chegada: %d", novo_processo.pid, novo_processo.duracao, novo_processo.chegada);

    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label = gtk_label_new(buffer);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
    gtk_list_box_insert(GTK_LIST_BOX(widgets->list_view), row, -1);

    gtk_widget_set_visible(row, TRUE);

    gtk_editable_set_text(GTK_EDITABLE(widgets->entrada_pid), "");
    gtk_editable_set_text(GTK_EDITABLE(widgets->entrada_duracao), "");
    gtk_editable_set_text(GTK_EDITABLE(widgets->entrada_chegada), "");
}

static void remover_processo(GtkWidget *widget, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    GtkListBoxRow *row_selecionada = gtk_list_box_get_selected_row(GTK_LIST_BOX(widgets->list_view));

    if (!row_selecionada) {
        g_warning("Erro: Nenhum processo selecionado.");
        return;
    }

    GtkWidget *label = gtk_list_box_row_get_child(row_selecionada);
    if (!GTK_IS_LABEL(label)) {
        g_warning("Erro: O widget selecionado não contém um rótulo válido.");
        return;
    }

    const gchar *texto_processo = gtk_label_get_text(GTK_LABEL(label));
    int pid_remover;
    if (sscanf(texto_processo, "PID: %d", &pid_remover) != 1) {
        g_warning("Erro: Não foi possível extrair o PID do processo.");
        return;
    }

    int indice_remover = -1;
    for (int i = 0; i < widgets->num_processos; i++) {
        if (widgets->processos[i].pid == pid_remover) {
            indice_remover = i;
            break;
        }
    }

    if (indice_remover == -1) {
        g_warning("Erro: Processo com PID %d não encontrado.", pid_remover);
        return;
    }

    for (int i = indice_remover; i < widgets->num_processos - 1; i++) {
        widgets->processos[i] = widgets->processos[i + 1];
    }
    widgets->num_processos--;

    gtk_widget_unparent(GTK_WIDGET(row_selecionada));
    gtk_widget_unparent(label);

    printf("Processo com PID %d removido.\n", pid_remover);
}

static void executar_escalonamento(GtkWidget *widget, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    if (widgets->algoritmo_selecionado == NULL) {
        g_warning("Erro: Nenhum algoritmo de escalonamento selecionado.");
        return;
    }

    if (g_strcmp0(widgets->algoritmo_selecionado, "FIFO") == 0) {
        escalonador_fifo(widgets->processos, widgets->num_processos);
    } else if (g_strcmp0(widgets->algoritmo_selecionado, "SJF") == 0) {
        escalonador_sjf(widgets->processos, widgets->num_processos);
    } else if (g_strcmp0(widgets->algoritmo_selecionado, "SRTN") == 0) {
        escalonador_srtn(widgets->processos, widgets->num_processos);
    } else {
        g_warning("Erro: Algoritmo desconhecido.");
        return;
    }

    for (int i = 0; i < widgets->num_processos; i++) {
        printf("PID: %d, Inicio: %d, Fim: %d, Tempo de Espera: %d, Turnaround: %d\n",
               widgets->processos[i].pid, widgets->processos[i].inicio, widgets->processos[i].fim,
               widgets->processos[i].tempo_espera, widgets->processos[i].tempo_turnaround);
    }

    exibir_grafico(widgets);
    gtk_widget_queue_draw(widgets->drawing_area);
}

static void on_row_selected(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    if (row) {
        GtkWidget *label = gtk_list_box_row_get_child(row);
        if (GTK_IS_LABEL(label)) {
            const gchar *algoritmo = gtk_label_get_text(GTK_LABEL(label));
            widgets->algoritmo_selecionado = g_strdup(algoritmo);
        }
    }
}

static void aplicar_tema(GtkWidget *janela) {
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_string(provider,
        ".window { background-color: #34495E; }"
        ".label-formulario { color: white; font-family: Helvetica; font-size: 14px; }"
        ".label-lista { color: black; font-family: Helvetica; font-size: 14px; }"
        ".entry { background-color: #D5DBDB; color: black; font-size: 14px; }"
        ".button { background-color: #1E90FF; color: black; font-size: 14px; }"
    );

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    g_object_unref(provider);
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *janela = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(janela), "Gerenciador de Processos");
    gtk_window_set_default_size(GTK_WINDOW(janela), 680, 800);

    aplicar_tema(janela);
    gtk_widget_add_css_class(janela, "window");

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_window_set_child(GTK_WINDOW(janela), grid);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 0, 8, 2, 1);

    struct Widgets *widgets = g_new(struct Widgets, 1);
    widgets->drawing_area = drawing_area;

    // Labels do formulário com classe específica
    GtkWidget *label_pid = gtk_label_new("PID do Processo:");
    gtk_widget_add_css_class(label_pid, "label-formulario");
    gtk_grid_attach(GTK_GRID(grid), label_pid, 0, 0, 1, 1);
    widgets->entrada_pid = gtk_entry_new();
    gtk_widget_add_css_class(widgets->entrada_pid, "entry");
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada_pid, 1, 0, 1, 1);

    GtkWidget *label_duracao = gtk_label_new("Tempo de Duração do Processo:");
    gtk_widget_add_css_class(label_duracao, "label-formulario");
    gtk_grid_attach(GTK_GRID(grid), label_duracao, 0, 1, 1, 1);
    widgets->entrada_duracao = gtk_entry_new();
    gtk_widget_add_css_class(widgets->entrada_duracao, "entry");
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada_duracao, 1, 1, 1, 1);

    GtkWidget *label_chegada = gtk_label_new("Tempo de Chegada do Processo:");
    gtk_widget_add_css_class(label_chegada, "label-formulario");
    gtk_grid_attach(GTK_GRID(grid), label_chegada, 0, 2, 1, 1);
    widgets->entrada_chegada = gtk_entry_new();
    gtk_widget_add_css_class(widgets->entrada_chegada, "entry");
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada_chegada, 1, 2, 1, 1);

    // Botões
    GtkWidget *botao_adicionar = gtk_button_new_with_label("Adicionar Processo");
    gtk_widget_add_css_class(botao_adicionar, "button");
    gtk_grid_attach(GTK_GRID(grid), botao_adicionar, 0, 3, 2, 1);

    GtkWidget *botao_remover = gtk_button_new_with_label("Remover Processo");
    gtk_widget_add_css_class(botao_remover, "button");
    gtk_grid_attach(GTK_GRID(grid), botao_remover, 0, 4, 2, 1);

    GtkWidget *label_algoritmo = gtk_label_new("Algoritmo de Escalonamento:");
    gtk_widget_add_css_class(label_algoritmo, "label-formulario");
    gtk_grid_attach(GTK_GRID(grid), label_algoritmo, 0, 5, 1, 1);

    GtkWidget *list_box = gtk_list_box_new();
    gtk_grid_attach(GTK_GRID(grid), list_box, 1, 5, 1, 1);

    // Algoritmos com classe para texto preto
    GtkWidget *row_fifo = gtk_list_box_row_new();
    GtkWidget *label_fifo = gtk_label_new("FIFO");
    gtk_widget_add_css_class(label_fifo, "label-lista");
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_fifo), label_fifo);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), row_fifo, -1);

    GtkWidget *row_sjf = gtk_list_box_row_new();
    GtkWidget *label_sjf = gtk_label_new("SJF");
    gtk_widget_add_css_class(label_sjf, "label-lista");
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_sjf), label_sjf);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), row_sjf, -1);

    GtkWidget *row_srtn = gtk_list_box_row_new();
    GtkWidget *label_srtn = gtk_label_new("SRTN");
    gtk_widget_add_css_class(label_srtn, "label-lista");
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_srtn), label_srtn);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), row_srtn, -1);

    widgets->list_box = list_box;
    widgets->list_view = gtk_list_box_new();
    gtk_widget_set_vexpand(widgets->list_view, TRUE);
    gtk_grid_attach(GTK_GRID(grid), widgets->list_view, 0, 7, 2, 1);

    widgets->processos = g_new(struct processo, MAX_PROCESSOS);
    widgets->num_processos = 0;

    g_signal_connect(widgets->list_box, "row-selected", G_CALLBACK(on_algoritmo_selecionado), widgets);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), desenhar_grafico, widgets, NULL);
    g_signal_connect(list_box, "row-selected", G_CALLBACK(on_row_selected), widgets);

    GtkWidget *botao_executar = gtk_button_new_with_label("Executar Escalonamento");
    gtk_widget_add_css_class(botao_executar, "button");
    gtk_grid_attach(GTK_GRID(grid), botao_executar, 0, 6, 2, 1);

    g_signal_connect(botao_adicionar, "clicked", G_CALLBACK(adicionar_processo), widgets);
    g_signal_connect(botao_executar, "clicked", G_CALLBACK(executar_escalonamento), widgets);
    g_signal_connect(botao_remover, "clicked", G_CALLBACK(remover_processo), widgets);

    gtk_window_present(GTK_WINDOW(janela));
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.exemplo.gerenciador", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
