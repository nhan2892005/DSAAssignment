#ifndef QUEUE_H
#define QUEUE_H
#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"
// template<class T>
// class Queue {
// protected:
//     DLinkedList<T> list;
// public:
//     Queue(  void (*deleteUserData)(DLinkedList<T> *) = 0,
//             bool (*itemEqual)(T &, T &) = 0)
//         :list(deleteUserData, itemEqual){}
        
//     void push(T item) {
//         this->list.add(item);
//     }

//     T pop() {
//         if (this->empty()) throw std::out_of_range("Empty Queue");
//         return this->list.removeAt(0);
//     }

//     T top() {
//         if (this->empty()) throw std::out_of_range("Empty Queue");
//         return this->list.get(0);
//     }

//     bool empty() {
//         return this->list.empty();
//     }

//     int size() {
//         return this->list.size();
//     }

//     void clear() {
//         return this->list.clear();
//     }
// };
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
    }
    void push(T item){
        //TODO: add item to the rear
    }
    T pop(){
        //TODO: remove and return the front item
        return T{}; //Placeholder
    }
    T& peek(){
        //TODO: return the front item
        return T{}; //Placeholder
    }
    bool empty(){
        //TODO: check if the queue is empty
        return false;
    }
    int size(){
        //TODO: return the number of items in the queue
        return 0;       
    }
    void clear(){
        //TODO: remove all items in the queue
    }
    bool remove(T item){
        //TODO: remove the item from the queue
        //     return true if the item is removed successfully
        return false;
    }
    bool contains(T item){
        //TODO: check if the item is in the queue
        return false;
    }
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