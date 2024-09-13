/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "ann/xtensor_lib.h"
using namespace std;

template<typename DType, typename LType>
class DataLabel{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    xt::xarray<DType> getData() const{ return data; }
    xt::xarray<LType> getLabel() const{ return label; }
};

template<typename DType, typename LType>
class Batch{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    Batch() {
        // Initialize with empty xarrays
        data = xt::xarray<DType>();
        label = xt::xarray<LType>();
    }
    Batch(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    virtual ~Batch(){}
    xt::xarray<DType>& getData(){return data; }
    xt::xarray<LType>& getLabel(){return label; }
};


template<typename DType, typename LType>
class Dataset{
private:
public:
    Dataset(){};
    virtual ~Dataset(){};
    
    virtual int len()=0;
    virtual DataLabel<DType, LType> getitem(int index)=0;
    virtual xt::svector<unsigned long> get_data_shape()=0;
    virtual xt::svector<unsigned long> get_label_shape()=0;
    
};

//////////////////////////////////////////////////////////////////////
template<typename DType, typename LType>
class TensorDataset: public Dataset<DType, LType>{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
    
public:
    /* TensorDataset: 
     * need to initialize:
     * 1. data, label;
     * 2. data_shape, label_shape
    */
    TensorDataset(const xt::xarray<DType>& data, const xt::xarray<LType>& label)
        : data(data), label(label) {
        this->data_shape = this->data.shape();
        this->label_shape = this->label.shape();
    }

    // Copy constructor
    TensorDataset(const TensorDataset& other)
        : data(other.data), label(other.label),
        data_shape(other.data_shape), label_shape(other.label_shape) {}

    // Copy assignment operator
    TensorDataset& operator=(const TensorDataset& other) {
        if (this != &other) {
            data = other.data;
            label = other.label;
            data_shape = other.data_shape;
            label_shape = other.label_shape;
        }
        return *this;
    }

    // Move assignment operator
    TensorDataset& operator=(TensorDataset&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            label = std::move(other.label);
            data_shape = std::move(other.data_shape);
            label_shape = std::move(other.label_shape);
        }
        return *this;
    }

    /* len():
     *  return the size of dimension 0
    */
    int len(){
        /* TODO: your code is here to return the dataset's length
         */
        return this->data_shape[0];
    }
    
    /* getitem:
     * return the data item (of type: DataLabel) that is specified by index
     */
    DataLabel<DType, LType> getitem(int index){
        /* TODO: your code is here
         */
        return DataLabel<DType, LType>(this->data[index], this->label[index]);
    }
    
    xt::svector<unsigned long> get_data_shape(){
        /* TODO: your code is here to return data_shape
         */
        return this->data_shape;
    }
    xt::svector<unsigned long> get_label_shape(){
        /* TODO: your code is here to return label_shape
         */
        return this->label_shape;
    }
};




#endif /* DATASET_H */

