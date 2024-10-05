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
//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType> {
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
public:
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label) 
    :data(data), label(label), data_shape(data.shape()), label_shape(label.shape()) {
        if (label.size() == 0) {
            this->label = xt::empty<LType>(data_shape);
            this->label_shape = data_shape;
        }
    }

    int len() {return int(this->data_shape[0]);}

    DataLabel<DType, LType> getitem(int index) {

        if(index < 0 || index >= this->len()){
            throw std::out_of_range("Index is out of range!");
        }

        unsigned long tmp_idx = static_cast<unsigned long>(index);
        xt::xarray<DType> temp_data;
        xt::xarray<LType> temp_label;
        if(this->data.size() > 1){
            temp_data = xt::view(this->data, tmp_idx);
        }
        if(this->label.size() > 1){
            temp_label = xt::view(this->label, tmp_idx);
        }
        return DataLabel<DType, LType>(temp_data, temp_label);
    }

    xt::svector<unsigned long> get_data_shape() { 
        return data_shape; 
    }

    xt::svector<unsigned long> get_label_shape() { 
        return label_shape; 
    }
};

#endif /* DATASET_H */

