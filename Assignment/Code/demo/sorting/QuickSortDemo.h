/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QuickSortDemo.h
 * Author: LTSACH
 *
 * Created on 14 September 2020, 18:27
 */

#ifndef QUICKSORTDEMO_H
#define QUICKSORTDEMO_H



#include <iostream>
#include <iomanip>
#include "sorting/QuickSort.h"
using namespace std;

void QuickSortDemo1(){
    int values[] = {45, 97, 12, 2, 39, 3, 37, 87};
    cout << left << setw(22) << "Before:";
    for(int idx=0; idx < 8; idx++){
        cout << values[idx] << ", ";
    }
    cout << endl;
    
    QuickSort<int> sortAlg;
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

#endif /* QUICKSORTDEMO_H */

