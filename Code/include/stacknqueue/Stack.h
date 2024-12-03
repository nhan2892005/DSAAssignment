#ifndef STACK_H
#define STACK_H
#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

// template<class T>
// class Stack {
// protected:
//     DLinkedList<T> list;

//     bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
//     void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

// public:
//     Stack(  void (*deleteUserData)(DLinkedList<T> *) = 0,
//             bool (*itemEqual)(T &, T &) = 0)
//         :list(deleteUserData, itemEqual){}

//     void push(T item) {
//         this->list.add(0, item);
//     }

//     T pop() {
//         if (this->list.empty()) throw Underflow("Stack");
//         return this->list.removeAt(0);
//     }

//     T top() {
//         if (this->list.empty()) throw Underflow("Stack");
//         return this->list.get(0);
//     }

//     bool empty() {
//         return this->list.empty();
//     }

//     int size() {
//         return this->list.size();
//     }

//     void clear() {
//         this->list.clear();
//     }
// };

template<class T>
class Stack: public IDeck<T>{
public:
    class Iterator; //forward declaration
    
protected:
    DLinkedList<T> list; //internal list
    void (*deleteUserData)(DLinkedList<T>*); //function pointer: be called to remove items (if they are pointer type)
    bool (*itemEqual)(T& lhs, T& rhs); //function pointer: test if two items (type: T&) are equal or not
    
public:
    Stack(  void (*deleteUserData)(DLinkedList<T>*)=0, 
            bool (*itemEqual)(T&, T&)=0){
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
        list = DLinkedList<T>(deleteUserData, itemEqual);
        list.setDeleteUserDataPtr(deleteUserData);
    }
    void push(T item){
        //TODO: add item to the top
        list.add(item, 0);
    }
    T pop(){
        //TODO: remove and return the top item
        return list.removeAt(0);
    }
    T& peek(){
        //TODO: return the top item
        return list.get(0);
    }    
    bool empty(){
        //TODO: check if the stack is empty
        return list.empty();
    }
    int size(){
        //TODO: return the number of items in the stack
        return list.size();
    }
    void clear(){
        //TODO: remove all items in the stack
        list.clear();
    }
    bool remove(T item){
        //TODO: remove the first occurrence of the item
        //     return true if the item is removed successfully
        return list.remove(item);
    }
    bool contains(T item){
        //TODO: check if the item is in the stack
        return list.contains(item);
    }
    string  toString(string (*item2str)(T&)=0 ){
        stringstream os;
        os << "FROM TOP: " << list.toString(item2str);
        return os.str();
    }
    void println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }
    ///
    Iterator top(){
        return Iterator(this, true);
    }
    Iterator bottom(){
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
        Stack<T>* stack;
        typename DLinkedList<T>::Iterator listIt;
    public:
        Iterator(Stack<T>* stack=0, bool begin=true){
            this->stack = stack;
            if(begin){
                if(stack != 0) this->listIt = stack->list.begin();
                else this->listIt = 0;
            }
            else{
                if(stack != 0) this->listIt = stack->list.end();
                else this->listIt = 0;
            }
        }
        Iterator& operator=(const Iterator& iterator ){
            this->stack = iterator.stack;
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

#endif