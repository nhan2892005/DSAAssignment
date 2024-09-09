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
#include "ann/Layer.h"
#include <string>
using namespace std;

class FCLayer: public Layer {
public:
    FCLayer(int in_features=2, int out_features=10, bool use_bias=true);
    FCLayer(const FCLayer& orig);
    virtual ~FCLayer();
    
    xt::xarray<double> forward(xt::xarray<double> X);
    static FCLayer* fromPretrained(string filename, bool use_bias);

protected:
    virtual void init_weights();
private:
    int m_nIn_Features, m_nOut_Features;
    bool m_bUse_Bias;
    
    xt::xarray<double> m_aWeights; //out_features x in_features
    xt::xarray<double> m_aBias;
    
    xt::xarray<double> m_aGrad_W; //be used in Assignment-2
    xt::xarray<double> m_aGrad_b; //be used in Assignment-2
    xt::xarray<double> m_aCached_X; //be used in Assignment-2
    unsigned long long m_unSample_Counter; //be used in Assignment-2
};

#endif /* FCLAYER_H */

