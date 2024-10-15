/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Config.h
 * Author: ltsach
 *
 * Created on October 6, 2024, 9:04 PM
 */

#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem> //require C++17
namespace fs = std::filesystem;
#include "ann/functions.h"
using namespace std;
#include "dsaheader.h"

class Config {
public:
    Config(string cfg_filename="config.txt");
    Config(const Config& orig);
    virtual ~Config();
    string get(string key, string def_value);
    string get_new_checkpoint(string model_name);
    
protected:
    virtual void load_default();
    virtual void load_from(string filename);
    
protected:
    string m_cfg_filename;
    xmap<string, string>* m_pMap;
   
private:

};

#endif /* CONFIG_H */

