#include <iostream>
using namespace std;

typedef struct no_str *Apontador;

typedef struct no_str {
    int valor;
    int altura;
    Apontador pai;
    Apontador esq;
    Apontador dir;
} No;

typedef struct {
    Apontador raiz;
} Tree;

Tree* construct(int valor);
void addNo(Tree* tree, int valor);
void removeNo(Tree* tree, int valor);
void printTree(No* no);