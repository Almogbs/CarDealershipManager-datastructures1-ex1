#include "avl_tree.h"
using namespace DataStructures;

int main(int argc, char** argv){

    AVLTree<int> t1;
    t1.insert(2);
    t1.insert(3);
    t1.insert(1);
        cout << t1.getSize() << endl;
    try{
        t1.insert(3);
    } catch(const NodeExist &e){
        cout << "yaa" << endl;
    }
    t1.insert(5);
    t1.insert(77);
    cout << t1.getSize() << endl;
        t1.remove(2);

    try{
        t1.remove(4);
    } catch(const NodeNotExist &e){
        cout << "yaaaa" << endl;
    }
    cout << t1.getSize() << endl;
    cout << " i run" << endl;
    return 0;
}