/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file implements the Fully Connected Layer
    * Fully Connected Layer is a layer that connects every neuron in one layer to every neuron in another layer
    * Feedforward Neural Networks are composed of multiple layers of neurons.
    * a0 = x
    * z1 = W1*a0 + b1
*/

#include <fstream>
#include "ann/FCLayer.h"
#include "ann/funtions.h"

FCLayer::FCLayer(int in_features, int out_features, bool use_bias) {
    this->m_nIn_Features = in_features;
    this->m_nOut_Features = out_features;
    this->m_bUse_Bias = use_bias;
    name = "FC_" + to_string(layer_idx);
    m_unSample_Counter = 0;
    
    init_weights();
}
void FCLayer::init_weights(){
    m_aWeights = 0.01 * xt::random::randn<double>({m_nOut_Features, m_nIn_Features});
    if (m_bUse_Bias){
        m_aBias = xt::zeros<double>({m_nOut_Features, 1});
    }
}

FCLayer::FCLayer(const FCLayer& orig) {
    name = "FC_" + to_string(++layer_idx);
    this->m_nIn_Features = orig.m_nIn_Features;
    this->m_nOut_Features = orig.m_nOut_Features;
    this->m_bUse_Bias = orig.m_bUse_Bias;
    this->m_aWeights = orig.m_aWeights;
    this->m_aBias = orig.m_aBias;
    this->m_unSample_Counter = orig.m_unSample_Counter;
    this->m_aGrad_W = orig.m_aGrad_W;
    this->m_aGrad_b = orig.m_aGrad_b;
    this->m_aCached_X = orig.m_aCached_X;
}

FCLayer::~FCLayer() {  
}

xt::xarray<double> FCLayer::forward(xt::xarray<double> X) {
    // * Store the input X for backpropagation
    m_aCached_X = X;

    // * Calculate the output Z
    // * Z = W*X + b
    xt::xarray<double> Z = matmul_on_stack(m_aWeights, X);

    // * Add bias if needed
    if (m_bUse_Bias){
        Z = Z + m_aBias;
    }
    return Z;
}

