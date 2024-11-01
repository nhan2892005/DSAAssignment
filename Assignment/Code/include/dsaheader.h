/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dsaheader.h
 * Author: ltsach
 *
 * Created on October 12, 2024, 1:57 PM
 */

#ifndef DSAHEADER_H
#define DSAHEADER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include "hash/xMap.h"

template<class T>
using xvector = XArrayList<T>;
template<class T>
using xlist = DLinkedList<T>;
template<class K, class V>
using xmap = xMap<K, V>;


#endif /* DSAHEADER_H */

