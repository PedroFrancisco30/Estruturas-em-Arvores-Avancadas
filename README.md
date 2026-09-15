# Estudo Comparativo de Estruturas em Árvores Avançadas 🌳

Trabalho Prático desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados II (AEDS II)** no **CEFET-MG**.

## 📌 Sobre o Projeto
Este repositório contém a implementação em **C++** de cinco estruturas de dados hierárquicas avançadas:
1. **Trie (Árvore de Prefixos)**
2. **Árvore Patricia (Radix Tree compacta)**
3. **Árvore Splay**
4. **Treap (Tree + Heap)**
5. **KD-Tree (Bidimensional)**

O objetivo do projeto é analisar empírica e teoricamente as operações de **Busca, Inserção e Remoção** dessas estruturas, comparando seu tempo de execução e consumo de memória contra estruturas clássicas como a BST e AVL.

## 📂 Estrutura do Repositório
- `src/` - Código-fonte individual de cada árvore separada por pastas.
- `src/benchmark/` - Sistema de testes automatizados para medir o tempo de execução (10k, 50k e 100k elementos).
- `resultados/` - Dados tabulados em `.csv` gerados pelo benchmark.
- `docs/` - Relatório Técnico final em formato acadêmico (IEEEtran) com os diagramas de rastreamento visual.

## 🚀 Como Compilar e Rodar

O projeto foi construído sem dependências externas complexas. Para rodar a bateria de testes de performance (Benchmark), você precisará apenas do compilador G++:

```bash
# Navegue até a raiz do projeto
cd "Trabalho 1"

# Compile o arquivo de benchmark com a flag de otimização máxima
g++ src/benchmark/benchmark.cpp -o benchmark -O3

# Execute
./benchmark
```
*O executável gerará (ou sobrescreverá) o arquivo `resultados_benchmark.csv` na pasta `resultados`.*

## 📄 Autoria
- **Pedro Francisco Sousa Silva** - *Engenharia da Computação - CEFET-MG* (pedrofrancisco3010@gmail.com)
