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
#define LOOP_in_range(i, start, end) for (size_t i = start; i < end; ++i)

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
    xt::xarray<int> indices;
    ulong n_samples;
    ulong n_batches;
    ulong n_remain;
    Batch<DType, LType>* storage_of_batches;
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

        // * Shuffle the dataset if needed
        if(this->shuffle == true){
            this->ShuffleDataset();
        }

        // * Split the dataset into batches
        this->SplitDataset();
    }
    virtual ~DataLoader(){delete[] storage_of_batches;}
private:
    void ShuffleDataset() {

        // * Create a random permutation of indices
        this->indices = xt::arange(n_samples);
        xt::random::shuffle(indices);

        // * Create a new dataset with the shuffled data
        // * The shape of new data and label is the same as the original dataset
        xt::svector<size_t> data_shape;
        data_shape.push_back(n_samples);
        size_t data_shape_size = this->ptr_dataset->get_data_shape().size();
        LOOP_in_range(i, 1, data_shape_size) {
            data_shape.push_back(this->ptr_dataset->get_data_shape()[i]);
        }
        xt::svector<size_t> label_shape;
        label_shape.push_back(n_samples);
        size_t label_shape_size = this->ptr_dataset->get_label_shape().size();
        LOOP_in_range(i, 1, data_shape_size) {
            label_shape.push_back(this->ptr_dataset->get_label_shape()[i]);
        }

        // * Create new xarrays for data and label
        xt::xarray<DType> data = xt::zeros<DType>(data_shape);
        xt::xarray<LType> label = xt::zeros<LType>(label_shape);
        LOOP_in_range(i, 0, n_samples) {
            xt::view(data, i) = this->ptr_dataset->getitem((int)indices(i)).getData();
            xt::view(label, i) = this->ptr_dataset->getitem((int)indices(i)).getLabel();
        }
        
        // * Create a new dataset with the shuffled data
        this->ptr_dataset = new TensorDataset<DType, LType>(std::move(data), std::move(label));
    }
    void SplitDataset() {
        storage_of_batches = new Batch<DType, LType>[n_batches];
        LOOP_in_range(i, 0, n_batches) {
            size_t start_idx_batch = i * batch_size;
            size_t end_idx_batch = (i + 1) * batch_size;
            if (i == n_batches - 1 && n_remain > 0 && drop_last == false) {
                end_idx_batch += n_remain;
            }
            xt::svector<size_t> data_shape;
            data_shape.push_back(end_idx_batch - start_idx_batch);
            for (size_t j = 1; j < this->ptr_dataset->get_data_shape().size(); j++) {
                data_shape.push_back(this->ptr_dataset->get_data_shape()[j]);
            }
            xt::svector<size_t> label_shape;
            label_shape.push_back(end_idx_batch - start_idx_batch);
            for (size_t j = 1; j < this->ptr_dataset->get_label_shape().size(); j++) {
                label_shape.push_back(this->ptr_dataset->get_label_shape()[j]);
            }

            xt::xarray<DType> data = xt::zeros<DType>(data_shape);
            xt::xarray<LType> label = xt::zeros<LType>(label_shape);
            for (size_t j = 0; j < batch_size; j++) {
                xt::view(data, j) = this->ptr_dataset->getitem(i * batch_size + j).getData();
                xt::view(label, j) = this->ptr_dataset->getitem(i * batch_size + j).getLabel();
            }
            storage_of_batches[i] = Batch<DType, LType>(std::move(data), std::move(label));
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
            return ptr_loader->storage_of_batches[index];
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
        return Iterator(this, this->n_batches);
    }
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};
#endif /* DATALOADER_H */