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
#include "loss/CrossEntropy.h"
#include "metrics/ClassMetrics.h"

using namespace std;
namespace fs = std::filesystem;
int num_task = 4;

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

void test1() {
    // test cross entropy
    xt::xarray<double> Ypred = {{0.3, 0.3, 0.4}, {0.1, 0.6, 0.3}, {0.2, 0.2, 0.6}, {0.1, 0.1, 0.8}
                                ,{0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}};
    xt::xarray<unsigned long> Ygt = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 1}
                                    ,{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}};

    CrossEntropy loss;
    double loss_value = loss.forward(Ypred, Ygt);
    std::cout << "Loss value: " << loss_value << std::endl;
}

void test2() {
    // test cross entropy
    xt::xarray<double> Ypred = {{0.3, 0.3, 0.4}, {0.1, 0.6, 0.3}, {0.2, 0.2, 0.6}, {0.1, 0.1, 0.8}
                                ,{0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}};
    xt::xarray<unsigned long> Ygt = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 1}
                                    ,{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}};

    CrossEntropy loss;
    double loss_value = loss.forward(Ypred, Ygt);
    std::cout << "Loss value: " << loss_value << std::endl;
    
    // Backward
    xt::xarray<double> dY = loss.backward();
    std::cout << "dY: " << dY << std::endl;
}

void test3() {
    // test metrics
    ClassMetrics metrics(3);
    metrics.reset_metrics();

    xt::xarray<double> y_true = {{0, 1, 0}, {1, 0, 0}, {0, 0, 1}, {0, 0, 1}
                                ,{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}};
    xt::xarray<double> y_pred = {{0.3, 0.3, 0.4}, {0.1, 0.6, 0.3}, {0.2, 0.2, 0.6}, {0.1, 0.1, 0.8}
                                ,{0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}};
    xt::xarray<double> metrics_value = metrics.calculate_metrics(y_true, y_pred);
    std::cout << "Metrics value: " << metrics_value << std::endl;
}

void test4() {
    // test IMetrics
    IMetrics* metrics = new ClassMetrics(3);
    metrics->reset_metrics();

    xt::xarray<double> y_true = {{0, 1, 0}, {1, 0, 0}, {0, 0, 1}, {0, 0, 1}
                                ,{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}};
    xt::xarray<double> y_pred = {{0.3, 0.3, 0.4}, {0.1, 0.6, 0.3}, {0.2, 0.2, 0.6}, {0.1, 0.1, 0.8}
                                ,{0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}, {0.1, 0.1, 0.8}};
    metrics->accumulate(y_true, y_pred);
    double accum = metrics->get_metrics()(0);
    std::cout << "Accumulated metrics value: " << accum << std::endl;

    std::cout << "Get counts: " << metrics->get_counts() << std::endl;

    std::cout << "Evaluate: " << metrics->evaluate(y_pred, y_true) << std::endl;

    std::cout << "Get metrics: " << metrics->get_metrics() << std::endl;

    delete metrics;
}

void runDemo(){
    // test cross entropy
    xt::xarray<double> Ypred = {{0.1, 0.9}, {0.9, 0.1}, {0.8, 0.2}};
    xt::xarray<unsigned long> Ygt = {1, 0, 0};
    double ce = cross_entropy(Ypred, Ygt, true);
    std::cout << "Cross entropy: " << ce << std::endl;
    
    std::cout << "Test with CrossEntropy class" << std::endl;
    CrossEntropy loss;
    xt::xarray<double> X = {{0.1, 0.9}, {0.9, 0.1}, {0.8, 0.2}};
    xt::xarray<double> t = {{0, 1}, {1, 0}, {1, 0}};
    double loss_value = loss.forward(X, t);
    std::cout << "Loss value: " << loss_value << std::endl;
    xt::xarray<double> dY = loss.backward();
    std::cout << "dY: " << dY << std::endl;

    std::cout << "Test with ClassMetrics class" << std::endl;
    ClassMetrics metrics(2);
    metrics.reset_metrics();
    xt::xarray<double> y_true = {{0, 1}, {1, 0}, {1, 0}};
    xt::xarray<double> y_pred = {{0.1, 0.9}, {0.9, 0.1}, {0.8, 0.2}};
    xt::xarray<double> metrics_value = metrics.calculate_metrics(y_true, y_pred);
    std::cout << "Metrics value: " << metrics_value << std::endl;

    std::cout << "Test with Class IMetrics class" << std::endl;
    IMetrics* metrics2 = new ClassMetrics(2);
    metrics2->reset_metrics();
    xt::xarray<double> y_true2 = {{0, 1}, {1, 0}, {1, 0}};
    xt::xarray<double> y_pred2 = {{0.1, 0.9}, {0.9, 0.1}, {0.8, 0.2}};
    double metrics_value2 = metrics2->evaluate(y_pred2, y_true2);
    std::cout << "Metrics value: " << metrics_value2 << std::endl;
    std::cout << "Get counts: " << metrics2->get_counts() << std::endl;
    delete metrics2;
}

// pointer function to store 15 test
void (*testFuncs[])() = {
    test1,
    test2,
    test3,
    test4
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
            string folder = "TestLog/LossAndMetrics";
            string path = "LossAndMetricsTestLog_NhanOutput.txt";
            string output = "LossAndMetricsTestLog_YourOutput.txt";
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

            //compareFile(folder + "/" + path, folder + "/" + output);
        }
    }
    return 0;
}