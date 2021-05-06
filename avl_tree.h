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

		bool searchFrom(Node<T>* node, const T& key);
		void balance(Node<T>* node);

	public:
		AVLTree() : root(nullptr), min_node(nullptr), size(0) {};
		~AVLTree();
		bool search(const T& key);
		void insert(const T& key);
		void remove(const T& key);
		Node<T>* getNode(const T& key);
		Node<T>* getMinNode();
		Node<T>* getRoot();
		int getSize();
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
    delete node;
}

template <class T>
void printOperation<T>::operator()(const Node<T>* node){
    cout << node->key << endl;
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
AVLTree<T>::~AVLTree(){
	postOrder<deleteOperation<T>>(deleteOperation<T>());
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
		if(temp_parent != nullptr) temp_parent->height++;
	}

	try {
		//alloc and insert the new node in the right place
		int new_height = (temp_parent == nullptr) ? EMPTY_TREE_HEIGHT : temp_parent->height + 1; 
		to_insert = new Node<T>(key, new_height);
		to_insert->parent = temp_parent;
	} catch(const std::bad_alloc &e){
		//change the tree as nothing happened
		size--;
		Node<T>* bad_alloc_node = root;
		while(bad_alloc_node != nullptr){
		bad_alloc_node->height--;
		if (bad_alloc_node->key > key) bad_alloc_node = bad_alloc_node->left;
		else bad_alloc_node = bad_alloc_node->right;
		}
		//throw further for user manegment
		throw e;
	}

	if(temp_parent != nullptr && to_insert->key > temp_parent->key) temp_parent->right = to_insert;
	else if(temp_parent != nullptr && to_insert->key < temp_parent->key) temp_parent->left = to_insert;
	else if(temp_parent == nullptr) root = to_insert; 

	//update minimum node
	if(min_node == nullptr || min_node->key > key) min_node = to_insert;

	//rotations for balancing, if needed
	if(temp_parent != nullptr) balance(temp_parent->parent);
}

template <class T>
void AVLTree<T>::remove(const T& key){
	if(!search(key)) throw NodeNotExist();
	size--;
	Node<T>* to_delete = root;s
	Node<T>* to_delete_parent = nullptr;
	while(to_delete->key != key){
		if (to_delete->key > key) to_delete = to_delete->left;
		else to_delete = to_delete->right;
		to_delete_parent = to_delete;
		to_delete_parent->height--;
		
	//remove the node and fix the tree
	//!@#$%^&*()!@#$%^&*()

	//rotations for balancing, if needed
	if(to_delete_parent != nullptr) balance(to_delete_parent->parent);
	}
}

template <class T>
Node<T>* AVLTree<T>::getNode(const T& key){
	if(!search(key)) throw NodeNotExist();
	if(root == nullptr) return false;
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
	Node<T>* to_fix = node;
	while(to_fix != nullptr && std::abs(to_fix->get_balanced_factor) <= 1){
		to_fix = to_fix->parent;
	}


}
#endif  /* AVL_TREE_H_ */
