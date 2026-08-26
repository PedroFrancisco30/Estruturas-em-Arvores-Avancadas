#include <iostream>
#include <memory>
#include <string>

const int ALFABETO = 26;

typedef struct trieNo{
    trieNo* filhos[ALFABETO];
    bool fim_palavra;

    trieNo(){
        fim_palavra = false;
        for(int i=0;i<ALFABETO;i++){
            filhos[i] = nullptr;
        }
    }
};

class Trie{

private:

trieNo* root = nullptr;

public:
    Trie(trieNo* root){
        root = new trieNo();
    }

    ~Trie(){
        for(int i =0;i<)

    }

    void limparNos(trieNo* node[]){
        if(node == nullptr){
            return 0;
        }

        for(int i=0;i<node[];i++){
            if(node[i] != nullptr){
                
            }
        }

    }

};


int main(){

const std:: string;



    return 0;
}