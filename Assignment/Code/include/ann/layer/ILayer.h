/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Layer.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 10:54 AM
 */

#ifndef LAYER_H
#define LAYER_H
#include "tensor/xtensor_lib.h"
#include "ann/functions.h"
#include "optim/IParamGroup.h"
#include <string>
using namespace std;

enum LayerType{
    FC=0,
    RELU,
    SIGMOID,
    TANH,
    SOFTMAX,
    NUM_LAYERS
};
class ILayer {
public:
    ILayer();
    ILayer(const ILayer& orig);
    virtual ~ILayer();
    
    virtual void set_working_mode(bool mode=true){ m_trainable = mode; };
    virtual xt::xarray<double> forward(xt::xarray<double> X)=0;
    virtual xt::xarray<double> backward(xt::xarray<double> DY)=0;
    virtual void init_gradbuffer(){};
    virtual int register_params(IParamGroup* ptr_group){ return 0; } //default: 0=no learnable parameters
    virtual string getname(){return m_sName; }
    virtual void setname(string name){m_sName = name; }
    virtual string get_desc()=0;
    virtual void save(string model_path){};
    virtual void load(string model_path, string layer_name=""){};
    virtual bool has_learnable_param(){ return false; };
    virtual LayerType get_type()=0;

protected:
    bool m_trainable;
    static unsigned long long m_unLayer_idx;
    string m_sName;
private:
};


#endif /* LAYER_H */

