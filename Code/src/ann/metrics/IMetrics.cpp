/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-06
    * Version 1.0
    * This file implements the Metrics API class
*/

#include "metrics/IMetrics.h"

IMetrics::IMetrics(int nOutputs): m_nOutputs(nOutputs) {
}

IMetrics::IMetrics(const IMetrics& orig) {
}

IMetrics::~IMetrics() {
}
double IMetrics::evaluate(xt::xarray<double> pred, xt::xarray<double> target){
    accumulate(target, pred);
    return m_metrics(0);
}

void IMetrics::accumulate(double_tensor y_true, double_tensor y_pred){
    ulong prev_nsamples = m_sample_counter;
    ulong batch_size = y_true.shape()[0];
    m_sample_counter += batch_size;
    m_metrics = prev_nsamples*m_metrics + batch_size*calculate_metrics(y_true, y_pred);
    m_metrics = m_metrics/m_sample_counter;
    //cout << "bcc: " << calc_metrics(y_true, y_pred) << endl;
    //cout << "acc: " << m_train_metrics << endl;
}
