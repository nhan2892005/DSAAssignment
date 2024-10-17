#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include "heap/Heap.h"
#include "util/Point.h"
#include "heap/HeapDemo.h"
#include <regex>
#include <random>

using namespace std;
namespace fs = std::filesystem;
int num_task = 6;

vector<vector<string>> expected_task (num_task, vector<string>(1000, ""));
vector<vector<string>> output_task (num_task, vector<string>(1000, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/Heap/HeapTestLog_Compare.txt";
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
        doTasks.push_back(task + 1);
        for (int line = 0; line < expected_task[task].size(); line++) {
            if (expected_task[task][line] != output_task[task][line]) {
                count_diff++;
                cout << "Task " << task << " - Line " << line << ":\n";
                cout << "Expected: " << expected_task[task][line];
                cout << "Output: " << output_task[task][line];
            }
        }
        if (count_diff > 0) {
            diffTasks.push_back(task + 1);
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

enum CommandType {
    PUT, GET, REMOVE, CONTAINS, SIZE, CLEAR, EMPTY, KEYS, VALUES, CLASHES, TO_STRING
};

void printUsage() {
    std::cout << "Usage: exe_file [OPTIONS] [TASK]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  ?help: show help" << std::endl;
    std::cout << "  demo: run demo" << std::endl;
    std::cout << "  test: run all test" << std::endl;
    std::cout << "  test [task]: run specific test" << std::endl;
    std::cout << "  test [start_task] [end_task] : run test from start_task to end_task" << std::endl;
    std::cout << "This test has 15 tasks" << std::endl;
}
/*
    virtual void push(T item)=0;
    virtual T pop()=0;
    virtual const T peek()=0;
    virtual void remove(T item, void (*removeItemData)(T)=0)=0;
    virtual bool contains(T item)=0;
    virtual int size()=0;
    virtual void heapify(T array[], int size)=0; //build heap from array having size items
    virtual void clear()=0;
    virtual bool empty()=0;
    virtual string toString(string (*item2str)(T&) =0)=0;
*/
void test1() {
    Heap<int> heap;
    
    int size = 100;
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = size - i;
    }

    // Create heap from array
    heap.heapify(array, size);
    cout << "Heapify: " << heap.toString() << endl;

    // Check copy constructor
    Heap<int> heap2(heap);
    cout << "Copy constructor: " << heap2.toString() << endl;

    // Check operator=
    Heap<int> heap3;
    heap3 = heap;
    cout << "Operator=: " << heap3.toString() << endl;

    delete[] array;
}

void test2() {
    // random create array char in range 'a' to 'z'
    int size = 10;
    char* array = new char[size];
    for (int i = 0; i < size; i++) {
        array[i] = (((int)('a' + 'z') * (3*i*i - 7*i + 1)) % 26) + 'a';
    }

    // Create heap from array
    Heap<char> heap;
    for (int i = 0; i < size; i++) {
        heap.clear();
        heap.heapify(array, i + 1);
        cout << "Heapify: " << heap.toString() << endl;
    }

    // Check push
    heap.push('z');
    cout << "Push z: " << heap.toString() << endl;
    heap.push('a');
    cout << "Push a: " << heap.toString() << endl;
    heap.push('m');
    cout << "Push m: " << heap.toString() << endl;

    // Check pop
    heap.pop();
    cout << "Pop: " << heap.toString() << endl;
    heap.pop();
    cout << "Pop: " << heap.toString() << endl;
    heap.pop();
    cout << "Pop: " << heap.toString() << endl;

    // Size
    cout << "Size: " << heap.size() << endl;
    // Empty
    cout << "Empty: " << heap.empty() << endl;
    // Clear
    heap.clear();
    cout << "Clear: " << heap.toString() << endl;
    cout << "Empty: " << heap.empty() << endl;

    delete[] array;
}

void test3() {
    // random create array int in range 0 to 100
    int size = 100;
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = (13*i*i - 27*i + 1) % 100;
    }

    // Create heap from array
    Heap<int> heap;
    heap.heapify(array, size);
    cout << "Heapify: " << heap.toString() << endl;
    cout << "Size: " << heap.size() << endl;

    // Check contains
    cout << "Contains 50: " << heap.contains(50) << endl;
    cout << "Contains 101: " << heap.contains(101) << endl;
    cout << "Contains 0: " << heap.contains(0) << endl;

    // Check remove
    heap.remove(50);
    cout << "Remove 50: " << heap.toString() << endl;
    heap.remove(101);
    cout << "Remove 101: " << heap.toString() << endl;
    heap.remove(0);
    cout << "Remove 0: " << heap.toString() << endl;

    delete[] array;
}

void test4() {
    std::default_random_engine engine(static_cast<long unsigned int>(time(0)));
    uniform_int_distribution<int> dist(0, 100);
    int size = 100;
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = dist(engine);
    }

    // Create heap from array
    Heap<int> heap;
    for (int i = 0; i < size; i++) {
        heap.push(array[i]);
    }

    // Sort array
    for (int i = 0; i < size; i++) {
        array[i] = heap.pop();
    }

    // Check sort
    bool sorted = true;
    for (int i = 1; i < size; i++) {
        if (array[i] < array[i - 1]) {
            sorted = false;
            break;
        }
    }
    if (sorted) {
        cout << "Sort: OK" << endl;
    } else {
        cout << "Sort: Fail" << endl;
    }

    delete[] array;
}

// test with pointer type

// compare two value of pointer
int compareToMaxHeap(int*& lhs, int*& rhs){
    if (*lhs > *rhs) return -1;
    else if (*lhs < *rhs) return +1;
    else return 0;
}

string printInt(int*& value){
    stringstream os;
    os << *value;
    return os.str();
}

void test5() {
    // test with int*
    int size = 10;
    int** array = new int*[size];
    for (int i = 0; i < size; i++) {
        array[i] = new int(size - i);
    }

    // Create heap from array
    Heap<int*> heap(&compareToMaxHeap, &Heap<int*>::free);
    for (int i = 0; i < size; i++) {
        heap.push(array[i]);
        cout << "Heapify: " << heap.toString(printInt) << endl;
    }

    // Check push
    int* value = new int(0);
    heap.push(value);
    cout << "Push 0: " << heap.toString(printInt) << endl;
    value = new int(5);
    heap.push(value);
    cout << "Push 5: " << heap.toString(printInt) << endl;
    value = new int(10);
    heap.push(value);
    cout << "Push 10: " << heap.toString(printInt) << endl;

    // Check pop
    value = heap.pop();
    cout << "Pop: " << heap.toString(printInt) << endl;
    delete value;
    value = heap.pop();
    cout << "Pop: " << heap.toString(printInt) << endl;
    delete value;
    value = heap.pop();
    cout << "Pop: " << heap.toString(printInt) << endl;
    delete value;

    // Size
    cout << "Size: " << heap.size() << endl;
    // Empty
    cout << "Empty: " << heap.empty() << endl;
    // Clear
    heap.clear();
    cout << "Clear: " << heap.toString(printInt) << endl;
    cout << "Empty: " << heap.empty() << endl;

    delete[] array;

    // Push again
    for (int i = 0; i < size; i++) {
        heap.push(new int(size - i));
    }

    // Check remove
    int* value1 = new int(5);
    int* value2 = new int(10);
    int* value3 = new int(0);
    heap.remove(value1);
    cout << "Removed: " << heap.toString(printInt) << endl;

    // Check contains
    cout << "Containss 5: " << heap.contains(value1) << endl;
    cout << "Contains 10: " << heap.contains(value2) << endl;
    cout << "Contains 0: " << heap.contains(value3) << endl;

    delete value1;
    delete value2;
    delete value3;

}

void test6() {
    Heap<Point*> heap(&myPointComparatorMAX, &Heap<Point*>::free);
    heap.push(new Point(18.2, 0));
    heap.push(new Point(0, 0));
    heap.push(new Point(0, 18.2));
    heap.push(new Point(18.2, 18.2));
    heap.push(new Point(9.1, 9.1));
    heap.push(new Point(9.1, 0));
    heap.push(new Point(0, 9.1));
    heap.push(new Point(18.2, 9.1));
    heap.push(new Point(9.1, 18.2));

    heap.println(&myPoint2Str);
    
    for(Heap<Point*>::Iterator it = heap.begin(); it != heap.end(); it++){
        Point* point = *it;
        cout << point->radius() << ", ";
    }
    cout << endl;

    // Test remove
    Point* point = new Point(9.1, 9.1);
    heap.remove(point);
    heap.println(&myPoint2Str);
    delete point;

    // Test clear
    heap.clear();
    heap.println(&myPoint2Str);

    // Test empty
    cout << "Empty: " << heap.empty() << endl;
    cout << endl;
}


void runDemo() {

    cout << "Demo: anotherHeap" << endl;
    try {
        anotherHeap();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 0: simpleHeap" << endl;
    try {
        simpleMinHeap();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 1: heapDemo1" << endl;
    try {
        heapDemo1();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 2: heapDemo2" << endl;
    try {
        heapDemo2();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 3: heapDemo3" << endl;
    try {
        heapDemo3();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }
}

// pointer function to store 15 test
void (*testFuncs[])() = {
    test1, test2, test3, test4, test5, test6
};

// ! NOTES: in function removeItem from original source
// * Param removeItemData in removeItem (line 40) 
// *             doesn't use pass by reference
// *             => can't free pointer 
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
        else if (string(argv[1]) == "test")
        {
            string folder = "TestLog/Heap";
            string path = "HeapTestLog_NhanOutput.txt";
            string output = "HeapTestLog_YourOutput.txt";
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