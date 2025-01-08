#include "treeAVL.hpp"
using namespace std;

int main() {
    Tree* minhaArvore = construct(10);
    addNo(minhaArvore, 20);
    addNo(minhaArvore, 5);
    addNo(minhaArvore, 4);
    addNo(minhaArvore, 15);
    addNo(minhaArvore, 13);
    addNo(minhaArvore, 20);
    addNo(minhaArvore, 27);

    printTree(minhaArvore->raiz);

    removeNo(minhaArvore, 20);
    cout << "Após remoção:" << endl;
    printTree(minhaArvore->raiz);
    
    removeNo(minhaArvore, 50);
    cout << "Após remoção:" << endl;
    printTree(minhaArvore->raiz);
}