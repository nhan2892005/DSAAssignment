/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   xt_demo.h
 * Author: ltsach
 *
 * Created on September 1, 2024, 3:44 PM
 */

#ifndef XT_DEMO_H
#define XT_DEMO_H

#include "tensor/xtensor/xarray.hpp"
#include "tensor/xtensor/xio.hpp"
#include "tensor/xtensor/xview.hpp"
#include "tensor/xtensor/xbuilder.hpp"
#include "tensor/xtensor-blas/xlinalg.hpp"
#include "tensor/xtensor/xrandom.hpp"
#include <ctime>

void demo_1(){
    xt::xarray<double> data = xt::zeros<double>({2, 3});
    xt::xarray<int> label = xt::ones<int>({2});
    cout << data << endl;
    cout << label << endl;
}

void demo_2(){
    xt::xarray<double> data = xt::zeros<double>({2, 3});
    xt::xarray<int> label = xt::ones<int>({2});
    
    xt::xarray<double> row = {6, 10, 12};
    xt::view(data, 0) = row;
    label[0] = 10;
    xt::xarray<int> alabel = {100};
    
    
    cout << "data(dim, shape[0]): " << data.dimension() << ", " << data.shape()[0] << endl;
    cout << "row(dim, shape[0]): " << row.dimension() << ", " << row.shape()[0] << endl;
    cout << "alabel(dim, shape[0]): " << alabel.dimension() << ", " << alabel.shape()[0] << endl;
    
    cout << "view (dim, shape[0]): " << xt::view(label, 1).dimension() << ", " << xt::view(label, 1).shape()[0] << endl;
    cout << data << endl;
    cout << label << endl;
}


#endif /* XT_DEMO_H */

