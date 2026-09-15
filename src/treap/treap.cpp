#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

struct treapNo {
    std::string chave;
    int prioridade;
    treapNo* esquerdo;
    treapNo* direito;

    treapNo(std::string valor) {
        chave = valor;
        prioridade = rand(); // Prioridade aleatória para balanceamento probabilístico
        esquerdo = nullptr;
        direito = nullptr;
    }
};

class Treap {
private:
    treapNo* root;

    // Rotação à direita (zig)
    treapNo* rotacaoDireita(treapNo* y) {
        treapNo* x = y->esquerdo;
        treapNo* T2 = x->direito;

        x->direito = y;
        y->esquerdo = T2;

        return x;
    }

    // Rotação à esquerda (zag)
    treapNo* rotacaoEsquerda(treapNo* x) {
        treapNo* y = x->direito;
        treapNo* T2 = y->esquerdo;

        y->esquerdo = x;
        x->direito = T2;

        return y;
    }

    treapNo* inserirRecursivo(treapNo* no, const std::string& palavra) {
        // Passo 1: Inserção normal de BST
        if (no == nullptr) {
            return new treapNo(palavra);
        }

        if (palavra < no->chave) {
            no->esquerdo = inserirRecursivo(no->esquerdo, palavra);
            
            // Passo 2: Conserta propriedade de Max-Heap (pai deve ter maior prioridade)
            if (no->esquerdo->prioridade > no->prioridade) {
                no = rotacaoDireita(no);
            }
        } else if (palavra > no->chave) {
            no->direito = inserirRecursivo(no->direito, palavra);
            
            // Conserta propriedade de Max-Heap
            if (no->direito->prioridade > no->prioridade) {
                no = rotacaoEsquerda(no);
            }
        }
        // Se a palavra for igual, não inserimos duplicatas
        return no;
    }

    treapNo* removerRecursivo(treapNo* no, const std::string& palavra) {
        if (no == nullptr) return no;

        // Se a palavra for menor, desce pra esquerda
        if (palavra < no->chave) {
            no->esquerdo = removerRecursivo(no->esquerdo, palavra);
        } 
        // Se a palavra for maior, desce pra direita
        else if (palavra > no->chave) {
            no->direito = removerRecursivo(no->direito, palavra);
        } 
        // Encontramos o nó a ser removido
        else {
            // Caso 1: Folha
            if (no->esquerdo == nullptr && no->direito == nullptr) {
                delete no;
                no = nullptr;
            } 
            // Caso 2: Apenas um filho
            else if (no->esquerdo == nullptr) {
                treapNo* temp = no->direito;
                delete no;
                no = temp;
            } else if (no->direito == nullptr) {
                treapNo* temp = no->esquerdo;
                delete no;
                no = temp;
            } 
            // Caso 3: Dois filhos - Empurra o nó pra baixo rotacionando pro lado do filho com maior prioridade
            else {
                if (no->esquerdo->prioridade > no->direito->prioridade) {
                    no = rotacaoDireita(no);
                    no->direito = removerRecursivo(no->direito, palavra);
                } else {
                    no = rotacaoEsquerda(no);
                    no->esquerdo = removerRecursivo(no->esquerdo, palavra);
                }
            }
        }
        return no;
    }

    bool buscarRecursivo(treapNo* no, const std::string& palavra) {
        if (no == nullptr) return false;
        
        if (no->chave == palavra) return true;
        
        if (palavra < no->chave) {
            return buscarRecursivo(no->esquerdo, palavra);
        } else {
            return buscarRecursivo(no->direito, palavra);
        }
    }

    void limparNos(treapNo* no) {
        if (no == nullptr) return;
        limparNos(no->esquerdo);
        limparNos(no->direito);
        delete no;
    }

    void printTreeRecursivo(treapNo* no, std::string prefix, bool isLeft) {
        if (no != nullptr) {
            std::cout << prefix;
            std::cout << (isLeft ? "├──" : "└──" );
            std::cout << no->chave << "(" << no->prioridade << ")" << std::endl;
            printTreeRecursivo(no->esquerdo, prefix + (isLeft ? "│   " : "    "), true);
            printTreeRecursivo(no->direito, prefix + (isLeft ? "│   " : "    "), false);
        }
    }

public:
    Treap() {
        // Inicializa a semente randômica para as prioridades
        srand(time(nullptr));
        root = nullptr;
    }

    ~Treap() {
        limparNos(root);
    }

    void inserir(const std::string& palavra) {
        root = inserirRecursivo(root, palavra);
    }

    void remover(const std::string& palavra) {
        root = removerRecursivo(root, palavra);
    }

    bool buscar(const std::string& palavra) {
        return buscarRecursivo(root, palavra);
    }

    void printTree() {
        if (root == nullptr) {
            std::cout << "(vazia)" << std::endl;
            return;
        }
        std::cout << root->chave << "(" << root->prioridade << ")" << std::endl;
        printTreeRecursivo(root->esquerdo, "", true);
        printTreeRecursivo(root->direito, "", false);
    }
};

#ifndef EVAL_BENCHMARK
int main() {
    Treap arvore;

    std::cout << "--- ESTADO INICIAL (Inserindo palavras) ---" << std::endl;
    // O formato será Nome(Prioridade). O mais prioritário sobe!
    arvore.inserir("banana");
    arvore.inserir("abacaxi");
    arvore.inserir("cebola");
    arvore.inserir("damasco");
    arvore.printTree();

    std::cout << "\n--- ESTADO INTERMEDIÁRIO (Inserindo 'ameixa') ---" << std::endl;
    arvore.inserir("ameixa");
    arvore.printTree();

    std::cout << "\n--- ESTADO APÓS REMOÇÃO (Removendo 'banana') ---" << std::endl;
    // Banana será rotacionada para baixo até virar folha e então removida
    arvore.remover("banana");
    arvore.printTree();

    return 0;
}
#endif
