/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   twoclasses.h
 * Author: ltsach
 *
 * Created on October 8, 2024, 8:42 PM
 */

#ifndef TWOCLASSES_H
#define TWOCLASSES_H
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

#include "sformat/fmt_lib.h"
#include "tensor/xtensor_lib.h"
#include "ann/annheader.h"
#include "loader/dataset.h"
#include "loader/dataloader.h"
#include "config/Config.h"
#include "dataset/DSFactory.h"
#include "optim/Adagrad.h"
#include "optim/Adam.h"

void twoclasses_classification(){
    DSFactory factory("./config.txt");
    xmap<string, TensorDataset<double, double>*>* pMap = factory.get_datasets_2cc();
    TensorDataset<double, double>* train_ds = pMap->get("train_ds");
    TensorDataset<double, double>* valid_ds = pMap->get("valid_ds");
    TensorDataset<double, double>* test_ds = pMap->get("test_ds");
    DataLoader<double, double> train_loader(train_ds, 50, true, false);
    DataLoader<double, double> valid_loader(valid_ds, 50, false, false);
    DataLoader<double, double> test_loader(test_ds, 50, false, false);
    
    int nClasses = 2;
    ILayer* layers[] = {
                    new FCLayer(2, 50, true),
                    new ReLU(),
                    new FCLayer(50, 20, true),
                    new ReLU(),
                    new FCLayer(20, nClasses, true),
                    new Softmax()
    };
    MLPClassifier model("./config.txt", "2c-classification", layers, sizeof(layers)/sizeof(ILayer*));
    
    
    SGD optim(2e-3);
    CrossEntropy loss;
    ClassMetrics metrics(nClasses);
    
    //train + eval
    model.compile(&optim, &loss, &metrics);
    model.fit(&train_loader, &valid_loader, 1000);
    string base_path = "./models";
    model.save(base_path + "/" + "2c-classification-1");
    double_tensor eval_rs = model.evaluate(&test_loader);
    cout << "Evaluation result on the testing dataset: " << endl;
    cout << eval_rs << endl;
    
    //load + eval
    MLPClassifier pretrained1("./config.txt");
    pretrained1.load(base_path + "/" + "2c-classification-1", true);
    double_tensor eval_rs1 = pretrained1.evaluate(&test_loader);
    cout << "Load + Eval a pretrained model : " << endl;
    cout << eval_rs1 << endl;
}


#endif /* TWOCLASSES_H */

