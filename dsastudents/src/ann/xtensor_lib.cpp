/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

/*
typedef unsigned long ulong;
typedef xt::xarray<ulong> ulong_array;
typedef xt::xarray<double> double_array;
enum class_metrics{
    ACCURACY = 0,
    PRECISION_MACRO,
    PRECISION_WEIGHTED,
    RECALL_MACRO,
    RECALL_WEIGHTED,
    F1_MEASURE_MACRO,
    F1_MEASURE_WEIGHTED,
    NUM_CLASS_METRICS
};
*/

#include "ann/xtensor_lib.h"


string shape2str(xt::svector<unsigned long> vec){
    stringstream ss;
    ss << "(";
    for(int idx=0; idx < vec.size(); idx++){
        ss << vec[idx] << ", ";
    }
    string res = ss.str();
    if(vec.size() > 1) res = res.substr(0, res.rfind(','));
    else res = res.substr(0, res.rfind(' '));
    return res + ")";
}

int positive_index(int idx, int size){
    // * NOTES: assume if idx >= size, then idx = idx - size
    if(idx >= size) return idx = idx - size;
    
    if(idx < 0) return idx = size + idx;
    return idx;
}

//should use einsum if it exists
xt::xarray<double> outer_stack(xt::xarray<double> X, xt::xarray<double>  Y){
    /*TODO: Your code is here*/
    auto shape_X = X.shape();
    auto shape_Y = Y.shape();

    int dim_X = shape_X.size();
    int dim_Y = shape_Y.size();

    int dim = dim_X + dim_Y;
    int size = shape_X[0] * shape_Y[0];
    xt::svector<unsigned long> shape(dim, 0);
    for(int idx=0; idx < dim_X; idx++) shape[idx] = shape_X[idx];
    for(int idx=0; idx < dim_Y; idx++) shape[dim_X + idx] = shape_Y[idx];

    xt::xarray<double> Z = xt::zeros<double>(shape);
    for(int idx=0; idx < size; idx++){
        int idx_X = idx / shape_Y[0];
        int idx_Y = idx % shape_Y[0];
        Z[idx_X][idx_Y] = X[idx_X] * Y[idx_Y];
    }
    return Z;
}
xt::xarray<double> diag_stack(xt::xarray<double> X){
    /*TODO: Your code is here*/
    auto shape_X = X.shape();
    int dim_X = shape_X.size();
    int size = shape_X[0];
    xt::svector<unsigned long> shape(dim_X + 1, 0);
    for(int idx=0; idx < dim_X; idx++) shape[idx] = shape_X[idx];
    shape[dim_X] = shape_X[0];

    xt::xarray<double> Z = xt::zeros<double>(shape);
    for(int idx=0; idx < size; idx++){
        Z[idx][idx] = X[idx];
    }
    return Z;    
}
xt::xarray<double> matmul_on_stack(xt::xarray<double> X, xt::xarray<double>  Y){
    /*TODO: Your code is here*/
    auto shape_X = X.shape();
    auto shape_Y = Y.shape();

    int dim_X = shape_X.size();
    int dim_Y = shape_Y.size();

    int dim = dim_X + dim_Y - 2;
    int size = shape_X[0] * shape_Y[1];
    xt::svector<unsigned long> shape(dim, 0);
    for(int idx=0; idx < dim_X - 1; idx++) shape[idx] = shape_X[idx];
    for(int idx=0; idx < dim_Y - 1; idx++) shape[dim_X - 1 + idx] = shape_Y[idx];

    xt::xarray<double> Z = xt::zeros<double>(shape);
    for(int idx=0; idx < size; idx++){
        int idx_X = idx / shape_Y[1];
        int idx_Y = idx % shape_Y[1];
        for(int jdx=0; jdx < shape_X[1]; jdx++){
            Z[idx_X][idx_Y] += X[idx_X][jdx] * Y[jdx][idx_Y];
        }
    }
    return Z;
}


ulong_array confusion_matrix(ulong_array y_true, ulong_array y_pred){
    /*TODO: Your code is here*/
    ulong n_classes = xt::amax(y_true)[0] + 1;
    ulong_array confusion = xt::zeros<ulong>({n_classes, n_classes});
    for(int idx=0; idx < y_true.size(); idx++){
        confusion[y_true[idx]][y_pred[idx]] += 1;
    }
    return confusion;
}
xt::xarray<ulong> class_count(xt::xarray<ulong> confusion){
    xt::xarray<ulong> count = xt::sum(confusion, -1);
    return count;
}

double_array calc_metrics(ulong_array y_true, ulong_array y_pred){
    /*TODO: Your code is here*/
    ulong_array confusion = confusion_matrix(y_true, y_pred);
    xt::xarray<ulong> count = class_count(confusion);

    double_array metrics = xt::zeros<double>({(int)class_metrics::NUM_CLASS_METRICS});
    for(int idx=0; idx < count.size(); idx++){
        double tp = confusion[idx][idx];
        double tp_fp = xt::sum(confusion[idx]);
        double tp_fn = xt::sum(confusion(_, idx));
        double tp_tn = xt::sum(confusion) - tp_fp - tp_fn + tp;

        double precision = tp_fp == 0? 0: tp / tp_fp;
        double recall = tp_fn == 0? 0: tp / tp_fn;
        double f1 = (precision + recall) == 0? 0: 2 * precision * recall / (precision + recall);

        metrics[(int)class_metrics::ACCURACY] += tp_tn;
        metrics[(int)class_metrics::PRECISION_MACRO] += precision;
        metrics[(int)class_metrics::RECALL_MACRO] += recall;
        metrics[(int)class_metrics::F1_MEASURE_MACRO] += f1;
    }
    metrics[(int)class_metrics::ACCURACY] /= xt::sum(confusion);
    metrics[(int)class_metrics::PRECISION_MACRO] /= count.size();
    metrics[(int)class_metrics::RECALL_MACRO] /= count.size();
    metrics[(int)class_metrics::F1_MEASURE_MACRO] /= count.size();

    return metrics;
}