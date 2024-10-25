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
        Z(idx_X, idx_Y) = X(idx_X) * Y(idx_Y);
    }
    return Z;
}

xt::xarray<double> diag_stack(xt::xarray<double> X) {
    auto shape_X = X.shape();
    int dim_X = shape_X.size();
    int dim = dim_X + 1;
    int size = shape_X[0];
    xt::svector<unsigned long> shape(dim, 0);
    for(int idx=0; idx < dim_X; idx++) shape[idx] = shape_X[idx];
    shape[dim_X] = shape_X[0];

    xt::xarray<double> Z = xt::zeros<double>(shape);
    for(int idx=0; idx < size; idx++){
        Z(idx, idx) = X(idx);
    }
    return Z;
}

xt::xarray<double> matmul_on_stack(xt::xarray<double> X, xt::xarray<double> Y){
    auto shape_X = X.shape();
    auto shape_Y = Y.shape();

    cout << "Shape X: " << shape2str(shape_X) << endl;
    cout << "Shape Y: " << shape2str(shape_Y) << endl;

    if(shape_X[1] != shape_Y[0]){
        throw std::invalid_argument("Invalid shape");
    }

    int row_X = shape_X[0];
    int col_Y = shape_Y[1];

    xt::xarray<double> Z = xt::zeros<double>({row_X, col_Y});

    for (int row = 0; row < row_X; row++) {
        for (int col = 0; col < col_Y; col++) {
            // dot
            xt::xarray<double> x_row = xt::view(X, row, xt::all());
            xt::xarray<double> y_col = xt::view(Y, xt::all(), col);
            xt::xarray<double> z = x_row * y_col;
            Z(row, col) = xt::sum(z)();
        }
    }
    return Z;
}


ulong_array confusion_matrix(ulong_array y_true, ulong_array y_pred) {
    auto num_classes = xt::unique(y_true).size();
    ulong_array confusion = xt::zeros<ulong>({num_classes, num_classes});

    for (size_t idx = 0; idx < y_true.size(); ++idx) {
        confusion(y_true(idx), y_pred(idx)) += 1;
    }
    return confusion;
}

xt::xarray<ulong> class_count(xt::xarray<ulong> confusion) {
    return xt::sum(confusion, 0);
}

double_array calc_metrics(ulong_array y_true, ulong_array y_pred) {
    auto confusion = confusion_matrix(y_true, y_pred);
    auto num_classes = confusion.shape()[0];
    
    ulong true_positive_total = xt::linalg::trace(confusion)();
    
    ulong total_samples = y_true.size();
    double accuracy = static_cast<double>(true_positive_total) / total_samples;

    double precision_macro = 0.0, recall_macro = 0.0, f1_macro = 0.0;
    double precision_weighted = 0.0, recall_weighted = 0.0, f1_weighted = 0.0;
    auto class_counts = class_count(confusion);
    ulong_array true_positives = xt::diagonal(confusion);
    ulong_array false_positives = xt::sum(confusion, 1) - true_positives;
    ulong_array false_negatives = xt::sum(confusion, 0) - true_positives;
    
    for (size_t i = 0; i < num_classes; ++i) {
        ulong tp = true_positives(i);
        ulong fp = false_positives(i);
        ulong fn = false_negatives(i);
        ulong class_count = class_counts(i);
        
        double precision = (tp + fp > 0) ? static_cast<double>(tp) / (tp + fp) : 0.0;
        double recall = (tp + fn > 0) ? static_cast<double>(tp) / (tp + fn) : 0.0;
        double f1 = (precision + recall > 0) ? 2.0 * (precision * recall) / (precision + recall) : 0.0;

        precision_macro += precision;
        recall_macro += recall;
        f1_macro += f1;

        precision_weighted += precision * class_count;
        recall_weighted += recall * class_count;
        f1_weighted += f1 * class_count;
    }

    precision_macro /= num_classes;
    recall_macro /= num_classes;
    f1_macro /= num_classes;

    precision_weighted /= total_samples;
    recall_weighted /= total_samples;
    f1_weighted /= total_samples;

    double_array metrics = xt::zeros<double>({static_cast<size_t>(class_metrics::NUM_CLASS_METRICS)});
    metrics(static_cast<size_t>(class_metrics::ACCURACY)) = accuracy;
    metrics(static_cast<size_t>(class_metrics::PRECISION_MACRO)) = precision_macro;
    metrics(static_cast<size_t>(class_metrics::PRECISION_WEIGHTED)) = precision_weighted;
    metrics(static_cast<size_t>(class_metrics::RECALL_MACRO)) = recall_macro;
    metrics(static_cast<size_t>(class_metrics::RECALL_WEIGHTED)) = recall_weighted;
    metrics(static_cast<size_t>(class_metrics::F1_MEASURE_MACRO)) = f1_macro;
    metrics(static_cast<size_t>(class_metrics::F1_MEASURE_WEIGHTED)) = f1_weighted;

    return metrics;
}
