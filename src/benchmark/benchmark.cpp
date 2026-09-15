#define EVAL_BENCHMARK 1

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

#include "../trie/trie.cpp"
#include "../patricia/patricia.cpp"
#include "../splay/splay.cpp"
#include "../treap/treap.cpp"
#include "../kd_tree/kd.cpp"

using namespace std;
using namespace std::chrono;

// Gera string aleatória de tamanho fixo
string randomString(int length) {
    string str("abcdefghijklmnopqrstuvwxyz");
    string result;
    for (int i = 0; i < length; i++) {
        result += str[rand() % 26];
    }
    return result;
}

// Medidor de tempo universal (retorna em microsegundos)
template <typename Func>
long long medirTempo(Func funcao) {
    auto start = high_resolution_clock::now();
    funcao();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}

int main() {
    srand(42); // Seed fixa para reproducibilidade
    
    // Tamanhos de entrada a serem testados
    vector<int> tamanhos = {10000, 50000, 100000};
    
    ofstream file("resultados_benchmark.csv");
    file << "Estrutura,Operacao,Tamanho,Tempo_Microsegundos\n";

    for (int n : tamanhos) {
        cout << "Gerando dados para N = " << n << "..." << endl;
        vector<string> palavras;
        for (int i = 0; i < n; i++) {
            palavras.push_back(randomString(8));
        }

        // ----------- SPLAY TREE -----------
        SplayTree splay;
        long long tempoInsercao = medirTempo([&]() {
            for (const string& p : palavras) {
                splay.inserir(p);
            }
        });
        file << "Splay,Insercao," << n << "," << tempoInsercao << "\n";

        long long tempoBusca = medirTempo([&]() {
            for (const string& p : palavras) {
                splay.buscar(p);
            }
        });
        file << "Splay,Busca," << n << "," << tempoBusca << "\n";

        // ----------- TREAP -----------
        Treap treap;
        tempoInsercao = medirTempo([&]() {
            for (const string& p : palavras) {
                treap.inserir(p);
            }
        });
        file << "Treap,Insercao," << n << "," << tempoInsercao << "\n";

        tempoBusca = medirTempo([&]() {
            for (const string& p : palavras) {
                treap.buscar(p);
            }
        });
        file << "Treap,Busca," << n << "," << tempoBusca << "\n";

        // ----------- TRIE -----------
        Trie trie;
        tempoInsercao = medirTempo([&]() {
            for (const string& p : palavras) {
                trie.inserir(p);
            }
        });
        file << "Trie,Insercao," << n << "," << tempoInsercao << "\n";

        tempoBusca = medirTempo([&]() {
            for (const string& p : palavras) {
                trie.buscar(p);
            }
        });
        file << "Trie,Busca," << n << "," << tempoBusca << "\n";

        // ----------- PATRICIA -----------
        PatriciaTree patricia;
        tempoInsercao = medirTempo([&]() {
            for (const string& p : palavras) {
                patricia.inserir(p);
            }
        });
        file << "Patricia,Insercao," << n << "," << tempoInsercao << "\n";

        tempoBusca = medirTempo([&]() {
            for (const string& p : palavras) {
                patricia.buscar(p);
            }
        });
        file << "Patricia,Busca," << n << "," << tempoBusca << "\n";

        // ----------- KD TREE -----------
        KDTree kd;
        // Para a KD Tree, converteremos os 2 primeiros chars e os 2 seguintes em coords
        vector<pair<double, double>> pontos;
        for (const string& p : palavras) {
            double x = p[0] * 100 + p[1];
            double y = p[2] * 100 + p[3];
            pontos.push_back({x, y});
        }
        
        tempoInsercao = medirTempo([&]() {
            for (auto& pt : pontos) {
                kd.inserir(pt.first, pt.second);
            }
        });
        file << "KDTree,Insercao," << n << "," << tempoInsercao << "\n";

        tempoBusca = medirTempo([&]() {
            for (auto& pt : pontos) {
                kd.buscar(pt.first, pt.second);
            }
        });
        file << "KDTree,Busca," << n << "," << tempoBusca << "\n";
    }

    file.close();
    cout << "Benchmark concluído! Resultados salvos em resultados_benchmark.csv" << endl;
    return 0;
}
