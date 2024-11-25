/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   MetricLayer.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 11:41 AM
 */

#ifndef METRICLAYER_H
#define METRICLAYER_H

#include "tensor/xtensor_lib.h"

class IMetrics {
public:
    IMetrics(int nOutputs);
    IMetrics(const IMetrics& orig);
    virtual ~IMetrics();
    virtual double evaluate(xt::xarray<double> pred, xt::xarray<double> target);
    //
    virtual void reset_metrics()=0;
    virtual void accumulate(double_tensor y_true, double_tensor y_pred);
    virtual double_tensor calculate_metrics(double_tensor y_true, double_tensor y_pred) = 0;
    virtual const double_tensor& get_metrics(){return m_metrics; };
    virtual ulong get_counts(){ return m_sample_counter; }
    
protected:
    ulong m_sample_counter;
    double_tensor m_metrics;
    int m_nOutputs;
    
};

#endif /* METRICLAYER_H */

