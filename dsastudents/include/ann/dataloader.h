/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

/*
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

// #ifndef DATASET_H
// #define DATASET_H
// #include "ann/xtensor_lib.h"
// using namespace std;

// template<typename DType, typename LType>
// class DataLabel{
// private:
//     xt::xarray<DType> data;
//     xt::xarray<LType> label;
// public:
//     DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label):
//     data(data), label(label){
//     }
//     xt::xarray<DType> getData() const{ return data; }
//     xt::xarray<LType> getLabel() const{ return label; }
// };

// template<typename DType, typename LType>
// class Batch{
// private:
//     xt::xarray<DType> data;
//     xt::xarray<LType> label;
// public:
//     Batch(xt::xarray<DType> data,  xt::xarray<LType> label):
//     data(data), label(label){
//     }
//     virtual ~Batch(){}
//     xt::xarray<DType>& getData(){return data; }
//     xt::xarray<LType>& getLabel(){return label; }
// };


// template<typename DType, typename LType>
// class Dataset{
// private:
// public:
//     Dataset(){};
//     virtual ~Dataset(){};
    
//     virtual int len()=0;
//     virtual DataLabel<DType, LType> getitem(int index)=0;
//     virtual xt::svector<unsigned long> get_data_shape()=0;
//     virtual xt::svector<unsigned long> get_label_shape()=0;
    
// };

// //////////////////////////////////////////////////////////////////////
// template<typename DType, typename LType>
// class TensorDataset: public Dataset<DType, LType>{
// private:
//     xt::xarray<DType> data;
//     xt::xarray<LType> label;
//     xt::svector<unsigned long> data_shape, label_shape;
    
// public:
//     /* TensorDataset: 
//      * need to initialize:
//      * 1. data, label;
//      * 2. data_shape, label_shape
//     */
//     TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label){
//         /* TODO: your code is here for the initialization
//          */
//         this->data = data;
//         this->label = label;
//         this->data_shape = data.shape();
//         this->label_shape = label.shape();
//     }
//     /* len():
//      *  return the size of dimension 0
//     */
//     int len(){
//         /* TODO: your code is here to return the dataset's length
//          */
//         return this->data_shape[0];
//     }
    
//     /* getitem:
//      * return the data item (of type: DataLabel) that is specified by index
//      */
//     DataLabel<DType, LType> getitem(int index){
//         /* TODO: your code is here
//          */
//         return DataLabel<DType, LType>(this->data[index], this->label[index]);
//     }
    
//     xt::svector<unsigned long> get_data_shape(){
//         /* TODO: your code is here to return data_shape
//          */
//         return this->data_shape;
//     }
//     xt::svector<unsigned long> get_label_shape(){
//         /* TODO: your code is here to return label_shape
//          */
//         return this->label_shape;
//     }
// };




// #endif /* DATASET_H */


#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    /*TODO: add more member variables to support the iteration*/
    xt::svector<Batch<DType, LType>, 4> batches;
    xt::xarray<int> indices;
    ulong n_samples;
    ulong n_batches;
    ulong n_remain;
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset,
            int batch_size,
            bool shuffle=true,
            bool drop_last=false){
        /*TODO: Add your code to do the initialization */
        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->n_samples = this->ptr_dataset->len();
        this->n_batches = this->n_samples / this->batch_size;
        this->n_remain = this->n_samples % this->batch_size;
        if(this->drop_last == false && this->n_remain > 0){
            this->n_batches += 1;
        }
        if(this->shuffle == true){
            this->ShuffleDataset();
        }
        this->SplitDataset();
    }
    virtual ~DataLoader(){delete ptr_dataset;}
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
    
    /*TODO: Add your code here to support iteration on batch*/
    void ShuffleDataset() {
        unsigned long n_samples = this->ptr_dataset->len();
        this->indices = xt::arange(n_samples);
        xt::random::shuffle(indices);

        // Tạo hình dạng cho dữ liệu và nhãn
        std::vector<size_t> data_shape = {n_samples, this->ptr_dataset->get_data_shape()[1]};
        std::vector<size_t> label_shape = {n_samples, this->ptr_dataset->get_label_shape()[1]};

        xt::xarray<DType> data = xt::zeros<DType>(data_shape);
        xt::xarray<LType> label = xt::zeros<LType>(label_shape);

        for (size_t i = 0; i < n_samples; i++) {
            xt::view(data, i) = this->ptr_dataset->getitem(indices[i]).getData();
            xt::view(label, i) = this->ptr_dataset->getitem(indices[i]).getLabel();
        }

        // Giải phóng bộ nhớ của đối tượng cũ
        delete this->ptr_dataset;

        // Tạo đối tượng mới với dữ liệu đã xáo trộn
        this->ptr_dataset = new TensorDataset<DType, LType>(std::move(data), std::move(label));
    }


    void SplitDataset() {
        unsigned long n_samples = this->ptr_dataset->len();
        unsigned long n_batches = n_samples / this->batch_size;
        unsigned long n_remain = n_samples % this->batch_size;

        if (this->drop_last == false && n_remain > 0) {
            n_batches += 1;
        }

        for (size_t i = 0; i < n_batches; i++) {
            size_t start = i * this->batch_size;
            size_t end = (i + 1) * this->batch_size;

            if (i == n_batches - 1 && n_remain > 0) {
                end = start + n_remain;
            }

            // Creating the shape using std::vector<size_t> and casting to size_t
            std::vector<size_t> data_shape = {static_cast<size_t>(end - start), this->ptr_dataset->get_data_shape()[1]};
            std::vector<size_t> label_shape = {static_cast<size_t>(end - start), this->ptr_dataset->get_label_shape()[1]};

            xt::xarray<DType> data = xt::zeros<DType>(data_shape);
            xt::xarray<LType> label = xt::zeros<LType>(label_shape);

            for (size_t j = start; j < end; j++) {
                // Use xt::view to assign the row correctly
                xt::view(data, j - start) = this->ptr_dataset->getitem(j).getData();
                xt::view(label, j - start) = this->ptr_dataset->getitem(j).getLabel();
            }

            this->batches.push_back(Batch<DType, LType>(data, label));
        }
    }



    // begin and end for the for-each loop
    // overload * operator to return the pointer to the current batch
    // overload ++ operator to move to the next batch

    class Iterator{
    private:
        DataLoader* ptr_loader;
        int index;
    public:
        Iterator(DataLoader* ptr_loader, int index): ptr_loader(ptr_loader), index(index){}
        Batch<DType, LType>& operator*(){
            return ptr_loader->batches[index];
        }
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

        bool operator!=(const Iterator& other){
            return index != other.index;
        }
    };

    Iterator begin(){
        return Iterator(this, 0);
    }

    Iterator end(){
        return Iterator(this, this->batches.size());
    }
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

