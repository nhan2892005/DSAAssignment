/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   MetricLayer.cpp
 * Author: ltsach
 * 
 * Created on September 2, 2024, 11:41 AM
 */

#include "metrics/IMetrics.h"

IMetrics::IMetrics(int nOutputs): m_nOutputs(nOutputs) {
}

IMetrics::IMetrics(const IMetrics& orig) {
}

IMetrics::~IMetrics() {
}
double IMetrics::evaluate(xt::xarray<double> pred, xt::xarray<double> target){
    return 0;
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
