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
            std::cout << "-" << palavraAcumulada << std::endl;
        }

        for(int i=0;i<ALFABETO;i++){
            if(atual->filhos[i] != nullptr){
                char letraDaPorta = i+'a';
                imprimirRecursivamente(atual->filhos[i], palavraAcumulada + letraDaPorta);
            }
        }
    }

// Retorna true se o 'atual' puder ser deletado pelo pai
    bool removerRecursivamente(trieNo* atual, const std::string& palavra, int profundidade) {
        
        // 1. Caso base: chegamos no fundo!
        if (profundidade == palavra.length()) {
            if (atual->fim_palavra) {
                atual->fim_palavra = false;
            }
            // Checando se tem filhos (sem cair na armadilha do else!)
            for (int i = 0; i < ALFABETO; i++) {
                if (atual->filhos[i] != nullptr) {
                    return false; // Opa, achei um filho vivo! Não posso morrer.
                }
            }
            
            // Se o for rodou inteiro e não deu 'return false', 
            // é porque não tem filhos. Posso morrer!
            return true; 
        }
        // 2. Caminhando para baixo
        int indice = palavra[profundidade] - 'a';
        if (atual->filhos[indice] == nullptr) {
            return false;
        }
        // 3. Mergulha!
        bool filhoAvisouPraApagar = removerRecursivamente(atual->filhos[indice], palavra, profundidade + 1);
        // 4. Subindo de volta
        if (filhoAvisouPraApagar) {
            delete atual->filhos[indice];
            atual->filhos[indice] = nullptr; 
            
            // Será que eu (atual) também me tornei inútil?
            if (atual->fim_palavra == false) {
                
                // Mesma lógica: vamos ver se sobrou algum outro filho vivo
                for (int i = 0; i < ALFABETO; i++) {
                    if (atual->filhos[i] != nullptr) {
                        return false; // Sobrou outro filho, não posso morrer.
                    }
                }
                
                // Se rodou o for inteiro e não tem mais nenhum filho, morre também!
                return true; 
            }
        }
        
        return false;
    }
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

    std::cout << "\n--- ANTES DA REMOCAO ---" << std::endl;
    arvore.printar(); // Vai imprimir algoritmo, algodao e arvore

    std::cout << "\nRemovendo 'algodao'..." << std::endl;
    arvore.remover("algodao");
    
    std::cout << "\n--- DEPOIS DA REMOCAO ---" << std::endl;
    arvore.printar(); // Vai imprimir apenas algoritmo e arvore
    return 0;
}