#ifndef QUEUE_H
#define QUEUE_H
#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"
#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

template<class T>
class Queue: public IDeck<T>{
public:
    class Iterator; //forward declaration
    
protected:
    DLinkedList<T> list;
    void (*deleteUserData)(DLinkedList<T>*);
    bool (*itemEqual)(T& lhs, T& rhs);
    
public:
    Queue(
            void (*deleteUserData)(DLinkedList<T>*)=0, 
            bool (*itemEqual)(T&, T&)=0){
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
        list = DLinkedList<T>(deleteUserData, itemEqual);
        list.setDeleteUserDataPtr(deleteUserData);
    }
    /*
    ! push(T item)
    ? Functional:
        * Add an item to the rear of the queue
    ? Parameters:
        * T item: The item to be added
    ? Return:
        * void: add the item to the rear of the list
    */
    void push(T item){
        list.add(item);
    }

    /*
    ! pop()
    ? Functional:
        * Remove the front item from the queue
    ? Return:
        * T: the front item
    */
    T pop(){
        if(list.empty()) throw Underflow("Queue");
        return list.removeAt(0);
    }

    /*
    ! peek()
    ? Functional:
        * Return the front item of the queue
    ? Return:
        * T: the front item
    */
    T& peek(){
        if(list.empty()) throw Underflow("Queue");
        return list.get(0);
    }

    /*
    ! empty()
    ? Functional:
        * Check if the queue is empty
    ? Return:
        * bool: true if the queue is empty, false otherwise
    */
    bool empty(){
        return list.empty();
    }

    /*
    ! size()
    ? Functional:
        * Return the number of items in the queue
    ? Return:
        * int: the number of items in the queue
    */
    int size(){
        return list.size();      
    }

    /*
    ! clear()
    ? Functional:
        * Remove all items in the queue
    ? Return:
        * void: remove all items in the queue
    */
    void clear(){
        list.clear();
    }

    /*
    ! remove(T item)
    ? Functional:
        * Remove an item from the queue
    ? Parameters:
        * T item: the item to be removed
    ? Return:
        * bool: true if the item is removed successfully, false otherwise
    */
    bool remove(T item){
        return list.removeItem(item);
    }

    /*
    ! contains(T item)
    ? Functional:
        * Check if the item is in the queue
    ? Parameters:
        * T item: the item to be checked
    ? Return:
        * bool: true if the item is in the queue, false otherwise
    */
    bool contains(T item){
        return false;
    }

    /*
    ! toString(string (*item2str)(T&)=0)
    ? Functional:
        * Convert the queue to a string
    ? Parameters:
        * string (*item2str)(T&): a function to convert an item to a string
    ? Return:
        * string: the string representation of the queue
    */
    string  toString(string (*item2str)(T&)=0 ){
        stringstream os;
        os << "FRONT-TO-REAR: " << list.toString(item2str);
        return os.str();
    }
    void println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }
    
    ///
    
    Iterator front(){
        return Iterator(this, true);
    }
    Iterator rear(){
        return Iterator(this, false);
    }
    
private:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T& )){
        if(itemEqual == 0) return lhs == rhs;
        else return itemEqual(lhs, rhs);
    }
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////

public:
//Iterator: BEGIN
    class Iterator{
    private:
        Queue<T>* queue;
        typename DLinkedList<T>::Iterator listIt;
    public:
        Iterator(Queue<T>* queue=0, bool begin=true){
            this->queue = queue;
            if(begin){
                if(queue != 0) this->listIt = queue->list.begin();
                else this->listIt = 0;
            }
            else{
                if(queue != 0) this->listIt = queue->list.end();
                else this->listIt = 0;
            }
        }
        Iterator& operator=(const Iterator& iterator ){
            this->queue = iterator.queue;
            this->listIt = iterator.listIt;
            return *this;
        }
        
        T& operator*(){
            return *(this->listIt);
        }
        bool operator!=(const Iterator& iterator){
            return this->listIt != iterator.listIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            listIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(void (*removeItem)(T)=0){
            listIt.remove(removeItem);
        }
    };
    //Iterator: END
};
#endif /* QUEUE_H */