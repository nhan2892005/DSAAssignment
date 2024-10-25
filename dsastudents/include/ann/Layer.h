/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file defines ADT for Layer
*/

#ifndef LAYER_H
#define LAYER_H
#include "ann/xtensor_lib.h"
#include "ann/funtions.h"
#include <string>
using namespace std;

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


#endif /* LAYER_H */

