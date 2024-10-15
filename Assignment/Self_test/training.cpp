#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdlib> 
#include <ctime> 
#include <string>
#include <random>
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
#include "ann/dataloader.h"
#include "ann/FCLayer.h"
#include "ann/ReLU.h"
#include "ann/Softmax.h"
#include "ann/BaseModel.h"

unsigned long N = 300;
unsigned long d0 = 2;
unsigned long C = 3; 
xt::svector<unsigned long> data_shape = {N, d0};
xt::xarray<double> Data(data_shape);
xt::svector<unsigned long> label_shape = {N};
xt::xarray<double> Labels(label_shape);
std::ifstream file("dataset.csv");

double cost(const xt::xarray<double>& Y, const xt::xarray<double>& Yhat) {
    auto loss = -xt::sum(Y * xt::log(Yhat)) / Y.shape()[1];

    return loss(0);
}

xt::xarray<double> convert_to_one_hot(xt::xarray<double> y) {
    std::vector<size_t> shape = {C, y.size()};
    xt::xarray<double> one_hot = xt::zeros<double>(shape);
    for (size_t i = 0; i < y.size(); ++i) {
        one_hot(y(i,0), i) = 1;
    }
    return one_hot;
}


void feedforward(BaseModel& model, xt::xarray<double>& X) {
    auto y_pred = model.predict(X);
    auto argmax = xt::argmax(y_pred, {0});

    auto confusion = confusion_matrix(Labels, argmax);
    auto counts = class_count(confusion);
    auto metrics = calc_metrics(Labels, argmax);
    cout << "Confusion Matrix: \n" << confusion << endl;
    cout << "Class Count: " << counts << endl;
    
    for (size_t i = 0; i < static_cast<size_t>(class_metrics::NUM_CLASS_METRICS); ++i) {
        cout << "Metrics[" << i << "]: " << metrics(i) << endl;
    }

    auto y_one_hot = convert_to_one_hot(Labels);
    auto loss = cost(y_one_hot, y_pred);

    cout << "Loss: " << loss << endl;
}

void read_csv(xt::xarray<double>& X, xt::xarray<double>& y) {
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << std::endl;
        return;
    }

    string line;
    int row = 0;
    while (getline(file, line)) {
        istringstream ss(line);
        string token;
        int col = 0;
        while (getline(ss, token, ',')) {
            try {
                if (col < d0) {
                    X(row, col) = stod(token);
                } else {
                    y(row) = stod(token);
                }
            } catch (const std::exception& e) {
                std::cerr << "Conversion error: " << e.what() << std::endl;
                return;
            }
            col++;
        }
        row++;
    }
    cout << "Shape of Data: " << shape2str(X.shape()) << endl;
    cout << "Shape Labels: " << shape2str(y.shape()) << endl;
}


int main() {
    read_csv(Data, Labels);
    TensorDataset dataset(Data, Labels);
    DataLoader dataloader(&dataset, 40, true, false);
    for (auto batch : dataloader) {
        auto X = batch.getData();
        auto y = batch.getLabel();
        std::cout << "Shape of X: " << shape2str(X.shape()) << std::endl;
        std::cout << "Shape of y: " << shape2str(y.shape()) << std::endl;
    }
    Layer** layers = new Layer*[4];
    layers[0] = new FCLayer(2, 20, true);
    layers[1] = new ReLU();
    layers[2] = new FCLayer(20, 3, true);
    layers[3] = new Softmax(0);
    Data = xt::transpose(Data);
    BaseModel model(layers, 4);
    feedforward(model, Data);
    
    delete[] layers;
    return 0;
}
