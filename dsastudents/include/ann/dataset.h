/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * This file contains the definition of Dataset, DataLabel, Batch, and TensorDataset classes
*/

#ifndef DATASET_H
#define DATASET_H
#include "ann/xtensor_lib.h"
using namespace std;

/*
! DataLabel class:
? This class is used to store a pair of data and label
* It's defined by original source
TODO: Do not modify this class
*/
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

/*
! Batch class:
? This class is used to store a pair of data and label but in batch (multiple data and label)
* It's defined by original source
TODO: Do not modify this class
*/
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

/*
! Dataset class:
? This class is an abstract class that defines the interface for a dataset
TODO: You need to implement the following methods:
* len(): return the size of the dataset
* getitem(int index): return the DataLabel object at the specified index
* get_data_shape(): return the shape of the data
* get_label_shape(): return the shape of the label
*/
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
/*
! TensorDataset class:
? This class is used to store a pair of data and label in the form of xarray
? It inherits from the Dataset class
TODO: You need to implement the following methods:
* len(): return the size of the dataset
* getitem(int index): return the DataLabel object at the specified index
* get_data_shape(): return the shape of the data
* get_label_shape(): return the shape of the label
*/
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

    // * Copy constructor
    TensorDataset(const TensorDataset& other)
        : data(other.data), label(other.label),
        data_shape(other.data_shape), label_shape(other.label_shape) {}

    // * Copy assignment operator
    TensorDataset& operator=(const TensorDataset& other) {
        if (this != &other) {
            data = other.data;
            label = other.label;
            data_shape = other.data_shape;
            label_shape = other.label_shape;
        }
        return *this;
    }

    ~TensorDataset() {}

    // * Move assignment operator
    TensorDataset& operator=(TensorDataset&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            label = std::move(other.label);
            data_shape = std::move(other.data_shape);
            label_shape = std::move(other.label_shape);
        }
        return *this;
    }

    // * len(): return the size of the dataset
    int len(){
        return this->data_shape[0];
    }
    
    // * getitem(int index): return the DataLabel object at the specified index
    // * Notes: user can be use negative index
    // *        => use positive_index function in xtensor_lib.h
    DataLabel<DType, LType> getitem(int index){
        index = positive_index(index, this->data_shape[0]);
        auto data_slice = xt::view(this->data, index, xt::all());
        auto label_slice = xt::view(this->label, index, xt::all());
        return DataLabel<DType, LType>(data_slice, label_slice);
    }

    
    // * get_data_shape(): return the shape of the data
    xt::svector<unsigned long> get_data_shape(){
        return this->data_shape;
    }

    // * get_label_shape(): return the shape of the label
    xt::svector<unsigned long> get_label_shape(){
        return this->label_shape;
    }
};
#endif /* DATASET_H */

