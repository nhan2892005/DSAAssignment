/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-06
    * Version 1.0
    * Final Test for Layer
*/
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
using namespace std;

#include "ann/layer/FCLayer.h"
#include "ann/layer/ReLU.h"
#include "ann/layer/Sigmoid.h"
#include "ann/layer/Softmax.h"
#include "ann/layer/Tanh.h"
#include "ann/layer/ILayer.h"
#include "tensor/xtensor_lib.h"
#include "list/DLinkedList.h"

using namespace std;
namespace fs = std::filesystem;
int num_task = 21;


vector<vector<string>> expected_task (num_task, vector<string>(1000, ""));
vector<vector<string>> output_task (num_task, vector<string>(1000, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/Layer/LayerTestLog_Compare.txt";
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
    for (int task = 0; task < num_task; task++) {
        int count_diff = 0;
        if (output_task[task][0] == "") continue;
        doTasks.push_back(task);
        for (int line = 0; line < expected_task[task].size(); line++) {
            if (expected_task[task][line] != output_task[task][line]) {
                count_diff++;
                cout << "Task " << task + 1 << " - Line " << line << ":\n";
                cout << "Expected: " << expected_task[task][line];
                cout << "Output: " << output_task[task][line];
            }
        }
        if (count_diff > 0) {
            diffTasks.push_back(task);
        }
    }
    
    cout.rdbuf(stream_buffer_cout);
    float prop = (float)diffTasks.size() / (float)doTasks.size();
    if (prop == 0) {
        std::cout << "All tasks are correct" << std::endl;
    } else {
        std::cout << "Number of tasks to do: " << doTasks.size() << std::endl;
        std::cout << "Number of tasks with differences: " << diffTasks.size() << std::endl;
        std::cout << "Pass: " << 1 - prop << std::endl;
        for (int i = 0; i < diffTasks.size(); i++) {
            std::cout << "Task " << diffTasks[i] + 1 << std::endl;
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

void test1() {
    cout << "Test Fully Connected Layer Forward with number" << endl;
    xt::xarray<int> X = {1};
    FCLayer in_layer(1, 1, false);
    auto Y = in_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test2() {
    FCLayer in_layer(2, 3, false);
    xt::xarray<double> X = {{1, 2}, {3, 4}};
    auto Y = in_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "Length of X: " << X.size() << endl;
    cout << "Shape of X: " << shape2str(X.shape()) << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
    cout << "Length of Y: " << Y.size() << endl;
    cout << "Shape of Y: " << shape2str(Y.shape()) << endl;

    // backward
    auto dX = in_layer.backward(Y);
    cout << "dY: " << Y << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << dX << endl;
    cout << "Length of dX: " << dX.size() << endl;
    cout << "Shape of dX: " << shape2str(dX.shape()) << endl;
}

void test3() {
    cout << "Test Fully Connected Layer Forward with 1x2 matrix" << endl;
    std::vector<int> data = {1, 2};
    std::vector<std::size_t> shape = {2, 1}; 
    
    xt::xarray<int> X = xt::adapt(data, shape);
    FCLayer in_layer(1, 2, false);
    auto Y = in_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test4() {
    cout << "Test Fully Connected Layer Backward with 1x2 matrix" << endl;
    FCLayer in_layer(1, 2, false);
    // Khởi tạo xarray bằng xt::adapt
    std::vector<int> data = {1, 2};
    std::vector<std::size_t> shape = {2, 1};
    
    xt::xarray<int> X = xt::adapt(data, shape);
    auto Y = in_layer.forward(X);
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
    auto DX = in_layer.backward(Y);
    cout << "dY: " << Y << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test5() {
    cout << "Test Fully Connected Layer Forward with 2x2" << endl;
    xt::xarray<int> X = {{1, 1}, {2, 2}};
    FCLayer in_layer(2, 1, false);
    auto Y = in_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test6() {
    cout << "Test Fully Connected Layer Backward with 2x1 matrix" << endl;
    FCLayer in_layer(2, 1, false);
    xt::xarray<int> X = {{1, 2}, {3, 4}, {5, 6}};
    auto DY = in_layer.forward(X);
    auto DX = in_layer.backward(DY);
    cout << "dY: " << DY << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test7() {
    cout << "Test ReLU Layer Forward with 2x2 matrix" << endl;
    xt::xarray<int> X = {{1, 2}, {3, 4}};
    ReLU relu_layer;
    auto Y = relu_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test8() {
    cout << "Test ReLU Layer Backward with 2x2 matrix" << endl;
    xt::xarray<int> DY = {{1, 2}, {3, 4}};
    ReLU relu_layer;
    auto DX = relu_layer.forward(DY);
    DX = relu_layer.backward(DY);
    cout << "dY: " << DY << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test9() {
    cout << "Test ReLU Layer Forward" << endl;
    xt::xarray<int> X = {{-1, 2}, {3, 4}, {5, -6}, {7, 8}, {-9, -10}};
    ReLU relu_layer;
    auto Y = relu_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test10() {
    cout << "Test ReLU Layer Backward" << endl;
    xt::xarray<int> DY = {{1, -2}, {3, 4}, {5, -6}, {-7, -8}, {-9, 10}};
    ReLU relu_layer;
    auto DX = relu_layer.forward(DY);
    DX = relu_layer.backward(DY);
    cout << "dY: " << DY << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test11() {
    cout << "Test Sigmoid Layer Forward" << endl;
    xt::xarray<int> X = {{-1, 2}, {3, 4}, {5, -6}, {7, 8}, {-9, -10}};
    Sigmoid sigmoid_layer;
    auto Y = sigmoid_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test12() {
    cout << "Test Sigmoid Layer Backward" << endl;
    xt::xarray<int> DY = {{1, -2}, {3, 4}, {5, -6}, {-7, -8}, {-9, 10}};
    Sigmoid sigmoid_layer;
    auto DX = sigmoid_layer.forward(DY);
    DX = sigmoid_layer.backward(DY);
    cout << "dY: " << DY << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test13() {
    cout << "Test Softmax Layer Forward" << endl;
    xt::xarray<int> X = {5, 5, 2};
    Softmax softmax_layer;
    auto Y = softmax_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test14() {
    cout << "Test Softmax Layer Backward" << endl;
    xt::xarray<int> X = {8, 8, -6};
    Softmax softmax_layer;
    auto DY = softmax_layer.forward(X);
    auto DX = softmax_layer.backward(DY);
    cout << "dY: " << DY << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test15() {
    cout << "Test Softmax Layer Forward" << endl;
    xt::xarray<int> X = {{3, 2, 3}, {9, 5, 6}, {8, 8, 10}, {9, 3, 10}, {-2, 10, 5}};
    Softmax softmax_layer;
    auto Y = softmax_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test16() {
    cout << "Test Softmax Layer Backward" << endl;
    xt::xarray<int> one_hot_X = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 0}, {0, 0, 1}};
    Softmax softmax_layer;
    auto DY = softmax_layer.forward(one_hot_X);

    auto DX = softmax_layer.backward(DY);
    cout << "dY: " << DY << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test17() {
    cout << "Test Tanh Layer Forward" << endl;
    xt::xarray<int> X = {{3, 2, 3}, {9, 5, 6}, {8, 8, 10}, {9, 3, 10}, {-2, 10, 5}};
    Tanh tanh_layer;
    auto Y = tanh_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test18() {
    cout << "Test Tanh Layer Backward" << endl;
    xt::xarray<int> DY = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}};
    Tanh tanh_layer;
    auto DX = tanh_layer.forward(DY);
    cout << "After forward: " << endl;
    cout << "Y: " << DX << endl;
    
    cout << "dY: " << DX << endl;
    DX = tanh_layer.backward(DX);
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test19() {
    cout << "Test Tanh Layer Forward" << endl;
    xt::xarray<int> X = {{3, -2, 3}, {9, 5, -6}, {8, 8, 10}, {-9, 3, 10}, {-2, 10, 5}};
    Tanh tanh_layer;
    auto Y = tanh_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
}

void test20() {
    cout << "Test Tanh Layer Backward" << endl;
    xt::xarray<int> DY = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}};
    Tanh tanh_layer;
    auto DX = tanh_layer.forward(DY);
    cout << "After forward: " << endl;
    cout << "Y: " << DX << endl;
    cout << "dY: " << DX << endl;
    DX = tanh_layer.backward(DY);
    cout << "After backward: " << endl;
    cout << "dX: " << DX << endl;
}

void test_all_layer() {
    FCLayer in_layer(2, 10, false);
    FCLayer hidden_layer_1(10, 6, false);
    FCLayer hidden_layer_2(6, 4, false);
    FCLayer out_layer(4, 3, false);
    ReLU active_1("relu");
    Sigmoid active_2("sigmoid");
    Tanh active_3("tanh");
    Softmax active_4(1, "softmax");

    // Forward
    xt::xarray<double> X = {{1, 2}, {3, 4}, {5, 6}};
    cout << "1." << endl;
    cout << "Z0 = X = A0: " << X << endl;
    cout << "Shape of Z0: " << shape2str(X.shape()) << endl;
    auto Y1 = in_layer.forward(X);
    cout << "After forward: " << endl;
    cout << "Z1: " << Y1 << endl;
    cout << "Shape of Z1: " << shape2str(Y1.shape()) << endl;
    cout << "2." << endl;
    auto Y2 = active_1.forward(Y1);
    cout << "After activation: " << endl;
    cout << "A1: " << Y2 << endl;
    cout << "Shape of A1: " << shape2str(Y2.shape()) << endl;
    cout << "3." << endl;
    auto Y3 = hidden_layer_1.forward(Y2);
    cout << "After forward: " << endl;
    cout << "Z2: " << Y3 << endl;
    cout << "Shape of Z2: " << shape2str(Y3.shape()) << endl;
    auto Y4 = active_2.forward(Y3);
    cout << "After activation: " << endl;
    cout << "A2: " << Y4 << endl;
    cout << "Shape of A2: " << shape2str(Y4.shape()) << endl;
    cout << "4." << endl;
    auto Y5 = hidden_layer_2.forward(Y4);
    cout << "After forward: " << endl;
    cout << "Z3: " << Y5 << endl;
    cout << "Shape of Z3: " << shape2str(Y5.shape()) << endl;
    auto Y6 = active_3.forward(Y5);
    cout << "After activation: " << endl;
    cout << "A3: " << Y6 << endl;
    cout << "Shape of A3: " << shape2str(Y6.shape()) << endl;
    cout << "5." << endl;
    auto Y7 = out_layer.forward(Y6);
    cout << "After forward: " << endl;
    cout << "Z4: " << Y7 << endl;
    cout << "Shape of Z4: " << shape2str(Y7.shape()) << endl;
    auto Y8 = active_4.forward(Y7);
    cout << "After activation: " << endl;
    cout << "A4: " << Y8 << endl;
    cout << "Shape of A4: " << shape2str(Y8.shape()) << endl;

    // Backward
    xt::xarray<double> dY = xt::ones<double>({3, 3});
    auto dY7 = active_4.backward(dY);
    auto dY6 = out_layer.backward(dY7);
    auto dY5 = active_3.backward(dY6);
    auto dY4 = hidden_layer_2.backward(dY5);
    auto dY3 = active_2.backward(dY4);
    auto dY2 = hidden_layer_1.backward(dY3);
    auto dY1 = active_1.backward(dY2);
    auto dX = in_layer.backward(dY1);

    // Print log
    cout << "dY8: " << dY << endl;
    cout << "After backward: " << endl;
    cout << "dY7: " << dY7 << endl;
    cout << "dY6: " << dY6 << endl;
    cout << "dY5: " << dY5 << endl;
    cout << "dY4: " << dY4 << endl;
    cout << "dY3: " << dY3 << endl;
    cout << "dY2: " << dY2 << endl;
    cout << "dY1: " << dY1 << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << dX << endl;
}

void test_all_layer_with_one_data() {
    FCLayer in_layer(2, 10, false);
    FCLayer hidden_layer_1(10, 6, false);
    FCLayer hidden_layer_2(6, 4, false);
    FCLayer out_layer(4, 3, false);
    ReLU active_1("relu");
    Sigmoid active_2("sigmoid");
    Tanh active_3("tanh");
    Softmax active_4(1, "softmax");

    // Forward
    xt::xarray<double> X = {1,2};
    cout << "1." << endl;
    cout << "Z0 = X = A0: " << X << endl;
    cout << "Shape of Z0: " << shape2str(X.shape()) << endl;
    auto Y1 = in_layer.forward(X);
    cout << "After forward: " << endl;
    cout << "Z1: " << Y1 << endl;
    cout << "Shape of Z1: " << shape2str(Y1.shape()) << endl;
    cout << "2." << endl;
    auto Y2 = active_1.forward(Y1);
    cout << "After activation: " << endl;
    cout << "A1: " << Y2 << endl;
    cout << "Shape of A1: " << shape2str(Y2.shape()) << endl;
    cout << "3." << endl;
    auto Y3 = hidden_layer_1.forward(Y2);
    cout << "After forward: " << endl;
    cout << "Z2: " << Y3 << endl;
    cout << "Shape of Z2: " << shape2str(Y3.shape()) << endl;
    auto Y4 = active_2.forward(Y3);
    cout << "After activation: " << endl;
    cout << "A2: " << Y4 << endl;
    cout << "Shape of A2: " << shape2str(Y4.shape()) << endl;
    cout << "4." << endl;
    auto Y5 = hidden_layer_2.forward(Y4);
    cout << "After forward: " << endl;
    cout << "Z3: " << Y5 << endl;
    cout << "Shape of Z3: " << shape2str(Y5.shape()) << endl;
    auto Y6 = active_3.forward(Y5);
    cout << "After activation: " << endl;
    cout << "A3: " << Y6 << endl;
    cout << "Shape of A3: " << shape2str(Y6.shape()) << endl;
    cout << "5." << endl;
    auto Y7 = out_layer.forward(Y6);
    cout << "After forward: " << endl;
    cout << "Z4: " << Y7 << endl;
    cout << "Shape of Z4: " << shape2str(Y7.shape()) << endl;
    auto Y8 = active_4.forward(Y7);
    cout << "After activation: " << endl;
    cout << "A4: " << Y8 << endl;
    cout << "Shape of A4: " << shape2str(Y8.shape()) << endl;

    // Backward
    xt::xarray<double> dY = xt::ones<double>({3});
    auto dY7 = active_4.backward(dY);
    auto dY6 = out_layer.backward(dY7);
    auto dY5 = active_3.backward(dY6);
    auto dY4 = hidden_layer_2.backward(dY5);
    auto dY3 = active_2.backward(dY4);
    auto dY2 = hidden_layer_1.backward(dY3);
    auto dY1 = active_1.backward(dY2);
    auto dX = in_layer.backward(dY1);

    // Print log
    cout << "dY8: " << dY << endl;
    cout << "After backward: " << endl;
    cout << "dY7: " << dY7 << endl;
    cout << "dY6: " << dY6 << endl;
    cout << "dY5: " << dY5 << endl;
    cout << "dY4: " << dY4 << endl;
    cout << "dY3: " << dY3 << endl;
    cout << "dY2: " << dY2 << endl;
    cout << "dY1: " << dY1 << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << dX << endl;
}

void runDemo() {
    FCLayer in_layer(2, 3, false);
    xt::xarray<double> X = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}};
    auto Y = in_layer.forward(X);
    cout << "X: " << X << endl;
    cout << "Length of X: " << X.size() << endl;
    cout << "Shape of X: " << shape2str(X.shape()) << endl;
    cout << "After forward: " << endl;
    cout << "Y: " << Y << endl;
    cout << "Length of Y: " << Y.size() << endl;
    cout << "Shape of Y: " << shape2str(Y.shape()) << endl;

    // backward
    auto dY = xt::ones<double>({6, 3});
    auto dX = in_layer.backward(dY);
    cout << "dY: " << dY << endl;
    cout << "After backward: " << endl;
    cout << "dX: " << dX << endl;
    cout << "Length of dX: " << dX.size() << endl;
    cout << "Shape of dX: " << shape2str(dX.shape()) << endl;
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
    test18,
    test19,
    test20,
    test_all_layer_with_one_data
};

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "?help") {
            printUsage();
            return 0;
        }
        else
        if (string(argv[1]) == "demo") {
            string file_name = "DemoLog.txt";
            fstream file_demo(file_name, ios::out);
            if (!file_demo.is_open()) {
                std::cout << "Cannot open file" << std::endl;
                return 0;
            } 
            streambuf* stream_buffer_cout = cout.rdbuf();
            cout.rdbuf(file_demo.rdbuf());

            runDemo();

            cout.rdbuf(stream_buffer_cout);
            return 0;
        }
        else 
        {
            string folder = "TestLog/Layer";
            string path = "LayerTestLog_NhanOutput.txt";
            string output = "LayerTestLog_YourOutput.txt";
            fstream file(folder + "/" + output, ios::out);
            if (!file.is_open()) {
                fs::create_directory(folder);
                std::cout << "Create folder " << fs::absolute(folder) << std::endl;
                file.open(folder + "/" + output, ios::out);
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