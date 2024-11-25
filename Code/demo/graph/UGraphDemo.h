/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UGraphDemo.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:26
 */

#ifndef UGRAPHDEMO_H
#define UGRAPHDEMO_H

#include <string>
#include <iostream>
#include <iomanip>
#include "graph/UGraphModel.h"
using namespace std;


bool strEQ(string& lhs, string& rhs){
    return lhs.compare(rhs) == 0;
}
string vertex2str(string& item){
    return item;
}

void ugraphDemo1(){
    UGraphModel<string> model(&strEQ, &vertex2str);
    
    model.add("HCM");
    model.add("LongAn");
    model.add("BinhDuong");
    model.add("DongNai");
    model.add("BinhThuan");
    model.connect("HCM", "LongAn");
    model.connect("HCM", "BinhDuong");
    model.connect("HCM", "DongNai");
    model.connect("DongNai", "BinhThuan");
    
    model.println();
}

bool charEQ(char& lhs, char& rhs){
    return lhs == rhs;
}
string char2str(char& item){
    stringstream os;
    os << item;
    return os.str();
}

void ugraphDemo2(){
    UGraphModel<char> model(&charEQ, &char2str);
    model.add('A');
    model.add('B');
    model.add('C');
    model.add('D');
    model.add('E');
    model.add('F');
    model.connect('A', 'B', 6);
    model.connect('A', 'C', 3);

    model.connect('B', 'C', 2);
    model.connect('B', 'D', 5);

    model.connect('C', 'D', 3);
    model.connect('C', 'E', 4);

    model.connect('D', 'E', 2);
    model.connect('D', 'F', 3);

    model.connect('E', 'F', 5);

    model.println();

    model.disconnect('C', 'A');
    model.disconnect('C', 'B');
    model.disconnect('C', 'D');
    model.disconnect('C', 'E');

    model.println();
}


void ugraphDemo3(){
    UGraphModel<char> model(&charEQ, &char2str);
    model.add('A');
    model.add('B');
    model.add('C');
    model.add('D');
    model.add('E');
    model.add('F');
    model.connect('A', 'B', 6);
    model.connect('A', 'C', 3);

    model.connect('B', 'C', 2);
    model.connect('B', 'D', 5);

    model.connect('C', 'D', 3);
    model.connect('C', 'E', 4);

    model.connect('D', 'E', 2);
    model.connect('D', 'F', 3);

    model.connect('E', 'F', 5);

    model.println();
    
    cout << "Minimum Spanning Tree:" << endl;
    UGraphAlgorithm<char> mst;
    UGraphModel<char> tree = mst.minSpanningTree(&model);
    tree.println();
}


#endif /* UGRAPHDEMO_H */

