#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include "include/list/DLinkedList.h"
#include "include/list/DLinkedListDemo.h"
#include "include/util/Point.h"

/*
template <class T>
void addAtIndex(DLinkedList<T>& list, int index, T item){
    list.add(index, item);
}
template <class T>
void addData(DLinkedList<T>& list, T item){
    list.add(item);
}
template <class T>
T getData(DLinkedList<T>& list, int index){
    return list.get(index);
}
template <class T>
int indexOfData(DLinkedList<T>& list, T item){
    return list.indexOf(item);
}
template <class T>
bool containsData(DLinkedList<T>& list, T item){
    return list.contains(item);
}
template <class T>
string toString(DLinkedList<T>& list, string (*item2str)(T &)){
    return list.toString(item2str);
}
template <class T>
int size(DLinkedList<T>& list){
    return list.size();
}
template <class T>
bool isEmpty(DLinkedList<T>& list){
    return list.empty();
}
template <class T>
void clear(DLinkedList<T>& list){
    list.clear();
}
template <class T>
T removeData(DLinkedList<T>& list, T item){
    return list.remove(item);
}
template <class T>
T removeAtIndex(DLinkedList<T>& list, int index){
    return list.removeAt(index);
}
template <class T>
bool removeItem(DLinkedList<T>& list, T item, void (*removeItemData)(T)){
    return list.removeItem(item, removeItemData);
}
template <class T>
bool removeItem(DLinkedList<T>& list, T item){
    return list.removeItem(item);
}
*/

int main(int argc, char* argv[]) {
    List<int> dllist;
    dllist.add(1);
    dllist.add(2);
    dllist.add(3);
    dllist.add(4);
    dllist.add(5);
    dllist.add(6);
    dllist.add(7);
    dllist.add(8);
    dllist.add(9);

    cout << "Original list: ";
    dllist.println();

    cout << "Index of 5: " << dllist.indexOf(5) << endl;
    cout << "Index of 15: " << dllist.indexOf(15) << endl;

    cout << "Contains 5: " << (dllist.contains(5)? "found; " : "not found; ")
                << " indexOf returns: " << dllist.indexOf(5) << endl;
    cout << "Contains 15: " << (dllist.contains(15)? "found; " : "not found; ")
                << " indexOf returns: " << dllist.indexOf(15) << endl;

    cout << "Remove 5: " << dllist.removeItem(5) << endl;
    cout << "After removing 5: ";
    dllist.println();

    cout << "Remove 15: " << dllist.removeItem(15) << endl;
    cout << "After removing 15: ";
    dllist.println();

    cout << "Remove at 3: " << dllist.removeAt(3) << endl;
    cout << "After removing at 3: ";
    dllist.println();

    cout << "Remove at 10: " << dllist.removeAt(6) << endl;
    cout << "After removing at 6: ";
    dllist.println();

    cout << "Remove at 0: " << dllist.removeAt(0) << endl;
    cout << "After removing at 0: ";
    dllist.println();

    cout << "Remove at 5: " << dllist.removeAt(4) << endl;
    cout << "After removing at 4: ";
    dllist.println();

    cout << "Remove at 0: " << dllist.removeAt(0) << endl;
    cout << "After removing at 0: ";
    dllist.println();

    cout << "Add 10 at 0: ";
    dllist.add(0, 10);
    dllist.println();

    cout << "Add 20 at 3: ";
    dllist.add(3, 20);
    dllist.println();

    cout << "Add 30 at 3: ";
    dllist.add(3, 30);
    dllist.println();

    cout << "Size: " << dllist.size() << endl;

    cout << "Clear: ";
    dllist.clear();
    dllist.println();

    cout << "Size: " << dllist.size() << endl;
    cout << "Is empty: " << (dllist.empty()? "yes" : "no") << endl;

    List<Point> dlist(0, &Point::pointEQ);
    dlist.add(Point(1.5, 3.5, 3.6));
    dlist.add(Point(2.5, 4.5, 2.7));
    dlist.add(Point(1.6, 3.1, 1.8));
    dlist.add(Point(1.6, 5.3, 4.9));
    dlist.add(Point(4.5, 6.1, 7.1));
    dlist.add(Point(3.5, 7.5, 8.2));
    dlist.add(Point(2.6, 8.1, 9.3));
    dlist.add(Point(1.7, 9.5, 6.4));
    dlist.add(Point(1.8, 1.1, 5.5));
    dlist.add(Point(1.9, 2.5, 4.6));
    
    cout << "Original list: ";
    dlist.println(&Point::point2str);

    Point p1(1.6, 3.1, 1.8);
    Point p2(1.6, 5.3, 4.9);
    cout << p1 << "=> " << (dlist.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << dlist.indexOf(p1) << endl;
    cout << p2 << "=> " << (dlist.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << dlist.indexOf(p2) << endl;

    // Remove p1 and p2
    cout << "Remove " << p1 << ": " << dlist.removeItem(p1) << endl;
    cout << "After removing " << p1 << ": ";
    dlist.println(&Point::point2str);

    cout << "Remove " << p2 << ": " << dlist.removeItem(p2) << endl;
    cout << "After removing " << p2 << ": ";
    dlist.println(&Point::point2str);

    // Remove at head and tail
    cout << "Remove at 0: " << dlist.removeAt(0) << endl;
    cout << "After removing at 0: ";
    dlist.println(&Point::point2str);

    cout << "Remove at tail: " << dlist.removeAt(dlist.size() - 1) << endl;
    cout << "After removing at " << dlist.size() - 1 << ": ";
    dlist.println(&Point::point2str);

    // Add p1 and p2
    cout << "Add " << p1 << " at 0: ";
    dlist.add(0, p1);
    dlist.println(&Point::point2str);

    cout << "Add " << p2 << " at tail: ";
    dlist.add(dlist.size(), p2);
    dlist.println(&Point::point2str);

    cout << "Add at tail: ";
    dlist.add(dlist.size() - 1, Point(1.1, 2.2, 3.3));
    dlist.println(&Point::point2str);

    DLinkedList<Point> copyList = dlist;
    cout << "Copy list: ";
    copyList.println(&Point::point2str);

    for (DLinkedList<Point>::Iterator it = copyList.begin(); it != copyList.end(); it++) {
        cout << *it << endl;
    }

    cout << "Clear: ";
    dlist.clear();
    dlist.println(&Point::point2str);

    cout << "Copy list: ";
    copyList.println(&Point::point2str);

    // inline void to compare two int*
    auto intPtrEQ = [](int* &lhs, int* &rhs) {
        return *lhs > *rhs;
    };

    List<int*> list(&DLinkedList<int*>::free, intPtrEQ);

    int* a = new int(1);
    int* b = new int(2);
    int* c = new int(3);
    int* d = new int(4);
    int* e = new int(5);
    int* f = new int(6);
    int* g = new int(7);
    int* h = new int(8);
    int* i = new int(9);
    int* j = new int(10);

    list.add(a);
    list.add(b);
    list.add(c);
    list.add(d);
    list.add(e);
    list.add(f);
    list.add(g);
    list.add(h);
    list.add(i);
    list.add(j);

    cout << "Original list: ";
    auto printPointer = [](int* &ptr) {
        stringstream os;
        os << *ptr;
        return os.str();
    };
    list.println(printPointer);

    cout << "Index of 5: " << list.indexOf(e) << endl;
    cout << "Index of 7: " << list.indexOf(new int(7)) << endl;
    cout << "Index of 15: " << list.indexOf(new int(15)) << endl;

    cout << "Contains 5: " << (list.contains(e)? "found; " : "not found; ")
                << " indexOf returns: " << list.indexOf(e) << endl;
    cout << "Contains 15: " << (list.contains(new int(15))? "found; " : "not found; ")
                << " indexOf returns: " << list.indexOf(new int(15)) << endl;
    cout << "Contains 15: " << (list.contains(new int(-1))? "found; " : "not found; ")
                << " indexOf returns: " << list.indexOf(new int(-1)) << endl;

    cout << "Remove 5: " << list.removeItem(e) << endl;
    cout << "After removing 5: ";
    list.println(printPointer);

    cout << "Remove 15: " << list.removeItem(new int(15)) << endl;
    cout << "After removing 15: ";
    list.println(printPointer);

    cout << "Remove at 3: " << list.removeAt(3) << endl;
    cout << "After removing at 3: ";
    list.println(printPointer);

    cout << "Remove at 7: " << list.removeAt(7) << endl;
    cout << "After removing at 7: ";
    list.println(printPointer);

    cout << "Remove at 0: " << list.removeAt(0) << endl;
    cout << "After removing at 0: ";
    list.println(printPointer);

    cout << "Remove at 5: " << list.removeAt(5) << endl;
    cout << "After removing at 5: ";
    list.println(printPointer);

    cout << "Remove at 0: " << list.removeAt(0) << endl;
    cout << "After removing at 0: ";
    list.println(printPointer);

    cout << "Add 10 at 0: ";
    list.add(0, new int(10));
    list.println(printPointer);

    cout << "Add 20 at 5: ";
    list.add(5, new int(20));
    list.println(printPointer);

    // clear
    list.clear();

    cout << "Size: " << list.size() << endl;
    cout << "Is empty: " << (list.empty()? "yes" : "no") << endl;

    // Object DLinkedList
    List<char>* doubList;
    doubList = new DLinkedList<char>();
    doubList->add(0,'a');
    doubList->add(1,'b');
    doubList->add(2,'c');
    doubList->add(3,'d');
    doubList->add(4,'e');
    doubList->add(5,'f');
    doubList->add(5,'g');

    cout << "Original list: ";
    doubList->println();

    cout << "Index of 'e': " << doubList->indexOf('e') << endl;
    cout << "Index of 'h': " << doubList->indexOf('h') << endl;

    delete doubList;

    dlist.add(Point(1.5, 3.5, 3.6));

    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete h;
    delete i;
    delete j;

    return 0;
}