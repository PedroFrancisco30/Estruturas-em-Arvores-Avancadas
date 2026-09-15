#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // para std::min

struct Ponto {
    double x;
    double y;

    // Construtor utilitário
    Ponto(double _x = 0, double _y = 0) : x(_x), y(_y) {}

    bool operator==(const Ponto& outro) const {
        return x == outro.x && y == outro.y;
    }
};

struct kdNo {
    Ponto ponto;
    kdNo* esquerdo;
    kdNo* direito;

    kdNo(Ponto p) : ponto(p), esquerdo(nullptr), direito(nullptr) {}
};

class KDTree {
private:
    kdNo* root;

    // Função utilitária que compara um ponto dependendo da profundidade (eixo X = 0, eixo Y = 1)
    bool isMenor(Ponto p1, Ponto p2, int profundidade) {
        if (profundidade % 2 == 0) {
            return p1.x < p2.x;
        } else {
            return p1.y < p2.y;
        }
    }

    kdNo* inserirRecursivo(kdNo* no, Ponto p, int profundidade) {
        if (no == nullptr) {
            return new kdNo(p);
        }

        if (no->ponto == p) {
            return no; // Evita duplicatas exatas
        }

        if (isMenor(p, no->ponto, profundidade)) {
            no->esquerdo = inserirRecursivo(no->esquerdo, p, profundidade + 1);
        } else {
            no->direito = inserirRecursivo(no->direito, p, profundidade + 1);
        }

        return no;
    }

    bool buscarRecursivo(kdNo* no, Ponto p, int profundidade) {
        if (no == nullptr) return false;
        
        if (no->ponto == p) return true;

        if (isMenor(p, no->ponto, profundidade)) {
            return buscarRecursivo(no->esquerdo, p, profundidade + 1);
        } else {
            return buscarRecursivo(no->direito, p, profundidade + 1);
        }
    }

    // Auxiliar para a remoção: Encontrar o mínimo em uma dimensão específica 'dim' (0 para X, 1 para Y)
    kdNo* encontrarMinimo(kdNo* x, kdNo* y, kdNo* z, int dim) {
        kdNo* res = x;
        if (y != nullptr) {
            if ((dim == 0 && y->ponto.x < res->ponto.x) || (dim == 1 && y->ponto.y < res->ponto.y))
                res = y;
        }
        if (z != nullptr) {
            if ((dim == 0 && z->ponto.x < res->ponto.x) || (dim == 1 && z->ponto.y < res->ponto.y))
                res = z;
        }
        return res;
    }

    kdNo* findMinRecursivo(kdNo* no, int dim, int profundidade) {
        if (no == nullptr) return nullptr;

        int dimAtual = profundidade % 2;

        // Se a dimensão que corta este nó for a dimensão que queremos minimizar, 
        // o menor valor só pode estar na subárvore esquerda (se existir) ou no próprio nó.
        if (dimAtual == dim) {
            if (no->esquerdo == nullptr) return no;
            return findMinRecursivo(no->esquerdo, dim, profundidade + 1);
        }

        // Se a dimensão for diferente, o mínimo pode estar em qualquer lugar:
        // no próprio nó, na esquerda ou na direita.
        return encontrarMinimo(no, 
                               findMinRecursivo(no->esquerdo, dim, profundidade + 1),
                               findMinRecursivo(no->direito, dim, profundidade + 1), 
                               dim);
    }

    kdNo* removerRecursivo(kdNo* no, Ponto p, int profundidade) {
        if (no == nullptr) return nullptr;

        int dimAtual = profundidade % 2;

        if (no->ponto == p) {
            // Caso 1: Nó folha
            if (no->esquerdo == nullptr && no->direito == nullptr) {
                delete no;
                return nullptr;
            }

            // Caso 2: Tem subárvore direita
            // Achamos o mínimo da subárvore direita na dimensão atual,
            // copiamos o valor para este nó e removemos o mínimo de lá.
            if (no->direito != nullptr) {
                kdNo* min = findMinRecursivo(no->direito, dimAtual, profundidade + 1);
                no->ponto = min->ponto;
                no->direito = removerRecursivo(no->direito, min->ponto, profundidade + 1);
            } 
            // Caso 3: Tem subárvore esquerda, mas não direita
            // Achamos o mínimo da subárvore esquerda, copiamos o valor,
            // e como não podemos deixar na esquerda (violaria a propriedade da KD-Tree se houver repetição),
            // movemos a subárvore esquerda para a direita.
            else {
                kdNo* min = findMinRecursivo(no->esquerdo, dimAtual, profundidade + 1);
                no->ponto = min->ponto;
                no->direito = removerRecursivo(no->esquerdo, min->ponto, profundidade + 1);
                no->esquerdo = nullptr;
            }
            return no;
        }

        // Se não for o nó atual, desce na árvore buscando
        if (isMenor(p, no->ponto, profundidade)) {
            no->esquerdo = removerRecursivo(no->esquerdo, p, profundidade + 1);
        } else {
            no->direito = removerRecursivo(no->direito, p, profundidade + 1);
        }

        return no;
    }

    void limparNos(kdNo* no) {
        if (no == nullptr) return;
        limparNos(no->esquerdo);
        limparNos(no->direito);
        delete no;
    }

    void printTreeRecursivo(kdNo* no, std::string prefix, bool isLeft, int profundidade) {
        if (no != nullptr) {
            std::cout << prefix;
            std::cout << (isLeft ? "├──" : "└──" );
            
            // Mostra por qual eixo esse nível está dividindo (X ou Y)
            char eixo = (profundidade % 2 == 0) ? 'X' : 'Y';
            std::cout << "[" << eixo << "] (" << no->ponto.x << ", " << no->ponto.y << ")" << std::endl;
            
            printTreeRecursivo(no->esquerdo, prefix + (isLeft ? "│   " : "    "), true, profundidade + 1);
            printTreeRecursivo(no->direito, prefix + (isLeft ? "│   " : "    "), false, profundidade + 1);
        }
    }

public:
    KDTree() {
        root = nullptr;
    }

    ~KDTree() {
        limparNos(root);
    }

    void inserir(double x, double y) {
        root = inserirRecursivo(root, Ponto(x, y), 0);
    }

    bool buscar(double x, double y) {
        return buscarRecursivo(root, Ponto(x, y), 0);
    }

    void remover(double x, double y) {
        root = removerRecursivo(root, Ponto(x, y), 0);
    }

    void printTree() {
        if (root == nullptr) {
            std::cout << "(vazia)" << std::endl;
            return;
        }
        std::cout << "[X] (" << root->ponto.x << ", " << root->ponto.y << ")" << std::endl;
        printTreeRecursivo(root->esquerdo, "", true, 1);
        printTreeRecursivo(root->direito, "", false, 1);
    }
};

#ifndef EVAL_BENCHMARK
int main() {
    KDTree arvore;

    std::cout << "--- ESTADO INICIAL (Inserindo pontos 2D) ---" << std::endl;
    // Padrão de eixos: X na raiz (nível 0), Y no nível 1, X no nível 2...
    arvore.inserir(30, 40);
    arvore.inserir(5, 25);
    arvore.inserir(70, 70);
    arvore.inserir(10, 12);
    arvore.inserir(50, 30);
    arvore.inserir(35, 45);
    arvore.printTree();

    std::cout << "\nBusca (50, 30): " << (arvore.buscar(50, 30) ? "Encontrado" : "Não encontrado") << std::endl;
    std::cout << "Busca (10, 10): " << (arvore.buscar(10, 10) ? "Encontrado" : "Não encontrado") << std::endl;

    std::cout << "\n--- ESTADO APÓS REMOÇÃO (Removendo a raiz 30, 40) ---" << std::endl;
    // Remover a raiz é a operação mais complexa, forçará a árvore a encontrar o mínimo no eixo X
    arvore.remover(30, 40);
    arvore.printTree();

    return 0;
}
#endif
