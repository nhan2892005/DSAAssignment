/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   FCLayer.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 11:06 AM
 */

#ifndef FCLAYER_H
#define FCLAYER_H
#include "layer/ILayer.h"

class FCLayer: public ILayer {
public:
    FCLayer(int Nin=2, int Nout=10, bool use_bias=true);
    FCLayer(string sParams, string filename_w, string filename_b, string sName="");
    
    FCLayer(const FCLayer& orig);
    virtual ~FCLayer();
    
    xt::xarray<double> forward(xt::xarray<double> X);
    xt::xarray<double> backward(xt::xarray<double> DY);
    int register_params(IParamGroup* ptr_group);
    void save(string model_path);
    void load(string model_path, string layer_name="");
    int getNin(){return m_nNin; }
    int getNout(){return m_nNout; }
    string get_desc();
    void set_weights(double_tensor W){
        this->m_aWeights = W;
    }
    void set_bias(double_tensor b){
        this->m_aBias = b;
    }
    void set_use_bias(bool use_bias){
        this->m_bUse_Bias = use_bias;
    }
    bool has_learnable_param(){ return true; };
    LayerType get_type(){ return LayerType::FC; };

protected:
    virtual void init_weights();
    
private:
    int m_nNin, m_nNout;
    bool m_bUse_Bias;
    
    xt::xarray<double> m_aWeights; //N_out x N_in
    xt::xarray<double> m_aBias;
    
    xt::xarray<double> m_aGrad_W;
    xt::xarray<double> m_aGrad_b;
    xt::xarray<double> m_aCached_X;
    unsigned long long m_unSample_Counter;
};


#endif /* FCLAYER_H */

