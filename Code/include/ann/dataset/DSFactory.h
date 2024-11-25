/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   DSFactory.h
 * Author: ltsach
 *
 * Created on October 7, 2024, 8:56 AM
 */

#ifndef DSFACTORY_H
#define DSFACTORY_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem> //require C++17
namespace fs = std::filesystem;
#include "ann/functions.h"
#include "config/Config.h"
#include "loader/dataset.h"
#include "loader/dataloader.h"
using namespace std;
#include "dsaheader.h"

class DSFactory {
public:
    DSFactory(string cfg_filename);
    DSFactory(const DSFactory& orig);
    virtual ~DSFactory();
    
    xmap<string, TensorDataset<double, double>*>* get_datasets_3cc();
    xmap<string, TensorDataset<double, double>*>* get_datasets_2cc();
    
protected:
    
    Config* m_pConfig;
private:

};

#endif /* DSFACTORY_H */

