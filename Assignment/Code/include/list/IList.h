/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IList.h
 * Author: LTSACH
 *
 * Created on 13 August 2020, 13:54
 */


#ifndef ILIST_H
#define ILIST_H
#include <string>
using namespace std;

template<class T>
class IList{
public:
    virtual ~IList(){};
    /* add(T e): append item "e" to the list
     */
    virtual void    add(T e)=0;
    
    
    
    /* add(int index, T e): insert item "e" at location "index";
     *      location is an integer started from 0
     */
    virtual void    add(int index, T e)=0;
    
    
    
    /* removeAt(int index): remove the item at location "index"
     * 
     * return:
     *  >> the item stored at index
     *  >> throw an exception (std::out_of_range) if index is invalid
     */
    virtual T       removeAt(int index)=0;
    
    
    /* removeItem(T item, void (*removeItemData)(T)=0): remove item stored in the list
     *   >> removeItemData: a function pointer (maybe NULL, default),
     *          that will be called inside of removeItem to delete the item's data stored in the list
     *   >> removeItemData, why need? because we DO NOT type T is a pointer or not.
     * 
     * return:
     *   >> true if "item" stored in the list; 
     *   >> otherwise, return false
     */
    virtual bool    removeItem(T item, void (*removeItemData)(T)=0)=0;
    
    
    
    /* empty(): return true if the list is empty; otherwise, return false
     */
    virtual bool    empty()=0;
    
    
    
    /* size(): return number of items stored in the list
     */
    virtual int     size()=0;
    
    
    
    /* clear(): make the list empty by clearing all data and putting the list to the initial condition
     */
    virtual void    clear()=0;
    
    
    
    /* get(int index): return a reference to the item at location "index"
     *      if index is invalid, this function will throw an exception "std::out_of_range"
     * 
     * NOTE: programmers can change the item returned by this function
     */
    virtual T&      get(int index)=0;
    
    
    /* indexOf(T item): return the index of item
     *      if item is not found, then return -1
     */
    virtual int     indexOf(T item)=0;
    
    
    
    /* contains(T item): return true if the list contains "item", else: return false
     */
    virtual bool    contains(T item)=0;
    
    
    
    /* toString(string (*item2str)(T&)=0 ): return a string describing the list.
     *    >> We do not know the item type, so we do not know how to convert each item to string.
     *       Therefore, we need a pointer to a function
     *          that can convert the item (passed to that function) to a string
     */
    virtual string  toString(string (*item2str)(T&)=0 )=0;
};
#endif /* ILIST_H */

