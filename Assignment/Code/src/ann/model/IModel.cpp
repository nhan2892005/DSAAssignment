/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "model/IModel.h"
#include "config/Config.h"
#include "sformat/fmt_lib.h"

IModel::IModel(string cfg_filename, string sModelName): 
    m_cfg_filename(cfg_filename), m_sModelName(sModelName){
    //Create configuration object
    m_pConfig = new Config(cfg_filename);
}

IModel::~IModel(){
    if(m_pConfig != nullptr) delete m_pConfig;
}

void IModel::fit(DataLoader<double, double>* pTrainLoader,
         DataLoader<double, double>* pValidLoader,
         unsigned int nepoch,
         unsigned int verbose){
    //
    on_begin_training(pTrainLoader, pValidLoader, nepoch, verbose);

    for(int epoch=1; epoch <= nepoch; epoch++){
        on_begin_epoch();
        m_pMetricLayer->reset_metrics();
        
        for(auto batch: *pTrainLoader){
            double_tensor X = batch.getData();
            double_tensor t = batch.getLabel();
            on_begin_step(X.shape()[0]);
            
            //(0) Set gradient buffer to zeros
            //YOUR CODE IS HERE
            
            //(1) FORWARD-Pass
            //YOUR CODE IS HERE
            
            //(2) BACKWARD-Pass
            //YOUR CODE IS HERE
            
            //(3) UPDATE learnable parameters
            //YOUR CODE IS HERE
            
            //Record the performance for each batch
            ulong_tensor y_true = xt::argmax(t, 1);
            ulong_tensor y_pred = xt::argmax(Y, 1);
            m_pMetricLayer->accumulate(y_true, y_pred);
            
            on_end_step(batch_loss);
        }//for-each batch: end
        on_end_epoch();
    }//for-epoch: end
    on_end_training();
}

//Method for doing the logging
void IModel::on_begin_training(
            DataLoader<double, double>* pTrainLoader,
            DataLoader<double, double>* pValidLoader,
            unsigned int nepoch,
            int verbose){
    this->m_pTrainLoader = pTrainLoader;
    this->m_pValidLoader = pValidLoader;
    this->m_nepoches = nepoch;
    this->m_verbose = verbose;
    
    this->m_current_epoch = 0;
    set_working_mode(true); //to training mode
    cout << "Start the training ..." << endl;
}
void IModel::on_end_training(){
    set_working_mode(false); //to inference mode
    cout << "End the training ..." << endl;
}
void IModel::on_begin_epoch(){
    this->m_current_epoch += 1; //the first epoch: 1
    this->m_current_batch = 0; //reset
    this->m_epoch_loss = 0; //reset
    this->m_sample_counter = 0; //reset
}
void IModel::on_end_epoch(){
    cout << "Validation results: " << endl;
    cout << this->evaluate(m_pValidLoader) << endl;
}
void IModel::on_begin_step(int batch_size){
    this->m_current_batch += 1; //the first batch: 1
    this->m_curent_batch_size = batch_size;
    this->m_sample_counter += batch_size;
    
}
void IModel::on_end_step(double batch_loss){
    this->m_epoch_loss += m_curent_batch_size * batch_loss;
    const double_tensor train_metrics = m_pMetricLayer->get_metrics();
    
    string message = fmt::format("{:3d}/{:3d}|{:4d}| {:6.2f} {:6.2f} | {:6.2f}",
            m_current_epoch, m_nepoches, m_current_batch,
            batch_loss, m_epoch_loss/m_sample_counter,
            train_metrics[ulong(ACCURACY)]);
    cout << message << endl;
}