#ifndef NODE_H_
#define NODE_H_

/**
* Generic Linked List
*
* Implements a Linked List of generic type.
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

namespace DataStructures{

	template <class T>
	class Node {
    public:
        T key;
		int height;
		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;
        Node(const T& key, int height = -1) : key(key), height(height) {};
        int get_height();
		int get_balanced_factor();
	};


    template <class T>
    int Node<T>::get_height(){
        return height;
    }

    template <class T>
    int Node<T>::get_balanced_factor(){
        if(left == nullptr && right == nullptr) return 0;
        else if(left == nullptr) return -right->get_height();
        return left->get_height();
    }

}

#endif  /* NODE_H_ */