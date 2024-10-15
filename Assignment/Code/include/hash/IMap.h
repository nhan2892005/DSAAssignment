/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IMap.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 21:53
 */

#ifndef IMAP_H
#define IMAP_H

#include "list/DLinkedList.h"
#include <string>
using namespace std;


class KeyNotFound: public std::exception{
private:
    string desc;
public:
    KeyNotFound(string desc){
        this->desc = desc;
    }
    const char * what () const throw (){
        return desc.c_str();
    }
};


template<class K, class V>
struct Pair{
    K key;
    V value;
    Pair(K key, V value){
        this->key = key;
        this->value = value;
    }
    Pair(const Pair& pair){
        this->key = pair.key;
        this->value = pair.value;
    }
    Pair& operator=(const Pair& pair){
        this->key = pair.key;
        this->value = pair.value;
    }
};

template<class K, class V>
class IMap {
public:
    virtual ~IMap(){};
    //
    /*
    put(K key, V value): 
    if key is not in the map: 
        + add a mapping key->value to the map
        + return value
    else:
        + associate key with the new value (passed as parameter) 
        + return the old value
    */
    virtual V put(K key, V value)=0;
    
    /*
    get(K key):
    if key in the map: return the associated value
     else: KeyNotFound exception thrown

    */
    virtual V& get(K key)=0;
    
    /*
    remove(K key):
    if key is in the map: remove it from the map, and return the associated value
    else: KeyNotFound exception thrown
    
    >> deleteKeyInMap(K key): delete key stored in map; in cases, K is a pointer type
    */
    virtual V remove(K key, void (*deleteKeyInMap)(K)=0)=0;
    
    /*
    remove(K key, V value):
    if there is a mapping key->value in the map: remove it and return true
    else: return false
    
    >> deleteKeyInMap(K key): delete key stored in map; in cases, K is a pointer type
    >> deleteValueInMap(V value): delete key stored in map; in cases, V is a pointer type
    */
    virtual bool remove(K key, V value, void (*deleteKeyInMap)(K)=0, void (*deleteValueInMap)(V)=0)=0;
    
    /*
    containsKey(K key):
    if key is in the map: return true
    else: return false
    */
    virtual bool containsKey(K key)=0;
    
    /*
    containsKey(V value):
    if value is in the map: return true
    else: return false
    */
    virtual bool containsValue(V value)=0;
    
    /*
    empty():
    return true if the map is empty
    else: return false
    */
    virtual bool empty()=0;
    
    /*
    size():
    return number of pairs key->value
    */
    virtual int size()=0;
    
    /*
    clear():
    clear all pairs key->value in the map
    */
    virtual void clear() = 0;
    
    /*
    toString(): 
    return a string representing the map
    
    >> key2str(K& key): convert key to string; if not supplied then K must support extraction operator (<<)
    >> value2str(V& value): convert value to string; if not supplied then V must support extraction operator (<<)
    */
    virtual string toString(string (*key2str)(K&)=0, string (*value2str)(V&)=0 )=0;
    
    /*
     * keys(): return a set of keys stored in the map
     */
    virtual DLinkedList<K> keys()=0;
    
    /*
     * values(): return a set of values stored in the map
     */
    virtual DLinkedList<V> values()=0;
    
    /*
     * clashes(): return a list containing the collision count for each address
     */
    virtual DLinkedList<int> clashes()=0;
};


#endif /* IMAP_H */

