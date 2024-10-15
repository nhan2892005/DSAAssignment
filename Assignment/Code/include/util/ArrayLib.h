/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IntArray.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 20:21
 */

#ifndef INTARRAY_H
#define INTARRAY_H
#include <random>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cassert>
using namespace std;

typedef unsigned char uint8;
typedef char int8;

template<class T>
string int_to_string(T *data, int nrows, int ncols, int width=6){
    int stride = ncols;
    stringstream os;
    os << "[" << endl;
    for(int r=0; r < nrows; r++){
        os << "\t[";
        for(int c=0; c < ncols; c++){
            int index = r*stride + c;
            os << setw(width) << (int)data[index];
        }
        os << "]" << endl;
    }
    os << "]" << endl;
    return os.str();
}



template<class T>
string float_to_string(T *data, int nrows, int ncols, int width=6, int prec=2){
    int stride = ncols;
    stringstream os;
    os << "[" << endl;
    for(int r=0; r < nrows; r++){
        os << "\t[";
        for(int c=0; c < ncols; c++){
            int index = r*stride + c;
            os << fixed
               << setw(width) << setprecision(prec) << data[index];
        }
        os << "]" << endl;
    }
    os << "]" << endl;
    return os.str();
}



template<class T>
void int_uniform(   T *data, int numel,
                    T minValue=0, T maxValue=100, bool manualSeed=false, int seedValue=0
                    ){
    uniform_int_distribution<T> dist(minValue, maxValue);

    if(manualSeed){
        std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(seedValue));
        for(int idx=0; idx < numel; idx++) data[idx] = dist(engine);
    }
    else{
        std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(time(0)));
        for(int idx=0; idx < numel; idx++) data[idx] = dist(engine);
    }
}
template<class T>
void real_uniform(  T *data, int numel,
                    T minValue=0, T maxValue=100, bool manualSeed=false, int seedValue=0
                    ){
    uniform_real_distribution<T> dist(minValue, maxValue);

    if(manualSeed){
        std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(seedValue));
        for(int idx=0; idx < numel; idx++) data[idx] = dist(engine);
    }
    else{
        std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(time(0)));
        for(int idx=0; idx < numel; idx++) data[idx] = dist(engine);
    }
}

template<class T>
class Matrix{
public:
    static const int INT32    = 0;
    static const int FLOAT32  = 10;
    static const int FLOAT64  = 20;
    static const int INT8     = 30;
    static const int UINT8    = 40;

    
private:
    T* data;
    int nrows;
    int ncols;
    int dtype;
    
protected:

    void init_buffer(int nrows=0, int ncols=0, int dtype=INT32){
        int nitems = nrows * ncols;
        if(nitems > 0){
            this->data = new T[nitems]{};
            //init here
        }
        else this->data = NULL;
    }
    void do_assert(int dtype){
        switch(dtype){
            case INT8:
                assert((std::is_same<T, int8>::value) == true); break;
            case UINT8:
                assert((std::is_same<T, uint8>::value) == true); break;
            case INT32:
                assert((std::is_same<T, int>::value) == true); break;                 
            case FLOAT32:
                assert((std::is_same<T, float>::value) == true); break; 
            case FLOAT64:
                assert((std::is_same<T, double>::value) == true); break; 
            default:
                throw std::exception();
        }
    }
  
public:
        
    Matrix(int nrows=0, int ncols=0, int dtype=INT32){
        do_assert(dtype); //REQUIRE: dtype == T
        //
        this->nrows = nrows;
        this->ncols = ncols;
        this->dtype = dtype;
        
        init_buffer(nrows, ncols, dtype);
        
    }
    ~Matrix(){
        if(this->data != NULL) delete []data;
    }
    
    int numel(){
        return nrows*ncols;
    }
    int buffersize(){
        return nrows*ncols*itemsize();
    }
    int itemsize(){
        return sizeof(T);
    }
    int rows(){
        return this->nrows;
    }
    int cols(){
        return this->ncols;
    }
    T* const ptr(int row, int col){
        int stride = this->ncols;
        int index = row*stride + col;
        return &(this->data[index]);
    }
    T get(int row, int col){
        int stride = this->ncols;
        return this->data[row*stride + col];
    }
    void set(int row, int col, T value){
        int stride = this->ncols;
        this->data[row*stride + col] = value;
    }
    vector<int> sumrows(){
        vector<int> results;
        for(int r=0; r < rows(); r++){
            int sum = 0;
            for(int c=0; c < cols(); c++){
                sum += get(r, c);
            }
            results.push_back(sum);
        }
        return results;
    }
    vector<int> sumcols(){
        vector<int> results;
        for(int c=0; c < cols(); c++){
            int sum = 0;
            for(int r=0; r < rows(); r++){
                sum += get(r, c);
            }
            results.push_back(sum);
        }
        return results;
    }
    
    Matrix& uniform(double minValue=0, double maxValue=100, bool manualSeed=false, int seedValue=0, bool symetric=false){
        if(this->dtype == INT32)
            int_uniform<int>((int*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == INT8)
            int_uniform<int8>((int8*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == UINT8)
            int_uniform<uint8>((uint8*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == FLOAT32)
            real_uniform<float>((float*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == FLOAT64)
            real_uniform<double>((double*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        //
        if(symetric){
            for(int r=0; r < rows(); r++){
                for(int c= r+1; c < cols(); c++){
                    T value = 0;
                    if((c < rows()) && (r < cols())) value = get(c, r);
                    set(r, c, value);
                }
            }
        }
        //
        return *this;
    }
    Matrix& uniformsym(double minValue=0, double maxValue=100, bool manualSeed=false, int seedValue=0, bool symetric=false){
        if(this->dtype == INT32)
            int_uniform<int>((int*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == INT8)
            int_uniform<int8>((int8*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == UINT8)
            int_uniform<uint8>((uint8*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == FLOAT32)
            real_uniform<float>((float*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        if(this->dtype == FLOAT64)
            real_uniform<double>((double*)(this->data), this->numel(), minValue, maxValue, manualSeed, seedValue);
        //
        if(symetric){
            for(int r=0; r < rows(); r++){
                for(int c= r+1; c < cols(); c++){
                    T value = 0;
                    if((c < rows()) && (r < cols())) value = get(c, r);
                    set(r, c, value);
                }
            }
        }
        //
        return *this;
    }
    Matrix& binomial(float p=0.5, bool manualSeed=false, int seedValue=0, bool symetric=false){
        //data_binomial<T>(this->data, this->numel(), p, manualSeed, seedValue);
        binomial_distribution<int> dist(1, p);

        if(manualSeed){
            std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(seedValue));
            for(int idx=0; idx < numel(); idx++) data[idx] = dist(engine);
        }
        else{
            std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(time(0)));
            for(int idx=0; idx < numel(); idx++) data[idx] = dist(engine);
        }
        if(symetric){
            for(int r=0; r < rows(); r++){
                for(int c= r+1; c < cols(); c++){
                    int value = 0;
                    if((c < rows()) && (r < cols())) value = get(c, r);
                    set(r, c, value);
                }
            }
        }
        return *this;
    }
    /*
    Matrix& binomialsym(float p=0.5, bool manualSeed=false, int seedValue=0){
        binomial(p, manualSeed, seedValue);
        for(int r=0; r < rows(); r++){
            for(int c= r+1; c < cols(); c++){
                int value = 0;
                if((c < rows()) && (r < cols())) value = get(c, r);
                set(r, c, value);
            }
        }
        return *this;
    }
    */
    void println(int width=6, int prec=2){
        cout << toString(width, prec) << endl;
    }
    string toString(int width=6, int prec=2){
        if(this->dtype == INT32)
            return int_to_string<int>((int*)(this->data), this->nrows, this->ncols, width);
        if(this->dtype == INT8)
            return int_to_string<int8>((int8*)(this->data), this->nrows, this->ncols, width);
        if(this->dtype == UINT8)
            return int_to_string<uint8>((uint8*)(this->data), this->nrows, this->ncols, width);
        if(this->dtype == FLOAT32)
            return float_to_string<float>((float*)(this->data), this->nrows, this->ncols, width, prec);
        if(this->dtype == FLOAT64)
            return float_to_string<double>((double*)(this->data), this->nrows, this->ncols, width, prec);
        return "dtype: not supported\n";
    }
};


int* genIntArray(int size, int minValue=0, int maxValue=100, 
               bool manualSeed=false, int seedValue=0){

    int* head = new int[size];

    std::default_random_engine* engine;
    if(manualSeed)
        engine = new std::default_random_engine(static_cast<long unsigned int>(seedValue));
    else
        engine = new std::default_random_engine(static_cast<long unsigned int>(time(0)));
    uniform_int_distribution<int> dist(minValue, maxValue);

    //
    for(int idx=0; idx < size; idx++) head[idx] = dist(*engine);

    delete engine;
    return head;
}
int genInt(int minValue=0, int maxValue=100, 
               bool manualSeed=false, int seedValue=0){

    int value;
    uniform_int_distribution<int> dist(minValue, maxValue);
    
    if(manualSeed){
        std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(seedValue));
        value = dist(engine);
    }
    else{
        std::default_random_engine engine = std::default_random_engine(static_cast<long unsigned int>(time(0)));
        value = dist(engine);
    }
        
    return value;
}
int* permutation(int size, bool manualSeed=false, int seedValue=0){
    int *ptr = new int[size];
    for(int idx=0; idx < size; idx++) ptr[idx] = idx;
    
    uniform_int_distribution<int> dist(0, size-1);
    int a, b, t;
    if(manualSeed){
        std::default_random_engine engine = 
                    std::default_random_engine(static_cast<long unsigned int>(seedValue));
        for(int idx=0; idx < size; idx++){
            a = dist(engine); b = dist(engine);
            t = ptr[a]; ptr[a] = ptr[b]; ptr[b] = t;
        }
    }
    else{
        std::default_random_engine engine = 
                std::default_random_engine(static_cast<long unsigned int>(time(0)));
        for(int idx=0; idx < size; idx++){
            a = dist(engine); b = dist(engine);
            t = ptr[a]; ptr[a] = ptr[b]; ptr[b] = t;
        }
    }
    
    return ptr;
}




template<class T>
bool isOrdered(T* array, int size, bool ascending ){
    bool ordered = true;
    for(int idx=0; idx < size-1; idx++){
        //check for ascending
        if(ascending){
            if(array[idx] > array[idx+1]){
                ordered = false;
                break;
            }
        }
        else{
            //check for descending
            if(array[idx] < array[idx+1]){
                ordered = false;
                break;
            }
        }
    }
    return ordered;
}


template<class T>
void println(T* array, int size){
    cout << "[\n";
    for(int idx=0; idx < size; idx++){
        cout << array[idx] << endl;
    }
    cout << "]\n" << endl;
}

template<class T>
void print(T* array, int size){
    cout << "[";
    for(int idx=0; idx < size-1; idx++){
        cout << array[idx] << ", ";
    }
    if(size > 0) cout << array[size-1] << "]\n";
    else cout << "]\n" << endl;
}

#endif /* INTARRAY_H */

