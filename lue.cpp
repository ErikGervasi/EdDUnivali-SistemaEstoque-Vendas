#include "lue.h"

bool inserirFinalLue(LUE &lista, NoProduto* novo) {
    if (novo == nullptr) return false;

    if (lista.comeco == nullptr) {
        lista.comeco = reinterpret_cast<No*>(novo);
        lista.fim = reinterpret_cast<No*>(novo);
    } else {
        lista.fim->elo = reinterpret_cast<No*>(novo);
        lista.fim = reinterpret_cast<No*>(novo);
    }
    return true;
}

void liberarLue(LUE &lista) {
    No *aux = lista.comeco;
    No *prox;

    while (aux != nullptr) {
        prox = aux->elo;
        delete aux;
        aux = prox;
    }
    lista.comeco = nullptr;
    lista.fim = nullptr;
}

bool buscarLue(LUE lista, char valor) {
    No *aux = lista.comeco;

    while (aux != nullptr) {
        if (aux->info == valor) return true;
        aux = aux->elo;
    }
    return false;
}

bool retirarLue(LUE &lista, char valor) {
    if (lista.comeco == nullptr) return false;

    No *aux = lista.comeco;
    No *ant = nullptr;
    while (aux != nullptr && valor != aux->info) {
        ant = aux;
        aux = aux->elo;
    }
    if (aux == nullptr) return false;

    if (aux == lista.comeco && aux == lista.fim) { // Caso 1
        lista.comeco = nullptr;
        lista.fim = nullptr;
        delete aux;
        return true;
    }
    if (aux == lista.comeco) { // Caso 2
        lista.comeco = aux->elo;
        delete aux;
        return true;
    }
    if (aux == lista.fim) { // Caso 3
        ant->elo = nullptr;
        lista.fim = ant;
        delete aux;
        return true;
    }
    // Caso 4
    ant->elo = aux->elo;
    delete aux;
    return true;
}
