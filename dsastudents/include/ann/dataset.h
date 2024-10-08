/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * This file contains the definition of Dataset, DataLabel, Batch, and TensorDataset classes
*/

#ifndef DATASET_H
#define DATASET_H
#include "ann/xtensor_lib.h"
#include "list/listheader.h"
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
    data(data), label(label){}
    xt::xarray<DType> getData() const { return data; }
    xt::xarray<LType> getLabel() const { return label; }
};

/*
! Batch class:
? This class is used to store a pair of data and label but in batch (multiple data and label)
* It's defined by original source
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
    xt::xarray<DType>& getData() {return data; }
    xt::xarray<LType>& getLabel() {return label; }
    const xt::xarray<DType>& getData() const {return data; }
    const xt::xarray<LType>& getLabel() const {return label; }
    friend std::ostream& operator<<(std::ostream& os, const Batch<DType, LType>& batch) {
        os << "Data: " << batch.data << ", Label: " << batch.label;
        return os;
    }
    bool operator==(const Batch<DType, LType>& other) const {
        return xt::allclose(data, other.data) && xt::allclose(label, other.label);
    }
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
    virtual bool is_empty_label() const = 0;
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
        exception_throw_ivlarg(data.dimension() == 0, "Dataset has no samples.");
        exception_throw_ivlarg(label.dimension() != 0 && data_shape[0] != label_shape[0], 
                                "Data and label must have the same number of samples.");
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

    // * len(): return the size of the dataset
    int len(){
        return (int)this->data_shape[0];
    }
    
    // * getitem(int index): return the DataLabel object at the specified index
    // * Notes: user can be use negative index
    // *        => use positive_index function in xtensor_lib.h
    DataLabel<DType, LType> getitem(int index){
        unsigned long pos_index = (unsigned long)index;
        // * Ensure that the index is within the valid range for both data and label
        if (pos_index >= data_shape[0]) {
            throw std::out_of_range("Index is out of bounds for number of datas.");
        }
        // * Check if the label tensor is uninitialized (i.e., it only contains a scalar or shape is zero)
        if (label.dimension() == 0) {
            auto empty_label = xt::xarray<LType>();
            auto slice_data = xt::view(data, pos_index);
            return DataLabel<DType, LType>(slice_data, empty_label);  
        }
        // * Ensure that the index is within the valid range for both data and label
        if (pos_index >= label_shape[0]) {
            throw std::out_of_range("Index is out of bounds for number of labels.");
        }
        auto slice_data = xt::view(data, pos_index);
        auto slice_label = xt::view(label, pos_index);
        // * Return a DataLabel object containing both the data and label at the specified index
        return DataLabel<DType, LType>(slice_data, slice_label);
    }

    // * get_data_shape(): return the shape of the data
    xt::svector<unsigned long> get_data_shape(){
        return this->data_shape;
    }

    // * get_label_shape(): return the shape of the label
    xt::svector<unsigned long> get_label_shape(){
        return this->label_shape;
    }

    bool is_empty_label() const {
        return label.dimension() == 0;
    }
};
#endif /* DATASET_H */

