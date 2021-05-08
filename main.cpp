#include "avl_tree.h"

int main(int argc, char** argv){

    AVLTree<int> t1;
    t1.insert(24);
    t1.insert(4); 
    t1.insert(24364);
    t1.insert(2);
    t1.insert(924);
    t1.insert(1);
    t1.insert(244);
    t1.insert(242);
    t1.insert(31);
    t1.insert(12);

    t1.printInorder();
    t1.printPostOrder();
    t1.printPreOrder();

    cout << t1.getSize() << endl;
  
    t1.remove(4);
    t1.printInorder();
    t1.printPostOrder();
    t1.printPreOrder();
    cout << t1.getSize() << endl;
    cout << " i run" << endl;
    return 0;
}