/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file contains the definition of Fully Connected Layer class
    * This class is derived from Layer class
    * Fully Connected Layer is a layer that connects every neuron in one layer to every neuron in another layer
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
    static FCLayer* fromPretrained(string filename, bool use_bias){
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
            return Inputlayer;
        }
        return nullptr;
    };

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

