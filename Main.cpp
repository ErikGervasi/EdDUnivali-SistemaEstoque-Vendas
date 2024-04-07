#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <windows.h>

using namespace std;

struct Produto {
    string codigo;  // Modificado de char para string
    string nome;
    float preco;
    int quantidade;
};

struct NoProduto {
    Produto* produto;
    NoProduto* elo = nullptr;
};

struct LUE {
    NoProduto* comeco = nullptr;
    NoProduto* fim = nullptr;
};

struct ItemCompra {
    Produto* produto;
    int quantidade;
    ItemCompra* proximo = nullptr;
};

struct ListaCompras {
    ItemCompra* primeiro = nullptr;
    ItemCompra* ultimo = nullptr;

    ~ListaCompras() {
        ItemCompra* atual = primeiro;
        while (atual != nullptr) {
            ItemCompra* temp = atual;
            atual = atual->proximo;
            delete temp;
        }
    }
};

struct Caixa {
    string idVendedor;
    float total = 0.0;
    float desconto = 0.0;
    string formaPagamento;
};

struct RegistroVenda {
    float valorTotal;
    float desconto;
    string formaPagamento;
    string idVendedor;

    RegistroVenda* proximo = nullptr;
};

struct ListaVendas {
    RegistroVenda* primeiro = nullptr;
    RegistroVenda* ultimo = nullptr;

    ~ListaVendas() {
        RegistroVenda* atual = primeiro;
        while (atual != nullptr) {
            RegistroVenda* temp = atual;
            atual = atual->proximo;
            delete temp;
        }
    }
};


bool inserirFinalLue(LUE& lista, NoProduto* novo) {
    if (novo == nullptr) return false;
    if (lista.comeco == nullptr) {
        lista.comeco = novo;
        lista.fim = novo;
    } else {
        lista.fim->elo = novo;
        lista.fim = novo;
    }
    return true;
}

void liberarLue(LUE& lista) {
    NoProduto* aux = lista.comeco;
    NoProduto* prox;
    while (aux != nullptr) {
        prox = aux->elo;
        delete aux->produto;  // Primeiro delete o produto
        delete aux;  // Depois delete o no
        aux = prox;
    }
    lista.comeco = nullptr;
    lista.fim = nullptr;
}

void carregarLista(LUE& lista) {
    ifstream arquivo("produtos.txt");
    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string item;
        vector<string> dados;
        while (getline(ss, item, ',')) {
            dados.push_back(item);
        }
        Produto* prod = new Produto{dados[0], dados[1], stof(dados[2]), stoi(dados[3])};  // Modificação aqui
        NoProduto* novo = new NoProduto{prod, nullptr};
        inserirFinalLue(lista, novo);
    }
    arquivo.close();
}

void salvarLista(LUE lista) {
    ofstream arquivo("produtos.txt");
    NoProduto* atual = lista.comeco;
    while (atual != nullptr) {
        Produto* p = atual->produto;
        arquivo << p->codigo << ',' << p->nome << ',' << p->preco << ',' << p->quantidade << endl;
        atual = atual->elo;
    }
    arquivo.close();
}

void pesquisarProduto(LUE& listaProdutos) {
    string codigo;  // Modificado de char para string
    cout << "Digite o codigo do produto: ";
    cin >> codigo;

    NoProduto* atual = listaProdutos.comeco;
    while (atual != nullptr) {
        if (atual->produto->codigo == codigo) {
            cout << "Produto encontrado!" << endl;
            cout << "Nome do produto: " << atual->produto->nome << endl;
            cout << "Preco do produto: R$" << atual->produto->preco << endl;
            cout << "Quantidade disponivel: " << atual->produto->quantidade << endl;
            return;
        }
        atual = atual->elo;
    }
    cout << "Produto nao encontrado." << endl;
}

void adicionarProduto(LUE &listaProdutos) {
    Produto *novoProduto = new Produto;
    cout << "Digite o codigo do produto: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, novoProduto->codigo);  // Modificado para usar getline


    cout << "Digite o nome do produto: ";
    getline(cin, novoProduto->nome);

    cout << "Digite o preco do produto: ";
    cin >> novoProduto->preco;

    cout << "Digite a quantidade em estoque: ";
    cin >> novoProduto->quantidade;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    NoProduto* novoNo = new NoProduto{ novoProduto, nullptr };
    if (!inserirFinalLue(listaProdutos, novoNo)) {
        cout << "Nao foi possivel adicionar o produto." << endl;
    } else {
        cout << "Produto adicionado com sucesso." << endl;
        salvarLista(listaProdutos);  // Salva a lista atualizada no arquivo
    }
}

bool retirarProdutoLue(LUE& lista, string valor) {  // Modificado de char para string
    if (lista.comeco == nullptr) return false;
    NoProduto* aux = lista.comeco;
    NoProduto* ant = nullptr;
    while (aux != nullptr && valor != aux->produto->codigo) {
        ant = aux;
        aux = aux->elo;
    }
    if (aux == nullptr) return false;  // Produto nao encontrado.
    if (aux == lista.comeco) {
        lista.comeco = aux->elo;
    } else {
        ant->elo = aux->elo;
    }
    if (aux == lista.fim) {
        lista.fim = ant;
    }
    delete aux->produto;  // Libera o produto
    delete aux;  // Libera o no
    return true;
}

void reajustarPreco(LUE& listaProdutos) {
    string codigo;  // Modificado de char para string
    float novoPreco;
    cout << "Digite o codigo do produto para reajustar o preco: ";
    cin >> codigo;  // Sem necessidade de cin.ignore para strings
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Digite o novo preco: ";
    cin >> novoPreco;
    if (cin.fail()) {
        cout << "Entrada invalida para o novo preco." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    NoProduto* atual = listaProdutos.comeco;
    while (atual != nullptr) {
        if (atual->produto->codigo == codigo) {
            atual->produto->preco = novoPreco;
            cout << "Preco reajustado com sucesso." << endl;
            return;
        }
        atual = atual->elo;
    }
    cout << "Produto nao encontrado." << endl;
}

void controlarQuantidade(LUE& listaProdutos) {
    string codigo;  // Modificado de char para string
    int novaQuantidade;
    cout << "Digite o codigo do produto para atualizar a quantidade: ";
    cin >> codigo;  // Sem necessidade de cin.ignore para strings
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Digite a nova quantidade: ";
    cin >> novaQuantidade;
    NoProduto* atual = listaProdutos.comeco;
    while (atual != nullptr) {
        if (atual->produto->codigo == codigo) {
            atual->produto->quantidade = novaQuantidade;
            cout << "Quantidade atualizada com sucesso." << endl;
            return;
        }
        atual = atual->elo;
    }
    cout << "Produto nao encontrado." << endl;
}

void aplicarDesconto(LUE& listaProdutos) {
    string codigo;  // Modificado de char para string
    float percentualDesconto;
    cout << "Digite o codigo do produto para aplicar desconto: ";
    cin >> codigo;  // Sem necessidade de cin.ignore para strings
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Digite o percentual de desconto (ex: 10 para 10%): ";
    cin >> percentualDesconto;
    NoProduto* atual = listaProdutos.comeco;
    while (atual != nullptr) {
        if (atual->produto->codigo == codigo) {
            atual->produto->preco -= (atual->produto->preco * (percentualDesconto / 100));
            cout << "Desconto aplicado com sucesso." << endl;
            return;
        }
        atual = atual->elo;
    }
    cout << "Produto nao encontrado." << endl;
}

bool adicionarAListaDeCompras(ListaCompras& lista, Produto* produto, int quantidade) {
    if (produto->quantidade < quantidade) {
        cout << "Quantidade solicitada nao disponivel." << endl;
        return false;
    }

    ItemCompra* novoItem = new ItemCompra{produto, quantidade, nullptr};

    if (lista.primeiro == nullptr) {
        lista.primeiro = novoItem;
        lista.ultimo = novoItem;
    } else {
        lista.ultimo->proximo = novoItem;
        lista.ultimo = novoItem;
    }

    produto->quantidade -= quantidade;  // Atualiza estoque
    cout << "Produto adicionado a lista de compras." << endl;
    return true;
}

void adicionarProdutoListaCompras(LUE& listaProdutos, ListaCompras& listaCompras) {
    string codigoProduto;  // Modificado de char para string
    int quantidadeProduto;
    cout << "Digite o codigo do produto: ";
    cin >> codigoProduto;  // Sem necessidade de cin.ignore para strings
    cout << "Digite a quantidade desejada: ";
    cin >> quantidadeProduto;

    NoProduto* atual = listaProdutos.comeco;
    bool produtoEncontrado = false;
    while (atual != nullptr) {
        if (atual->produto->codigo == codigoProduto) {
            produtoEncontrado = true;
            if (atual->produto->quantidade >= quantidadeProduto) {
                adicionarAListaDeCompras(listaCompras, atual->produto, quantidadeProduto);
                //cout << "Produto adicionado a lista de compras." << endl;
            } else {
                cout << "Quantidade solicitada nao disponível." << endl;
            }
            break;
        }
        atual = atual->elo;
    }
    if (!produtoEncontrado) {
        cout << "Produto nao encontrado." << endl;
    }
}

void limparListaCompras(ListaCompras& lista) {
    ItemCompra* atual = lista.primeiro;
    while (atual != nullptr) {
        ItemCompra* temp = atual;
        atual = atual->proximo;
        delete temp;
    }
    lista.primeiro = nullptr;
    lista.ultimo = nullptr;
}

void finalizarCompra(ListaCompras& listaCompras, Caixa& caixa, ListaVendas& listaVendas) {
    // Zera o total da caixa para evitar acumulo de valores de compras anteriores
    caixa.total = 0.0;
    caixa.desconto = 0.0;

    // Calcula o total da compra
    ItemCompra* atual = listaCompras.primeiro;
    while (atual != nullptr) {
        caixa.total += atual->produto->preco * atual->quantidade;
        atual = atual->proximo;
    }

    // Aplica descontos se aplicavel
    if (caixa.total > 100) { // Supondo um desconto para compras acima de 100
        caixa.desconto = caixa.total * 0.10; // Desconto de 10%
        caixa.total -= caixa.desconto;
    }

    cout << "Total da compra (com desconto, se aplicavel): R$ " << caixa.total << endl;
    cout << "Desconto aplicado: R$ " << caixa.desconto << endl;

    // Pede a forma de pagamento e o ID do vendedor
    cout << "Informe a forma de pagamento (ex: Dinheiro, Cartao): ";
    cin.ignore(); // Limpa o buffer de entrada antes de ler a string
    getline(cin, caixa.formaPagamento);
    cout << "Informe o ID do vendedor: ";
    getline(cin, caixa.idVendedor);

    // Pergunta o valor pago pelo cliente e calcula o troco
    float valorPago;
    cout << "Valor pago pelo cliente: R$ ";
    cin >> valorPago;

    if (valorPago < caixa.total) {
        cout << "Valor insuficiente. Transacao nao completada." << endl;
        // A lista de compras nao e limpa aqui, permitindo ao usuario tentar novamente
    } else {
        float troco = valorPago - caixa.total;
        cout << "Troco: R$" << troco << endl;

        // Adiciona a venda a lista de vendas
        RegistroVenda* novaVenda = new RegistroVenda{caixa.total, caixa.desconto, caixa.formaPagamento, caixa.idVendedor, nullptr};
        if (listaVendas.primeiro == nullptr) {
            listaVendas.primeiro = novaVenda;
            listaVendas.ultimo = novaVenda;
        } else {
            listaVendas.ultimo->proximo = novaVenda;
            listaVendas.ultimo = novaVenda;
        }

        // Limpeza da lista de compras apos finalizar a compra com sucesso
        while (listaCompras.primeiro != nullptr) {
            ItemCompra* temp = listaCompras.primeiro;
            listaCompras.primeiro = listaCompras.primeiro->proximo;
            delete temp;
        }
        listaCompras.ultimo = nullptr; // Importante para indicar que a lista está vazia
    }
}

void exibirProdutosDisponiveis() {
    ifstream arquivo("produtos.txt");
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo produtos.txt." << endl;
        return;
    }

    string linha;
    cout << "Produtos Disponiveis:" << endl;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string codigo, nome;
        float preco;
        int quantidade;
        getline(ss, codigo, ',');
        getline(ss, nome, ',');
        ss >> preco;
        ss.ignore(); // Ignora a vírgula
        ss >> quantidade;

        cout << "Codigo: " << codigo << ", Nome: " << nome << ", Preco: R$" << preco << ", Quantidade: " << quantidade << endl;
    }

    arquivo.close();
}

void removerProduto(LUE& listaProdutos) {
    string codigo;
    cout << "Digite o codigo do produto a ser removido: ";
    cin >> codigo;

    ifstream arquivoEntrada("produtos.txt");
    ofstream arquivoTemporario("temp.txt");

    if (!arquivoEntrada.is_open() || !arquivoTemporario.is_open()) {
        cout << "Erro ao abrir os arquivos." << endl;
        return;
    }

    string linha;
    bool produtoEncontrado = false;
    while (getline(arquivoEntrada, linha)) {
        stringstream ss(linha);
        string codigoProduto;
        getline(ss, codigoProduto, ',');
        if (codigoProduto != codigo) {
            arquivoTemporario << linha << endl;
        } else {
            produtoEncontrado = true;
        }
    }

    arquivoEntrada.close();
    arquivoTemporario.close();

    if (!produtoEncontrado) {
        cout << "Produto nao encontrado." << endl;
        remove("temp.txt"); // Remove o arquivo temporário, pois não é necessário
    } else {
        remove("produtos.txt"); // Remove o arquivo original
        rename("temp.txt", "produtos.txt"); // Renomeia o arquivo temporário para o nome original
        cout << "Produto removido com sucesso." << endl;
    }
}


void mostrarEstatisticas(ListaVendas& listaVendas) {
    float totalVendas = 0;
    map<string, float> vendasPorFormaPagamento;
    map<string, float> vendasPorVendedor;

    RegistroVenda* atual = listaVendas.primeiro;
    while (atual != nullptr) {
        totalVendas += atual->valorTotal;
        vendasPorFormaPagamento[atual->formaPagamento] += atual->valorTotal;
        vendasPorVendedor[atual->idVendedor] += atual->valorTotal;
        atual = atual->proximo;
    }

    cout << "Total de vendas: R$ " << totalVendas << endl;
    for (auto& par : vendasPorFormaPagamento) {
        cout << "Total por forma de pagamento (" << par.first << "): R$ " << par.second << endl;
    }
    for (auto& par : vendasPorVendedor) {
        cout << "Total por vendedor (" << par.first << "): R$ " << par.second << endl;
    }
}


void menuGerencia(LUE& listaProdutos) {
    int opcao;
    do {
        cout << endl;
        cout << "- Menu da Gerencia\n"
             << "\t1. Pesquisar produto\n"
             << "\t2. Adicionar produto ao estoque\n"
             << "\t3. Remover produto do estoque\n"
             << "\t4. Reajustar preco\n"
             << "\t5. Aplicar desconto\n"
             << "\t6. Controlar quantidade disponivel\n"
             << "\t7. Sair\n"
             << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                pesquisarProduto(listaProdutos);
                break;
            case 2:
                adicionarProduto(listaProdutos);
                break;
            case 3:
                removerProduto(listaProdutos);
                break;
            case 4:
                reajustarPreco(listaProdutos);
                break;
            case 5:
                aplicarDesconto(listaProdutos);
                break;
            case 6:
                controlarQuantidade(listaProdutos);
                break;
            case 7:
                cout << "Voltando ao menu principal..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    } while (opcao != 7);
}

void menuCompras(LUE& listaProdutos, ListaCompras& listaCompras) {
    int opcao;
    do {
        cout << endl;
        cout << "- Fazer Compras\n"
             << "\t1. Ver lista de produtos disponiveis\n"
             << "\t2. Adicionar produtos a lista de compras\n"
             << "\t3. Voltar\n"
             << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                exibirProdutosDisponiveis();
                break;
            case 2:
                adicionarProdutoListaCompras(listaProdutos, listaCompras);
                break;
            case 3:
                cout << "Voltando ao menu principal..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    } while (opcao != 3);
}

void menuCaixa(ListaCompras& listaCompras, Caixa& caixa, ListaVendas& listaVendas) {
    int opcao;
    do {
        cout << endl;
        cout << "- Caixa\n"
             << "\t1. Finalizar compra\n"
             << "\t2. Voltar\n"
             << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                finalizarCompra(listaCompras, caixa, listaVendas);
                break;
            case 2:
                cout << "Voltando ao menu principal..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    } while (opcao != 2);
}

void menuPrincipal(LUE& listaProdutos, ListaVendas& listaVendas) {
    ListaCompras listaCompras;
    Caixa caixa;
    int opcao;
    do {
        cout << endl;
        cout << "Menu Principal:\n"
             << "\t1- Menu da Gerencia\n"
             << "\t2- Fazer Compras\n"
             << "\t3- Finalizar Compra\n"
             << "\t4- Estatisticas de Vendas\n"
             << "\t0- Sair\n"
             << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                menuGerencia(listaProdutos);
                break;
            case 2:
                menuCompras(listaProdutos, listaCompras);
                break;
            case 3:
                finalizarCompra(listaCompras, caixa, listaVendas);
                break;
            case 4:
                mostrarEstatisticas(listaVendas);
                break;
            case 0:
                cout << "Saindo do sistema..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente!" << endl;
        }
    } while (opcao != 0);
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    ListaVendas listaVendas;
    LUE listaProdutos;
    carregarLista(listaProdutos);
    menuPrincipal(listaProdutos, listaVendas);

    return 0;
}
