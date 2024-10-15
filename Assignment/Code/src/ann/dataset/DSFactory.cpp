/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   DSFactory.cpp
 * Author: ltsach
 * 
 * Created on October 7, 2024, 8:56 AM
 */

#include "dataset/DSFactory.h"

DSFactory::DSFactory(string cfg_filename) {
    m_pConfig = new Config(cfg_filename);
}

DSFactory::DSFactory(const DSFactory& orig) {
}

DSFactory::~DSFactory() {
    if(m_pConfig != nullptr) delete m_pConfig;
}

xmap<string, TensorDataset<double, double>*>* DSFactory::get_datasets_3cc(){
    //YOUR CODE IS HERE
}


xmap<string, TensorDataset<double, double>*>* DSFactory::get_datasets_2cc(){
    //prepare the path to files
    string ds_name = "2c-classification";
    string dataset_root = m_pConfig->get("dataset_root", "datasets");
    fs::path dataset_path = fs::path(dataset_root) / fs::path(ds_name);
    string train_file = (dataset_path / fs::path("2c_train.npy")).string();
    string valid_file = (dataset_path / fs::path("2c_valid.npy")).string();
    string test_file = (dataset_path / fs::path("2c_test.npy")).string();
    
    //load data from files
    // tables: the first two columns are data (i.e., x and y) and the last is target
    xt::xarray<double> train_table = xt::load_npy<double>(train_file);
    xt::xarray<double> valid_table = xt::load_npy<double>(valid_file);
    xt::xarray<double> test_table = xt::load_npy<double>(test_file);
    
    
    xt::xarray<double> mu, sigma;
    estimate_params(xt::view(train_table, xt::all(), xt::range(0,2)), mu, sigma);
    
    cout << shape2str(train_table.shape()) << endl;
    xt::xarray<double> X_train = normalize(xt::view(train_table, xt::all(), xt::range(0,2)), mu, sigma );
    xt::xarray<double> t_train = xt::view(train_table, xt::all(), -1);
    xt::xarray<double> T_train = onehot_enc(xt::cast<unsigned long>(t_train), 2);
    
    xt::xarray<double> X_valid = normalize(xt::view(valid_table, xt::all(), xt::range(0,2)), mu, sigma  );
    xt::xarray<double> t_valid = xt::view(valid_table, xt::all(), -1);
    xt::xarray<double> T_valid = onehot_enc(xt::cast<unsigned long>(t_valid), 2);
    
    xt::xarray<double> X_test = normalize(xt::view(test_table, xt::all(), xt::range(0,2)), mu, sigma  );
    xt::xarray<double> t_test = xt::view(test_table, xt::all(), -1);
    xt::xarray<double> T_test = onehot_enc(xt::cast<unsigned long>(t_test), 2);
    
  
    TensorDataset<double, double>* train_ds = new TensorDataset<double, double>(X_train, T_train);
    TensorDataset<double, double>* valid_ds = new TensorDataset<double, double>(X_valid, T_valid);
    TensorDataset<double, double>* test_ds = new TensorDataset<double, double>(X_test, T_test);
    
    xmap<string, TensorDataset<double, double>*>* pMap =
        new xmap<string, TensorDataset<double, double>*>(
            &stringHash,
            0.75, //load-factor
            0, //value-comparator: use ==
            xmap<string, TensorDataset<double, double>*>::freeValue);
    pMap->put("train_ds", train_ds);
    pMap->put("valid_ds", valid_ds);
    pMap->put("test_ds", test_ds);
    return pMap;
}
