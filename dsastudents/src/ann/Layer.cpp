/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file implements class Layer
*/

#include "ann/Layer.h"

Layer::Layer() {
    layer_idx++;
    name = "Layer_" + to_string(layer_idx);
    is_training = true;
}

Layer::Layer(const Layer& orig) {
    is_training = orig.is_training;
    name = orig.name;
    layer_idx++;
}

Layer::~Layer() {
}

unsigned long long Layer::layer_idx = -1;

