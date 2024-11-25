/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StraightSelectionSortDemo.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:10
 */

#ifndef STRAIGHTSELECTIONSORTDEMO_H
#define STRAIGHTSELECTIONSORTDEMO_H


#include <iostream>
#include <iomanip>
#include "sorting/StraightSelectionSort.h"
using namespace std;

void straightSelDemo1(){
    int values[] = {45, 97, 12, 2, 39, 3, 37, 87};
    cout << left << setw(22) << "Before:";
    for(int idx=0; idx < 8; idx++){
        cout << values[idx] << ", ";
    }
    cout << endl;
    
    StraightSelectionSort<int> sortAlg;
    sortAlg.sort(values, 8, &SortSimpleOrder<int>::compare4Ascending);
    
    cout << left << setw(22) << "After, Ascending:";
    for(int idx=0; idx < 8; idx++){
        cout << values[idx] << ", ";
    }
    cout << endl;
    
    sortAlg.sort(values, 8, &SortSimpleOrder<int>::compare4Desending);
    
    cout << left << setw(22) << "After, Descending:";
    for(int idx=0; idx < 8; idx++){
        cout << values[idx] << ", ";
    }
    cout << endl;
}


#endif /* STRAIGHTSELECTIONSORTDEMO_H */

