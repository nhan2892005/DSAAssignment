/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-17
    * Version 1.0
    * This file defines the XArrayList class, 
    *       which is a generic list implementation using a dynamic array.
*/

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include "list/IList.h"
#include <memory.h>
#include <sstream>
#include <iostream>
#include <type_traits>
#define LOOP_in_range(i, start, end) for (int i = start; i < end; ++i)
#define LOOP_in_range_reverse(i, start, end) for (int i = start; i >= end; --i)
#define push_to_stringstream(item) ss << (item)
using namespace std;

template <class T>
class XArrayList : public IList<T>
{
public:
    class Iterator; // * forward declaration

protected:
    T *data;                                 // * dynamic array to store the list's items
    int capacity;                            // * size of the dynamic array
    int count;                               // * number of items stored in the array
    bool (*itemEqual)(T &lhs, T &rhs);       // * function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(XArrayList<T> *); // * function pointer: be called to remove items (if they are pointer type)

public:
    XArrayList(
        void (*deleteUserData)(XArrayList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0,
        int capacity = 10);
    XArrayList(const XArrayList<T> &list);
    XArrayList<T> &operator=(const XArrayList<T> &list);
    ~XArrayList();

    // ! Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // ! Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    Iterator begin()
    {
        return Iterator(this, 0);
    }
    Iterator end()
    {
        return Iterator(this, count);
    }

    // ! free:
    /* 
     ? if T is pointer type:
     *     pass THE address of method "free" to XArrayList<T>'s constructor:
     *     to:  remove the user's data (if needed)
     > Example:
     *  XArrayList<Point*> list(&XArrayList<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
    */
    static void free(XArrayList<T> *list)
    {
        typename XArrayList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

protected:
    void checkIndex(int index);     // * check validity of index for accessing
    void ensureCapacity(int index); // * auto-allocate if needed

    // ! equals:
    /* 
     ? if T: primitive type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     ? if T: object type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     *      Therefore, class of type T MUST override operator ==
     ? if T: pointer type:
     *      indexOf, contains: will use function pointer "itemEqual"
     *      to: compare two items of T type
     *      Therefore:
     *      (1): must pass itemEqual to the constructor of XArrayList
     *      (2): must define a method for comparing
     *           the content pointed by two pointers of type T
     *          See: definition of "equals" of class Point for more detail
     */
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

    void copyFrom(const XArrayList<T> &list);

    void removeInternalData();

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    // ! Iterator: BEGIN
    class Iterator
    {
    private:
        int cursor;
        XArrayList<T> *pList;

    public:
        Iterator(XArrayList<T> *pList = 0, int index = 0)
        {
            this->pList = pList;
            this->cursor = index;
        }
        Iterator &operator=(const Iterator &iterator)
        {
            cursor = iterator.cursor;
            pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            T item = pList->removeAt(cursor);
            if (removeItemData != 0)
                removeItemData(item);
            cursor -= 1; // MUST keep index of previous, for ++ later
        }

        T &operator*()
        {
            return pList->data[cursor];
        }
        bool operator!=(const Iterator &iterator)
        {
            return cursor != iterator.cursor;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            this->cursor++;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    // ! Iterator: END
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
XArrayList<T>::XArrayList(
    void (*deleteUserData)(XArrayList<T> *),
    bool (*itemEqual)(T &, T &),
    int capacity)
    :deleteUserData(deleteUserData), itemEqual(itemEqual), count(0), capacity(capacity)
{
    this->data = new T[this->capacity];
}

template <class T>
void XArrayList<T>::copyFrom(const XArrayList<T> &list)
{
    /*
     * Copies the contents of another XArrayList into this list.
     * Initializes the list with the same capacity as the source list and copies all elements.
     * Also duplicates user-defined comparison and deletion functions, if applicable.
     */

    // Check self-assignment
    if (this == &list) {
        return;
    }

    // Delete current data if it exists
    if (this->count > 0) {
        this->removeInternalData();
    }

    // Copies the contents into this list
    this->capacity = list.capacity;
    this->count = list.count;
    this->itemEqual = list.itemEqual;
    this->deleteUserData = list.deleteUserData;

    // This for copy data array.
    this->data = new T[this->capacity];
    for (int i = 0; i < this->count; i++) {
        this->data[i] = list.data[i];
    }
}

template <class T>
void XArrayList<T>::removeInternalData()
{
    /*
     * Clears the internal data of the list by deleting the dynamic array and any user-defined data.
     * If a custom deletion function is provided, it is used to free the stored elements.
     * Finally, the dynamic array itself is deallocated from memory.
     */

    // Use default delete data if it exists
    if (this->deleteUserData != nullptr) {
        this->deleteUserData(this);
    }

    // Reset parameters
    this->count = 0;
}

template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list)
{
    this->count = 0;
    copyFrom(list);
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list)
{
    delete[] data;
    copyFrom(list);
    return *this;
}

template <class T>
XArrayList<T>::~XArrayList()
{
    removeInternalData();
    delete[] data;
}

template <class T>
void XArrayList<T>::add(T e)
{
    /*
    * Objectives: add an item to the end of the list
    */
    // Call to expansion mechanism of object (if list is full then expand it)
    ensureCapacity(count);
    
    // Add the item to the end of the list
    data[count] = e;
    
    // Increase current size of the list
    count++;
}

template <class T>
void XArrayList<T>::add(int index, T e)
{
    /*
    * Objectives: add an item to the list at a specific index
    * Exception: throw an out_of_range exception if the index is invalid
    */

    // Call to expansion mechanism of object (if list is full then expand it)
    ensureCapacity(index);

    // Shift all elements to the right from index to count
    LOOP_in_range_reverse(i, count, index + 1) {
        data[i] = data[i - 1];
    }
    
    // Add the item to the list at index
    data[index] = e;

    // Increase current size of the list
    count++;
}

template <class T>
T XArrayList<T>::removeAt(int index)
{
    /*
    * Objectives: remove an item from the list at a specific index
    * Return: the removed item
    * Exception: throw an out_of_range exception if the index is invalid
    */
    // Check index is valid or not
    checkIndex(index);
    
    // Save the item to be removed
    T removedItem = data[index];
    
    // Shift all elements to the left from index + 1 to count
    LOOP_in_range(i, index, count - 1) {
        data[i] = data[i + 1];
    }
    
    // Decrease current size of the list
    count--;
    
    // Return the removed item
    return removedItem;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T))
{
    /*
    * Objectives: remove an item from the list
    * Return: true if the item is removed, otherwise false
    * Note: if the item is a pointer type, the removeItemData function will be called to remove the item data
    *      if it is not null
    */
    // Find the index of the item
    int index = indexOf(item);

    // Check if the item is in the list
    if (index == -1) return false;
    
    // Remove the item at the index
    T removedItem = removeAt(index);
    
    // Call the removeItemData function if it exists
    if (removeItemData != nullptr) {
        removeItemData(removedItem);
    }
    
    return true;
}

template <class T>
bool XArrayList<T>::empty()
{
    /*
    * Objectives: check if the list is empty
    * Return: true if the list is empty, otherwise false
    */
    return count == 0;
}

template <class T>
int XArrayList<T>::size()
{
    /*
    * Objectives: get the number of items in the list
    * Return: the number of items in the list
    */
    return count;
}

template <class T>
void XArrayList<T>::clear()
{
    /*
    * Objectives: clear the list
    */
    removeInternalData();
}

template <class T>
T &XArrayList<T>::get(int index)
{
    /*
    * Objectives: get an item from the list at a specific index
    * Return: the item at the index
    * Exception: throw an out_of_range exception if the index is invalid
    */
    checkIndex(index);
    return data[index];
}

template <class T>
int XArrayList<T>::indexOf(T item)
{
    /*
    * Objectives: get the index of an item in the list
    * Return: the index of the item if it exists, otherwise -1
    */
    LOOP_in_range(i, 0, count) {
        if (equals(data[i], item, itemEqual)) {
            return i;
        }
    }
    return -1;
}

template <class T>
bool XArrayList<T>::contains(T item)
{
    /*
    * Objectives: check if the list contains an item
    */
    return indexOf(item) != -1;
}

template <class T>
string XArrayList<T>::toString(string (*item2str)(T &))
{
    /*
     * Converts the array list into a string representation, formatting each element using a user-defined function.
     * If no function is provided, it uses the default string representation for each element.
     * Example: Given an array list with elements {1, 2, 3} and a function that converts integers to strings, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function pointer for converting items of type T to strings. If null, default to the string conversion of T.
     * @return A string representation of the array list with elements separated by commas and enclosed in square brackets.
     */

    stringstream ss;
    push_to_stringstream("[");
    
    LOOP_in_range(i, 0, count) {        
        if (item2str != nullptr) {
            push_to_stringstream(item2str(data[i]));
        } else {
            push_to_stringstream(data[i]);
        }
        if (i < count - 1) {
            push_to_stringstream(", ");
        }
    }
    
    push_to_stringstream("]");
    return ss.str();
}

//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////
template <class T>
void XArrayList<T>::checkIndex(int index)
{
    /*
     * Validates whether the given index is within the valid range of the list.
     * Throws an std::out_of_range exception if the index is negative or exceeds the number of elements.
     * Ensures safe access to the list's elements by preventing invalid index operations.
     */
    if (index < 0 || index >= count) {
        throw out_of_range("Index is out of range!");
    }
}
template <class T>
void XArrayList<T>::ensureCapacity(int index)
{
    /*
     * Ensures that the list has enough capacity to accommodate the given index.
     * If the index is out of range, it throws an std::out_of_range exception. 
     * If the index exceeds the current capacity, reallocates the internal array with increased capacity, copying the existing elements to the new array.
     * In case of memory allocation failure, catches std::bad_alloc.
     */
    if (index != count) {
        checkIndex(index);
    }

    if (count == capacity) {
        int newCapacity = capacity + 100;
        try {
            T* newData = new T[newCapacity];
            
            for (int i = 0; i < count; i++) {
                newData[i] = data[i];
            }
            
            delete[] data;
            data = newData;
            capacity = newCapacity;
        } catch (const bad_alloc& e) {
            throw runtime_error("Memory allocation failed: " + string(e.what()));
        }
    } else // Monitor if capacity too large
    {
        if (capacity > 1000 && count < capacity / 2) {
            int newCapacity = capacity / 2;
            T* newData = new T[newCapacity];
            for (int i = 0; i < count; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
    }
}

#endif /* XARRAYLIST_H */
