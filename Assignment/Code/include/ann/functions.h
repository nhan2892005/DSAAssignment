/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   funtions.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 3:28 PM
 */

#ifndef FUNTIONS_H
#define FUNTIONS_H
#include <string>
#include <sstream>
using namespace std;
#include <memory>
#include <stdexcept>
#include <algorithm> 
#include <cctype>
#include <locale>

#include "tensor/xtensor_lib.h"
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





xt::xarray<double> softmax(xt::xarray<double> X, int axis=-1);
double cross_entropy(xt::xarray<double> Ypred, xt::xarray<double> Ygt, bool mean_reduced=true);
double cross_entropy(xt::xarray<double> Ypred, xt::xarray<unsigned long> ygt, bool mean_reduced=true);
xt::xarray<double> onehot_enc(xt::xarray<unsigned long> x, int nclasses);
xt::xarray<ulong> confusion_matrix(xt::xarray<ulong> y_true, xt::xarray<ulong> y_pred,  int nclasses);
xt::xarray<ulong> class_count(xt::xarray<ulong> confusion);
double_tensor calc_classifcation_metrics(ulong_tensor y_true, ulong_tensor y_pred, int nclasses);



int stringHash(string& str, int size);

/*
 * Thanks to:
 * https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
 */
// trim from start (in place)
string ltrim(std::string &s);
// trim from end (in place)
string rtrim(std::string &s);
// trim both ends
string trim(std::string &s);

string to_lower(const string& str);


void estimate_params(xt::xarray<double> X, xt::xarray<double>& mu, xt::xarray<double>& sigma);
xt::xarray<double> normalize(xt::xarray<double> X, xt::xarray<double> mu, xt::xarray<double> sigma);
#endif /* FUNTIONS_H */

