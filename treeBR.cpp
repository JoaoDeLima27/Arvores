#include <iostream>

#ifndef TreeRB_hpp
#define TreeRB_hpp
#endif /* TreeRB_hpp */

enum cor {V, P};

using namespace std;

struct No {
    int valor;
    cor cor;
    No* esq;
    No* dir;
    No* pai;

    explicit No (int valor) : valor(valor), cor(V), esq(nullptr), dir(nullptr), pai(nullptr) {}
};

class TreeRB {
private:
    No* raiz;

    // Rotação à esquerda
    void rotacaoEsq(No* atual) {
        if (atual == nullptr || atual->dir == nullptr)
            return;

        No* atual_dir = atual->dir;
        atual->dir = atual_dir->esq;
        if (atual_dir->esq != nullptr)
            atual_dir->esq->pai = atual;
        atual_dir->pai = atual->pai;
        if (atual->pai == nullptr)
            raiz = atual_dir;
        else if (atual == atual->pai->esq)
            atual->pai->esq = atual_dir;
        else
            atual->pai->dir = atual_dir;
        atual_dir->esq = atual;
        atual->pai = atual_dir;
    }

    // Rotação à direita
    void rotacaoDir(No* atual) {
        if (atual == nullptr || atual->esq == nullptr)
            return;

        No* atual_esq = atual->esq;
        atual->esq = atual_esq->dir;
        if (atual_esq->dir != nullptr)
            atual_esq->dir->pai = atual;
        atual_esq->pai = atual->pai;
        if (atual->pai == nullptr)
            raiz = atual_esq;
        else if (atual == atual->pai->esq)
            atual->pai->esq = atual_esq;
        else
            atual->pai->dir = atual_esq;
        atual_esq->dir = atual;
        atual->pai = atual_esq;
    }

    // Função que transplanta (substitui) um nó por outro
    void transplant (No* atual, No* substituto) {
        if (atual->pai == nullptr)
            raiz = substituto;
        else if (atual == atual->pai->esq)
            atual->pai->esq = substituto;
        else
            atual->pai->dir = substituto;
        if (substituto != nullptr)
            substituto->pai = atual->pai;
    }

    // Consertar a árvore depois de uma inserção
    void fixInsert(No* atual) {
        // Atual não é raiz e o pai é vermelho
        while (atual != raiz && atual->pai->cor == V) {
            // Se o pai é o filho esquerdo do avô
            if (atual->pai == atual->pai->pai->esq) {
                No* tio = atual->pai->pai->dir;
                // Aqui o tio é vermelho e o pai também, então a cor do pai, do tio e do avô são trocadas para preto
                if (tio != nullptr && tio->cor == V) {
                    atual->pai->cor = P;
                    tio->cor = P;
                    atual->pai->pai->cor = V;
                    atual = atual->pai->pai;

                // Tio é nulo ou preto
                } else {
                    // Se o filho direito do pai é o atual, então a rotação à esquerda é realizada
                    if (atual == atual->pai->dir) {
                        atual = atual->pai;
                        rotacaoEsq(atual);
                    }
                    atual->pai->cor = P;
                    atual->pai->pai->cor = V;
                    rotacaoDir(atual->pai->pai);
                }
            // Se o pai é o filho direito do avô e é vermelho
            } else {
                // O tio é o filho esquerdo do avô
                No* tio = atual ->pai->pai->esq;
                if (tio != nullptr && tio->cor == V) {
                    // O tio não é nulo e é vermelho, então a cor do pai e do tio ficam pretas e a cor do avô é trocada para vermelho
                    atual->pai->cor = P;
                    tio->cor = P;
                    atual->pai->pai->cor = V;
                    atual = atual->pai->pai;
                } else {
                    // O tio não existe oou é preto, então ocorrem rotações
                    if (atual == atual->pai->esq) {
                        atual = atual->pai;
                        rotacaoDir(atual);
                    }
                    atual->pai->cor = P;
                    atual->pai->pai->cor = V;
                    rotacaoEsq(atual->pai->pai);
                }
            }
        }
        raiz->cor = P;
    }

    //Apaga um nó
    void deleteNo (No* atual) {
        if (atual == nullptr)
            return;

        No* y = atual;
        No* x = nullptr;
        cor y_cor = y->cor;

        if (atual->esq == nullptr) {
            x = atual->dir;
            transplant(atual, atual->dir);
        } else if (atual->dir == nullptr) {
            x = atual->esq;
            transplant(atual, atual->esq);
        } else {
            y = minimo(atual->dir);
            y_cor = y->cor;
            x = y->dir;

            if (y->pai == atual) {
                if (x != nullptr)
                    x->pai = y; 
            } else {
                if (x != nullptr)
                    x->pai = y->pai;
                transplant(y, y->dir);
                if (y->dir != nullptr)
                    y->dir->pai = y; 
                y->dir = atual->dir;
                if (y->dir != nullptr)
                    y->dir->pai = y; 
            }
            transplant(atual, y);
            y->esq = atual->esq;
            if (y->esq != nullptr)
                y->esq->pai = y;
            y->cor = atual->cor;
        }

        if (y_cor == P && x != nullptr) 
            fixDelete(x);

        delete atual; 
    }

    // Acha o menor nó da árvore
    No* minimo (No* node) {
        while (node->esq != nullptr)
            node = node->esq;
        return node;
    }

    // Conserta árvore depois de uma remoção
    void fixDelete (No* x) {
        // x é preto e não é a raiz
        while (x != raiz && x != nullptr && x->cor == P) {
            // Se x é filho esquerdo do seu pai
            if (x == x->pai->esq) {
                No* w = x->pai->dir;
                if (w->cor == V) {
                    // w é irmão vermelho de x
                    w->cor = P;
                    x->pai->cor = V;
                    rotacaoEsq(x->pai);
                    w = x->pai->dir;
                    //rotaciona pra esquerda
                }
                if ((w->esq == nullptr || w->esq->cor == P) &&
                    (w->dir == nullptr || w->dir->cor == P)) {
                    w->cor = V;
                    x = x->pai;
                } else {
                    if (w->dir == nullptr || w->dir->cor == P) {
                        if (w->esq != nullptr)
                            w->esq->cor = P;
                        w->cor = V;
                        rotacaoDir(w);
                        w = x->pai->dir;
                    }
                    w->cor = x->pai->cor;
                    x->pai->cor = P;
                    if (w->dir != nullptr)
                        w->dir->cor = P;
                    rotacaoEsq(x->pai);
                    x = raiz;
                }
            } else {
                No* w = x->pai->esq;
                if (w->cor == V) {
                    w->cor = P;
                    x->pai->cor = V;
                    rotacaoDir(x->pai);
                    w = x->pai->esq;
                }
                if ((w->dir == nullptr || w->dir->cor == P) &&
                    (w->esq == nullptr || w->esq->cor == P)) {
                    w->cor = V;
                    x = x->pai;
                } else {
                    if (w->esq == nullptr || w->esq->cor == P) {
                        if (w->dir != nullptr)
                            w->dir->cor = P;
                        w->cor = V;
                        rotacaoEsq(w);
                        w = x->pai->esq;
                    }
                    w->cor = x->pai->cor;
                    x->pai->cor = P;
                    if (w->esq != nullptr)
                        w->esq->cor = P;
                    rotacaoDir(x->pai);
                    x = raiz;
                }
            }
        }
        if (x != nullptr)
            x->cor = P;
    }



public:
    TreeRB() : raiz (nullptr) {}

    // Adiciona um nó
    void addNoRB (int valor) {
        No* newNo = new No(valor);
        No* y = nullptr;
        No* x = raiz;
    
        while (x != nullptr) {
            y = x;
            if (newNo->valor < x->valor)
                x = x->esq;
            else
                x = x->dir;
        }
    
        newNo->pai = y;
        if (y == nullptr)
            raiz = newNo;
        else if (newNo->valor < y->valor)
            y->esq = newNo;
        else
            y->dir = newNo;
    
        fixInsert (newNo);
    }

    // remove um nó
    void removeRB (int valor) {
        No* z = raiz;
        while (z != nullptr) {
            if (valor < z->valor)
                z = z->esq;
            else if (valor > z->valor)
                z = z->dir;
            else {
                deleteNo(z);
                return;
            }
        }
        cout << "Valor inexistente" << endl;
    }

    void printHelperRB (No* raiz, string espaco, bool last)
    {
        if (raiz != nullptr) {
            cout << espaco;
            if (last) {
                cout << "R----";
                espaco += "   ";
            }
            else {
                cout << "L----";
                espaco += "|  ";
            }
            string n_cor = (raiz->cor == V) ? "V" : "P";
            cout << raiz->valor << " (" << n_cor << ")" << endl;
            printHelperRB(raiz->esq, espaco, false);
            printHelperRB(raiz->dir, espaco, true);
        }
    }

    void printTreeRB ()
    {
        if (raiz == nullptr)
            cout << "Árvore vazia" << endl;
        else {
            cout << "Árvore do flamengo:" << endl;
            printHelperRB(raiz, "", true);
        }
    }
};