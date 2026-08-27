#include <string>
#include <iostream>

const int ALFABETO = 26;

struct patriciaNo{

    std::string prefixo;
    patriciaNo* filhos[ALFABETO];
    bool fim_palavra;

    patriciaNo(std::string texto){
        prefixo = texto;

        fim_palavra = false;

        for(int i=0;i<ALFABETO;i++){
            filhos[i] = nullptr;
        }
    }
};


class PatriciaTree {
private:
    patriciaNo* root;
    
    int prefixoComum(const std::string& s1, const std::string& s2) {
        
        return 0; 
    }

    void limparNos(patriciaNo* node) {}
    
    void imprimirRecursivamente(patriciaNo* atual, std::string palavraAcumulada) { 
        
    }

    bool removerRecursivamente(patriciaNo* atual, const std::string& palavra, int profundidade) {
     
        return false;
    }

    public:
    PatriciaTree() {
        
        root = new patriciaNo(""); 
    }
    ~PatriciaTree() {
        limparNos(root);
    }
    void inserir(const std::string& palavra) {
   
    }
    bool buscar(const std::string& palavra) {
       
        return false;
    }
    void remover(const std::string& palavra) {
        
    }
    void imprimirTodas() {
        std::cout << "Palavras na Patricia:" << std::endl;
        imprimirRecursivamente(root, "");
    }
};



int main(){

    return 0;
}