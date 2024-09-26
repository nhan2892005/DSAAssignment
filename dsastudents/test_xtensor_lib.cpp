#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
using namespace std;

#include "ann/funtions.h"
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
#include "ann/dataloader.h"

using namespace std;
namespace fs = std::filesystem;
int num_task = 16;

vector<vector<string>> expected_task (num_task, vector<string>(50, ""));
vector<vector<string>> output_task (num_task, vector<string>(50, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/DLinkedList/DLinkedListTestLog_Compare.txt";
    fstream file(log_file);
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

void test_outer_stack1() {
    xt::xarray<double> X = {1, 2, 3};
    xt::xarray<double> Y = {4, 5};
    xt::xarray<double> res = outer_stack(X, Y);
    auto sh = res.shape();
    std::cout << shape2str(sh) << std::endl;
    for (int i = 0; i < sh[0]; ++i) {
        for (int j = 0; j < sh[1]; ++j) {
            std::cout << res(i,j) << " ";
        }
        std::cout << std::endl;
    }
}

void test_outer_stack2() {
    xt::xarray<double> X = {{{1, 2, 3}, {4, 5, 6}},{{7, 8, 9}, {10, 11, 12}}};
    xt::xarray<double> Y = {{1, 2}, {3, 4}, {5, 6}};
    xt::xarray<double> res = outer_stack(X, Y);
    auto sh = res.shape();
    std::cout << shape2str(sh) << std::endl;

    cout << res << endl;
}

void test_matmul1() {
    xt::xarray<double> X = {{1, 2, 3}, {4, 5, 6}};
    xt::xarray<double> Y = {{1, 2}, {3, 4}, {5, 6}};
    xt::xarray<double> res = matmul_on_stack(X, Y);
    auto sh = res.shape();
    std::cout << shape2str(sh) << std::endl;

    cout << res << endl;
}

void test_matmul2() {
    try {
        xt::xarray<double> X = {{1, 2, 3}, {4, 5, 6}};
        xt::xarray<double> Y = {{1, 2}, {3, 4}};
        xt::xarray<double> res = matmul_on_stack(X, Y);
        auto sh = res.shape();
        std::cout << shape2str(sh) << std::endl;

        cout << res << endl;
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }
}

void test_diag_stack1() {
    xt::xarray<double> X = {{-5, 0, -6}, {2, 0 ,2}, {3, 0, 4}};
    xt::xarray<double> res = diag_stack(X);
    auto sh = res.shape();
    std::cout << shape2str(sh) << std::endl;

    cout << res << endl;
}
/*
xt::xarray<double> outer_stack(xt::xarray<double> X, xt::xarray<double>  Y){
    auto shape_X = X.shape();
    auto shape_Y = Y.shape();

    int dim_X = shape_X.size();
    int dim_Y = shape_Y.size();

    int dim = dim_X + dim_Y;
    int size = shape_X[0] * shape_Y[0];
    xt::svector<unsigned long> shape(dim, 0);
    for(int idx=0; idx < dim_X; idx++) shape[idx] = shape_X[idx];
    for(int idx=0; idx < dim_Y; idx++) shape[dim_X + idx] = shape_Y[idx];

    xt::xarray<double> Z = xt::zeros<double>(shape);
    for(int idx=0; idx < size; idx++){
        int idx_X = idx / shape_Y[0];
        int idx_Y = idx % shape_Y[0];
        Z(idx_X, idx_Y) = X(idx_X) * Y(idx_Y);
    }
    return Z;
}

xt::xarray<double> diag_stack(xt::xarray<double> X) {
    auto shape_X = X.shape();
    int dim_X = shape_X.size();
    int dim = dim_X + 1;
    int size = shape_X[0];
    xt::svector<unsigned long> shape(dim, 0);
    for(int idx=0; idx < dim_X; idx++) shape[idx] = shape_X[idx];
    shape[dim_X] = shape_X[0];

    xt::xarray<double> Z = xt::zeros<double>(shape);
    for(int idx=0; idx < size; idx++){
        Z(idx, idx) = X(idx);
    }
    return Z;
}

xt::xarray<double> matmul_on_stack(xt::xarray<double> X, xt::xarray<double> Y){
    auto shape_X = X.shape();
    auto shape_Y = Y.shape();

    if(shape_X[1] != shape_Y[0]){
        throw std::invalid_argument("Invalid shape");
    }

    int dim_X = shape_X.size();
    int dim_Y = shape_Y.size();

    int dim = dim_X + dim_Y - 2;
    int size = shape_X[0] * shape_Y[1];
    xt::svector<unsigned long> shape(dim, 0);
    for(int idx=0; idx < dim_X - 1; idx++) shape[idx] = shape_X[idx];
    for(int idx=0; idx < dim_Y - 1; idx++) shape[dim_X - 1 + idx] = shape_Y[idx];

    xt::xarray<double> Z = xt::zeros<double>(shape);
    for(int idx=0; idx < size; idx++){
        int idx_X = idx / shape_Y[1];
        int idx_Y = idx % shape_Y[1];
        for(int jdx=0; jdx < shape_X[1]; jdx++){
            Z(idx_X, idx_Y) += X(idx_X, jdx) * Y(jdx, idx_Y);
        }
    }
    return Z;
}
ulong_array confusion_matrix(ulong_array y_true, ulong_array y_pred){
    ulong_array confusion = xt::zeros<ulong>({y_true.size(), y_true.size()});
    for(int idx=0; idx < y_true.size(); idx++){
        confusion(y_true[idx], y_pred[idx]) += 1;
    }
    return confusion;
}

xt::xarray<ulong> class_count(xt::xarray<ulong> confusion) {
    return xt::sum(confusion, {1});
}

double_array calc_metrics(ulong_array y_true, ulong_array y_pred) {
    ulong_array confusion = confusion_matrix(y_true, y_pred);
    auto counts = class_count(confusion);

    double accuracy = xt::linalg::trace(confusion)() / xt::sum(counts)();

    xt::xarray<double> precision = xt::zeros<double>({confusion.shape()[0]});
    xt::xarray<double> recall = xt::zeros<double>({confusion.shape()[0]});

    for (size_t i = 0; i < confusion.shape()[0]; ++i) {
        precision(i) = confusion(i, i) / xt::sum(xt::row(confusion, i))();
        recall(i) = confusion(i, i) / xt::sum(xt::col(confusion, i))();
    }

    xt::xarray<double> f1_score = 2 * (precision * recall) / (precision + recall);

    double_array metrics = {accuracy, xt::mean(precision)(), xt::mean(recall)(), xt::mean(f1_score)()};
    return metrics;
}
*/
void runDemo() {
    xt::xarray<double> X = {{1, 2, 3}, {4, 5, 6}};
    xt::xarray<double> Y = {{1, 2}, {3, 4}, {5, 6}};
    xt::xarray<double> Z = matmul_on_stack(X, Y);
    std::cout << "Shape of X: " << shape2str(X.shape()) << std::endl;
    std::cout << "X: " << X << std::endl;
    std::cout << "Shape of Y: " << shape2str(Y.shape()) << std::endl;
    std::cout << "Y: " << Y << std::endl;
    std::cout << "Shape of Z: " << shape2str(Z.shape()) << std::endl;
    std::cout << "Z: " << Z << std::endl;

    xt::xarray<double> A = {{1, 2, 3}, {4, 5, 6}};
    xt::xarray<double> B = {{1, 2, 3}, {4, 5, 6}};
    try {
        xt::xarray<double> C = matmul_on_stack(A, B);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }

    // test diag_stack
    xt::xarray<double> D = {1, 2, 3};
    xt::xarray<double> E = diag_stack(D);
    std::cout << "Shape of D: " << shape2str(D.shape()) << std::endl;
    std::cout << "D: " << D << std::endl;
    std::cout << "Shape of E: " << shape2str(E.shape()) << std::endl;
    std::cout << "E: " << E << std::endl;

    // test outer_stack
    xt::xarray<double> F = {1, 2, 3};
    xt::xarray<double> G = {4, 5};
    xt::xarray<double> H = outer_stack(F, G);
    std::cout << "Shape of F: " << shape2str(F.shape()) << std::endl;
    std::cout << "F: " << F << std::endl;
    std::cout << "Shape of G: " << shape2str(G.shape()) << std::endl;
    std::cout << "G: " << G << std::endl;
    std::cout << "Shape of H: " << shape2str(H.shape()) << std::endl;
    std::cout << "H: " << H << std::endl;

    //test confusion_matrix
    ulong_array y_true = {0, 1, 2, 0, 1, 3};
    ulong_array y_pred = {0, 2, 1, 0, 0, 1};
    ulong_array confusion = confusion_matrix(y_true, y_pred);
    std::cout << "Confusion matrix: " << std::endl;
    std::cout << confusion << std::endl;

    //test class_count
    xt::xarray<ulong> class_count_res = class_count(confusion);
    std::cout << "Class count: " << std::endl;
    std::cout << class_count_res << std::endl;

    //test calc_metrics
    double_array metrics = calc_metrics(y_true, y_pred);
    std::cout << "Accuracy: " << metrics[0] << std::endl;
    std::cout << "Precision: " << metrics[1] << std::endl;
    std::cout << "Recall: " << metrics[2] << std::endl;
    std::cout << "F1 score: " << metrics[3] << std::endl;
}

void (*testFuncs[])() = {
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
            string folder = "TestLog/DLinkedList";
            string path = "DLinkedListTestLog_NhanOutput.txt";
            string output = "DLinkedListTestLog_YourOutput.txt";
            fstream file(folder + "/" + output);
            if (!file.is_open()) {
                fs::create_directory(folder);
                std::cout << "Create folder " << fs::absolute(folder) << std::endl;
                file.open(folder + "/" + output);
            }
            //change cout to file
            streambuf* stream_buffer_cout = cout.rdbuf();
            cout.rdbuf(file.rdbuf());

            if (string(argv[1]) == "!test") {
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

            //compareFile(folder + "/" + path, folder + "/" + output);
        }
    }
    return 0;
}