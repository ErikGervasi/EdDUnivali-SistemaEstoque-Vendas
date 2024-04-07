#ifndef LUE_H
#define LUE_H

#include <iostream>
using namespace std;

// Estrutura para representar um nó da lista encadeada
struct No {
    char info;
    No *elo = nullptr;
};

// Estrutura para representar a lista encadeada
struct LUE {
    No *comeco = nullptr;
    No *fim = nullptr;
};

// Estrutura para armazenar informações sobre os produtos
struct Produto {
    char codigo;
    string nome;
    float preco;
    int quantidade;
};

// Estrutura para representar um nó de produto na lista encadeada
struct NoProduto {
    Produto *produto;
    NoProduto *elo = nullptr;
};

// Função para inserir um novo nó no final da lista encadeada
bool inserirFinalLue(LUE &lista, NoProduto* novo);

// Função para liberar a memória alocada para a lista encadeada
void liberarLue(LUE &lista);

// Função para buscar um valor na lista encadeada
bool buscarLue(LUE lista, char valor);

// Função para retirar um valor da lista encadeada
bool retirarLue(LUE &lista, char valor);

#endif // LUE_H
