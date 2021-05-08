#ifndef AVL_TREE_H_
#define AVL_TREE_H_

/**2344
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

namespace DataStructures {

	template <class T>
	class AVLTree {

		Node<T>* root;
		Node<T>* min_node;
		int size;

		void swapKeys(Node<T>* node1, Node<T>* node2);
		bool searchFrom(Node<T>* node, const T& key);
		void balance(Node<T>* node, bool insert = false);
		void rotateRight(Node<T>* node);
		void rotateLeft(Node<T>* node);
		void LLRotation(Node<T>* node);
		void LRRotation(Node<T>* node);
		void RRRotation(Node<T>* node);
		void RLRotation(Node<T>* node);

	public:
		AVLTree() : root(nullptr), min_node(nullptr), size(0) {};
		~AVLTree();
		bool search(const T& key);
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
void AVLTree<T>::clear(){
	postOrder<deleteOperation<T>>(deleteOperation<T>());
	size = 0;
	min_node = 0;
	root = nullptr;
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
	if(temp_parent != nullptr) balance(temp_parent->parent, true);
}

template <class T>
void AVLTree<T>::remove(const T& key){
	size--;
	Node<T>* to_delete = getNode(key);
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
		//Node<T>* temp_parent = temp->parent;

		//temp_parent->left = temp->right;
		//temp->right->parent = temp_parent;
		to_delete = temp;
		/*
		temp->left = to_delete->left;
		Node<T>* temp_parent = to_delete->right;
		temp_parent->left = temp->right;
		temp->right = temp_parent;
		//removing the parent's left son
		if(to_delete_parent != nullptr && to_delete_parent->left == to_delete) to_delete_parent->left = temp;

		//removing the parent's right son
		else if(to_delete_parent != nullptr && to_delete_parent->right == to_delete) to_delete_parent->right = temp;
		*/
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

	
		/*
		//removing the parent's left son
		if(to_delete_parent != nullptr && to_delete_parent->left == to_delete) to_delete_parent->left = temp;

		//removing the parent's right son
		else if(to_delete_parent != nullptr && to_delete_parent->right == to_delete) to_delete_parent->right = temp;
		*/
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
	if(to_delete_parent != nullptr) balance(to_delete_parent->parent);
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
void AVLTree<T>::balance(Node<T>* node, bool insert){
	if(node == nullptr) return;
	/*
	//for observation 4 (tutorial 5)
	int num_of_rotations = 0;
	Node<T>* to_fix = node;

	while(!(insert && num_of_rotations == 1) && to_fix != nullptr){
		if(to_fix->get_balanced_factor() == 2){

			//LR rotation needed
			if(to_fix->get_left_height() == -1) LRRotation(to_fix);

			//LL rotation needed
			else if(to_fix->get_left_height() >= 0) LLRotation(to_fix);

			//should not reach here
			else throw Assert();
		}
		else if(to_fix->get_balanced_factor() == -2){

			//RL rotation needed
			if(to_fix->get_right_height() == 1) RLRotation(to_fix);

			//RR rotation needed
			else if(to_fix->get_right_height() <= 0) RRRotation(to_fix);

			//should not reach here
			else throw Assert();
		}

		to_fix = to_fix->parent;	
		num_of_rotations++;	
	}
	*/
}

template<class T>
void AVLTree<T>::rotateRight(Node<T>* node){

	Node<T>* new_root = node->left;              //A
	Node<T>* new_left_son = new_root->right;     //Ar
	new_root->right = node;                        //A-->B
	node->left = new_left_son;                     //B-->Ar
	new_root->update_height();
	node->update_height();
}

template<class T>
void AVLTree<T>::rotateLeft(Node<T>* node){

	Node<T>* new_root = node->right;              //B
	Node<T>* new_right_son = new_root->left;      //Bl
	new_root->left = node;                          //B-->A
	node->right = new_right_son;                    //A-->Bl
	new_root->update_height();
	node->update_height();
}

template <class T>
void AVLTree<T>::RRRotation(Node<T>* node){
	//maybe not node?
	rotateRight(node);
}

template <class T>
void AVLTree<T>::RLRotation(Node<T>* node){
	rotateRight(node->right);
	rotateLeft(node);
}

template <class T>
void AVLTree<T>::LLRotation(Node<T>* node){
	rotateLeft(node);
}

template <class T>
void AVLTree<T>::LRRotation(Node<T>* node){
	rotateLeft(node->left);
	rotateRight(node);
}

#endif  /* AVL_TREE_H_ */