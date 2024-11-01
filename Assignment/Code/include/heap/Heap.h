#ifndef HEAP_H
#define HEAP_H
#include <memory.h>
#include "heap/IHeap.h"
#include <sstream>
/*
 * function pointer: int (*comparator)(T& lhs, T& rhs)
 *      compares objects of type T given in lhs and rhs.
 *      return: sign of (lhs - rhs)
 *              -1: lhs < rhs
 *              0 : lhs == rhs
 *              +1: ls > rhs
 * 
 * function pointer: void (*deleteUserData)(Heap<T>* pHeap)
 *      remove user's data in case that T is a pointer type
 *      Users should pass &Heap<T>::free for "deleteUserData"
 * 
 */
template<class T>
class Heap: public IHeap<T>{
public:
    class Iterator; //forward declaration
    
protected:
    T *elements;    //a dynamic array to contain user's data
    int capacity;   //size of the dynamic array
    int count;      //current count of elements stored in this heap
    int (*comparator)(T& lhs, T& rhs);      //see above
    void (*deleteUserData)(Heap<T>* pHeap); //see above
    
public:
/*
? Notes:
    * The default value of "comparator" is 0, which means that the user does not provide any comparator.
        => always define min-heap, data type T must have operator <, >, ==
    * The default value of "comparator" isn't 0
        * Case 1: Want to define min-heap, return value of "comparator" is:
            *   -1: lhs < rhs
            *    0: lhs == rhs
            *    1: lhs > rhs
        * Case 2: Want to define max-heap, return value of "comparator" is:
            *   -1: lhs > rhs
            *    0: lhs == rhs
            *    1: lhs < rhs
    * The default value of "deleteUserData" is 0, which means that the user does not provide any method to delete user's data.
        => if T is a pointer type, the user must provide a method to delete user's data.
*/
    Heap(   int (*comparator)(T& , T&)=0, 
            void (*deleteUserData)(Heap<T>*)=0 );
    
    Heap(const Heap<T>& heap); //copy constructor 
    Heap<T>& operator=(const Heap<T>& heap); //assignment operator
    
    ~Heap();
    
    //Inherit from IHeap: BEGIN
    /*
    ! push(item): add item to the heap and maintains the heap property (min or max)
    * Exception: None
    */
    void push(T item);

    /*
    ! pop(): remove the root item from the heap and maintains the heap property (min or max)
    * Exception: If the heap is empty, throw std::underflow_error(“Calling to peek with the empty heap.“)
    */
    T pop();

    /*
    ! peek(): return the root item of the heap
    * Exception: If the heap is empty, throw std::underflow_error(“Calling to peek with the empty heap.“)
    */
    const T peek();

    /*
    ! remove(item): remove the item from the heap and maintains the heap property (min or max)
    * Exception: None
    */
    void remove(T item, void (*removeItemData)(T)=0);

    /*
    ! contains(item): check if the item is in the heap
    * Exception: None
    */
    bool contains(T item);

    /*
    ! size(): return the number of items in the heap
    * Exception: None
    */
    int size();

    /*
    ! heapify(array, size): build heap from array having size items
    * Exception: None
    */
    void heapify(T array[], int size);

    /*
    ! clear(): remove all items in the heap
    * Exception: None
    */
    void clear();

    /*
    ! empty(): check if the heap is empty
    * Exception: None
    */
    bool empty();

    /*
    ! toString(): convert the heap to string
    * Exception: None
    */
    string toString(string (*item2str)(T&)=0 );
    //Inherit from IHeap: END
    
    void println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }
    
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }
    
public:
    /* if T is pointer type:
     *     pass the address of method "free" to Heap<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  Heap<Point*> heap(&Heap<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
     */
    static void free(Heap<T> *pHeap){
        for(int idx=0; idx < pHeap->count; idx++) delete pHeap->elements[idx];
    }
    
    
private:
    bool aLTb(T& a, T& b){
        return compare(a, b) < 0;
    }
    int compare(T& a, T& b){
        if(comparator != 0) return comparator(a, b);
        else{
            if (a < b) return -1;
            else if(a > b) return 1;
            else return 0;
        }
    }
    
    /*
    ! ensureCapacity(minCapacity): ensure the capacity of the dynamic array
    * Exception: None
    */
    void ensureCapacity(int minCapacity); 

    /*
    ! swap(a, b): swap the elements at position a and b
    */
    void swap(int a, int b);

    /*
    ! reheapUp(position): maintain the heap property from position to the root
    */
    void reheapUp(int position);

    /*
    ! reheapDown(position): maintain the heap property from position to the leaf
    */
    void reheapDown(int position);

    /*
    ! getItem(item): return the index of the item in the heap
    */
    int getItem(T item);
    
    /*
    ! removeInternalData(): remove internal data
    */
    void removeInternalData();

    /*
    ! copyFrom(heap): copy from other heap
    */
    void copyFrom(const Heap<T>& heap);
    
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
    
    //Iterator: BEGIN
    class Iterator{
    private:
        Heap<T>* heap;
        int cursor;
    public:
        Iterator(Heap<T>* heap=0, bool begin=0){
            this->heap = heap;
            if(begin && (heap !=0)) cursor = 0;
            if(!begin && (heap !=0)) cursor = heap->size();
        }
        Iterator& operator=(const Iterator& iterator){
            this->heap = iterator.heap;
            this->cursor = iterator.cursor;
            return *this;
        }
        
        T& operator*(){
            return this->heap->elements[cursor];
        }
        bool operator!=(const Iterator& iterator){
            return this->cursor != iterator.cursor;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            cursor++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(void (*removeItemData)(T)=0){
            this->heap->remove(this->heap->elements[cursor], removeItemData);
        }
    };
    //Iterator: END
};


//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
Heap<T>::Heap(
        int (*comparator)(T&, T&), 
        void (*deleteUserData)(Heap<T>* ) ){
    //YOUR CODE IS HERE
    this->capacity = 10;
    this->count = 0;
    this->comparator = comparator;
    this->deleteUserData = deleteUserData;
    this->elements = new T[capacity];
}
template<class T>
Heap<T>::Heap(const Heap<T>& heap){
    //YOUR CODE IS HERE
    copyFrom(heap);
}

template<class T>
Heap<T>& Heap<T>::operator=(const Heap<T>& heap){
    //YOUR CODE IS HERE
    if(this == &heap) return *this;
    removeInternalData();
    copyFrom(heap);
    return *this;
}


template<class T>
Heap<T>::~Heap(){
    //YOUR CODE IS HERE
    removeInternalData();
}

template<class T>
void Heap<T>::push(T item){ //item  = 25
    //YOUR CODE IS HERE
    ensureCapacity(count + 1);
    elements[count] = item;
    reheapUp(count);
    count++;
}
/*
      18
     /  \
    15   13
    /
  25 
 =>
      25
     /  \
    18   13
    /
  15 
=> array: [25, 18, 13, 15, , ]
           0   1    2   3
 
 */
template<class T>
T Heap<T>::pop(){
    //YOUR CODE IS HERE
    if(empty()) throw std::underflow_error("Calling to peek with the empty heap.");
    T item = elements[0];
    elements[0] = elements[count - 1];
    count--;
    reheapDown(0);
    return item;
}

/*
      15
     /  \
    18   13
 => ReheapDown
      18
     /  \
    15   13
=> Array: [18, 15, 13, , , ]
 */

template<class T>
const T Heap<T>::peek(){
    //YOUR CODE IS HERE
    if(empty()) throw std::underflow_error("Calling to peek with the empty heap.");
    return elements[0];
}


template<class T>
void Heap<T>::remove(T item, void (*removeItemData)(T)){
    //YOUR CODE IS HERE
    int item_idx = getItem(item);
    if(item_idx == -1) return;
    if(removeItemData != 0) removeItemData(elements[item_idx]);
    elements[item_idx] = elements[count - 1];
    count--;
    reheapDown(item_idx);
}

template<class T>
bool Heap<T>::contains(T item){
    //YOUR CODE IS HERE
    if (getItem(item) == -1) return false;
    return true;
}

template<class T>
int Heap<T>::size(){
    //YOUR CODE IS HERE
    return count;
}

template<class T>
void Heap<T>::heapify(T array[], int size){
    for(int idx = 0; idx < size; idx++){
        this->push(array[idx]);
    }
}

template<class T>
void Heap<T>::clear(){
    //YOUR CODE IS HERE
    removeInternalData();
    capacity = 10;
    count = 0;
    elements = new T[capacity];
}

template<class T>
bool Heap<T>::empty(){
    //YOUR CODE IS HERE
    return count == 0;
}

template<class T>
string Heap<T>::toString(string (*item2str)(T&)){
    stringstream os;
    if(item2str != 0){
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << item2str(elements[idx]) << ",";
        if(count > 0) os << item2str(elements[count - 1]);
        os << "]";
    }
    else{
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << elements[idx] << ",";
        if(count > 0) os << elements[count - 1];
        os << "]";
    }
    return os.str();
}


//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
void Heap<T>::ensureCapacity(int minCapacity){
    if(minCapacity >= capacity){
        //re-allocate 
        int old_capacity = capacity;
        capacity = old_capacity + (old_capacity >> 2);
        try{
            T* new_data = new T[capacity];
            //OLD: memcpy(new_data, elements, capacity*sizeof(T));
            memcpy(new_data, elements, old_capacity*sizeof(T));
            delete []elements;
            elements = new_data;
        }
        catch(std::bad_alloc e){
            e.what();
        }
    }
}

template<class T>
void Heap<T>::swap(int a, int b){
    T temp = this->elements[a];
    this->elements[a] = this->elements[b];
    this->elements[b] = temp;
}

template<class T>
void Heap<T>::reheapUp(int position){
    int parent = (position - 1) / 2;
    while(position > 0 && aLTb(elements[position], elements[parent])){
        swap(position, parent);
        position = parent;
        parent = (position - 1) / 2;
    }
}

template<class T>
void Heap<T>::reheapDown(int position){
    int left = 2*position + 1;
    int right = 2*position + 2;
    int smaller = left;
    while(left < count){
        if(right < count && aLTb(elements[right], elements[left])) smaller = right;
        if(aLTb(elements[smaller], elements[position])){
            swap(position, smaller);
            position = smaller;
            left = 2*position + 1;
            right = 2*position + 2;
            smaller = left;
        }
        else break;
    }
}

template<class T>
int Heap<T>::getItem(T item){
    //YOUR CODE IS HERE
    int root_idx = 0;
    while(root_idx < count){
        if(elements[root_idx] == item) return root_idx;
        root_idx++;
    }
    return -1;
}

template<class T>
void Heap<T>::removeInternalData(){
    if(this->deleteUserData != 0) deleteUserData(this); //clear users's data if they want
    delete []elements;
}

template<class T>
void Heap<T>::copyFrom(const Heap<T>& heap){
    capacity = heap.capacity;
    count = heap.count;
    elements = new T[capacity];
    this->comparator = heap.comparator;
    this->deleteUserData = heap.deleteUserData;
    
    //Copy items from heap:
    for(int idx=0; idx < heap.count; idx++){
        this->elements[idx] = heap.elements[idx];
    }
}

#endif /* HEAP_H */

