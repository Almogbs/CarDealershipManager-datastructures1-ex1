#ifndef NODE_H_
#define NODE_H_

/**
* Generic Node
*
* Implements a Node of generic type.
*
* The following methods are available:
*   size		                - Returns the size of the list
*   contains	                - returns whether or not an element exists inside the list.
*   insert	                    - Insert an element to the list.
*   removeElement		        - Removes specified element from the list
*   GetFirst	                - Sets the internal iterator to the first element in the list and returns it
*   GetNext		                - Advances the internal iterator to the next key and returns it.
*	getElement		            - Returns a pointer to same element within the list
* 	LL_FOREACH	                - A macro for iterating over the list's elements.
*/

#include "exceptions.h"

#define EMPTY_TREE_HEIGHT -1
#define LEAF_TREE_HEIGHT 0


namespace DataStructures{

	template <class T>
	class Node {
    public:
        T         key;
		int       height;
		Node*     parent = nullptr;
		Node*     left = nullptr;
		Node*     right = nullptr;
        Node(const T& key, int height = LEAF_TREE_HEIGHT) : key(key), height(height) {};
        void update_height();
        T get_key() const;
        int get_height() const;
        int get_left_height();
        int get_right_height();
		int get_balanced_factor() const;
	};

    template <class T>
    T Node<T>::get_key() const{
	    return key;
    }

    template <class T>
    void Node<T>::update_height(){
        height = std::max(get_left_height(), get_right_height()) + 1;        //std good?
    }

    template <class T>
    int Node<T>::get_height() const{
        return height;
    }

    template <class T>
    int Node<T>::get_left_height(){
        return (left == nullptr) ? EMPTY_TREE_HEIGHT : left->get_height();
    }

    template <class T>
    int Node<T>::get_right_height(){
        return (right == nullptr) ? EMPTY_TREE_HEIGHT : right->get_height();
    }

    template <class T>
    int Node<T>::get_balanced_factor() const{
        if(left == nullptr && right == nullptr) return 0;
        else if(left == nullptr) return EMPTY_TREE_HEIGHT - right->get_height();
        else if(right == nullptr) return left->get_height() - EMPTY_TREE_HEIGHT;
        return left->get_height() - right->get_height();
    }
}

#endif  /* NODE_H_ */