Simulador de Escalonamento de Processos

Projeto acadêmico desenvolvido como parte da disciplina de Arquitetura de Computadores, com o objetivo de simular e visualizar o funcionamento de algoritmos de escalonamento de processos em um sistema operacional.
🧠 Objetivo

Desenvolver um simulador de escalonamento de processos com interface gráfica, utilizando a linguagem C e a biblioteca GTK4. O sistema permite a inserção de processos, escolha do algoritmo de escalonamento e visualização da execução por meio de um gráfico de Gantt.
🧩 Funcionalidades

    Inserção e remoção de processos (PID, tempo de chegada e duração).

    Seleção do algoritmo de escalonamento (FIFO, SJF, SRTN).

    Execução do algoritmo com exibição dos resultados no terminal.

    Visualização gráfica da execução dos processos em um gráfico de Gantt.

    Interface gráfica amigável construída com GTK4.

    Validação básica de entrada de dados.

⚙️ Algoritmos Implementados

    FIFO (First In, First Out)
    Executa os processos na ordem de chegada.

    SJF (Shortest Job First)
    Executa o processo com a menor duração entre os disponíveis.

    SRTN (Shortest Remaining Time Next)
    Variante preemptiva do SJF, avalia o menor tempo restante a cada unidade de tempo.

Todos os algoritmos calculam automaticamente:

    Tempo de início (inicio)

    Tempo de término (fim)

    Tempo de espera (tempo_espera)

    Tempo de turnaround (tempo_turnaround)

🖥️ Tecnologias Utilizadas

    Linguagem C

    GTK4 - Construção da interface gráfica

    Cairo - Geração do gráfico de Gantt

    GLib - Suporte a estruturas auxiliares

📁 Estrutura do Projeto

ESCALONAMENTO PROCESSOS/
├── código/
│   ├── escalonador.c     # Lógica dos algoritmos FIFO, SJF e SRTN
│   ├── grafico.c         # Desenho do gráfico de Gantt com Cairo/GTK
│   └── teste.c           # Testes ou execução inicial
│
├── Documentos/
│   ├── Planejamento Escalonamento.docx/pdf
│   ├── Relatório de Desenvolvimento do Produto - Parte Final.docx/pdf
│   ├── TAP.docx/pdf
│
└── Slide/                # Apresentações (em construção)

🔄 Fluxo de Funcionamento

    O usuário insere os processos manualmente pela GUI.

    Escolhe o algoritmo desejado.

    Clica em "Executar Escalonamento".

    O algoritmo processa os dados e exibe os resultados.

    O gráfico de Gantt é renderizado visualmente.

📈 Resultados

    Interface funcional com entrada e remoção de processos.

    Algoritmos corretamente implementados e testados.

    Representação gráfica clara e interativa.

    Interface responsiva construída com GTK4.

🚀 Próximas Melhorias

    Exibir métricas médias (ex: tempo médio de espera) diretamente na GUI.

    Adicionar salvamento/carregamento de processos.

    Melhorar visualmente o gráfico Gantt com mais informações.

    Implementar simulação passo a passo da execução (modo educacional).

👥 Autores

    Bruno Kauan Rodrigues Silva – 2022030340

    Gabriel Patrick Lima Carneiro – 2022030180

    Herick Vinicius Pinheiro da Conceição – 2022039711

    Eduardo dos Santos Oliveira – 2022039702

    Ellen Cristina de Sousa Castro – 2022030206

🏫 Instituição

Universidade Federal do Maranhão (UFMA)
Bacharelado Interdisciplinar em Ciência e Tecnologia
Disciplina: Arquitetura de Computadores