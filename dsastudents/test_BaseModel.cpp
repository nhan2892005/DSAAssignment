#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
using namespace std;

#include "ann/BaseModel.h"
#include "ann/FCLayer.h"
#include "ann/Softmax.h"
#include "ann/ReLU.h"
#include "ann/funtions.h"
#include "ann/xtensor_lib.h"

using namespace std;
namespace fs = std::filesystem;
int num_task = 6;

vector<vector<string>> expected_task (num_task, vector<string>(50, ""));
vector<vector<string>> output_task (num_task, vector<string>(50, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/BaseModel/BaseModelTestLog_Compare.txt";
    fstream file(log_file);
    if (!file.is_open()) {
        std::cout << "Cannot open file" << std::endl;
        return;
    }
    
    streambuf* stream_buffer_cout = cout.rdbuf();
    cout.rdbuf(file.rdbuf());

    fstream file1(filename1);
    fstream file2(filename2);

    // Store content of tasks in vector
    string line;
    int num = 1;
    int num_line = 0;
    while (getline(file1, line)) {
        // if line contains "Task", store it in expected_task
        if (regex_match(line, regex("Task [0-9]+.*"))) {
            num_line = 0;
            // Extract numbers from the line
            regex number_regex("[0-9]+");
            auto numbers_begin = sregex_iterator(line.begin(), line.end(), number_regex);
            auto numbers_end = sregex_iterator();

            for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
                std::smatch match = *i;
                num = std::stoi(match.str());
                break;  
            }
        }
        expected_task[num - 1][num_line] += line + '\n';
        num_line++;
    }

    num = 1;
    num_line = 0;
    while (getline(file2, line)) {
        // if line contains "Task", store it in output_task
        if (regex_match(line, regex("Task [0-9]+.*"))) {
            num_line = 0;
            // Extract numbers from the line
            regex number_regex("[0-9]+");
            auto numbers_begin = sregex_iterator(line.begin(), line.end(), number_regex);
            auto numbers_end = sregex_iterator();

            for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
                std::smatch match = *i;
                num = std::stoi(match.str());
                break;  
            }
        }
        output_task[num - 1][num_line] += line + '\n';
        num_line++;
    }

    // Compare content of tasks
    for (int task = 1; task < num_task; task++) {
        int count_diff = 0;
        if (output_task[task][0] == "") continue;
        doTasks.push_back(task);
        for (int line = 0; line < expected_task[task].size(); line++) {
            if (expected_task[task][line] != output_task[task][line]) {
                count_diff++;
                cout << "Task " << task << " - Line " << line << ":\n";
                cout << "Expected: " << expected_task[task][line];
                cout << "Output: " << output_task[task][line];
            }
        }
        if (count_diff > 0) {
            diffTasks.push_back(task);
        }
    }
    
    cout.rdbuf(stream_buffer_cout);
    float prop = (float)diffTasks.size() / (float)doTasks.size();
    if (prop == 0) {
        std::cout << "All tasks are correct" << std::endl;
    } else {
        std::cout << "Number of tasks to do: " << doTasks.size() << std::endl;
        std::cout << "Number of tasks with differences: " << diffTasks.size() << std::endl;
        std::cout << "Pass: " << 1 - prop << std::endl;
        for (int i = 0; i < diffTasks.size(); i++) {
            std::cout << "Task " << diffTasks[i] << std::endl;
        }
    }
}

void printUsage() {
    std::cout << "Usage: exe_file [OPTIONS] [TASK]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  ?help: show help" << std::endl;
    std::cout << "  demo: run demo" << std::endl;
    std::cout << "  !test: run all test" << std::endl;
    std::cout << "  !test [task]: run specific test" << std::endl;
    std::cout << "  !test [start_task] [end_task] : run test from start_task to end_task" << std::endl;
    std::cout << "This test has 15 tasks" << std::endl;
}

/*
template <class T>
using List = DLinkedList<T>;


BaseModel::BaseModel() { 
    layers = List<Layer*>(List<Layer*>::free);
}
BaseModel::BaseModel(Layer** seq, int size) {
    layers = List<Layer*>(List<Layer*>::free);
    for(int idx=0; idx < size; idx++){
        layers.add(std::move(seq[idx]));
    }
}

BaseModel::BaseModel(const BaseModel& orig) {
    layers = List<Layer*>(List<Layer*>::free);
    for(auto ptr_layer: orig.layers){
        layers.add(ptr_layer);
    }
}

BaseModel::~BaseModel() {
}

xt::xarray<double> BaseModel::predict(xt::xarray<double> X){
    xt::xarray<double> A = X;
    for(Layer* layer: layers){
        A = layer->forward(A);
    }
    return A;
}

FCLayer::FCLayer(int in_features, int out_features, bool use_bias) {
    this->m_nIn_Features = in_features;
    this->m_nOut_Features = out_features;
    this->m_bUse_Bias = use_bias;
    name = "FC_" + to_string(++layer_idx);
    m_unSample_Counter = 0;
    
    init_weights();
}
void FCLayer::init_weights(){
    m_aWeights = xt::random::randn<double>({m_nOut_Features, m_nIn_Features});
    if(m_bUse_Bias){
         m_aBias = xt::random::randn<double>({m_nOut_Features});
    }
    else{
         m_aBias = xt::zeros<double>({m_nOut_Features});
    }
}

FCLayer::FCLayer(const FCLayer& orig) {
    name = "FC_" + to_string(++layer_idx);
    this->m_nIn_Features = orig.m_nIn_Features;
    this->m_nOut_Features = orig.m_nOut_Features;
    this->m_bUse_Bias = orig.m_bUse_Bias;
    this->m_aWeights = orig.m_aWeights;
    this->m_aBias = orig.m_aBias;
    this->m_unSample_Counter = orig.m_unSample_Counter;
    this->m_aGrad_W = orig.m_aGrad_W;
    this->m_aGrad_b = orig.m_aGrad_b;
    this->m_aCached_X = orig.m_aCached_X;
}

FCLayer::~FCLayer() {  
}

xt::xarray<double> FCLayer::forward(xt::xarray<double> X) {
    // * Store the input X for backpropagation
    m_aCached_X = X;

    // * Calculate the output Z
    // * Z = W*X + b
    xt::xarray<double> Z = xt::linalg::dot(m_aWeights, X);

    // * Add bias if needed
    if (m_bUse_Bias){
        auto bias_expanded = xt::expand_dims(m_aBias, 1);
        Z = Z + bias_expanded;
    }
    return Z;
}

#include "ann/ReLU.h"

ReLU::ReLU() {
    name = "ReLU" + to_string(++layer_idx);
}

ReLU::ReLU(const ReLU& orig) {
    name = "ReLU" + to_string(++layer_idx);
    this->mask = orig.mask;
}

ReLU::~ReLU() {
}

xt::xarray<double> ReLU::forward(xt::xarray<double> X) {
    // * ReLU function is defined as f(x) = max(0, x).
    mask = (X > 0);
    return X * mask;
}
#include "ann/Softmax.h"
#include "ann/funtions.h"

Softmax::Softmax(int axis): axis(axis) {
    name = "Softmax_" + to_string(++layer_idx);
    cached_Y = xt::zeros<double>({1});
}

Softmax::Softmax(const Softmax& orig) {
    name = "Softmax_" + to_string(++layer_idx);
    axis = orig.axis;
    cached_Y = orig.cached_Y;
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    // * Softmax function is defined as f(x) = exp(x) / sum(exp(x))
    cached_Y = softmax(X, axis);
    return cached_Y;
}

#include "ann/Layer.h"

Layer::Layer() {
    layer_idx++;
    name = "Layer_" + to_string(layer_idx);
    is_training = true;
}

Layer::Layer(const Layer& orig) {
    is_training = orig.is_training;
    name = orig.name;
    layer_idx++;
}

Layer::~Layer() {
}

unsigned long long Layer::layer_idx = -1;

// #ifndef DLINKEDLIST_H
// #define DLINKEDLIST_H

// #include "list/IList.h"

// #include <sstream>
// #include <iostream>
// #include <type_traits>
// using namespace std;

// template <class T>
// class DLinkedList : public IList<T>
// {
// public:
//     class Node;        // Forward declaration
//     class Iterator;    // Forward declaration
//     class BWDIterator; // Forward declaration

// protected:
//     Node *head; // this node does not contain user's data
//     Node *tail; // this node does not contain user's data
//     int count;
//     bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
//     void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

// public:
//     DLinkedList(
//         void (*deleteUserData)(DLinkedList<T> *) = 0,
//         bool (*itemEqual)(T &, T &) = 0);
//     DLinkedList(const DLinkedList<T> &list);
//     DLinkedList<T> &operator=(const DLinkedList<T> &list);
//     ~DLinkedList();

//     // Inherit from IList: BEGIN
//     void add(T e);
//     void add(int index, T e);
//     T removeAt(int index);
//     bool removeItem(T item, void (*removeItemData)(T) = 0);
//     bool empty();
//     int size();
//     void clear();
//     T &get(int index);
//     int indexOf(T item);
//     bool contains(T item);
//     string toString(string (*item2str)(T &) = 0);
//     // Inherit from IList: END

//     // Student's methods
//     // BEGIN
//     /* Check valid for index of element in list
//         * + index: index of element in list
//         * + for_extend: if for_extend = 1, list need to add element => index can be equal count
//         *                if for_extend = 0, list need to get element => index must be less than count
//     */
//     void checkIndex(int index, bool for_extend);
//     // END

//     void println(string (*item2str)(T &) = 0)
//     {
//         cout << toString(item2str) << endl;
//     }
//     void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
//     {
//         this->deleteUserData = deleteUserData;
//     }

//     bool contains(T array[], int size)
//     {
//         int idx = 0;
//         for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
//         {
//             if (!equals(*it, array[idx++], this->itemEqual))
//                 return false;
//         }
//         return true;
//     }

//     /*
//      * free(DLinkedList<T> *list):
//      *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
//      *  + if users want a DLinkedList removing their data,
//      *      he/she must pass "free" to constructor of DLinkedList
//      *      Example:
//      *      DLinkedList<T> list(&DLinkedList<T>::free);
//      */
//     static void free(DLinkedList<T> *list)
//     {
//         typename DLinkedList<T>::Iterator it = list->begin();
//         while (it != list->end())
//         {
//             delete *it;
//             it++;
//         }
//     }

//     /* begin, end and Iterator helps user to traverse a list forwardly
//      * Example: assume "list" is object of DLinkedList

//      DLinkedList<char>::Iterator it;
//      for(it = list.begin(); it != list.end(); it++){
//             char item = *it;
//             std::cout << item; //print the item
//      }
//      */
//     Iterator begin()
//     {
//         return Iterator(this, true);
//     }
//     Iterator end()
//     {
//         return Iterator(this, false);
//     }

//     /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
//      * Example: assume "list" is object of DLinkedList

//      DLinkedList<char>::BWDIterator it;
//      for(it = list.last(); it != list.beforeFirst(); it--){
//             char item = *it;
//             std::cout << item; //print the item
//      }
//      */
//     BWDIterator bbegin()
//     {
//         return BWDIterator(this, true);
//     }
//     BWDIterator bend()
//     {
//         return BWDIterator(this, false);
//     }

// protected:
//     static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
//     {
//         if (itemEqual == 0)
//             return lhs == rhs;
//         else
//             return itemEqual(lhs, rhs);
//     }
//     void copyFrom(const DLinkedList<T> &list);
//     void removeInternalData();
//     Node *getPreviousNodeOf(int index);

//     //////////////////////////////////////////////////////////////////////
//     ////////////////////////  INNER CLASSES DEFNITION ////////////////////
//     //////////////////////////////////////////////////////////////////////
// public:
//     class Node
//     {
//     public:
//         T data;
//         Node *next;
//         Node *prev;
//         friend class DLinkedList<T>;

//     public:
//         Node(Node *next = 0, Node *prev = 0)
//         {
//             this->next = next;
//             this->prev = prev;
//         }
//         Node(T data, Node *next = 0, Node *prev = 0)
//         {
//             this->data = data;
//             this->next = next;
//             this->prev = prev;
//         }
//     };

//     //////////////////////////////////////////////////////////////////////
//     class Iterator
//     {
//     private:
//         DLinkedList<T> *pList;
//         Node *pNode;

//     public:
//         Iterator(DLinkedList<T> *pList = 0, bool begin = true)
//         {
//             if (begin)
//             {
//                 if (pList != 0)
//                     this->pNode = pList->head->next;
//                 else
//                     pNode = 0;
//             }
//             else
//             {
//                 if (pList != 0)
//                     this->pNode = pList->tail;
//                 else
//                     pNode = 0;
//             }
//             this->pList = pList;
//         }

//         Iterator &operator=(const Iterator &iterator)
//         {
//             this->pNode = iterator.pNode;
//             this->pList = iterator.pList;
//             return *this;
//         }
//         void remove(void (*removeItemData)(T) = 0)
//         {
//             pNode->prev->next = pNode->next;
//             pNode->next->prev = pNode->prev;
//             Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
//             if (removeItemData != 0)
//                 removeItemData(pNode->data);
//             delete pNode;
//             pNode = pNext;
//             pList->count -= 1;
//         }

//         T &operator*()
//         {
//             return pNode->data;
//         }
//         bool operator!=(const Iterator &iterator)
//         {
//             return pNode != iterator.pNode;
//         }
//         // Prefix ++ overload
//         Iterator &operator++()
//         {
//             pNode = pNode->next;
//             return *this;
//         }
//         // Postfix ++ overload
//         Iterator operator++(int)
//         {
//             Iterator iterator = *this;
//             ++*this;
//             return iterator;
//         }
//     };
// };
// //////////////////////////////////////////////////////////////////////
// // Define a shorter name for DLinkedList:

// template <class T>
// using List = DLinkedList<T>;

// //////////////////////////////////////////////////////////////////////
// ////////////////////////     METHOD DEFNITION      ///////////////////
// //////////////////////////////////////////////////////////////////////

// template <class T>
// void DLinkedList<T>::checkIndex(int index, bool for_extend){
//     if (for_extend && index == count) return;
//     if (index < 0 || index >= count) {
//         throw "Index is out of range!";
//     }
// }

// template <class T>
// DLinkedList<T>::DLinkedList(
//     void (*deleteUserData)(DLinkedList<T> *),
//     bool (*itemEqual)(T &, T &))
//     : deleteUserData(deleteUserData), itemEqual(itemEqual), count(0)
// {
//     this->head = new Node();
//     this->tail = new Node();
//     head->next = tail;
//     tail->prev = head;
// }

// template <class T>
// DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
// {
//     /*
//     * Objectives: copy constructor
//     * Parameters: a list to be copied
//     */
//     this->count = 0;
//     this->copyFrom(list);
// }

// template <class T>
// DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
// {
//     /*
//     * Objectives: assignment operator
//     * Parameters: a list to be assigned
//     */
//     if (this != &list)
//     {
//         this->removeInternalData();
//         this->copyFrom(list);
//     }

//     return *this;
// }

// template <class T>
// DLinkedList<T>::~DLinkedList()
// {
//     // TODO
//     if (this->count > 0)
//     {
//         this->removeInternalData();
//     }
//     this->count = 0;
// }

// template <class T>
// void DLinkedList<T>::add(T e)
// {
//     /*
//     * Objectives: add an item to the end of the list
//     */
//     Node *newNode = new Node(e);
//     if (head->next == tail) {           // ? empty list
//         head->next = newNode;
//         tail->prev = newNode;
//         newNode->prev = head;
//         newNode->next = tail;
//     } else {                            // : non-empty list
//         tail->prev->next = newNode;
//         newNode->prev = tail->prev;
//         newNode->next = tail;
//         tail->prev = newNode;
//     }
//     count++;
// }
// template <class T>
// void DLinkedList<T>::add(int index, T e)
// {
//     /*
//     * Objectives: add an item to the list at a specific index
//     */
//     checkIndex(index, 1);
//     if (index == count) {                       // ? add to the end of the list
//         add(e);
//         return;
//     }
//     Node *current = getPreviousNodeOf(index);
//     Node *newNode = new Node(e, current->next, current);
//     current->next = newNode;
//     newNode->next->prev = newNode;
//     ++count;
// }

// template <class T>
// typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
// {
//     /**
//      * Returns the node preceding the specified index in the doubly linked list.
//      * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
//      * Efficiently navigates to the node by choosing the shorter path based on the index's position.
//      */
//     Node *current = head;
//     for (int i = 0; i < index; i++) {
//         current = current->next;
//     }
//     return current;
// }

// template <class T>
// T DLinkedList<T>::removeAt(int index)
// {
//     /*
//     * Objectives: remove an item from the list at a specific index
//     * Return: the removed item
//     * Exception: throw an out_of_range exception if the index is invalid
//     */
//     checkIndex(index, 0);
//     Node *current = getPreviousNodeOf(index);
//     Node *removeNode = current->next;
//     T data = removeNode->data;
//     current->next = removeNode->next;
//     removeNode->next->prev = current;
//     delete removeNode;
//     count--;
//     return data;
// }

// template <class T>
// bool DLinkedList<T>::empty()
// {
//     // * Objectives: check if the list is empty
//     return count == 0;
// }

// template <class T>
// int DLinkedList<T>::size()
// {
//     // * Objectives: get the number of items in the list
//     return count;
// }

// template <class T>
// void DLinkedList<T>::clear()
// {
//     // * Objectives: clear the list
//     if (this->count > 0) {
//         this->removeInternalData();
//     }
//     if (head->next != tail) {
//         head->next = tail;
//         tail->prev = head;
//     }
//     count = 0;
// }

// template <class T>
// T &DLinkedList<T>::get(int index)
// {
//     /*
//     * Objectives: get an item from the list at a specific index
//     * Return: the item at the index
//     * Exception: throw an out_of_range exception if the index is invalid
//     * Note: index is 0-based
//     */
//     checkIndex(index, 0);
//     Node *current = getPreviousNodeOf(index);
//     return current->next->data;
// }

// template <class T>
// int DLinkedList<T>::indexOf(T item)
// {
//     /*
//     * Objectives: get the index of an item in the list
//     * Return: the index of the item if it exists, otherwise -1
//     * Note: if the item is a pointer type, the itemEqual function will be called to compare the item data
//     *     if it is not null
//     */
//     Node *current = head->next;
//     for (int i = 0; i < count; i++) {
//         if (equals(current->data, item, itemEqual)) {
//             return i;
//         }
//         current = current->next;
//     }
//     return -1;
// }

// template <class T>
// bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
// {
//     /*
//     * Objectives: remove an item from the list
//     * Return: true if the item is removed, otherwise false
//     * Note: if the item is a pointer type, the removeItemData function will be called to remove the item data
//     *    if it is not null
//     */
//     Node *current = head->next;
//     for (int i = 0; i < count; i++) {
//         if (equals(current->data, item, itemEqual)) {
//             Node *removeNode = current;
//             current->prev->next = current->next;
//             current->next->prev = current->prev;
//             if (removeItemData) {
//                 removeItemData(current->data);
//             }
//             delete removeNode;
//             count--;
//             return true;
//         }
//         current = current->next;
//     }
//     return false;
// }

// template <class T>
// bool DLinkedList<T>::contains(T item)
// {
//     /*
//     * Objectives: check if the list contains an item
//     * Return: true if the item is in the list, otherwise false
//     * Note: if the item is a pointer type, the itemEqual function will be called to compare the item data
//     *   if it is not null
//     */
//     Node *current = head->next;
//     for (int i = 0; i < count; i++) {
//         if (equals(current->data, item, itemEqual)) {
//             return true;
//         }
//         current = current->next;
//     }
//     return false;
// }

// template <class T>
// string DLinkedList<T>::toString(string (*item2str)(T &))
// {
//     /**
//      * Converts the list into a string representation, where each element is formatted using a user-provided function.
//      * If no custom function is provided, it directly uses the element's default string representation.
//      * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
//      *
//      * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
//      * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
//      */
//     // TODO
//     stringstream ss;
//     ss << "[";
//     Node *current = head->next;
//     for (int i = 0; i < count; i++) {
//         if (item2str) {
//             ss << item2str(current->data);
//         } else {
//             ss << current->data;
//         }
//         if (i < count - 1) {
//             ss << ", ";
//         }
//         current = current->next;
//     }
//     ss << "]";
//     return ss.str();
// }

// template <class T>
// void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
// {
//     /**
//      * Copies the contents of another doubly linked list into this list.
//      * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
//      * Iterates through the source list and adds each element, preserving the order of the nodes.
//      */
//     if (this->count > 0) {
//         this->removeInternalData();
//     }
//     this->itemEqual = list.itemEqual;
//     this->deleteUserData = list.deleteUserData;
//     this->head = new Node();
//     this->tail = new Node();
//     this->head->next = this->tail;
//     this->tail->prev = this->head;
    
//     for (Node *current = list.head->next; current != list.tail; current = current->next) {
//         this->add(current->data);
//     }
//     this->count = list.count;
// }

// template <class T>
// void DLinkedList<T>::removeInternalData()
// {
//     if (this->deleteUserData != nullptr)
//     {
//         this->deleteUserData(this);
//     }
//     Node* tmp = head->next;
//     while (tmp != nullptr)
//     {
//         Node *current = tmp;
//         tmp = tmp->next;
//         delete current;
//         --count;
//     }
//     count = 0;
//     delete head;
// }

// #endif /* DLINKEDLIST_H */

void runDemo() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(2, 3, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(3, 2, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    cout << "After feedforward: " << endl;
    xt::xarray<double> X = xt::xarray<double>({1, 2});
    xt::xarray<double> Yhat = model.predict(X);

    cout << "Shape of X: " << shape2str(X.shape()) << endl;
    cout << "X: " << X << endl;

    cout << "Shape of Yhat: " << shape2str(Yhat.shape()) << endl;
    cout << "Yhat: " << Yhat << endl;
}

void test1() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(2, 3, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(3, 2, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, 0.5});
    cout << Yhat << endl;
}

void test2() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(3, 3, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(3, 2, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2, 3});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, 0.5});
    cout << Yhat << endl;
}

void test3() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(4, 3, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(3, 3, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2, 3, 4});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, 1.5, 0});
    cout << Yhat << endl;
}

void test4() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(5, 2, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(2, 2, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2, 3, 4, 5});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, 2.5});
    cout << Yhat << endl;
}

void test5() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(6, 7, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(7, 2, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2, 3, 4, 5, 6});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, 1.5});
    cout << Yhat << endl;
}

void test6() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(7, 4, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(4, 3, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2, 3, 4, 5, 6, 7});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, -0.5, 1});
    cout << Yhat << endl;
}

void test7() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(8, 3, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(3, 2, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2, 3, 4, 5, 6, 7, 8});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, -0.5});
    cout << Yhat << endl;
}

void test8() {
    int num_layers = 3;
    Layer *layers[num_layers];
    layers[0] = new FCLayer(9, 10, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(10, 2, true);
    layers[3] = new Softmax();

    BaseModel model(layers, 4);

    xt::xarray<double> X = xt::xarray<double>({1, 2, 3, 4, 5, 6, 7, 8, 9});
    xt::xarray<double> Yhat = model.predict(X);

    xt::xarray<double> expected_Yhat = xt::xarray<double>({0.5, -0.5});
    cout << Yhat << endl;
}


// pointer function to 
void (*testFuncs[])() = {
    test1, test2, test3, test4, test5, test6, test7, test8
};

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "?help") {
            printUsage();
            return 0;
        }
        else
        if (string(argv[1]) == "demo") {
            runDemo();
            return 0;
        }
        else 
        {
            string folder = "TestLog/BaseModel";
            string path = "BaseModelTestLog_NhanOutput.txt";
            string output = "BaseModelTestLog_YourOutput.txt";
            fstream file(folder + "/" + output);
            if (!file.is_open()) {
                fs::create_directory(folder);
                std::cout << "Create folder " << fs::absolute(folder) << std::endl;
                file.open(folder + "/" + output);
            }
            //change cout to file
            streambuf* stream_buffer_cout = cout.rdbuf();
            cout.rdbuf(file.rdbuf());

            if (string(argv[1]) == "!test") {
                if (argc == 2) {
                    for (int i = 0; i < num_task; i++) {
                        std::cout << "Task " << i + 1 << "---------------------------------------------------" <<std::endl;
                        testFuncs[i]();
                    }
                } else
                if (argc == 3) {
                    int task = stoi(argv[2]);
                    if (task >= 1 && task <= num_task) {
                        std::cout << "Task " << task << "---------------------------------------------------" <<std::endl;
                        testFuncs[task - 1]();
                    } else {
                        std::cout << "Task not found" << std::endl;
                    }
                } else
                if (argc == 4) {
                    int start = stoi(argv[2]);
                    int end = stoi(argv[3]);
                    if (start >= 1 && start <= num_task && end >= 1 && end <= num_task && start <= end) {
                        for (int i = start - 1; i < end; i++) {
                            std::cout << "Task " << i + 1 << "---------------------------------------------------" <<std::endl;
                            testFuncs[i]();
                        }
                    } else {
                        std::cout << "Task not found or you enter error" << std::endl;
                    }
                }
            }
            // Restore cout
            cout.rdbuf(stream_buffer_cout);

            compareFile(folder + "/" + path, folder + "/" + output);
        }
    }
    return 0;
}