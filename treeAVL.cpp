#include "treeAVL.hpp"

// Função para criar um novo nó
static Apontador novoNo(int valor) {
    Apontador no = new No;
    no->valor = valor;
    no->altura = 1;
    no->pai = nullptr;
    no->esq = nullptr;
    no->dir = nullptr;
    return no;
}

// Função para obter a altura de um nó
static int altura(Apontador no) {
    return (no == nullptr) ? 0 : no->altura;
}

// Função para calcular o fator de balanceamento
static int fatorBalanceamento(Apontador no) {
    return (no == nullptr) ? 0 : altura(no->esq) - altura(no->dir);
}

// Atualiza a altura de um nó
static void atualizarAltura(Apontador no) {
    no->altura = 1 + max(altura(no->esq), altura(no->dir));
}

// Rotação à direita
static Apontador rotacaoDireita(Apontador y) {
    Apontador x = y->esq;
    Apontador T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    if (T2 != nullptr) T2->pai = y;
    atualizarAltura(y);
    atualizarAltura(x);
    return x;
}

// Rotação à esquerda
static Apontador rotacaoEsquerda(Apontador x) {
    Apontador y = x->dir;
    Apontador T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    if (T2 != nullptr) T2->pai = x;
    atualizarAltura(x);
    atualizarAltura(y);
    return y;
}

// Balanceamento do nó
static Apontador balancear(Apontador no) {
    atualizarAltura(no);
    int fb = fatorBalanceamento(no);

    if (fb > 1 && fatorBalanceamento(no->esq) >= 0)
        return rotacaoDireita(no);

    if (fb > 1 && fatorBalanceamento(no->esq) < 0) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

    if (fb < -1 && fatorBalanceamento(no->dir) <= 0)
        return rotacaoEsquerda(no);

    if (fb < -1 && fatorBalanceamento(no->dir) > 0) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;
}

// Inserção na árvore AVL
static Apontador inserir(Apontador no, int valor) {
    if (no == nullptr) return novoNo(valor);

    if (valor < no->valor) {
        no->esq = inserir(no->esq, valor);
        no->esq->pai = no;
    } else if (valor > no->valor) {
        no->dir = inserir(no->dir, valor);
        no->dir->pai = no;
    } else {
        return no;
    }

    return balancear(no);
}

// Menor valor de um nó
static Apontador menorValorNo(Apontador no) {
    while (no->esq != nullptr)
        no = no->esq;
    return no;
}

// Remoção de um nó na árvore AVL
static Apontador remover(Apontador raiz, int valor) {
    if (raiz == nullptr) return raiz;

    if (valor < raiz->valor)
        raiz->esq = remover(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = remover(raiz->dir, valor);
    else {
        if (raiz->esq == nullptr || raiz->dir == nullptr) {
            Apontador temp = (raiz->esq != nullptr) ? raiz->esq : raiz->dir;
            delete raiz;
            return temp;
        } else {
            Apontador temp = menorValorNo(raiz->dir);
            raiz->valor = temp->valor;
            raiz->dir = remover(raiz->dir, temp->valor);
        }
    }

    return balancear(raiz);
}

Tree* construct(int valor) {
    Tree* tree = new Tree;
    tree->raiz = novoNo(valor);
    return tree;
}

void addNo(Tree* tree, int valor) {
    tree->raiz = inserir(tree->raiz, valor);
}

void removeNo(Tree* tree, int valor) {
    tree->raiz = remover(tree->raiz, valor);
}

static void printHelper(No* raiz, string espaco, bool last) {
    if (raiz != nullptr) {
        cout << espaco;
        if (last) {
            cout << "R----";
            espaco += "   ";
        } else {
            cout << "L----";
            espaco += "|  ";
        }
        cout << raiz->valor << endl;
        printHelper(raiz->esq, espaco, false);
        printHelper(raiz->dir, espaco, true);
    }
}

void printTree(No* raiz) {
    if (raiz == nullptr)
        cout << "Árvore vazia" << endl;
    else {
        cout << "Árvore AVL:" << endl;
        printHelper(raiz, "", true);
    }
}
