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
    std::cout << "  !demo: run demo" << std::endl;
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

void runDemo() {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5});
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 30, true , false);
    for(auto batch: loader){
        cout << shape2str(batch.getData ().shape ()) << endl;
        cout << shape2str(batch.getLabel ().shape ()) << endl;
    }
}

// pointer function to store 15 test
void (*testFuncs[])() = {
};

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "?help") {
            printUsage();
            return 0;
        }
        else
        if (string(argv[1]) == "!demo") {
            test_diag_stack1();
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