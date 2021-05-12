#ifndef AVL_TREE_H_
#define AVL_TREE_H_

/**
* Generic AVL Tree
*
* Implements an AVL Tree of generic type.
*
* The following methods are available:
*   getSize		            - Returns the size of the list
*   search 		            - returns whether or not an element exists inside the list.
*   insert	                - Insert an element to the list.
*   remove		       		- Removes specified element from the list
*   GetMinNode	                - Sets the internal iterator to the first element in the list and returns it
*   GetNode	                - Sets the internal iterator to the first element in the list and returns it
*   GetRoot	                - Sets the internal iterator to the first element in the list and returns it
*   ...
*/

#include <iostream>
#include <stdlib.h>
#include "node.h"
#include "exceptions.h"

#define EMPTY_TREE_HEIGHT -1

using namespace DataStructures;
using std::endl;
using std::cout;

static int calcHeight(int num){
	int result = 0;
	while(num > 1){
		num /= 2;
		result++;
	}
	return result;
}

namespace DataStructures {

	template <class T>
	class AVLTree {

		Node<T>* 	root;
		Node<T>* 	min_node;
		Node<T>* 	iterator;
		int			size;

		Node<T>* buildRoot(T* sorted_keys, int left, int right, Node<T>* root_parent, int root_height);
		void swapKeys(Node<T>* node1, Node<T>* node2);
		void balance(Node<T>* node);
		void rotateRight(Node<T>* node);
		void rotateLeft(Node<T>* node);
		void LLRotation(Node<T>* node);
		void LRRotation(Node<T>* node);
		void RRRotation(Node<T>* node);
		void RLRotation(Node<T>* node);

	public:
		AVLTree() : root(nullptr), min_node(nullptr), size(0) {};
		AVLTree(T* sorted_keys, int num);
		~AVLTree();
		bool search(const T& key);
		bool searchFrom(Node<T>* node, const T& key);
		void insert(const T& key);
		void remove(const T& key);
		void clear();
		Node<T>* getNode(const T& key);
		Node<T>* getMinNode();
		Node<T>* getRoot();
		int getSize();

		template <class Operation>
		void preOrder(Operation op);

		template <class Operation>
		void preOrderFrom(Node<T>* node, Operation op);

		template <class Operation>
		void postOrder(Operation op);

		template <class Operation>
		void postOrderFrom(Node<T>* node, Operation op);

		template <class Operation>
		void inOrder(Operation op);

		template <class Operation>
		void inOrderFrom(Node<T>* node, Operation op);

		//template <class Operation>
		T* InOrderGetFirst();

		//template <class Operation>
		T* InOrderGetNext();

		void printInorder();
		void printPostOrder();
		void printPreOrder();
	};
}

// aux
template <class T>
class deleteOperation{
public:
    void operator()(const Node<T>* node);
};

template <class T>
class printOperation{
public:
    void operator()(const Node<T>* node);
};

template <class T>
void deleteOperation<T>::operator()(const Node<T>* node){
    if(node != nullptr) delete node;
}

template <class T>
void printOperation<T>::operator()(const Node<T>* node){
	//to change after validate adt
    cout << node->key << " BF: " << node->get_balanced_factor() <<  " height: " << node->get_height() << endl;
}

//dec

template <class T>
template <class Operation>
void AVLTree<T>::preOrderFrom(Node<T>* node, Operation op){
	if(node == nullptr) return;
	op(node);
	preOrderFrom(node->left, op);
	preOrderFrom(node->right, op);
}

template <class T>
template <class Operation>
void AVLTree<T>::postOrderFrom(Node<T>* node, Operation op){
	if(node == nullptr) return;
	postOrderFrom(node->left, op);
	postOrderFrom(node->right, op);
	op(node);
}

template <class T>
template <class Operation>
void AVLTree<T>::inOrderFrom(Node<T>* node, Operation op){
	if(node == nullptr) return;
	inOrderFrom(node->left, op);
	op(node);
	inOrderFrom(node->right, op);
}

template <class T>
template <class Operation>
void AVLTree<T>::preOrder(Operation op){
	preOrderFrom(root, op);
}

template <class T>
template <class Operation>
void AVLTree<T>::postOrder(Operation op){
	postOrderFrom(root, op);
}

template <class T>
template <class Operation>
void AVLTree<T>::inOrder(Operation op){
	inOrderFrom(root, op);
}


template <class T>
//template <class Operation>
T* AVLTree<T>::InOrderGetFirst(){
	iterator = min_node;
	return iterator->get_key();
}

template <class T>
//template <class Operation>
T* AVLTree<T>::InOrderGetNext(){
	if(iterator == nullptr) return nullptr;
	Node<T>* result = iterator->right;
	if(result == nullptr){
		result = iterator;
		while(result->parent != nullptr && *(result->get_key()) > *(result->parent->get_key())){
			result = result->parent;
		}
		if(result->parent != nullptr) iterator = result->parent;
		return iterator->get_key();
	}
	while(result->left != nullptr) result = result->left;
	iterator = result;
	return iterator->get_key();
}

template <class T>
void AVLTree<T>::clear(){
	postOrder<deleteOperation<T>>(deleteOperation<T>());
	size = 0;
	min_node = 0;
	root = nullptr;
}

template <class T>
Node<T>* AVLTree<T>::buildRoot(T* sorted_keys, int left, int right, Node<T>* root_parent, int root_height){
	if(left > right) return nullptr;

	int mid = (left + right)/2;
	Node<T>* root = new Node<T>(sorted_keys[mid], root_height);
	root->parent = root_parent;
	root->left = buildRoot(sorted_keys, left, mid - 1, root, root_height - 1);
	root->right = buildRoot(sorted_keys, mid + 1, right, root, root_height - 1);
	return root;
}

template <class T>
AVLTree<T>::AVLTree(T* sorted_keys, int num) : size(num){
	root = buildRoot(sorted_keys, 0, num-1, nullptr, calcHeight(num));
	min_node = root;
	while(min_node->left != nullptr) min_node = min_node->left;
}

template <class T>
AVLTree<T>::~AVLTree(){
	postOrder<deleteOperation<T>>(deleteOperation<T>());
}

template <class T>
void AVLTree<T>::swapKeys(Node<T>* node1, Node<T>* node2){
	//should not happen
	if(node1 == nullptr || node2 == nullptr) throw Assert();
	T temp_key = node1->key;
	node1->key = node2->key;
	node2->key = temp_key;
}

template <class T>
bool AVLTree<T>::search(const T& key){
	return searchFrom(root, key);
}

template <class T>
void AVLTree<T>::insert(const T& key){
	if(search(key)) throw NodeExist();

	//find the right place for the new node
	Node<T>* to_insert;
	Node<T>* temp = root;
	Node<T>* temp_parent = nullptr;
	size++;
	while(temp != nullptr){
		temp_parent = temp;
		if (temp->key > key) temp = temp->left;
		else temp = temp->right;
	}

	try {
		//alloc and insert the new node in the right place
		to_insert = new Node<T>(key);
		to_insert->parent = temp_parent;
	} catch(const std::bad_alloc &e){
		//change the tree as nothing happened
		size--;

		//throw further for user manegment
		throw e;
		}

	if(temp_parent != nullptr && to_insert->key > temp_parent->key) temp_parent->right = to_insert;
	else if(temp_parent != nullptr && to_insert->key < temp_parent->key) temp_parent->left = to_insert;
	else if(temp_parent == nullptr) root = to_insert; 

	//update minimum node
	if(min_node == nullptr || min_node->key > key) min_node = to_insert;

	//update the heights of the nodes in the path
	Node<T>* fix_height = to_insert->parent;
	while(fix_height != nullptr){
		fix_height->update_height();
		fix_height = fix_height->parent;	
	}

	//rotations for balancing, if needed
	if(temp_parent != nullptr) balance(temp_parent->parent);
}

template <class T>
void AVLTree<T>::remove(const T& key){
	Node<T>* to_delete = getNode(key);
	size--;
	Node<T>* to_delete_parent = to_delete->parent;

	//remove the node and fix the tree
	//if to_delete has both sons
	if(to_delete->right != nullptr && to_delete->left != nullptr){

		//get the closest valued node from right
		Node<T>* temp = to_delete->right;
		while(temp->left != nullptr){
			temp = temp->left;
		}
		swapKeys(temp, to_delete);
		to_delete = temp;
	}

	to_delete_parent = to_delete->parent;

	//if to_delete is leaf(lucky me)
	if(to_delete->right == nullptr && to_delete->left == nullptr){
		//sole node in the tree
		if(to_delete_parent == nullptr) root = nullptr;
		//removing the parent's left son
		else if(to_delete_parent->left == to_delete) to_delete_parent->left = nullptr;
		//removing the parent's right son
		else if(to_delete_parent->right == to_delete) to_delete_parent->right = nullptr;
	}

	//if to_delete have only one son
	else {
		Node<T>* temp = (to_delete->right != nullptr) ? to_delete->right : to_delete->left;
		//to_delete is the root
		if(to_delete_parent == nullptr) {
			root = temp;
			root->parent = nullptr;
			//to_delete = temp;
		}
		else {
				swapKeys(temp, to_delete);
				(to_delete->right == temp) ? to_delete->right = nullptr : to_delete->left = nullptr;
				to_delete = temp;
		}
	}

	//update the min node
	if(to_delete->key == min_node->key) min_node = to_delete_parent;

	//update the heights of the nodes in the path
	Node<T>* fix_height = to_delete_parent;
	while(fix_height != nullptr){
		fix_height->update_height();
		fix_height = fix_height->parent;
	}

	delete to_delete;
	//rotations for balancing, if needed
	balance(to_delete_parent);
}

template <class T>
void AVLTree<T>::printInorder(){
	inOrder<printOperation<T>>(printOperation<T>());
	cout << endl;
}

template <class T>
void AVLTree<T>::printPostOrder(){
	postOrder<printOperation<T>>(printOperation<T>());
	cout << endl;
}

template <class T>
void AVLTree<T>::printPreOrder(){
	preOrder<printOperation<T>>(printOperation<T>());
	cout << endl;
}

template <class T>
Node<T>* AVLTree<T>::getNode(const T& key){
	if(!search(key)) throw NodeNotExist();
	if(root == nullptr) return nullptr;
	Node<T>* to_return = root;
	while(to_return->key != key){
		if(to_return->key > key) to_return = to_return->left;
		else to_return = to_return->right;
	}
	return to_return;
}

template <class T>
Node<T>* AVLTree<T>::getRoot(){
	return root;
}

template <class T>
Node<T>* AVLTree<T>::getMinNode(){
	return min_node;
}

template <class T>
int AVLTree<T>::getSize(){
	return size;
}

template <class T>
bool AVLTree<T>::searchFrom(Node<T>* node, const T& key){
	if(node == nullptr) return false;
	else if(node->key == key) return true;
	return (node->key < key) ? searchFrom(node->right, key) : searchFrom(node->left, key);
}

template <class T>
void AVLTree<T>::balance(Node<T>* node){
	if(node == nullptr) return;
	//for observation 4 (tutorial 5)
	Node<T>* to_fix = node;

	while(to_fix != nullptr){
		to_fix->update_height();
		if(to_fix->get_balanced_factor() == 2){

			//LR rotation needed
			if(to_fix->left->get_balanced_factor() == -1){
				LRRotation(to_fix);
			}
			//LL rotation needed
			else if(to_fix->left->get_balanced_factor() >= 0){
				LLRotation(to_fix);
			}
			//should not reach here
			else throw Assert();
		}
		else if(to_fix->get_balanced_factor() == -2){

			//RL rotation needed
			if(to_fix->right->get_balanced_factor() == 1){
				RLRotation(to_fix);
			}

			//RR rotation needed
			else if(to_fix->right->get_balanced_factor() <= 0){
				RRRotation(to_fix);
			}

			//should not reach here
			else throw Assert();
		}
		to_fix = to_fix->parent;	
	}
}

template<class T>
void AVLTree<T>::rotateRight(Node<T>* node){
	Node<T>* B = node; 
	Node<T>* Bparent = B->parent;
	Node<T>* A = B->left;
	Node<T>* Aright = A->right;
	if(Aright != nullptr) Aright->parent = B;
	B->left = Aright;
	A->parent = Bparent;
	if(Bparent == nullptr) root = A;
	else if (Bparent->right == B) Bparent->right = A;
	else if (Bparent->left == B) Bparent->left = A;
	else throw Assert();
	A->right = B;
	B->parent = A;
	B->update_height();
	A->update_height();
}

template<class T>
void AVLTree<T>::rotateLeft(Node<T>* node){
	Node<T>* A = node;
	Node<T>* Aparent = A->parent;
	Node<T>* B = A->right;
	Node<T>* Bleft = B->left;
	if (Bleft != nullptr) Bleft->parent = A;
	A->right = Bleft;
	B->parent = Aparent;
	if(Aparent == nullptr) root = B;
	else if (Aparent->right == A) Aparent->right = B;
	else if (Aparent->left == A) Aparent->left = B;
	else throw Assert();
	B->left = A;
	A->parent = B;
	A->update_height();
	B->update_height();
}

template <class T>
void AVLTree<T>::RRRotation(Node<T>* node){
	rotateLeft(node);
}

template <class T>
void AVLTree<T>::RLRotation(Node<T>* node){
	rotateRight(node->right);
	rotateLeft(node);
}

template <class T>
void AVLTree<T>::LLRotation(Node<T>* node){
	rotateRight(node);
}

template <class T>
void AVLTree<T>::LRRotation(Node<T>* node){
	rotateLeft(node->left);
	rotateRight(node);
}

#endif  /* AVL_TREE_H_ */
