#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
using namespace std;

#include "ann/functions.h"
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"
#include "loader/dataloader.h"
#include "loader/DataLoaderDemo.h"

using namespace std;
namespace fs = std::filesystem;
int num_task = 18;

vector<vector<string>> expected_task (num_task, vector<string>(10000, ""));
vector<vector<string>> output_task (num_task, vector<string>(10000, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/DataSetAndLoader/DataSetAndLoaderTestLog_Compare.txt";
    fstream file(log_file, ios::out);
    if (!file.is_open()) {
        std::cout << "Cannot open file" << std::endl;
        return;
    }
    
    streambuf* stream_buffer_cout = cout.rdbuf();
    cout.rdbuf(file.rdbuf());

    fstream file1(filename1);
    fstream file2(filename2);

    // Store content of tasks in vector
    string line;
    int num = 1;
    int num_line = 0;
    while (getline(file1, line)) {
        // if line contains "Task", store it in expected_task
        if (regex_match(line, regex("Task [0-9]+.*"))) {
            num_line = 0;
            // Extract numbers from the line
            regex number_regex("[0-9]+");
            auto numbers_begin = sregex_iterator(line.begin(), line.end(), number_regex);
            auto numbers_end = sregex_iterator();

            for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
                std::smatch match = *i;
                num = std::stoi(match.str());
                break;  
            }
        }
        expected_task[num - 1][num_line] += line + '\n';
        num_line++;
    }

    num = 1;
    num_line = 0;
    while (getline(file2, line)) {
        // if line contains "Task", store it in output_task
        if (regex_match(line, regex("Task [0-9]+.*"))) {
            num_line = 0;
            // Extract numbers from the line
            regex number_regex("[0-9]+");
            auto numbers_begin = sregex_iterator(line.begin(), line.end(), number_regex);
            auto numbers_end = sregex_iterator();

            for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
                std::smatch match = *i;
                num = std::stoi(match.str());
                break;  
            }
        }
        output_task[num - 1][num_line] += line + '\n';
        num_line++;
    }

    // Compare content of tasks
    for (int task = 1; task < num_task; task++) {
        int count_diff = 0;
        if (output_task[task][0] == "") continue;
        doTasks.push_back(task);
        for (int line = 0; line < expected_task[task].size(); line++) {
            if (expected_task[task][line] != output_task[task][line]) {
                count_diff++;
                cout << "Task " << task << " - Line " << line << ":\n";
                cout << "Expected: " << expected_task[task][line];
                cout << "Output: " << output_task[task][line];
            }
        }
        if (count_diff > 0) {
            diffTasks.push_back(task);
        }
    }
    cout << "End of compare" << endl;
    
    cout.rdbuf(stream_buffer_cout);
    float prop = (float)diffTasks.size() / (float)doTasks.size();
    if (prop == 0) {
        std::cout << "All tasks are correct" << std::endl;
    } else {
        std::cout << "Number of tasks to do: " << doTasks.size() << std::endl;
        std::cout << "Number of tasks with differences: " << diffTasks.size() << std::endl;
        std::cout << "Pass: " << 1 - prop << std::endl;
        for (int i = 0; i < diffTasks.size(); i++) {
            std::cout << "Task " << diffTasks[i] << std::endl;
        }
    }
}

void printUsage() {
    std::cout << "Usage: exe_file [OPTIONS] [TASK]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  ?help: show help" << std::endl;
    std::cout << "  demo: run demo" << std::endl;
    std::cout << "  !test: run all test" << std::endl;
    std::cout << "  !test [task]: run specific test" << std::endl;
    std::cout << "  !test [start_task] [end_task] : run test from start_task to end_task" << std::endl;
    std::cout << "This test has 15 tasks" << std::endl;
}

void dataloadertc1(){
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    cout << ds.len() << endl;
    DataLabel<double, int> item = ds.getitem(0);
    cout << item.getData() << endl;
    cout << item.getLabel() << endl;
}
void dataloadertc2(){
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    cout << xt::adapt(ds.get_data_shape()) << endl;
    cout << xt::adapt(ds.get_label_shape()) << endl;
}
void dataloadertc3(){
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false);
    auto it = loader.begin();
    it++;
    cout << xt::adapt((*it).getData().shape()) << endl;
    cout << xt::adapt((*it).getLabel().shape()) << endl;
    cout << xt::adapt((*(++it)).getData().shape()) << endl;
    cout << xt::adapt((*(++it)).getLabel().shape()) << endl;
}
void dataloadertc4(){
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false);
    for(auto it = loader.begin(); it != loader.end(); it++){
        cout << ((*it).getData().shape()[0]) << endl;
        cout << ((*it).getLabel().shape()[0]) << endl;
    }
}
void dataloadertc5(){
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false);
    for(auto batch: loader){
        cout << (xt::adapt(batch.getData().shape())) << endl;
        cout << (xt::adapt(batch.getLabel().shape())) << endl;
    }
}
void dataloadertc6(){
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({100, 3, 3});
    xt::xarray<int> t = xt::ones<int>({100});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false, true);
    for(auto batch: loader){
        cout << batch.getData() << endl;
        cout << batch.getLabel() << endl;
    }
}
void dataloadertc7(){
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false, true);
    for(auto batch: loader){
        cout << (xt::adapt(batch.getData().shape())) << endl;
        cout << (xt::adapt(batch.getLabel().shape())) << endl;
    }
}

void runDemo() {
    int nsamples = 100;
    xt::xarray <double > X = xt::random::randn<double>({ nsamples , 10, 10});
    xt::xarray <double > T = xt::random::randn<double>({ nsamples , 5});
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 30, true , false);
    for(auto batch: loader){
        cout << shape2str(batch.getData().shape()) << endl;
        cout << shape2str(batch.getLabel().shape()) << endl;
    }
    // change output buffer to file lms_output.txt
    std::ofstream out("lms_output.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    cout << "Test 1" << endl;
    dataloadertc1();
    cout << endl << "Test 2" << endl;
    dataloadertc2();
    cout << endl << "Test 3" << endl;
    dataloadertc3();
    cout << endl << "Test 4" << endl;
    dataloadertc4();
    cout << endl << "Test 5" << endl;
    dataloadertc5();
    cout << endl << "Test 6" << endl;
    dataloadertc6();
    cout << endl << "Test 7" << endl;
    dataloadertc7();
    case_data_wo_label_1();
    case_data_wi_label_1();
    case_batch_larger_nsamples();

    // reset output buffer
    std::cout.rdbuf(coutbuf);
}

// basic
void test1() {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5});
    TensorDataset <double , double > ds(X, T);
    cout << "Length of dataset: " << ds.len() << endl;
    cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
    DataLoader <double , double > loader (&ds, 30, true , false);
    for(auto batch: loader){
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
    }
}

void test2() {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5, 8});
    TensorDataset <double , double > ds(X, T);
    cout << "Length of dataset: " << ds.len() << endl;
    cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
    DataLoader <double , double > loader (&ds, 30, false , true);
    for(auto batch: loader){
        cout << shape2str(batch.getData().shape ()) << endl;
        cout << shape2str(batch.getLabel().shape ()) << endl;
    }
}

// test for tensor dataset
void test3() {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5, 8});
    TensorDataset <double , double > ds(X, T);
    cout << "Length of dataset: " << ds.len() << endl;
    cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
    for (int i = 0; i < ds.len(); i++) {
        auto dl = ds.getitem(i);
        cout << "Shape of data: " << shape2str(dl.getData().shape()) << endl;
        cout << "Shape of label: " << shape2str(dl.getLabel().shape()) << endl;
    }
}

// test copy constructor for tensor dataset
void test4() {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5, 8});
    TensorDataset <double , double > ds2(X, T);
    
    cout << "Length of dataset: " << ds2.len() << endl;
    cout << "Data shape: " << shape2str(ds2.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds2.get_label_shape()) << endl;
    for (int i = 0; i < ds2.len(); i++) {
        auto dl = ds2.getitem(i);
        cout << "Shape of data: " << shape2str(dl.getData().shape()) << endl;
        cout << "Shape of label: " << shape2str(dl.getLabel().shape()) << endl;
    }

    TensorDataset <double , double > ds3(ds2);
    cout << "Length of dataset: " << ds3.len() << endl;
    cout << "Data shape: " << shape2str(ds3.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds3.get_label_shape()) << endl;

    TensorDataset <double , double > ds4 = ds3;
    cout << "Length of dataset: " << ds4.len() << endl;
    cout << "Data shape: " << shape2str(ds4.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds4.get_label_shape()) << endl;

    cout << &ds2 << endl;
    cout << &ds3 << endl;
    cout << &ds4 << endl;
}

// assignment operator
void test5()  {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5, 8});
    TensorDataset <double , double > ds2(X, T);
    
    cout << "Length of dataset: " << ds2.len() << endl;
    cout << "Data shape: " << shape2str(ds2.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds2.get_label_shape()) << endl;
    for (int i = 0; i < ds2.len(); i++) {
        auto dl = ds2.getitem(i);
        cout << "Shape of data: " << shape2str(dl.getData().shape()) << endl;
        cout << "Shape of label: " << shape2str(dl.getLabel().shape()) << endl;
    }

    TensorDataset <double , double > ds3(X, T);
    ds3 = ds2;
    cout << "Length of dataset: " << ds3.len() << endl;
    cout << "Data shape: " << shape2str(ds3.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds3.get_label_shape()) << endl;

    TensorDataset <double , double > ds4(X, T);
    ds4 = std::move(ds2);
    cout << "Length of dataset: " << ds4.len() << endl;
    cout << "Data shape: " << shape2str(ds4.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds4.get_label_shape()) << endl;

    cout << &ds2 << endl;
    cout << &ds3 << endl;
    cout << &ds4 << endl;
}

// test for dataloader
void test6() {
    int nsamples = 800;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 6});
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 60, true , true);
    for(auto batch: loader){
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
    }
}

// test for dataloader
void test7() {
    int nsamples = 900;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 1, 2});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 1});
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 200, false , false);
    for(auto batch = loader.begin(); batch != loader.end(); ++batch){
        auto data = *batch;
        cout << shape2str(data.getData().shape ()) << endl;
        cout << shape2str(data.getLabel().shape ()) << endl;
    }
}

void test8() {
    // For empty label
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
    xt::xarray <double > T;
    TensorDataset <double , double > ds(X, T);
    cout << "Length of dataset: " << ds.len() << endl;
    cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
    for (int i = 0; i < ds.len(); i++) {
        auto dl = ds.getitem(i);
        cout << "Shape of data: " << shape2str(dl.getData().shape()) << endl;
        cout << "Shape of label: " << shape2str(dl.getLabel().shape()) << endl;
    }
}

void test9() {
    // For size of data and label not match, data size > label size
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples - 40, 5, 8});
    try {
        TensorDataset <double , double > ds(X, T);
        cout << "Length of dataset: " << ds.len() << endl;
        cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
        cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
        for (int i = 0; i < ds.len(); i++) {
            auto dl = ds.getitem(i);
            cout << "Shape of data: " << shape2str(dl.getData().shape()) << endl;
            cout << "Shape of label: " << shape2str(dl.getLabel().shape()) << endl;
        }
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}

void test10() {
    // For size of data and label not match, label size > data size
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples - 40, 10, 3, 6});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5, 8});
    try {
        TensorDataset <double , double > ds(X, T);
        cout << "Length of dataset: " << ds.len() << endl;
        cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
        cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
        for (int i = 0; i < ds.len(); i++) {
            auto dl = ds.getitem(i);
            cout << "Shape of data: " << shape2str(dl.getData().shape()) << endl;
            cout << "Shape of label: " << shape2str(dl.getLabel().shape()) << endl;
        }
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}

void test11() {
    // dataloader data without label
    try {
        int nsamples = 100;
        xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
        xt::xarray <double > T;
        TensorDataset <double , double > ds(X, T);
        DataLoader <double , double > loader (&ds, 30, true , true);
        for(auto batch: loader){
            cout << shape2str(batch.getData ().shape ()) << endl;
            cout << shape2str(batch.getLabel ().shape ()) << endl;
        }
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}

void test12() {
    // load data with data and label have different size
    try {
        int nsamples = 100;
        xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 3, 6});
        xt::xarray <double > T = xt:: random ::randn <double >({ nsamples - 40, 5, 8});
        TensorDataset <double , double > ds(X, T);
        DataLoader <double , double > loader (&ds, 30, true , true);
        for(auto batch: loader){
            cout << shape2str(batch.getData ().shape ()) << endl;
            cout << shape2str(batch.getLabel ().shape ()) << endl;
        }
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}

void test13() {
    // load data with data and label have different size
    try {
        int nsamples = 100;
        xt::xarray <double > X = xt:: random ::randn <double >({ nsamples - 40, 10, 3, 6});
        xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5, 8});
        TensorDataset <double , double > ds(X, T);
        DataLoader <double , double > loader (&ds, 30, true , true);
        for(auto batch: loader){
            cout << shape2str(batch.getData ().shape ()) << endl;
            cout << shape2str(batch.getLabel ().shape ()) << endl;
        }
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}

// test data without label
void test14() {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 10});
    xt::xarray <double > T;
    TensorDataset <double , double > ds(X, T);
    cout << "Length of dataset: " << ds.len() << endl;
    cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
    DataLoader <double , double > loader (&ds, 30, true , false);
    for(auto batch: loader){
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
    }
}

// test data with label
void test15() {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10, 10});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5});
    TensorDataset <double , double > ds(X, T);
    cout << "Length of dataset: " << ds.len() << endl;
    cout << "Data shape: " << shape2str(ds.get_data_shape()) << endl;
    cout << "Label shape: " << shape2str(ds.get_label_shape()) << endl;
    DataLoader <double , double > loader (&ds, 30, true , false);
    for(auto batch: loader){
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
    }
}

// test batch larger than nsamples
void test16() {
    int nsamples = 10;
    xt::xarray <double > X = xt::arange<double>(nsamples*4).reshape({nsamples, 4});
    xt::xarray <double > T;
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 30, true , false);
    for(auto batch: loader){
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
    }
}

// test with seed in dataloader
void test17() {
    int nsamples = 10;
    xt::xarray <double > X = xt::arange<double>(nsamples*4).reshape({nsamples, 4});
    xt::xarray <double > T;
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 30, true , false, 100);
    for(auto batch: loader){
        auto data = batch.getData();
        auto label = batch.getLabel();
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << data << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
        cout << label << endl;
    }
}

// test with seed in dataloader, seed < 0
void test18() {
    int nsamples = 10;
    xt::xarray <double > X = xt::arange<double>(nsamples*4).reshape({nsamples, 4});
    xt::xarray <double > T;
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 30, true , false, -100);
    for(auto batch: loader){
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
    }
}

// pointer function to store 15 test
void (*testFuncs[])() = {
    test1,
    test2,
    test3,
    test4,
    test5,
    test6,
    test7,
    test8,
    test9,
    test10,
    test11,
    test12,
    test13,
    test14,
    test15,
    test16,
    test17,
    test18
};

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "?help") {
            printUsage();
            return 0;
        }
        else
        if (string(argv[1]) == "demo") {
            runDemo();
            return 0;
        }
        else
        {
            string folder = "TestLog/DataSetAndLoader";
            string path = "DataSetAndLoaderTestLog_NhanOutput.txt";
            string output = "DataSetAndLoaderTestLog_YourOutput.txt";
            fstream file(folder + "/" + output, ios::out);
            if (!file.is_open()) {
                fs::create_directory(folder);
                std::cout << "Create folder " << fs::absolute(folder) << std::endl;
                file.open(folder + "/" + output);
            }
            //change cout to file
            streambuf* stream_buffer_cout = cout.rdbuf();
            cout.rdbuf(file.rdbuf());

            if (string(argv[1]) == "test") {
                if (argc == 2) {
                    for (int i = 0; i < num_task; i++) {
                        std::cout << "Task " << i + 1 << "---------------------------------------------------" <<std::endl;
                        testFuncs[i]();
                    }
                } else
                if (argc == 3) {
                    int task = stoi(argv[2]);
                    if (task >= 1 && task <= num_task) {
                        std::cout << "Task " << task << "---------------------------------------------------" <<std::endl;
                        testFuncs[task - 1]();
                    } else {
                        std::cout << "Task not found" << std::endl;
                    }
                } else
                if (argc == 4) {
                    int start = stoi(argv[2]);
                    int end = stoi(argv[3]);
                    if (start >= 1 && start <= num_task && end >= 1 && end <= num_task && start <= end) {
                        for (int i = start - 1; i < end; i++) {
                            std::cout << "Task " << i + 1 << "---------------------------------------------------" <<std::endl;
                            testFuncs[i]();
                        }
                    } else {
                        std::cout << "Task not found or you enter error" << std::endl;
                    }
                }
            }
            // Restore cout
            cout.rdbuf(stream_buffer_cout);

            compareFile(folder + "/" + path, folder + "/" + output);
        }
    }
    return 0;
}