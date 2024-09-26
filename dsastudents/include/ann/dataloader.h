/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file contains the definition of DataLoader class
*/

#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"

using namespace std;

/*
! DataLoader class:
? This class is used to load data from a dataset in batch
TODO: You need to implement the following methods:
* Constructor
* Destructor
* ShuffleDataset(): if "shuffle" is true, shuffle the dataset, using uniform random permutation
* SplitDataset(): split the dataset into batches
                  *remember: if "drop_last" is false, the last batch may have fewer samples than "batch_size"
* Iterator class: support the iteration and for-each loop
* begin(): return an iterator to the first batch
* end(): return an iterator to the end of the batches

! Explain function of xtensor library be used in this file:
* xt::arange(n): return an array with values from 0 to n-1
* xt::random::shuffle(indices): shuffle the indices
* xt::zeros(shape): return an array with shape and all elements are zeros
* xt::view(array, idx): return a view of the idx-th element of the array
* xt::amax(array): return the maximum value of the array
* xt::sum(array, axis): return the sum of the array along the axis
*/
template<typename DType, typename LType>
class DataLoader{
public:
    
private:
    // ? This is part of the original source code
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    // * Add more member variables to support the iteration
    xvector<Batch<DType, LType>> batches;
    xt::xarray<int> indices;
    ulong n_samples;
    ulong n_batches;
    ulong n_remain;
public:
    // * Constructor
    DataLoader(Dataset<DType, LType>* ptr_dataset,
            int batch_size,
            bool shuffle=true,
            bool drop_last=false){
        
        // * Initialize the member variables
        this->ptr_dataset = ptr_dataset;
        for (size_t i = 0; i < ptr_dataset->len(); i++) {
            this->ptr_dataset->getitem(i) = ptr_dataset->getitem(i);
        }
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->n_samples = this->ptr_dataset->len();

        // * Calculate the number of batches and remaining samples
        this->n_batches = this->n_samples / this->batch_size;
        this->n_remain = this->n_samples % this->batch_size;
        // * If drop_last is false and there are remaining samples
        if(this->drop_last == false && this->n_remain > 0){
            this->n_batches += 1;
        }

        this->batches = xvector<Batch<DType, LType>>(0, 0, n_batches);

        // * Shuffle the dataset if needed
        if(this->shuffle == true){
            this->ShuffleDataset();
        }

        // * Split the dataset into batches
        this->SplitDataset();
    }
    virtual ~DataLoader(){delete ptr_dataset;}
private:
    void ShuffleDataset() {

        // * Create a random permutation of indices
        this->indices = xt::arange(n_samples);
        xt::random::shuffle(indices);

        // * Create a new dataset with the shuffled data
        // * The shape of new data and label is the same as the original dataset
        xt::svector<size_t> data_shape;
        data_shape.push_back(n_samples);
        for (size_t i = 1; i < this->ptr_dataset->get_data_shape().size(); i++) {
            data_shape.push_back(this->ptr_dataset->get_data_shape()[i]);
        }
        xt::svector<size_t> label_shape;
        label_shape.push_back(n_samples);
        for (size_t i = 1; i < this->ptr_dataset->get_label_shape().size(); i++) {
            label_shape.push_back(this->ptr_dataset->get_label_shape()[i]);
        }

        // * Create new xarrays for data and label
        xt::xarray<DType> data = xt::zeros<DType>(data_shape);
        xt::xarray<LType> label = xt::zeros<LType>(label_shape);
        for (size_t i = 0; i < n_samples; i++) {
            xt::view(data, i) = this->ptr_dataset->getitem(indices[i]).getData();
            xt::view(label, i) = this->ptr_dataset->getitem(indices[i]).getLabel();
        }
        
        // * Create a new dataset with the shuffled data
        this->ptr_dataset = new TensorDataset<DType, LType>(std::move(data), std::move(label));
    }

    void SplitDataset() {
        for (size_t i = 0; i < this->n_batches; i++) {
            // * Calculate the start and end index of the batch
            size_t start = i * this->batch_size;
            size_t end = (i + 1) * this->batch_size;
            if (i == n_batches - 1 && n_remain > 0) {
                end = start + n_remain;
            }

            // * Create a new batch
            xt::svector<size_t> data_shape;
            data_shape.push_back(end - start);
            for (size_t j = 1; j < this->ptr_dataset->get_data_shape().size(); j++) {
                data_shape.push_back(this->ptr_dataset->get_data_shape()[j]);
            }
            xt::svector<size_t> label_shape;
            label_shape.push_back(end - start);
            for (size_t j = 1; j < this->ptr_dataset->get_label_shape().size(); j++) {
                label_shape.push_back(this->ptr_dataset->get_label_shape()[j]);
            }

            xt::xarray<DType> data = xt::zeros<DType>(data_shape);
            xt::xarray<LType> label = xt::zeros<LType>(label_shape);

            for (size_t j = start; j < end; j++) {
                xt::view(data, j - start) = this->ptr_dataset->getitem(j).getData();
                xt::view(label, j - start) = this->ptr_dataset->getitem(j).getLabel();
            }

            // * Add the new batch to the list of batches
            Batch<DType, LType> batch(std::move(data), std::move(label));
            this->batches.add(batch);
        }
    }
public:
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
    /* 
    / begin and end for the for-each loop
    / overload * operator to return the pointer to the current batch
    / overload ++ operator to move to the next batch
    / overload != operator to compare two iterators
    */
    class Iterator{
    private:
        DataLoader* ptr_loader;
        int index;
    public:
        Iterator(DataLoader* ptr_loader, int index): ptr_loader(ptr_loader), index(index){}

        // Copy constructor
        Iterator(const Iterator& other): ptr_loader(other.ptr_loader), index(other.index){}
        // Copy assignment operator
        Iterator& operator=(const Iterator& other){
            if(this != &other){
                ptr_loader = other.ptr_loader;
                index = other.index;
            }
            return *this;
        }

        // * Overload the operators*
        Batch<DType, LType>& operator*(){
            return ptr_loader->batches.get(index);
        }

        // * Overload the operators++
        // prefix ++
        Iterator& operator++(){
            index++;
            return *this;
        }
        // postfix ++
        Iterator operator++(int){
            Iterator temp = *this;
            index++;
            return temp;
        }

        // * Overload the operators!=
        bool operator!=(const Iterator& other){
            return index != other.index;
        }
    };

    // * begin
    Iterator begin(){
        return Iterator(this, 0);
    }
    // * end
    Iterator end(){
        return Iterator(this, this->batches.size());
    }
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};
#endif /* DATALOADER_H */