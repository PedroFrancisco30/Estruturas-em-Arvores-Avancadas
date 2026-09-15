#include <iostream>
#include <string>
#include <vector>

struct splayNo {
    std::string chave;
    splayNo* esquerdo;
    splayNo* direito;
    splayNo* pai;

    splayNo(std::string valor) {
        chave = valor;
        esquerdo = nullptr;
        direito = nullptr;
        pai = nullptr;
    }
};

class SplayTree {
private:
    splayNo* root;

    void rotacaoEsquerda(splayNo* x) {
        splayNo* y = x->direito;
        if (y == nullptr) return;

        x->direito = y->esquerdo;
        if (y->esquerdo != nullptr) {
            y->esquerdo->pai = x;
        }

        y->pai = x->pai;
        if (x->pai == nullptr) {
            root = y;
        } else if (x == x->pai->esquerdo) {
            x->pai->esquerdo = y;
        } else {
            x->pai->direito = y;
        }

        y->esquerdo = x;
        x->pai = y;
    }

    void rotacaoDireita(splayNo* x) {
        splayNo* y = x->esquerdo;
        if (y == nullptr) return;

        x->esquerdo = y->direito;
        if (y->direito != nullptr) {
            y->direito->pai = x;
        }

        y->pai = x->pai;
        if (x->pai == nullptr) {
            root = y;
        } else if (x == x->pai->direito) {
            x->pai->direito = y;
        } else {
            x->pai->esquerdo = y;
        }

        y->direito = x;
        x->pai = y;
    }

    void splay(splayNo* x) {
        while (x->pai != nullptr) {
            if (x->pai->pai == nullptr) {
                // Zig
                if (x == x->pai->esquerdo) {
                    rotacaoDireita(x->pai);
                } else {
                    rotacaoEsquerda(x->pai);
                }
            } else if (x == x->pai->esquerdo && x->pai == x->pai->pai->esquerdo) {
                // Zig-Zig
                rotacaoDireita(x->pai->pai);
                rotacaoDireita(x->pai);
            } else if (x == x->pai->direito && x->pai == x->pai->pai->direito) {
                // Zag-Zag
                rotacaoEsquerda(x->pai->pai);
                rotacaoEsquerda(x->pai);
            } else if (x == x->pai->direito && x->pai == x->pai->pai->esquerdo) {
                // Zig-Zag (Zag-Zig dependendo da nomenclatura, mas significa curva)
                rotacaoEsquerda(x->pai);
                rotacaoDireita(x->pai);
            } else {
                // Zag-Zig
                rotacaoDireita(x->pai);
                rotacaoEsquerda(x->pai);
            }
        }
    }

    splayNo* maximo(splayNo* no) {
        if (no == nullptr) return nullptr;
        while (no->direito != nullptr) {
            no = no->direito;
        }
        return no;
    }

    void limparNos(splayNo* no) {
        if (no == nullptr) return;
        limparNos(no->esquerdo);
        limparNos(no->direito);
        delete no;
    }

    void printTreeRecursivo(splayNo* no, std::string prefix, bool isLeft) {
        if (no != nullptr) {
            std::cout << prefix;
            std::cout << (isLeft ? "├──" : "└──" );
            std::cout << no->chave << std::endl;
            printTreeRecursivo(no->esquerdo, prefix + (isLeft ? "│   " : "    "), true);
            printTreeRecursivo(no->direito, prefix + (isLeft ? "│   " : "    "), false);
        }
    }

public:
    SplayTree() {
        root = nullptr;
    }

    ~SplayTree() {
        limparNos(root);
    }

    void inserir(const std::string& palavra) {
        if (root == nullptr) {
            root = new splayNo(palavra);
            return;
        }

        splayNo* atual = root;
        splayNo* paiDoAtual = nullptr;

        while (atual != nullptr) {
            paiDoAtual = atual;
            if (palavra < atual->chave) {
                atual = atual->esquerdo;
            } else if (palavra > atual->chave) {
                atual = atual->direito;
            } else {
                // Palavra já existe. Como foi acessada, aplicamos o splay.
                splay(atual);
                return;
            }
        }

        splayNo* novoNo = new splayNo(palavra);
        novoNo->pai = paiDoAtual;

        if (palavra < paiDoAtual->chave) {
            paiDoAtual->esquerdo = novoNo;
        } else {
            paiDoAtual->direito = novoNo;
        }

        // A propriedade chave da Splay: jogar para a raiz após acesso/inserção
        splay(novoNo);
    }

    bool buscar(const std::string& palavra) {
        splayNo* atual = root;
        splayNo* ultimo = nullptr; // Rastreia o último nó acessado (para splay no miss)

        while (atual != nullptr) {
            ultimo = atual;
            if (palavra == atual->chave) {
                splay(atual);
                return true;
            } else if (palavra < atual->chave) {
                atual = atual->esquerdo;
            } else {
                atual = atual->direito;
            }
        }

        // Se não achou, faz o splay no último nó visitado
        if (ultimo != nullptr) {
            splay(ultimo);
        }
        return false;
    }

    void remover(const std::string& palavra) {
        if (root == nullptr) return;

        // O buscar joga a palavra (se existir) para a raiz via splay
        if (!buscar(palavra)) {
            return; // Não está na árvore
        }

        // Agora root tem a chave que queremos remover
        splayNo* velhaRaiz = root;

        if (root->esquerdo == nullptr) {
            root = root->direito;
            if (root != nullptr) root->pai = nullptr;
        } else {
            splayNo* subEsquerda = root->esquerdo;
            splayNo* subDireita = root->direito;

            subEsquerda->pai = nullptr;
            root = subEsquerda;
            
            // Encontra o maior da subárvore esquerda e joga pra raiz dela
            splayNo* maxEsq = maximo(subEsquerda);
            splay(maxEsq); // Agora maxEsq é o root (e maxEsq->direito é nullptr, pois ele era o máximo)

            // Gruda a subárvore direita que estava solta
            root->direito = subDireita;
            if (subDireita != nullptr) {
                subDireita->pai = root;
            }
        }

        delete velhaRaiz;
    }

    void printTree() {
        if (root == nullptr) {
            std::cout << "(vazia)" << std::endl;
            return;
        }
        std::cout << root->chave << std::endl;
        printTreeRecursivo(root->esquerdo, "", true);
        printTreeRecursivo(root->direito, "", false);
    }
};

#ifndef EVAL_BENCHMARK
int main() {
    SplayTree arvore;

    std::cout << "--- ESTADO INICIAL (Inserindo a, c, b) ---" << std::endl;
    // O último a ser inserido será a raiz
    arvore.inserir("a");
    arvore.inserir("c");
    arvore.inserir("b");
    arvore.printTree();

    std::cout << "\n--- ESTADO INTERMEDIÁRIO (Buscando 'a') ---" << std::endl;
    // A busca força um Splay, reorganizando 'a' para a raiz
    arvore.buscar("a");
    arvore.printTree();

    std::cout << "\n--- ESTADO APÓS REMOÇÃO (Removendo 'a') ---" << std::endl;
    arvore.remover("a");
    arvore.printTree();

    return 0;
}
#endif