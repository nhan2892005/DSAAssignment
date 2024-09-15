/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   FCLayer.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 11:06 AM
 */

/*
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
*/

#include <fstream>
#include "ann/FCLayer.h"
#include "ann/funtions.h"

FCLayer::FCLayer(int in_features, int out_features, bool use_bias) {
    this->m_nIn_Features = in_features;
    this->m_nOut_Features = out_features;
    this->m_bUse_Bias = use_bias;
    name = "FC_" + to_string(++layer_idx);
    m_unSample_Counter = 0;
    
    init_weights();
}
void FCLayer::init_weights(){
   /*TODO: Your code is here*/
    m_aWeights = xt::random::randn<double>({m_nOut_Features, m_nIn_Features});
    if(m_bUse_Bias){
         m_aBias = xt::random::randn<double>({m_nOut_Features});
    }
    else{
         m_aBias = xt::zeros<double>({m_nOut_Features});
    }
}

FCLayer::FCLayer(const FCLayer& orig) {
    name = "FC_" + to_string(++layer_idx);
}

FCLayer::~FCLayer() {
    /*TODO: Your code is here*/     
}

xt::xarray<double> FCLayer::forward(xt::xarray<double> X) {
    /*TODO: Your code is here*/ 
    m_aCached_X = X;
    xt::xarray<double> Z = xt::linalg::dot(m_aWeights, X) + m_aBias;
    if (m_bUse_Bias){
        Z = Z + m_aBias;
    }
    return Z;
}

static FCLayer* FCLayer::fromPretrained(string filename, bool use_bias){
    /*TODO: Your code is here*/ 
    // Load mô hình đã huấn luyện từ file
    // Implement logic to load weights and biases from the file
    // Return a new FCLayer object with the loaded weights and biases
    // Return nullptr if the file does not exist
    fstream file(filename);
    file.open(filename, ios::in);
    if(file.is_open()){
        int in_features, out_features;
        file >> in_features;
        file >> out_features;
        FCLayer* Inputlayer = new FCLayer(in_features, out_features, use_bias);
        file.close();
        return layer;
    }
    return nullptr;
}