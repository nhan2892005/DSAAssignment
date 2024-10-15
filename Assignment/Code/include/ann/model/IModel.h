/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   IModel.h
 * Author: ltsach
 *
 * Created on October 4, 2024, 8:53 AM
 */

#ifndef IMODEL_H
#define IMODEL_H
#include "tensor/xtensor_lib.h"
#include "optim/IOptimizer.h"
#include "loss/ILossLayer.h"
#include "metrics/IMetrics.h"
#include "loader/dataloader.h"
#include "config/Config.h"


class IModel {
public:
    IModel(string cfg_filename, string sModelName);
    virtual ~IModel();
    
    //for the inference mode:
    /* predict
     *  + X: tensor input
     *  + make_decision: only used for the classification models
     *      * make_decision = true: 
     *          => do argmax for single-label classification
     *          => or, do thresholding for multi-label classification
     *      * make_decision = false:
     *          => output probabilities for each class.
     *  + make_decision: do not use for regression.
     */
    virtual double_tensor predict(
                double_tensor X, 
                bool make_decision=false)=0;
    virtual double_tensor predict(
                DataLoader<double, double>* pLoader,
                bool make_decision=false)=0;
    virtual double_tensor evaluate(
                DataLoader<double, double>* pLoader)=0;
    
    
    //for the training mode:
    virtual void compile(
                IOptimizer* pOptimizer,
                ILossLayer* pLossLayer, 
                IMetrics* pMetricLayer)=0;
    
    /*
     * fit : used to train models
     *  + NOTES:
     *      * MUST CALL 'compile' before calling 'fit'
     */
    virtual void fit(
            DataLoader<double, double>* pTrainLoader,
            DataLoader<double, double>* pValidLoader,
            unsigned int nepoch=10,
            unsigned int verbose=1); //defined in this class
    
    /*
     * Subclasses of IModel should:
     * 1. set m_bIs_training := bIs_training 
     * 2. do its own task: set its layers to with the given training mode
     */
    virtual void set_working_mode(bool trainable)=0;
    
    
    //for loading and saving
    /* save:
     *  + model_path: the path where to store the model
     *      * if model_path: not exist
     *          => store in: model_root/model-name/checkpoint-k
     *      * if model_path: exist
     *          => store the model to model_path
     */
    virtual bool save(string model_path="")=0;
    /* load:
     *  + model_path: the path where to load the model
     *      * if model_path: not exist
     *          => return false
     *      * if model_path: exist
     *          => load the model from model_path
     *  + use_name_in_file = true:
     *      * use the name of layer specified in the model_path/arch.txt
     *  + use_name_in_file = false:
     *      * use the name created in layer's constructor. 
     */
    virtual bool load(string model_path, bool use_name_in_file=false)=0;
    
protected:
    virtual double_tensor forward(double_tensor X)=0;
    virtual void backward()=0;
    
protected:
    bool m_trainable; //TRUE: training; False: Inference
    string m_sModelName; //name of model (used in save/load)
    string m_cfg_filename; //config filename
    Config* m_pConfig; //config object
    
    /////////////////////////////////////////////////////////////
    // The following variables/methods: ONLY USED for method fit  
    // to avoid passing between method "on_xxxx"
    /////////////////////////////////////////////////////////////
    void on_begin_training(
            DataLoader<double, double>* pTrainLoader,
            DataLoader<double, double>* pValidLoader,
            unsigned int nepoch=10,
            int verbose=1);
    void on_end_training();
    void on_begin_epoch();
    void on_end_epoch();
    void on_begin_step(int batch_size);
    void on_end_step(double batch_loss);
    //
    IOptimizer* m_pOptimizer;
    ILossLayer* m_pLossLayer; 
    IMetrics* m_pMetricLayer;
    //
    DataLoader<double, double>* m_pTrainLoader;
    DataLoader<double, double>* m_pValidLoader;
    int m_nepoches; //total number of epoches
    int m_current_epoch; //current epoch-idx
    int m_current_batch; //current batch-idx
    int m_verbose; //lelvel of outputs
    double m_epoch_loss; //accumulated loss for epoch
    int m_curent_batch_size; //current batch-size
    int m_sample_counter; //total samples processed in epoch
private:
};

#endif /* IMODEL_H */

