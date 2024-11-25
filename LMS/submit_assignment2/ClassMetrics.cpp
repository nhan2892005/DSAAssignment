/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-06
    * Version 1.0
    * This file implements the Metrics class
*/

#include "metrics/ClassMetrics.h"
#include "ann/functions.h"

ClassMetrics::ClassMetrics(int nClasses): IMetrics(nClasses) {
    m_sample_counter = 0;
    m_metrics = xt::zeros<double>({NUM_CLASS_METRICS});
}

ClassMetrics::ClassMetrics(const ClassMetrics& orig): IMetrics(orig.m_nOutputs)  {
    m_sample_counter = orig.m_sample_counter;
    m_metrics = orig.m_metrics;
}

ClassMetrics::~ClassMetrics() {
}

void ClassMetrics::reset_metrics(){
    //YOUR CODE IS HERE
    m_sample_counter = 0;
    m_metrics = xt::zeros<double>({NUM_CLASS_METRICS});
    
}
double_tensor ClassMetrics::calculate_metrics(double_tensor y_true, double_tensor y_pred){
    return calc_classifcation_metrics(y_true, y_pred, m_nOutputs);
}