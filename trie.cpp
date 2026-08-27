#include <iostream>
#include <string>

const int ALFABETO = 26;

struct trieNo {
    trieNo* filhos[ALFABETO];
    bool fim_palavra;

    trieNo() {
        fim_palavra = false;
        for (int i = 0; i < ALFABETO; i++) {
            filhos[i] = nullptr;
        }
    }
};

class Trie {
private:
    trieNo* root;

    // Função auxiliar privada para o destrutor (recursiva)
    void limparNos(trieNo* node) {
        if (node == nullptr) {
            return;
        }
        
        // 1. Percorre todos os 26 possíveis filhos
        for (int i = 0; i < ALFABETO; i++) {
            if (node->filhos[i] != nullptr) {
                // 2. Chama a limpeza recursivamente para os filhos
                limparNos(node->filhos[i]);
            }
        }
        // 3. Depois que todos os filhos foram deletados, deleta o nó atual
        delete node;
    }


    void imprimirRecursivamente(trieNo* atual,std::string palavraAcumulada){
        if (atual == nullptr){
            return;
        }

        if(atual->fim_palavra){
            std::cout << "." << palavraAcumulada << std::endl;
        }

        for(int i=0;i<ALFABETO;i++){
            if(atual->filhos != nullptr){
                char letraDaPorta = i+'a';
                imprimirRecursivamente(atual->filhos[i], palavraAcumulada + letraDaPorta);
            }
        }
    }

    bool removerRecursivamente(){}

public:
    // Construtor: Inicializa a raiz vazia
    Trie() {
        root = new trieNo();
    }

    // Destrutor: Chama a função recursiva para limpar toda a árvore e evitar memory leak
    ~Trie() {
        limparNos(root);
    }

    // Método de Inserção
    void inserir(const std::string& palavra) {
        trieNo* atual = root;

        for (int i = 0; i < palavra.length(); i++) {
            char letra = palavra[i];
            
            // Converte a letra (ex: 'a' a 'z') em um índice (0 a 25)
            // Assumindo que todas as palavras sejam minúsculas!
            int indice = letra - 'a'; //subtraindo por causa da tabela ascii
        
            if(atual->filhos[indice] == nullptr){
                atual->filhos[indice] = new trieNo();   
            }

            atual = atual->filhos[indice];
            
        }
        atual->fim_palavra = true;//falando que acabou a palavra
    }

    // Método de Busca
    bool buscar(const std::string& palavra) {
        trieNo* atual = root;

        for (int i = 0; i < palavra.length(); i++) {
            char letra = palavra[i];
            int indice = letra - 'a';

            if(atual->filhos[indice] == nullptr){
                return false;
            }else{
                atual = atual->filhos[indice]; 
            }
        }

        return atual->fim_palavra;
    }

    void remover(const std::string& palavra){
        removerRecursivamente(root,palavra,0);
    }        



    void printar(){
        std::cout << "Palavras na trie:" << std::endl;
        imprimirRecursivamente(root,"");
    }


};

int main() {
    Trie arvore;

    arvore.inserir("algoritmo");
    arvore.inserir("algodao");
    arvore.inserir("arvore");

    std::cout << "Busca 'algoritmo': " << arvore.buscar("algoritmo") << std::endl; // Deve imprimir 1 (true)
    std::cout << "Busca 'algo': " << arvore.buscar("algo") << std::endl;           // Deve imprimir 0 (false)

    return 0;
}