#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

/**
* Exception List
*
* an Exception List for DataStructures namespace.
*
* The following exceptions are available:
*   size		                - Returns the size of the list
*   contains	                - returns whether or not an element exists inside the list.
*   insert	                    - Insert an element to the list.
*   removeElement		        - Removes specified element from the list
*   GetFirst	                - Sets the internal iterator to the first element in the list and returns it
*   GetNext		                - Advances the internal iterator to the next key and returns it.
*	getElement		            - Returns a pointer to same element within the list
* 	LL_FOREACH	                - A macro for iterating over the list's elements.
*/

#include <stdexcept>

namespace DataStructures{

	class AllocationError : public std::exception {};
	class InvalidArg : public std::exception {};
	class NodeExist : public std::exception {};
	class NodeNotExist : public std::exception {};
	class Failure : public std::exception {};
	class Assert : public std::exception {};
}

#endif  /* EXCEPTIONS_H_ */
