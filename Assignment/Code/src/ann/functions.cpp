#include "ann/functions.h"
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <limits>



xt::xarray<double> softmax(xt::xarray<double> X, int axis){
    xt::svector<unsigned long> shape = X.shape();
    axis = positive_index(axis, shape.size());
    shape[axis] = 1;
    
    xt::xarray<double> Xmax = xt::amax(X, axis);
    X = xt::exp(X - Xmax.reshape(shape));
    xt::xarray<double> SX = xt::sum(X, -1); SX = SX.reshape(shape);
    X = X/SX;
    
    return X;
}

/*
 */
double cross_entropy(xt::xarray<double> Ypred, xt::xarray<double> Ygt, bool mean_reduced){
    int nsamples = Ypred.shape()[0];
    xt::xarray<double> logYpred = xt::log(Ypred + 1e-7);
    xt::xarray<double> R = -Ygt * logYpred;
    R = xt::sum(R, -1);
    
    xt::xarray<double>  sum = xt::sum(R);
    if(mean_reduced) return (sum/nsamples)[0];
    else return sum[0];
}

/*
 */
double cross_entropy(xt::xarray<double> Ypred, xt::xarray<unsigned long> ygt, bool mean_reduced){
    int nclasses = Ypred.shape()[1];
    xt::xarray<double> Ytarget = onehot_enc(ygt, nclasses);
    
    return cross_entropy(Ypred, Ytarget, mean_reduced);
}

xt::xarray<double> onehot_enc(xt::xarray<unsigned long> x, int nclasses){
    int nsamples = x.shape()[0];
    xt::xarray<double> Y = xt::zeros<double>({nsamples, nclasses});
    for(int r=0; r < nsamples; r++){
        int c = x[r];
        xt:view(Y, r, c) = 1.0;
    }
    return Y;
}

int stringHash(string& str, int size) {
    long long int sum = 0;
    for (int idx = 0; idx < str.length(); idx++) sum += str[idx];
    return sum % size;
}

// trim from start (in place)
string ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// trim from end (in place)
string rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}
string trim(std::string &s) {
    rtrim(s);
    ltrim(s);
    return s;
}

string to_lower(const string& str){
    string lowercase = "";
    for (char ch : str) lowercase += std::tolower(ch);
    return lowercase;
}

void estimate_params(xt::xarray<double> X, xt::xarray<double>& mu, xt::xarray<double>& sigma){
    mu = xt::mean(X, 0);
    sigma = xt::stddev(X, 0);
    return;
}
xt::xarray<double> normalize(xt::xarray<double> X, xt::xarray<double> mu, xt::xarray<double> sigma){
    return (X - mu)/sigma;
}


ulong_tensor confusion_matrix(ulong_tensor y_true, ulong_tensor y_pred,  int nclasses){
    //int nclasses = xt::amax(y_true)[0] + 1;
    int nsamples = y_true.shape()[0];
    
    ulong_tensor C = xt::zeros<ulong>({nclasses, nclasses});
    ulong_tensor indices = xt::arange<ulong>(0, nsamples);
    for(auto idx: indices){
        ulong r = y_true[idx];
        ulong c = y_pred[idx];
        xt::view(C, r, c) += 1;
    }
    return C;
}
xt::xarray<ulong> class_count(xt::xarray<ulong> confusion){
    xt::xarray<ulong> count = xt::sum(confusion, -1);
    return count;
}

double_tensor calc_classifcation_metrics(ulong_tensor y_true, ulong_tensor y_pred,  int nclasses){
    double_tensor lookup = xt::zeros<double>({NUM_CLASS_METRICS});
    
    ulong_tensor C = confusion_matrix(y_true, y_pred, nclasses);
    ulong nsamples = xt::sum(C)[0];
    ulong ncorrect = xt::sum(xt::diagonal(C))[0];
    
    double_tensor label_per_class = xt::cast<double>(xt::sum(C, 1));
    double_tensor pred_per_class = xt::cast<double>(xt::sum(C, 0));
    double_tensor diag = xt::cast<double>(xt::diagonal(C));
    
    double_tensor prec_per_class = diag/pred_per_class;
    double_tensor recall_per_class =  diag/label_per_class;
    double_tensor weights = label_per_class/nsamples;
    double_tensor f1_measures = 2*diag/(pred_per_class + label_per_class);

    lookup[ulong(ACCURACY)] = double(ncorrect)/nsamples;
    lookup[ulong(PRECISION_MACRO)] = xt::mean(prec_per_class)[0];
    lookup[ulong(PRECISION_WEIGHTED)] = xt::sum(weights*prec_per_class)[0];
    lookup[ulong(RECALL_MACRO)] = xt::mean(recall_per_class)[0];
    lookup[ulong(RECALL_WEIGHTED)] = xt::sum(weights*recall_per_class)[0];
    lookup[ulong(F1_MEASURE_MACRO)] = xt::mean(f1_measures)[0];
    lookup[ulong(F1_MEASURE_WEIGHTED)] = xt::sum(weights*f1_measures)[0];
    
    return lookup;
}