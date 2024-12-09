#ifndef STACK_H
#define STACK_H
#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

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

    /*
    ! push(item: T)
    ? Functional:
        * Add item to the top of the stack
    ? Parameters:
        * item: T - The item to be added
    ? Return:
        * void
    */
    void push(T item){
        list.add(0, item);
    }

    /*
    ! pop()
    ? Functional:
        * Remove and return the top item
    ? Parameters:
        * None
    ? Return:
        * T - The top item
    */
    T pop(){
        return list.removeAt(0);
    }

    /*
    ! peek()
    ? Functional:
        * Return the top item
    ? Parameters:
        * None
    ? Return:
        * T - The top item
    */
    T& peek(){
        return list.get(0);
    }    

    /*
    ! empty()
    ? Functional:
        * Check if the stack is empty
    ? Parameters:
        * None
    ? Return:
        * bool - True if the stack is empty, False otherwise
    */
    bool empty(){
        return list.empty();
    }

    /*
    ! size()
    ? Functional:
        * Return the number of items in the stack
    ? Parameters:
        * None
    ? Return:
        * int - The number of items in the stack
    */
    int size(){
        return list.size();
    }

    /*
    ! clear()
    ? Functional:
        * Remove all items in the stack
    ? Parameters:
        * None
    ? Return:
        * void
    */
    void clear(){
        list.clear();
    }

    /*
    ! remove(item: T)
    ? Functional:
        * Remove the first occurrence of the item
    ? Parameters:
        * item: T - The item to be removed
    ? Return:
        * bool - True if the item is removed successfully, False otherwise
    */
    bool remove(T item){
        return list.removeItem(item);
    }

    /*
    ! contains(item: T)
    ? Functional:
        * Check if the item is in the stack
    ? Parameters:
        * item: T - The item to be checked
    ? Return:
        * bool - True if the item is in the stack, False otherwise
    */
    bool contains(T item){
        return list.contains(item);
    }

    /*
    ! toString(item2str: string (*item2str)(T&)=0)
    ? Functional:
        * Convert the stack to a string
    ? Parameters:
        * item2str: string (*item2str)(T&) - The function pointer to convert an item to a string
    ? Return:
        * string - The string representation of the stack
    */
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