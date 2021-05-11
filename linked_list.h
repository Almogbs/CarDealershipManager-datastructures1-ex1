#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define LEFT 0
#define RIGHT 1


namespace DataStructures {

    template <class T>
    struct ListNodeL{
        ListNodeL<T>* next;
        ListNodeL<T>* previous;
        T element;
    };

    template<class T>
    class LinkedList {
    private:
        ListNodeL<T>* head;
        ListNodeL<T>* iterator;
        int size;

    public:
        //T* (*copy_function)(const T&);
        //LinkedList(T* (*new_copy_function)(const T&));
        //LinkedList(const LinkedList<T>& list);
        LinkedList() : head(nullptr), iterator(nullptr), size(0);
        ~LinkedList();
        int getSize();
        bool contains(const T& element);
        void insert(NodeL<T>* father, int direction);
        void removeElement(const T& element);
        T getFirst();
        T getNext();
        T getPrevious();
        T getElement(const T& element);
    };
}

using DataStructures::ListNodeL;
using DataStructures::LinkedList;
/*
template<class T>
LinkedList<T>::LinkedList(T* (*new_copy_function)(const T&))
{
    head = nullptr ,iterator = nullptr, copy_function = new_copy_function;
}


template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list){
    if(list.head == nullptr){
        head = nullptr ,iterator = nullptr;//, copy_function = list.copy_function;
        return;
    }
    //copy_function = list.copy_function;
    head = new NodeL<T>;
    head->element = list.getElement();
    head->next = nullptr;
    head->previous = nullptr;
    NodeL<T>* to_add = list.head->next;
    NodeL<T>* temp_head_ptr = head;

    while(to_add){
        NodeL<T>* new_element = new NodeL<T>;
        new_element->element = copy_function(*to_add->element);
        new_element->next = nullptr;
        temp_head_ptr->next = new_element;
        temp_head_ptr = temp_head_ptr->next;
        to_add = to_add->next;
    }
}
*/

template<class T>
LinkedList<T>::~LinkedList(){
    while(head)
    {
        NodeL<T>* to_delete = head;
        head = head->next;
        delete to_delete->element;
        delete to_delete;
    }
}

template<class T>
int LinkedList<T>::getSize(){
    return this->size;
}

template<class T>
T LinkedList<T>::getElement(const T& element){
    if(head == nullptr){
        return nullptr;
    }
    NodeL<T>* temp = head;
    while(temp){
        if(*temp->element == element){
            return temp->element;
        }
        temp = temp->next;
    }
    return nullptr;
}

template<class T>
bool LinkedList<T>::contains(const T& element){
    if(head == nullptr){
        return false;
    }
    NodeL<T>* temp = head;
    while(temp){
        if(*temp->element == element){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

template<class T>
void LinkedList<T>::insert(NodeL<T>* father, int direction, modelzerotreeptr element)
{
    if (father == nullptr)
    {
        head = new NodeL<T>;
        head->element = element;
        head->next = nullptr;
        return;
    }
    if (direction = LEFT)
    {
        NodeL<T>* new_nodeL = new NodeL<T>;
        new_nodeL->element = element;
        new_nodeL->next = father;
        fathr->previous = new_nodeL;
        new_nodeL->previous = father->previous;
        if (head = father)
        {
            head = new_nodeL;
        }
        else
        return;
    }
    if (direction = RIGHT)
    {
        NodeL<T>* new_nodeL = new NodeL<T>;
        new_nodeL->element = element;
        new_nodeL->next = father->next;
        new_nodeL->previous = father->previous;
        if (head = father)
        {
            head = new_nodeL;
        }
        return;
    }
    NodeL<T>* temp = head;
    while (temp->next && element > *temp->next->element)
    {
        temp = temp->next;
    }
    NodeL<T>* to_add = new NodeL<T>;
    to_add->element = copy_function(element);
    to_add->next = temp->next;
    temp ->next = to_add;
    return;
}


template<class T>
void LinkedList<T>::removeElement(const T& element){
    NodeL<T>* temp = head;
    if(!temp){
        return;
    }
    if(*temp->element == element)
    {
        head = head->next;
        delete temp->element;
        delete temp;
        return;
    }
    while(temp->next && *temp->next->element != element)
    {
        temp = temp->next;
    }
    if(!temp->next)
    {
        return;
    }
    NodeL<T>* to_delete = temp->next;
    temp->next = to_delete->next;
    delete to_delete->element;
    delete to_delete;
    return;
}


template<class T>
T* LinkedList<T>::getFirst(){
    if(!head){
        return nullptr;
    }
    iterator = head;
    return head->element;
}


template<class T>
T* LinkedList<T>::getNext(){
    if(!iterator->next){
        return nullptr;
    }
    iterator = iterator->next;
    return iterator->element;
}


#define LL_FOREACH(type, iterator, listname) \
    for(type iterator = listname->getFirst() ; \
        iterator ;\
        iterator = listname->getNext())

#endif /**  LINKED_LIST_H  */
