#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "escalonador.h"
#include "grafico.h"  // Incluir o header do grafico.c
#include "widgets.h"
#define MAX_PROCESSOS 100  // Definir um limite para a quantidade de processos

struct processo processos[MAX_PROCESSOS];
int num_processos = 0;  // Contador para o número de processos

// Função chamada quando uma linha é selecionada na lista
void on_algoritmo_selecionado(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data) {
    if (row != NULL) {
        GtkWidget *label = gtk_list_box_row_get_child(row);
        const gchar *texto = gtk_label_get_text(GTK_LABEL(label));

        // Aqui você pode manipular o texto ou a lógica associada à seleção
        g_print("Algoritmo selecionado: %s\n", texto);
    }
}

// Função para adicionar um processo
static void adicionar_processo(GtkWidget *widget, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    // Verificação de segurança para evitar ponteiros nulos
    if (!widgets || !widgets->entrada_pid || !widgets->entrada_duracao || !widgets->entrada_chegada || !widgets->list_view) {
        g_warning("Erro: Um ou mais widgets são NULL!");
        return;
    }

    // Capturar os valores dos campos de entrada
    const gchar *texto_pid = gtk_editable_get_text(GTK_EDITABLE(widgets->entrada_pid));
    const gchar *texto_duracao = gtk_editable_get_text(GTK_EDITABLE(widgets->entrada_duracao));
    const gchar *texto_chegada = gtk_editable_get_text(GTK_EDITABLE(widgets->entrada_chegada));

    // Verificar se os campos não estão vazios
    if (!texto_pid || !texto_duracao || !texto_chegada || *texto_pid == '\0' || *texto_duracao == '\0' || *texto_chegada == '\0') {
        g_warning("Erro: Campos de entrada vazios.");
        return;
    }

    // Converter os valores para inteiros
    int pid = atoi(texto_pid);
    int duracao = atoi(texto_duracao);
    int chegada = atoi(texto_chegada);

    // Criar uma instância da estrutura processo
    struct processo novo_processo = {
        .pid = pid,
        .duracao = duracao,
        .chegada = chegada,
        .restante = duracao, // Inicialmente, o tempo restante é igual à duração
        .inicio = -1,
        .fim = -1,
        .tempo_espera = 0,
        .tempo_turnaround = 0,
        .tempo_resposta = -1
    };

    // Adicionar o novo processo à lista global
    if (widgets->num_processos < MAX_PROCESSOS) {
        widgets->processos[widgets->num_processos++] = novo_processo;  // Aqui é onde a variável num_processos é atualizada
    } else {
        g_warning("Erro: Limite de processos alcançado.");
        return;
    }

    // Criar uma string para exibir o processo na lista
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "PID: %d, Duração: %d, Chegada: %d", novo_processo.pid, novo_processo.duracao, novo_processo.chegada);

    // Criar e adicionar o processo à GtkListBox (list_view)
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label = gtk_label_new(buffer);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
    gtk_list_box_insert(GTK_LIST_BOX(widgets->list_view), row, -1);

    // Mostrar a nova linha na lista
    gtk_widget_set_visible(row, TRUE);

    // Limpar os campos de entrada
    gtk_editable_set_text(GTK_EDITABLE(widgets->entrada_pid), "");
    gtk_editable_set_text(GTK_EDITABLE(widgets->entrada_duracao), "");
    gtk_editable_set_text(GTK_EDITABLE(widgets->entrada_chegada), "");
}


// Função para remover processo
static void remover_processo(GtkWidget *widget, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    // Obter a linha selecionada
    GtkListBoxRow *row_selecionada = gtk_list_box_get_selected_row(GTK_LIST_BOX(widgets->list_view));

    // Verifica se há uma linha selecionada
    if (!row_selecionada) {
        g_warning("Erro: Nenhum processo selecionado.");
        return;
    }

    // Obter o widget do label dentro da linha
    GtkWidget *label = gtk_list_box_row_get_child(row_selecionada);
    
    if (!GTK_IS_LABEL(label)) {
        g_warning("Erro: O widget selecionado não contém um rótulo válido.");
        return;
    }

    // Obter o texto do label
    const gchar *texto_processo = gtk_label_get_text(GTK_LABEL(label));

    // Extrair o PID do texto
    int pid_remover;
    if (sscanf(texto_processo, "PID: %d", &pid_remover) != 1) {
        g_warning("Erro: Não foi possível extrair o PID do processo.");
        return;
    }

    // Encontrar o índice do processo a ser removido
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

    // Remover o processo da lista de processos
    for (int i = indice_remover; i < widgets->num_processos - 1; i++) {
        widgets->processos[i] = widgets->processos[i + 1];
    }
    widgets->num_processos--;  // Atualiza o contador de processos

    // Remover a linha da GtkListBox corretamente
    gtk_widget_unparent(GTK_WIDGET(row_selecionada));

    // Liberar a memória do label
    gtk_widget_unparent(label);

    printf("Processo com PID %d removido.\n", pid_remover);
}


static void executar_escalonamento(GtkWidget *widget, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;

    // Verificar se um algoritmo foi selecionado
    if (widgets->algoritmo_selecionado == NULL) {
        g_warning("Erro: Nenhum algoritmo de escalonamento selecionado.");
        return;
    }

    // Executar o algoritmo de escalonamento
    if (g_strcmp0(widgets->algoritmo_selecionado, "FIFO") == 0) {
        escalonador_fifo(widgets->processos, widgets->num_processos);  // Chama o escalonador FIFO
    } else if (g_strcmp0(widgets->algoritmo_selecionado, "SJF") == 0) {
        escalonador_sjf(widgets->processos, widgets->num_processos);  // Chama o escalonador SJF
    } else if (g_strcmp0(widgets->algoritmo_selecionado, "SRTN") == 0) {
        escalonador_srtn(widgets->processos, widgets->num_processos);  // Chama o escalonador SRTN
    } else {
        g_warning("Erro: Algoritmo desconhecido.");
        return;
    }

    // Exibir os resultados do escalonamento
    for (int i = 0; i < widgets->num_processos; i++) {
        printf("Entrou aqui");
        printf("PID: %d, Inicio: %d, Fim: %d, Tempo de Espera: %d, Turnaround: %d\n",
               widgets->processos[i].pid, widgets->processos[i].inicio, widgets->processos[i].fim,
               widgets->processos[i].tempo_espera, widgets->processos[i].tempo_turnaround);
    }

    // Exibir a janela do gráfico
    exibir_grafico(widgets);  // Chama a função para exibir a janela do gráfico

    // Atualiza o gráfico (força a recriação do gráfico)
    gtk_widget_queue_draw(widgets->drawing_area);  // Atualiza a área de desenho
}

// Função chamada quando uma linha da lista de algoritmos é selecionada
static void on_row_selected(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data) {
    struct Widgets *widgets = (struct Widgets *)user_data;
    
    if (row) {
        // Obtém o widget da linha selecionada
        GtkWidget *label = gtk_list_box_row_get_child(row);
        
        // Verifica se o widget é um GtkLabel e obtém o texto
        if (GTK_IS_LABEL(label)) {
            const gchar *algoritmo = gtk_label_get_text(GTK_LABEL(label));
            
            // Salva o algoritmo selecionado para usar depois
            widgets->algoritmo_selecionado = g_strdup(algoritmo);  // Armazena o algoritmo
        }
    }
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    // Janela principal
    GtkWidget *janela = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(janela), "Gerenciador de Processos");
    gtk_window_set_default_size(GTK_WINDOW(janela), 680, 800);

    // Container principal
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_window_set_child(GTK_WINDOW(janela), grid);

    // Criar a área de desenho para o gráfico
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 0, 8, 2, 1);  // Adiciona ao grid

    // Aloca a estrutura widgets e inicializa os widgets
    struct Widgets *widgets = g_new(struct Widgets, 1);  // Aloca a estrutura widgets
    widgets->drawing_area = drawing_area;  // Atribui o drawing_area à estrutura

    // Entradas e rótulos
    GtkWidget *label_pid = gtk_label_new("PID do Processo:");
    gtk_grid_attach(GTK_GRID(grid), label_pid, 0, 0, 1, 1);
    widgets->entrada_pid = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada_pid, 1, 0, 1, 1);

    GtkWidget *label_duracao = gtk_label_new("Tempo de Duração do Processo:");
    gtk_grid_attach(GTK_GRID(grid), label_duracao, 0, 1, 1, 1);
    widgets->entrada_duracao = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada_duracao, 1, 1, 1, 1);

    GtkWidget *label_chegada = gtk_label_new("Tempo de Chegada do Processo:");
    gtk_grid_attach(GTK_GRID(grid), label_chegada, 0, 2, 1, 1);
    widgets->entrada_chegada = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada_chegada, 1, 2, 1, 1);

    // Botões
    GtkWidget *botao_adicionar = gtk_button_new_with_label("Adicionar Processo");
    gtk_grid_attach(GTK_GRID(grid), botao_adicionar, 0, 3, 2, 1);

    GtkWidget *botao_remover = gtk_button_new_with_label("Remover Processo");
    gtk_grid_attach(GTK_GRID(grid), botao_remover, 0, 4, 2, 1);

    // ComboBox usando GtkDropDown
    GtkWidget *label_algoritmo = gtk_label_new("Algoritmo de Escalonamento:");
    gtk_grid_attach(GTK_GRID(grid), label_algoritmo, 0, 5, 1, 1);

    // Usando GtkListBox para a lista de algoritmos
    GtkWidget *list_box = gtk_list_box_new();
    gtk_grid_attach(GTK_GRID(grid), list_box, 1, 5, 1, 1);

    GtkWidget *row_fifo = gtk_list_box_row_new();
    GtkWidget *label_fifo = gtk_label_new("FIFO");
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_fifo), label_fifo);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), row_fifo, -1);

    GtkWidget *row_sjf = gtk_list_box_row_new();
    GtkWidget *label_sjf = gtk_label_new("SJF");
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_sjf), label_sjf);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), row_sjf, -1);

    GtkWidget *row_srtn = gtk_list_box_row_new();
    GtkWidget *label_srtn = gtk_label_new("SRTN");
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_srtn), label_srtn);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), row_srtn, -1);

    // Inicializando o list_view
    widgets->list_box = list_box;
    widgets->list_view = gtk_list_box_new();  // Inicializa o list_view corretamente

    // Inicializando os campos de processos
    widgets->processos = g_new(struct processo, MAX_PROCESSOS);
    widgets->num_processos = 0; // Inicie com zero ou com o valor atual

    // Configura o list_view
    gtk_widget_set_vexpand(widgets->list_view, TRUE);
    gtk_grid_attach(GTK_GRID(grid), widgets->list_view, 0, 7, 2, 1); // Adiciona ao grid

    // Criar janela para o gráfico
    GtkWidget *janela_grafico = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(janela_grafico), "Gráfico de Escalonamento");
    gtk_window_set_default_size(GTK_WINDOW(janela_grafico), 640, 480);

    // Removendo a conexão do evento de fechamento da janela
    // A janela será fechada de forma padrão pelo GTK4

    // Conectar o algoritmo selecionado
    g_signal_connect(widgets->list_box, "row-selected", G_CALLBACK(on_algoritmo_selecionado), widgets);

    // Conectar o sinal de desenho
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), desenhar_grafico, widgets, NULL);  // Passando widgets como user_data

    // Conectar o sinal de seleção da lista
    g_signal_connect(list_box, "row-selected", G_CALLBACK(on_row_selected), widgets);

    // Botão de executar escalonamento
    GtkWidget *botao_executar = gtk_button_new_with_label("Executar Escalonamento");
    gtk_grid_attach(GTK_GRID(grid), botao_executar, 0, 6, 2, 1);

    // Conectar os sinais dos botões
    g_signal_connect(botao_adicionar, "clicked", G_CALLBACK(adicionar_processo), widgets);
    g_signal_connect(botao_executar, "clicked", G_CALLBACK(executar_escalonamento), widgets);
    g_signal_connect(botao_remover, "clicked", G_CALLBACK(remover_processo), widgets);

    // Mostrar a janela principal
    gtk_window_present(GTK_WINDOW(janela));
}





int main(int argc, char *argv[]) {
    // Criar uma nova aplicação GTK
    GtkApplication *app = gtk_application_new("com.exemplo.gerenciador", G_APPLICATION_DEFAULT_FLAGS);

    // Conectar o sinal "activate"
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Executar o aplicativo
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Liberar recursos
    g_object_unref(app);
    return status;
}