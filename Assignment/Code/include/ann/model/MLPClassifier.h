/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Model.h
 * Author: ltsach
 *
 * Created on September 1, 2024, 5:09 PM
 */

#ifndef MODEL_H
#define MODEL_H

#include "tensor/xtensor_lib.h"
#include "list/DLinkedList.h"
#include "layer/ILayer.h"
#include "layer/FCLayer.h"
#include "model/IModel.h"
#include "config/Config.h"

class MLPClassifier: public IModel {
public:
    MLPClassifier(string cfg_filename, string sModelName="MLPClassifier");
    MLPClassifier(string cfg_filename, string sModelName, ILayer** seq, int size);
    MLPClassifier(const MLPClassifier& orig);
    ~MLPClassifier();
    
    //for the inference mode:
    double_tensor predict(double_tensor X, 
                bool make_decision=false);
    double_tensor predict(
                DataLoader<double, double>* pLoader,
                bool make_decision=false);
    double_tensor evaluate(DataLoader<double, double>* pLoader);
    
    //for the training mode:
    void compile(
                IOptimizer* pOptimizer,
                ILossLayer* pLossLayer, 
                IMetrics* pMetricLayer);
    bool save(string model_path="");
    bool load(string model_path, bool use_name_in_file=false);
    
    
    void set_working_mode(bool trainable);
    int get_num_classes(){
        FCLayer* pLayer = (FCLayer*)m_layers.get(m_layers.size() - 2); 
        return pLayer->getNout();
    };

protected:
    double_tensor forward(double_tensor X);
    void backward();
    
protected:
    DLinkedList<ILayer*> m_layers;
    
private:
};

#endif /* MODEL_H */

