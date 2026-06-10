# Trabalho Final de Algoritmo e Estrutura de Dados(AEDS)

## Agenda Telefônica com Busca Otimizada 📞

Este repositório contém o projeto final desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados (AEDS)**, sob a orientação da **Professora Camille Nader**. O objetivo principal é consolidar de forma integrada os conceitos de gerenciamento de memória dinâmica e o acoplamento sinérgico de múltiplas estruturas de dados heterogêneas na linguagem C.

O sistema resolve de forma cirúrgica os gargalos de busca sequencial comuns em agendas ingênuas, dividindo as responsabilidades operacionais em um tripé de estruturas interligadas por ponteiros precisos, garantindo eficiência máxima sem qualquer redundância ou duplicidade de dados.

---

## 🛠️ O Problema e a Arquitetura da Solução

Em implementações rudimentares utilizando vetores ou listas simples, a busca por qualquer campo exige complexidade $O(n)$, demandando milhares de comparações à medida que o volume de contatos cresce. Para mitigar isso, este software utiliza três estruturas de dados dinâmicas integradas:

1. **Árvore Binária de Busca (ABB):** É a estrutura espinhal do sistema. Armazena os dados principais do contato (Nome) de forma hierárquica e ordenada.
   - **Por que usar?** Permite a listagem em ordem alfabética imediata através de um percurso *In-Ordem* e reduz a complexidade de busca por nome para tempo logarítmico: **$O(\\log n)$**.
2. **Lista Encadeada Simples Dinâmica:** Acoplada internamente a cada nó da ABB.
   - **Por que usar?** Permite que um mesmo contato possua múltiplos números de telefone associados. Como o tamanho da lista se molda dinamicamente, não há desperdício de memória por tamanho fixo.
3. **Tabela Hash com Encadeamento:** Uma estrutura indexada externa cujos nós contêm ponteiros que referenciam diretamente os nós correspondentes dentro da ABB.
   - **Por que usar?** Resolve a **busca reversa** (encontrar o dono a partir de um número de telefone) em tempo constante médio: **$O(1)$**, independentemente do tamanho da agenda.

---

## 📊 Análise de Complexidade de Algoritmos

| Operação | Estrutura Principal | Complexidade Algorítmica |
| :--- | :--- | :--- |
| **Busca por Nome** | Árvore Binária de Busca | $O(\\log n)$ (Tempo Logarítmico) |
| **Busca por Número** | Tabela Hash (Acesso Direto) | $O(1)$ (Tempo Constante Médio) |
| **Listagem Alfabética** | Percurso In-Ordem na ABB | $O(n)$ |
| **Gerenciamento de Memória** | Percurso Pós-Ordem (Desalocação) | $0$ Bytes Lost (Valgrind Clean) |

---

## 🗂️ Estrutura de Arquivos do Repositório


```

```text
File README.md successfully created!

```bash
├── main.c              # Fluxo principal do programa e interface do Menu
├── agenda.c            # Implementação das funções da ABB, Lista e Tabela Hash
├── agenda.h            # Definição dos TADs (Tipos Abstratos de Dados) e protótipos
├── Makefile            # Automação de compilação do sistema
└── README.md           # Documentação do projeto (este arquivo)

```

---

## 💻 Como Compilar e Executar

### Pré-requisitos

Certifique-se de possuir o compilador `gcc` e a ferramenta `make` (ou utilize o comando manual) instalados no seu ambiente de desenvolvimento (Linux, macOS ou Windows via WSL/Ubuntu).

### Passo 1: Compilação

No terminal do seu sistema operacional, navegue até a pasta do projeto e execute o comando:

```bash
make

```

*Caso não possua o utilitário Make, compile manualmente com o comando:*

```bash
gcc -g main.c agenda.c -o agenda

```

*(O parâmetro `-g` é fundamental para incluir os símbolos de depuração que o Valgrind necessita).*

### Passo 2: Execução Comum

Para rodar o executável gerado:

```bash
./agenda

```

---

## 🛡️ Verificação de Vazamento de Memória (Valgrind)

A gestão correta da memória RAM é um dos pilares de avaliação deste projeto. Como a desalocação de uma estrutura híbrida exige uma ordem estrita (primeiro as listas internas de telefones, depois o nó da árvore e por fim a tabela hash auxiliar), o código foi validado rigorosamente.

Para verificar que **todo byte alocado foi perfeitamente devolvido ao sistema operacional**, utilize o comando abaixo dentro de um terminal Linux/WSL:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./agenda

```

### Comprovação de Sucesso Esperada:

Ao finalizar a execução do menu (Opção `0`), o Valgrind deverá emitir o seguinte relatório final no terminal:

```text
==XXXX== HEAP SUMMARY:
==XXXX==     in use at exit: 0 bytes in 0 blocks
==XXXX==   total heap usage: X allocs, X frees, Y bytes allocated
==XXXX== 
==XXXX== All heap blocks were freed -- no leaks are possible
==XXXX== 
==XXXX== ERROR SUMMARY: 0 errors from 0 contexts

```

---

## 👨‍🎓 Autor

* **Estudante:** Vin Yaro
* **Componente Curricular:** Algoritmos e Estruturas de Dados (AEDS)
* **Instituição:** Curso de Desenvolvimento de Sistemas
* **Orientadora:** Profª. Camille Nader

---

*A engenharia de software não reside em encontrar uma estrutura de dados universal, mas sim em acoplar as ferramentas certas para dominar cada domínio do problema de forma limpa, veloz e estável.*
