#include <string>
#include <iostream>
#include <algorithm> // para std::min

const int ALFABETO = 26;

struct patriciaNo{
    std::string prefixo;
    patriciaNo* filhos[ALFABETO];
    bool fim_palavra;

    patriciaNo(std::string texto){
        prefixo = texto;
        fim_palavra = false;
        
        for(int i = 0; i < ALFABETO; i++){
            filhos[i] = nullptr;
        }
    }
};

class PatriciaTree {
private:    
    patriciaNo* root;
    
    int idxFilho(const std::string& resto) {
        return resto[0] - 'a';
    }
    
    int prefixoComum(const std::string& s1, const std::string& s2) {
        int i = 0;
        int tamanhoMenor = std::min(s1.length(), s2.length());
        while (i < tamanhoMenor && s1[i] == s2[i]) {
            i++;
        }
        return i; 
    }

    void limparNos(patriciaNo* node) {
        if (node == nullptr) {
            return;
        }
        for (int i = 0; i < ALFABETO; i++) {
            if (node->filhos[i] != nullptr) {
                limparNos(node->filhos[i]);
            }
        }
        delete node;
    }

    void inserirRecursivo(patriciaNo* atual, const std::string& resto) {
        int idx = idxFilho(resto);

        // Caso 1: Caminho livre
        if (atual->filhos[idx] == nullptr) {
            patriciaNo* novoNo = new patriciaNo(resto);
            novoNo->fim_palavra = true;
            atual->filhos[idx] = novoNo;
            return;
        }

        // Caminho ocupado, precisamos comparar
        patriciaNo* filho = atual->filhos[idx];
        int lcp = prefixoComum(filho->prefixo, resto);

        // Caso 2: O prefixo do filho está totalmente contido no resto
        if (lcp == filho->prefixo.length()) {
            if (lcp == resto.length()) {
                // Encaixe perfeito! A palavra termina exatamente aqui
                filho->fim_palavra = true;
            } else {
                // A palavra é maior, continua descendo
                inserirRecursivo(filho, resto.substr(lcp));
            }
        } 
        // Caso 3: O Split (Bifurcação)
        else {
            // Cria o novo nó que vai ficar no meio
            patriciaNo* novoNo = new patriciaNo(resto.substr(0, lcp));
            
            // O filho velho perde a parte comum
            filho->prefixo = filho->prefixo.substr(lcp);
            
            // Pendura o filho velho no novo nó
            novoNo->filhos[idxFilho(filho->prefixo)] = filho;

            // Lida com o resto da nova palavra
            if (lcp == resto.length()) {
                novoNo->fim_palavra = true;
            } else {
                novoNo->fim_palavra = false;
                patriciaNo* novoFilho = new patriciaNo(resto.substr(lcp));
                novoFilho->fim_palavra = true;
                novoNo->filhos[idxFilho(novoFilho->prefixo)] = novoFilho;
            }

            // O pai agora aponta para o novo nó do meio
            atual->filhos[idx] = novoNo;
        }
    }

    bool buscarRecursivo(patriciaNo* atual, const std::string& resto) {
        int idx = idxFilho(resto);
        patriciaNo* filho = atual->filhos[idx];

        if (filho == nullptr) {
            return false;
        }

        int lcp = prefixoComum(filho->prefixo, resto);

        if (lcp < filho->prefixo.length()) {
            return false; // Divergiu no meio do prefixo do nó
        }

        if (lcp == filho->prefixo.length() && lcp == resto.length()) {
            return filho->fim_palavra; // Encaixe exato
        }

        if (lcp == filho->prefixo.length() && lcp < resto.length()) {
            return buscarRecursivo(filho, resto.substr(lcp)); // Continua descendo
        }

        return false;
    }

    bool removerRecursivo(patriciaNo* atual, const std::string& resto) {
        int idx = idxFilho(resto);
        patriciaNo* filho = atual->filhos[idx];

        if (filho == nullptr) {
            return true; // Não achou, não faz nada
        }

        int lcp = prefixoComum(filho->prefixo, resto);

        if (lcp < filho->prefixo.length()) {
            return true; // Divergiu, não achou
        }

        if (lcp == filho->prefixo.length() && lcp == resto.length()) {
            // Achou o nó exato! Remove a bandeirinha
            filho->fim_palavra = false;
        } else {
            // Continua descendo
            removerRecursivo(filho, resto.substr(lcp));
        }

        // Lógica de limpeza (Garbage Collection & Merge)
        int filhos_count = 0;
        patriciaNo* unicoNeto = nullptr;

        for (int i = 0; i < ALFABETO; i++) {
            if (filho->filhos[i] != nullptr) {
                filhos_count++;
                unicoNeto = filho->filhos[i];
            }
        }

        if (filho->fim_palavra == false && filhos_count == 0) {
            // Inútil e sem filhos -> Apaga da memória
            delete filho;
            atual->filhos[idx] = nullptr;
        } 
        else if (filho->fim_palavra == false && filhos_count == 1) {
            // Inútil mas com 1 filho -> Merge (Funde com o neto)
            unicoNeto->prefixo = filho->prefixo + unicoNeto->prefixo;
            atual->filhos[idx] = unicoNeto;
            delete filho;
        }
        
        return true;
    }

    void imprimirRecursivamente(patriciaNo* atual, std::string palavraAcumulada) { 
        if (atual == nullptr) return;

        std::string palavraDesseNo = palavraAcumulada + atual->prefixo;

        if (atual->fim_palavra) {
            std::cout << "- " << palavraDesseNo << std::endl;
        }

        for (int i = 0; i < ALFABETO; i++) {
            if (atual->filhos[i] != nullptr) {
                imprimirRecursivamente(atual->filhos[i], palavraDesseNo);
            }
        }
    }

public:
    PatriciaTree() {
        root = new patriciaNo(""); 
    }
    
    ~PatriciaTree() {
        limparNos(root);
    }
    
    void inserir(const std::string& palavra) {
        if (palavra.empty()) return;
        inserirRecursivo(root, palavra);
    }
    
    bool buscar(const std::string& palavra) {
        if (palavra.empty()) return false;
        return buscarRecursivo(root, palavra);
    }
    
    void remover(const std::string& palavra) {
        if (palavra.empty()) return;
        removerRecursivo(root, palavra);
    }
    
    void imprimirTodas() {
        std::cout << "Palavras na Patricia:" << std::endl;
        imprimirRecursivamente(root, "");
    }
};


int main(){
    PatriciaTree arvore;

    arvore.inserir("algoritmo");
    arvore.inserir("algodao");
    arvore.inserir("arvore");

    std::cout << "Busca 'algoritmo': " << arvore.buscar("algoritmo") << std::endl; // 1
    std::cout << "Busca 'algo': " << arvore.buscar("algo") << std::endl;           // 0

    std::cout << "\n--- ANTES DA REMOCAO ---" << std::endl;
    arvore.imprimirTodas();

    std::cout << "\nRemovendo 'algoritmo'..." << std::endl;
    arvore.remover("algoritmo");

    std::cout << "\n--- DEPOIS DA REMOCAO ---" << std::endl;
    arvore.imprimirTodas();

    return 0;
}