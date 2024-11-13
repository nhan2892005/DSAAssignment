/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Config.cpp
 * Author: ltsach
 * 
 * Created on October 6, 2024, 9:04 PM
 */

#include "config/Config.h"
#include "sformat/fmt_lib.h"

Config::Config(string cfg_filename): m_cfg_filename(cfg_filename) {
   m_pMap = new xmap<string, string>(&stringHash);
    //
   load_default();
   load_from(cfg_filename);
}



Config::Config(const Config& orig) {
}

Config::~Config() {
    delete m_pMap;
}

void Config::load_default(){
    m_pMap->put("model_root", "./models");
    m_pMap->put("ckpt_name", "checkpoint");
    m_pMap->put("arch_file", "arch.txt");
    m_pMap->put("dataset_root", "datasets");
}

void Config::load_from(string filename){
    ifstream datastream(filename);
    if(!datastream.is_open()){
        string message = fmt::format("{:s}: can not open for reading.", filename); 
        cerr << message << endl;
        return;
    }
    //read header: to be here
    
    //read and parse lines
    string line;
    while(getline(datastream, line)){
        //skip empty and comment line (started with #)
        line = trim(line);
        if(line.size() == 0) continue;
        if(line[0] == '#') continue;
        
        //parse line
        char delimiter=':';
        istringstream linestream(line);
        string key, value;
        getline(linestream, key, delimiter); //key: maybe an empty string
        getline(linestream, value, delimiter); //value: maybe an empty string
        key = trim(key); value = trim(value);
        if((key.size() == 0) || (value.size() == 0)) continue;
        
        key = to_lower(key);
        m_pMap->put(key, value);
    }
    datastream.close();
}
string Config::get(string key, string def_value){
    string value;
    try{
        value = m_pMap->get(key);
    }
    catch(KeyNotFound& e){
        value = def_value;
    }
    return value;
}
string Config::get_new_checkpoint(string model_name){
    string model_root = get("model_root", "./models");
    string ckpt_name = get("ckpt_name", "checkpoint");
    string model_path = fs::path(model_root) / fs::path(model_name);
    if(!fs::exists(model_path)) // the first checkpoint: checkpoint-1
        return fs::path(model_path) / fs::path(ckpt_name + "-1");
    
    //find the largest idx
    int largest = 0;
    for (const auto & entry : fs::directory_iterator(model_path)){
        if(entry.is_directory()){
            string path = entry.path().string();
            string ckpt_idx_str = path.substr(path.rfind("-") + 1);
            int ckpt_idx;
            try{
                ckpt_idx = stoi(ckpt_idx_str);
            }
            catch(std::invalid_argument& e){
                //cout << "invalid_argument" << endl;
                ckpt_idx = -1;
            }
            if(largest < ckpt_idx) largest = ckpt_idx;
        }
    }
    int next_idx = largest + 1;
    string ckpt_folder = ckpt_name + "-" + to_string(next_idx);
    return fs::path(model_path) / fs::path(ckpt_folder);
}