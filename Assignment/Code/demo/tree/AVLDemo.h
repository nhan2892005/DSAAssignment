/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AVLTreeDemo.h
 * Author: LTSACH
 *
 * Created on 12 September 2020, 20:49
 */

#ifndef AVLTREEDEMO_H
#define AVLTREEDEMO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "tree/AVL.h"
#include "util/sampleFunc.h"
using namespace std;


void LHofLH(){
    AVL<int, int*> tree;
    int keys[] = {50, 45, 40};
    for(int idx=0; idx < 3; idx++) tree.add(keys[idx]);
    tree.println();
}
void RHofLH(){
    AVL<int, int*> tree;
    int keys[] = {50, 40, 45};
    for(int idx=0; idx < 3; idx++) tree.add(keys[idx]);
    tree.println();
}
void RHofRH(){
    AVL<int, int*> tree;
    int keys[] = {40, 45, 50};
    for(int idx=0; idx < 3; idx++) tree.add(keys[idx]);
    tree.println();
}
void LHofRH(){
    AVL<int, int*> tree;
    int keys[] = {40, 50, 45};
    for(int idx=0; idx < 3; idx++) tree.add(keys[idx]);
    tree.println();
}

/*
 * SEQ: keys1[] = {50, 40, 60, 30, 45, 20};
 * EXP: (40:EH  (30:LH [20:EH ][.]) (50:EH [45:EH ][60:EH ]) ) 
 * 
 * int keys2[] = {50, 40, 60, 30, 45, 35};
 * EXP: (40:EH  (30:RH [.][35:EH ]) (50:EH [45:EH ][60:EH ]) ) 
 */
void leftBalance1(){
    AVL<int, int*> tree;
    int keys1[] = {50, 40, 60, 30, 45, 20};
    int keys2[] = {50, 40, 60, 30, 45, 35};
    for(int idx=0; idx < 6; idx++) tree.add(keys1[idx]);
    tree.println();
    tree.clear();
    for(int idx=0; idx < 6; idx++) tree.add(keys2[idx]);
    tree.println();
}

void leftBalance2(){
    AVL<int, int*> tree;
    int keys1[] = {50, 40, 60, 30, 45, 20};
    int nodes_exp1[] = {40, 30, 50, 20, 45, 60};
    string bfactor_exp1[] = {sEH, sLH, sEH, sEH, sEMPTY, sEH, sEH}; 
    int keys2[] = {50, 40, 60, 30, 45, 35};
    int nodes_exp2[] = {40, 30, 50, 35, 45, 60};
    string bfactor_exp2[] = {sEH, sRH, sEH, sEMPTY, sEH, sEH, sEH}; 

    for(int idx=0; idx < 6; idx++) tree.add(keys1[idx] );
    List<string> factor1 = tree.bfactor();
    factor1.contains(bfactor_exp1, 7);

    List<int> nodes1 = tree.bfsKey();
    nodes1.contains(nodes_exp1, 6);

    tree.clear();
    for(int idx=0; idx < 6; idx++) tree.add(keys2[idx]);

    List<string> factor2 = tree.bfactor();
    factor2.contains(bfactor_exp2, 7);

    List<int> nodes2 = tree.bfsKey();
    nodes2.contains(nodes_exp2, 6);
}
/*
     Current tree:
                    45
                   /  \
                 33    72
                      /  \
                     55  84

    Adding 75:
                    45    : RH of RH
                   /  \
                 33    72
                      /  \
                     55  84
                        /
                       75    
     
    rotateLeft(at 45):
                    72
                   /  \
                 45    84
                / \    /
              33  55  75        
    
*/
void AVLDemo1(){
    AVL<int, int*> tree;
    int keys[] = {45, 33, 72, 55, 84, 75};
    for(int idx=0; idx < 6; idx++) tree.add(keys[idx]);
    tree.println();
}
/*
     Current tree:
                    72
                   /  \
                 45    84
                / \    /
              33  55  75 
 
    Adding 73:
                    72
                   /  \
                 45    84  => LH of LH
                / \    /
              33  55  75 
                     /
                    73
 
    rotateLeft(at 84):
                    72
                   /  \
                 45    75
                / \    / \
              33  55  73  84
     
*/
void AVLDemo2(){
    AVL<int, int*> tree;
    int keys[] = {45, 33, 72, 55, 84, 75, 73};
    for(int idx=0; idx < 7; idx++) tree.add(keys[idx]);
    tree.println();
}

/*
     Current tree:
                    72
                   /  \
                 45    84
                / \    /
              33  55  75     

    Adding 75:
                    72
                   /  \
                 45    84   =>  RH of LH
                / \    /
              33  55  75     
                       \
                        80
     
    rotateLeft(at 75):
                    72
                   /  \
                 45    84   => LH of LH
                / \    /
              33  55  80
                     /
                    75
 
    rotateRight(at 84):
                    72
                   /  \
                 45    80
                / \    / \
              33  55  75  84
*/
void AVLDemo3(){
    AVL<int, int*> tree;
    int keys[] = {45, 33, 72, 55, 84, 75, 80};
    for(int idx=0; idx < 7; idx++) tree.add(keys[idx]);
    tree.println();
}
/*
Current tree:
            45
           /  \
         33    72
        /  \
      24    40

Adding 15:
            45    : LH of LH => Rotate right
           /  \
         33    72
        /  \
      24    40
     /
   15     
rotateRight(at 45):
         33
        /  \
      24    45
     /     /  \
   15     40   72
 *     
 */


void AVLDemo4(){
    AVL<int, int*> tree;
    int keys[] = {45, 33, 72, 24, 40, 15};
    for(int idx=0; idx < 6; idx++) tree.add(keys[idx], new int(keys[idx]) );
    tree.println();
    DLinkedList<int*> list = tree.ascendingList();
    list.println(&int2str);
}


void AVLDemo5(){
    AVL<int, int*> tree;
    int keys[] = {72, 45, 84, 33, 35, 75};
    for(int idx=0; idx < 6; idx++) tree.add(keys[idx]);
    tree.println();
}

void AVLDemo6(){
    AVL<int, int*> tree;
    int keys[] = {72, 45, 84, 50, 95, 90};
    for(int idx=0; idx < 6; idx++) tree.add(keys[idx]);
    tree.println();
}

void AVLDemo7(){
    AVL<int, int*> tree;
    int keys[] = {50, 30, 60, 20, 40, 55, 10, 25, 35, 5};
    for(int idx=0; idx < 10; idx++) tree.add(keys[idx]);
    tree.println();
    DLinkedList<string> factor = tree.bfactor();
    cout << "Balance Factors: " << endl;
    factor.println();
}

void AVLDemo8(){
    AVL<int, int*> tree;
    for(int idx=0; idx <12; idx++) tree.add(idx);
    cout << "AVL: #nodes: " << tree.size() << endl;
    cout << "AVL: height: " << tree.height() << endl;
    tree.println();
}

string pointEntry2Str(float& key, Point*& point){
    stringstream os;
    os << *point;
    return os.str();
}
void AVL_Point1(){
    Point* array[] = {
        new Point(1.5, 2.5), new Point(7.5, 2.5), new Point(9.5, 2.5),
        new Point(12.5, 2.5), new Point(13.5, 2.5), new Point(14.5, 2.5),
        new Point(15.5, 2.5), new Point(16.5, 2.5), new Point(21.5, 2.5)
    };
    AVL<float, Point*> tree;
    for(int idx=0; idx < 9; idx++) tree.add(array[idx]->getX(), array[idx]);
    tree.println(&pointEntry2Str);
    List<Point*> list = tree.select(10, 15);
    list.println(&point2str);
}

#endif /* AVLTREEDEMO_H */

