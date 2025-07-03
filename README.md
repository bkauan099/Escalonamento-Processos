# **Simulador de Escalonamento de Processos**

Este projeto é um **simulador interativo de escalonamento de processos** desenvolvido para fins educacionais na disciplina de **Arquitetura de Computadores** do curso de **Bacharelado Interdisciplinar em Ciência e Tecnologia (BICT)** da Universidade Federal do Maranhão.  O simulador foi construído em **C**, com **GTK4** e **Cairo**, permitindo ao usuário inserir processos, escolher algoritmos clássicos (FIFO, SJF, SRTN) e visualizar sua execução em um **Gráfico de Gantt*.

---

## 🚀 Funcionalidades

- **Interface Gráfica Completa**:
  - Inserção de novos processos com PID, tempo de chegada e duração.
  - Remoção de processos da lista com um clique.
  - Seleção do algoritmo desejado (FIFO, SJF ou SRTN).
  - Validação básica para evitar entradas inválidas.

- **Execução de Algoritmos de Escalonamento**:
  - FIFO: Executa os processos na ordem de chegada.
  - SJF: Seleciona o processo com menor duração.
  - SRTN: Variante preemptiva do SJF que avalia o menor tempo restante em tempo real.
  - Exibe resultados como tempos de início, fim, espera e turnaround no terminal.

- **Visualização Gráfica**:
  - Geração de **Gráfico de Gantt** colorido para ilustrar a execução dos processos.
  - Blocos proporcionais ao tempo de execução.

- **Integração Entre Módulos**:
  - Separação clara entre interface, lógica de escalonamento e renderização gráfica.
  - Modularidade para facilitar manutenção e futuras expansões.

---

## 🛠️ Tecnologias Utilizadas

- **C**: Linguagem principal do projeto.
- **GTK4**: Construção da interface gráfica.
- **Cairo**: Renderização do gráfico de Gantt.
- **GLib**: Utilitários e estruturas auxiliares.

---

## 📂 Estrutura do Projeto
```
escalonador-processos/
escalonador-processos/
├── código/
│ ├── main.c # Interface gráfica e controle geral
│ ├── escalonador.c # Implementação dos algoritmos FIFO, SJF, SRTN
│ ├── grafico.c # Desenho do Gráfico de Gantt usando Cairo/GTK
│ ├── widgets.h # Estruturas de dados para a GUI
│ └── grafico.h # Definições para renderização
│
├── Documentos/
│ ├── Planejamento Escalonamento.docx
│ ├── Relatório de Desenvolvimento do Produto - Parte Final.docx
│ └── TAP.docx
│
└── Slide/
└── (em construção)
```
---

## 🖥️ Como Executar o Projeto

> **Requisitos**:  
> - Linux com GTK4 instalado (pacote `libgtk-4-dev` ou equivalente)  
> - Compilador GCC

1️⃣ Clone o repositório:
git clone https://github.com/seu-usuario/escalonador-processos.git

2️⃣ Acesse a pasta do projeto:
cd escalonador-processos/código

3️⃣ Compile o projeto:
gcc main.c escalonador.c grafico.c -o simulador `pkg-config --cflags --libs gtk4`

4️⃣ Execute o simulador:
./simulador

---

## 📈 Resultados Esperados

  * Interface gráfica funcional para adicionar/remover processos.

  * Escolha de algoritmo (FIFO, SJF ou SRTN).

  * Saída de tempos de execução no terminal.

  * Renderização clara do Gráfico de Gantt com os processos escalonados.

---

## 🎯 Próximas Melhorias

   * Exibir métricas médias na GUI (ex: tempo médio de espera, turnaround médio).

  *  Adicionar salvamento e carregamento de listas de processos.

   * Melhorar a qualidade visual do gráfico (início, fim, legendas).

  * Implementar animação "passo a passo" para fins educacionais.
---

## ✅ Validação de Dados

* Verificação de campos obrigatórios na inserção de processos.

* Limites mínimos/máximos para tempos de chegada e duração.

* Bloqueio de inserção de entradas vazias ou inválidas.
---

## 👨‍💻 Autores

    Bruno Kauan Rodrigues Silva – 2022030340

    Gabriel Patrick Lima Carneiro – 2022030180

    Herick Vinicius Pinheiro da Conceição – 2022039711

    Eduardo dos Santos Oliveira – 2022039702

    Ellen Cristina de Sousa Castro – 2022030206

---

## 🏫 Instituição

Universidade Federal do Maranhão (UFMA)
Bacharelado Interdisciplinar em Ciência e Tecnologia
Disciplina: Arquitetura de Computadores

---

## 📚 Licença
Este projeto é acadêmico e de uso educacional. Sinta-se livre para estudar e modificar para fins não comerciais.



