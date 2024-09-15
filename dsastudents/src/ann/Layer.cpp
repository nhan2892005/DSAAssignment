/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Layer.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 10:54 AM
 */
/*
class Layer {
public:
    Layer();
    Layer(const Layer& orig);
    virtual ~Layer();
    
    virtual xt::xarray<double> forward(xt::xarray<double> X)=0;
    virtual string getname(){return name; }
protected:
    
    bool is_training;
    static unsigned long long layer_idx;
    string name;
private:
};
*/

#include "ann/Layer.h"

Layer::Layer() {
    /*TODO: Your code is here*/
    layer_idx++;
    name = "Layer_" + to_string(layer_idx);
    is_training = true;
}

Layer::Layer(const Layer& orig) {
    /*TODO: Your code is here*/
    is_training = orig.is_training;
    name = orig.name;
}

Layer::~Layer() {
}

unsigned long long Layer::layer_idx =0;

