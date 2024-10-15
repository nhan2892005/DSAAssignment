/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   ClassMetrics.h
 * Author: ltsach
 *
 * Created on October 7, 2024, 5:14 PM
 */

#ifndef CLASSMETRICS_H
#define CLASSMETRICS_H
#include "metrics/IMetrics.h"

class ClassMetrics: public IMetrics{
public:
    ClassMetrics(int nClasses);
    ClassMetrics(const ClassMetrics& orig);
    virtual ~ClassMetrics();
    
    void reset_metrics();
    double_tensor calculate_metrics(double_tensor y_true, double_tensor y_pred);
private:

};

#endif /* CLASSMETRICS_H */

